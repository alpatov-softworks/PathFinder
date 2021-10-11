#include "Hooks.h"


ImVec3 WorldToScreen(ImVec3 pos)
{
	auto matrix_prt = *(DWORD*)((DWORD)GetModuleHandle("engine.dll") + 0x594F4C);
	auto dwViewmatrix = *reinterpret_cast<viewmatrix*>(matrix_prt + 0x2d4);
	ImVec2 window_size(1920, 1080);

	float _x = dwViewmatrix[0][0] * pos.x + dwViewmatrix[0][1] * pos.y + dwViewmatrix[0][2] * pos.z + dwViewmatrix[0][3];
	float _y = dwViewmatrix[1][0] * pos.x + dwViewmatrix[1][1] * pos.y + dwViewmatrix[1][2] * pos.z + dwViewmatrix[1][3];
	float _z = dwViewmatrix[2][0] * pos.x + dwViewmatrix[2][1] * pos.y + dwViewmatrix[2][2] * pos.z + dwViewmatrix[2][3];
	float w  = dwViewmatrix[3][0] * pos.x + dwViewmatrix[3][1] * pos.y + dwViewmatrix[3][2] * pos.z + dwViewmatrix[3][3];


	float x = (window_size.x / 2 * (_x / w)) + ((_x / w) + window_size.x / 2);

	float y = -(window_size.y / 2 * (_y / w)) + ((_y / w) + window_size.y / 2);

	return ImVec3(x, y, w);
}

ImVec3 hooks::GetClosestPoint(ImVec3 current_position, ImVec3 dest, std::vector<ImVec3>& positions)
{
	std::vector<ImVec3> temp;

	for (auto& pos : positions)
	{
		if (current_position.DistTo(pos) > 20 and current_position.DistTo(dest) > pos.DistTo(dest))
			temp.push_back(pos);
	}

	for (int i = 0; i < temp.size(); i++)
	{
		for (int j = 0; j < temp.size() - 1; j++)
		{

			if (current_position.DistTo(temp[j]) > current_position.DistTo(temp[j + 1]))
			{
				const auto& b = temp[j];
				temp[j] = temp[j + 1];
				temp[j + 1] = b;
			}
		}
	}
	if (!temp.empty())
		return temp[0];
	return dest;
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

	for (int i = 0; i < positions.size(); i++)
	{
			auto pos1 = WorldToScreen(positions[i]);
			if (pos1.z > 0)
				ImGui::GetBackgroundDrawList()->AddText(pos1, ImColor(255, 255, 255), std::format("{}", positions[i].DistTo(ImVec3(1069, -185, 76))).c_str());
	}
	if (positions.size() > 5)
	{
		ClientBase* client = (ClientBase*)GetModuleHandle("client.dll");
		auto pos = WorldToScreen(GetClosestPoint(client->LocalPlayer->m_vecOrigin, ImVec3(1069, -185, 76), positions));
		if (pos.z > 0)
			ImGui::GetBackgroundDrawList()->AddLine(pos, ImVec2(1920/ 2, 1080), ImColor(255, 0, 0));
	}
	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

	typedef long(__stdcall* EndScene)(LPDIRECT3DDEVICE9);
	return reinterpret_cast<EndScene>(hooks::oEndScene)(p_device);
}