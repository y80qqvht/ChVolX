/* ===========================================================================
                           TTB Plugin Template(VC++)

                                 Plugin.cpp

   =========================================================================== */
#include "Plugin.h"
#include "Main.h"
#include "MessageDef.h"


// --------------------------------------------------------
//    �{�̑��G�N�X�|�[�g�֐�
// --------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif
PLUGIN_INFO* (WINAPI *TTBPlugin_GetPluginInfo)(DWORD hPlugin);
void (WINAPI *TTBPlugin_SetPluginInfo)(DWORD hPlugin, PLUGIN_INFO *PluginInfo);
void (WINAPI *TTBPlugin_FreePluginInfo)(PLUGIN_INFO *PluginInfo);
void (WINAPI *TTBPlugin_SetMenuProperty)(DWORD hPlugin, int CommandID, DWORD CheckFlag, DWORD Flag);
PLUGIN_INFO** (WINAPI *TTBPlugin_GetAllPluginInfo)(void);
void (WINAPI *TTBPlugin_FreePluginInfoArray)(PLUGIN_INFO **PluginInfoArray);
#ifdef __cplusplus
};
#endif

/* ���[�J���֐���` */
void GetVersion(char *, DWORD *, DWORD *);

/* �O���[�o���ϐ� */
char *PLUGIN_FILENAME;	// �v���O�C���̃t�@�C�����BTTBase�t�H���_����̑��΃p�X
DWORD PLUGIN_HANDLE;	// TTBase���v���O�C�������ʂ��邽�߂̃R�[�h

// ****************************************************************
// *
// *         �v���O�C�� �C�x���g
// *
// --------------------------------------------------------
//    �v���O�C�����\���̂̃Z�b�g
// --------------------------------------------------------
extern "C"
PLUGIN_INFO* WINAPI TTBEvent_InitPluginInfo(char *PluginFilename)
{
	PLUGIN_INFO		*result;	/* �Ԓl	*/

	/* �t�@�C�����i���΃p�X�j���R�s�[ */
	PLUGIN_FILENAME = (char *)malloc(strlen(PluginFilename) + 1);
	if (PLUGIN_FILENAME != NULL)
		strcpy(PLUGIN_FILENAME, PluginFilename);

	/* �v���O�C�����\���̂̐��� */
	result = (PLUGIN_INFO *)malloc(sizeof(PLUGIN_INFO));
	if (result == NULL) return NULL;
	
	/* �v���O�C���̖��O */
	result->Name = (char *)malloc(strlen(PLUGIN_NAME) + 1);
	if (result->Name != NULL)
		strcpy(result->Name, PLUGIN_NAME);
	/* �v���O�C���t�@�C���� */
	result->Filename = (char *)malloc(strlen(PLUGIN_FILENAME) + 1);
	if (result->Filename != NULL)
		strcpy(result->Filename, PLUGIN_FILENAME);
	/* �v���O�C���^�C�v */
	result->PluginType = PLUGIN_TYPE;
	/* �o�[�W�������̎擾 */
	GetVersion(PLUGIN_FILENAME, &result->VersionMS, &result->VersionLS);
	/* �R�}���h�̍쐬 */
	SetCommand(result);

	return result;
}

// --------------------------------------------------------
//    �v���O�C�����\���̂̔j��
// --------------------------------------------------------
extern "C"
void WINAPI TTBEvent_FreePluginInfo(PLUGIN_INFO *PluginInfo)
{
	DWORD i;
	PLUGIN_COMMAND_INFO *pCI;

	for (i = 0; i < PluginInfo->CommandCount; i++) {
		pCI = &PluginInfo->Commands[i];
		free(pCI->Name);
		free(pCI->Caption);
	}
	free(PluginInfo->Commands);
	free(PluginInfo->Filename);
	free(PluginInfo->Name);
	free(PluginInfo);
}

