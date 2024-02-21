#include "gamemechanics.h"
#include "networkevents.h"

// ********************************************************************************
// **************************** Print/Display Functions ***************************
// ********************************************************************************

void PrintPlayers(void) {
  puts("List of players:");
  for (int i = 0; i < currentPlayers; i++) {
    printf("    %d: %s\n", i + 1, player[i].username);
  }
}

void PrintPile(struct Deck d) {
  for (int i = 0; i < d.size; i++) {
    printf("%d: %s [ID: %d]\n", i + 1, d.cards[i].name, d.cards[i].id);
  }
}

void PrintPileFilter(struct Deck d, int cType, int species) {
  for (int i = 0; i < d.size; i++) {
    if ((species == ANY || species == d.cards[i].species) &&
        CheckType(cType, d.cards[i].cType))
    printf("%d: %s [ID: %d]\n", i + 1, d.cards[i].name, d.cards[i].id);
  }
}

void PrintHand(int pnum) {
  printf("\n%s's hand:\n", player[pnum].username);
  for (int i = 0; i < player[pnum].hand.numCards; i++) {
    printf("    %d. %s [ID: %d]\n", i + 1, player[pnum].hand.cards[i].name, player[pnum].hand.cards[i].id);
  }
}

void PrintStable(int pnum) {
  printf("\n%s's stable:\n", player[pnum].username);
  for (int i = 0; i < player[pnum].stable.size; i++) {
    if (player[pnum].stable.unicorns[i].cType == UPGRADE)
      Green();
    else if (player[pnum].stable.unicorns[i].cType == DOWNGRADE)
      Yellow();

    printf("    %d. %s [ID: %d]\n", i + 1, player[pnum].stable.unicorns[i].name, player[pnum].stable.unicorns[i].id);
    ResetCol();
  }
}

// currently isn't scalable to multiple decks
void DisplayCardDesc() {
  int index;
  char *end, buf[LINE_MAX];

  do {
    printf("Enter the card ID of the card you wish to display the description of: ");
    index = NumInput(buf, &end, sizeof buf);
  } while (index < 0 || index >= DECK_SIZE + NURSERY_SIZE || end != (buf + strlen(buf)));

  Cyan();
  printf("Card: %s\nDescription: %s\n", Base_DECK[index].name, Base_DECK[index].description);
  // if (index < NURSERY_SIZE)
  //   printf("Card: %s\nDescription: %s\n", nursery.cards[index].name, nursery.cards[index].description);
  // else
  //   printf("Card: %s\nDescription: %s\n", deck.cards[index].name, deck.cards[index].description);
  ResetCol();
}

void DisplayDesiredStable(void) {
  int index;
  char *end, buf[LINE_MAX];

  PrintPlayers();
  do {
    printf("Choose the index associated with the player whose stable you'd like to see: ");
    index = NumInput(buf, &end, sizeof buf) - 1;
  } while (index < 0 || index >= currentPlayers || end != (buf + strlen(buf)));

  PrintStable(index);
}

