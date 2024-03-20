#pragma once
#include "gamephase.h"
#include "networkevents.h"
#include "windowsapp.h"

void BeginningOfTurn(int pnum) {
  turnCount = 1;
  uniLassoIndex[0] = -1;

  for (int i = 0; i < player[pnum].stable.size; i++) {
    Base_BeginningTurnEffects(pnum, player[pnum].stable.unicorns[i]);
  }

  // drawing comes after the beginning turn effects
  Draw(pnum, 1);
}

int ActionPhase(int pnum) {
  int tab = ACTION_TAB;
  int cindex = -2;

  while (turnCount > 0) {
    if (networkToggle & 2) {
      networkToggle ^= 2;
      cindex = SelectCard(pnum, &tab, clientPnt);
    }

    if (cindex < -1)
      return -1;
    else if (cindex >= 0)
      PlayCard(pnum, cindex);
    else
      Draw(pnum, 1);

    turnCount--;
  }

  return 0;
}

int EndOfTurn(int pnum) {
  // discard extra cards since max hand limit in-game is 7
  if (player[pnum].hand.numCards > 7) {
    Discard(pnum, player[pnum].hand.numCards - 7, ANY);
  }

  // check if the player has enough unicorns (notably not pandas) to win the game
  if (CheckWin(pnum))
    return 1;

  // return any unicorns or pass any unicorns to their proper owner
  if (uniLassoIndex[0] != -1) {
    // uniLassoIndex[2] represents the proper owner, and the thief [1] has the card removed from their stable
    int cindex = uniLassoIndex[0];
    int thief = uniLassoIndex[1];
    int owner = uniLassoIndex[2];
    struct Unicorn tmp = player[thief].stable.unicorns[cindex];

    // rearrange the player's stable and toggle any flags before swapping the card to the other stable;
    // this will (hopefully) prevent dupes from card effect shenanigans
    RearrangeStable(thief, cindex);

    // send a notice to add a card to the person's stable outside of their normal turn
    if (isClient) {
      SendInt(ENTER_STABLE_EVENT, sockfd);
      SendEnterStablePacket(tmp, owner, sockfd); // index = target player index
    
      // need to look out for nested network events
      ClientEnterLeaveStable(pnum);
    }
    else {
      SendInt(ENTER_STABLE_EVENT, clientsockfd[owner - 1]);
      SendEnterStablePacket(tmp, pnum, clientsockfd[owner - 1]); // pnum = original player index
    
      // need to look out for nested network events
      ServerEnterLeaveStable(pnum, owner);
    }
  }

  // puppicorn swap; pnum isn't always equal to the current puppicornIndex[1]
  if (puppicornIndex[0] != -1) {
    int old_cindex = puppicornIndex[0]; // this gets changed in AddStable
    int old_pindex = puppicornIndex[1];
    int next_pnum;
    struct Unicorn corn = player[old_pindex].stable.unicorns[old_cindex];
    RearrangeStable(old_pindex, old_cindex);

    if (old_pindex == currentPlayers - 1) {
      next_pnum = 0;
    }
    else {
      next_pnum = old_pindex + 1;
    }

    // send a notice to add a card to the person's stable outside of their normal turn;
    // just doing AddStable for them ignores effects like "Tiny Stable" or "Barbed Wire" where
    // they have to perform some action
    if (isClient) {
      SendInt(ENTER_STABLE_EVENT, sockfd);
      SendEnterStablePacket(corn, next_pnum, sockfd); // index = target player index

      // need to look out for nested network events
      ClientEnterLeaveStable(pnum);
    }
    else {
      SendInt(ENTER_STABLE_EVENT, clientsockfd[next_pnum - 1]);
      SendEnterStablePacket(corn, pnum, clientsockfd[next_pnum - 1]); // pnum = original player index

      // need to look out for nested network events
      ServerEnterLeaveStable(pnum, next_pnum);
    }
  }

  return 0;
}
