#pragma once
#include "basedeck.h"

// ********************************************************************************
// **************************** Print/Display Functions ***************************
// ********************************************************************************

void printPlayers(void);
void printPile(struct Deck d);
void printPileFilter(struct Deck d, int class, int species);
void printHand(int pnum);
void printStable(int pnum);
void displayCardDesc(void);
void displayDesiredStable(void);
void printStableGrid(void);

// ********************************************************************************
// ******************************* Utility Functions ******************************
// ********************************************************************************

// OPTIMIZE: use species UNICORN instead of class ANYUNICORN and then combine
// this with checking for species too
// return 0 = FALSE, the card's class does not equal the desired class
// return 1 = TRUE, card match!
int checkClass(int desired_class, int card_class);

// randomize deck between specific indices
void shuffleDeck(struct Deck* d);

// shuffles the discard pile into the deck, then shuffles the entire deck
void shuffleDiscard(void);

// shuffles the indexed card out of the deck, nursery, or discard pile's range
void rearrangePile(struct Deck* d, int index);

// shuffles the indexed card out of the hand's range
void rearrangeHand(int pnum, int index);

// shuffles the indexed card out of the stable's range
// OPTIMIZE: put toggleFlags here instead of writing it out a few times like
// when cards are stolen or returned to someone's hand?
void rearrangeStable(int pnum, int index);

// searching through the deck or discard pile for a specific card
// 0 = return failed
// 1 = return successful
int searchPile(int pnum, struct Deck* d, int class, int species);

void addNursery(struct Unicorn corn);

void addDiscard(struct Unicorn corn);

void addStable(int pnum, struct Unicorn corn);

// return card from stable to hand, or the nursery if it's a baby
void returnCardToHand(int pnum, int cindex);

// ********************************************************************************
// **************************** Boolean Check Functions ***************************
// ********************************************************************************

// check for flags that make the player immune to Neigh cards
int canBeNeighed(int pnum);

// check for flags that make the player unable to use Neigh cards
int canNeighOthers(int pnum);

// checks for all of the specific edge cases for card destruction
int canBeDestroyed(int pindex, int cindex, int class, int isMagicCard);

// returns number of available cards to destroy
int checkNumCardsToDestroy(int pnum, int class, int isMagicCard);

// checks for all of the specific edge cases for sacrificing cards
int canBeSacrificed(int pindex, int cindex, int class);

// ********************************************************************************
// ************************** Basic Card Effect Functions *************************
// ********************************************************************************

int draw(int pnum, int num_drawn);

void discard(int pnum, int num_discard, int class);

// 0 = sacrifice failed
// 1 = successful
int sacrifice(int pnum, int class);

// treat ANYUNICORN as all unicorns when checking class for Unicorn cards
void destroy(int pnum, int class, int isMagicCard);

// treat ANYUNICORN as all unicorns when checking class for Unicorn cards
void steal(int pnum, int class);

// ********************************************************************************
// *************************** Core Game Loop Functions ***************************
// ********************************************************************************

// codes the part where players are able to use an instant card against a play
// 0 = nobody used Neigh/Super Neigh or Neigh's cancelled out
// 1 = card is gone
// TODO: bug where Neigh remained in player's hand that last refuted it (e.g. 3
// neighs were used in total and when player 3 used americorn on player 1 to get
// a random card, it picked the same Neigh ID that was in the discard pile)
int refutePhase(int orig_pnum, int orig_cindex);

void playCard(int pnum);
