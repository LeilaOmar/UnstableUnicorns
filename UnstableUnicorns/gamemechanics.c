#include "gamemechanics.h"
#include "networkfuncs.h"

// ********************************************************************************
// **************************** Print/Display Functions ***************************
// ********************************************************************************

void printPlayers(void) {
  puts("List of players:");
  for (int i = 0; i < current_players; i++) {
    printf("    %d: %s\n", i + 1, player[i].username);
  }
}

// TODO (for this and printDeck): remove start and size params because those
// are like always global variables
void printNursery(size_t start, size_t size) {
  int index = 1;

  for (size_t i = start; i < size; i++) {
    printf("%d: %s [ID: %d]\n", index++, deck[nursery_ref[i]].name,
      nursery_ref[i]);
  }
}

void printDeck(size_t start, size_t size, int class, int species) {
  // note that size references the size of the entire deck, not the size of the
  // search
  for (size_t i = start; i < size; i++) {
    // first part: any species means it is dependent on the class being the same
    // second part: class doesn't matter, only check if species matches
    if (((species == ANY) &&
      ((class == ANY) ||
        (class == ANYUNICORN && (deck[deck_ref[i]].class == BASICUNICORN ||
          deck[deck_ref[i]].class == MAGICUNICORN)) ||
        (class == deck[deck_ref[i]].class))) ||
      (species == deck[deck_ref[i]].species))
      printf("%zu: %s [ID: %d]\n", i + 1, deck[deck_ref[i]].name, deck_ref[i]);
  }
}

void printDiscard(int class) {
  for (size_t i = 0; i < discard_index; i++) {
    if ((class == ANY) ||
      (class == ANYUNICORN && (deck[discard_ref[i]].class == BASICUNICORN ||
        deck[discard_ref[i]].class == MAGICUNICORN)) ||
      (class == deck[discard_ref[i]].class))
      printf("%zu: %s [ID: %d]\n", i + 1, deck[discard_ref[i]].name,
        discard_ref[i]);
  }
}

void printHand(int pnum) {
  printf("%s's hand:\n", player[pnum].username);
  for (int i = 0; i < player[pnum].hand.num_cards; i++) {
    printf("    %d. %s [ID: %d]\n", i + 1,
      deck[player[pnum].hand.cards[i]].name, player[pnum].hand.cards[i]);
  }
}

void printStable(int pnum) {
  printf("%s's stable:\n", player[pnum].username);
  for (int i = 0; i < player[pnum].stable.size; i++) {
    if (deck[player[pnum].stable.unicorns[i]].class == 3)
      green();
    else if (deck[player[pnum].stable.unicorns[i]].class == 4)
      yellow();

    printf("    %d. %s [ID: %d]\n", i + 1,
      deck[player[pnum].stable.unicorns[i]].name,
      player[pnum].stable.unicorns[i]);
    reset_col();
  }
}

void displayCardDesc(void) {
  int index;
  char *end, buf[LINE_MAX];

  do {
    printf("Enter the ID of the card you wish to display the description of: ");
    fgets(buf, sizeof buf, stdin);
    buf[strlen(buf) - 1] = 0;
    index = strtol(buf, &end, 10);
  } while (index < 0 || index >= NURSERY_SIZE + DECK_SIZE || end != (buf + strlen(buf)));

  cyan();
  printf("Card: %s\nDescription: %s\n", deck[index].name,
    deck[index].description);
  reset_col();
}

void displayDesiredStable(void) {
  int index;
  char *end, buf[LINE_MAX];

  printPlayers();
  do {
    printf("Choose the index associated with the player whose stable you'd like to see: ");
    fgets(buf, sizeof buf, stdin);
    buf[strlen(buf) - 1] = 0;
    index = strtol(buf, &end, 10) - 1;
  } while (index < 0 || index >= current_players || end != (buf + strlen(buf)));

  printStable(index);
}

// ********************************************************************************
// ******************************* Utility Functions ******************************
// ********************************************************************************

// OPTIMIZE: use species UNICORN instead of class ANYUNICORN and then combine
// this with checking for species too
// return 0 = FALSE, the card's class does not equal the desired class
// return 1 = TRUE, card match!
int checkClass(int desired_class, int card_class) {
  if (desired_class == ANY || desired_class == card_class ||
    (desired_class == ANYUNICORN &&
      (card_class == BABYUNICORN || card_class == BASICUNICORN ||
        card_class == MAGICUNICORN))) {
    return 1;
  }
  return 0;
}

