#pragma once
#define WINDOWS_IGNORE_PACKING_MISMATCH
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib,"ws2_32.lib") // Winsock library

#define NAME_SIZE 32
#define DESC_SIZE 256
#define LINE_MAX 32
#define BUF_SIZE 1024
#define HAND_START 5
#define HAND_SIZE 16
#define MAX_PLAYERS 8
#define DECK_SIZE 116
#define STABLE_SIZE 25
#define NURSERY_SIZE 13

enum UnicornDatatypes {ANY = -1, BABYUNICORN, BASICUNICORN, MAGICUNICORN, UPGRADE, DOWNGRADE, MAGIC, INSTANT, ANYUNICORN, UNICORN, NARWHAL, NOSPECIES};

enum flag {broken_stable = 1, slowdown = 2, yay = 4, rainbow_aura = 8,
           nanny_cam = 16, queen_bee_unicorn = 32, ginormous_unicorn = 64,
           blinding_light = 128, pandamonium = 256, tiny_stable = 512,
           black_knight_unicorn = 1024, barbed_wire = 2048};

struct Unicorn {
  unsigned char class;    // 0 = Baby Unicorn
                          // 1 = Basic Unicorn
                          // 2 = Magic Unicorn
                          // 3 = Upgrade
                          // 4 = Downgrade
                          // 5 = Magic
                          // 6 = Instant
  unsigned char species;  // 8 = Unicorn
                          // 9 = Narwhal
                          // 10 = Other or N/A [for now...]
  char name[NAME_SIZE];
  char description[DESC_SIZE];
  short
    effect;  // switch table; 0 = nothing
             // 1 = DRAW 1 card (when this card enters your stable - >
             // referred to as e.y.s.). if this card is sacrificed or
             // destroyed, return it to your hand
             // 2 = DRAW 2 cards and DISCARD 1 card (e.y.s.)
             // 3 = DRAW 2 cards and DISCARD 3 cards (then
             // take another turn)
             // 4 = DRAW 3 cards and DISCARD 1 card
             // 5 = shuffle your card, hand, and discard pile into the deck.
             // DRAW 5 cards
             // 6 = DRAW 1 card (if this card is in your stable at the
             // beginning of your turn -> referred to as b.o.t.); this card
             // can only enter a stable if there is a basic unicorn card
             // there
             // 7 = force another player to DISCARD 1 card (e.y.s.).
             // if this card is sacrificed or destroyed, return it to your
             // hand
             // 8 = DISCARD 1 card, then bring a baby unicorn from the
             // nursery directly into your stable (b.o.t.)
             // 9 = every player
             // must DISCARD 1 card. shuffle the discard pile into the deck
             // (e.y.s.)
             // 10 = DISCARD 1 card, then STEAL a unicorn card (e.y.s.)
             // 11 = DISCARD 1 card, then choose a unicorn card
             // from the discard pile and bring it directly into your
             // stable, then immediately end your turn (b.o.t.)
             // 12 = each player must DISCARD 1 card. shuffle the discard pile
             // into the deck (no trigger)
             // 13 = DISCARD 2 cards, then bring a unicorn card directly from
             // the discard pile into your stable (b.o.t.)
             // 14 = SACRIFICE this card, then choose a unicorn from the
             // discard pile and bring it directly into your stable (b.o.t.)
             // 15 = SACRIFICE 1 unicorn card, then DRAW a card (b.o.t.)
             // 16 = each player must SACRIFICE 1 card (e.y.s.)
             // 17 = SACRIFICE all downgrade cards (e.y.s.)
             // 18 = SACRIFICE this card, then destroy a unicorn card (e.y.s.)
             // 19 = SACRIFICE all upgrade/downgrade cards (all players).
             // shuffle discard pile into the deck (no trigger)
             // 20 = SACRIFICE 1 card, then DESTROY 2 cards (no trigger)
             // 21 = SACRIFICE 1 card, then DESTROY 1 card (b.o.t.)
             // 22 = SACRIFICE 1 card, then  DRAW 1 card (b.o.t.)
             // 23 = DESTROY an upgrade card or SACRIFICE a downgrade card
             // (e.y.s.)
             // 24 = DESTROY an upgrade card or SACRIFICE a downgrade card
             // (no trigger)
             // 25 = DESTROY 1 unicorn card, then immediately end your turn
             // (b.o.t.)
             // 26 = DESTROY 1 unicorn card (no trigger)
             // 27 = STEAL 1 upgrade card (e.y.s.)
             // 28 = STEAL 1 unicorn card, then return it at the end of your
             // turn (b.o.t.)
             // 29 = move a unicorn card from your stable to any other
             // player's then STEAL a unicorn card from that player's stable
             // 30 = move any player's unicorn cards to the discard pile
             // without triggering any of their effects, then bring the same
             // number of baby unicorns from the nursery directly into that
             // player's stable
             // 31 = move an upgrade or downgrade card from any player's
             // stable to any other player's stable
             // 32 = if this card is sacrificed or destroyed, you may DESTROY
             // a unicorn card
             // 33 = if this card is sacrificed or destroyed, you may DISCARD
             // a card instead
             // 35 = if a unicorn card in your stable would be destroyed, you
             // may SACRIFICE this card instead
             // 36 = if at any time you have more than 5 Unicorns in your
             // Stable, SACRIFICE 1 Unicorn card
             // 37 = each time a unicorn card enters or leaves your stable,
             // discard a card [this is evil with the unicorn shrinkray lol]
             // 38 = add a Unicorn card from the discard pile to your hand
             // (e.y.s.). if this card is sacrificed or destroyed, return it
             // to your hand
             // 39 = add a Magic card from the discard pile to your hand
             // (e.y.s.). if this card is sacrificed or destroyed, return it
             // to your hand
             // 40 = add a Neigh card from the discard pile to your hand
             // (e.y.s.). if this card is sacrificed or destroyed, return it
             // to your hand
             // 41 = return a card in another player's stable to their hand.
             // that player must DISCARD a card
             // 42 = return a card in another player's stable to their hand
             // (e.y.s.)
             // 43 = pull a card at random from another player's hand into
             // your hand (e.y.s.)
             // 44 = look at another player's hand. choose a card and add it
             // to your hand
             // 45 = trade hands with any other player
             // 46 = your hand must be visible to all players
             // 47 = search the deck for an upgrade card and add it to your
             // hand. shuffle the deck (e.y.s.)
             // 48 = search the deck for a downgrade card and add it to your
             // hand. shuffle the deck (e.y.s.)
             // 49 = search the deck for a card with "Narwhal" in its name and
             // add it to your hand. shuffle the deck (e.y.s.)
             // 50 = basic unicorn cards cannot enter any player's stable
             // except yours
             // 51 = this card cannot be destroyed by magic cards
             // 52 = your unicorn cards cannot be destroyed
             // 53 = you cannot play any upgrade cards
             // 54 = you cannot play any neigh cards
             // 55 = this card counts for 2 unicorns. you cannot play any
             // neigh cards
             // 56 = cards you play cannot be neigh'd
             // 57 = play 2 cards during your action phase (b.o.t.)
             // 58 = this card can only enter a stable if there is a basic
             // unicorn card there. second half of rainbow mane (bring in
             // basic unicorn card...)
             // 59 = bring a basic unicorn card from your hand directly into
             // your stable (e.y.s.)
             // 60 = all of your unicorn cards are considered basic unicorns
             // with no effects
             // 61 = all of your unicorns are considered pandas. cards that
             // affect unicorn cards do not affect your pandas
             // 62 = puppicorn nonsense
             // 63 = glitter tornado [forgot...]
             // 66 = neigh
             // 67 = super neigh
};