// prints stables of all players in two columns
void PrintStableGrid(void) {
  char gridbuf[50] = { 0 };
  char spacebuf[50] = { 0 };
  int colsize = 48;

  for (int k = 0; k < currentPlayers; k += 2) {
    printf("\n");

    printf("%s's stable", player[k].username);
    memset(spacebuf, ' ', colsize - strlen(player[k].username) - 7);
    printf("%s", spacebuf);
    memset(spacebuf, '\0', sizeof spacebuf);

    if ((k + 1) < currentPlayers)
    {
      printf("%s's stable", player[k + 1].username);
      memset(spacebuf, ' ', colsize - strlen(player[k + 1].username) - 7);
      printf("%s", spacebuf);
      memset(spacebuf, '\0', sizeof spacebuf);
    }

    int large = max(player[k].stable.size, player[k + 1].stable.size);

    printf("\n");
    for (int i = 0; i < large; i++) {
      if (player[k].stable.size > i) {
        if (player[k].stable.unicorns[i].cType == UPGRADE)
          Green();
        else if (player[k].stable.unicorns[i].cType == DOWNGRADE)
          Yellow();

        sprintf_s(gridbuf, sizeof gridbuf, "    %d. %s [ID: %d]", i + 1, player[k].stable.unicorns[i].name, player[k].stable.unicorns[i].id);
        memset(spacebuf, ' ', colsize - strlen(gridbuf));
        printf("%s%s", gridbuf, spacebuf);
        memset(gridbuf, '\0', sizeof gridbuf);
        memset(spacebuf, '\0', sizeof spacebuf);

        ResetCol();
      }
      else {
        memset(spacebuf, ' ', colsize);
        printf("%s", spacebuf);
      }

      if ((k + 1) < currentPlayers && player[k + 1].stable.size > i)
      {
        if (player[k + 1].stable.unicorns[i].cType == UPGRADE)
          Green();
        else if (player[k + 1].stable.unicorns[i].cType == DOWNGRADE)
          Yellow();
        printf("    %d: %s [ID: %d]", i + 1, player[k + 1].stable.unicorns[i].name, player[k + 1].stable.unicorns[i].id);
        ResetCol();
      }

      memset(spacebuf, '\0', sizeof spacebuf);
      printf("\n");
    }
  }
}

// ********************************************************************************
// ******************************* Utility Functions ******************************
// ********************************************************************************

int CheckType(int desiredType, int cardType) {
  if (desiredType == ANY || desiredType == cardType ||
    (desiredType == ANYUNICORN &&
      (cardType == BABYUNICORN || cardType == BASICUNICORN ||
        cardType == MAGICALUNICORN))) {
    return 1;
  }
  return 0;
}

int CheckWin(int pnum) {
  if ((player[pnum].stable.numUnicorns >= WIN_CONDITION ||
    (player[pnum].stable.numUnicorns >= (WIN_CONDITION - 1) &&
      (player[pnum].flags & (GINORMOUS_UNICORN | BLINDING_LIGHT)) == GINORMOUS_UNICORN)) &&
    (player[pnum].flags & PANDAMONIUM) == 0)
    return 1;

  return 0;
}

void ShuffleDeck(struct Deck *d) {
  struct Unicorn tmp;

  for (int i = d->size - 1; i > 0; i--) {
    int j = rand() % (i + 1);
    tmp = d->cards[i];
    d->cards[i] = d->cards[j];
    d->cards[j] = tmp;
  }
  
#ifdef SHOWDECK
    Blue();
    PrintPile(*d);
    ResetCol();
#endif
}

void ShuffleDiscard(void) {
  for (int i = discardpile.size - 1; i >= 0; i--) {
    deck.cards[deck.size] = discardpile.cards[i];
    deck.size++;
    discardpile.size--;
  }

  ShuffleDeck(&deck);

  Blue();
  PrintPile(deck);
  ResetCol();
}

void RearrangePile(struct Deck *d, int index) {
  struct Unicorn tmp;

  if (index < 0 || index >= d->size)
    return;

  for (int i = index; i < d->size - 1; i++) {
    tmp = d->cards[i];
    d->cards[i] = d->cards[i + 1];
    d->cards[i + 1] = tmp;
  }

  d->size--;
}

void RearrangeHand(int pnum, int index) {
  struct Unicorn tmp;

  for (int i = index; i < player[pnum].hand.numCards - 1; i++) {
    tmp = player[pnum].hand.cards[i];
    player[pnum].hand.cards[i] = player[pnum].hand.cards[i + 1];
    player[pnum].hand.cards[i + 1] = tmp;
  }
  player[pnum].hand.numCards--;
}