// randomize deck between specific indices
void shuffleDeck(int start, int size) {
  int index = 0;

  // randomize deck with the Fisher-Yates algorithm; starts from the end
  // because the "taken" cards are pushed towards the beginning, which is
  // outside of deck_index's or start's range
  if (start >= 0) {
    for (int i = size - 1; i > start; i--) {
      int j = rand() % (i + 1 - start) + start;
      index = deck_ref[i];
      deck_ref[i] = deck_ref[j];
      deck_ref[j] = index;
    }
  }

  if (deck_flag) {
    blue();
    printDeck(index, DECK_SIZE, ANY, ANY);
    reset_col();
  }
}

// shuffles the indexed car out of the nursery's range
// (so the nursery_index only keeps track of the cards within
// nursery_ref that are still inside the nursery and not taken out)
void rearrangeNursery(int index) {
  int tmp;

  if (index >= 0) {
    for (size_t i = index; i > nursery_index; i--) {
      tmp = nursery_ref[i];
      nursery_ref[i] = nursery_ref[i - 1];
      nursery_ref[i - 1] = tmp;
    }
    nursery_index++;
    dnurse_size--;
  }
}

// nursery_index points to the top most card of the ones available in the
// nursery at the time. this function basically lowers the index by one and
// overwrites previous data to include the newly added card. order doesn't
// matter
void addNursery(int cardid) {
  nursery_index--;
  nursery_ref[nursery_index] = cardid;
  dnurse_size++;
}

void addStable(int pnum, int cardid) {
  player[pnum].stable.unicorns[player[pnum].stable.size++] = cardid;
  if (deck[cardid].species != NOSPECIES) {
    player[pnum].stable.num_unicorns++;

    // barbed wire check
    if ((player[pnum].flags & barbed_wire) != 0) {
      discard(pnum, 1, ANY);
    }
  }

  // sacrifice unicorns if there are more than 5 with Tiny Stable
  if ((player[pnum].flags & tiny_stable) != 0 && (player[pnum].flags & pandamonium) == 0) {
    while (player[pnum].stable.num_unicorns > 5) {
      sacrifice(pnum, ANYUNICORN);
    }
  }
}

// shuffles the indexed card out of the hand's range
void rearrangeHand(int pnum, int index) {
  int tmp;

  for (int i = index; i < player[pnum].hand.num_cards - 1; i++) {
    tmp = player[pnum].hand.cards[i];
    player[pnum].hand.cards[i] = player[pnum].hand.cards[i + 1];
    player[pnum].hand.cards[i + 1] = tmp;
  }
  player[pnum].hand.num_cards--;
}

// shuffles the indexed card out of the stable's range
// OPTIMIZE: put toggleFlags here instead of writing it out a few times like
// when cards are stolen or returned to someone's hand?
void rearrangeStable(int pnum, int index) {
  int tmp;

  if (deck[player[pnum].stable.unicorns[index]].species != NOSPECIES) {
    player[pnum].stable.num_unicorns--;

    // barbed wire check; this doesn't trigger for pandas when leaving the stable unlike
    // in the add function when they enter (since they enter as unicorns before "transforming")
    if ((player[pnum].flags & barbed_wire) != 0 && (player[pnum].flags & pandamonium) == 0) {
      discard(pnum, 1, ANY);
    }
  }

  for (int i = index; i < player[pnum].stable.size - 1; i++) {
    tmp = player[pnum].stable.unicorns[i];
    player[pnum].stable.unicorns[i] = player[pnum].stable.unicorns[i + 1];
    player[pnum].stable.unicorns[i + 1] = tmp;
  }
  player[pnum].stable.size--;
}

// OPTIMIZE: combine this with rearrangeNursery for cleanup purposes; basically it
// is the same thing (except the indexes are arranged differently?)
void rearrangeDiscard(int index) {
  int tmp;

  for (size_t i = index; i < discard_index - 1; i++) {
    tmp = discard_ref[i];
    discard_ref[i] = discard_ref[i + 1];
    discard_ref[i + 1] = tmp;
  }
  discard_index--;
}

// shuffles the indexed card out of the deck's range
void rearrangeDeck(int index) {
  int tmp;

  for (size_t i = index; i > deck_index; i--) {
    tmp = deck_ref[i];
    deck_ref[i] = deck_ref[i - 1];
    deck_ref[i - 1] = tmp;
  }
  deck_index++;
}

