#include <basedeck.h>
#include "resetfns.h"

// ********************************************************************************
// *************************** Save and Reset Functions ***************************
// ********************************************************************************

void reset_players(void) {
  for (int i = 0; i < currentPlayers; i++) {
    player[i].hand.numCards = 0;
    player[i].stable.numUnicorns = 0;
    player[i].stable.size = 0;
    player[i].flags = 0;
    memset(player[i].stable.unicorns, 0, sizeof player[i].stable.unicorns);
    memset(player[i].hand.cards, 0, sizeof player[i].hand.cards);
  }

  currentPlayers = 1;
  moveCount = 1;
}

void reset_nursery(void) {
  int counter = 0;

  for (int i = 0; i < NURSERY_SIZE; i++) {
    nursery.cards[i] = Base_DECK[i];
    nursery.cards[i].id = counter;
    counter++;
  }
  nursery.size = NURSERY_SIZE;
}

void reset_deck(void) {
  int counter = NURSERY_SIZE;

  for (int i = 0; i < DECK_SIZE; i++) {
    deck.cards[i] = Base_DECK[i + NURSERY_SIZE];
    deck.cards[i].id = counter;
    counter++;
  }
  deck.size = DECK_SIZE;
}

void reset_discard(void) {
  memset(discardpile.cards, 0, DECK_SIZE);
  discardpile.size = 0;
}

void rainbow_error(char *str) {
  unsigned int index = 0;
  for (size_t c = 0; c < strlen(str); c++) {
    if (index % 6 == 0) Red();
    else if (index % 6 == 1) Yellow();
    else if (index % 6 == 2) Green();
    else if (index % 6 == 3) Cyan();
    else if (index % 6 == 4) Blue();
    else Magenta();

    fprintf(stderr, "%c", str[c]);

    if (str[c] != '\n' && str[c] != ' ') index++;
  }
  ResetCol();
}
