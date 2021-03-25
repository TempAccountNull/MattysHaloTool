#include "pch.h"
#include "ui.h"
#include "../utils.h"
#include "../games/halo-reach/hooks.h"

// DX11 imports
#pragma comment(lib, "D3dcompiler.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "winmm.lib")
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }  // NOLINT(cppcoreguidelines-macro-usage)

//ImGUI imports
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>

// D3X HOOK DEFINITIONS// D3X HOOK DEFINITIONS
typedef HRESULT(__fastcall* IDXGISwapChainPresent)(IDXGISwapChain* p_swap_chain, UINT sync_interval, UINT flags);
typedef void(__stdcall* ID3D11DrawIndexed)(ID3D11DeviceContext* p_context, UINT index_count, UINT start_index_location, INT base_vertex_location);
// Definition of WndProc Hook. Its here to avoid dragging dependencies on <windows.h> types.
// ReSharper disable once CppInconsistentNaming
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Main D3D11 Objects
ID3D11DeviceContext* p_context = nullptr;
ID3D11Device* p_device = nullptr;
ID3D11RenderTargetView* main_render_target_view;
static IDXGISwapChain* p_swap_chain = nullptr;
static WNDPROC original_wnd_proc_handler = nullptr;
HWND window = nullptr;
IDXGISwapChainPresent fn_idxgi_swap_chain_present;
DWORD_PTR* p_device_context_v_table = nullptr;
ID3D11DrawIndexed fn_id3d_11draw_indexed;
UINT i_index_count = 0;
UINT i_start_index_location;
INT i_base_vertex_location;

// Boolean
bool ui::hooking::g_b_initialized = false;
bool g_show_menu = false;
bool ui::hooking::g_present_hooked = false;
bool show_wireframe = false;

LRESULT CALLBACK h_wnd_proc(const HWND h_wnd, const UINT u_msg, const WPARAM w_param, const LPARAM l_param)
{
	ImGuiIO& io = ImGui::GetIO();
	POINT m_pos;
	GetCursorPos(&m_pos);
	ScreenToClient(window, &m_pos);
	ImGui::GetIO().MousePos.x = m_pos.x;
	ImGui::GetIO().MousePos.y = m_pos.y;

	if (u_msg == WM_KEYUP)
	{
		if (w_param == VK_INSERT)
		{
			g_show_menu = !g_show_menu;
		}
	}

	//TODO: Fix mouse input being passed into mcc.
	io.MouseDrawCursor = false;

	if (g_show_menu)
	{
		ImGui_ImplWin32_WndProcHandler(h_wnd, u_msg, w_param, l_param);
		io.MouseDrawCursor = true;
		return true;
	}

	return CallWindowProc(original_wnd_proc_handler, h_wnd, u_msg, w_param, l_param);
}

void wireframe_mode(const bool show)
{
	ID3D11RasterizerState* r_state;
	D3D11_RASTERIZER_DESC r_desc;

	// cd3d is the ID3D11DeviceContext
	p_context->RSGetState(&r_state); // retrieve the current state
	if (r_state != nullptr)
	{
		r_state->GetDesc(&r_desc);    // get the desc of the state
		if (show)
		{
			r_desc.FillMode = D3D11_FILL_WIREFRAME; // change the ONE setting
		}
		else
		{
			r_desc.FillMode = D3D11_FILL_SOLID; // change the ONE setting
		}
		// create a whole new rasterizer state
		// d3d is the ID3D11Device
		p_device->CreateRasterizerState(&r_desc, &r_state);

		p_context->RSSetState(r_state);    // set the new rasterizer state
	}
}

void __stdcall hook_d3d11_draw_indexed(ID3D11DeviceContext* p_context, const UINT index_count, const UINT start_index_location, const INT base_vertex_location)
{
	//TODO: Make wireframe mode not affect the ui.
	if (show_wireframe)
	{
		wireframe_mode(true);
	}
	else
	{
		wireframe_mode(false);
	}

	return fn_id3d_11draw_indexed(p_context, index_count, start_index_location, base_vertex_location);
}

HRESULT get_device_and_ctx_from_swapchain(IDXGISwapChain* p_swap_chain, ID3D11Device** pp_device, ID3D11DeviceContext** pp_context)
{
	const HRESULT ret = p_swap_chain->GetDevice(__uuidof(ID3D11Device), reinterpret_cast<PVOID*>(pp_device));

	if (SUCCEEDED(ret))
		(*pp_device)->GetImmediateContext(pp_context);

	return ret;
}

static void help_marker(const char* desc)
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

float game_speed = 1.0f;