// shuffles the discard pile into the deck, then shuffles the entire deck
void shuffleDiscard(void) {
  // printf("deck_index = %u; discard_index = %u\n", deck_index, discard_index);
  for (; discard_index > 0; discard_index--) {
    // decrements the deck index so it is referencing the card that is being
    // added instead of overwriting the top card in the deck
    deck_ref[--deck_index] = discard_ref[discard_index - 1];
  }

  shuffleDeck(deck_index, DECK_SIZE);

  blue();
  printDeck(deck_index, DECK_SIZE, ANY, ANY);
  reset_col();
}

// searching through the discard pile for a specific card
void searchDiscard(int pnum, int class) {
  int index, count = 0;
  char* end, buf[LINE_MAX];

  // check if there are actually cards available to take
  for (size_t i = 0; i < discard_index; i++) {
    if (checkClass(class, deck[discard_ref[i]].class)) count++;
  }

  // no valid cards are available
  if (count == 0) return;

  printDiscard(class);
  do {
    printf("Pick a valid card number to add to your hand: ");
    fgets(buf, sizeof buf, stdin);
    buf[strlen(buf) - 1] = 0;
    index = strtol(buf, &end, 10) - 1;

    if (index >= 0 && index < (int)discard_index && end == (buf + strlen(buf))) {
      if (!checkClass(class, deck[discard_ref[index]].class)) {
        index = -1;
      }
    }
  } while (index < 0 || index >= (int)discard_index || end != (buf + strlen(buf)));

  player[pnum].hand.cards[player[pnum].hand.num_cards++] = discard_ref[index];
  rearrangeDiscard(index);
}

// searching through the deck for a specific card, then shuffles the deck
void searchDeck(int pnum, int class, int species) {
  int index, count = 0;
  char* end, buf[LINE_MAX];

  // check if there are actually cards available to take
  for (int i = deck_index; i < DECK_SIZE; i++) {
    if (checkClass(class, deck[deck_ref[i]].class) ||
      species == deck[deck_ref[i]].species) {
      count++;
    }
  }

  // no valid cards are available
  if (count == 0) return;

  blue();
  printDeck(deck_index, DECK_SIZE, class, species);
  reset_col();
  do {
    printf("Pick a valid card number to add to your hand: ");
    fgets(buf, sizeof buf, stdin);
    buf[strlen(buf) - 1] = 0;
    index = strtol(buf, &end, 10) - 1;

    // check if the card actually matches with the specified class or species
    if (index >= (int)deck_index && index < DECK_SIZE && index > 0 && end == (buf + strlen(buf)))
      if (!checkClass(class, deck[deck_ref[index]].class) &&
        species != deck[deck_ref[index]].species)
        index = -1;
  } while (index < (int)deck_index || index >= DECK_SIZE || index < 0 || end != (buf + strlen(buf)));

  // check for index < 0 in case deck_index is a negative number
  player[pnum].hand.cards[player[pnum].hand.num_cards++] = deck_ref[index];
  // take the card out of the deck (by rearranging it) before shuffling
  rearrangeDeck(index);
  shuffleDeck(deck_index, DECK_SIZE);
}

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

// ********************************************************************************
// ************************** Basic Card Effect Functions *************************
// ********************************************************************************

void draw(int pnum, int num_drawn) {
  // accounting for edge cases where it could be 1-4 cards left to draw after
  // the deck is reshuffled; writing it this way makes it so that it's not
  // constantly checking in the for statement
  if (deck_index + num_drawn >= DECK_SIZE) {
    while (deck_index < DECK_SIZE) {
      player[pnum].hand.cards[player[pnum].hand.num_cards++] =
        deck_ref[deck_index++];
      num_drawn--;
    }
    shuffleDiscard();
  }
  for (int i = 0; i < num_drawn; i++) {
    player[pnum].hand.cards[player[pnum].hand.num_cards++] =
      deck_ref[deck_index++];
  }
}

