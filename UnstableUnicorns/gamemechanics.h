#pragma once
#include "basedeck.h"

// ********************************************************************************
// **************************** Print/Display Functions ***************************
// ********************************************************************************

void printPlayers(void);
void printPile(struct Deck d);
void printPileFilter(struct Deck d, int cType, int species);
void printHand(int pnum);
void printStable(int pnum);
void displayCardDesc(void);
void displayDesiredStable(void);
void printStableGrid(void); // prints stables of all players in two columns

// ********************************************************************************
// ******************************* Utility Functions ******************************
// ********************************************************************************

// return 0 = FALSE, the card's type does not equal the desired type
// return 1 = TRUE, card match!
int checkType(int desired_type, int card_type);

// returns 1 if the player satisfies the win condition
int checkWin(int pnum);

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
int searchPile(int pnum, struct Deck* d, int cType, int species);

void addNursery(struct Unicorn corn);

void addDiscard(struct Unicorn corn);

void addStable(int pnum, struct Unicorn corn);

// return card from stable to hand, or the nursery if it's a baby
void returnCardToHand(int pnum, int cindex);

// utility tie-breaker func for games that end early
// return [0-7] == winning player index
// return    -1 == complete tie, nobody wins
int tiebreaker();

// ********************************************************************************
// **************************** Boolean Check Functions ***************************
// ********************************************************************************

// check for flags that make the player immune to Neigh cards
int canBeNeighed(int pnum);

// check for flags that make the player unable to use Neigh cards
int canNeighOthers(int pnum);

// checks for all of the specific edge cases for card destruction
int canBeDestroyed(int pindex, int cindex, int cType, int isMagicCard);

// returns number of available cards to destroy
int checkNumCardsToDestroy(int pnum, int cType, int isMagicCard);

// checks for all of the specific edge cases for sacrificing cards
int canBeSacrificed(int pindex, int cindex, int cType);

// ********************************************************************************
// ************************** Basic Card Effect Functions *************************
// ********************************************************************************

int draw(int pnum, int num_drawn);

void discard(int pnum, int num_discard, int cType);

// 0 = sacrifice failed
// 1 = successful
int sacrifice(int pnum, int cType);

// treat ANYUNICORN as all unicorns when checking cType for Unicorn cards
void destroy(int pnum, int cType, int isMagicCard);

// treat ANYUNICORN as all unicorns when checking cType for Unicorn cards
void steal(int pnum, int cType);

// ********************************************************************************
// *************************** Core Game Loop Functions ***************************
// ********************************************************************************

void playCard(int pnum);

void endGame(int winningpnum);
