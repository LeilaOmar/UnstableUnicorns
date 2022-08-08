#pragma once
#include "networkfuncs.h"

void newConnection(SOCKET *cfd);
void initGame(void);
int serverMain(void);
int serverInit(short portno);