void discard(int pnum, int num_discard, int class) {
  int index;
  char* end, buf[LINE_MAX];

  // repeat for num_discard times or until the number of cards in hand is zero
  while (player[pnum].hand.num_cards > 0 && num_discard > 0) {
    printHand(pnum);
    do {
      printf("Pick a valid card number to discard: ");
      fgets(buf, sizeof buf, stdin);
      buf[strlen(buf) - 1] = 0;
      index = strtol(buf, &end, 10) - 1;
      if (index >= 0 && index < player[pnum].hand.num_cards && end == (buf + strlen(buf))) {
        if (!checkClass(class, deck[player[pnum].hand.cards[index]].class)) {
          index = -1;
        }
      }
    } while (index < 0 || index >= player[pnum].hand.num_cards || end != (buf + strlen(buf)));

    discard_ref[discard_index++] = player[pnum].hand.cards[index];
    rearrangeHand(pnum, index);
    num_discard--;
  }
}

void sacrifice(int pnum, int class) {
  int index, isvalid = 0;
  char* end, buf[LINE_MAX];

  for (int i = 0; i < player[pnum].stable.size; i++) {
    if (checkClass(class, deck[player[pnum].stable.unicorns[i]].class) &&
      strcmp(deck[player[pnum].stable.unicorns[i]].name, "Puppicorn") != 0) {
      isvalid++;
    }
  }

  if ((class == ANYUNICORN && (player[pnum].flags & pandamonium) != 0) || !isvalid) {
    red();
    printf("%s;", player[pnum].username);
    reset_col();
    printf("There are no available cards to sacrifice\n");
    return;
  }

  printStable(pnum);
  do {
    printf("Pick a valid card number to sacrifice: ");
    fgets(buf, sizeof buf, stdin);
    buf[strlen(buf) - 1] = 0;
    index = strtol(buf, &end, 10) - 1;

    // check for class stuff
    if (index >= 0 && index < player[pnum].stable.size && end == (buf + strlen(buf))) {
      if (!checkClass(class, deck[player[pnum].stable.unicorns[index]].class) ||
        strcmp(deck[player[pnum].stable.unicorns[index]].name, "Puppicorn") == 0 ||
        (class == ANYUNICORN && (player[pnum].flags & pandamonium) != 0)) {
        index = -1;
      }
    }
  } while (index < 0 || index >= player[pnum].stable.size || end != (buf + strlen(buf)));

  // for edge cases where the person who used unicorn lasso sacrifices a unicorn;
  // this is so that it doesn't return an incorrect card (or effect) back to the victim
  if (pnum == uni_lasso_flag[1]) {
    if (index == uni_lasso_flag[0])
      uni_lasso_flag[0] = -1;
    else if (index < uni_lasso_flag[0]) {
      uni_lasso_flag[0]--;
    } // if it's greater than, then it doesn't affect the position of the card
  }

  sacrificeDestroyEffects(pnum, index,
    deck[player[pnum].stable.unicorns[index]].effect);
}

