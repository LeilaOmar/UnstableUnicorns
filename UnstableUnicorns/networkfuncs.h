#pragma once
#include <time.h>
#include "globals.h"

#define MSGBUF 1024
#define NEIGHTIME 4000 // 4000 ms

extern char stdinbuf[MSGBUF];
extern int bufindex;

int sendInt(int num, int fd);
int receiveInt(int* num, int fd);

// sends the entire player structure up to current_players
int sendPlayers(int fd);
int receivePlayers(int fd);

// sends the entire unicorn array
int sendUnicorns(struct Unicorn* corns, int size, int fd);
int receiveUnicorns(struct Unicorn* corns, int size, int fd);

// sends deck, nursery, discard pile, and player info
int sendGamePacket(int fd);
int receiveGamePacket(int fd);

void receiveMsg(char* str, int fd);

// handles stdin events and filters everything but keydown presses
// https://stackoverflow.com/questions/19955617/win32-read-from-stdin-with-timeout
void processStdin(char* stdinbuf, int bufindex);
