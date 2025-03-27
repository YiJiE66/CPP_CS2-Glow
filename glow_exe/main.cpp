#include "memory.h"
#include <thread>
#include <iostream>
#include <random>

namespace offsets {
    constexpr std::ptrdiff_t dwEntityList = 0x1A38800;
    constexpr std::ptrdiff_t m_hPlayerPawn = 0x814;
    constexpr std::ptrdiff_t m_iTeamNum = 0x3E3;
    constexpr std::ptrdiff_t dwLocalPlayerController = 0x1A89E90;
    DWORD64 m_Glow = 0xC00;
    DWORD64 m_glowColorOverride = 0x40;
    DWORD64 m_bGlowing = 0x51;
}

int main()
{
    SetConsoleTitleA("System Service");
    std::cout << "[+] YJ-Glow 启动\n";
	std::cout << "[+] 作者: YJ\n";
	std::cout << "[+] 关闭此窗口以关闭发光功能\n";

    while (true) {
        uintptr_t localplayercontroller = VARS::memRead<uintptr_t>(VARS::baseAddress + offsets::dwLocalPlayerController);
        if (!localplayercontroller) {
            Sleep(10);
            continue;
        }

        int localteam = VARS::memRead<int>(localplayercontroller + offsets::m_iTeamNum);
        uintptr_t entity_list = VARS::memRead<uintptr_t>(VARS::baseAddress + offsets::dwEntityList);

        if (!entity_list) continue;

        for (int i = 1; i < 64; i++) {
            uintptr_t list_entry = VARS::memRead<uintptr_t>(entity_list + (8 * (i & 0x7FFF) >> 9) + 16);
            if (!list_entry) continue;

            uintptr_t player = VARS::memRead<uintptr_t>(list_entry + 120 * (i & 0x1FF));
            if (!player) continue;

            int Pteam = VARS::memRead<int>(player + offsets::m_iTeamNum);
            if (Pteam == localteam) continue;

            uint32_t player_pawn = VARS::memRead<uint32_t>(player + offsets::m_hPlayerPawn);
            uintptr_t list_entry2 = VARS::memRead<uintptr_t>(entity_list + 0x8 * ((player_pawn & 0x7FFF) >> 9) + 16);
            if (!list_entry2) continue;

            uintptr_t p_cs_player_pawn = VARS::memRead<uintptr_t>(list_entry2 + 120 * (player_pawn & 0x1FF));
            if (!p_cs_player_pawn) continue;

            VARS::memWrite<DWORD64>(p_cs_player_pawn + offsets::m_Glow + offsets::m_glowColorOverride, 0x6FFF9F9F);
            VARS::memWrite<DWORD64>(p_cs_player_pawn + offsets::m_Glow + offsets::m_bGlowing, 1);
        }
        Sleep(0.2);
    }
    return 0;
}