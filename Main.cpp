//---------------------------------------------------------------------------//
//
// Main.cpp
//
//---------------------------------------------------------------------------//

#include <windows.h>
#include <strsafe.h>
#include <set>

#include "Plugin.hpp"
#include "MessageDef.hpp"
#include "Utility.hpp"

#include "Main.hpp"

//---------------------------------------------------------------------------//
//
// グローバル変数
//
//---------------------------------------------------------------------------//

HINSTANCE g_hInst { nullptr };

//---------------------------------------------------------------------------//

// プラグインの名前
LPCTSTR PLUGIN_NAME { TEXT("ボリュームを変更") };

//---------------------------------------------------------------------------//

// コマンドID
enum CMD : INT32
{
    CMD_MUTE                = 1,
    CMD_VOLUME_UP_FIRST     = 1000,
    CMD_VOLUME_UP_LAST      = 1100,
    CMD_VOLUME_DOWN_FIRST   = 2000,
    CMD_VOLUME_DOWN_LAST    = 2100,
    CMD_VOLUME_SET_FIRST    = 3000,
    CMD_VOLUME_SET_LAST     = 3100
};

//---------------------------------------------------------------------------//

// プラグインの情報
PLUGIN_INFO g_info =
{
    0,                   // プラグインI/F要求バージョン
    (LPTSTR)PLUGIN_NAME, // プラグインの名前（任意の文字が使用可能）
    nullptr,             // プラグインのファイル名（相対パス）
    ptLoadAtUse,         // プラグインのタイプ
    0,                   // バージョン
    0,                   // バージョン
    0,                   // コマンド個数（InitPluginInfo で動的に更新する）
    nullptr,             // コマンド（InitPluginInfo で動的に更新する）
    0,                   // ロードにかかった時間（msec）
};

//---------------------------------------------------------------------------//

constexpr INT32 CMD_MAX { 10 };
constexpr UINT32 VOL_MAX { 100 };

LPCTSTR SECTION_COMMAND { TEXT("Command") };

void SetCommandData(PLUGIN_COMMAND_INFO *cmdInfo, LPCTSTR Name, LPCTSTR Caption, INT32 CommandID)
{
    cmdInfo->Name = CopyString(Name);
    cmdInfo->Caption = CopyString(Caption);
    cmdInfo->CommandID = CommandID;
    cmdInfo->Attr = 0;
    cmdInfo->ResID = -1;
    cmdInfo->DispMenu = dmHotKeyMenu;
    cmdInfo->TimerInterval = 0;
    cmdInfo->TimerCounter = 0;
}

void UnsetCommandData(PLUGIN_COMMAND_INFO *cmdInfo)
{
    DeleteString(cmdInfo->Name);
    cmdInfo->Name = nullptr;
    DeleteString(cmdInfo->Caption);
    cmdInfo->Caption = nullptr;
}

//---------------------------------------------------------------------------//

// TTBEvent_InitPluginInfo() の内部実装
void WINAPI InitPluginInfo(void)
{
    if ( g_info.CommandCount != 0 ) { return; }

    TCHAR ininame[MAX_PATH];

    // iniファイル名取得
    const auto len = ::GetModuleFileName(g_hInst, ininame, MAX_PATH);
    ininame[len - 3] = 'i';
    ininame[len - 2] = 'n';
    ininame[len - 1] = 'i';

    //ボリュームアップ＆ダウン
    std::set<UINT> vol;
    for ( auto i = 0; i < CMD_MAX; i++ )
    {
        TCHAR key[8];
        if ( SUCCEEDED(::StringCchPrintf(key, 8, TEXT("UpDown%d"), i)) )
        {
            auto param = ::GetPrivateProfileInt(SECTION_COMMAND, key, 0, ininame);
            if ( 0 < param && param <= VOL_MAX )
            {
                vol.insert(param);
            }
        }
    }

    //セット
    std::set<UINT> set;
    for ( auto i = 0; i < CMD_MAX; i++ )
    {
        TCHAR key[8];
        if ( SUCCEEDED(::StringCchPrintf(key, 8, TEXT("Set%d"), i)) )
        {
            auto param = ::GetPrivateProfileInt(SECTION_COMMAND, key, 0, ininame);
            if ( 0 <= param && param <= VOL_MAX )
            {
                set.insert(param);
            }
        }
    }

    /* メモリ確保 */
    const auto count = 1 + vol.size() * 2 + set.size();
    auto pCI = new PLUGIN_COMMAND_INFO[count];
    g_info.CommandCount = static_cast<DWORD>(count);
    g_info.Commands = pCI;

    /* コマンド作成 */
    //ミュート
    SetCommandData(pCI++, TEXT("Mute"), TEXT("ミュート"), CMD_MUTE);

    //ボリュームアップ＆ダウン
    for ( auto i : vol )
    {
        TCHAR Name[32], Caption[32];
        if ( SUCCEEDED(::StringCchPrintf(Name, 32, TEXT("VolumeUp(%d)"), i)) &&
            SUCCEEDED(::StringCchPrintf(Caption, 32, TEXT("ボリュームを%d上げる"), i)) )
        {
            SetCommandData(pCI++, Name, Caption, CMD_VOLUME_UP_FIRST + i);
        }
        if ( SUCCEEDED(::StringCchPrintf(Name, 32, TEXT("VolumeDown(%d)"), i)) &&
            SUCCEEDED(::StringCchPrintf(Caption, 32, TEXT("ボリュームを%d下げる"), i)) )
        {
            SetCommandData(pCI++, Name, Caption, CMD_VOLUME_DOWN_FIRST + i);
        }
    }

    //セット
    for ( auto i : set )
    {
        TCHAR Name[32], Caption[32];
        if ( SUCCEEDED(::StringCchPrintf(Name, 32, TEXT("Set(%d)"), i)) &&
            SUCCEEDED(::StringCchPrintf(Caption, 32, TEXT("ボリュームを%dにセット"), i)) )
        {
            SetCommandData(pCI++, Name, Caption, CMD_VOLUME_SET_FIRST + i);
        }
    }
}