void RearrangeStable(int pnum, int index) {
  struct Unicorn tmp;

  // toggle special flags off
  Base_ToggleFlags(pnum, player[pnum].stable.unicorns[index].effect);

  if (player[pnum].stable.unicorns[index].species != NOSPECIES) {
    player[pnum].stable.numUnicorns--;

    // barbed wire check; this doesn't trigger for pandas when leaving the stable unlike
    // in the add function when they enter (since they enter as unicorns before "transforming")
    if ((player[pnum].flags & BARBED_WIRE) != 0 && (player[pnum].flags & PANDAMONIUM) == 0) {
      Discard(pnum, 1, ANY);
    }
  }

  // unicorn lasso check; make sure this gets disabled if the card leaves the stable in any way
  // before the end of the turn (e.g. it gets sacrificed/destroyed, returned to hand, swapped, etc.).
  // this is so that it doesn't return an incorrect card (or effect) back to the victim
  if (pnum == uniLassoIndex[1]) {
    if (index == uniLassoIndex[0])
      uniLassoIndex[0] = -1;
    else if (index < uniLassoIndex[0]) {
      uniLassoIndex[0]--;
    } // if index is greater than the unicorn_lasso_index, then it doesn't affect the position of the card
  }

  // puppicorn check; make sure it shifts down by 1 if the index that got
  // deleted is less than puppicornIndex
  if (puppicornIndex[1] == pnum) {
    if (puppicornIndex[0] > index)
      puppicornIndex[0]--;
    // theoretically don't need to check if the puppicorn index is the same as the selected card index
    // because it should never get triggered like this, but it shall stay in just in case
    else if (puppicornIndex[0] == index) {
      puppicornIndex[0] = -1;
    }
  }

  for (int i = index; i < player[pnum].stable.size - 1; i++) {
    tmp = player[pnum].stable.unicorns[i];
    player[pnum].stable.unicorns[i] = player[pnum].stable.unicorns[i + 1];
    player[pnum].stable.unicorns[i + 1] = tmp;
  }
  player[pnum].stable.size--;
}

int SearchPile(int pnum, struct Deck *d, int cType, int species) {
  int index, count = 0;
  char *end, buf[LINE_MAX];

  // check if there are actually available cards to take
  for (int i = 0; i < d->size; i++) {
    if ((species == ANY || species == d->cards[i].species) &&
        CheckType(cType, d->cards[i].cType))
      count++;
  }

  // no valid cards are available
  if (count == 0)
    return 0;

  Blue();
  PrintPileFilter(*d, cType, species);
  ResetCol();

  for (;;) {
    printf("Pick a valid card number to add to your hand: ");
    index = NumInput(buf, &end, sizeof buf) - 1;

    // index validation
    if (index < 0 || index >= d->size || end != (buf + strlen(buf)))
      continue;

    if ((species == ANY || species == d->cards[index].species) &&
        CheckType(cType, d->cards[index].cType))
      break;
  }

  player[pnum].hand.cards[player[pnum].hand.numCards++] = d->cards[index];
  RearrangePile(d, index);

  return 1;
}

void AddNursery(struct Unicorn corn) {
  if (nursery.size == NURSERY_SIZE) {
    // the nursery is already full, so some cards must've been duplicated for this to happen?
    // the cardid check already takes place before this function
    return;
  }
  nursery.cards[nursery.size] = corn;
  nursery.size++;
}

void AddDiscard(struct Unicorn corn) {
  if (discardpile.size == DECK_SIZE) {
    // this seems impossible unless there's a card that returns everyone's hand to the discard
    // pile, AND everyone's stables are empty, AND the deck is empty; if the deck was empty then
    // it's game over anyways
    return;
  }
  discardpile.cards[discardpile.size] = corn;
  discardpile.size++;
}

void AddStable(int pnum, struct Unicorn corn) {
  player[pnum].stable.unicorns[player[pnum].stable.size] = corn;
  player[pnum].stable.size++;

  if (corn.species != NOSPECIES) {
    player[pnum].stable.numUnicorns++;

    // barbed wire check
    if ((player[pnum].flags & BARBED_WIRE) != 0) {
      Discard(pnum, 1, ANY);
    }
  }

  // trigger the enter stable effects and apply any flag changes
  // this goes before the tiny stable trigger just to allow for unique card synergies
  // (e.g. tiny stable + flying unicorn cards that return to your hand
  Base_EnterStableEffects(pnum, corn.effect);
  Base_ToggleFlags(pnum, corn.effect);

  // sacrifice unicorns if there are more than 5 with Tiny Stable
  if ((player[pnum].flags & TINY_STABLE) != 0 && (player[pnum].flags & PANDAMONIUM) == 0) {
    while (player[pnum].stable.numUnicorns > 5) {
      Sacrifice(pnum, ANYUNICORN);
    }
  }

  // puppicorn tracker
  if (corn.effect == PUPPICORN) {
    puppicornIndex[0] = player[pnum].stable.size - 1;
    puppicornIndex[1] = pnum;
  }
}

