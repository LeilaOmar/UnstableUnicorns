#include "gamemechanics.h"
#include "networkfuncs.h"

#define BUFSIZE 200

// ******************** Utility Functions ********************

// declare these in advance so they can be used in the card effect functions
void discard(int pnum, int num_discard, int class);
void sacrifice(int pnum, int class);
void enterStableEffects(int pnum, int effect);
void sacrificeDestroyEffects(int pnum, int cindex, int effect);
void toggleFlags(int pnum, int effect);

// hash functions for scrambling IP addresses
// https://stackoverflow.com/questions/664014/what-integer-hash-function-are-good-that-accepts-an-integer-hash-key
unsigned int hash(unsigned int x) {
  x = ((x >> 16) ^ x) * 0x45d9f3b;
  x = ((x >> 16) ^ x) * 0x45d9f3b;
  x = (x >> 16) ^ x;
  return x;
}
unsigned int unhash(unsigned int x) {
  x = ((x >> 16) ^ x) * 0x119de1f3;
  x = ((x >> 16) ^ x) * 0x119de1f3;
  x = (x >> 16) ^ x;
  return x;
}

// IP stuff
// https://stackoverflow.com/questions/39566240/how-to-get-the-external-ip-address-in-c
// unfortunately the internetopen way didn't work :(
void getIPreq(char *ip_address) {
  //WSADATA wsaData;
  struct addrinfo hints;
  struct addrinfo* result = NULL;
  int sd, err;
  char buffer[BUFSIZE] = { 0 };

  // api.ipify.org is only for IPv4; api64.ipify.org is both v4 and v6 w/ priority for v6
  // **don't forget to change it in getaddrinfo too if ever**
  char* httpreq = "GET / HTTP/1.1\r\nHost: api.ipify.org\r\nConnection: close\r\n\r\n";

  //if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
  //  // error handling
  //  return;
  //  //cout << "WSAStartup failed.\n";
  //  //system("pause");
  //}

  ZeroMemory(&hints, sizeof(hints));
  hints.ai_family = AF_UNSPEC; // allows for both ipv4 and ipv6
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;
  err = getaddrinfo("api.ipify.org", "80", &hints, &result);
  if (err != 0)
  {
    // error handling
    abort();
    //fprintf(stderr, "%s: %s\n", url, gai_strerror(err));
  }


  for (struct addrinfo* addr = result; addr != NULL; addr = addr->ai_next)
  {
    sd = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
    if (sd == -1)
    {
      err = errno;
      continue; // if using AF_UNSPEC above instead of AF_INET/6 specifically,
                // replace this 'break' with 'continue' instead, as the 'ai_family'
                // may be different on the next iteration...
    }

    if (connect(sd, addr->ai_addr, addr->ai_addrlen) == 0)
      break;

    err = errno;
  }

  send(sd, httpreq, strlen(httpreq), 0);
  recv(sd, buffer, BUFSIZE, 0);

  freeaddrinfo(result);
  closesocket(sd);
  //WSACleanup();

  // this only works because the text is the only output from the site
  // therefore it's the last line and does not have a newline after it
  int index = 0;
  for (int i = BUFSIZE - 1; i >= 0; i--) {
    if (buffer[i] == '\n') {
      index = i + 1;
      break;
    }
  }

  // using sizeof(ip) for MaxCount may not be appropriate since the length is variable
  // it still works though since everything afterwards is \0 *shrugs in laziness*
  strncpy_s(ip_address, 16, buffer + index, 16);
}

// uses hash function to get code for ip (and vice versa); only for IPv4
unsigned int IPtoHexCode(char* ip) {
  struct in_addr addr;
  inet_pton(AF_INET, ip, &addr);
  unsigned int num = ntohl(addr.s_addr); // convert network byte to machine byte order

  return hash(num);
}

void HexCodetoIP(char* code, char* dest) {
  memset(dest, 0, 16); // reset ip in case it was entered before somehow
  unsigned int tmp = unhash(strtoul(code, NULL, 16)); // 16 represents base
  sprintf_s(dest, 16, "%d.%d.%d.%d", (tmp & 0xFF000000) >> 24, (tmp & 0x00FF0000) >> 16, (tmp & 0x0000FF00) >> 8, tmp & 0x000000FF);
}

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

// OPTIMIZE: use species UNICORN instead of class ANYUNICORN and then combine
// this with checking for species too
// returns 1 if the desired class matches with the card's class, otherwises
// return 0
int checkClass(int desired_class, int card_class) {
  if (desired_class == ANY || desired_class == card_class ||
    (desired_class == ANYUNICORN &&
      (card_class == BABYUNICORN || card_class == BASICUNICORN ||
        card_class == MAGICUNICORN))) {
    return 1;
  }
  return 0;
}

// randomize deck between specific indeces
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

// ******************** Card Effect Functions ********************
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

// ******************** Misc Game Mechanics ********************

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

    // check for flags that make the player unable to use Neigh cards
    if (index >= 0 && index < current_players && index != pnum && end == (buf + strlen(buf)) &&
      (player[index].flags & slowdown) == 0 &&
      ((player[index].flags & ginormous_unicorn) == 0 ||
        ((player[index].flags & ginormous_unicorn) != 0) &&
        (player[index].flags & blinding_light) != 0)) {
      for (int i = 0; i < player[index].hand.num_cards; i++) {
        if (deck[player[index].hand.cards[i]].class == INSTANT) {
          isvalid++;
        }
      }
    }
    if (!isvalid && index != -1) index = -2;
  } while (index < -1 || index >= current_players || index == pnum || end != (buf + strlen(buf)));

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

    // card ID of 128 is a Super Neigh, which cannot be refuted; loop until
    // Neigh's have been exhausted
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
        if (index != oldpindex && index >= 0 && index < current_players && end == (buf + strlen(buf)) &&
          (player[index].flags & slowdown) == 0 &&
          (player[oldpindex].flags & yay) == 0 &&
          ((player[index].flags & ginormous_unicorn) == 0 ||
            ((player[index].flags & ginormous_unicorn) != 0) &&
            (player[index].flags & blinding_light) != 0)) {
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
  }

  if (oddcheck & 1) {
    discard_ref[discard_index++] = player[pnum].hand.cards[cindex];
    rearrangeHand(pnum, cindex);
    return 1;
  }

  return 0;
}

// switch cases for cards that may trigger specific player flags (e.g. cannot
// neigh)
void toggleFlags(int pnum, int effect) {
  switch (effect) {
  case 0:
    break;
  case 35:
    // Playing Black Knight Unicorn
    // if a unicorn card in your stable would be destroyed, you may
    // SACRIFICE this card instead
    player[pnum].flags ^= black_knight_unicorn;
    break;
  case 36:
    // Playing Tiny Stable:
    // If at any time you have more than 5 Unicorns in your Stable,
    // SACRIFICE a Unicorn card
    player[pnum].flags ^= tiny_stable;
    break;
  case 37:
    // Playing Barbed Wire:
    // Each time a Unicorn card enters or leaves your Stable, DISCARD a
    // card. 
    player[pnum].flags ^= barbed_wire;
    break;
  case 46:
    // Playing Nanny Cam:
    // your hand must be visible to all players
    player[pnum].flags ^= nanny_cam;
    break;
  case 50:
    // Playing Queen Bee Unicorn:
    // basic unicorn cards cannot enter any player's stable except yours
    for (int i = 0; i < current_players; i++) {
      if (i != pnum) player[i].flags ^= queen_bee_unicorn;
    }
    break;
  case 52:
    // Playing Rainbow Aura:
    // your unicorn cards cannot be destroyed
    player[pnum].flags ^= rainbow_aura;
    break;
  case 53:
    // Playing Broken Stable:
    // you cannot play any upgrade cards
    player[pnum].flags ^= broken_stable;
    break;
  case 54:
    // Playing Slowdown:
    // you cannot play any neigh cards
    player[pnum].flags ^= slowdown;
    break;
  case 55:
    // Playing Ginormous Unicorn:
    // this card counts for 2 unicorns. you cannot play any neigh cards
    player[pnum].flags ^= ginormous_unicorn;
    break;
  case 56:
    // Playing Yay:
    // cards you play cannot be neigh'd
    player[pnum].flags ^= yay;
    break;
  case 60:
    // Playing Blinding Light:
    // all of your unicorn cards are considered basic unicorns with
    // no effects
    player[pnum].flags ^= blinding_light;
    break;
  case 61:
    // Playing Pandamonium
    // all of your unicorns are considered pandas. cards that
    // affect unicorn cards do not affect your pandas
    player[pnum].flags ^= pandamonium;
    break;
  default:
    break;
  }
}