// treat ANYUNICORN as all unicorns when checking class for Unicorn cards
void destroy(int pnum, int class) {
  int pindex, cindex, isvalid = 0;
  char ans, * end, buf[LINE_MAX], buf2[LINE_MAX];

  // commenting out because it should check before it gets destroyed anyways
  // and even though this could be a "just-in-case" thing, there is no 
  // current way to know which card was lost in order to destroy, if there
  // even was a card that was sacrificed or used up
  //for (int pindex = 0; pindex < current_players; pindex++) {
  //  for (int cindex = 0; cindex < player[pindex].stable.size; cindex++) {
  //    if (checkClass(class, deck[player[pindex].stable.unicorns[pindex]].class)) {
  //      // check for Rainbow Aura and Pandemonium flag (where unicorn
  //      // cards/pandas can't be destroyed)
  //      if (!((player[pindex].flags & rainbow_aura) != 0 &&
  //        deck[player[pindex].stable.unicorns[cindex]].species != NOSPECIES) &&
  //        !((player[pindex].flags & pandamonium) != 0 &&
  //          class == ANYUNICORN) &&
  //        strcmp(deck[player[pindex].stable.unicorns[cindex]].name, "Puppicorn") != 0) {
  //        isvalid++;
  //        cindex = DESC_SIZE;
  //        pindex = DESC_SIZE;
  //      }
  //    }
  //  }
  //}

  //if (!isvalid) {
  //  printf("There are no available cards for Player %s to destroy\n", player[pnum].username);
  //  // place where player[pnum] gets their card back maybe
  //  return;
  //}

  printPlayers();
  do {
    printf("Choose a player to destroy from: ");
    fgets(buf, sizeof buf, stdin);
    buf[strlen(buf) - 1] = 0;
    pindex = strtol(buf, &end, 10) - 1;

    // check if player has any valid unicorns to destroy
    if (pindex >= 0 && pindex < current_players && pindex != pnum && end == (buf + strlen(buf))) {
      for (int i = 0; i < player[pindex].stable.size; i++) {
        if (checkClass(class, deck[player[pindex].stable.unicorns[i]].class)) {
          // check for Rainbow Aura and Pandemonium flag (where unicorn
          // cards/pandas can't be destroyed)
          if (!((player[pindex].flags & rainbow_aura) != 0 &&
            deck[player[pindex].stable.unicorns[i]].species != NOSPECIES) &&
            !((player[pindex].flags & pandamonium) != 0 &&
              deck[player[pindex].stable.unicorns[i]].species != NOSPECIES) &&
            strcmp(deck[player[pindex].stable.unicorns[i]].name, "Puppicorn") != 0) {
            isvalid++;
            i = DESC_SIZE;
          }
        }
      }
      if (!isvalid) pindex = -1;
    }
  } while (pindex < 0 || pindex >= current_players || pindex == pnum ||
    end != (buf + strlen(buf)));

  printStable(pindex);
  do {
    printf("Choose the card number to destroy: ");
    fgets(buf, sizeof buf, stdin);
    buf[strlen(buf) - 1] = 0;
    cindex = strtol(buf, &end, 10) - 1;

    // check for class stuff
    if (cindex >= 0 && cindex < player[pindex].stable.size && end == (buf + strlen(buf))) {
      if (!checkClass(class,
        deck[player[pindex].stable.unicorns[cindex]].class) ||
        ((player[pindex].flags & rainbow_aura) != 0 &&
          deck[player[pindex].stable.unicorns[cindex]].species != NOSPECIES) ||
        ((player[pindex].flags & pandamonium) != 0 &&
          deck[player[pindex].stable.unicorns[cindex]].species != NOSPECIES) ||
        strcmp(deck[player[pindex].stable.unicorns[cindex]].name, "Puppicorn") == 0) {
        cindex = -1;
      }
      else if (deck[player[pindex].stable.unicorns[cindex]].species != NOSPECIES &&
        (player[pindex].flags & black_knight_unicorn) != 0 &&
        (player[pindex].flags & blinding_light) == 0) {
        // blinding light isn't active but the black knight unicorn is
        // ask player pindex if they'd like to sacrifice bku instead of cindex (unicorn)
        do {
          printf(
            "Would you like to sacrifice 'Black Knight Unicorn' instead of "
            "card '%s' (y/n)?: ",
            deck[player[pindex].stable.unicorns[cindex]].name);
          fgets(buf2, sizeof buf2, stdin);
          ans = buf2[0];
        } while (ans != 'y' && ans != 'n' && strlen(buf2) != 2);
        if (ans == 'y') {
          for (int i = 0; i < player[pindex].stable.size; i++) {
            if (strcmp(deck[player[pindex].stable.unicorns[i]].name,
              "Black Knight Unicorn") == 0) {
              cindex = i;
              break;
            }
          }
        }
      }
    }
  } while (cindex < 0 || cindex >= player[pindex].stable.size || end != (buf + strlen(buf)));

  sacrificeDestroyEffects(pindex, cindex,
    deck[player[pindex].stable.unicorns[cindex]].effect);
}