void ReturnCardToHand(int pnum, int cindex) {
  // return babies to the nursery
  if (player[pnum].stable.unicorns[cindex].cType == BABYUNICORN)
    AddNursery(player[pnum].stable.unicorns[cindex]);
  else {
    // "disable" the puppicorn swap since it was returned to someone's hand
    if (player[pnum].stable.unicorns[cindex].effect == PUPPICORN) {
      puppicornIndex[0] = -1;
    }
    player[pnum].hand.cards[player[pnum].hand.numCards] =
      player[pnum].stable.unicorns[cindex];
    player[pnum].hand.numCards++;
  }

  RearrangeStable(pnum, cindex);
}

int Tiebreaker(void) {
  int winningPnum = 0;
  int maximum = 0;
  int tie = 0;
  int letters[MAX_PLAYERS] = { 0 };

  for (int i = 0; i < currentPlayers; i++) {
    if ((player[i].flags & PANDAMONIUM) != 0)
      continue;

    int extra = 0;
    if ((player[i].flags & GINORMOUS_UNICORN) != 0 &&
      (player[i].flags & BLINDING_LIGHT) == 0)
      extra = 1;

    for (int j = 0; j < player[i].stable.size; j++) {
      if (player[i].stable.unicorns[j].species != NOSPECIES) {
        // technically, this is including non-alphanumeric characters (i.e. spaces and parentheses)
        // the real game specifically counts letters
        letters[i] += strlen(player[i].stable.unicorns[j].name);
      }
    }

    if ((player[i].stable.numUnicorns + extra) == maximum) {
      // compare the character count of all unicorns in the event of ties
      // between the number of unicorns
      if (letters[i] > letters[winningPnum]) {
        winningPnum = i;
        tie = 0;
      }
      else if (letters[i] == letters[winningPnum]) {
        tie = 1;
      }
    }
    else if ((player[i].stable.numUnicorns + extra) > maximum) {
      maximum = player[i].stable.numUnicorns + extra;
      winningPnum = i;
      tie = 0;
    }
  }

  // if there's STILL a tie, then nobody wins! (boo)
  return tie ? -1 : winningPnum;
}

// ********************************************************************************
// **************************** Boolean Check Functions ***************************
// ********************************************************************************

int CanBeNeighed(int pnum) {
  if ((player[pnum].flags & YAY) != 0) {
    return 0;
  }

  return 1;
}

int CanNeighOthers(int pnum) {
  if ((player[pnum].flags & SLOWDOWN) != 0) {
    return 0;
  }

  // ginormous unicorn is a magical unicorn that disables the opportunity
  // to play instant cards, but blinding light disables all card effects
  // and cancels it, so that should be checked too
  if ((player[pnum].flags & GINORMOUS_UNICORN) != 0 &&
      (player[pnum].flags & BLINDING_LIGHT) == 0) {
    return 0;
  }

  return 1;
}

int CanBeDestroyed(int pindex, int cindex, int cType, int isMagicCard) {
  // card type doesn't match
  if (!CheckType(cType, player[pindex].stable.unicorns[cindex].cType))
    return 0;

  // player[pindex]'s unicorn cards can't be destroyed
  if (((player[pindex].flags & RAINBOW_AURA) != 0) &&
      ((player[pindex].flags & PANDAMONIUM) == 0) &&
      player[pindex].stable.unicorns[cindex].species != NOSPECIES)
    return 0;

  // player[pindex]'s unicorns are pandas and can't be destroyed by cards targeting unicorns
  // no need to check for spces != NOSPECIES because those cards should have been filtered
  // out already if cType == ANYUNICORN
  if (((player[pindex].flags & PANDAMONIUM) != 0) &&
      cType == ANYUNICORN)
    return 0;

  // puppicorn can't be destroyed
  if (player[pindex].stable.unicorns[cindex].effect == PUPPICORN)
    return 0;

  // magical kittencorn can't be destroyed by magic cards
  if (isMagicCard &&
      player[pindex].stable.unicorns[cindex].effect == MAGICAL_KITTENCORN)
    return 0;

  return 1;
}

