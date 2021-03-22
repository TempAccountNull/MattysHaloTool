#include "pch.h"
#include "ui.h"
#include "../utils.h"
#include "../games/halo-reach/hooks.h"

// DX11 imports
#pragma comment(lib, "D3dcompiler.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "winmm.lib")
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }

//ImGUI imports
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>

// D3X HOOK DEFINITIONS// D3X HOOK DEFINITIONS
typedef HRESULT(__fastcall* IDXGISwapChainPresent)(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
typedef void(__stdcall* ID3D11DrawIndexed)(ID3D11DeviceContext* pContext, UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation);
// Definition of WndProc Hook. Its here to avoid dragging dependencies on <windows.h> types.
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

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
bool ui::hooking::g_PresentHooked = false;
bool show_wireframe = false;

LRESULT CALLBACK hWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	ImGuiIO& io = ImGui::GetIO();
	POINT mPos;
	GetCursorPos(&mPos);
	ScreenToClient(window, &mPos);
	ImGui::GetIO().MousePos.x = mPos.x;
	ImGui::GetIO().MousePos.y = mPos.y;

	if (uMsg == WM_KEYUP)
	{
		if (wParam == VK_INSERT)
		{
			g_ShowMenu = !g_ShowMenu;
		}
	}

	if (g_ShowMenu)
	{
		ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
		return true;
	}

	return CallWindowProc(OriginalWndProcHandler, hWnd, uMsg, wParam, lParam);
}

void WireframeMode(bool show)
{
	ID3D11RasterizerState* rState;
	D3D11_RASTERIZER_DESC rDesc;

	// cd3d is the ID3D11DeviceContext
	pContext->RSGetState(&rState); // retrieve the current state
	if (rState != nullptr)
	{
		rState->GetDesc(&rDesc);    // get the desc of the state
		if (show)
		{
			rDesc.FillMode = D3D11_FILL_WIREFRAME; // change the ONE setting
		}
		else
		{
			rDesc.FillMode = D3D11_FILL_SOLID; // change the ONE setting
		}
		// create a whole new rasterizer state
		// d3d is the ID3D11Device
		pDevice->CreateRasterizerState(&rDesc, &rState);

		pContext->RSSetState(rState);    // set the new rasterizer state
	}
}

void __stdcall hookD3D11DrawIndexed(ID3D11DeviceContext* pContext, UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation)
{
	if (show_wireframe)
	{
		WireframeMode(true);
	}
	else
	{
		WireframeMode(false);
	}

	return fnID3D11DrawIndexed(pContext, IndexCount, StartIndexLocation, BaseVertexLocation);
}

HRESULT GetDeviceAndCtxFromSwapchain(IDXGISwapChain* pSwapChain, ID3D11Device** ppDevice, ID3D11DeviceContext** ppContext)
{
	HRESULT ret = pSwapChain->GetDevice(__uuidof(ID3D11Device), (PVOID*)ppDevice);

	if (SUCCEEDED(ret))
		(*ppDevice)->GetImmediateContext(ppContext);

	return ret;
}

static void HelpMarker(const char* desc)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

