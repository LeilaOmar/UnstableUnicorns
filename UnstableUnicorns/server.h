#pragma once
#include "networkfuncs.h"

int newConnection(SOCKET *cfd);
int serverMain(void);
int serverInit(short portno);
