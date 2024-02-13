#pragma once
#include "networkfuncs.h"
#include "networkstates.h"

// codes the part where players are able to use an instant card against a play
// 0 = nobody used Neigh/Super Neigh cards, or Neigh cards cancelled out
// 1 = card is gone
// 
// TODO: (unsure if this bug carried through from ye old refutePhase circa 2022)
// bug where Neigh remained in player's hand that last refuted it (e.g. 3
// neighs were used in total and when player 3 used americorn on player 1 to get
// a random card, it picked the same Neigh ID that was in the discard pile)
int clientNeigh(int clientpnum, int orig_pnum, int *orig_cindex);
int serverNeigh(int orig_pnum, int *orig_cindex);

void clientDiscard(int clientpnum, int target_pnum, int cType);
void serverDiscard(int orig_pnum, int target_pnum, int cType);

void clientSacrifice(int clientpnum, int target_pnum, int cType);
void serverSacrifice(int orig_pnum, int target_pnum, int cType);

void clientDestroyEffect(int clientpnum);
void serverDestroyEffect(int orig_pnum, int target_pnum);

void clientEnterStable(int clientpnum);
void serverEnterStable(int orig_pnum, int target_pnum);
