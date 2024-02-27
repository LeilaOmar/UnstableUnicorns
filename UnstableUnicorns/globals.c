#include <conio.h>
#include "globals.h"
#include "basedeck.h"
#include "networkstates.h"

int isClient; //!< 0 = server, 1 = client

struct Deck deck;
struct Deck nursery;
struct Deck discardpile;
struct Player player[MAX_PLAYERS];
int currentPlayers = 1; //!< the host is a player too! :)
int turnCount = 0;      //!< number of moves during the action phase
int WIN_CONDITION = 7;  //!< usually 7, but can be 6 with 6-8 players

// ********************************************************************************
// ******************************** Input Functions *******************************
// ********************************************************************************

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

// ********************************************************************************
// ******************************* Logging Functions ******************************
// ********************************************************************************

int isLog = 0;
FILE *fplog; //!< file descriptor for the log file

void SetLogFD(char *filename) {
  fopen_s(&fplog, filename, "w");

  if (fplog == NULL) {
    fprintf(stderr, "ERROR: Could not open/create the log file :(\n");
    isLog = 0;
  }
}

void LogMove(int pnum, struct Unicorn corn, char *move) {
  if (strcmp(move, "play") == 0) {
    fprintf(fplog, "\n\nplayer[%d] is trying to play the card %s [ID: %d]\n", pnum, corn.name, corn.id);
  }
  else if (strcmp(move, "neigh") == 0) {
    fprintf(fplog, "\n\nplayer[%d] is trying to Neigh the last played card\n", pnum);
  }
  else if (strcmp(move, "discard") == 0) {
    fprintf(fplog, "\n\nplayer[%d] discarded the card %s [ID: %d]\n", pnum, corn.name, corn.id);
  }
  else if (strcmp(move, "sacrifice") == 0) {
    fprintf(fplog, "\n\nplayer[%d] sacrificed the card %s [ID: %d]\n", pnum, corn.name, corn.id);
  }
  else {
    fprintf(fplog, "\n\nMove type error: player[%d] did something with the card %s [ID: %d]\n", pnum, corn.name, corn.id);
  }
}

void LogPlayer(int pnum) {
  fprintf(fplog, "\nplayer[%d]: %s", pnum, player[pnum].username);

  char shortbits[18]; //!< num bits + num spaces in between bytes + NULL = (sizeof(short) * 8) + (sizeof(short) - 1) + 1
  int count = 0;
  for (int k = 2; k > 0; k--) {
    for (int j = 7; j >= 0; j--) {
      shortbits[count] = ((player[pnum].flags >> (((k - 1) * 8) + j)) & 1) ? '1' : '0';
      count++;
    }
    shortbits[count] = ' ';
    count++;
  }
  shortbits[count - 1] = '\0';
  fprintf(fplog, "\n    flags = %s", shortbits);

  fprintf(fplog, "\n    numCards = %d", player[pnum].hand.numCards);
  for (int j = 0; j < player[pnum].hand.numCards; j++) {
    fprintf(fplog, "\n        %s [ID: %d]", player[pnum].hand.cards[j].name, player[pnum].hand.cards[j].id);
  }

  fprintf(fplog, "\n    numUnicorns = %d"
    "\n    size = %d", player[pnum].stable.numUnicorns, player[pnum].stable.size);
  for (int j = 0; j < player[pnum].stable.size; j++) {
    fprintf(fplog, "\n        %s [ID: %d]", player[pnum].stable.unicorns[j].name, player[pnum].stable.unicorns[j].id);
  }
}

void LogGameData(int pnum, int state) {
  if (state == END_TURN) {
    fprintf(fplog, "\n\n***** End of player[%d]'s Turn *****\n", pnum);
  }
  else if (state == START_GAME) {
    fprintf(fplog, "***** Game Start! *****");
  }
  else if (state == END_GAME) {
    if (pnum != -1) {
      fprintf(fplog, "\n\n***** Game End: %s (player[%d]) won the game! *****\n", player[pnum].username, pnum);
    }
    else {
      fprintf(fplog, "\n\n***** Game End: Tie! Nobody Wins *****");
    }
  }

  for (int i = 0; i < currentPlayers; i++) {
    LogPlayer(i);
  }

  fprintf(fplog, "\nPuppicorn:"
    "\n    puppicornIndex[1] (owner) = %d"
    "\n    puppicornIndex[0] (index) = %d", puppicornIndex[1], puppicornIndex[0]);

#ifdef SHOWDECK
  fprintf(fplog, "\nDeck:");
  for (int i = 0; i < deck.size; i++) {
    fprintf(fplog, "\n    %03d. %s [ID: %d]", i + 1, deck.cards[i].name, deck.cards[i].id);
  }
#endif

  fprintf(fplog, "\nNursery:");
  for (int i = 0; i < nursery.size; i++) {
    fprintf(fplog, "\n    %02d. %s [ID: %d]", i + 1, nursery.cards[i].name, nursery.cards[i].id);
  }

  fprintf(fplog, "\nDiscard Pile:");
  for (int i = 0; i < discardpile.size; i++) {
    fprintf(fplog, "\n    %02d. %s [ID: %d]", i + 1, discardpile.cards[i].name, discardpile.cards[i].id);
  }
}

// ********************************************************************************
// ***************************** Init/De-Init Functions ***************************
// ********************************************************************************

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

  if (isLog) fclose(fplog);

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