// switch cases for when a card has a special effect upon being sacrificed or
// destroyed
void sacrificeDestroyEffects(int pnum, int cindex, int effect) {
  int isvalid = 0;
  char ans, buf[LINE_MAX];

  // Blinding Light makes it so that all unicorn cards have no effects; exempt
  // Black Knight Unicorn (effect 35) and Ginormous Unicorn (effect 55) because
  // their flags still needs to be toggled
  if ((player[pnum].flags & blinding_light) == blinding_light &&
    deck[player[pnum].stable.unicorns[cindex]].species != NOSPECIES &&
    effect != 35 && effect != 50 && effect != 55)
    effect = 0;

  switch (effect) {
  case 0:
    break;
  case 1:
  case 7:
  case 38:
  case 39:
  case 40:
    // Playing XYZ Flying Unicorn
    // if this card is sacrificed or destroyed, return it to your hand
    player[pnum].hand.cards[player[pnum].hand.num_cards++] =
      player[pnum].stable.unicorns[cindex];
    rearrangeStable(pnum, cindex);
    return;  // leave the function early
  case 32:
    // Playing Stabby the Unicorn
    // if this card is sacrificed or destroyed, you may DESTROY a unicorn
    // card

    // check if there are unicorn cards to destroy; rainbow aura protects
    // unicorn cards and pandamonium makes it so that your "pandas" don't
    // classify as unicorn cards; inner loop for puppicorn *blegh*
    for (int i = 0; i < current_players; i++) {
      if ((player[i].flags & rainbow_aura) == 0 &&
        (player[i].flags & pandamonium) == 0 && i != pnum) {
        for (int j = 0; j < player[i].stable.size; j++) {
          if (deck[player[i].stable.unicorns[j]].species != NOSPECIES &&
            strcmp(deck[player[i].stable.unicorns[j]].name, "Puppicorn") != 0) {
            isvalid++;
            i = DESC_SIZE;
            break;
          }
        }
      }
    }
    if (!isvalid) break;

    do {
      printf("Would you like to destroy a Unicorn card (y/n)?: ");
      fgets(buf, sizeof buf, stdin);
      ans = buf[0];
    } while ((ans != 'y' && ans != 'n') || strlen(buf) != 2);
    if (ans == 'y') destroy(pnum, ANYUNICORN);
    break;
  case 33:
    // Playing Unicorn Phoenix
    // if this card would be sacrificed or destroyed, you may DISCARD a
    // unicorn card instead

    // check if there are unicorn cards to discard
    for (int i = 0; i < player[pnum].hand.num_cards; i++) {
      if (deck[player[pnum].hand.cards[i]].species != NOSPECIES) {
        isvalid++;
        i = DESC_SIZE;
      }
    }
    if (!isvalid) break;

    do {
      printf(
        "Would you like to discard a Unicorn card instead of disposing "
        "the card 'Unicorn Phoenix' (y/n)?: ");
      fgets(buf, sizeof buf, stdin);
      ans = buf[0];
    } while ((ans != 'y' && ans != 'n') || strlen(buf) != 2);
    if (ans == 'y') discard(pnum, 1, ANYUNICORN);
    return;

    // the following card effects are "turned off" after being sacrificed or
    // destroyed
  case 35:
    // Playing Black Knight Unicorn
    // if a unicorn card in your stable would be destroyed, you may
    // SACRIFICE this card instead
    player[pnum].flags ^= 1024;
    break;
  case 36:
    // Playing Tiny Stable:
    // If at any time you have more than 5 Unicorns in your Stable,
    // SACRIFICE a Unicorn card
    player[pnum].flags ^= 512;
    break;
  case 37:
    // Playing Barbed Wire:
    // Each time a Unicorn card enters or leaves your Stable, DISCARD a
    // card. 
    player[pnum].flags ^= 2048;
    break;
  case 46:
    // Playing Nanny Cam:
    // your hand must be visible to all players
    player[pnum].flags ^= 16;
    break;
  case 50:
    // Playing Queen Bee Unicorn:
    // basic unicorn cards cannot enter any player's stable except yours
    for (int i = 0; i < current_players; i++) {
      if (i != pnum) player[i].flags ^= 32;
    }
    break;
  case 52:
    // Playing Rainbow Aura:
    // your unicorn cards cannot be destroyed
    player[pnum].flags ^= 8;
    break;
  case 53:
    // Playing Broken Stable:
    // you cannot play any upgrade cards
    player[pnum].flags ^= 1;
    break;
  case 54:
    // Playing Slowdown:
    // you cannot play any neigh cards
    player[pnum].flags ^= 2;
    break;
  case 55:
    // Playing Ginormous Unicorn:
    // this card counts for 2 unicorns. you cannot play any neigh cards
    player[pnum].flags ^= 64;
    break;
  case 56:
    // Playing Yay:
    // cards you play cannot be neigh'd
    player[pnum].flags ^= 4;
    break;
  case 60:
    // Playing Blinding Light:
    // all of your unicorn cards are considered basic unicorns with
    // no effects
    player[pnum].flags ^= 128;
    break;
  case 61:
    // Playing Pandamonium
    // all of your unicorns are considered pandas. cards that
    // affect unicorn cards do not affect your pandas
    player[pnum].flags ^= 256;
    break;
  default:
    break;
  }

  if (deck[player[pnum].stable.unicorns[cindex]].class == BABYUNICORN)
    addNursery(player[pnum].stable.unicorns[cindex]);
  else
    discard_ref[discard_index++] = player[pnum].stable.unicorns[cindex];

  rearrangeStable(pnum, cindex);
}

