/* ===========================================================================
                            �{�����[����ύX(VC++)

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
//    �R�}���h���쐬�B
// --------------------------------------------------------
void SetCommand(PLUGIN_INFO *plugin)
{
	const int CMD_MAX = 10;
	char ininame[MAX_PATH];
	int i, buf, vol[CMD_MAX], set[CMD_MAX];
	DWORD count = 1;
	PLUGIN_COMMAND_INFO *pCI;
	char Name[128], Caption[128];

/* INI����f�[�^�擾 */
	GetIniFileName(ininame);

	//�{�����[���A�b�v���_�E��
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

	//�Z�b�g
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

/* �������m�� */
	pCI = (PLUGIN_COMMAND_INFO *)malloc(sizeof(PLUGIN_COMMAND_INFO) * count);
	plugin->CommandCount = count;
	plugin->Commands = pCI;

/* �R�}���h�쐬 */
	//�{�����[���A�b�v���_�E��
	for (i = 0; i < CMD_MAX; i++) {
		if (vol[i] < 0) continue;

		sprintf(Name, "VolumeUp(%d)", vol[i]);
		sprintf(Caption, "�{�����[����%d�グ��", vol[i]);
		SetCommandData(pCI++, Name, Caption, CMD_VOLUME_UP_FIRST + vol[i]);

		sprintf(Name, "VolumeDown(%d)", vol[i]);
		sprintf(Caption, "�{�����[����%d������", vol[i]);
		SetCommandData(pCI++, Name, Caption, CMD_VOLUME_DOWN_FIRST + vol[i]);
	}

	//�Z�b�g
	for (i = 0; i < CMD_MAX; i++) {
		if (set[i] < 0) continue;

		sprintf(Name, "Set(%d)", set[i]);
		sprintf(Caption, "�{�����[����%d�ɃZ�b�g", set[i]);
		SetCommandData(pCI++, Name, Caption, CMD_VOLUME_SET_FIRST + set[i]);
	}

	//�~���[�g
	SetCommandData(pCI++, "Mute", "�~���[�g", CMD_MUTE);
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
//    �v���O�C�������[�h���ꂽ�Ƃ��ɌĂ΂��
// --------------------------------------------------------
BOOL Init(void)
{
	GetSetting();
	return TRUE;
}

// --------------------------------------------------------
//    �v���O�C�����A�����[�h���ꂽ�Ƃ��ɌĂ΂��
// --------------------------------------------------------
void Unload(void)
{
}

// --------------------------------------------------------
//    �R�}���h���s���ɌĂ΂��
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

	//�{�����[���ύX�ɐ��������玞�v�����t���b�V��
	if (ret) {
		RefreshClock();
	}

	return ret;
}

// --------------------------------------------------------
//    �O���[�o���t�b�N���b�Z�[�W������Ă���
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
//    ini�t�@�C�����擾
// --------------------------------------------------------
BOOL GetIniFileName(char *buff)
{
	int len;

	//ini�t�@�C�����擾
	len = ::GetModuleFileName(g_hInstance, buff, MAX_PATH);
	buff[len - 3] = 'i';
	buff[len - 2] = 'n';
	buff[len - 1] = 'i';

	return TRUE;
}

// --------------------------------------------------------
//    �ݒ���擾
// --------------------------------------------------------
void GetSetting(void)
{
	char ininame[MAX_PATH];

	GetIniFileName(ininame);
	g_VolComponentType = ::GetPrivateProfileInt("Setting", "VolComponentType", 4, ininame);
}


// --------------------------------------------------------
//    TClock�ŏ����ꂽ���v�����t���b�V��
// --------------------------------------------------------
void RefreshClock(void)
{
	//���v�����̃E�B���h�E�n���h����T��
	HWND hWndTaskbar = FindWindow("Shell_TrayWnd", "");
	HWND hWndTray = FindWindowEx(hWndTaskbar, NULL, "TrayNotifyWnd", "");
	HWND hWndClock = FindWindowEx(hWndTray, NULL, "TrayClockWClass", NULL);

	//���t���b�V�����b�Z�[�W�𑗂�
	SendMessage(hWndClock, CLOCKM_VOLCHANGE, 0, 0);
}
