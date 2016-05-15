/* ===========================================================================
                    �A�N�e�B�u�E�B���h�E�T�C�Y��ύX(VC++)

                                 Main.h

   =========================================================================== */
#include "Plugin.h"

// --------------------------------------------------------
//    �v���O�C���̏��
// --------------------------------------------------------
/* �v���O�C���̖��O�i�Q�o�C�g���\�j */
#define	PLUGIN_NAME	"�{�����[����ύX"

/* �v���O�C���̃^�C�v */
#define	PLUGIN_TYPE	ptLoadAtUse

// --------------------------------------------------------
//    �R�}���h�̏��
// --------------------------------------------------------
/* �R�}���hID */
#define CMD_MUTE					1
#define CMD_VOLUME_UP_FIRST			1000
#define CMD_VOLUME_UP_LAST			1100
#define CMD_VOLUME_DOWN_FIRST		2000
#define CMD_VOLUME_DOWN_LAST		2100
#define CMD_VOLUME_SET_FIRST		3000
#define CMD_VOLUME_SET_LAST			3100

// ���R�}���h���e�ɂ��ẮAMain.cpp���Q��

// --------------------------------------------------------
//    �֐���`
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

