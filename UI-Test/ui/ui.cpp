#include "pch.h"

//Nuklear

nk_context* g_pNkContext;


// DX11 imports

#pragma comment(lib, "D3dcompiler.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "winmm.lib")
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }


// D3X HOOK DEFINITIONS
typedef HRESULT(__fastcall* IDXGISwapChainPresent)(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
typedef void(__stdcall* ID3D11DrawIndexed)(ID3D11DeviceContext* pContext, UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation);
// Definition of WndProc Hook. Its here to avoid dragging dependencies on <windows.h> types.
//extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


// Main D3D11 Objects
ID3D11DeviceContext* pContext = NULL;
ID3D11Device* pDevice = NULL;
ID3D11RenderTargetView* mainRenderTargetView;
static IDXGISwapChain* pSwapChain = NULL;
static WNDPROC OriginalWndProcHandler = nullptr;
HWND window = nullptr;
IDXGISwapChainPresent fnIDXGISwapChainPresent;
DWORD_PTR* pDeviceContextVTable = NULL;
ID3D11DrawIndexed fnID3D11DrawIndexed;
UINT iIndexCount = 0;
UINT iStartIndexLocation;
INT iBaseVertexLocation;

// Boolean
bool ui::hooking::g_bInitialised = false;
bool g_ShowMenu = false;
bool bDrawIndexed = true;
bool bCurrent;
bool ui::hooking::g_PresentHooked = false;


int g_iWindowHeight;

int g_iWindowWidth;



HRESULT GetDeviceAndg_pNkContextFromSwapchain(IDXGISwapChain* pSwapChain, ID3D11Device** ppDevice, ID3D11DeviceContext** ppContext)
{
	HRESULT ret = pSwapChain->GetDevice(__uuidof(ID3D11Device), (PVOID*)ppDevice);

	if (SUCCEEDED(ret))
		(*ppDevice)->GetImmediateContext(ppContext);

	return ret;
}

void GUIResize(void)
{
	ID3D11Texture2D* back_buffer;
	D3D11_RENDER_TARGET_VIEW_DESC desc;
	HRESULT hr;

	if (mainRenderTargetView)
		mainRenderTargetView->Release();

	pContext->OMSetRenderTargets(0, NULL, NULL);
	hr = pSwapChain->ResizeBuffers(0, g_iWindowWidth, g_iWindowHeight, DXGI_FORMAT_UNKNOWN, 0);

	//Wrapper::Memset(&desc, 0, sizeof(desc));
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

	hr = pSwapChain->GetBuffer(0, __uuidof(back_buffer), (void**)&back_buffer);
	hr = pDevice->CreateRenderTargetView((ID3D11Resource*)back_buffer, &desc, &mainRenderTargetView);
	back_buffer->Release();

	nk_d3d11_resize(pContext, g_iWindowWidth, g_iWindowHeight);
}

LRESULT CALLBACK hWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//ImGuiIO& io = ImGui::GetIO();
	//POINT mPos;
	//GetCursorPos(&mPos);
	//ScreenToClient(window, &mPos);
	//ImGui::GetIO().MousePos.x = mPos.x;
	//ImGui::GetIO().MousePos.y = mPos.y;

	// Do Window Validation
	RECT WindowRect;

	if (GetClientRect(hWnd, &WindowRect))
	{
		if (g_iWindowHeight == 0)
			g_iWindowHeight = WindowRect.bottom - WindowRect.top;

		if (g_iWindowWidth == 0)
			g_iWindowWidth = WindowRect.right - WindowRect.left;

		int CurrentWidth = WindowRect.right - WindowRect.left;
		int CurrentHeight = WindowRect.bottom - WindowRect.top;

		if (g_iWindowHeight != CurrentHeight || g_iWindowWidth != CurrentWidth)
			GUIResize();
	}

	if (uMsg == WM_KEYUP)
	{
		if (wParam == VK_INSERT)
		{
			g_ShowMenu = !g_ShowMenu;
		}

	}

	if (g_ShowMenu)
	{
		//ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
		return true;
	}

	return CallWindowProc(OriginalWndProcHandler, hWnd, uMsg, wParam, lParam);
}

void Start(void)
{
	// This is my custom crap, I suggest to look at the nuklear file and do a window hook.
	nk_input_begin(g_pNkContext);
	//InputHandler();
	nk_input_end(g_pNkContext);
}

void End(void)
{
	pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
	nk_d3d11_render(pContext, NK_ANTI_ALIASING_ON);
}

void MenuBackend(void)
{
	if (nk_begin(g_pNkContext, "Demo", nk_rect(50, 50, 230, 250),
		NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE |
		NK_WINDOW_MINIMIZABLE | NK_WINDOW_TITLE))
	{
		enum { EASY, HARD };
		static int op = EASY;
		static int property = 20;

		nk_layout_row_static(g_pNkContext, 30, 80, 1);
		if (nk_button_label(g_pNkContext, "button"))
			fprintf(stdout, "button pressed\n");
		nk_layout_row_dynamic(g_pNkContext, 30, 2);
		if (nk_option_label(g_pNkContext, "easy", op == EASY)) op = EASY;
		if (nk_option_label(g_pNkContext, "hard", op == HARD)) op = HARD;
		nk_layout_row_dynamic(g_pNkContext, 22, 1);
		nk_property_int(g_pNkContext, "Compression:", 0, &property, 100, 10, 1);

		nk_layout_row_dynamic(g_pNkContext, 20, 1);
		nk_label(g_pNkContext, "background:", NK_TEXT_LEFT);
		nk_layout_row_dynamic(g_pNkContext, 25, 1);
	}
	nk_end(g_pNkContext);
}

