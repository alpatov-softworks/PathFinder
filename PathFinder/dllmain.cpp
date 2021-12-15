#include <Windows.h>
#include <d3d9.h>
#include "imgui/imgui.h"
#include "hooks/Hooks.h"
#include "MinHook.h"
#include "SDK/CBaseEntity.h"
#include "SDK/ClientBase.h"
#include "Utils/memory.h"


DWORD WINAPI LoadThread(HMODULE h_module)
{
    hooks::Initialize();
    ClientBase* client = (ClientBase*)GetModuleHandle("client.dll");

    while (!GetAsyncKeyState(VK_END))
    {
        if (GetAsyncKeyState(VK_XBUTTON1) & 1)
        {
            hooks::positions.push_back(client->LocalPlayer->m_vecOrigin);
        }
        else if (GetAsyncKeyState(VK_DELETE) & 1)
        {
            while (client->LocalPlayer->m_vecOrigin.DistTo(ImVec3(0, 0, 0)) > 200)
            {
                client->m_ForceForward = 1;
                client->LocalPlayer->AimAt(hooks::GetClosestPoint(client->LocalPlayer->m_vecOrigin, ImVec3(0, 0, 0), hooks::positions) + client->LocalPlayer->m_vecViewOffset);
            }
            client->m_ForceForward = 0;
        }
        Sleep(10);

    }
    hooks::DetchHooks();

    ImGui_ImplWin32_Shutdown();
    ImGui_ImplDX9_Shutdown();
    ImGui::DestroyContext();

    FreeLibraryAndExitThread(h_module, 0);
}

BOOL APIENTRY DllMain( HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{

    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)LoadThread, hModule, 0, nullptr);
    }

    return TRUE;
}

