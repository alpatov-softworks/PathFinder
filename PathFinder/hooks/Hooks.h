#pragma once
#include "../imgui/imgui.h"
#include <Windows.h>
#include <d3d9.h>
#include "../imgui/imgui_impl_dx9.h"
#include "../imgui/imgui_impl_win32.h"
#include <vector>
#include "../Utils/ImVec3.h"
#include "../SDK/EngineBase.h"
#include "../SDK/ClientBase.h"
#include <string>
#include <format>
#include "../DirectX9/CustomDirectX9Device.h"
#include "../Utils/memory.h"
#include "MinHook.h"
#include "../Utils/xorstr.h"
#include <fstream>
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace hooks
{

	ImVec3 GetClosestPoint(ImVec3 current_position, ImVec3 dest, std::vector<ImVec3>& positions);
	void Initialize();
	void DetchHooks();
	inline DWORD oPresent = NULL;
	inline DWORD oWndProc = NULL;
	inline std::vector<ImVec3> positions;
	int __stdcall hkPresent(CustomDirect3D9Device* pDevice, int a2, int a3, int a4, int a5);
	LRESULT WINAPI WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
}