#pragma once
#include "gamephase.h"

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
  printHand(pnum);
}

// choose between drawing or playing a card
// OPTIMIZE: skip go straight to drawing another card in case all cards in
// hand are impossible plays (e.g. you MUST [xxx] when there are no
// available cards to be affected)
void actionPhase(int pnum) {
  int index;
  char* end, buf[LINE_MAX];

  while (turn_count > 0) {
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

  // cycle between players; count after checking number of unicorns
  // maybe just check if the number is >= 7 every time a card enters your
  // stable; ginormous unicorn counts for 2 unless blinding light is in effect
  // pandamonium means you have 7+ pandas, not unicorns
  // TODO: player won with this stable??? only 6 unicorns; tested it in a different
  // game and rainbow mane bringing in cards did not make the game end prematurely
  // so maybe there is some kind of issue with accidentally using the wrong effect
  // that would trigger the ginormous unicorn flag?
  //  1. Baby Unicorn(Rainbow) [ID:10]
  //  2. Basic Unicorn(Green) [ID:23]
  //  3. Basic Unicorn(Purple) [ID:32]
  //  4. Rainbow Mane [ID:94]
  //  5. Basic Unicorn(Blue) [ID:26]
  //  6. Alluring Narwhal [ID:63]
  //  7. Black Knight Unicorn [ID:59]
  // future me: this could have been triggered from a multitude of things LOL, but
  // maybe shark with a horn triggered it because it never decreased the number of unicorns
  // in the stable...
  // another possibility is that blinding light skipped the ginormous unicorn toggle, and
  // when the unicorn got removed, it mistakenly toggled the flag true instead of false
  if ((player[pnum].stable.num_unicorns >= WIN_CONDITION ||
    (player[pnum].stable.num_unicorns >= (WIN_CONDITION - 1) &&
      (player[pnum].flags & (ginormous_unicorn | blinding_light)) == ginormous_unicorn)) &&
    (player[pnum].flags & pandamonium) == 0)
    return 1;

  // return any unicorns or pass any unicorns to their proper owner
  if (uni_lasso_flag[0] != -1) {
    // uni_lasso_flag[2] represents proper owner, and the thief [1] has the
    // card removed from their stable
    int owner = uni_lasso_flag[2];
    int thief = uni_lasso_flag[1];
    int cindex = uni_lasso_flag[0];
    struct Unicorn tmp = player[thief].stable.unicorns[cindex];

    // rearrange the player's stable and toggle any flags before swapping the card to the other stable;
    // this will (hopefully) prevent dupes from card effect shenanigans
    rearrangeStable(thief, cindex);

    addStable(owner, tmp);
    enterStableEffects(owner, tmp.effect);
  }

  // puppicorn swap
  if (puppicorn_index[0] != -1) {
    if (pnum == current_players - 1) {
      addStable(0, player[pnum].stable.unicorns[puppicorn_index[0]]);
      rearrangeStable(pnum, puppicorn_index[0]);
      puppicorn_index[0] = player[0].stable.size - 1;
    }
    else {
      // puppicorn should be removed first before adding it to the stable
      addStable(pnum + 1, player[pnum].stable.unicorns[puppicorn_index[0]]);
      rearrangeStable(pnum, puppicorn_index[0]);
      puppicorn_index[0] = player[pnum + 1].stable.size - 1;
    }
  }

  return 0;
}
