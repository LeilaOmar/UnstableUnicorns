#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib,"ws2_32.lib") // Winsock library
#define WINDOWS_IGNORE_PACKING_MISMATCH

#define LINE_MAX 33
#define DESC_SIZE 256
#define BUF_SIZE 1024
#define HAND_START 5
#define HAND_SIZE 16
#define MAX_PLAYERS 8
#define STABLE_SIZE 25

// in the future, these would be changed to global ints composed of all chosen decks;
// the packet structures would have to allocate space at runtime

#define DECK_SIZE 116
#define NURSERY_SIZE 13

enum UnicornDatatypes {ANY = -1, BABYUNICORN, BASICUNICORN, MAGICALUNICORN, UPGRADE, DOWNGRADE, MAGIC, INSTANT, ANYUNICORN, UNICORN, NARWHAL, NOSPECIES};

enum BabySelection { BABYRED, BABYPINK, BABYORANGE, BABYYELLOW, BABYGREEN, BABYBLUE, BABYPURPLE, BABYBLACK, BABYWHITE, BABYBROWN, BABYRAINBOW, BABYDEATH, BABYNARWHAL };

struct Unicorn {
  unsigned char cType;    //!<  0 = Baby Unicorn
                          //!<  1 = Basic Unicorn
                          //!<  2 = Magical Unicorn
                          //!<  3 = Upgrade
                          //!<  4 = Downgrade
                          //!<  5 = Magic
                          //!<  6 = Instant
  unsigned char species;  //!<  8 = Unicorn
                          //!<  9 = Narwhal
                          //!< 10 = Other or N/A [for now...]
  char name[LINE_MAX];
  char description[DESC_SIZE];
  short effect;
  int id;
  int bitmap;
};

struct Deck {
  struct Unicorn* cards;
  int size; //!< This should never exceed the max, so no there's point in a max variable for reallocating
};

struct CardsInHand {
  struct Unicorn cards[HAND_SIZE];
  int numCards;
};

struct Stable {
  struct Unicorn unicorns[STABLE_SIZE]; //!< Despite being called unicorns, this struct includes Upgrade/Downgrade cards too
  int numUnicorns;
  int size;
};

struct Player {
  struct CardsInHand hand;
  struct Stable stable;
  char username[LINE_MAX];
  short flags;  //!< enum BaseFlags
  enum BabySelection icon;
  short filler; //!< padding for prev variable
};

SOCKET clientsockfd[MAX_PLAYERS - 1];
SOCKET sockfd;
SOCKET udpfd;
HANDLE mutex;
extern int isClient;

extern struct Deck deck;
extern struct Deck nursery;
extern struct Deck discardpile;
extern struct Player player[MAX_PLAYERS];
extern int currentPlayers;
extern int moveCount;
extern int totalTurns;
extern int WIN_CONDITION;

// ********************************************************************************
// ******************************** Input Functions *******************************
// ********************************************************************************

extern FILE *fpinput;
int NumInput(char *buf, char **end, int size);
char CharInput(char *buf, int size);

// ********************************************************************************
// ******************************* Logging Functions ******************************
// ********************************************************************************

extern int isLog;

/**
 * @brief Creates a new log file and sets the file descriptor as write only
 */
void SetLogFD(char *filename);

/**
 * @brief Logs the player action
 * @param pnum Player number
 * @param corn Card played
 * @param move C-string of the type of move/action performed (e.g. "play" or "discard")
 */
void LogMove(int pnum, struct Unicorn corn, char *move);

/**
 * @brief Logs all of player[pnum]'s data
 */
void LogPlayer(int pnum);

/**
 * @brief Logs info for every player and card pile during a game state
 * @param state The current game state of the game (e.g. END_TURN = logging at the end of a turn)
 */
void LogGameData(int pnum, int state);

// ********************************************************************************
// ***************************** Init/De-Init Functions ***************************
// ********************************************************************************

/**
 * @brief Initializes the Deck, Nursery, and Discard piles based on the chosen card packs
 * @note In the future, use a flag for decks chosen
 */
void InitDeck(struct Deck *nursery, struct Deck *deck, struct Deck *discard);

/**
 * @brief Closes sockets and frees memory
 */
void Cleanup(void);

// ********************************************************************************
// **************************** Pretty Color Functions ****************************
// ********************************************************************************

// https://www.codeproject.com/Tips/5255355/How-to-Put-Color-on-Windows-Console
// add: reg add HKEY_CURRENT_USER\Console /v VirtualTerminalLevel /t REG_DWORD /d 0x00000001 /f
// undo: reg add HKEY_CURRENT_USER\Console /v VirtualTerminalLevel /t REG_DWORD /d 0x00000000 /f

void Red(void);
void Yellow(void);
void Green(void);
void Cyan(void);
void Blue(void);
void Magenta(void);
void ResetCol(void);
void Rainbow(char *str);