int CheckNumCardsToDestroy(int pnum, int cType, int isMagicCard) {
  int isvalid = 0;

  // check if there are actual cards to destroy
  for (int i = 0; i < currentPlayers; i++) {
    if (i == pnum) continue;

    for (int j = 0; j < player[i].stable.size; j++) {
      if (CanBeDestroyed(i, j, cType, isMagicCard))
        isvalid++;
    }
  }

  return isvalid;
}

int CanBeSacrificed(int pindex, int cindex, int cType) {
  // card type doesn't match
  if (!CheckType(cType, player[pindex].stable.unicorns[cindex].cType))
    return 0;

  // puppicorn can't be sacrificed
  if (player[pindex].stable.unicorns[cindex].effect == PUPPICORN)
    return 0;

  // pandas aren't unicorns, so they can't be sacrificed in place of a unicorn card
  // no need to check for spces != NOSPECIES because those cards should have been filtered
  // out already if cType == ANYUNICORN
  if ((player[pindex].flags & PANDAMONIUM) != 0 &&
      cType == ANYUNICORN)
    return 0;

  return 1;
}

// ********************************************************************************
// ************************** Basic Card Effect Functions *************************
// ********************************************************************************

int Draw(int pnum, int numDrawn) {
  // game is over when the deck is empty
  if (deck.size - numDrawn <= 0) {
    if (isClient)
      ClientSendEndGame(-1, sockfd);
    else
      ServerSendEndGame(-1);

    return -1;
  }

  for (int i = 0; i < numDrawn; i++) {
    player[pnum].hand.cards[player[pnum].hand.numCards] = deck.cards[deck.size - 1];
    player[pnum].hand.numCards++;
    deck.size--;
  }

  return 0;
}

void Discard(int pnum, int numDiscard, int cType) {
  int index;
  char *end, buf[LINE_MAX];

  // repeat for numDiscard times or until the number of cards in hand is zero
  while (player[pnum].hand.numCards > 0 && numDiscard > 0) {
    PrintHand(pnum);
    for (;;) {
      printf("Pick a valid card number to discard: ");
      index = NumInput(buf, &end, sizeof buf) - 1;

      // index validation
      if (index < 0 || index >= player[pnum].hand.numCards || end != (buf + strlen(buf)))
        continue;

      // check for a type match
      if (CheckType(cType, player[pnum].hand.cards[index].cType))
        break;
    }

    AddDiscard(player[pnum].hand.cards[index]);
    RearrangeHand(pnum, index);
    numDiscard--;
  }
}

int Sacrifice(int pnum, int cType) {
  int index, isvalid = 0;
  char *end, buf[LINE_MAX];

  for (int i = 0; i < player[pnum].stable.size; i++) {
    if (CanBeSacrificed(pnum, i, cType)) {
      isvalid = 1;
      break;
    }
  }

  if (!isvalid) {
    Red();
    printf("%s;", player[pnum].username);
    ResetCol();
    printf("There are no available cards to sacrifice\n");
    return 0;
  }

  PrintStable(pnum);
  for (;;) {
    printf("Pick a valid card number to sacrifice: ");
    index = NumInput(buf, &end, sizeof buf) - 1;

    // index validation
    if (index < 0 || index >= player[pnum].stable.size || end != (buf + strlen(buf)))
      continue;

    // condition met :)
    if (CanBeSacrificed(pnum, index, cType))
      break;
  }

  Base_SacrificeDestroyEffects(pnum, index, player[pnum].stable.unicorns[index].effect);
  return 1;
}

