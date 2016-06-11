#include <windows.h>
#include <wrl.h>
#include <mmdeviceapi.h>
#include <endpointvolume.h>

#include "AudioEndpointVolume.hpp"

namespace ChVol
{
    AudioEndpointVolume::AudioEndpointVolume()
    {
    }

    AudioEndpointVolume::~AudioEndpointVolume()
    {
    }

    HRESULT AudioEndpointVolume::Activate()
    {
        if ( audioEndpointVolume != nullptr ) { return S_FALSE; }

        Microsoft::WRL::ComPtr<IMMDeviceEnumerator> mmDeviceEnumerator;
        HRESULT hr = CoCreateInstance(__uuidof(MMDeviceEnumerator),
            nullptr, CLSCTX_INPROC_SERVER,
            IID_PPV_ARGS(mmDeviceEnumerator.GetAddressOf()));
        if ( FAILED(hr) ) { return hr; }

        Microsoft::WRL::ComPtr<IMMDevice> mmDevice;
        hr = mmDeviceEnumerator->GetDefaultAudioEndpoint(
            EDataFlow::eRender, ERole::eConsole, mmDevice.GetAddressOf());
        if ( FAILED(hr) ) { return hr; }

        hr = mmDevice->Activate(__uuidof(IAudioEndpointVolume),
            CLSCTX_INPROC_SERVER, nullptr,
            reinterpret_cast<void**>(audioEndpointVolume.GetAddressOf()));
        return hr;
    }

    VOID AudioEndpointVolume::Deactivate()
    {
        audioEndpointVolume = nullptr;
    }

    HRESULT AudioEndpointVolume::IsHardwareSupported(LPBOOL lpbSupported)
    {
        DWORD hardwareSupportMask;

        HRESULT hr = audioEndpointVolume->QueryHardwareSupport(&hardwareSupportMask);
        if ( SUCCEEDED(hr) )
        {
            *lpbSupported = (hardwareSupportMask & ENDPOINT_HARDWARE_SUPPORT_MUTE) &&
                (hardwareSupportMask & ENDPOINT_HARDWARE_SUPPORT_VOLUME);
        }

        return hr;
    }

    HRESULT AudioEndpointVolume::ToggleMute()
    {
        BOOL bMute = FALSE;

        HRESULT hr = audioEndpointVolume->GetMute(&bMute);
        if ( FAILED(hr) ) { return hr; }

        hr = audioEndpointVolume->SetMute(!bMute, nullptr);
        return hr;
    }

    HRESULT AudioEndpointVolume::GetMasterVolumeLevel(float *pfLevel)
    {
        return audioEndpointVolume->GetMasterVolumeLevelScalar(pfLevel);
    }

    HRESULT AudioEndpointVolume::SetMasterVolumeLevel(float fLevel)
    {
        return audioEndpointVolume->SetMasterVolumeLevelScalar(fLevel, nullptr);
    }
}
