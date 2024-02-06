#pragma once
#include "networkevents.h"

int newConnection(SOCKET *cfd);
int serverMain(void);
int serverInit(short portno);
