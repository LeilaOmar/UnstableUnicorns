#pragma once
#include "gamephase.h"
#include "networkevents.h"

void BeginningOfTurn(int pnum) {
  turnCount = 1;
  uniLassoIndex[0] = -1;

  // Nanny Cam check
  for (int i = 0; i < currentPlayers; i++) {
    if (i != pnum && (player[i].flags & NANNY_CAM) != 0)
      PrintHand(i);
  }

  for (int i = 0; i < player[pnum].stable.size; i++) {
    Base_BeginningTurnEffects(pnum, player[pnum].stable.unicorns[i]);
  }

  // drawing comes after the beginning turn effects
  Draw(pnum, 1);
}

void ActionPhase(int pnum) {
  int index;
  char *end, buf[LINE_MAX];

  while (turnCount > 0) {
    PrintHand(pnum);
    do {
      printf(
        "\nAction phase options:"
        "\n1. Draw a card"
        "\n2. Play a card"
        "\n3. Display card description"
        "\n4. Display a player's stable"
        "\n5. Display your own hand"
        "\n6. Display Nursery"
        "\n7. Display Discard Pile"
        "\nChoice: ");
      index = NumInput(buf, &end, sizeof buf);

      if (index == 3)
        DisplayCardDesc();
      else if (index == 4)
        DisplayDesiredStable();
      else if (index == 5)
        PrintHand(pnum);
      else if (index == 6) {
        Yellow();
        PrintPile(nursery);
        ResetCol();
      }
      else if (index == 7) {
        Magenta();
        PrintPile(discardpile);
        ResetCol();
      }
    } while (index < 1 || index > 2 || end != (buf + strlen(buf)));

    if (index == 1)
      Draw(pnum, 1);
    else
      PlayCard(pnum);

    turnCount--;
  }

}

int EndOfTurn(int pnum) {
  // discard extra cards since max hand limit in-game is 7
  if (player[pnum].hand.numCards > 7) {
    Discard(pnum, player[pnum].hand.numCards - 7, ANY);
  }

  // print stuff to see what's going on
  PrintHand(pnum);
  PrintStable(pnum);

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