HRESULT __fastcall Present(IDXGISwapChain* pChain, UINT SyncInterval, UINT Flags)
{
	if (!ui::hooking::g_bInitialised) {
		if (FAILED(GetDeviceAndg_pNkContextFromSwapchain(pChain, &pDevice, &pContext)))
			return fnIDXGISwapChainPresent(pChain, SyncInterval, Flags);
		pSwapChain = pChain;
		DXGI_SWAP_CHAIN_DESC sd;
		pChain->GetDesc(&sd);
		
		//ImGui::CreateContext();
		//ImGuiIO& io = ImGui::GetIO(); (void)io;
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		
		window = sd.OutputWindow;

		//Set OriginalWndProcHandler to the Address of the Original WndProc function
		OriginalWndProcHandler = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)hWndProc);

		//ImGui_ImplWin32_Init(window);
		//ImGui_ImplDX11_Init(pDevice, pContext);
		//ImGui::GetIO().ImeWindowHandle = window;

		ID3D11Texture2D* pBackBuffer;

		pChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
		pDevice->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
		pBackBuffer->Release();

		// Initialize Nuklear
		g_pNkContext = nk_d3d11_init(pDevice, g_iWindowWidth, g_iWindowHeight, MAX_VERTEX_BUFFER, MAX_INDEX_BUFFER);

		nk_font_atlas* pNkAtlas;
		nk_d3d11_font_stash_begin(&pNkAtlas);
		nk_d3d11_font_stash_end();
		
		ui::hooking::g_bInitialised = true;
	}
	//ImGui_ImplWin32_NewFrame();
	//ImGui_ImplDX11_NewFrame();

	//ImGui::NewFrame();
	//
	//Menu is displayed when g_ShowMenu is TRUE
	if (g_ShowMenu)
	{
		bool bShow = true;
		//ImGui::ShowDemoWindow(&bShow);
	}
	
	//ImGui::EndFrame();

	//ImGui::Render();

	pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
	
	//ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	return fnIDXGISwapChainPresent(pChain, SyncInterval, Flags);
}

void ui::hooking::detourDirectXPresent()
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	// Detours the original fnIDXGISwapChainPresent with our Present
	DetourAttach(&(LPVOID&)fnIDXGISwapChainPresent, (PBYTE)Present);
	DetourTransactionCommit();
}

void ui::hooking::retrieveValues()
{
	DWORD_PTR hDxgi = (DWORD_PTR)GetModuleHandle(L"dxgi.dll");

	fnIDXGISwapChainPresent = (IDXGISwapChainPresent)((DWORD_PTR)hDxgi + 0x5070);
}

LRESULT CALLBACK DXGIMsgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) { return DefWindowProc(hwnd, uMsg, wParam, lParam); }

void  ui::hooking::GetPresent()
{
	WNDCLASSEXA wc = { sizeof(WNDCLASSEX), CS_CLASSDC, DXGIMsgProc, 0L, 0L, GetModuleHandleA(NULL), NULL, NULL, NULL, NULL, "DX", NULL };
	RegisterClassExA(&wc);
	HWND hWnd = CreateWindowA("DX", NULL, WS_OVERLAPPEDWINDOW, 100, 100, 300, 300, NULL, NULL, wc.hInstance, NULL);

	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = 2;
	sd.BufferDesc.Height = 2;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	D3D_FEATURE_LEVEL FeatureLevelsRequested = D3D_FEATURE_LEVEL_11_0;
	UINT numFeatureLevelsRequested = 1;
	D3D_FEATURE_LEVEL FeatureLevelsSupported;
	HRESULT hr;
	IDXGISwapChain* swapchain = 0;
	ID3D11Device* dev = 0;
	ID3D11DeviceContext* devcon = 0;
	if (FAILED(hr = D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		0,
		&FeatureLevelsRequested,
		numFeatureLevelsRequested,
		D3D11_SDK_VERSION,
		&sd,
		&swapchain,
		&dev,
		&FeatureLevelsSupported,
		&devcon)))
	{
		return;
	}
	DWORD_PTR* pSwapChainVtable = NULL;
	pSwapChainVtable = (DWORD_PTR*)swapchain;
	pSwapChainVtable = (DWORD_PTR*)pSwapChainVtable[0];
	fnIDXGISwapChainPresent = (IDXGISwapChainPresent)(DWORD_PTR)pSwapChainVtable[8];
	g_PresentHooked = true;
	Sleep(2000);
}

void ui::hooking::UnhookUI()
{
	DetourDetach(&(LPVOID&)fnIDXGISwapChainPresent, (PBYTE)Present);
}

//BOOL APIENTRY DllMain(HMODULE hModule,
//	DWORD  ul_reason_for_call,
//	LPVOID lpReserved
//)
//{
//	switch (ul_reason_for_call)
//	{
//	case DLL_PROCESS_ATTACH:
//	{
//		DisableThreadLibraryCalls(hModule);
//		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)main, NULL, NULL, NULL);
//	}
//	case DLL_THREAD_ATTACH:
//	case DLL_THREAD_DETACH:
//	case DLL_PROCESS_DETACH:
//		break;
//	}
//	return TRUE;
//}