void Destroy(int pnum, int cType, int isMagicCard) {
  int pindex, cindex, isvalid = 0;
  char *end, buf[LINE_MAX];

  PrintPlayers();
  do {
    printf("Choose a player to destroy from: ");
    pindex = NumInput(buf, &end, sizeof buf) - 1;

    // index validation
    if (pindex < 0 || pindex >= currentPlayers || pindex == pnum ||
        end != (buf + strlen(buf)))
      continue;

    for (int i = 0; i < player[pindex].stable.size; i++) {
      if (CanBeDestroyed(pindex, i, cType, isMagicCard)) {
        isvalid = 1;
        break;
      }
    }
  } while (!isvalid);

  PrintStable(pindex);
  do {
    printf("Choose the card number to destroy: ");
    cindex = NumInput(buf, &end, sizeof buf) - 1;

    // input validation
    if (cindex < 0 || cindex >= player[pindex].stable.size || end != (buf + strlen(buf)))
      continue;

    // check for cType stuff
    if (!CanBeDestroyed(pindex, cindex, cType, isMagicCard)) {
      cindex = -1;
    }
  } while (cindex < 0 || cindex >= player[pindex].stable.size || end != (buf + strlen(buf)));

  if (player[pindex].stable.unicorns[cindex].species == NOSPECIES) {
    // Base_SacrificeDestroyEffects(pindex, cindex, player[pindex].stable.unicorns[cindex].effect);

    // there's no need to send data over to the victim because upgrade/downgrade cards don't
    // have any special sacrifice/destroy effects
    AddDiscard(player[pindex].stable.unicorns[cindex]);
    RearrangeStable(pindex, cindex);
  }
  else {
    // send a notice to players to apply special sacrifice/destroy effects
    if (isClient) {
      SendInt(DESTROY_EVENT, sockfd);
      SendCardEffectPacket(pindex, cindex, sockfd); // send card index instead of type

      // need to look out for nested network events
      ClientEnterLeaveStable(pnum);
    }
    else {
      SendInt(DESTROY_EVENT, clientsockfd[pindex - 1]);
      SendCardEffectPacket(pnum, cindex, clientsockfd[pindex - 1]); // send origPnum (pnum) instead of targetPnum (pindex), and send card index instead of type

      // need to look out for nested network events
      ServerEnterLeaveStable(pnum, pindex);
    }
  }
}

void Steal(int pnum, int cType) {
  int pindex = -1, cindex, isvalid = 0;
  char *end, buf[LINE_MAX];

  PrintPlayers();
  do {
    printf("Choose a player to steal from: ");
    pindex = NumInput(buf, &end, sizeof buf) - 1;

    // index validation
    if (pindex < 0 || pindex >= currentPlayers || pindex == pnum || end != (buf + strlen(buf)))
      continue;

    // check if player has any valid unicorns to steal
    for (int i = 0; i < player[pindex].stable.size; i++) {
      if (CheckType(cType, player[pindex].stable.unicorns[i].cType)) {
        isvalid = 1;
        break;
      }
    }
  } while (!isvalid);

  PrintStable(pindex);
  for (;;) {
    printf("Choose the card number to steal: ");
    cindex = NumInput(buf, &end, sizeof buf) - 1;

    // index validation
    if (cindex < 0 || cindex >= player[pindex].stable.size || end != (buf + strlen(buf)))
      continue;

    // check for a type match
    if (CheckType(cType, player[pindex].stable.unicorns[cindex].cType)) {
      break;
    }
  }

  // assign card from chosen player's stable (pindex) to the current player's stable (pnum)
  struct Unicorn tmp = player[pindex].stable.unicorns[cindex];
  RearrangeStable(pindex, cindex);

  AddStable(pnum, tmp);
}

// ********************************************************************************
// *************************** Core Game Loop Functions ***************************
// ********************************************************************************

