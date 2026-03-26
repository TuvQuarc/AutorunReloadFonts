#include "pch.h"

struct AutorunInterface
{
    DWORD dwSize = sizeof(AutorunInterface);
    HWND hWnd = nullptr;
    LPVOID lpAdapter = nullptr;
};

struct PluginFuncInfo
{
    DWORD dwSize = sizeof(PluginFuncInfo);
    DWORD dwFlags = 0;
    LPCWSTR lpFuncName = nullptr;
    LPVOID lpFuncAddr = nullptr;
};

using APIProc = int(__stdcall*)(UINT, LPVOID);

namespace {
    LPVOID adapter_ptr = nullptr;
    HWND tc_hwnd = nullptr;

    constexpr UINT WM_RELOAD_FONTS = WM_USER + 21;
}

int ReloadFonts(int nCommand, LPVOID pData)
{
    if (tc_hwnd)
    {
        SetLastError(NO_ERROR);
        
        SendNotifyMessage(HWND_BROADCAST, WM_FONTCHANGE, 0, 0);
        SendMessage(tc_hwnd, WM_RELOAD_FONTS, 0, 0);
        
        const auto error_code = GetLastError();
        if (error_code == NO_ERROR) {
            return 0;
        }
        else {
            return 1;
        }
    }
    return 0;
}

extern "C" __declspec(dllexport) void __stdcall Autorun_PluginInit(AutorunInterface *pInterface)
{
    if (!pInterface || pInterface->dwSize < sizeof(AutorunInterface))
        return;

    adapter_ptr = pInterface->lpAdapter;
    tc_hwnd = pInterface->hWnd;

    PluginFuncInfo func_info{
        sizeof(PluginFuncInfo),
        0,
        L"ReloadFonts",
        reinterpret_cast<LPVOID>(ReloadFonts)
    };

    if (auto proc = reinterpret_cast<APIProc>(adapter_ptr))
        proc(1, &func_info);
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    return TRUE;
}