// switch cases for cards that may be unable to be played due to certain
// circumstances; this is separated so that the hand is not rearranged
// unnecessarily; 1 means success, 0 means it returned prematurely, 2 means
// the effect isn't here
int conditionalEffects(int pnum, int effect, int cardid, int hindex, int upgrade_target) {
  int index, index2, index3, isvalid = 0, isvalid2 = 0, tmp_hand[HAND_SIZE];
  char ans, * end, buf[LINE_MAX];

  switch (effect) {
  case 0:
    break;
  case 6:
    // Playing Extra Tail:
    // can only enter a stable if there is a Basic Unicorn card there
    for (int i = 0; i < player[upgrade_target].stable.size; i++) {
      if (deck[player[upgrade_target].stable.unicorns[i]].class == BASICUNICORN) {
        player[upgrade_target].stable.unicorns[player[upgrade_target].stable.size++] =
          cardid;
        rearrangeHand(pnum, hindex);
        return 1;
      }
    }

    printf(
      "You are unable to play 'Extra Tail' because there aren't any "
      "Basic Unicorn cards in the chosen player's stable\n");
    turn_count++;
    return 0;
  case 20:
    // Playing Two-For-One:
    // SACRIFICE 1 card, then DESTROY 2 cards

    // check if there are at least 2 cards to destroy; rainbow aura protects
    // unicorn cards and pandamonium makes it so that your "pandas" don't
    // classify as unicorn cards; inner loop for puppicorn *blegh*
    for (int i = 0; i < current_players; i++) {
      if (i != pnum && ((player[i].flags & rainbow_aura) == 0 ||
        ((player[i].flags & rainbow_aura) != 0 && player[i].stable.size != player[i].stable.num_unicorns) ||
        ((player[i].flags & rainbow_aura) != 0 && (player[i].flags & pandamonium) != 0))) {
        for (int j = 0; j < player[i].stable.size; j++) {
          if (strcmp(deck[player[i].stable.unicorns[j]].name, "Puppicorn") != 0 &&
            strcmp(deck[player[i].stable.unicorns[j]].name, "Magical Kittencorn") != 0) {
            isvalid++;
          }
        }
      }
    }

    // quit and reassign the card to the player's hand
    if (isvalid < 2 || player[pnum].stable.size == 0) {
      printf(
        "You are unable to play 'Two-For-One' because there are "
        "not enough cards to Sacrifice and/or Destroy\n");
      turn_count++;
      return 0;
    }

    discard_ref[discard_index++] = cardid;
    rearrangeHand(pnum, hindex);
    sacrifice(pnum, ANY);
    destroyMagic(pnum, ANY);
    destroyMagic(pnum, ANY);
    return 1;
  case 24:
    // Playing Targeted Destruction:
    // you must DESTROY an upgrade card or SACRIFICE a downgrade card

    for (int j = 0; j < current_players; j++) {
      for (int i = 0; i < player[j].stable.size; i++) {
        if (j != pnum && deck[player[j].stable.unicorns[i]].class == UPGRADE)
          isvalid = 1;
        if (j == pnum &&
          deck[player[j].stable.unicorns[i]].class == DOWNGRADE)
          isvalid2 = 1;
      }
      // quit if both conditions have been met
      if (isvalid == 1 && isvalid2 == 1) j = current_players;
    }
    // quit if there isn't an upgrade card or downgrade card available
    if (isvalid == 0 && isvalid2 == 0) {
      printf(
        "You are unable to play 'Targeted Destruction' because there are "
        "no Upgrade or Downgrade cards in place\n");
      turn_count++;
      return 0;
    }

    discard_ref[discard_index++] = cardid;
    rearrangeHand(pnum, hindex);

    do {
      printf(
        "Pick between the following options:"
        "\n  1. Destroy an upgrade card"
        "\n  2. Sacrifice a downgrade card"
        "\nChoice: ");
      fgets(buf, sizeof buf, stdin);
      ans = buf[0];
    } while ((ans != '1' && ans != '2') || (ans == '1' && isvalid != 1) ||
      (ans == '2' && isvalid2 != 1) || strlen(buf) != 2);

    if (ans == '1') {
      // destroy an upgrade card (this doesn't need to be in the special function)
      destroy(pnum, UPGRADE);
    }
    else if (ans == '2') {
      // sacrifice a downgrade card
      sacrifice(pnum, DOWNGRADE);
    }
    return 1;
  case 26:
    // Playing Unicorn Poison:
    // DESTROY 1 unicorn card

    // check if there's at least 1 unicorn card to destroy first
    for (int i = 0; i < current_players; i++) {
      if ((player[i].flags & rainbow_aura) == 0 &&
        (player[i].flags & pandamonium) == 0 && i != pnum) {
        for (int j = 0; j < player[i].stable.size; j++) {
          if (strcmp(deck[player[i].stable.unicorns[j]].name, "Magical Kittencorn") != 0 &&
            strcmp(deck[player[i].stable.unicorns[j]].name, "Puppicorn") != 0) {
            isvalid++;
            i = DESC_SIZE;
            break;
          }
        }
      }
    }

    // quit and reassign the card to the player's hand
    if (!isvalid) {
      printf(
        "You are unable to play 'Unicorn Poison' because there are "
        "no available cards to Destroy\n");
      turn_count++;
      return 0;
    }

    discard_ref[discard_index++] = cardid;
    rearrangeHand(pnum, hindex);
    destroyMagic(pnum, ANYUNICORN);
    return 1;
  case 29:
    // Playing Unicorn Swap:
    // move a unicorn card from your stable to any other player's then STEAL
    // a unicorn card from that player's stable

    // check if there are unicorn cards to steal
    for (int i = 0; i < current_players; i++) {
      if (i != pnum && player[i].stable.num_unicorns > 0) {
        isvalid++;
        break;
      }
    }

    if (!isvalid) {
      printf(
        "You are unable to play 'Unicorn Swap' because there are no "
        "Unicorn cards to steal\n");
      turn_count++;
      return 0;
    }

    discard_ref[discard_index++] = cardid;
    rearrangeHand(pnum, hindex);

    printPlayers();
    do {
      printf("Choose a player to swap unicorn cards with: ");
      fgets(buf, sizeof buf, stdin);
      buf[strlen(buf) - 1] = 0;
      index = strtol(buf, &end, 10) - 1;
      // check if chosen player actually has a unicorn card
      if (index > 0 && index < current_players && index != pnum && end == (buf + strlen(buf)))
        if (player[index].stable.num_unicorns <= 0)
          index = -1;  // invalidate index if there's nothing to steal
    } while (index < 0 || index >= current_players || index == pnum || end != (buf + strlen(buf)));

    printStable(pnum);
    do {
      printf("Choose a valid unicorn card number to move to %s's stable: ",
        player[index].username);
      fgets(buf, sizeof buf, stdin);
      buf[strlen(buf) - 1] = 0;
      index2 = strtol(buf, &end, 10) - 1;
      if (index2 > 0 && index2 < player[pnum].stable.size && end == (buf + strlen(buf)))
        if (deck[player[pnum].stable.unicorns[index2]].species == NOSPECIES)
          index2 = -1;
    } while (index2 < 0 || index2 >= player[pnum].stable.size || end != (buf + strlen(buf)));

    // add the unicorn card to the other player (index), then update the current
    // player's stable to remove it properly
    addStable(index, player[pnum].stable.unicorns[index2]);
    rearrangeStable(pnum, index2);

    enterStableEffects(
      index,
      deck[player[index].stable.unicorns[player[index].stable.size - 1]]
      .effect);
    toggleFlags(pnum,
      deck[player[index].stable.unicorns[player[index].stable.size - 1]].effect);

    // steal a unicorn from the chosen player's stable
    printStable(index);
    do {
      printf("Choose the card number to steal: ");
      fgets(buf, sizeof buf, stdin);
      buf[strlen(buf) - 1] = 0;
      index2 = strtol(buf, &end, 10) - 1;

      // check for class stuff
      if (index2 >= 0 && index2 < player[index].stable.size && end == (buf + strlen(buf))) {
        if (deck[player[index].stable.unicorns[index2]].species == NOSPECIES) {
          index2 = -1;
        }
      }
    } while (index2 < 0 || index2 >= player[index].stable.size || end != (buf + strlen(buf)));

    // assign card from chosen player's stable (pindex) to the current player's
    // stable (pnum); trigger any effects and toggle any flags from the chosen
    // player since the card is no longer in play for them (pindex)
    addStable(pnum, player[index].stable.unicorns[index2]);
    rearrangeStable(index, index2);
    enterStableEffects(pnum, deck[player[pnum].stable.unicorns[player[pnum].stable.size - 1]].effect);
    toggleFlags(index, deck[player[pnum].stable.unicorns[player[pnum].stable.size - 1]].effect);
    return 1;
  case 30:
    // Playing Unicorn Shrinkray:
    // move all player's unicorn cards to the discard pile without
    // triggering any of their effects, then bring the same number of baby
    // unicorns from the nursery directly into that player's stable

    for (int i = 0; i < current_players; i++) {
      for (int j = 0; j < player[i].stable.size; j++) {
        if (deck[player[i].stable.unicorns[j]].class == BASICUNICORN ||
          deck[player[i].stable.unicorns[j]].class == MAGICUNICORN) {
          isvalid++;
        }
      }
      // adds to a counter check if nursery size is less than the amount of non-baby unicorns
      if ((int)dnurse_size < isvalid && i != pnum) {
        isvalid2++;
      }
      isvalid = 0;
    }

    // if there aren't enough baby unicorns in the nursery, quit
    if (isvalid2 == current_players - 1) {
      printf(
        "You are unable to play 'Unicorn Shrinkray' because there aren't "
        "enough available Baby Unicorns to replace any player's stable\n");
      turn_count++;
      return 0;
    }

    discard_ref[discard_index++] = cardid;
    rearrangeHand(pnum, hindex);

    printPlayers();
    do {
      printf("Choose a player to swap their stable with Baby Unicorns: ");
      fgets(buf, sizeof buf, stdin);
      buf[strlen(buf) - 1] = 0;
      index = strtol(buf, &end, 10) - 1;
      if (index >= 0 && index < current_players && index != pnum && end == (buf + strlen(buf))) {
        for (int i = 0; i < player[index].stable.size; i++) {
          if (deck[player[index].stable.unicorns[i]].class == BASICUNICORN ||
            deck[player[index].stable.unicorns[i]].class == MAGICUNICORN) {
            isvalid++;
          }
        }
        if (isvalid > (int)dnurse_size)
          index = -1;
        isvalid = 0;
      }
    } while (index < 0 || index >= current_players || index == pnum || end != (buf + strlen(buf)));

    for (int i = 0; i < player[index].stable.size; i++) {
      if ((deck[player[index].stable.unicorns[i]].class == BASICUNICORN ||
        deck[player[index].stable.unicorns[i]].class == MAGICUNICORN) &&
        strcmp(deck[player[index].stable.unicorns[i]].name, "Puppicorn") != 0) {
        // some unicorns may have flag effects that need to be removed
        toggleFlags(index, deck[player[index].stable.unicorns[i]].effect);
        discard_ref[discard_index++] = player[index].stable.unicorns[i];
        player[index].stable.unicorns[i] = nursery_ref[nursery_index++];
        dnurse_size--;
      }
    }

    // barbed wire check; that's a lot of unicorns to discard!
    if ((player[index].flags & barbed_wire) != 0)
      discard(index, player[index].stable.num_unicorns * 2, ANY);

    return 1;
  case 31:
    // Playing Re-Target:
    // move an upgrade or downgrade card from any player's stable to any
    // other player's stable

    for (int j = 0; j < current_players; j++) {
      for (int i = 0; i < player[j].stable.size; i++) {
        if (deck[player[j].stable.unicorns[i]].class == UPGRADE ||
          deck[player[j].stable.unicorns[i]].class == DOWNGRADE) {
          isvalid = 1;
          // quit if either condition was met
          j = DESC_SIZE;
          break;
        }
      }
    }

    if (!isvalid) {
      printf(
        "You are unable to play 'Re-Target' because there are no Upgrade "
        "or Downgrade cards in place\n");
      turn_count++;
      return 0;
    }

    discard_ref[discard_index++] = cardid;
    rearrangeHand(pnum, hindex);

    printPlayers();
    do {
      printf("Choose a player to take an upgrade or downgrade card from: ");
      fgets(buf, sizeof buf, stdin);
      buf[strlen(buf) - 1] = 0;
      index = strtol(buf, &end, 10) - 1;

      // make sure chosen player has an upgrade or downgrade card
      if (index >= 0 && index < current_players && end == (buf + strlen(buf))) {
        for (int j = 0; j < current_players; j++) {
          for (int i = 0; i < player[j].stable.size; i++) {
            if (deck[player[j].stable.unicorns[i]].class == UPGRADE ||
              deck[player[j].stable.unicorns[i]].class == DOWNGRADE) {
              isvalid2 = 1;
              // quit if either condition was met
              j = DESC_SIZE;
              break;
            }
          }
        }
        if (!isvalid2) index = -1;
      }
    } while (index < 0 || index >= current_players || end != (buf + strlen(buf)));

    printStable(index);
    do {
      printf("Choose a valid card number to take: ");
      fgets(buf, sizeof buf, stdin);
      buf[strlen(buf) - 1] = 0;
      index2 = strtol(buf, &end, 10) - 1;
      if (index2 > 0 && index2 < player[index].stable.size && end == (buf + strlen(buf)))
        if (deck[player[index].stable.unicorns[index2]].class != UPGRADE &&
          deck[player[index].stable.unicorns[index2]].class != DOWNGRADE)
          index2 = -1;  // invalidate index2 if the class doesn't align
    } while (index2 < 0 || index2 >= player[index].stable.size || end != (buf + strlen(buf)));

    printPlayers();
    do {
      printf("Choose a player to give the taken upgrade or downgrade card: ");
      fgets(buf, sizeof buf, stdin);
      buf[strlen(buf) - 1] = 0;
      index3 = strtol(buf, &end, 10) - 1;
      // can't be the same as the player it was taken from (i.e. player #
      // index)
    } while (index3 < 0 || index3 >= current_players || index3 == index || end != (buf + strlen(buf)));

    player[index3].stable.unicorns[player[index3].stable.size++] =
      player[index].stable.unicorns[index2];
    rearrangeStable(index, index2);

    // disables or enables certain effects/flags for both players if
    // applicable
    toggleFlags(
      index,
      deck[player[index3].stable.unicorns[player[index3].stable.size - 1]]
      .effect);
    toggleFlags(
      index3,
      deck[player[index3].stable.unicorns[player[index3].stable.size - 1]]
      .effect);
    return 1;
  case 41:
    // Playing Back Kick:
    // return a card in another player's stable to their hand. that player
    // must DISCARD a card

    for (int i = 0; i < current_players; i++) {
      if (player[i].stable.size > 0) {
        isvalid = 1;
        break;
      }
    }

    if (isvalid == 0) {
      printf(
        "You are unable to play 'Back Kick' because there are no cards "
        "in other player's stables\n");
      turn_count++;
      return 0;
    }

    discard_ref[discard_index++] = cardid;
    rearrangeHand(pnum, hindex);

    printPlayers();
    do {
      printf(
        "Choose a player to return a card from their stable to their "
        "hand: ");
      fgets(buf, sizeof buf, stdin);
      buf[strlen(buf) - 1] = 0;
      index = strtol(buf, &end, 10) - 1;

      if (index > 0 && index < current_players && index != pnum && end == (buf + strlen(buf)))
        if (player[index].stable.size == 0)
          index = -1;  // invalidate index if there are no cards to return
    } while (index < 0 || index >= current_players || index == pnum || end != (buf + strlen(buf)));

    printStable(index);
    do {
      printf("Choose a valid card number to return: ");
      fgets(buf, sizeof buf, stdin);
      buf[strlen(buf) - 1] = 0;
      index2 = strtol(buf, &end, 10) - 1;
    } while (index2 < 0 || index2 >= player[index].stable.size || end != (buf + strlen(buf)));

    // return it to their hand; if baby unicorn return it to the nursery
    if (deck[player[index].stable.unicorns[index2]].class == BABYUNICORN)
      addNursery(player[index].stable.unicorns[index2]);
    else {
      // "disable" the puppicorn swap since it was returned to someone's hand
      if (strcmp(deck[player[index].stable.unicorns[index2]].name, "Puppicorn") == 0) {
        puppicorn_index = -1;
      }
      player[index].hand.cards[player[index].hand.num_cards++] =
        player[index].stable.unicorns[index2];
    }

    toggleFlags(index, deck[player[index].stable.unicorns[index2]].effect);
    rearrangeStable(index, index2);
    discard(index, 1, ANY);
    return 1;
  case 44:
    // Playing Blatant Thievery:
    // look at another player's hand. choose a card and add it to your hand

    for (int i = 0; i < current_players; i++) {
      if (i != pnum && player[i].hand.num_cards > 0) {
        isvalid = 1;
        i = current_players;  // quit the for loop
      }
    }

    if (isvalid == 0) {
      printf(
        "You are unable to play 'Blatant Thievery' because no other "
        "players have any cards in their hands\n");
      turn_count++;
      return 0;
    }

    discard_ref[discard_index++] = cardid;
    rearrangeHand(pnum, hindex);

    printPlayers();
    do {
      printf("Choose a player to steal a random card from their hand: ");
      fgets(buf, sizeof buf, stdin);
      buf[strlen(buf) - 1] = 0;
      index = strtol(buf, &end, 10) - 1;
      if (index > 0 && index < current_players && index != pnum && end == (buf + strlen(buf)))
        if (player[index].hand.num_cards == 0) index = -1;
    } while (index < 0 || index >= current_players || index == pnum || end != (buf + strlen(buf)));

    printHand(index);
    do {
      printf("Choose a card to steal: ");
      fgets(buf, sizeof buf, stdin);
      buf[strlen(buf) - 1] = 0;
      index2 = strtol(buf, &end, 10) - 1;
    } while (index2 < 0 || index2 >= player[index].hand.num_cards || end != (buf + strlen(buf)));

    // readjust hands and num_card lengths
    player[pnum].hand.cards[player[pnum].hand.num_cards++] =
      player[index].hand.cards[index2];
    rearrangeHand(index, index2);
    return 1;
  case 45:
    // Playing Unfair Bargain:
    // trade hands with any other player

    for (int i = 0; i < current_players; i++) {
      if (i != pnum && player[i].hand.num_cards > 0) {
        isvalid = 1;
        break;
      }
    }

    if (isvalid == 0) {
      printf(
        "You are unable to play 'Unfair Bargain' because no other "
        "players have any cards in their hands\n");
      turn_count++;
      return 0;
    }

    discard_ref[discard_index++] = cardid;
    rearrangeHand(pnum, hindex);

    printPlayers();
    do {
      printf("Choose a player to trade hands with: ");
      fgets(buf, sizeof buf, stdin);
      buf[strlen(buf) - 1] = 0;
      index = strtol(buf, &end, 10) - 1;

      if (index > 0 && index < current_players && index != pnum && end == (buf + strlen(buf)))
        if (player[index].hand.num_cards == 0)
          index = -1;
    } while (index < 0 || index >= current_players || index == pnum || end != (buf + strlen(buf)));

    for (int i = 0;
      i < player[index].hand.num_cards * (player[index].hand.num_cards >
        player[pnum].hand.num_cards) +
      player[pnum].hand.num_cards * (player[pnum].hand.num_cards >
        player[index].hand.num_cards);
      i++) {
      // swap each element until i hits the max number between the highest
      // number of cards in each player's hand
      tmp_hand[i] = player[index].hand.cards[i];
      player[index].hand.cards[i] = player[pnum].hand.cards[i];
      player[pnum].hand.cards[i] = tmp_hand[i];
    }
    // swap num_cards info too
    tmp_hand[0] = player[index].hand.num_cards;
    player[index].hand.num_cards = player[pnum].hand.num_cards;
    player[pnum].hand.num_cards = tmp_hand[0];
    return 1;
  case 58:
    // Playing Rainbow Mane:
    // can only enter a stable if there is a Basic Unicorn card there
    for (int i = 0; i < player[upgrade_target].stable.size; i++) {
      if (deck[player[upgrade_target].stable.unicorns[i]].class == BASICUNICORN) {
        player[upgrade_target].stable.unicorns[player[upgrade_target].stable.size++] =
          cardid;
        rearrangeHand(pnum, hindex);
        return 1;
      }
    }

    printf(
      "You are unable to play 'Rainbow Mane' because there aren't any "
      "Basic Unicorn cards in the chosen player's stable\n");
    turn_count++;
    return 0;
  default:
    break;
  }

  return 2;
}

