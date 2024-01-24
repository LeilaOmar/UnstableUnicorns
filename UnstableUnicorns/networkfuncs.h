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
