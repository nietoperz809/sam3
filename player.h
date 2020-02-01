#pragma once

void PL_Init();
void PL_Stop();
MMRESULT PL_Play(WORD channels, DWORD s_rate, WORD bps, void* data, DWORD length, BOOL mapflag);