// switch cases for enter your stable effects with magical unicorn cards
void enterStableEffects(int pnum, int effect) {
  int index, index2;
  char ans, * end, buf[LINE_MAX];
  int isvalid = 0, isvalid2 = 0;

  // Blinding Light makes it so that all unicorn cards have no effects
  if ((player[pnum].flags & blinding_light) != 0) return;

  switch (effect) {
  case 0:
    break;
  case 1:
    // Playing Greedy Flying Unicorn:
    // DRAW 1 card
    draw(pnum, 1);
    break;
  case 2:
    // Playing Unicorn on the Cob:
    // DRAW 2 cards and DISCARD 1 card
    draw(pnum, 2);
    discard(pnum, 1, ANY);
    break;
  case 7:
    // Playing Annoying Flying Unicorn:
    // you may force another player to DISCARD 1 card

    // check if other players have at least 1 card
    for (int i = 0; i < current_players; i++) {
      if (player[i].hand.num_cards > 0 && i != pnum) {
        isvalid++;
        i = DESC_SIZE;
      }
    }

    if (!isvalid) break;

    printPlayers();
    do {
      printf("Choose a player other than yourself to discard a card: ");
      fgets(buf, sizeof buf, stdin);
      buf[strlen(buf) - 1] = 0;
      index = strtol(buf, &end, 10) - 1;
      if (index >= 0 && index < current_players && end == (buf + strlen(buf)))
        if (player[index].hand.num_cards == 0)
          index = -1;
    } while (index < 0 || index >= current_players || index == pnum || end != (buf + strlen(buf)));
    discard(index, 1, ANY);
    break;
  case 10:
    // Playing Seductive Unicorn:
    // you may DISCARD 1 card, then STEAL a unicorn card
    do {
      printf(
        "Would you like to discard a card in order to steal a unicorn "
        "card (y/n)?: ");
      fgets(buf, sizeof buf, stdin);
      ans = buf[0];
    } while ((ans != 'y' && ans != 'n') || strlen(buf) != 2);

    if (ans == 'y') {
      discard(pnum, 1, ANY);
      steal(pnum, ANYUNICORN);
    }
    break;
  case 12:
    // Playing Llamacorn:
    // each player must DISCARD 1 card.
    for (int i = 0; i < current_players; i++) {
      discard(i, 1, ANY);
    }
    break;
  case 16:
    // Playing Extremely Destructive Unicorn:
    // each player must SACRIFICE 1 unicorn card

    for (int i = 0; i < current_players; i++) {
      sacrifice(i, ANYUNICORN);
    }
    break;
  case 17:
    // Playing Narwhal Torpedo
    // SACRIFICE all downgrade cards
    for (int i = 0; i < player[pnum].stable.size; i++) {
      if (deck[player[pnum].stable.unicorns[i]].class == DOWNGRADE) {
        discard_ref[discard_index++] = player[pnum].stable.unicorns[i];
        toggleFlags(pnum, deck[player[pnum].stable.unicorns[i]].effect);
        rearrangeStable(pnum, i);
      }
    }
    break;
  case 18:
    // Playing Shark With a Horn
    // you may SACRIFICE this card, then destroy a unicorn card

    // check if there are unicorn cards to destroy; rainbow aura protects
    // unicorn cards and pandemonium makes it so that your "pandas" don't
    // classify as unicorn cards; inner loop for puppicorn *blegh*
    // Unhandled exception at 0x00854874 in UnstableUnicorns.exe: 0xC0000005: Access violation reading location 0x0087FBCC
    // Exception thrown at 0x00854874 in UnstableUnicorns.exe: 0xC0000005: Access violation reading location 0x0087FBCC
    for (int i = 0; i < current_players; i++) {
      if ((player[i].flags & rainbow_aura) == 0 &&
        (player[i].flags & pandamonium) == 0 && i != pnum) {
        for (int j = 0; j < player[i].stable.size; j++) {
          if (deck[player[i].stable.unicorns[j]].species != NOSPECIES &&
            strcmp(deck[player[i].stable.unicorns[j]].name, "Puppicorn") != 0) {
            isvalid++;
            i = DESC_SIZE;
            break;
          }
        }
      }
    }
    if (!isvalid) break;

    do {
      printf(
        "Would you like to sacrifice this card in order to destroy a "
        "Unicorn card (y/n)?: ");
      fgets(buf, sizeof buf, stdin);
      ans = buf[0];
    } while ((ans != 'y' && ans != 'n') || strlen(buf) != 2);
    if (ans == 'y') {
      // decrease the size before using that variable to get the last
      // inserted unicorn in the stable
      discard_ref[discard_index++] =
        player[pnum].stable.unicorns[--player[pnum].stable.size];
      destroy(pnum, ANYUNICORN);
    }
    break;
  case 23:
    // Playing Chainsaw Unicorn:
    // you may DESTROY an upgrade card or SACRIFICE a downgrade card
    for (int j = 0; j < current_players; j++) {
      for (int i = 0; i < player[j].stable.size; i++) {
        if (j != pnum && deck[player[j].stable.unicorns[i]].class == UPGRADE)
          isvalid = 1;
        if (j == pnum &&
          deck[player[j].stable.unicorns[i]].class == DOWNGRADE)
          isvalid2 = 1;
      }
      // quit if both conditions have been met
      if (isvalid == 1 && isvalid2 == 1) j = current_players;
    }
    // quit if there isn't an upgrade card or downgrade card available
    if (isvalid == 0 && isvalid2 == 0) break;

    do {
      printf(
        "Pick between the following options:"
        "\n  1. Destroy an upgrade card"
        "\n  2. Sacrifice a downgrade card"
        "\n  3. Nothing"
        "\nChoice: ");
      fgets(buf, sizeof buf, stdin);
      ans = buf[0];
    } while ((ans != '1' && ans != '2' && ans != '3') || strlen(buf) != 2 ||
      (ans == '1' && isvalid != 1) || (ans == '2' && isvalid2 != 1));

    if (ans == '1') {
      // destroy an upgrade card
      destroy(pnum, UPGRADE);
    }
    else if (ans == '2') {
      // sacrifice a downgrade card
      sacrifice(pnum, DOWNGRADE);
    }
    break;
  case 27:
    // Playing Alluring Narwhal:
    // STEAL 1 upgrade card (e.y.s.)
    for (int i = 0; i < current_players; i++) {
      for (int j = 0; j < player[i].stable.size; j++) {
        if (deck[player[i].stable.unicorns[j]].class == UPGRADE && i != pnum) {
          isvalid++;
          i = DESC_SIZE;
          break;
        }
      }
    }

    if (!isvalid) return;

    steal(pnum, UPGRADE);
    break;
  case 35:
    // Playing Black Knight Unicorn
    // if a unicorn card in your stable would be destroyed, you may
    // SACRIFICE this card instead
    player[pnum].flags |= 1024;
    break;
  case 38:
    // Playing Majestic Flying Unicorn:
    // you may add a Unicorn card from the discard pile to your hand
    searchDiscard(pnum, ANYUNICORN);
    break;
  case 39:
    // Playing Magical Flying Unicorn:
    // you may add a Magic card from the discard pile to your hand
    searchDiscard(pnum, MAGIC);
    break;
  case 40:
    // Playing Swift Flying Unicorn:
    // you may add a Neigh card from the discard pile to your hand
    searchDiscard(pnum, INSTANT);
    break;
  case 42:
    // Playing Mermaid Unicorn:
    // return a card in another player's stable to their hand
    for (int i = 0; i < current_players; i++) {
      if (i != pnum && player[i].stable.size > 0) {
        isvalid = 1;
        i = DESC_SIZE;  // quit the for loop
      }
    }

    // you don't have to use the special effect so do nothing if there's no
    // valid card
    if (isvalid == 0) break;

    printPlayers();
    do {
      printf(
        "Choose a player to return a card from their stable to their "
        "hand: ");
      fgets(buf, sizeof buf, stdin);
      buf[strlen(buf) - 1] = 0;
      index = strtol(buf, &end, 10) - 1;
      if (index > 0 && index < current_players && index != pnum && end == (buf + strlen(buf)))
        if (player[index].stable.size == 0)
          index = -1;
    } while (index < 0 || index >= current_players || index == pnum || end != (buf + strlen(buf)));

    printStable(index);
    do {
      printf("Choose a valid card number to return: ");
      fgets(buf, sizeof buf, stdin);
      buf[strlen(buf) - 1] = 0;
      index2 = strtol(buf, &end, 10) - 1;
    } while (index2 < 0 || index2 >= player[index].stable.size || end != (buf + strlen(buf)));

    // return it to their hand; if baby unicorn return it to the nursery
    if (deck[player[index].stable.unicorns[index2]].class == BABYUNICORN)
      addNursery(player[index].stable.unicorns[index2]);
    else {
      // "disable" the puppicorn swap since it was returned to someone's hand
      if (strcmp(deck[player[index].stable.unicorns[index2]].name, "Puppicorn") == 0) {
        puppicorn_index = -1;
      }
      player[index].hand.cards[player[index].hand.num_cards++] =
        player[index].stable.unicorns[index2];
    }

    toggleFlags(index, deck[player[index].stable.unicorns[index2]].effect);
    rearrangeStable(index, index2);
    break;
  case 43:
    // Playing Americorn:
    // you may pull a card at random from another player's hand into your
    // hand

    // check if other players actually have at least one card
    for (int i = 0; i < current_players; i++) {
      if (i != pnum && player[i].hand.num_cards > 0) {
        isvalid = 1;
        i = DESC_SIZE;  // quit the for loop
      }
    }

    if (!isvalid) break;

    printPlayers();
    do {
      printf("Choose a player to steal a random card from their hand: ");
      fgets(buf, sizeof buf, stdin);
      buf[strlen(buf) - 1] = 0;
      index = strtol(buf, &end, 10) - 1;
      if (index >= 0 && index < current_players && end == (buf + strlen(buf)))
        if (player[index].hand.num_cards == 0)
          index = -1;
    } while (index < 0 || index >= current_players || index == pnum || end != (buf + strlen(buf)));

    // set index2 to be some random card within the chosen player's range
    index2 = rand() % (player[index].hand.num_cards + 1);

    // discard time! pnum is current player, index is the stolen player
    // number and index2 is the card item being transferred
    player[pnum].hand.cards[player[pnum].hand.num_cards++] =
      player[index].hand.cards[index2];
    rearrangeHand(index, index2);
    break;
  case 47:
    // Playing Classy Narwhal
    // search the deck for an upgrade card and add it to your hand. shuffle
    // the deck
    searchDeck(pnum, UPGRADE, ANY);
    break;
  case 48:
    // Playing Shabby the Narwhal
    // search the deck for a downgrade card and add it to your hand. shuffle
    // the deck
    searchDeck(pnum, DOWNGRADE, ANY);
    break;
  case 49:
    // Playing The Great Narwhal
    // search the deck for a card with "Narwhal" in its name and add it to
    // your hand. shuffle the deck
    searchDeck(pnum, ANY, NARWHAL);
    break;
  case 50:
    // Playing Queen Bee Unicorn:
    // basic unicorn cards cannot enter any player's stable except yours
    for (int i = 0; i < current_players; i++) {
      if (i != pnum) player[i].flags ^= 32;
    }
    break;
  case 55:
    // Playing Ginormous Unicorn:
    // this card counts for 2 unicorns. you cannot play any neigh cards
    player[pnum].flags ^= 64;
    break;
  case 59:
    // Playing Rainbow Unicorn
    // bring a basic unicorn card from your hand directly into your
    // stable

    // someone else has a queen bee unicorn, so the player is unable to play
    // basic unicorns
    if ((player[pnum].flags & queen_bee_unicorn) != 0) break;

    for (int i = 0; i < player[pnum].hand.num_cards; i++) {
      // use the player's card ID (taken from deck_ref) to access the main
      // deck to check the original cards classification
      if (deck[player[pnum].hand.cards[i]].class == BASICUNICORN) {
        isvalid = 1;
        i = player[pnum].hand.num_cards;  // quit the for loop
      }
    }

    // don't have to check before because this is an optional effect
    if (isvalid == 0) break;

    printHand(pnum);
    do {
      printf("Choose a Basic Unicorn to bring into your stable: ");
      fgets(buf, sizeof buf, stdin);
      buf[strlen(buf) - 1] = 0;
      index = strtol(buf, &end, 10) - 1;
      // make sure it's actually a basic unicorn
      if (index >= 0 && index < player[pnum].hand.num_cards && end == (buf + strlen(buf)))
        if (deck[player[pnum].hand.cards[index]].class != BASICUNICORN)
          index = -1;
    } while (index < 0 || index >= player[pnum].hand.num_cards || end != (buf + strlen(buf)));

    addStable(pnum, player[pnum].hand.cards[index]);
    rearrangeHand(pnum, index);
    break;
  default:
    break;
  }
}

