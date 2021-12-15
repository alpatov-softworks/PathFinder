#pragma once
#include <d3d9.h>
#include "../imgui/imgui.h"
#include <d3dx9core.h>
#include "../SDK/EngineBase.h"


class CustomDirect3D9Device : public IDirect3DDevice9
{
public:
	ImVec2 GetWindowSize();

	ImVec3 WorldToScreen(ImVec3 pos);
};