void Main_Menu()
{
	ImGui::Begin("Matthew's Halo Tool!", &g_ShowMenu, ImGuiWindowFlags_MenuBar);
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Menu"))
		{
			if (ImGui::MenuItem("Settings")) { /* Open Settings Window WIP */ }
			if (ImGui::MenuItem("Detach DLL")) { /*utils::DLL_Management::Kill_DLL();*/ }
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_NoCloseWithMiddleMouseButton | ImGuiTabBarFlags_Reorderable;
	if (ImGui::BeginTabBar("MainTabs", tab_bar_flags))
	{
		if (ImGui::BeginTabItem("Halo Reach"))
		{
			ImGui::Checkbox("Infinite Magazines", &haloreach::hooks::infinite_mags);
			//ImGui::Checkbox("Player projectiles only.", nullptr);
			ImGui::Checkbox("AI go crazy", &haloreach::hooks::ai_go_crazy);
			//ImGui::Checkbox("AI Perception", nullptr);

			//ImGui::Checkbox("Medusa", nullptr);
			//ImGui::SameLine();
			//HelpMarker("This kills any ai that gets looked at by the player.");

			//ImGui::InputFloat("Game Speed",nullptr);

			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Universal"))
		{
			ImGui::Checkbox("Wireframe Mode", &show_wireframe);
			ImGui::SameLine();
			HelpMarker("This is quite crashy. Do not use the pause menu or toggle in-game!");
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
	ImGui::End();
}

HRESULT __fastcall Present(IDXGISwapChain* pChain, UINT SyncInterval, UINT Flags)
{
	if (!ui::hooking::g_bInitialised) {
		if (FAILED(GetDeviceAndCtxFromSwapchain(pChain, &pDevice, &pContext)))
			return fnIDXGISwapChainPresent(pChain, SyncInterval, Flags);
		pSwapChain = pChain;
		DXGI_SWAP_CHAIN_DESC sd;
		pChain->GetDesc(&sd);

		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		// Not needed right now.

		// Style
		ImGuiStyle& style = ImGui::GetStyle();
		style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
		style.WindowMenuButtonPosition = ImGuiDir_None;

		ImVec4* colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.00f, 0.00f, 0.94f);
		colors[ImGuiCol_PopupBg] = ImVec4(0.15f, 0.00f, 0.00f, 0.94f);
		colors[ImGuiCol_Border] = ImVec4(0.34f, 0.00f, 0.00f, 0.50f);
		colors[ImGuiCol_FrameBg] = ImVec4(0.19f, 0.00f, 0.00f, 0.54f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(1.00f, 0.00f, 0.00f, 0.40f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(1.00f, 0.00f, 0.00f, 0.67f);
		colors[ImGuiCol_TitleBg] = ImVec4(0.55f, 0.00f, 0.00f, 1.00f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.67f, 0.00f, 0.00f, 1.00f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.42f, 0.00f, 0.00f, 0.51f);
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.34f, 0.00f, 0.00f, 1.00f);
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.18f, 0.00f, 0.00f, 0.53f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.44f, 0.00f, 0.00f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.66f, 0.00f, 0.00f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
		colors[ImGuiCol_CheckMark] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
		colors[ImGuiCol_SliderGrab] = ImVec4(0.88f, 0.24f, 0.24f, 1.00f);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.98f, 0.26f, 0.26f, 1.00f);
		colors[ImGuiCol_Button] = ImVec4(0.98f, 0.26f, 0.26f, 0.40f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.98f, 0.26f, 0.26f, 1.00f);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.98f, 0.06f, 0.06f, 1.00f);
		colors[ImGuiCol_Header] = ImVec4(0.98f, 0.26f, 0.26f, 0.31f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.98f, 0.26f, 0.26f, 0.80f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.98f, 0.26f, 0.26f, 1.00f);
		colors[ImGuiCol_Separator] = ImVec4(0.50f, 0.43f, 0.43f, 0.50f);
		colors[ImGuiCol_SeparatorHovered] = ImVec4(0.75f, 0.10f, 0.10f, 0.78f);
		colors[ImGuiCol_SeparatorActive] = ImVec4(0.75f, 0.10f, 0.10f, 1.00f);
		colors[ImGuiCol_ResizeGrip] = ImVec4(0.98f, 0.26f, 0.26f, 0.20f);
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.98f, 0.26f, 0.26f, 0.67f);
		colors[ImGuiCol_ResizeGripActive] = ImVec4(0.98f, 0.26f, 0.26f, 0.95f);
		colors[ImGuiCol_Tab] = ImVec4(0.58f, 0.18f, 0.18f, 0.86f);
		colors[ImGuiCol_TabHovered] = ImVec4(0.98f, 0.26f, 0.26f, 0.80f);
		colors[ImGuiCol_TabActive] = ImVec4(0.68f, 0.20f, 0.20f, 1.00f);
		colors[ImGuiCol_TabUnfocused] = ImVec4(0.15f, 0.07f, 0.07f, 0.97f);
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.42f, 0.14f, 0.14f, 1.00f);
		colors[ImGuiCol_TableHeaderBg] = ImVec4(0.20f, 0.19f, 0.19f, 1.00f);
		colors[ImGuiCol_TableBorderStrong] = ImVec4(0.35f, 0.31f, 0.31f, 1.00f);
		colors[ImGuiCol_TableBorderLight] = ImVec4(0.25f, 0.23f, 0.23f, 1.00f);
		colors[ImGuiCol_TextSelectedBg] = ImVec4(0.98f, 0.26f, 0.26f, 0.35f);
		colors[ImGuiCol_NavHighlight] = ImVec4(0.98f, 0.26f, 0.26f, 1.00f);

		// Get path of dll and save imgui config there.
		std::string ImguiConfigPath = utils::locations::GetCurrentWorkingDir();
		ImguiConfigPath.append("\\ui.ini");
		io.IniFilename = ImguiConfigPath.c_str();

		window = sd.OutputWindow;

		//Set OriginalWndProcHandler to the Address of the Original WndProc function
		OriginalWndProcHandler = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)hWndProc);

		ImGui_ImplWin32_Init(window);
		ImGui_ImplDX11_Init(pDevice, pContext);
		ImGui::GetIO().ImeWindowHandle = window;

		ImGui_ImplDX11_InvalidateDeviceObjects();

		WireframeMode(true);

		ID3D11Texture2D* pBackBuffer;

		pChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
		pDevice->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
		pBackBuffer->Release();

		ui::hooking::g_bInitialised = true;
	}

	ImGui_ImplWin32_NewFrame();
	ImGui_ImplDX11_NewFrame();

	ImGui::NewFrame();
	//
	//Menu is displayed when g_ShowMenu is TRUE
	if (g_ShowMenu)
	{
		Main_Menu();
	}

	ImGui::EndFrame();

	ImGui::Render();

	pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);

	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

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

void ui::hooking::detourDirectXDrawIndexed()
{
	// Get values
	pDeviceContextVTable = (DWORD_PTR*)pContext;
	pDeviceContextVTable = (DWORD_PTR*)pDeviceContextVTable[0];
	//fnID3D11DrawIndexed
	fnID3D11DrawIndexed = (ID3D11DrawIndexed)pDeviceContextVTable[12];

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	// Detours the original fnIDXGISwapChainPresent with our Present fnID3D11DrawIndexed, (PBYTE)hookD3D11DrawIndexed
	DetourAttach(&(LPVOID&)fnID3D11DrawIndexed, (PBYTE)hookD3D11DrawIndexed);
	DetourTransactionCommit();
}

void ui::hooking::retrieveValues()
{
	DWORD_PTR hDxgi = (DWORD_PTR)GetModuleHandle("dxgi.dll");

	fnIDXGISwapChainPresent = (IDXGISwapChainPresent)((DWORD_PTR)hDxgi + 0x4670);
}

LRESULT CALLBACK DXGIMsgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) { return DefWindowProc(hwnd, uMsg, wParam, lParam); }

void ui::hooking::UnhookUI()
{
	ImGui_ImplWin32_Shutdown();
	ImGui_ImplDX11_Shutdown();
	ImGui::DestroyContext();

	DetourDetach(&(LPVOID&)fnIDXGISwapChainPresent, (PBYTE)Present);
	DetourDetach(&(LPVOID&)fnID3D11DrawIndexed, (PBYTE)hookD3D11DrawIndexed);
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