// treat ANYUNICORN as all unicorns when checking class for Unicorn cards;
// this is for edge cases where Magic cards are being used to destroy cards
// that cannot be destroyed by those specifically
void destroyMagic(int pnum, int class) {
  int pindex, cindex, isvalid = 0;
  char ans, * end, buf[LINE_MAX], buf2[LINE_MAX];

  printPlayers();
  do {
    printf("Choose a player to destroy from: ");
    fgets(buf, sizeof buf, stdin);
    buf[strlen(buf) - 1] = 0;
    pindex = strtol(buf, &end, 10) - 1;

    // check if player has any valid unicorns to destroy
    if (pindex >= 0 && pindex < current_players && pindex != pnum && end == (buf + strlen(buf))) {
      for (int i = 0; i < player[pindex].stable.size; i++) {
        if (checkClass(class, deck[player[pindex].stable.unicorns[i]].class)) {
          // check for Rainbow Aura, Pandemonium flag (where unicorn
          // cards/pandas can't be destroyed), and the cards Puppicorn and
          // Magical Kittencorn since those cannot be destroyed
          if (!((player[pindex].flags & rainbow_aura) != 0 &&
            deck[player[pindex].stable.unicorns[i]].species != NOSPECIES) &&
            !((player[pindex].flags & pandamonium) != 0 &&
              deck[player[pindex].stable.unicorns[i]].species != NOSPECIES) &&
            strcmp(deck[player[pindex].stable.unicorns[i]].name, "Puppicorn") != 0 &&
            strcmp(deck[player[pindex].stable.unicorns[i]].name, "Magical Kittencorn") != 0) {
            isvalid++;
            i = DESC_SIZE;
          }
        }
      }
      if (!isvalid) pindex = -1;
    }
  } while (pindex < 0 || pindex >= current_players || pindex == pnum ||
    end != (buf + strlen(buf)));

  printStable(pindex);
  do {
    printf("Choose the card number to destroy: ");
    fgets(buf, sizeof buf, stdin);
    buf[strlen(buf) - 1] = 0;
    cindex = strtol(buf, &end, 10) - 1;

    // check for class stuff
    if (cindex >= 0 && cindex < player[pindex].stable.size && end == (buf + strlen(buf))) {
      if (!checkClass(class,
        deck[player[pindex].stable.unicorns[cindex]].class) ||
        ((player[pindex].flags & rainbow_aura) != 0 &&
          deck[player[pindex].stable.unicorns[cindex]].species != NOSPECIES) ||
        ((player[pindex].flags & pandamonium) != 0 &&
          deck[player[pindex].stable.unicorns[cindex]].species != NOSPECIES) ||
        strcmp(deck[player[pindex].stable.unicorns[cindex]].name, "Puppicorn") == 0 ||
        strcmp(deck[player[pindex].stable.unicorns[cindex]].name, "Magical Kittencorn") == 0) {
        cindex = -1;
      }
      else if (deck[player[pindex].stable.unicorns[cindex]].species != NOSPECIES &&
        (player[pindex].flags & black_knight_unicorn) != 0 &&
        (player[pindex].flags & blinding_light) == 0) {
        // blinding light isn't active but the black knight unicorn is
        // ask player pindex if they'd like to sacrifice bku instead of cindex
        do {
          printf(
            "Would you like to sacrifice 'Black Knight Unicorn' instead of "
            "card '%s' (y/n)?: ",
            deck[player[pindex].stable.unicorns[cindex]].name);
          fgets(buf2, sizeof buf2, stdin);
          ans = buf2[0];
        } while (ans != 'y' && ans != 'n' && strlen(buf2) != 2);
        if (ans == 'y') {
          for (int i = 0; i < player[pindex].stable.num_unicorns; i++) {
            if (strcmp(deck[player[pindex].stable.unicorns[i]].name,
              "Black Knight Unicorn") == 0) {
              cindex = i;
              i = DESC_SIZE;
            }
          }
        }
      }
    }
  } while (cindex < 0 || cindex >= player[pindex].stable.size || end != (buf + strlen(buf)));

  sacrificeDestroyEffects(pindex, cindex,
    deck[player[pindex].stable.unicorns[cindex]].effect);
}

// treat ANYUNICORN as all unicorns when checking class for Unicorn cards
void steal(int pnum, int class) {
  int pindex, cindex, isvalid = 0;
  char* end, buf[LINE_MAX];

  printPlayers();
  do {
    printf("Choose a player to steal from: ");
    fgets(buf, sizeof buf, stdin);
    buf[strlen(buf) - 1] = 0;
    pindex = strtol(buf, &end, 10) - 1;

    // check if player has any valid unicorns to steal
    if (pindex >= 0 && pindex < current_players && pindex != pnum && end == (buf + strlen(buf))) {
      for (int i = 0; i < player[pindex].stable.size; i++) {
        if (checkClass(class, deck[player[pindex].stable.unicorns[i]].class)) {
          isvalid++;
          i = DESC_SIZE;
        }
      }
      if (!isvalid) pindex = -1;
    }
  } while (pindex < 0 || pindex >= current_players || pindex == pnum || end != (buf + strlen(buf)));

  printStable(pindex);
  do {
    printf("Choose the card number to steal: ");
    fgets(buf, sizeof buf, stdin);
    buf[strlen(buf) - 1] = 0;
    cindex = strtol(buf, &end, 10) - 1;

    // check for class stuff
    if (cindex >= 0 && cindex < player[pindex].stable.size && end == (buf + strlen(buf))) {
      if (!checkClass(class,
        deck[player[pindex].stable.unicorns[cindex]].class)) {
        cindex = -1;
      }
    }
  } while (cindex < 0 || cindex >= player[pindex].stable.size || end != (buf + strlen(buf)));

  // assign card from chosen player's stable (pindex) to the current player's
  // stable (pnum); trigger any effects and toggle any flags from the chosen
  // player since the card is no longer in play for them (pindex)
  addStable(pnum, player[pindex].stable.unicorns[cindex]);
  rearrangeStable(pindex, cindex);
  enterStableEffects(pnum, deck[player[pnum].stable.unicorns[player[pnum].stable.size - 1]].effect);
  toggleFlags(pindex, deck[player[pnum].stable.unicorns[player[pnum].stable.size - 1]].effect);
}