// switch cases for Magic cards
void magicEffects(int pnum, int effect) {
  int index;
  char* end, buf[LINE_MAX];

  switch (effect) {
  case 0:
    break;
  case 3:
    // Playing Change of Luck:
    // DRAW 2 cards and DISCARD 3 cards; take another turn
    draw(pnum, 2);
    discard(pnum, 3, ANY);
    turn_count++;
    break;
  case 4:
    // Playing Good Deal:
    // DRAW 3 cards and DISCARD 1 card
    draw(pnum, 3);
    discard(pnum, 1, ANY);
    break;
  case 5:
    // Playing Shake Up:
    // shuffle your card, hand, and discard pile into the deck. DRAW 5 cards
    for (int i = 0; i < player[pnum].hand.num_cards; i++) {
      // move all cards from hand to the discard pile
      discard_ref[discard_index++] = player[pnum].hand.cards[i];
    }
    player[pnum].hand.num_cards = 0;  // oof actually forgot this...
    shuffleDiscard();
    draw(pnum, 5);
    break;
  case 9:
    // Playing Mystical Vortex:
    // every player must DISCARD 1 card. shuffle the discard pile into the
    // deck
    for (int i = 0; i < current_players; i++) {
      discard(i, 1, ANY);
    }
    shuffleDiscard();
    break;
  case 19:
    // Playing Reset Button:
    // each player must SACRIFICE all upgrade/downgrade cards. shuffle
    // discard pile into the deck
    for (int j = 0; j < current_players; j++) {
      for (int i = 0; i < player[j].stable.size; i++) {
        if (deck[player[j].stable.unicorns[i]].class == DOWNGRADE ||
          deck[player[j].stable.unicorns[i]].class == UPGRADE) {
          discard_ref[discard_index++] = player[j].stable.unicorns[i];
          toggleFlags(j, deck[player[j].stable.unicorns[i]].effect);
          rearrangeStable(j, i);
        }
      }
    }
    shuffleDiscard();
    break;
  case 63:
    // Playing Glitter Tornado
    // Return a card in each player's Stable (including yours) to their
    // hand.
    for (int i = 0; i < current_players; i++) {
      if (player[i].stable.num_unicorns > 0) {
        printStable(i);
        do {
          printf("Choose a valid card number to return: ");
          fgets(buf, sizeof buf, stdin);
          buf[strlen(buf) - 1] = 0;
          index = strtol(buf, &end, 10) - 1;
        } while (index < 0 || index >= player[i].stable.size || end != (buf + strlen(buf)));

        // if the returned card is puppicorn, disable the swap flag
        if (strcmp(deck[player[i].stable.unicorns[index]].name, "Puppicorn") == 0) {
          puppicorn_index = -1;
        }

        // return it to their hand; if baby unicorn return it to the nursery
        if (deck[player[i].stable.unicorns[index]].class == BABYUNICORN)
          addNursery(player[i].stable.unicorns[index]);
        else
          player[i].hand.cards[player[i].hand.num_cards++] =
          player[i].stable.unicorns[index];

        // for edge cases where the person who used unicorn lasso sacrifices a unicorn;
        // this is so that it doesn't return an incorrect card (or effect) back to the victim
        if (pnum == uni_lasso_flag[1]) {
          if (index == uni_lasso_flag[0])
            uni_lasso_flag[0] = -1;
          else if (index < uni_lasso_flag[0]) {
            uni_lasso_flag[0]--;
          } // if it's greater than, then it doesn't affect the position of the card
        }

        toggleFlags(i, deck[player[i].stable.unicorns[index]].effect);
        rearrangeStable(i, index);
      }
    }
    break;
  default:
    break;
  }
}

