/* ===========================================================================
                           TTB Plugin Template(VC++)

                                 Plugin.h

   =========================================================================== */
#ifndef _PLUG_H
#define _PLUG_H

#include "Windows.h"

// �\���̃A���C�����g���k
#pragma pack(1)

/* �v���O�C���̃��[�h�^�C�v */
#define	ptAlwaysLoad		0
#define	ptLoadAtUse			1
#define	ptSpecViolation		0xFFFF

/* ���j���[�\���Ɋւ���萔 */
#define dmNone 			0	/* �����o���Ȃ� 	*/
#define dmSystemMenu	1	/* �V�X�e�����j���[	*/
#define dmToolMenu		2	/* �c�[�����j���[	*/
#define dmHotKeyMenu	4	/* �z�b�g�L�[		*/
#define dmChecked		8	/* ���j���[�̃`�F�b�N�}�[�N */
#define dmUnchecked		0	/* ���j���[�̃`�F�b�N�}�[�N�����Ȃ� */
#define dmEnabled		0	/* ���j���[��Enable�� */
#define dmDisabled		0	/* ���j���[��Disable���� */
#define DISPMENU_MENU		dmToolMenu|dmSystemMenu
#define	DISPMENU_ENABLED	dmDisabled
#define DISPMENU_CHECKED	dmChecked


/* --------------------------------------------------------*/
/*    �\���̒�`                                           */
/* --------------------------------------------------------*/
/* �R�}���h���\����  */
typedef struct
{
	char	*Name;		/*	�R�}���h�̖��O�i�p���j				*/
	char	*Caption;	/*	�R�}���h�̐����i���{��j			*/
	int		CommandID;	/*	�R�}���h�ԍ�						*/
	int		Attr;		/*	�A�g���r���[�g�i���g�p�j			*/
	int		ResID;		/*	���\�[�X�ԍ��i���g�p�j				*/
	int		DispMenu;	/*	���j���[�ɏo�����ǂ����B			*/
						/*		SysMenu:1 ToolMenu:2 None: 0	*/
	DWORD	TimerInterval;	/* �^�C�}�[���s�Ԋu[msec]�@0: �g�p���Ȃ�	*/
    DWORD	TimerCounter;	/* �V�X�e�������Ŏg�p				*/
} PLUGIN_COMMAND_INFO;

/* �v���O�C�����\����  */
typedef struct
{
	WORD	NeedVersion;	/* �v���O�C��I/F�v���o�[�W����		*/
	char	*Name;			/* �v���O�C���̐����i���{��j		*/
	char	*Filename;		/* �v���O�C���̃t�@�C�����i���΃p�X�j*/
	WORD	PluginType;		/* �v���O�C���̃��[�h�^�C�v			*/
	DWORD	VersionMS;		/* �o�[�W����						*/
	DWORD	VersionLS;		/* �o�[�W����						*/
	DWORD	CommandCount;	/* �R�}���h��						*/
	PLUGIN_COMMAND_INFO	*Commands;		/* �R�}���h				*/
	/* �ȉ��V�X�e���ŁATTBase�{�̂Ŏg�p����						*/
	DWORD	LoadTime;      /* ���[�h�ɂ����������ԁimsec�j		*/
} PLUGIN_INFO;


#ifdef __cplusplus
extern "C" {
#endif

// --------------------------------------------------------
//    �v���O�C�����G�N�X�|�[�g�֐�
// --------------------------------------------------------
#ifdef __BORLANDC__
#define DLLEXPORT __declspec(dllexport)
#endif
#if     _MSC_VER > 1000
#define DLLEXPORT
#endif
// �K�{
DLLEXPORT PLUGIN_INFO* WINAPI TTBEvent_InitPluginInfo(char *PluginFilename);
DLLEXPORT void WINAPI TTBEvent_FreePluginInfo(PLUGIN_INFO *PluginInfo);
// �C��
DLLEXPORT BOOL WINAPI TTBEvent_Init(char *PluginFilename, DWORD hPlugin);
DLLEXPORT void WINAPI TTBEvent_Unload(void);
DLLEXPORT BOOL WINAPI TTBEvent_Execute(int CommandID, HWND hWnd);
DLLEXPORT void WINAPI TTBEvent_WindowsHook(UINT Msg, DWORD wParam, DWORD lParam);

// --------------------------------------------------------
//    �{�̑��G�N�X�|�[�g�֐�
// --------------------------------------------------------
extern PLUGIN_INFO* (WINAPI *TTBPlugin_GetPluginInfo)(DWORD hPlugin);
extern void (WINAPI *TTBPlugin_SetPluginInfo)(DWORD hPlugin, PLUGIN_INFO *PluginInfo);
extern void (WINAPI *TTBPlugin_FreePluginInfo)(PLUGIN_INFO *PluginInfo);
extern void (WINAPI *TTBPlugin_SetMenuProperty)(DWORD hPlugin, int CommandID, DWORD CheckFlag, DWORD Flag);
extern PLUGIN_INFO** (WINAPI *TTBPlugin_GetAllPluginInfo)(void);
extern void (WINAPI *TTBPlugin_FreePluginInfoArray)(PLUGIN_INFO **PluginInfoArray);

// --------------------------------------------------------
//    ���[�e�B���e�B���[�`��
// --------------------------------------------------------
extern PLUGIN_INFO* CopyPluginInfo(PLUGIN_INFO *PluginInfo);
extern void FreePluginInfo(PLUGIN_INFO *PluginInfo);

/* �O���ϐ� */
extern char *PLUGIN_FILENAME;	// �v���O�C���̃t�@�C�����BTTBase�t�H���_����̑��΃p�X
extern DWORD PLUGIN_HANDLE;		// TTBase���v���O�C�������ʂ��邽�߂̃R�[�h

#ifdef __cplusplus
};
#endif

#endif