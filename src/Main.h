/* ===========================================================================
                    アクティブウィンドウサイズを変更(VC++)

                                 Main.h

   =========================================================================== */
#include "Plugin.h"

// --------------------------------------------------------
//    プラグインの情報
// --------------------------------------------------------
/* プラグインの名前（２バイトも可能） */
#define	PLUGIN_NAME	"ボリュームを変更"

/* プラグインのタイプ */
#define	PLUGIN_TYPE	ptLoadAtUse

// --------------------------------------------------------
//    コマンドの情報
// --------------------------------------------------------
/* コマンドID */
#define CMD_MUTE					1
#define CMD_VOLUME_UP_FIRST			1000
#define CMD_VOLUME_UP_LAST			1100
#define CMD_VOLUME_DOWN_FIRST		2000
#define CMD_VOLUME_DOWN_LAST		2100
#define CMD_VOLUME_SET_FIRST		3000
#define CMD_VOLUME_SET_LAST			3100

// ★コマンド内容については、Main.cppを参照

// --------------------------------------------------------
//    関数定義
// --------------------------------------------------------
void SetCommand(PLUGIN_INFO*);
BOOL Init(void);
void Unload(void);
BOOL Execute(int, HWND); 
void Hook(UINT Msg, DWORD wParam, DWORD lParam);

// mixer.cpp
BOOL SetMasterVolume(int Val, int volControlType);
BOOL UpDownMasterVolume(int dif, int volControlType);
BOOL ReverseMasterMute(void);

