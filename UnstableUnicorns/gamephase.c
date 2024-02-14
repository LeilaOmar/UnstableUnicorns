#pragma once
#include "gamephase.h"
#include "networkevents.h"

// beginning turn effects all trigger at once, so any cards that enter before
// the action phase do not count towards these card effects
// (e.g. gained a card through unicorn lasso or puppicorn)
void beginningOfTurn(int pnum) {
  turn_count = 1;
  uni_lasso_flag[0] = -1;

  // Nanny Cam check
  for (int i = 0; i < current_players; i++) {
    if (i != pnum && (player[i].flags & nanny_cam) != 0)
      printHand(i);
  }

  for (int i = 0; i < player[pnum].stable.size; i++) {
    beginningTurnEffects(pnum, player[pnum].stable.unicorns[i]);
  }

  // drawing comes after the beginning turn effects
  draw(pnum, 1);
}

// choose between drawing or playing a card
void actionPhase(int pnum) {
  int index;
  char* end, buf[LINE_MAX];

  while (turn_count > 0) {
    printHand(pnum);
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
      index = numinput(buf, &end, sizeof buf);

      if (index == 3)
        displayCardDesc();
      else if (index == 4)
        displayDesiredStable();
      else if (index == 5)
        printHand(pnum);
      else if (index == 6) {
        yellow();
        printPile(nursery);
        reset_col();
      }
      else if (index == 7) {
        magenta();
        printPile(discardpile);
        reset_col();
      }
    } while (index < 1 || index > 2 || end != (buf + strlen(buf)));

    if (index == 1)
      draw(pnum, 1);
    else
      playCard(pnum);

    turn_count--;
  }

}

// end of turn sequence, so
// - discard extra cards
// - check win condition (return 1 = game over, return 0 = continue)
// - return cards to their owner (unicorn lasso)
// - give puppicorn to next player (puppicorn)
int endOfTurn(int pnum) {
  // discard extra cards since max hand limit in-game is 7
  if (player[pnum].hand.num_cards > 7) {
    discard(pnum, player[pnum].hand.num_cards - 7, ANY);
  }

  // print stuff to see what's going on
  printHand(pnum);
  printStable(pnum);

  // check if the player has enough unicorns (notably not pandas) to win the game
  if (checkWin(pnum))
    return 1;

  // return any unicorns or pass any unicorns to their proper owner
  if (uni_lasso_flag[0] != -1) {
    // uni_lasso_flag[2] represents proper owner, and the thief [1] has the
    // card removed from their stable
    int cindex = uni_lasso_flag[0];
    int thief = uni_lasso_flag[1];
    int owner = uni_lasso_flag[2];
    struct Unicorn tmp = player[thief].stable.unicorns[cindex];

    // rearrange the player's stable and toggle any flags before swapping the card to the other stable;
    // this will (hopefully) prevent dupes from card effect shenanigans
    rearrangeStable(thief, cindex);

    // send a notice to add a card to the person's stable outside of their normal turn
    if (isclient) {
      sendInt(enter_stable_event, sockfd);
      sendEnterStablePacket(tmp, owner, sockfd); // index = target player index
    
      // need to look out for nested network events
      clientEnterLeaveStable(pnum);
    }
    else {
      sendInt(enter_stable_event, clientsockfd[owner - 1]);
      sendEnterStablePacket(tmp, pnum, clientsockfd[owner - 1]); // pnum = original player index
    
      // need to look out for nested network events
      serverEnterLeaveStable(pnum, owner);
    }
  }

  // puppicorn swap; pnum isn't always equal to the current puppicorn_index[1]
  if (puppicorn_index[0] != -1) {
    int old_cindex = puppicorn_index[0]; // this gets changed in addStable
    int old_pindex = puppicorn_index[1];
    int next_pnum;
    struct Unicorn corn = player[old_pindex].stable.unicorns[old_cindex];
    rearrangeStable(old_pindex, old_cindex);

    if (old_pindex == current_players - 1) {
      next_pnum = 0;
    }
    else {
      next_pnum = old_pindex + 1;
    }

    // send a notice to add a card to the person's stable outside of their normal turn;
    // just doing addStable for them ignores effects like "Tiny Stable" or "Barbed Wire" where
    // they have to perform some action
    if (isclient) {
      sendInt(enter_stable_event, sockfd);
      sendEnterStablePacket(corn, next_pnum, sockfd); // index = target player index

      // need to look out for nested network events
      clientEnterLeaveStable(pnum);
    }
    else {
      sendInt(enter_stable_event, clientsockfd[next_pnum - 1]);
      sendEnterStablePacket(corn, pnum, clientsockfd[next_pnum - 1]); // pnum = original player index

      // need to look out for nested network events
      serverEnterLeaveStable(pnum, next_pnum);
    }
  }

  return 0;
}
