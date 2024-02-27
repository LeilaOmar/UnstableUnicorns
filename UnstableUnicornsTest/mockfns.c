#include <globals.h>
#include "mockfns.h"

void ProcessStdin_mock(char *stdinBuf, int *bufIndex) {
  fgets(stdinBuf, 64, fpinput);
  *bufIndex = strlen(stdinBuf);
}

DWORD simple(HANDLE hHandle, DWORD dwMilliseconds) {
  return WAIT_OBJECT_0;
}

void EndGame_mock(int winningPnum) {
  // blank function to avoid ending the tests prematurely
  return;
}
