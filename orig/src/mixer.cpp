/* ===========================================================================
    mixer.cpp

  �\�[�X�͉��L���x�[�X�ɍ쐬���Ă��܂��B���ӁI
    ���v���O���~���O<http://hp.vector.co.jp/authors/VA016117/>
    TClock2ch ��FDQ3TClock�_������

  ����Ȃ킯�ŁA���̃t�@�C���Ɋւ��鏔���̌����͎��̂��̂ł͂Ȃ��ł��B
   =========================================================================== */
#include <windows.h>
#include <mmsystem.h>

BOOL InitMasterVolumeControl(int);
BOOL InitMasterMuteControl(void);

#define	MAXCHANNEL	10		//�`�����l���̍ő吔�i���E�����Ȃ�Q�`�����l���j

HMIXER		g_hMixer=NULL;
MIXERCAPS	g_mxcaps;
DWORD		g_dwMinimum,g_dwMaximum;
DWORD		g_cChannels;
DWORD		g_dwVolumeControlID=-1;
DWORD		g_dwMuteControlID=-1;

typedef struct{
	MIXERCONTROLDETAILS_UNSIGNED	Volume[MAXCHANNEL];
	DWORD							Max;
} LASTVOLINFO,*LPLASTVOLINFO;

LASTVOLINFO	g_LastVolInfo;

BOOL InitMixer(void)
{
	if(g_hMixer) return TRUE;
	if(mixerOpen(&g_hMixer,0,0,0,MIXER_OBJECTF_MIXER)!=MMSYSERR_NOERROR) return FALSE;
	if(mixerGetDevCaps((UINT)g_hMixer, &g_mxcaps, sizeof(MIXERCAPS))!=MMSYSERR_NOERROR) return FALSE;
	return	TRUE;
}

void ReleaseMixer(void)
{
	if(g_hMixer) mixerClose(g_hMixer);
	g_hMixer=NULL;
	g_dwVolumeControlID=-1;
	g_dwMuteControlID=-1;
}

//-----------------------------------------------------------------------------
//             �@�@�@�@�@�@�@�@�@�@�@�@�{�����[��
//-----------------------------------------------------------------------------

BOOL InitMasterVolumeControl(int volComponentType)
{
	int	i;
	DWORD dwVolComponentType;
	MIXERLINE mxl;
	MIXERCONTROL mxc;
	MIXERLINECONTROLS mxlc;

	if(g_dwVolumeControlID != -1) return TRUE;
	if(InitMixer() == FALSE) return FALSE;

	// ���ߐ�؂�ւ�
	if ( volComponentType >= 0 && volComponentType <= 8 )
		dwVolComponentType = MIXERLINE_COMPONENTTYPE_DST_FIRST + volComponentType;
	else if ( volComponentType >= 9 && volComponentType <= 19 )
		dwVolComponentType = MIXERLINE_COMPONENTTYPE_SRC_FIRST + volComponentType - 9;
	else
		dwVolComponentType = MIXERLINE_COMPONENTTYPE_DST_FIRST + 4;

	mxl.cbStruct = sizeof(MIXERLINE);
	mxl.dwComponentType = dwVolComponentType;
	if(mixerGetLineInfo((HMIXEROBJ)g_hMixer,&mxl,
				MIXER_OBJECTF_HMIXER | MIXER_GETLINEINFOF_COMPONENTTYPE)!=MMSYSERR_NOERROR) return FALSE;

	mxlc.cbStruct = sizeof(MIXERLINECONTROLS);
	mxlc.dwLineID = mxl.dwLineID;
	mxlc.dwControlType = MIXERCONTROL_CONTROLTYPE_VOLUME;
	mxlc.cControls = 1;
	mxlc.cbmxctrl = sizeof(MIXERCONTROL);
	mxlc.pamxctrl = &mxc;
	if(mixerGetLineControls((HMIXEROBJ)g_hMixer,&mxlc,
			   MIXER_OBJECTF_HMIXER | MIXER_GETLINECONTROLSF_ONEBYTYPE) != MMSYSERR_NOERROR) return FALSE;

	g_cChannels = (mxl.cChannels > MAXCHANNEL)? MAXCHANNEL : mxl.cChannels;
	g_dwMinimum = mxc.Bounds.dwMinimum;
	g_dwMaximum = mxc.Bounds.dwMaximum;
	g_dwVolumeControlID = mxc.dwControlID;
	
	for(i = 0; i < MAXCHANNEL; i++) g_LastVolInfo.Volume[i].dwValue = 1;
	g_LastVolInfo.Max = 1;

	return TRUE;
}

