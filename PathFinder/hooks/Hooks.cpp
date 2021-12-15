#include "Hooks.h"



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

void hooks::Initialize()
{
	MH_Initialize();
	auto present_addr = (DWORD)(GetModuleHandle("d3d9.dll")) + 0xE3640;

	byte original_present_bytes[]{ '\x8B', '\xFF', '\x55', '\x8B', '\xEC' };
	Memory::patch((BYTE*)present_addr, original_present_bytes, 5);

	hooks::oWndProc = (DWORD)(SetWindowLongPtr(FindWindow(NULL, xorstr("Team Fortress 2")), GWL_WNDPROC, reinterpret_cast<LONG_PTR>(hooks::WndProc)));

	MH_CreateHook((LPVOID)present_addr, &hooks::hkPresent, reinterpret_cast<LPVOID*>(&hooks::oPresent));
	MH_EnableHook((LPVOID)present_addr);
}
void hooks::DetchHooks()
{
	MH_DisableHook(MH_ALL_HOOKS);
	Sleep(500);
	MH_RemoveHook(MH_ALL_HOOKS);
	SetWindowLongPtr(FindWindowA(NULL, xorstr(("Team Fortress 2"))), GWLP_WNDPROC, (LONG_PTR)(oWndProc));
	MH_Uninitialize();
}
bool init = false;
int __stdcall hooks::hkPresent(CustomDirect3D9Device* pDevice, int a2, int a3, int a4, int a5)
{
	if (!init)
	{
		ImGui::CreateContext();

		ImGui_ImplWin32_Init(FindWindow(NULL, "Team Fortress 2"));
		ImGui_ImplDX9_Init(pDevice);
		init = true;

	}

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("PathFinder");
	{
		ImGui::Text("Hello world!");
		if (ImGui::Button("Save path"))
		{
			std::ofstream file("main.path", std::ios::binary);
			file.write((const char*)&hooks::positions[0], sizeof(ImVec3) * positions.size());
			file.close();
		}
		if (ImGui::Button("Load path"))
		{

			std::ifstream ifs("main.path", std::ios::binary | std::ios::ate);

			int file_size = ifs.tellg();
			char* tmp = new char[file_size];

			ifs.seekg(0, std::ios::beg);
			ifs.read(tmp, file_size);
			ifs.close();

			if (file_size > 0)
			{
				hooks::positions.clear();

				for (int i = 0; i < file_size; i+= 12)
				{
					ImVec3 tmp_vec = *(ImVec3*)( (DWORD)(tmp) + i );
					hooks::positions.push_back(tmp_vec);
				}
			}
			delete tmp;
		}
		if (ImGui::Button("Clear path"))
		{
			hooks::positions.clear();
		}
	}
	ImGui::End();

	for (int i = 0; i < positions.size(); i++)
	{
			auto pos1 = pDevice->WorldToScreen(positions[i]);
			if (pos1.z > 0)
				ImGui::GetBackgroundDrawList()->AddText(pos1, ImColor(255, 255, 255), std::format("{}", positions[i].DistTo(ImVec3(0, 0, 0))).c_str());
	}
	ClientBase* client = (ClientBase*)GetModuleHandle("client.dll");
	auto pos = pDevice->WorldToScreen(GetClosestPoint(client->LocalPlayer->m_vecOrigin, ImVec3(0, 0, 0), positions));

	if (pos.z > 0)
	{
		auto line_start_pos = pDevice->GetWindowSize();
		line_start_pos.x /= 2;
		ImGui::GetBackgroundDrawList()->AddLine(pos, line_start_pos, ImColor(255, 0, 0));
	}
	
	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

	typedef int(__stdcall* Present)(CustomDirect3D9Device*, int, int, int, int);
	return reinterpret_cast<Present>(oPresent)(pDevice, a2, a3, a4, a5);
}

LRESULT WINAPI hooks::WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
	typedef LRESULT(CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);
	return CallWindowProc((WNDPROC)hooks::oWndProc, hWnd, uMsg, wParam, lParam);
}