// ********************************************************************************
// *************************** Core Game Loop Functions ***************************
// ********************************************************************************

// codes the part where players are able to use an instant card against a play
// 0 = nobody used Neigh/Super Neigh or Neigh's cancelled out; 1 = card is gone
// TODO: bug where Neigh remained in player's hand that last refuted it (e.g. 3
// neighs were used in total and when player 3 used americorn on player 1 to get
// a random card, it picked the same Neigh ID that was in the discard pile)
int refutePhase(int pnum, int cindex) {
  int index, index2, oldpindex, isvalid = 0, oddcheck = 0;
  char* end, buf[LINE_MAX];

  red();
  printf("%s is about to play card %s.\n", player[pnum].username,
    deck[player[pnum].hand.cards[cindex]].name);
  reset_col();

  do {
    printf(
      "If a player would like to Neigh this card, then please type in the "
      "player number. Otherwise enter 0 to quit: ");
    fgets(buf, sizeof buf, stdin);
    buf[strlen(buf) - 1] = 0;
    index = strtol(buf, &end, 10) - 1;

    if (index >= 0 && index < current_players && index != pnum && end == (buf + strlen(buf)) && // valid input check
        canNeighOthers(index)) { // neigh check
      for (int i = 0; i < player[index].hand.num_cards; i++) {
        if (deck[player[index].hand.cards[i]].class == INSTANT) {
          isvalid++;
        }
      }
    }

    if (!isvalid && index != -1) index = -2;
  } while (index < -1 || index >= current_players || index == pnum || end != (buf + strlen(buf)));

  if (index == -1) {
    return 0;
  }

  printHand(index);
  do {
    printf("Pick the specific Neigh card to use: ");
    fgets(buf, sizeof buf, stdin);
    buf[strlen(buf) - 1] = 0;
    index2 = strtol(buf, &end, 10) - 1;
    // make sure it's actually a Neigh or Super Neigh
    if (index2 >= 0 && index2 < player[index].hand.num_cards && end == (buf + strlen(buf)))
      if (deck[player[index].hand.cards[index2]].class != INSTANT)
        index2 = -1;
  } while (index2 < 0 || index2 >= player[index].hand.num_cards || end != (buf + strlen(buf)));

  discard_ref[discard_index++] = player[index].hand.cards[index2];
  rearrangeHand(index, index2);

  oddcheck++;

  // card ID of 128 is a Super Neigh, which cannot be refuted; loop until
  // Neigh's have been exhausted
  // TODO: the magic number is kind of disgusting...
  while (discard_ref[discard_index - 1] != 128 && index != -1) {
    isvalid = 0;
    oldpindex = index;
    do {
      printf(
        "If a player would like to negate the previous Neigh card, then "
        "please type in the player number. \nOtherwise enter 0 to quit: ");
      fgets(buf, sizeof buf, stdin);
      buf[strlen(buf) - 1] = 0;
      index = strtol(buf, &end, 10) - 1;

      // flags 2 and 64 indicate new player index cannot use a Neigh card,
      // while flag 4 indicates the previous player oldpindex is unable to be
      // Neigh'd; flag 64 is negated if blinding light (128) is enabled since
      // that renders it a basic unicorn
      if (index != oldpindex && index >= 0 && index < current_players && end == (buf + strlen(buf)) && // valid input check
        canNeighOthers(index) && canBeNeighed(oldpindex)) { // neigh check
        for (int i = 0; i < player[index].hand.num_cards; i++) {
          if (deck[player[index].hand.cards[i]].class == INSTANT) {
            isvalid++;
          }
        }
      }
      if (!isvalid && index != -1) index = -2;
    } while (index < -1 || index >= current_players || end != (buf + strlen(buf)));

    if (index != -1) {
      printHand(index);
      do {
        printf("Pick the specific Neigh card to use: ");
        fgets(buf, sizeof buf, stdin);
        buf[strlen(buf) - 1] = 0;
        index2 = strtol(buf, &end, 10) - 1;
        // make sure it's actually a Neigh or Super Neigh
        if (index2 >= 0 && index2 < player[index].hand.num_cards && end == (buf + strlen(buf)))
          if (deck[player[index].hand.cards[index2]].class != INSTANT)
            index2 = -1;
      } while (index2 < 0 || index2 >= player[index].hand.num_cards || end != (buf + strlen(buf)));

      discard_ref[discard_index++] = player[index].hand.cards[index2];
      rearrangeHand(index, index2);

      oddcheck++;
    }
  }

  // the neigh went through
  if (oddcheck & 1) {
    discard_ref[discard_index++] = player[pnum].hand.cards[cindex];
    rearrangeHand(pnum, cindex);
    return 1;
  }

  return 0;
}

