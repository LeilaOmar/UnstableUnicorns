#include <conio.h>
#include "globals.h"
#include "basedeck.h"

int isClient; //!< 0 = server, 1 = client

struct Deck deck;
struct Deck nursery;
struct Deck discardpile;
struct Player player[MAX_PLAYERS];
int currentPlayers = 1; //!< the host is a player too! :)
int turnCount = 0;      //!< number of moves during the action phase
int WIN_CONDITION = 7;  //!< usually 7, but can be 6 with 6-8 players

FILE *fpinput; //!< stand-in for stdin in case it needs to read from a file instead

int NumInput(char *buf, char **end, int size) {
  int index;

  fgets(buf, size, fpinput);
  buf[strlen(buf) - 1] = 0;
  index = strtol(buf, end, 10);

  return index;
}

char CharInput(char *buf, int size) {
  char c;

  fgets(buf, size, fpinput);
  c = buf[0];

  return c;
}

void InitDeck(struct Deck *nursery, struct Deck *deck, struct Deck *discard) {
  int counter = 0;

  nursery->cards = malloc(NURSERY_SIZE * sizeof(struct Unicorn));
  nursery->size = NURSERY_SIZE;

  if (nursery->cards == NULL) {
    perror(NULL);
    exit(1);
  }

  for (int i = 0; i < NURSERY_SIZE; i++) {
    nursery->cards[i] = Base_DECK[i];
    nursery->cards[i].id = counter;
    counter++;
  }

  deck->cards = malloc(DECK_SIZE * sizeof(struct Unicorn));
  deck->size = DECK_SIZE;

  if (deck->cards == NULL) {
    perror(NULL);
    exit(1);
  }

  for (int i = 0; i < DECK_SIZE; i++) {
    deck->cards[i] = Base_DECK[i + NURSERY_SIZE];
    deck->cards[i].id = counter;
    counter++;
  }

  discard->cards = malloc(DECK_SIZE * sizeof(struct Unicorn));
  discard->size = 0;

  if (discard->cards == NULL) {
    perror(NULL);
    exit(1);
  }
}

void Cleanup(void) {
  // hangs on exit until the user inputs something
  (void)_getch();

  if (!isClient) {
    for (int i = 0; i < currentPlayers - 1; i++) {
      closesocket(clientsockfd[i]);
    }
  }

  if (sockfd != INVALID_SOCKET)
    closesocket(sockfd);

  free(nursery.cards);
  free(deck.cards);
  free(discardpile.cards);

  WSACleanup();
}

// ********************************************************************************
// **************************** Pretty Color Functions ****************************
// ********************************************************************************

// https://www.codeproject.com/Tips/5255355/How-to-Put-Color-on-Windows-Console
// add: reg add HKEY_CURRENT_USER\Console /v VirtualTerminalLevel /t REG_DWORD /d 0x00000001 /f
// undo: reg add HKEY_CURRENT_USER\Console /v VirtualTerminalLevel /t REG_DWORD /d 0x00000000 /f
void Red(void) { printf("\033[1;31m"); }
void Yellow(void) { printf("\033[1;33m"); }
void Green(void) { printf("\033[1;32m"); }
void Cyan(void) { printf("\033[1;36m"); }
void Blue(void) { printf("\033[1;34m"); }
void Magenta(void) { printf("\033[1;35m"); }
void ResetCol(void) { printf("\033[0m"); }

void Rainbow(char *str) {
  unsigned int index = 0;
  for (size_t c = 0; c < strlen(str); c++) {
    if (index % 6 == 0) Red();
    else if (index % 6 == 1) Yellow();
    else if (index % 6 == 2) Green();
    else if (index % 6 == 3) Cyan();
    else if (index % 6 == 4) Blue();
    else Magenta();

    printf("%c", str[c]);

    if (str[c] != '\n' && str[c] != ' ') index++;
  }
  ResetCol();
}
