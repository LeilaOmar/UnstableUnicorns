#pragma once
#include "networkevents.h"

int newConnection(SOCKET *cfd);
void initGame(void);
int serverMain(void);
int serverInit(short portno);
