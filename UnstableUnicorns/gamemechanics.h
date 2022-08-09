#pragma once
#include "globals.h"

// ******************** Utility Functions ********************

void printPlayers(void);
void printNursery(size_t start, size_t size);
void printDeck(size_t start, size_t size, int class, int species);
void printDiscard(int class);
void printHand(int pnum);
void printStable(int pnum);
void displayCardDesc(void);
void displayDesiredStable(void);

// OPTIMIZE: use species UNICORN instead of class ANYUNICORN and then combine
// this with checking for species too
// returns 1 if the desired class matches with the card's class, otherwises
// return 0
int checkClass(int desired_class, int card_class);

// randomize deck between specific indeces
void shuffleDeck(int start, int size);

// shuffles the indexed car out of the nursery's range
// (so the nursery_index only keeps track of the cards within
// nursery_ref that are still inside the nursery and not taken out)
void rearrangeNursery(int index);

// nursery_index points to the top most card of the ones available in the
// nursery at the time. this function basically lowers the index by one and
// overwrites previous data to include the newly added card. order doesn't
// matter
void addNursery(int cardid);

void addStable(int pnum, int cardid);

// shuffles the indexed card out of the hand's range
void rearrangeHand(int pnum, int index);

// shuffles the indexed card out of the stable's range
// OPTIMIZE: put toggleFlags here instead of writing it out a few times like
// when cards are stolen or returned to someone's hand?
void rearrangeStable(int pnum, int index);

// OPTIMIZE: combine this with rearrangeNursery for cleanup purposes; basically it
// is the same thing (except the indexes are arranged differently?)
void rearrangeDiscard(int index);

// shuffles the indexed card out of the deck's range
void rearrangeDeck(int index);

// shuffles the discard pile into the deck, then shuffles the entire deck
void shuffleDiscard(void);

// searching through the discard pile for a specific card
void searchDiscard(int pnum, int class);

// searching through the deck for a specific card, then shuffles the deck
void searchDeck(int pnum, int class, int species);

// ******************** Card Effect Functions ********************
void draw(int pnum, int num_drawn);

void discard(int pnum, int num_discard, int class);

void sacrifice(int pnum, int class);

// treat ANYUNICORN as all unicorns when checking class for Unicorn cards
void destroy(int pnum, int class);

// treat ANYUNICORN as all unicorns when checking class for Unicorn cards;
// this is for edge cases where Magic cards are being used to destroy cards
// that cannot be destroyed by those specifically
void destroyMagic(int pnum, int class);

// treat ANYUNICORN as all unicorns when checking class for Unicorn cards
void steal(int pnum, int class);

// ******************** Misc Game Mechanics ********************

// codes the part where players are able to use an instant card against a play
// 0 = nobody used Neigh/Super Neigh or Neigh's cancelled out; 1 = card is gone
// TODO: bug where Neigh remained in player's hand that last refuted it (e.g. 3
// neighs were used in total and when player 3 used americorn on player 1 to get
// a random card, it picked the same Neigh ID that was in the discard pile)
int refutePhase(int pnum, int cindex);

// switch cases for cards that may trigger specific player flags (e.g. cannot
// neigh)
void toggleFlags(int pnum, int effect);

// switch cases for when a card has a special effect upon being sacrificed or
// destroyed
void sacrificeDestroyEffects(int pnum, int cindex, int effect);

// switch cases for cards that may be unable to be played due to certain
// circumstances; this is separated so that the hand is not rearranged
// unnecessarily; 1 means success, 0 means it returned prematurely, 2 means
// the effect isn't here
int conditionalEffects(int pnum, int effect, int cardid, int hindex, int upgrade_target);

// switch cases for enter your stable effects with magical unicorn cards
void enterStableEffects(int pnum, int effect);

// switch cases for Magic cards
void magicEffects(int pnum, int effect);

// switch cases for beginning of your turn effects
void beginningTurnEffects(int pnum, int effect);

void playCard(int pnum);