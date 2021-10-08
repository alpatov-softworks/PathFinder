#include "Hooks.h"
#include "../SDK/EngineBase.h"


ImVec3 WorldToScreen(ImVec3 pos)
{
	auto dwViewmatrix = *reinterpret_cast<viewmatrix*>(0x606D813C);
	ImVec2 window_size(800, 600);

	float _x = dwViewmatrix[0][0] * pos.x + dwViewmatrix[0][1] * pos.y + dwViewmatrix[0][2] * pos.z + dwViewmatrix[0][3];
	float _y = dwViewmatrix[1][0] * pos.x + dwViewmatrix[1][1] * pos.y + dwViewmatrix[1][2] * pos.z + dwViewmatrix[1][3];
	float _z = dwViewmatrix[2][0] * pos.x + dwViewmatrix[2][1] * pos.y + dwViewmatrix[2][2] * pos.z + dwViewmatrix[2][3];
	float w = dwViewmatrix[3][0] * pos.x + dwViewmatrix[3][1] * pos.y + dwViewmatrix[3][2] * pos.z + dwViewmatrix[3][3];


	float x = (window_size.x / 2 * (_x / w)) + ((_x / w) + window_size.x / 2);

	float y = -(window_size.y / 2 * (_y / w)) + ((_y / w) + window_size.y / 2);

	return ImVec3(x, y, w);
}

bool init = false;
long __stdcall hooks::hkEndScene(LPDIRECT3DDEVICE9 p_device)
{
	if (!init)
	{
		ImGui::CreateContext();

		ImGui_ImplWin32_Init(FindWindow(NULL, "Team Fortress 2"));
		ImGui_ImplDX9_Init(p_device);
		init = true;

	}

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (positions.size() > 2)
	{
		for (int i = 0; i < positions.size() - 1; i++)
		{
			ImVec3 pos1, pos2;
			pos1 = WorldToScreen(positions[i]);
			pos2 = WorldToScreen(positions[i + 1]);

			if (pos1.z > 0 and pos2.z > 0)
				ImGui::GetBackgroundDrawList()->AddLine(pos1, pos2, ImColor(255, 255, 255));
		}
	}

	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

	typedef long(__stdcall* EndScene)(LPDIRECT3DDEVICE9);
	return reinterpret_cast<EndScene>(hooks::oEndScene)(p_device);
}