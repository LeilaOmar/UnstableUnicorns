#include "gamemechanics.h"
#include "networkevents.h"

// ********************************************************************************
// **************************** Print/Display Functions ***************************
// ********************************************************************************

void printPlayers(void) {
  puts("List of players:");
  for (int i = 0; i < current_players; i++) {
    printf("    %d: %s\n", i + 1, player[i].username);
  }
}

void printPile(struct Deck d) {
  for (int i = 0; i < d.size; i++) {
    printf("%d: %s [ID: %d]\n", i + 1, d.cards[i].name, d.cards[i].id);
  }
}

void printPileFilter(struct Deck d, int cType, int species) {
  for (int i = 0; i < d.size; i++) {
    if ((species == ANY || species == d.cards[i].species) &&
        checkType(cType, d.cards[i].cType))
    printf("%d: %s [ID: %d]\n", i + 1, d.cards[i].name, d.cards[i].id);
  }
}

void printHand(int pnum) {
  printf("\n%s's hand:\n", player[pnum].username);
  for (int i = 0; i < player[pnum].hand.num_cards; i++) {
    printf("    %d. %s [ID: %d]\n", i + 1, player[pnum].hand.cards[i].name, player[pnum].hand.cards[i].id);
  }
}

void printStable(int pnum) {
  printf("\n%s's stable:\n", player[pnum].username);
  for (int i = 0; i < player[pnum].stable.size; i++) {
    if (player[pnum].stable.unicorns[i].cType == UPGRADE)
      green();
    else if (player[pnum].stable.unicorns[i].cType == DOWNGRADE)
      yellow();

    printf("    %d. %s [ID: %d]\n", i + 1, player[pnum].stable.unicorns[i].name, player[pnum].stable.unicorns[i].id);
    reset_col();
  }
}

// currently isn't scalable to multiple decks
void displayCardDesc() {
  int index;
  char *end, buf[LINE_MAX];

  do {
    printf("Enter the card ID of the card you wish to display the description of: ");
    index = numinput(buf, &end, sizeof buf);
  } while (index < 0 || index >= DECK_SIZE + NURSERY_SIZE || end != (buf + strlen(buf)));

  cyan();
  printf("Card: %s\nDescription: %s\n", basedeck[index].name, basedeck[index].description);
  // if (index < NURSERY_SIZE)
  //   printf("Card: %s\nDescription: %s\n", nursery.cards[index].name, nursery.cards[index].description);
  // else
  //   printf("Card: %s\nDescription: %s\n", deck.cards[index].name, deck.cards[index].description);
  reset_col();
}

void displayDesiredStable(void) {
  int index;
  char *end, buf[LINE_MAX];

  printPlayers();
  do {
    printf("Choose the index associated with the player whose stable you'd like to see: ");
    index = numinput(buf, &end, sizeof buf) - 1;
  } while (index < 0 || index >= current_players || end != (buf + strlen(buf)));

  printStable(index);
}

// prints stables of all players in two columns
void printStableGrid(void) {
  char gridbuf[50] = { 0 };
  char spacebuf[50] = { 0 };
  int colsize = 48;

  for (int k = 0; k < current_players; k += 2) {
    printf("\n");

    printf("%s's stable", player[k].username);
    memset(spacebuf, ' ', colsize - strlen(player[k].username) - 7);
    printf("%s", spacebuf);
    memset(spacebuf, '\0', sizeof spacebuf);

    if ((k + 1) < current_players)
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
          green();
        else if (player[k].stable.unicorns[i].cType == DOWNGRADE)
          yellow();

        sprintf_s(gridbuf, sizeof gridbuf, "    %d. %s [ID: %d]", i + 1, player[k].stable.unicorns[i].name, player[k].stable.unicorns[i].id);
        memset(spacebuf, ' ', colsize - strlen(gridbuf));
        printf("%s%s", gridbuf, spacebuf);
        memset(gridbuf, '\0', sizeof gridbuf);
        memset(spacebuf, '\0', sizeof spacebuf);

        reset_col();
      }
      else {
        memset(spacebuf, ' ', colsize);
        printf("%s", spacebuf);
      }

      if ((k + 1) < current_players && player[k + 1].stable.size > i)
      {
        if (player[k + 1].stable.unicorns[i].cType == UPGRADE)
          green();
        else if (player[k + 1].stable.unicorns[i].cType == DOWNGRADE)
          yellow();
        printf("    %d: %s [ID: %d]", i + 1, player[k + 1].stable.unicorns[i].name, player[k + 1].stable.unicorns[i].id);
        reset_col();
      }

      memset(spacebuf, '\0', sizeof spacebuf);
      printf("\n");
    }
  }
}

