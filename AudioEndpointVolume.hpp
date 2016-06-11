#pragma once

#include <windows.h>
#include <wrl.h>
#include <endpointvolume.h>

namespace ChVolX
{
    // スピーカー・マイク等の音声デバイスの音量を操作するクラス
    class AudioEndpointVolume
    {
    private:
        // コピーコンストラクタ：使用不可
        AudioEndpointVolume(const AudioEndpointVolume &);

        // 代入演算子：使用不可
        AudioEndpointVolume &operator=(const AudioEndpointVolume &);

        // 音量操作用COMコンポーネント
        Microsoft::WRL::ComPtr<IAudioEndpointVolume> audioEndpointVolume;

    public:
        // コンストラクタ
        AudioEndpointVolume();

        // デストラクタ
        ~AudioEndpointVolume();

        // 必要なリソースを確保し、音量操作を使用可能にする。
        // 内部でCOMコンポーネントを使用するため、
        // CoInitialize関数が実行済みである必要がある。
        // COMコンポーネントの使用結果をHRESULTで返す。
        HRESULT Activate();

        // 音量操作に必要なリソースを解放する。
        // 再度Activateメソッドを呼び出すことで、使用可能に戻すこともできる。
        VOID Deactivate();

        // 音量操作をハードウェアでサポートするかどうかを取得する。
        // Activateメソッドが実行済みである必要がある。
        // COMコンポーネントの使用結果をHRESULTで返す。
        // lpbSupportedの示す値がFALSEとなるデバイスの場合、
        // 排他モード使用中に音量の変更ができない。
        HRESULT IsHardwareSupported(LPBOOL lpbSupported);

        // ミュートのON/OFFを切り替える。
        // Activateメソッドが実行済みである必要がある。
        // COMコンポーネントの使用結果をHRESULTで返す。
        HRESULT ToggleMute();

        // 音量の現在値（0.0以上1.0以下）を取得する。
        // Activateメソッドが実行済みである必要がある。
        // COMコンポーネントの使用結果をHRESULTで返す。
        HRESULT GetMasterVolumeLevel(float *pfLevel);

        // 音量を指定された値（0.0以上1.0以下）へ設定する。
        // Activateメソッドが実行済みである必要がある。
        // COMコンポーネントの使用結果をHRESULTで返す。
        HRESULT SetMasterVolumeLevel(float fLevel);
    };
}
