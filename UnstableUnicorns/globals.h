#pragma once
#define WINDOWS_IGNORE_PACKING_MISMATCH
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib,"ws2_32.lib") // Winsock library

#define NAME_SIZE 33
#define DESC_SIZE 256
#define LINE_MAX 32
#define BUF_SIZE 1024
#define HAND_START 5
#define HAND_SIZE 16
#define MAX_PLAYERS 8
#define STABLE_SIZE 25

// in the future, these would be updated to the theoretical max value
// so that the packet structures know how much to allocate at compile time
#define DECK_SIZE 116
#define NURSERY_SIZE 13

enum UnicornDatatypes {ANY = -1, BABYUNICORN, BASICUNICORN, MAGICUNICORN, UPGRADE, DOWNGRADE, MAGIC, INSTANT, ANYUNICORN, UNICORN, NARWHAL, NOSPECIES};

enum BabySelection { BABYRED, BABYPINK, BABYORANGE, BABYYELLOW, BABYGREEN, BABYBLUE, BABYPURPLE, BABYBLACK, BABYWHITE, BABYBROWN, BABYRAINBOW, BABYDEATH, BABYNARWHAL };

struct Unicorn {
  unsigned char class;    // 0 = Baby Unicorn
                          // 1 = Basic Unicorn
                          // 2 = Magic Unicorn
                          // 3 = Upgrade
                          // 4 = Downgrade
                          // 5 = Magic
                          // 6 = Instant
  unsigned char species;  // 8 = Unicorn
                          // 9 = Narwhal
                          // 10 = Other or N/A [for now...]
  char name[NAME_SIZE];
  char description[DESC_SIZE];
  HBITMAP bitmap;
  short effect;
  void (*doEffect)(void*);
};

struct CardsInHand {
  int num_cards;
  int cards[HAND_SIZE];  // "lookup table" for cards; uses deck_ref and
                         // deck_index to get randomized location
};

struct Stable {
  int unicorns[STABLE_SIZE];
  int num_unicorns;
  int size;
};

struct Player {
  struct CardsInHand hand;
  struct Stable stable;
  char username[NAME_SIZE];
  enum BabySelection icon;
  short flags;  // 00000000 00000000 plain cheese
                // & 1 = cannot play upgrade cards
                // & 2 = cannot play neigh cards
                // & 4 = cards you play cannot be neigh'd
                // & 8 = your unicorn cards cannot be destroyed
                // & 16 = your hand must be visible to all players
                // & 32 = basic unicorns can't enter your stable
                // & 64 = ginormous unicorn check
                // & 128 = all your unicorns are considered basic and have no
                // effects
                // & 256 = all your unicorns are considered pandas;
                // cards that affect unicorns don't affect yours
                // & 512 = tiny unicorn; sacrifice cards after >5 unicorns
                // & 1024 = black knight unicorn; sacrifice that card instead
                // & 2048 = brbd wre; discrd a card upon entry/exit of stable
  short filler; // padding for prev variable
};

SOCKET sockfd;
SOCKET udpfd;
extern unsigned int isclient; // 0 = server, 1 = client

struct Player player[MAX_PLAYERS];
extern int current_players;
extern unsigned int WIN_CONDITION;
extern unsigned int turn_count;

// toggles whether the deck piles are printed out or not
extern unsigned char deck_flag;
extern unsigned char discard_flag;
extern unsigned char nursery_flag;

int numinput(char*, char**, int);

// ********************************************************************************
// **************************** Pretty Color Functions ****************************
// ********************************************************************************

// https://www.codeproject.com/Tips/5255355/How-to-Put-Color-on-Windows-Console
// add: reg add HKEY_CURRENT_USER\Console /v VirtualTerminalLevel /t REG_DWORD /d 0x00000001 /f
// undo: reg add HKEY_CURRENT_USER\Console /v VirtualTerminalLevel /t REG_DWORD /d 0x00000000 /f
void red(void);
void yellow(void);
void green(void);
void cyan(void);
void blue(void);
void magenta(void);
void reset_col(void);
void rainbow(char* str);