BOOL GetMasterVolume(int *Val, int volControlType)
{
	MIXERCONTROLDETAILS_UNSIGNED mxcdVolume;
	MIXERCONTROLDETAILS mxcd;

	if(InitMasterVolumeControl(volControlType) == FALSE) return FALSE;

	mxcd.cbStruct = sizeof(MIXERCONTROLDETAILS);
	mxcd.dwControlID = g_dwVolumeControlID;
	mxcd.cChannels = 1;
	mxcd.cMultipleItems = 0;
	mxcd.cbDetails = sizeof(MIXERCONTROLDETAILS_UNSIGNED);
	mxcd.paDetails = &mxcdVolume;
	if(mixerGetControlDetails((HMIXEROBJ)g_hMixer,&mxcd,
				 MIXER_OBJECTF_HMIXER | MIXER_GETCONTROLDETAILSF_VALUE)!=MMSYSERR_NOERROR) return FALSE;
	
	*Val = ((mxcdVolume.dwValue-g_dwMinimum)*100 + (g_dwMaximum-g_dwMinimum)/2)/(g_dwMaximum-g_dwMinimum);

	return TRUE;
}

BOOL SetMasterVolume(int Val, int volControlType)
{
	MIXERCONTROLDETAILS_UNSIGNED	mxcdVolume[MAXCHANNEL];
	MIXERCONTROLDETAILS				mxcd;
	DWORD							dwVal,dwMaxVal;
	DWORD							i;

	if(InitMasterVolumeControl(volControlType) == FALSE) return FALSE;

	mxcd.cbStruct = sizeof(MIXERCONTROLDETAILS);
	mxcd.dwControlID = g_dwVolumeControlID;
	mxcd.cChannels = g_cChannels;
	mxcd.cMultipleItems = 0;
	mxcd.cbDetails = sizeof(MIXERCONTROLDETAILS_UNSIGNED);
	mxcd.paDetails = mxcdVolume;

	if(mixerGetControlDetails((HMIXEROBJ)g_hMixer,&mxcd,
				 MIXER_OBJECTF_HMIXER | MIXER_GETCONTROLDETAILSF_VALUE)!=MMSYSERR_NOERROR) return FALSE;
	
	dwMaxVal = mxcdVolume[0].dwValue;
	for(i = 1; i < g_cChannels; i++){
		if(mxcdVolume[i].dwValue > dwMaxVal) dwMaxVal = mxcdVolume[i].dwValue;
	}

	if(dwMaxVal){
		CopyMemory(g_LastVolInfo.Volume,mxcdVolume,sizeof(g_LastVolInfo.Volume));
		g_LastVolInfo.Max = dwMaxVal;
	}

	dwVal = Val*(g_dwMaximum-g_dwMinimum)/100+g_dwMinimum;
	for(i = 0; i < g_cChannels; i++){
		mxcdVolume[i].dwValue = dwVal * g_LastVolInfo.Volume[i].dwValue / g_LastVolInfo.Max;
	}

	if (mixerSetControlDetails((HMIXEROBJ)g_hMixer,&mxcd,
				 MIXER_OBJECTF_HMIXER | MIXER_SETCONTROLDETAILSF_VALUE)!=MMSYSERR_NOERROR) return FALSE;
	
	return TRUE;
}