void main_menu()
{
	ImGui::Begin("Matthew's Halo Tool!", &g_show_menu, ImGuiWindowFlags_MenuBar);
	//if (ImGui::BeginMenuBar())
	//{
	//	if (ImGui::BeginMenu("Menu"))
	//	{
	//		//if (ImGui::MenuItem("Settings")) { /* Open Settings Window WIP */ }
	//		if (ImGui::MenuItem("Detach DLL")) { utils::dll_management::kill_dll(); }
	//		ImGui::EndMenu();
	//	}
	//	ImGui::EndMenuBar();
	//}

	const ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_NoCloseWithMiddleMouseButton | ImGuiTabBarFlags_Reorderable;
	if (ImGui::BeginTabBar("MainTabs", tab_bar_flags))
	{
		if (ImGui::BeginTabItem("Halo Reach"))
		{
			ImGui::Checkbox("Infinite Ammo", &haloreach::hooks::infinite_ammo);
			ImGui::SameLine();
			help_marker("Behaves the same as the infinite ammo multi-player setting.");

			ImGui::Checkbox("No Weapon Overheat", &haloreach::hooks::no_overheat);

			ImGui::Checkbox("Player projectiles only.", &haloreach::hooks::player_proj_only);
			ImGui::SameLine();
			help_marker("This causes any weapon that does not belong to the player to not fire any projectiles.\nWorks with vehicles.");

			ImGui::Checkbox("AI go crazy", &haloreach::hooks::ai_go_crazy);
			ImGui::SameLine();
			help_marker("Makes all units that are not in a vehicle run around like mad, players not included.");

			ImGui::Checkbox("AI No Perception", &haloreach::hooks::ai_null_perception);
			ImGui::SameLine();
			help_marker("Makes actor_perception_set_target return void meaning that the ai will not be able to notice much.");

			if (ImGui::InputFloat("Game Speed", &game_speed))
			{
				haloreach::hooks::game_speed = game_speed;
			}
			ImGui::SameLine();
			help_marker("1.0 is default value. Do not go below 0.1!");

			//ImGui::Checkbox("Medusa", nullptr);
			//ImGui::SameLine();
			//HelpMarker("This kills any ai that gets looked at by the player.");

			if (ImGui::Button("Reinit Hooks"))
			{
				haloreach::hooks::reinit_hooks();
			}
			ImGui::SameLine();
			help_marker("If things seem to be broken, press this button!");

			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Universal"))
		{
			ImGui::Checkbox("Wireframe Mode", &show_wireframe);
			ImGui::SameLine();
			help_marker("This is quite crashy. Do not use the pause menu or toggle in-game!");
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
	ImGui::End();
}

HRESULT __fastcall Present(IDXGISwapChain* p_chain, UINT sync_interval, UINT flags)
{
	if (!ui::hooking::g_b_initialized) {
		if (FAILED(get_device_and_ctx_from_swapchain(p_chain, &p_device, &p_context)))
			return fn_idxgi_swap_chain_present(p_chain, sync_interval, flags);
		p_swap_chain = p_chain;
		DXGI_SWAP_CHAIN_DESC sd;
		p_chain->GetDesc(&sd);

		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_NavEnableSetMousePos | ImGuiConfigFlags_NavEnableGamepad;
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
		std::string imgui_config_path = utils::locations::get_current_working_dir();
		imgui_config_path.append("\\ui.ini");
		io.IniFilename = imgui_config_path.c_str();

		window = sd.OutputWindow;

		//Set OriginalWndProcHandler to the Address of the Original WndProc function
		original_wnd_proc_handler = reinterpret_cast<WNDPROC>(SetWindowLongPtr(window, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(h_wnd_proc)));

		ImGui_ImplWin32_Init(window);
		ImGui_ImplDX11_Init(p_device, p_context);
		ImGui::GetIO().ImeWindowHandle = window;

		ImGui_ImplDX11_InvalidateDeviceObjects();

		wireframe_mode(true);

		ID3D11Texture2D* p_back_buffer;

		p_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(&p_back_buffer));
		p_device->CreateRenderTargetView(p_back_buffer, nullptr, &main_render_target_view);
		p_back_buffer->Release();

		ui::hooking::g_b_initialized = true;
	}

	ImGui_ImplWin32_NewFrame();
	ImGui_ImplDX11_NewFrame();

	ImGui::NewFrame();
	//
	//Menu is displayed when g_ShowMenu is TRUE
	if (g_show_menu)
	{
		main_menu();
	}

	ImGui::EndFrame();

	ImGui::Render();

	p_context->OMSetRenderTargets(1, &main_render_target_view, nullptr);

	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	return fn_idxgi_swap_chain_present(p_chain, sync_interval, flags);
}

void ui::hooking::detour_directx_present()
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	// Detours the original fnIDXGISwapChainPresent with our Present
	DetourAttach(&reinterpret_cast<LPVOID&>(fn_idxgi_swap_chain_present), reinterpret_cast<PBYTE>(Present));
	DetourTransactionCommit();
}

void ui::hooking::detour_directx_draw_indexed()
{
	// Get values
	p_device_context_v_table = reinterpret_cast<DWORD_PTR*>(p_context);
	p_device_context_v_table = reinterpret_cast<DWORD_PTR*>(p_device_context_v_table[0]);
	//fnID3D11DrawIndexed
	fn_id3d_11draw_indexed = reinterpret_cast<ID3D11DrawIndexed>(p_device_context_v_table[12]);

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	// Detours the original fnIDXGISwapChainPresent with our Present fnID3D11DrawIndexed, (PBYTE)hookD3D11DrawIndexed
	DetourAttach(&reinterpret_cast<LPVOID&>(fn_id3d_11draw_indexed), reinterpret_cast<PBYTE>(hook_d3d11_draw_indexed));
	DetourTransactionCommit();
}

void ui::hooking::retrieve_values()
{
	const DWORD_PTR h_dxgi = reinterpret_cast<DWORD_PTR>(GetModuleHandle("dxgi.dll"));

	fn_idxgi_swap_chain_present = reinterpret_cast<IDXGISwapChainPresent>(static_cast<DWORD_PTR>(h_dxgi) + 0x4670);
}

LRESULT CALLBACK DXGIMsgProc(const HWND hwnd, const UINT uMsg, const WPARAM wParam, const LPARAM lParam) { return DefWindowProc(hwnd, uMsg, wParam, lParam); }

void ui::hooking::unhook_ui()
{
	ImGui_ImplWin32_Shutdown();
	ImGui_ImplDX11_Shutdown();
	ImGui::DestroyContext();

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourDetach(&reinterpret_cast<LPVOID&>(fn_idxgi_swap_chain_present), reinterpret_cast<PBYTE>(Present));
	DetourDetach(&reinterpret_cast<LPVOID&>(fn_id3d_11draw_indexed), reinterpret_cast<PBYTE>(hook_d3d11_draw_indexed));
	DetourTransactionCommit();
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