// ********************************************************************************
// ******************************* Utility Functions ******************************
// ********************************************************************************

// return 0 = FALSE, the card's cType does not equal the desired cType
// return 1 = TRUE, card match!
int checkType(int desired_type, int card_type) {
  if (desired_type == ANY || desired_type == card_type ||
    (desired_type == ANYUNICORN &&
      (card_type == BABYUNICORN || card_type == BASICUNICORN ||
        card_type == MAGICUNICORN))) {
    return 1;
  }
  return 0;
}

int checkWin(int pnum) {
  if ((player[pnum].stable.num_unicorns >= WIN_CONDITION ||
    (player[pnum].stable.num_unicorns >= (WIN_CONDITION - 1) &&
      (player[pnum].flags & (ginormous_unicorn | blinding_light)) == ginormous_unicorn)) &&
    (player[pnum].flags & pandamonium) == 0)
    return 1;

  return 0;
}

// randomize deck
void shuffleDeck(struct Deck *d) {
  struct Unicorn tmp;

  // randomize deck with the Fisher-Yates algorithm; starts from the end
  // because the "taken" cards are pushed towards the beginning, which is
  // outside of deck_index's or start's range
  for (int i = d->size - 1; i > 0; i--) {
    int j = rand() % (i + 1);
    tmp = d->cards[i];
    d->cards[i] = d->cards[j];
    d->cards[j] = tmp;
  }

  if (deck_flag) {
    blue();
    printPile(*d);
    reset_col();
  }
}

// shuffles the discard pile into the deck, then shuffles the entire deck
void shuffleDiscard(void) {
  for (int i = discardpile.size - 1; i >= 0; i--) {
    deck.cards[deck.size] = discardpile.cards[i];
    deck.size++;
    discardpile.size--;
  }

  shuffleDeck(&deck);

  blue();
  printPile(deck);
  reset_col();
}