void PlayCard(int pnum) {
  int index;
  char *end, buf[LINE_MAX];

  for (;;) {
    printf("Pick the card number you'd like to play (non-instant card): ");
    index = NumInput(buf, &end, sizeof buf) - 1;

    // index validation
    if (index < 0 || index >= player[pnum].hand.numCards || end != (buf + strlen(buf)))
      continue;

    // check for instant cards
    if (player[pnum].hand.cards[index].cType != INSTANT)
      break;
  }

  // get the card ID before rearranging the hand so that enter stable effects
  // don't show the card you just played in your hand
  struct Unicorn corn = player[pnum].hand.cards[index];
  int target_pindex = pnum;

  // putting this in advance so that the neigh event stuff would only have to be written once
  if (corn.cType == UPGRADE || corn.cType == DOWNGRADE) {
    PrintPlayers();
    do {
      printf("Choose a player to give the upgrade/downgrade card: ");
      target_pindex = NumInput(buf, &end, sizeof buf) - 1;
    } while (target_pindex < 0 || target_pindex >= currentPlayers || end != (buf + strlen(buf)));
  }

  // skip over unplayable cards before the neigh prompt so that people don't waste
  // their neighs over nothing
  if (!Base_ConditionalEffects(pnum, corn, index, target_pindex)) {
    return;
  }

  // check if the player cannot be Neigh'd first before initiating the func
  if (CanBeNeighed(pnum)) {
    int ret;
    if (isClient) {
      SendInt(NEIGH_EVENT, sockfd);
      SendInt(index, sockfd);
      SendPlayers(sockfd);
      ret = ClientNeigh(pnum, pnum, &index);
    }
    else {
      ret = ServerNeigh(pnum, &index);
    }

    if (ret & 1) return;
  }

  // there were no successful neighs, so play the card!
  RearrangeHand(pnum, index);
  switch (corn.cType) {
  case BASICUNICORN:
  case MAGICALUNICORN:
  {
    AddStable(pnum, corn);
    break;
  }
  case UPGRADE:
  case DOWNGRADE:
  {
    // assign card to the chosen stable (target_pindex)
    AddStable(target_pindex, corn);
    break;
  }
  case MAGIC:
    // magic cards go to the discard pile!
    AddDiscard(corn);
    Base_MagicEffects(pnum, corn.effect);
    break;
  default:
    // something went wrong here... there should be no baby unicorns, instant, or typeless cards
    printf("unicorn type error, please choose a different card\n");
    turnCount++;
    return;
  }
}

void EndGame(int winningPnum) {
  printf("\n********************************************************************************\n");
  Red();
  printf("\nPlayer Stables\n");
  ResetCol();
  PrintStableGrid();

  // https://www.asciiart.eu/mythology/unicorns
  printf("\n\n"
    "                    /  \n"
    "               ,.. /   \n"
    "             ,'   ';        _____                         _____      _   _ \n"
    "  ,,.__    _,' /';  .      / ____|                       / ____|    | | | |\n"
    " :','  ~~~~    '. '~      | |  __  __ _ _ __ ___   ___  | (___   ___| |_| |\n"
    ":' (   )         )::,     | | |_ |/ _` | '_ ` _ \\ / _ \\  \\___ \\ / _ \\ __| |\n"
    "'. '. .=----=..-~  .;'    | |__| | (_| | | | | | |  __/  ____) |  __/ |_|_|\n"
    " '  ;'  ::   ':.  '\"       \\_____|\\__,_|_| |_| |_|\\___| |_____/ \\___|\\__(_)\n"
    "   (:   ':    ;)       \n"
    "    \\\\   '\"  ./        \n"
    "     '\"      '\"     *ASCII unicorn by Dr J   \n"
    "\n");

  // check if there's still a tie
  if (winningPnum != -1) {
    char winmsg[DESC_SIZE];
    sprintf_s(winmsg, LINE_MAX + 18, "%s won the game!!!\n", player[winningPnum].username);
    Rainbow(winmsg);
  }
  else {
    Blue();
    printf("The game ended in a tie! Nobody wins :(\n");
    ResetCol();
  }

  printf("\nPress any key to close the window...");
  exit(0);
}