void playCard(int pnum) {
  int index, index2;
  char* end, buf[LINE_MAX];

  do {
    printf("Pick the card number you'd like to play (non-instant card): ");
    // fgets(buf, sizeof buf, stdin);
    // buf[strlen(buf) - 1] = 0;
    // index = strtol(buf, &end, 10) - 1;
    index = numinput(buf, &end, sizeof buf);

    if (index >= 0 && index < player[pnum].hand.num_cards && end == (buf + strlen(buf)))
      if (deck[player[pnum].hand.cards[index]].class == INSTANT)
        index = -1;
  } while (index < 0 || index >= player[pnum].hand.num_cards || end != (buf + strlen(buf)));

  // check if the player cannot be Neigh'd first before initiating the func
  if (canBeNeighed(pnum))
    if (refutePhase(pnum, index)) return;

  // get the card ID before rearranging the hand so that when you discard
  // cards it doesn't show the one you just used when you're doing the
  // effect
  int cardid = player[pnum].hand.cards[index];

  // depending on class, decide whether it goes to stable or discard pile
  if (deck[cardid].class == BASICUNICORN) {
    if ((player[pnum].flags & queen_bee_unicorn) != 0) {
      printf("%s is unable to play Basic Unicorns due to someone else's "
        "Queen Bee Unicorn\n", player[pnum].username);
      turn_count++;
      return;
    }
    rearrangeHand(pnum, index);
    addStable(pnum, cardid);
  }
  else if (deck[cardid].class == BABYUNICORN ||
    deck[cardid].class == MAGICUNICORN) {
    rearrangeHand(pnum, index);
    addStable(pnum, cardid);
    if (strcmp(deck[cardid].name, "Puppicorn") == 0) {
      puppicorn_index = player[pnum].stable.size - 1;
    }
  }
  else if (deck[cardid].class == UPGRADE ||
    deck[cardid].class == DOWNGRADE) {
    // check for Broken Stable flag
    if (((player[pnum].flags & broken_stable) != 0) &&
      deck[cardid].class == UPGRADE) {
      printf(
        "Player %d is unable to play Upgrade cards due to the effect of "
        "'Broken Stable'\n",
        pnum + 1);
      turn_count++;
      return;
    }
    printPlayers();
    do {
      printf("Choose a player to give the upgrade/downgrade card: ");
      fgets(buf, sizeof buf, stdin);
      buf[strlen(buf) - 1] = 0;
      index2 = strtol(buf, &end, 10) - 1;
    } while (index2 < 0 || index2 >= current_players || end != (buf + strlen(buf)));

    // Upgrade/Downgrade card effects here since they may affect a different
    // stable; index2 is the affected player while pnum is current player
    toggleFlags(index2, deck[cardid].effect);

    // assign card from current player's hand (pnum) to the chosen stable
    // (index2)
    if (conditionalEffects(pnum, deck[cardid].effect, cardid, index, index2) == 2) {
      player[index2].stable.unicorns[player[index2].stable.size++] =
        cardid;
      rearrangeHand(pnum, index);
    }
  }
  else {
    // magic cards go to the discard pile!
    if (conditionalEffects(pnum, deck[cardid].effect, cardid, index, pnum) == 2) {
      discard_ref[discard_index++] = cardid;
      rearrangeHand(pnum, index);
      magicEffects(pnum, deck[cardid].effect);
    }
  }

  enterStableEffects(pnum, deck[cardid].effect);
}