// shuffles the indexed card out of the deck, nursery, or discard pile's range
void rearrangePile(struct Deck *d, int index) {
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

// shuffles the indexed card out of the hand's range
void rearrangeHand(int pnum, int index) {
  struct Unicorn tmp;

  for (int i = index; i < player[pnum].hand.num_cards - 1; i++) {
    tmp = player[pnum].hand.cards[i];
    player[pnum].hand.cards[i] = player[pnum].hand.cards[i + 1];
    player[pnum].hand.cards[i + 1] = tmp;
  }
  player[pnum].hand.num_cards--;
}

// shuffles the indexed card out of the stable's range
void rearrangeStable(int pnum, int index) {
  struct Unicorn tmp;

  // toggle special flags off
  toggleFlags(pnum, player[pnum].stable.unicorns[index].effect);

  if (player[pnum].stable.unicorns[index].species != NOSPECIES) {
    player[pnum].stable.num_unicorns--;

    // barbed wire check; this doesn't trigger for pandas when leaving the stable unlike
    // in the add function when they enter (since they enter as unicorns before "transforming")
    if ((player[pnum].flags & barbed_wire) != 0 && (player[pnum].flags & pandamonium) == 0) {
      discard(pnum, 1, ANY);
    }
  }

  // unicorn lasso check; make sure this gets disabled if the card leaves the stable in any way
  // before the end of the turn (e.g. it gets sacrificed/destroyed, returned to hand, swapped, etc.).
  // this is so that it doesn't return an incorrect card (or effect) back to the victim
  if (pnum == uni_lasso_flag[1]) {
    if (index == uni_lasso_flag[0])
      uni_lasso_flag[0] = -1;
    else if (index < uni_lasso_flag[0]) {
      uni_lasso_flag[0]--;
    } // if index is greater than the unicorn_lasso_index, then it doesn't affect the position of the card
  }

  // puppicorn check; make sure it shifts down by 1 if the index that got
  // deleted is less than puppicorn_index
  if (puppicorn_index[1] == pnum) {
    if (puppicorn_index[0] > index)
      puppicorn_index[0]--;
    // theoretically don't need to check if the puppicorn index is the same as the selected card index
    // because it should never get triggered like this, but it shall stay in just in case
    else if (puppicorn_index[0] == index) {
      puppicorn_index[0] = -1;
    }
  }

  for (int i = index; i < player[pnum].stable.size - 1; i++) {
    tmp = player[pnum].stable.unicorns[i];
    player[pnum].stable.unicorns[i] = player[pnum].stable.unicorns[i + 1];
    player[pnum].stable.unicorns[i + 1] = tmp;
  }
  player[pnum].stable.size--;
}

// searching through the deck or discard pile for a specific card
// 0 = return failed
// 1 = return successful
int searchPile(int pnum, struct Deck* d, int cType, int species) {
  int index, count = 0;
  char* end, buf[LINE_MAX];

  // check if there are actually available cards to take
  for (int i = 0; i < d->size; i++) {
    if ((species == ANY || species == d->cards[i].species) &&
        checkType(cType, d->cards[i].cType))
      count++;
  }

  // no valid cards are available
  if (count == 0)
    return 0;

  blue();
  printPileFilter(*d, cType, species);
  reset_col();

  for (;;) {
    printf("Pick a valid card number to add to your hand: ");
    index = numinput(buf, &end, sizeof buf) - 1;

    // index validation
    if (index < 0 || index >= d->size || end != (buf + strlen(buf)))
      continue;

    if ((species == ANY || species == d->cards[index].species) &&
        checkType(cType, d->cards[index].cType))
      break;
  }

  player[pnum].hand.cards[player[pnum].hand.num_cards++] = d->cards[index];
  rearrangePile(d, index);

  return 1;
}

void addNursery(struct Unicorn corn) {
  if (nursery.size == NURSERY_SIZE) {
    // the nursery is already full, so some cards must've been duplicated for this to happen?
    // the cardid check already takes place before this function
    return;
  }
  nursery.cards[nursery.size] = corn;
  nursery.size++;
}

void addDiscard(struct Unicorn corn) {
  if (discardpile.size == DECK_SIZE) {
    // this seems impossible unless there's a card that returns everyone's hand to the discard
    // pile, AND everyone's stables are empty, AND the deck is empty; if the deck was empty then
    // it's game over anyways
    return;
  }
  discardpile.cards[discardpile.size] = corn;
  discardpile.size++;
}

void addStable(int pnum, struct Unicorn corn) {
  player[pnum].stable.unicorns[player[pnum].stable.size] = corn;
  player[pnum].stable.size++;

  if (corn.species != NOSPECIES) {
    player[pnum].stable.num_unicorns++;

    // barbed wire check
    if ((player[pnum].flags & barbed_wire) != 0) {
      discard(pnum, 1, ANY);
    }
  }

  // trigger the enter stable effects and apply any flag changes
  // this goes before the tiny stable trigger just to allow for unique card synergies
  // (e.g. tiny stable + flying unicorn cards that return to your hand
  enterStableEffects(pnum, corn.effect);
  toggleFlags(pnum, corn.effect);

  // sacrifice unicorns if there are more than 5 with Tiny Stable
  if ((player[pnum].flags & tiny_stable) != 0 && (player[pnum].flags & pandamonium) == 0) {
    while (player[pnum].stable.num_unicorns > 5) {
      sacrifice(pnum, ANYUNICORN);
    }
  }

  // puppicorn tracker
  if (strcmp(corn.name, "Puppicorn") == 0) {
    puppicorn_index[0] = player[pnum].stable.size - 1;
    puppicorn_index[1] = pnum;
  }
}

// return card from stable to hand, or the nursery if it's a baby
void returnCardToHand(int pnum, int cindex) {
  // return babies to the nursery
  if (player[pnum].stable.unicorns[cindex].cType == BABYUNICORN)
    addNursery(player[pnum].stable.unicorns[cindex]);
  else {
    // "disable" the puppicorn swap since it was returned to someone's hand
    if (strcmp(player[pnum].stable.unicorns[cindex].name, "Puppicorn") == 0) {
      puppicorn_index[0] = -1;
    }
    player[pnum].hand.cards[player[pnum].hand.num_cards] =
      player[pnum].stable.unicorns[cindex];
    player[pnum].hand.num_cards++;
  }

  rearrangeStable(pnum, cindex);
}

// utility tie-breaker func for games that end early
// return [0-7] == winning player index
// return    -1 == complete tie, nobody wins
int tiebreaker() {
  int winningpnum = 0;
  int maximum = 0;
  int tie = 0;
  int letters[MAX_PLAYERS] = { 0 };

  for (int i = 0; i < current_players; i++) {
    if ((player[i].flags & pandamonium) != 0)
      continue;

    int extra = 0;
    if ((player[i].flags & ginormous_unicorn) != 0 &&
      (player[i].flags & blinding_light) == 0)
      extra = 1;

    for (int j = 0; j < player[i].stable.size; j++) {
      if (player[i].stable.unicorns[j].species != NOSPECIES) {
        // technically, this is including non-alphanumeric characters (i.e. spaces and parentheses)
        // the real game specifically counts letters
        letters[i] += strlen(player[i].stable.unicorns[j].name);
      }
    }

    if ((player[i].stable.num_unicorns + extra) == maximum) {
      // compare the character count of all unicorns in the event of ties
      // between the number of unicorns
      if (letters[i] > letters[winningpnum]) {
        winningpnum = i;
        tie = 0;
      }
      else if (letters[i] == letters[winningpnum]) {
        tie = 1;
      }
    }
    else if ((player[i].stable.num_unicorns + extra) > maximum) {
      maximum = player[i].stable.num_unicorns + extra;
      winningpnum = i;
      tie = 0;
    }
  }

  // if there's STILL a tie, then nobody wins! (boo)
  return tie ? -1 : winningpnum;
}

// ********************************************************************************
// **************************** Boolean Check Functions ***************************
// ********************************************************************************

// check for flags that make the player immune to Neigh cards
int canBeNeighed(int pnum) {
  if ((player[pnum].flags & yay) != 0) {
    return 0;
  }

  return 1;
}

// check for flags that make the player unable to use Neigh cards
int canNeighOthers(int pnum) {
  if ((player[pnum].flags & slowdown) != 0) {
    return 0;
  }

  // ginormous unicorn is a magic unicorn that disables the opportunity
  // to play instant cards, but blinding light disables all card effects
  // and cancels it, so that should be checked too
  if ((player[pnum].flags & ginormous_unicorn) != 0 &&
      (player[pnum].flags & blinding_light) == 0) {
    return 0;
  }

  return 1;
}

int canBeDestroyed(int pindex, int cindex, int cType, int isMagicCard) {
  // card type doesn't match
  if (!checkType(cType, player[pindex].stable.unicorns[cindex].cType))
    return 0;

  // player[pindex]'s unicorn cards can't be destroyed
  if (((player[pindex].flags & rainbow_aura) != 0) &&
      ((player[pindex].flags & pandamonium) == 0) &&
      player[pindex].stable.unicorns[cindex].species != NOSPECIES)
    return 0;

  // player[pindex]'s unicorns are pandas and can't be destroyed by cards targeting unicorns
  // no need to check for spces != NOSPECIES because those cards should have been filtered
  // out already if cType == ANYUNICORN
  if (((player[pindex].flags & pandamonium) != 0) &&
      cType == ANYUNICORN)
    return 0;

  // puppicorn can't be destroyed
  if (strcmp(player[pindex].stable.unicorns[cindex].name, "Puppicorn") == 0)
    return 0;

  // magical kittencorn can't be destroyed by magic cards
  if (isMagicCard &&
      strcmp(player[pindex].stable.unicorns[cindex].name, "Magical Kittencorn") == 0)
    return 0;

  return 1;
}

int checkNumCardsToDestroy(int pnum, int cType, int isMagicCard) {
  int isvalid = 0;

  // check if there are actual cards to destroy
  for (int i = 0; i < current_players; i++) {
    if (i == pnum) continue;

    for (int j = 0; j < player[i].stable.size; j++) {
      if (canBeDestroyed(i, j, cType, isMagicCard))
        isvalid++;
    }
  }

  return isvalid;
}

int canBeSacrificed(int pindex, int cindex, int cType) {
  // card type doesn't match
  if (!checkType(cType, player[pindex].stable.unicorns[cindex].cType))
    return 0;

  // puppicorn can't be sacrificed
  if (strcmp(player[pindex].stable.unicorns[cindex].name, "Puppicorn") == 0)
    return 0;

  // pandas aren't unicorns, so they can't be sacrificed in place of a unicorn card
  // no need to check for spces != NOSPECIES because those cards should have been filtered
  // out already if cType == ANYUNICORN
  if ((player[pindex].flags & pandamonium) != 0 &&
      cType == ANYUNICORN)
    return 0;

  return 1;
}

// ********************************************************************************
// ************************** Basic Card Effect Functions *************************
// ********************************************************************************

int draw(int pnum, int num_drawn) {
  // game is over when the deck is empty
  if (deck.size - num_drawn <= 0) {
    if (isclient)
      clientSendEndGame(-1, sockfd);
    else
      serverSendEndGame(-1);

    return -1;
  }

  for (int i = 0; i < num_drawn; i++) {
    player[pnum].hand.cards[player[pnum].hand.num_cards] = deck.cards[deck.size - 1];
    player[pnum].hand.num_cards++;
    deck.size--;
  }

  return 0;
}

void discard(int pnum, int num_discard, int cType) {
  int index;
  char* end, buf[LINE_MAX];

  // repeat for num_discard times or until the number of cards in hand is zero
  while (player[pnum].hand.num_cards > 0 && num_discard > 0) {
    printHand(pnum);
    for (;;) {
      printf("Pick a valid card number to discard: ");
      index = numinput(buf, &end, sizeof buf) - 1;

      // index validation
      if (index < 0 || index >= player[pnum].hand.num_cards || end != (buf + strlen(buf)))
        continue;

      // check for a type match
      if (checkType(cType, player[pnum].hand.cards[index].cType))
        break;
    }

    addDiscard(player[pnum].hand.cards[index]);
    rearrangeHand(pnum, index);
    num_discard--;
  }
}

int sacrifice(int pnum, int cType) {
  int index, isvalid = 0;
  char* end, buf[LINE_MAX];

  for (int i = 0; i < player[pnum].stable.size; i++) {
    if (canBeSacrificed(pnum, i, cType)) {
      isvalid = 1;
      break;
    }
  }

  if (!isvalid) {
    red();
    printf("%s;", player[pnum].username);
    reset_col();
    printf("There are no available cards to sacrifice\n");
    return 0;
  }

  printStable(pnum);
  for (;;) {
    printf("Pick a valid card number to sacrifice: ");
    index = numinput(buf, &end, sizeof buf) - 1;

    // index validation
    if (index < 0 || index >= player[pnum].stable.size || end != (buf + strlen(buf)))
      continue;

    // condition met :)
    if (canBeSacrificed(pnum, index, cType))
      break;
  }

  sacrificeDestroyEffects(pnum, index, player[pnum].stable.unicorns[index].effect);
  return 1;
}

// treat ANYUNICORN as all unicorns when checking cType for Unicorn cards
void destroy(int pnum, int cType, int isMagicCard) {
  int pindex, cindex, isvalid = 0;
  char *end, buf[LINE_MAX];

  printPlayers();
  do {
    printf("Choose a player to destroy from: ");
    pindex = numinput(buf, &end, sizeof buf) - 1;

    // index validation
    if (pindex < 0 || pindex >= current_players || pindex == pnum ||
        end != (buf + strlen(buf)))
      continue;

    for (int i = 0; i < player[pindex].stable.size; i++) {
      if (canBeDestroyed(pindex, i, cType, isMagicCard)) {
        isvalid = 1;
        break;
      }
    }
  } while (!isvalid);

  printStable(pindex);
  do {
    printf("Choose the card number to destroy: ");
    cindex = numinput(buf, &end, sizeof buf) - 1;

    // input validation
    if (cindex < 0 || cindex >= player[pindex].stable.size || end != (buf + strlen(buf)))
      continue;

    // check for cType stuff
    if (!canBeDestroyed(pindex, cindex, cType, isMagicCard)) {
      cindex = -1;
    }
  } while (cindex < 0 || cindex >= player[pindex].stable.size || end != (buf + strlen(buf)));

  if (player[pindex].stable.unicorns[cindex].species == NOSPECIES) {
    // sacrificeDestroyEffects(pindex, cindex, player[pindex].stable.unicorns[cindex].effect);

    // there's no need to send data over to the victim because upgrade/downgrade cards don't
    // have any special sacrifice/destroy effects
    addDiscard(player[pindex].stable.unicorns[cindex]);
    rearrangeStable(pindex, cindex);
  }
  else {
    // send a notice to players to apply special sacrifice/destroy effects
    if (isclient) {
      sendInt(destroy_event, sockfd);
      sendCardEffectPacket(pindex, cindex, sockfd); // send card index instead of type

      // need to look out for nested network events
      clientDestroyEffect(pnum);
    }
    else {
      sendInt(destroy_event, clientsockfd[pindex - 1]);
      sendCardEffectPacket(pnum, cindex, clientsockfd[pindex - 1]); // send orig_pnum (pnum) instead of target_pnum (pindex), and send card index instead of type

      // need to look out for nested network events
      serverDestroyEffect(pnum, pindex);
    }
  }
}

// treat ANYUNICORN as all unicorns when checking cType for Unicorn cards
void steal(int pnum, int cType) {
  int pindex = -1, cindex, isvalid = 0;
  char* end, buf[LINE_MAX];

  printPlayers();
  do {
    printf("Choose a player to steal from: ");
    pindex = numinput(buf, &end, sizeof buf) - 1;

    // index validation
    if (pindex < 0 || pindex >= current_players || pindex == pnum || end != (buf + strlen(buf)))
      continue;

    // check if player has any valid unicorns to steal
    for (int i = 0; i < player[pindex].stable.size; i++) {
      if (checkType(cType, player[pindex].stable.unicorns[i].cType)) {
        isvalid = 1;
        break;
      }
    }
  } while (!isvalid);

  printStable(pindex);
  for (;;) {
    printf("Choose the card number to steal: ");
    cindex = numinput(buf, &end, sizeof buf) - 1;

    // index validation
    if (cindex < 0 || cindex >= player[pindex].stable.size || end != (buf + strlen(buf)))
      continue;

    // check for a type match
    if (checkType(cType, player[pindex].stable.unicorns[cindex].cType)) {
      break;
    }
  }

  // assign card from chosen player's stable (pindex) to the current player's stable (pnum)
  struct Unicorn tmp = player[pindex].stable.unicorns[cindex];
  rearrangeStable(pindex, cindex);

  addStable(pnum, tmp);
}

// ********************************************************************************
// *************************** Core Game Loop Functions ***************************
// ********************************************************************************

void playCard(int pnum) {
  int index, target_pindex;
  char* end, buf[LINE_MAX];

  for (;;) {
    printf("Pick the card number you'd like to play (non-instant card): ");
    index = numinput(buf, &end, sizeof buf) - 1;

    // index validation
    if (index < 0 || index >= player[pnum].hand.num_cards || end != (buf + strlen(buf)))
      continue;

    // check for instant cards
    if (player[pnum].hand.cards[index].cType != INSTANT)
      break;
  }

  // get the card ID before rearranging the hand so that enter stable effects
  // don't show the card you just played in your hand
  struct Unicorn corn = player[pnum].hand.cards[index];
  target_pindex = pnum;

  // putting this in advance so that the neigh event stuff would only have to be written once
  if (corn.cType == UPGRADE || corn.cType == DOWNGRADE) {
    printPlayers();
    do {
      printf("Choose a player to give the upgrade/downgrade card: ");
      target_pindex = numinput(buf, &end, sizeof buf) - 1;
    } while (target_pindex < 0 || target_pindex >= current_players || end != (buf + strlen(buf)));
  }

  // skip over unplayable cards before the neigh prompt so that people don't waste
  // their neighs over nothing
  if (!conditionalEffects(pnum, corn, index, target_pindex)) {
    return;
  }

  // check if the player cannot be Neigh'd first before initiating the func
  if (canBeNeighed(pnum)) {
    int ret;
    if (isclient) {
      sendInt(neigh_event, sockfd);
      sendInt(index, sockfd);
      sendPlayers(sockfd);
      ret = clientNeigh(pnum, pnum, &index);
    }
    else {
      ret = serverNeigh(pnum, &index);
    }

    if (ret & 1) return;
  }

  // there were no successful neighs, so play the card!
  rearrangeHand(pnum, index);
  switch (corn.cType) {
  case BASICUNICORN:
  case MAGICUNICORN:
  {
    addStable(pnum, corn);
    break;
  }
  case UPGRADE:
  case DOWNGRADE:
  {
    // assign card to the chosen stable (target_pindex)
    addStable(target_pindex, corn);
    break;
  }
  case MAGIC:
    // magic cards go to the discard pile!
    addDiscard(corn);
    magicEffects(pnum, corn.effect);
    break;
  default:
    // something went wrong here... there should be no baby unicorns, instant, or typeless cards
    printf("unicorn type error, please choose a different card\n");
    turn_count++;
    return;
  }
}

void endGame(int winningpnum) {
  printf("\n********************************************************************************\n");
  red();
  printf("\nPlayer Stables\n");
  reset_col();
  printStableGrid();

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
  if (winningpnum != -1) {
    char winmsg[DESC_SIZE];
    sprintf_s(winmsg, NAME_SIZE + 18, "%s won the game!!!\n", player[winningpnum].username);
    rainbow(winmsg);
  }
  else {
    blue();
    printf("The game ended in a tie! Nobody wins :(\n");
    reset_col();
  }

  printf("\nPress any key to close the window...");
  exit(0);
}
