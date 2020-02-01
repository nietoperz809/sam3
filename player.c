#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <mmsystem.h>
#include "player.h"

static BOOL _playFlag;
static UINT m_numdevs;
static HWAVEOUT m_hwo;
static WAVEHDR m_whdr;
static WAVEFORMATEX m_wvx;

void PL_Init()
{
	m_hwo = NULL;
	_playFlag = FALSE;
	m_numdevs = waveOutGetNumDevs();
}

static void CALLBACK waveOutProc(HWAVEOUT hwo, UINT uMsg, DWORD_PTR inst, DWORD_PTR p1, DWORD_PTR p2)
{
	if (uMsg == WOM_DONE)
	{
		_playFlag = FALSE;
	}
}

void PL_Stop()
{
	if (m_hwo)
	{
		waveOutReset(m_hwo);
		waveOutUnprepareHeader(m_hwo, &m_whdr, sizeof(WAVEHDR));
		waveOutClose(m_hwo);
	}
	m_hwo = 0;
}

// Gibt eine Error-Msg aus
static void ViewError(MMRESULT mmerror, const wchar_t* caption)
{
	wchar_t errstring[256];
	waveOutGetErrorText(mmerror, errstring, 200);
	wcscat(errstring, caption);
	MessageBox(NULL, errstring, L"FUCK!", MB_OK | MB_ICONSTOP);
}


// Spielt einen DatenBlock als WAVE ab...
MMRESULT PL_Play (WORD channels, DWORD s_rate, WORD bps, void* data, DWORD length, BOOL mapflag)
{
	MMRESULT mmerror;
	UINT devid = WAVE_MAPPER;

	// Wave-Mapper benutzen oder nicht?
	if (!mapflag)
		devid = 0;

	// Falls vorher schon offen, erstmal schliessen
	PL_Stop();

	// WaveFormat-Struct einstellen
	ZeroMemory(&m_wvx, sizeof(m_wvx));
	m_wvx.wFormatTag = WAVE_FORMAT_PCM;
	m_wvx.nChannels = channels;
	m_wvx.nSamplesPerSec = s_rate;
	m_wvx.nAvgBytesPerSec = s_rate * channels * bps / 8;
	m_wvx.nBlockAlign = channels * bps / 8;
	m_wvx.wBitsPerSample = bps;
	m_wvx.cbSize = 0;

	// WaveHeader einstellen
	ZeroMemory(&m_whdr, sizeof(m_whdr));
	m_whdr.lpData = (LPSTR)data;
	m_whdr.dwBufferLength = length;
	m_whdr.dwFlags = 0;

	// Wave-Device oeffnen
	mmerror = waveOutOpen (&m_hwo, devid, &m_wvx, (DWORD_PTR)waveOutProc, 0,	CALLBACK_FUNCTION | WAVE_ALLOWSYNC);
	waveOutReset(m_hwo);

	// Falls Error -> Ausgeben und tschuess...
	if (mmerror != MMSYSERR_NOERROR)
	{
		ViewError(mmerror, L"WaveOutOpen:");
		return mmerror;
	}

	// WaveHeader anmelden
	mmerror = waveOutPrepareHeader (m_hwo, &m_whdr, sizeof(WAVEHDR));

	// Falls Error -> Ausgeben und tschuess...
	if (mmerror != MMSYSERR_NOERROR)
	{
		ViewError(mmerror, L"WaveOutPrepareHeader:");
		return mmerror;
	}

	// Wave abspielen
	_playFlag = TRUE;
	mmerror = waveOutWrite (m_hwo, &m_whdr, sizeof(WAVEHDR));

	// Falls Error -> Ausgeben und tschuess...
	if (mmerror != MMSYSERR_NOERROR)
	{
		_playFlag = FALSE;
		ViewError(mmerror, L"WaveOutWrite:");
		return mmerror;
	}
	while (_playFlag == TRUE)
		Sleep(100);

	return mmerror;
}