// switch cases for beginning of your turn effects
void beginningTurnEffects(int pnum, int effect) {
  int index, index2 = 0, isvalid = 0, isvalid2 = 0;
  char ans, * end, buf[LINE_MAX];

  // Blinding Light makes it so that all unicorn cards have no effects;
  // the listed effects are upgrade/downgrade cards
  // OPTIMIZE: put this in a different function so there's no need to check
  // all those different effects?
  if ((player[pnum].flags & blinding_light) != 0 &&
    effect != 13 && effect != 15 && effect != 21 &&
    effect != 28 && effect != 57 && effect != 58)
    return;

  switch (effect) {
  case 0:
    break;
  case 6:
    // Playing Extra Tail:
    // you may draw 1 card
    draw(pnum, 1);
    break;
  case 8:
    // Playing Extremely Fertile Unicorn:
    // you may discard 1 card to bring a baby unicorn directly to your
    // stable

    // check if there are actually cards in hand, otherwise break;
    if (player[pnum].hand.num_cards == 0) break;

    do {
      printf(
        "Would you like to discard 1 card in order to bring a baby "
        "unicorn directly to your stable (y/n)?: ");
      fgets(buf, sizeof buf, stdin);
      ans = buf[0];
    } while ((ans != 'y' && ans != 'n') || strlen(buf) != 2);
    if (ans == 'y') {
      // discard then bring in a baby unicorn :D
      discard(pnum, 1, ANY);
      printNursery(nursery_index, NURSERY_SIZE);
      do {
        printf("Pick the index associated with your desired Baby Unicorn: ");
        fgets(buf, sizeof buf, stdin);
        buf[strlen(buf) - 1] = 0;
        index = strtol(buf, &end, 10) - 1;
      } while (index < 0 || index >= (int)dnurse_size || end != (buf + strlen(buf)));

      addStable(pnum, nursery_ref[index + nursery_index]);
      rearrangeNursery(index + nursery_index);
    }
    break;
  case 11:
    // Playing Zombie Unicorn:
    // you may discard 1 unicorn card to bring a unicorn card from the
    // discard pile directly to your stable, then immediately skip to end of
    // turn phase

    // check if there are valid unicorn cards to discard
    for (int i = 0; i < player[pnum].hand.num_cards; i++) {
      if (deck[player[pnum].hand.cards[i]].class == BASICUNICORN ||
        deck[player[pnum].hand.cards[i]].class == MAGICUNICORN) {
        isvalid++;
        i = DESC_SIZE;  // rand number larger than variable for faster
                        // access
      }
    }

    // check if there are actually cards available to take
    for (size_t i = 0; i < discard_index; i++) {
      if (deck[discard_ref[i]].class == BASICUNICORN ||
        deck[discard_ref[i]].class == MAGICUNICORN) {
        isvalid2++;
        i = DESC_SIZE;
      }
    }

    // no valid cards are available
    if (isvalid == 0 || isvalid2 == 0) break;

    do {
      printf(
        "Would you like to discard a unicorn card in order to bring a "
        "unicorn card from the discard pile directly to your stable at "
        "the expense of immediately ending your turn (y/n)?: ");
      fgets(buf, sizeof buf, stdin);
      ans = buf[0];
    } while ((ans != 'y' && ans != 'n') || strlen(buf) != 2);
    if (ans == 'y') {
      // OPTIMIZE: write searchDiscard in a way that accepts your hand AND
      // stable
      discard(pnum, 1, ANYUNICORN);
      printDiscard(ANYUNICORN);

      do {
        printf("Pick a valid card number to add to your stable: ");
        fgets(buf, sizeof buf, stdin);
        buf[strlen(buf) - 1] = 0;
        index = strtol(buf, &end, 10) - 1;

        if (index >= 0 && index < (int)discard_index && end == (buf + strlen(buf)))
          if (deck[discard_ref[index]].class != BASICUNICORN &&
            deck[discard_ref[index]].class != MAGICUNICORN)
            index = -1;
      } while (index < 0 || index >= (int)discard_index || end != (buf + strlen(buf)));

      addStable(pnum, discard_ref[index]);
      rearrangeDiscard(index);

      // reduce turn_count in favor of "ending turn"
      turn_count = -1;
    }
    break;
  case 13:
    // Playing Summoning Ritual:
    // you may discard 2 unicorn cards to bring a unicorn card directly from
    // the discard pile into your stable

    // check if there are actually cards available to take
    for (size_t i = 0; i < discard_index; i++) {
      if (deck[discard_ref[i]].class == BASICUNICORN ||
        deck[discard_ref[i]].class == MAGICUNICORN) {
        isvalid++;
      }
    }

    // no valid cards are available
    if (isvalid < 2) break;

    do {
      printf(
        "Would you like to discard 2 unicorn cards in order to bring a "
        "unicorn card from the discard pile directly to your stable "
        "(y/n)?: ");
      fgets(buf, sizeof buf, stdin);
      ans = buf[0];
    } while ((ans != 'y' && ans != 'n') || strlen(buf) != 2);
    if (ans == 'y') {
      discard(pnum, 2, ANYUNICORN);
      printDiscard(ANYUNICORN);

      do {
        printf("Pick a valid card number to add to your stable: ");
        fgets(buf, sizeof buf, stdin);
        buf[strlen(buf) - 1] = 0;
        index = strtol(buf, &end, 10) - 1;

        if (index >= 0 && index < (int)discard_index && end == (buf + strlen(buf)))
          if (deck[discard_ref[index]].class != BASICUNICORN &&
            deck[discard_ref[index]].class != MAGICUNICORN)
            index = -1;
      } while (index < 0 || index >= (int)discard_index || end != (buf + strlen(buf)));

      addStable(pnum, discard_ref[index]);
      rearrangeDiscard(index);
    }
    break;
  case 14:
    // Playing Angel Unicorn:
    // you may sacrifice this card and bring a unicorn card from the discard
    // pile directly into your stable

    // check if there are actually cards available to take
    for (size_t i = 0; i < discard_index; i++) {
      if (deck[discard_ref[i]].class == BASICUNICORN ||
        deck[discard_ref[i]].class == MAGICUNICORN) {
        isvalid++;
      }
    }

    // no valid cards are available
    if (isvalid == 0) return;

    do {
      printf(
        "Would you like to sacrifice this card, the Angel Unicorn, in "
        "order to bring a unicorn card from the discard pile directly to "
        "your stable (y/n)?: ");
      fgets(buf, sizeof buf, stdin);
      ans = buf[0];
    } while ((ans != 'y' && ans != 'n') || strlen(buf) != 2);
    if (ans == 'y') {
      for (int i = 0; i < player[pnum].stable.size; i++) {
        // ID of Angel Unicorn is 51
        if (strcmp(deck[player[pnum].stable.unicorns[i]].name, "Angel Unicorn") == 0) {
          index2 = i;
          break;
        }
      }
      discard_ref[discard_index++] = player[pnum].stable.unicorns[index2];
      rearrangeStable(pnum, index2);
      printDiscard(ANYUNICORN);

      do {
        printf("Pick a valid card number to add to your stable: ");
        fgets(buf, sizeof buf, stdin);
        buf[strlen(buf) - 1] = 0;
        index = strtol(buf, &end, 10) - 1;

        if (index >= 0 && index < (int)discard_index && end == (buf + strlen(buf))) {
          if (deck[discard_ref[index]].class != BASICUNICORN &&
            deck[discard_ref[index]].class != MAGICUNICORN)
            index = -1;
        }
      } while (index < 0 || index >= (int)discard_index || end != (buf + strlen(buf)));

      addStable(pnum, discard_ref[index]);
      rearrangeDiscard(index);
    }
    break;
  case 15:
    // Playing Sadistic Ritual:
    // you must sacrifice 1 unicorn card, then draw a card

    sacrifice(pnum, ANYUNICORN);
    draw(pnum, 1);
    break;
  case 21:
    // Playing Glitter Bomb:
    // you may sacrifice a card, then destroy a card

    // check if there are cards to sacrifice and destroy first; only worry
    // about /that/ card in terms of sacrificing here since this can only
    // be the current player's turn
    for (int i = 0; i < current_players; i++) {
      if (i != pnum && player[i].stable.size > 0 && ((player[i].flags & rainbow_aura) == 0 ||
        ((player[i].flags & rainbow_aura) != 0 &&
          (player[i].stable.size != player[i].stable.num_unicorns || (player[i].flags & pandamonium) != 0)))) {
        isvalid = 1;
        break;
      }
    }

    if (player[pnum].stable.size <= 0 || !isvalid ||
      (player[pnum].stable.size == 1 && strcmp(deck[player[pnum].stable.unicorns[0]].name, "Puppicorn") == 0))
      break;

    do {
      printf(
        "Would you like to sacrifice a card in order to destroy another "
        "card (y/n)?: ");
      fgets(buf, sizeof buf, stdin);
      ans = buf[0];
    } while ((ans != 'y' && ans != 'n') || strlen(buf) != 2);
    if (ans == 'y') {
      sacrifice(pnum, ANY);
      destroy(pnum, ANY);
    }
    break;
  case 25:
    // Playing Rhinocorn:
    // you may destroy a unicorn card, then immediately end your turn

    // check if there are unicorn cards to destroy; rainbow aura protects
    // unicorn cards and pandemonium makes it so that your "pandas" don't
    // classify as unicorn cards; inner loop for puppicorn *blegh*
    for (int i = 0; i < current_players; i++) {
      if ((player[i].flags & rainbow_aura) == 0 &&
        (player[i].flags & pandamonium) == 0 && i != pnum) {
        for (int j = 0; j < player[i].stable.size; j++) {
          if (deck[player[i].stable.unicorns[j]].species != NOSPECIES &&
            strcmp(deck[player[i].stable.unicorns[j]].name, "Puppicorn") != 0) {
            isvalid++;
            i = DESC_SIZE;
            break;
          }
        }
      }
    }
    if (!isvalid) break;

    do {
      printf(
        "Would you like to destroy a unicorn card at the expense of "
        "ending your turn (y/n)?: ");
      fgets(buf, sizeof buf, stdin);
      ans = buf[0];
    } while ((ans != 'y' && ans != 'n') || strlen(buf) != 2);
    if (ans == 'y') {
      destroy(pnum, ANYUNICORN);
      turn_count = -1;
    }
    break;
  case 28:
    // Playing Unicorn Lasso:
    // you may steal a unicorn card, and then return it at the end of your
    // turn

    // check if there are unicorn cards to steal
    for (int i = 0; i < current_players; i++) {
      if (i != pnum && player[i].stable.num_unicorns > 0) {
        isvalid++;
        i = DESC_SIZE;
      }
    }

    if (!isvalid) break;

    do {
      printf(
        "Would you like to steal a unicorn card and then return it at "
        "the end of your turn (y/n)?: ");
      fgets(buf, sizeof buf, stdin);
      ans = buf[0];
    } while ((ans != 'y' && ans != 'n') || strlen(buf) != 2);

    // just copied the steal function because it has to be modified to fit
    // in the special flag
    if (ans == 'y') {
      printPlayers();
      do {
        printf("Choose a player to steal from: ");
        fgets(buf, sizeof buf, stdin);
        buf[strlen(buf) - 1] = 0;
        index = strtol(buf, &end, 10) - 1;
        // double checks that the chosen player actually has a unicorn card
        if (index >= 0 && index < current_players && index != pnum && end == (buf + strlen(buf))) {
          if (player[index].stable.num_unicorns <= 0) {
            index = -1;
          }
        }
      } while (index < 0 || index >= current_players || index == pnum || end != (buf + strlen(buf)));

      printStable(index);
      do {
        printf("Choose the card number to steal: ");
        fgets(buf, sizeof buf, stdin);
        buf[strlen(buf) - 1] = 0;
        index2 = strtol(buf, &end, 10) - 1;

        // check that the chosen card is actually a unicorn
        if (index2 >= 0 && index2 < player[index].stable.size && end == (buf + strlen(buf))) {
          if (deck[player[index].stable.unicorns[index2]].class !=
            BABYUNICORN &&
            deck[player[index].stable.unicorns[index2]].class !=
            BASICUNICORN &&
            deck[player[index].stable.unicorns[index2]].class !=
            MAGICUNICORN) {
            index2 = -1;
          }
        }
      } while (index2 < 0 || index2 >= player[index].stable.size || end != (buf + strlen(buf)));

      // keep track of the card index for pnum (which since it's the last
      // entry, it should equal the current stable size), current player ID,
      // and stolen player's ID in order to return the card at the end of
      // the turn
      uni_lasso_flag[0] = player[pnum].stable.size;
      uni_lasso_flag[1] = pnum;
      uni_lasso_flag[2] = index;

      // assign card from chosen player's stable (pindex) to the current player's
      // stable (pnum); trigger any effects and toggle any flags from the chosen
      // player since the card is no longer in play for them (pindex)
      addStable(pnum, player[index].stable.unicorns[index2]);
      rearrangeStable(index, index2);
      toggleFlags(index, deck[player[pnum].stable.unicorns[uni_lasso_flag[0]]].effect);
      enterStableEffects(pnum, deck[player[pnum].stable.unicorns[uni_lasso_flag[0]]].effect);
    }
    break;
  case 57:
    // Playing Double Dutch:
    // you may play 2 cards during your Action phase

    turn_count++;
    break;
  case 58:
    // Playing Rainbow Mane:
    // you may bring a Basic Unicorn card from your hand directly into your
    // stable

    // check if Basic Unicorns are even in hand
    for (int i = 0; i < player[pnum].hand.num_cards; i++) {
      if (deck[player[pnum].hand.cards[i]].class == BASICUNICORN) {
        isvalid++;
      }
    }

    if (!isvalid) break;

    do {
      printf("Choose a valid card number to place into your stable: \n");
      for (int i = 0; i < player[pnum].hand.num_cards; i++) {
        if (deck[player[pnum].hand.cards[i]].class == BASICUNICORN) {
          isvalid++;
          printf("    %d. %s [ID: %d]\n", i + 1,
            deck[player[pnum].hand.cards[i]].name,
            player[pnum].hand.cards[i]);
        }
      }
      printf("Choice: ");
      fgets(buf, sizeof buf, stdin);
      buf[strlen(buf) - 1] = 0;
      index = strtol(buf, &end, 10) - 1;
      if (index >= 0 && index < player[pnum].hand.num_cards && end == (buf + strlen(buf))) {
        if (deck[player[pnum].hand.cards[index]].class != BASICUNICORN) {
          index = -1;
        }
      }
    } while (index < 0 || index >= player[pnum].hand.num_cards || end != (buf + strlen(buf)));

    addStable(pnum, player[pnum].hand.cards[index]);
    rearrangeHand(pnum, index);
    break;
  default:
    break;
  }
}

void playCard(int pnum) {
  int index, index2;
  char* end, buf[LINE_MAX];

  do {
    printf("Pick the card number you'd like to play (non-instant card): ");
    fgets(buf, sizeof buf, stdin);
    buf[strlen(buf) - 1] = 0;
    index = strtol(buf, &end, 10) - 1;

    if (index >= 0 && index < player[pnum].hand.num_cards && end == (buf + strlen(buf)))
      if (deck[player[pnum].hand.cards[index]].class == INSTANT)
        index = -1;
  } while (index < 0 || index >= player[pnum].hand.num_cards || end != (buf + strlen(buf)));

  // check if the player cannot be Neigh'd first before initiating the func
  if ((player[pnum].flags & yay) == 0)
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