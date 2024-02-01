#pragma once
#include <time.h>
#include "globals.h"

#define MSGBUF 1024

extern char stdinbuf[MSGBUF];
extern int bufindex;

// hash functions for scrambling IP addresses
// https://stackoverflow.com/questions/664014/what-integer-hash-function-are-good-that-accepts-an-integer-hash-key
unsigned int hash(unsigned int x);
unsigned int unhash(unsigned int x);

// IP stuff
// https://stackoverflow.com/questions/39566240/how-to-get-the-external-ip-address-in-c
void getIPreq(char* ip_address);
int getLocalIP(char* ip_address);

// uses hash function to get code for ip (and vice versa)
unsigned int IPtoHexCode(char* ip);
void HexCodetoIP(char* code, char* dest);

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

void receiveMsg(int fd);

// handles stdin events and filters everything but keydown presses
// https://stackoverflow.com/questions/19955617/win32-read-from-stdin-with-timeout
void processStdin(void);
