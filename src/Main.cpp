/* ===========================================================================
                            ボリュームを変更(VC++)

                                 Main.cpp
   =========================================================================== */
#include "Windows.h"
#include <stdio.h>

#include "Main.h"
#include "MessageDef.h"

int g_VolComponentType;
HINSTANCE g_hInstance;

#define CLOCKM_VOLCHANGE	(WM_USER+6)

void SetCommandData(PLUGIN_COMMAND_INFO*, const char*, const char*, int);
void GetSetting(void);
BOOL GetIniFileName(char *);
void RefreshClock(void);

// --------------------------------------------------------
//    コマンドを作成。
// --------------------------------------------------------
void SetCommand(PLUGIN_INFO *plugin)
{
	const int CMD_MAX = 10;
	char ininame[MAX_PATH];
	int i, buf, vol[CMD_MAX], set[CMD_MAX];
	DWORD count = 1;
	PLUGIN_COMMAND_INFO *pCI;
	char Name[128], Caption[128];

/* INIからデータ取得 */
	GetIniFileName(ininame);

	//ボリュームアップ＆ダウン
	for (i = 0; i < CMD_MAX; i++) {
		char key[] = "UpDown0";
		key[6] += i;
		buf = ::GetPrivateProfileInt("Command", key, -1, ininame);
		if (buf < 0 || buf > 100) {
			vol[i] = -1;
		} else {
			vol[i] = buf;
			count += 2;
		}
	}

	//セット
	for (i = 0; i < CMD_MAX; i++) {
		char key[] = "Set0";
		key[3] += i;
		buf = ::GetPrivateProfileInt("Command", key, -1, ininame);
		if (buf < 0 || buf > 100) {
			set[i] = -1;
		} else {
			set[i] = buf;
			count++;
		}
	}

/* メモリ確保 */
	pCI = (PLUGIN_COMMAND_INFO *)malloc(sizeof(PLUGIN_COMMAND_INFO) * count);
	plugin->CommandCount = count;
	plugin->Commands = pCI;

/* コマンド作成 */
	//ボリュームアップ＆ダウン
	for (i = 0; i < CMD_MAX; i++) {
		if (vol[i] < 0) continue;

		sprintf(Name, "VolumeUp(%d)", vol[i]);
		sprintf(Caption, "ボリュームを%d上げる", vol[i]);
		SetCommandData(pCI++, Name, Caption, CMD_VOLUME_UP_FIRST + vol[i]);

		sprintf(Name, "VolumeDown(%d)", vol[i]);
		sprintf(Caption, "ボリュームを%d下げる", vol[i]);
		SetCommandData(pCI++, Name, Caption, CMD_VOLUME_DOWN_FIRST + vol[i]);
	}

	//セット
	for (i = 0; i < CMD_MAX; i++) {
		if (set[i] < 0) continue;

		sprintf(Name, "Set(%d)", set[i]);
		sprintf(Caption, "ボリュームを%dにセット", set[i]);
		SetCommandData(pCI++, Name, Caption, CMD_VOLUME_SET_FIRST + set[i]);
	}

	//ミュート
	SetCommandData(pCI++, "Mute", "ミュート", CMD_MUTE);
}
void SetCommandData(PLUGIN_COMMAND_INFO *cmdInfo, const char *Name, const char *Caption, int CommandID)
{
	cmdInfo->Name = (char *)malloc(strlen(Name) + 1);
	if (cmdInfo->Name != NULL)
		strcpy(cmdInfo->Name, Name);
	cmdInfo->Caption = (char *)malloc(strlen(Caption) + 1);
	if (cmdInfo->Caption != NULL)
		strcpy(cmdInfo->Caption, Caption);
	cmdInfo->CommandID = CommandID;
	cmdInfo->Attr = 0;
	cmdInfo->ResID = -1;
	cmdInfo->DispMenu = dmHotKeyMenu;
	cmdInfo->TimerInterval = 0;
	cmdInfo->TimerCounter = 0;
}


// --------------------------------------------------------
//    プラグインがロードされたときに呼ばれる
// --------------------------------------------------------
BOOL Init(void)
{
	GetSetting();
	return TRUE;
}

// --------------------------------------------------------
//    プラグインがアンロードされたときに呼ばれる
// --------------------------------------------------------
void Unload(void)
{
}

// --------------------------------------------------------
//    コマンド実行時に呼ばれる
// --------------------------------------------------------
BOOL Execute(int CmdId, HWND hWnd)
{
	BOOL ret = FALSE;
	if (CmdId == CMD_MUTE) {
		ret = ReverseMasterMute();

	} else if (CmdId >= CMD_VOLUME_UP_FIRST && CmdId <= CMD_VOLUME_UP_LAST) {
		ret = UpDownMasterVolume(CmdId - CMD_VOLUME_UP_FIRST, g_VolComponentType);
	
	} else if (CmdId >= CMD_VOLUME_DOWN_FIRST && CmdId <= CMD_VOLUME_DOWN_LAST) {
		ret = UpDownMasterVolume((CmdId - CMD_VOLUME_DOWN_FIRST) * (-1), g_VolComponentType);

	} else if (CmdId >= CMD_VOLUME_SET_FIRST && CmdId <= CMD_VOLUME_SET_LAST) {
		ret = SetMasterVolume(CmdId - CMD_VOLUME_SET_FIRST, g_VolComponentType);

	}

	//ボリューム変更に成功したら時計をリフレッシュ
	if (ret) {
		RefreshClock();
	}

	return ret;
}

// --------------------------------------------------------
//    グローバルフックメッセージがやってくる
// --------------------------------------------------------
void Hook(UINT Msg, DWORD wParam, DWORD lParam)
{
}


// --------------------------------------------------------
//    DLLMain
// --------------------------------------------------------
#ifdef __BORLANDC__
#pragma argsused
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
{
	return 1;
}
#endif

#if _MSC_VER > 1000
BOOL WINAPI DllMain (HINSTANCE hInstance, DWORD fdwReason, LPVOID lpvReserved)
{
	if (fdwReason == DLL_PROCESS_ATTACH)
		g_hInstance = hInstance;
	return TRUE;
}
#endif

// --------------------------------------------------------
//    iniファイル名取得
// --------------------------------------------------------
BOOL GetIniFileName(char *buff)
{
	int len;

	//iniファイル名取得
	len = ::GetModuleFileName(g_hInstance, buff, MAX_PATH);
	buff[len - 3] = 'i';
	buff[len - 2] = 'n';
	buff[len - 1] = 'i';

	return TRUE;
}

// --------------------------------------------------------
//    設定情報取得
// --------------------------------------------------------
void GetSetting(void)
{
	char ininame[MAX_PATH];

	GetIniFileName(ininame);
	g_VolComponentType = ::GetPrivateProfileInt("Setting", "VolComponentType", 4, ininame);
}


// --------------------------------------------------------
//    TClockで書かれた時計をリフレッシュ
// --------------------------------------------------------
void RefreshClock(void)
{
	//時計部分のウィンドウハンドルを探す
	HWND hWndTaskbar = FindWindow("Shell_TrayWnd", "");
	HWND hWndTray = FindWindowEx(hWndTaskbar, NULL, "TrayNotifyWnd", "");
	HWND hWndClock = FindWindowEx(hWndTray, NULL, "TrayClockWClass", NULL);

	//リフレッシュメッセージを送る
	SendMessage(hWndClock, CLOCKM_VOLCHANGE, 0, 0);
}