// --------------------------------------------------------
//    �v���O�C��������
// --------------------------------------------------------
extern "C"
BOOL WINAPI TTBEvent_Init(char *PluginFilename, DWORD hPlugin)
{
	// ���b�Z�[�W��`
	RegisterMessages();
	// �L���b�V���̂��߂ɁATTBPlugin_InitPluginInfo�͌Ă΂�Ȃ��ꍇ������
	// ���̂��߁AInit�ł�PLUGIN_FILENAME�̏��������s��
	if (PLUGIN_FILENAME != NULL) free(PLUGIN_FILENAME);
	PLUGIN_FILENAME = (char *)malloc(strlen(PluginFilename) + 1);
	if (PLUGIN_FILENAME != NULL)
		strcpy(PLUGIN_FILENAME, PluginFilename);

	// TTBase���v���O�C�������ʂ��邽�߂̃R�[�h
	PLUGIN_HANDLE = hPlugin;

	// API�֐��̎擾
	HMODULE hModule = GetModuleHandle(NULL);
	(FARPROC&)TTBPlugin_GetPluginInfo = GetProcAddress(hModule, "TTBPlugin_GetPluginInfo");
	(FARPROC&)TTBPlugin_SetPluginInfo = GetProcAddress(hModule, "TTBPlugin_SetPluginInfo");
	(FARPROC&)TTBPlugin_FreePluginInfo = GetProcAddress(hModule, "TTBPlugin_FreePluginInfo");
	(FARPROC&)TTBPlugin_SetMenuProperty = GetProcAddress(hModule, "TTBPlugin_SetMenuProperty");
	(FARPROC&)TTBPlugin_GetAllPluginInfo = GetProcAddress(hModule, "TTBPlugin_GetAllPluginInfo");
	(FARPROC&)TTBPlugin_FreePluginInfoArray = GetProcAddress(hModule, "TTBPlugin_FreePluginInfo");

	return Init();
}

// --------------------------------------------------------
//    �v���O�C���A�����[�h���̏���
// --------------------------------------------------------
extern "C"
void WINAPI TTBEvent_Unload(void)
{
	Unload();
}

// --------------------------------------------------------
//    �R�}���h���s
// --------------------------------------------------------
extern "C"
BOOL WINAPI TTBEvent_Execute(int CommandID, HWND hWnd)
{
	return Execute(CommandID, hWnd);
}

// --------------------------------------------------------
//    �t�b�N�iShellHook,MouseHook)
// --------------------------------------------------------
extern "C"
void WINAPI TTBEvent_WindowsHook(UINT Msg, DWORD wParam, DWORD lParam)
{
	Hook(Msg, wParam, lParam);
}

// ****************************************************************
// *
// *         ���[�e�B���e�B���[�`��
// *
// --------------------------------------------------------
//    �o�[�W��������Ԃ�
// --------------------------------------------------------
static void GetVersion(char *Filename, DWORD *VersionMS, DWORD *VersionLS)
{
	DWORD	Size;
	LPDWORD	hVersion;
	LPVOID	pVersionInfo;
	VS_FIXEDFILEINFO	*FixedFileInfo;
	UINT	ItemLen;


	*VersionMS = 0;
	*VersionLS = 0;

	hVersion = NULL;

	Size = GetFileVersionInfoSize(Filename, hVersion);
	if (Size == 0) return;

	pVersionInfo = (LPVOID)malloc(Size);
	if (pVersionInfo == NULL) return;

	GetFileVersionInfo(Filename, 0, Size, pVersionInfo);

	if (VerQueryValue(pVersionInfo, "\\", (void **)&FixedFileInfo, &ItemLen)) {
		*VersionMS = FixedFileInfo->dwFileVersionMS;
		*VersionLS = FixedFileInfo->dwFileVersionLS;
	}

	free(pVersionInfo);
}

// --------------------------------------------------------
//    �v���O�C�����\���̂�Src���R�s�[���ĕԂ�
// --------------------------------------------------------
PLUGIN_INFO* CopyPluginInfo(PLUGIN_INFO *PluginInfo)
{
	PLUGIN_INFO *Result;
	PLUGIN_COMMAND_INFO *Command, *SrcCommand;
	DWORD i;
	
	Result = (PLUGIN_INFO *)malloc(sizeof(PLUGIN_INFO));
	*Result = *PluginInfo;
	Result->Name = (char *)malloc(strlen(PluginInfo->Name) + 1);
	strcpy(Result->Name, PluginInfo->Name);
	Result->Filename = (char *)malloc(strlen(PluginInfo->Filename) + 1);
	strcpy(Result->Filename, PluginInfo->Filename);
	
	Result->Commands = (PLUGIN_COMMAND_INFO *)malloc(
		sizeof(PLUGIN_INFO) * PluginInfo->CommandCount);
	for (i = 0; i < PluginInfo->CommandCount; i++) {
		Command = &Result->Commands[i];
		SrcCommand = &PluginInfo->Commands[i];
		*Command = *SrcCommand;
		Command->Name = (char *)malloc(strlen(SrcCommand->Name) + 1);
		strcpy(Command->Name, SrcCommand->Name);
		Command->Caption = (char *)malloc(strlen(SrcCommand->Caption) + 1);
		strcpy(Command->Caption, SrcCommand->Caption);
	}

	return Result;
}

// --------------------------------------------------------
//  �v���O�C�����ō쐬���ꂽ�v���O�C�����\���̂�j������
// --------------------------------------------------------
void FreePluginInfo(PLUGIN_INFO *PluginInfo)
{
	TTBEvent_FreePluginInfo(PluginInfo);
}
