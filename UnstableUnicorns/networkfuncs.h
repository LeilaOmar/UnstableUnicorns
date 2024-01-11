#pragma once
#include <time.h>
#include "globals.h"

struct nurseryPacket {
	unsigned int index; // nursery_index (points to top-most baby)
	unsigned int size;  // total number of babies left
	int ref[NURSERY_SIZE]; // reference array for the nursery
};

struct discardPacket {
	unsigned int index;
	int ref[DECK_SIZE];
};

struct deckPacket {
	unsigned int index;
	int ref[DECK_SIZE];
};

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
int sendLobbyPacket(int num_players, int clientpnum, int fd);
int receiveLobbyPacket(int* num_players, int* clientpnum, int fd);
int sendNurseryInfo(struct nurseryPacket info, int fd);
int receiveNurseryInfo(struct nurseryPacket info, int fd);
int sendDiscardInfo(struct discardPacket info, int fd);
int receiveDiscardInfo(struct discardPacket info, int fd);
int sendDeckInfo(struct deckPacket info, int fd);
int receiveDeckInfo(struct deckPacket info, int fd);