BOOL UpDownMasterVolume(int dif, int volControlType)
{
	int	Val;

	if(GetMasterVolume(&Val, volControlType)==FALSE) return FALSE;

	Val+=dif;
	if(Val>100) Val=100;
	else if(Val<0) Val=0;

	if(SetMasterVolume(Val, volControlType)==FALSE) return FALSE;

	return	TRUE;
}

//-----------------------------------------------------------------------------
//             �@�@�@�@�@�@�@�@�@�@�@�@�~���[�g
//-----------------------------------------------------------------------------

BOOL InitMasterMuteControl()
{
	MIXERLINE mxl;
	MIXERCONTROL mxc;
	MIXERLINECONTROLS mxlc;

	if(g_dwMuteControlID != -1) return TRUE;
	if(InitMixer() == FALSE) return FALSE;

	mxl.cbStruct = sizeof(MIXERLINE);
	mxl.dwComponentType = MIXERLINE_COMPONENTTYPE_DST_SPEAKERS;
	if(mixerGetLineInfo((HMIXEROBJ)g_hMixer,&mxl,
				MIXER_OBJECTF_HMIXER | MIXER_GETLINEINFOF_COMPONENTTYPE)!=MMSYSERR_NOERROR) return FALSE;

	mxlc.cbStruct = sizeof(MIXERLINECONTROLS);
	mxlc.dwLineID = mxl.dwLineID;
	mxlc.dwControlType = MIXERCONTROL_CONTROLTYPE_MUTE;
	mxlc.cControls = 1;
	mxlc.cbmxctrl = sizeof(MIXERCONTROL);
	mxlc.pamxctrl = &mxc;
	if(mixerGetLineControls((HMIXEROBJ)g_hMixer,&mxlc,
			   MIXER_OBJECTF_HMIXER | MIXER_GETLINECONTROLSF_ONEBYTYPE) != MMSYSERR_NOERROR) return FALSE;

	g_dwMuteControlID = mxc.dwControlID;

	return TRUE;
}

BOOL GetMasterMute(BOOL *Val)
{
	MIXERCONTROLDETAILS_BOOLEAN mxcdMute;
	MIXERCONTROLDETAILS mxcd;

	if(InitMasterMuteControl() == FALSE) return FALSE;

	mxcd.cbStruct = sizeof(MIXERCONTROLDETAILS);
	mxcd.dwControlID = g_dwMuteControlID;
	mxcd.cChannels = 1;
	mxcd.cMultipleItems = 0;
	mxcd.cbDetails = sizeof(MIXERCONTROLDETAILS_BOOLEAN);
	mxcd.paDetails = &mxcdMute;
	if (mixerGetControlDetails((HMIXEROBJ)g_hMixer,&mxcd,
				 MIXER_OBJECTF_HMIXER | MIXER_GETCONTROLDETAILSF_VALUE)!=MMSYSERR_NOERROR) return FALSE;
	
	*Val = mxcdMute.fValue;

	return TRUE;
}

BOOL SetMasterMute(BOOL Val)
{
	MIXERCONTROLDETAILS_BOOLEAN mxcdMute;
	MIXERCONTROLDETAILS mxcd;

	if(InitMasterMuteControl() == FALSE) return FALSE;

	mxcdMute.fValue=Val;

	mxcd.cbStruct = sizeof(MIXERCONTROLDETAILS);
	mxcd.dwControlID = g_dwMuteControlID;
	mxcd.cChannels = 1;
	mxcd.cMultipleItems = 0;
	mxcd.cbDetails = sizeof(MIXERCONTROLDETAILS_BOOLEAN);
	mxcd.paDetails = &mxcdMute;
	if (mixerSetControlDetails((HMIXEROBJ)g_hMixer,&mxcd,
				 MIXER_OBJECTF_HMIXER | MIXER_SETCONTROLDETAILSF_VALUE)!=MMSYSERR_NOERROR) return FALSE;
	
	return TRUE;
}

BOOL ReverseMasterMute(void)
{
	BOOL	bVal;

	if(GetMasterMute(&bVal)==FALSE) return FALSE;
	if(SetMasterMute(!bVal)==FALSE) return FALSE;
	
	return TRUE;
}