struct CardsInHand {
  int num_cards;
  int cards[HAND_SIZE];  // "lookup table" for cards; uses deck_ref and
                         // deck_index to get randomized location
};

struct Stable {
  int unicorns[STABLE_SIZE];
  int num_unicorns;
  int size;
};

struct Player {
  struct CardsInHand hand;
  struct Stable stable;
  char username[NAME_SIZE];
  short flags;  // 00000000 00000000 plain cheese
                // & 1 = cannot play upgrade cards
                // & 2 = cannot play neigh cards
                // & 4 = cards you play cannot be neigh'd
                // & 8 = your unicorn cards cannot be destroyed
                // & 16 = your hand must be visible to all players
                // & 32 = basic unicorns can't enter your stable
                // & 64 = ginormous unicorn check
                // & 128 = all your unicorns are considered basic and have no
                // effects
                // & 256 = all your unicorns are considered pandas;
                // cards that affect unicorns don't affect yours
                // & 512 = tiny unicorn; sacrifice cards after >5 unicorns
                // & 1024 = black knight unicorn; sacrifice that card instead
                // & 2048 = brbd wre; discrd a card upon entry/exit of stable
  short filler; // padding for prev variable
};

extern int current_players;
struct Player player[MAX_PLAYERS];
extern struct Unicorn deck[129];

SOCKET sockfd;
SOCKET udpfd;

HANDLE mutex;

// deck_index and nursery_index point to top-most card that is currently
// available; discard index is basically the size count
extern unsigned int deck_index;
extern unsigned int discard_index;
extern unsigned int nursery_index;
extern unsigned int dnurse_size;
extern unsigned int isclient; // 0 = server, 1 = client
extern unsigned int WIN_CONDITION;
extern unsigned int turn_count;
extern int uni_lasso_flag[3];  // 0 = crd ind of p1, 1 = plyr stealer, 2 = victim
extern int puppicorn_index;
extern unsigned char deck_flag;            // toggles whether deck is printed out or not
extern unsigned char discard_flag;
extern unsigned char nursery_flag;
extern unsigned int deck_ref[DECK_SIZE];
extern unsigned int discard_ref[DECK_SIZE];
extern unsigned int nursery_ref[NURSERY_SIZE];

// ******************** Pretty Color Functions ********************

// https://www.codeproject.com/Tips/5255355/How-to-Put-Color-on-Windows-Console
// add: reg add HKEY_CURRENT_USER\Console /v VirtualTerminalLevel /t REG_DWORD /d 0x00000001 /f
// undo: reg add HKEY_CURRENT_USER\Console /v VirtualTerminalLevel /t REG_DWORD /d 0x00000000 /f
void red(void);
void yellow(void);
void green(void);
void cyan(void);
void blue(void);
void magenta(void);
void reset_col(void);
void rainbow(char* str);
