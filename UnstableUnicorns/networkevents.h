#pragma once
#include "networkfuncs.h"
#include "networkstates.h"

// codes the part where players are able to use an instant card against a play
// 0 = nobody used Neigh/Super Neigh cards, or Neigh cards cancelled out
// 1 = card is gone
int clientNeigh(int clientpnum, int orig_pnum, int *orig_cindex);
int serverNeigh(int orig_pnum, int *orig_cindex);

void clientDiscard(int clientpnum, int target_pnum, int cType);
void serverDiscard(int orig_pnum, int target_pnum, int cType);

void clientSacrifice(int clientpnum, int target_pnum, int cType);
void serverSacrifice(int orig_pnum, int target_pnum, int cType);

// idle function that handles nested network events;
// this works for both enter stable effects and unicorn sacrificeDestroy effects
void clientEnterLeaveStable(int clientpnum);

// idle function that handles nested network events;
// this works for both enter stable effects and unicorn sacrificeDestroy effects
void serverEnterLeaveStable(int orig_pnum, int target_pnum);
