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

namespace hooks
{

	ImVec3 GetClosestPoint(ImVec3 current_position, ImVec3 dest, std::vector<ImVec3>& positions);

	inline DWORD oEndScene = NULL;
	inline std::vector<ImVec3> positions;
	long __stdcall hkEndScene(LPDIRECT3DDEVICE9 p_device);
}