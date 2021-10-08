#pragma once
#include <Windows.h>
#include <d3d9.h>
#include "imgui/imgui.h"
#include "hooks/Hooks.h"
#include "MinHook.h"
#include "SDK/CBaseEntity.h"
#include "SDK/ClientBase.h"


DWORD WINAPI LoadThread(HMODULE h_module)
{
    MH_Initialize();
    
    auto end_scene_addr = (DWORD)(GetModuleHandle("d3d9.dll")) + 0x63130;

    MH_CreateHook((LPVOID)end_scene_addr, &hooks::hkEndScene, reinterpret_cast<LPVOID*>(&hooks::oEndScene));
    MH_EnableHook((LPVOID)end_scene_addr);

    CBaseEntity* local_player = *(CBaseEntity**)((DWORD)(GetModuleHandle("client.dll")) + 0xC3F650);
    ClientBase* client = (ClientBase*)GetModuleHandle("client.dll");
    while (!GetAsyncKeyState(VK_END))
    {
        if (GetAsyncKeyState(VK_INSERT) & 1)
        {
            hooks::positions.push_back(local_player->m_vecOrigin);
        }
        else if (GetAsyncKeyState(VK_DELETE) & 1)
        {
            client->m_ForceForward = 1;
            for (auto pos : hooks::positions)
            {
                while (pos.DistTo(local_player->m_vecOrigin) > 50)
                {
                    local_player->AimAt(pos + local_player->m_vecViewOffset);
                }
            }
            client->m_ForceForward = 0;
        }
        Sleep(10);

    }
    MH_DisableHook((LPVOID)end_scene_addr);
    MH_RemoveHook((LPVOID)end_scene_addr);
    MH_Uninitialize();
    Sleep(100);

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