// TTBEvent_InitPluginInfo() の内部実装
void WINAPI FreePluginInfo(void)
{
    if ( g_info.CommandCount == 0 ) { return; }

    const auto count = static_cast<INT32>(g_info.CommandCount);
    auto pCI = g_info.Commands;
    for ( auto i = 0; i < count; i++ )
    {
        UnsetCommandData(pCI++);
    }
    delete[] g_info.Commands;

    g_info.CommandCount = 0;
    g_info.Commands = nullptr;
}

// TTBEvent_Init() の内部実装
BOOL WINAPI Init(void)
{
    return TRUE;
}

//---------------------------------------------------------------------------//

// TTBEvent_Unload() の内部実装
void WINAPI Unload(void)
{
}

//---------------------------------------------------------------------------//

// TTBEvent_Execute() の内部実装
BOOL WINAPI Execute(INT32 CmdId, HWND hWnd)
{
    UNREFERENCED_PARAMETER(hWnd); // ERASE ME
    UNREFERENCED_PARAMETER(CmdId); // ERASE ME
    return TRUE;

    /*switch ( CmdId )
    {
        case CMD_DUMMY:
        {
            WriteLog(elDebug, TEXT("%s|%d"), g_info.Filename, CmdId);
            return TRUE;
        }
        default:
        {
            return FALSE;
        }
    }*/
}

//---------------------------------------------------------------------------//

// TTBEvent_WindowsHook() の内部実装
void WINAPI Hook(UINT Msg, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(Msg);    // ERASE ME
    UNREFERENCED_PARAMETER(wParam); // ERASE ME
    UNREFERENCED_PARAMETER(lParam); // ERASE ME
}

//---------------------------------------------------------------------------//
//
// CRT を使わないため new/delete を自前で実装
//
//---------------------------------------------------------------------------//

#if defined(_NODEFLIB)

void* __cdecl operator new(size_t size)
{
    return ::HeapAlloc(::GetProcessHeap(), 0, size);
}

void __cdecl operator delete(void* p)
{
    if ( p != nullptr ) ::HeapFree(::GetProcessHeap(), 0, p);
}

void __cdecl operator delete(void* p, size_t) // C++14
{
    if ( p != nullptr ) ::HeapFree(::GetProcessHeap(), 0, p);
}

void* __cdecl operator new[](size_t size)
{
    return ::HeapAlloc(::GetProcessHeap(), 0, size);
}

void __cdecl operator delete[](void* p)
{
    if ( p != nullptr ) ::HeapFree(::GetProcessHeap(), 0, p);
}

void __cdecl operator delete[](void* p, size_t) // C++14
{
    if ( p != nullptr ) ::HeapFree(::GetProcessHeap(), 0, p);
}

// プログラムサイズを小さくするためにCRTを除外
#pragma comment(linker, "/nodefaultlib:libcmt.lib")
#pragma comment(linker, "/entry:DllMain")

#endif

//---------------------------------------------------------------------------//

// DLL エントリポイント
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD fdwReason, LPVOID)
{
    if ( fdwReason == DLL_PROCESS_ATTACH )
    {
        g_hInst = hInstance;
    }
    return TRUE;
}

//---------------------------------------------------------------------------//

// Main.cpp