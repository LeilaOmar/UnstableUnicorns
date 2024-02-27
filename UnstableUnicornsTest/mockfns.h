#pragma once

#define ProcessStdin(a, b) ProcessStdin_mock(a, b)
#define WaitForSingleObject(a, b) simple(a, b)
#define EndGame EndGame_mock

void ProcessStdin_mock(char *stdinBuf, int *bufIndex);
DWORD simple(HANDLE hHandle, DWORD dwMilliseconds);
void EndGame_mock(int winningPnum);
