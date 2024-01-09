#include "globals.h"

int current_players = 1; // the host is a player too! :)
struct Unicorn deck[] = {
    {BABYUNICORN, UNICORN, "Baby Unicorn (Red)",
     "If this card would be sacrificed, destroyed, or returned to your hand, "
     "return it to the Nursery instead.",
     0},
    {BABYUNICORN, UNICORN, "Baby Unicorn (Pink)",
     "If this card would be sacrificed, destroyed, or returned to your hand, "
     "return it to the Nursery instead.",
     0},
    {BABYUNICORN, UNICORN, "Baby Unicorn (Orange)",
     "If this card would be sacrificed, destroyed, or returned to your hand, "
     "return it to the Nursery instead.",
     0},
    {BABYUNICORN, UNICORN, "Baby Unicorn (Yellow)",
     "If this card would be sacrificed, destroyed, or returned to your hand, "
     "return it to the Nursery instead.",
     0},
    {BABYUNICORN, UNICORN, "Baby Unicorn (Green)",
     "If this card would be sacrificed, destroyed, or returned to your hand, "
     "return it to the Nursery instead.",
     0},
    {BABYUNICORN, UNICORN, "Baby Unicorn (Blue)",
     "If this card would be sacrificed, destroyed, or returned to your hand, "
     "return it to the Nursery instead.",
     0},
    {BABYUNICORN, UNICORN, "Baby Unicorn (Purple)",
     "If this card would be sacrificed, destroyed, or returned to your hand, "
     "return it to the Nursery instead.",
     0},
    {BABYUNICORN, UNICORN, "Baby Unicorn (Black)",
     "If this card would be sacrificed, destroyed, or returned to your hand, "
     "return it to the Nursery instead.",
     0},
    {BABYUNICORN, UNICORN, "Baby Unicorn (White)",
     "If this card would be sacrificed, destroyed, or returned to your hand, "
     "return it to the Nursery instead.",
     0},
    {BABYUNICORN, UNICORN, "Baby Unicorn (Brown)",
     "If this card would be sacrificed, destroyed, or returned to your hand, "
     "return it to the Nursery instead.",
     0},
    {BABYUNICORN, UNICORN, "Baby Unicorn (Rainbow)",
     "If this card would be sacrificed, destroyed, or returned to your hand, "
     "return it to the Nursery instead.",
     0},
    {BABYUNICORN, UNICORN, "Baby Unicorn (Death)",
     "If this card would be sacrificed, destroyed, or returned to your hand, "
     "return it to the Nursery instead.",
     0},
    {BABYUNICORN, NARWHAL, "Baby Narwhal",
     "If this card would be sacrificed, destroyed, or returned to your hand, "
     "return it to the Nursery instead.",
     0},
    {BASICUNICORN, UNICORN, "Basic Unicorn (Red)", "Beards are like, so hot.",
     0},
    {BASICUNICORN, UNICORN, "Basic Unicorn (Red)", "Beards are like, so hot.",
     0},
    {BASICUNICORN, UNICORN, "Basic Unicorn (Red)", "Beards are like, so hot.",
     0},
    {BASICUNICORN, UNICORN, "Basic Unicorn (Orange)",
     "Pumpkin spice is the pumpkin spice of life.", 0},
    {BASICUNICORN, UNICORN, "Basic Unicorn (Orange)",
     "Pumpkin spice is the pumpkin spice of life.", 0},
    {BASICUNICORN, UNICORN, "Basic Unicorn (Orange)",
     "Pumpkin spice is the pumpkin spice of life.", 0},
    {BASICUNICORN, UNICORN, "Basic Unicorn (Yellow)",
     "Dance like nobody's watching.", 0},
    {BASICUNICORN, UNICORN, "Basic Unicorn (Yellow)",
     "Dance like nobody's watching.", 0},
    {BASICUNICORN, UNICORN, "Basic Unicorn (Yellow)",
     "Dance like nobody's watching.", 0},
    {BASICUNICORN, UNICORN, "Basic Unicorn (Green)",
     "Vinyl records and mixtapes only.", 0},
    {BASICUNICORN, UNICORN, "Basic Unicorn (Green)",
     "Vinyl records and mixtapes only.", 0},
    {BASICUNICORN, UNICORN, "Basic Unicorn (Green)",
     "Vinyl records and mixtapes only.", 0},
    {BASICUNICORN, UNICORN, "Basic Unicorn (Blue)",
     "Popped collars are for date nights only.", 0},
    {BASICUNICORN, UNICORN, "Basic Unicorn (Blue)",
     "Popped collars are for date nights only.", 0},
    {BASICUNICORN, UNICORN, "Basic Unicorn (Blue)",
     "Popped collars are for date nights only.", 0},
    {BASICUNICORN, UNICORN, "Basic Unicorn (Indigo)", "[emojis]", 0},
    {BASICUNICORN, UNICORN, "Basic Unicorn (Indigo)", "[emojis]", 0},
    {BASICUNICORN, UNICORN, "Basic Unicorn (Indigo)", "[emojis]", 0},
    {BASICUNICORN, UNICORN, "Basic Unicorn (Purple)",
     "#nomakeup #nofilter #sunnies #shameless #selfie #basic #TGIF # "
     "unicornhairdontcare",
     0},
    {BASICUNICORN, UNICORN, "Basic Unicorn (Purple)",
     "#nomakeup #nofilter #sunnies #shameless #selfie #basic #TGIF # "
     "unicornhairdontcare",
     0},
    {BASICUNICORN, UNICORN, "Basic Unicorn (Purple)",
     "#nomakeup #nofilter #sunnies #shameless #selfie #basic #TGIF # "
     "unicornhairdontcare",
     0},
    {BASICUNICORN, NARWHAL, "Narwhal",
     "This card has no special powers, but it sure is cute!", 0},
    {BASICUNICORN, NARWHAL, "Narwhal",
     "This card has no special powers, but it sure is cute!", 0},
    {BASICUNICORN, NARWHAL, "Narwhal",
     "This card has no special powers, but it sure is cute!", 0},
    {MAGICUNICORN, UNICORN, "Rhinocorn",
     "If this card is in your Stable at the beginning of your turn, you may "
     "DESTROY a Unicorn card. If you do, immediately end your turn.",
     25},
    {MAGICUNICORN, UNICORN, "Extremely Fertile Unicorn",
     "If this card is in your Stable at the beginning of your turn, you may "
     "DISCARD a card. If you do, bring a Baby Unicorn card from the Nursery "
     "directly into your Stable.",
     8},
    {MAGICUNICORN, UNICORN, "Magical Kittencorn",
     "This card cannot be destroyed by Magic cards.", 51},
    {MAGICUNICORN, UNICORN, "Stabby the Unicorn",
     "If this card is sacrificed or destroyed, you may DESTROY a Unicorn card.",
     32},
    {MAGICUNICORN, UNICORN, "Puppicorn",
     "Each time any player begins their turn, move this card to that player's "
     "Stable. This card cannot be sacrificed or destroyed.",
     62},
    {MAGICUNICORN, UNICORN, "Rainbow Unicorn",
     "When this card enters your Stable, you may bring a Basic Unicorn card "
     "from your hand into your Stable.",
     59},
    {MAGICUNICORN, UNICORN, "Zombie Unicorn",
     "If this card is in your Stable at the beginning of your turn, you may "
     "DISCARD a Unicorn card. If you do, choose a Unicorn card from the "
     "discard pile and bring it directly into your Stable, then immediately "
     "skip to your End of Turn Phase",
     11},
    {MAGICUNICORN, UNICORN, "Extremely Destructive Unicorn",
     "When this card enters your Stable, each player (including you) must "
     "SACRIFICE a Unicorn card.",
     16},
    {MAGICUNICORN, UNICORN, "Chainsaw Unicorn",
     "When this card enters your Stable, you may DESTROY an Upgrade card or "
     "SACRIFICE a Downgrade card.",
     23},
    {MAGICUNICORN, UNICORN, "Llamacorn",
     "When this card enters your Stable, each player (including you) must "
     "DISCARD a card.",
     12},
    {MAGICUNICORN, UNICORN, "Americorn",
     "When this card enters your Stable, you may pull a card at random from "
     "another player's hand.",
     43},
    {MAGICUNICORN, UNICORN, "Ginormous Unicorn",
     "This card counts for 2 Unicorns. You cannot play any Neigh cards.", 55},
    {MAGICUNICORN, UNICORN, "Seductive Unicorn",
     "When this card enters your Stable, you may DISCARD a card, then STEAL a "
     "Unicorn card.",
     10},
    {MAGICUNICORN, UNICORN, "Angel Unicorn",
     "If this card is in your Stable at the beginning of your turn, you may "
     "SACRIFICE this card. If you do, choose a Unicorn card from the discard "
     "pile and bring it directly into your Stable.",
     14},
    {MAGICUNICORN, UNICORN, "Queen Bee Unicorn",
     "Basic Unicorn cards cannot enter any player's Stable except yours.", 50},
    {MAGICUNICORN, UNICORN, "Greedy Flying Unicorn",
     "When this card enters your Stable, DRAW a card. If this card is "
     "sacrificed or destroyed, return it to your hand.",
     1},
    {MAGICUNICORN, UNICORN, "Annoying Flying Unicorn",
     "When this card enters your Stable, you may force another player to "
     "DISCARD a card. If this card is sacrificed or destroyed, return it to "
     "your hand.",
     7},
    {MAGICUNICORN, UNICORN, "Magical Flying Unicorn",
     "When this card enters your Stable, you may add a Magic card from the "
     "discard pile to your hand. If this card is sacrificed or destroyed, "
     "return it to your hand.",
     39},
    {MAGICUNICORN, UNICORN, "Swift Flying Unicorn",
     "When this card enters your Stable, you may add a Neigh card from the "
     "discard pile to your hand. If this card is sacrificed or destroyed, "
     "return it to your hand.",
     40},
    {MAGICUNICORN, UNICORN, "Majestic Flying Unicorn",
     "When this card enters your Stable, you may add a Unicorn card from the "
     "discard pile to your hand. If this card is sacrificed or destroyed, "
     "return it to your hand.",
     38},
    {MAGICUNICORN, UNICORN, "Unicorn Phoenix",
     "If this card would be sacrificed or destroyed, you may DISCARD a card "
     "instead.",
     33},
    {MAGICUNICORN, UNICORN, "Unicorn on the Cob",
     "When this card enters your Stable, DRAW 2 cards and DISCARD a card.", 2},
    {MAGICUNICORN, UNICORN, "Black Knight Unicorn",
     "If a Unicorn card in your Stable would be destroyed, you may SACRIFICE "
     "this card instead.",
     35},
    {MAGICUNICORN, UNICORN, "Shark With a Horn",
     "When this card enters your Stable, you may SACRIFICE this card, then "
     "DESTROY a Unicorn card.",
     18},
    {MAGICUNICORN, NARWHAL, "Shabby the Narwhal",
     "When this card enters your Stable, you may search the deck for a "
     "Downgrade card and add it to your hand, then shuffle the deck.",
     48},
    {MAGICUNICORN, NARWHAL, "Narwhal Torpedo",
     "When this card enters your Stable, SACRIFICE all Downgrade cards in your "
     "Stable.",
     17},
    {MAGICUNICORN, NARWHAL, "Alluring Narwhal",
     "When this card enters your Stable, you may STEAL an Upgrade card.", 27},
    {MAGICUNICORN, UNICORN, "Mermaid Unicorn",
     "When this card enters your Stable, return a card in another player's "
     "Stable to their hand.",
     42},
    {MAGICUNICORN, NARWHAL, "Classy Narwhal",
     "When this card enters your Stable, you may search the deck for an "
     "Upgrade card and add it to your hand, then shuffle the deck.",
     47},
    {MAGICUNICORN, NARWHAL, "The Great Narwhal",
     "When this card enters your Stable, you may search the deck for a card "
     "with \"Narwhal\" in its name and add it to your hand, then shuffle the "
     "deck.",
     49},
    {MAGIC, NOSPECIES, "Unicorn Poison", "DESTROY a Unicorn card.", 26},
    {MAGIC, NOSPECIES, "Unicorn Poison", "DESTROY a Unicorn card.", 26},
    {MAGIC, NOSPECIES, "Unicorn Poison", "DESTROY a Unicorn card.", 26},
    {MAGIC, NOSPECIES, "Back Kick",
     "Return a card in another player's Stable to their hand. That player must "
     "DISCARD a card.",
     41},
    {MAGIC, NOSPECIES, "Back Kick",
     "Return a card in another player's Stable to their hand. That player must "
     "DISCARD a card.",
     41},
    {MAGIC, NOSPECIES, "Back Kick",
     "Return a card in another player's Stable to their hand. That player must "
     "DISCARD a card.",
     41},
    {MAGIC, NOSPECIES, "Change of Luck",
     "DRAW 2 cards and DISCARD 3 cards, then take another turn.", 3},
    {MAGIC, NOSPECIES, "Change of Luck",
     "DRAW 2 cards and DISCARD 3 cards, then take another turn.", 3},
    {MAGIC, NOSPECIES, "Glitter Tornado",
     "Return a card in each player's Stable (including yours) to their hand.",
     63},
    {MAGIC, NOSPECIES, "Glitter Tornado",
     "Return a card in each player's Stable (including yours) to their hand.",
     63},
    {MAGIC, NOSPECIES, "Unicorn Swap",
     "Move a Unicorn card in your Stable to any other player's Stable, then "
     "STEAL a Unicorn card from that player's Stable.",
     29},
    {MAGIC, NOSPECIES, "Unicorn Swap",
     "Move a Unicorn card in your Stable to any other player's Stable, then "
     "STEAL a Unicorn card from that player's Stable.",
     29},
    {MAGIC, NOSPECIES, "Re-Target",
     "Move an Upgrade or Downgrade card from any player's Stable to any other "
     "player's Stable.",
     31},
    {MAGIC, NOSPECIES, "Re-Target",
     "Move an Upgrade or Downgrade card from any player's Stable to any other "
     "player's Stable.",
     31},
    {MAGIC, NOSPECIES, "Unfair Bargain", "Trade hands with any other player.",
     45},
    {MAGIC, NOSPECIES, "Unfair Bargain", "Trade hands with any other player.",
     45},
    {MAGIC, NOSPECIES, "Two-For-One", "SACRIFICE a card, then DESTROY 2 cards.",
     20},
    {MAGIC, NOSPECIES, "Two-For-One", "SACRIFICE a card, then DESTROY 2 cards.",
     20},
    {MAGIC, NOSPECIES, "Unicorn Shrinkray",
     "Choose any player. Move all of that player's Unicorn cards to the "
     "discard pile without triggering any of their effects, then bring the "
     "same number of Baby Unicorn cards from the Nursery directly into that "
     "player's Stable.",
     30},
    {MAGIC, NOSPECIES, "Targeted Destruction",
     "DESTROY an Upgrade card or SACRIFICE a Downgrade card.", 24},
    {MAGIC, NOSPECIES, "Mystical Vortex",
     "Each player (including you) must DISCARD a card. Shuffle the discard "
     "pile into the deck.",
     9},
    {MAGIC, NOSPECIES, "Good Deal", "DRAW 3 cards and DISCARD a card", 4},
    {MAGIC, NOSPECIES, "Shake Up",
     "Shuffle this card, your hand, and the discard pile into the deck. DRAW 5 "
     "cards.",
     5},
    {MAGIC, NOSPECIES, "Blatant Thievery",
     "Look at another player's hand. Choose a card and add it to your hand.",
     44},
    {MAGIC, NOSPECIES, "Reset Button",
     "Each player (including you) must SACRIFICE all Upgrade and Downgrade "
     "cards in their Stable. Shuffle the discard pile into the deck.",
     19},
    {UPGRADE, NOSPECIES, "Rainbow Mane",
     "This card can only enter a Stable if there is a Basic Unicorn card in "
     "that Stable. If this card is in your Stable at the beginning of your "
     "turn, you may bring a Basic Unicorn card from your hand directly into "
     "your Stable.",
     58},
    {UPGRADE, NOSPECIES, "Rainbow Mane",
     "This card can only enter a Stable if there is a Basic Unicorn card in "
     "that Stable. If this card is in your Stable at the beginning of your "
     "turn, you may bring a Basic Unicorn card from your hand directly into "
     "your Stable.",
     58},
    {UPGRADE, NOSPECIES, "Rainbow Mane",
     "This card can only enter a Stable if there is a Basic Unicorn card in "
     "that Stable. If this card is in your Stable at the beginning of your "
     "turn, you may bring a Basic Unicorn card from your hand directly into "
     "your Stable.",
     58},
    {UPGRADE, NOSPECIES, "Extra Tail",
     "This card can only enter a Stable if there is a Basic Unicorn card in "
     "that Stable. If this card is in your Stable at the beginning of your "
     "turn, you may DRAW an extra card.",
     6},
    {UPGRADE, NOSPECIES, "Extra Tail",
     "This card can only enter a Stable if there is a Basic Unicorn card in "
     "that Stable. If this card is in your Stable at the beginning of your "
     "turn, you may DRAW an extra card.",
     6},
    {UPGRADE, NOSPECIES, "Extra Tail",
     "This card can only enter a Stable if there is a Basic Unicorn card in "
     "that Stable. If this card is in your Stable at the beginning of your "
     "turn, you may DRAW an extra card.",
     6},
    {UPGRADE, NOSPECIES, "Glitter Bomb",
     "If this card is in your Stable at the beginning of your turn, you may "
     "SACRIFICE a card, then DESTROY a card.",
     21},
    {UPGRADE, NOSPECIES, "Glitter Bomb",
     "If this card is in your Stable at the beginning of your turn, you may "
     "SACRIFICE a card, then DESTROY a card.",
     21},
    {UPGRADE, NOSPECIES, "Yay", "Cards you play cannot be Neigh'd.", 56},
    {UPGRADE, NOSPECIES, "Yay", "Cards you play cannot be Neigh'd.", 56},
    {UPGRADE, NOSPECIES, "Unicorn Lasso",
     "If this card is in your Stable at the beginning of your turn, you may "
     "STEAL a Unicorn card. At the end of your turn, return that Unicorn card "
     "to the Stable from which you stole it.",
     28},
    {UPGRADE, NOSPECIES, "Rainbow Aura",
     "Your Unicorn cards cannot be destroyed.", 52},
    {UPGRADE, NOSPECIES, "Double Dutch",
     "If this card is in your Stable at the beginning of your turn, you may "
     "play 2 cards during your Action phase.",
     57},
    {UPGRADE, NOSPECIES, "Summoning Ritual",
     "If this card is in your Stable at the beginning of your turn, you may "
     "DISCARD 2 Unicorn cards. If you do, bring a Unicorn card directly from "
     "the discard pile into your Stable.",
     13},
    {DOWNGRADE, NOSPECIES, "Barbed Wire",
     "Each time a Unicorn card enters or leaves your Stable, DISCARD a card.",
     37},
    {DOWNGRADE, NOSPECIES, "Pandamonium",
     "All of your Unicorns are considered Pandas. Cards that affect Unicorn "
     "cards do not affect your Pandas.",
     61},
    {DOWNGRADE, NOSPECIES, "Sadistic Ritual",
     "If this card is in your Stable at the beginning of your turn, SACRIFICE "
     "a Unicorn card, then DRAW a card.",
     15},
    {DOWNGRADE, NOSPECIES, "Slowdown", "You cannot play Neigh cards.", 54},
    {DOWNGRADE, NOSPECIES, "Nanny Cam",
     "Your hand must be visible to all players.", 46},
    {DOWNGRADE, NOSPECIES, "Broken Stable", "You cannot play Upgrade cards.",
     53},
    {DOWNGRADE, NOSPECIES, "Blinding Light",
     "All of your Unicorn cards are considered Basic Unicorns with no effects.",
     60},
    {DOWNGRADE, NOSPECIES, "Tiny Stable",
     "If at any time you have more than 5 Unicorns in your Stable, SACRIFICE a "
     "Unicorn card.",
     36},
    {INSTANT, NOSPECIES, "Neigh",
     "Play this card when another player tries to play a card. Stop their card "
     "from being played and send it to the discard pile.",
     66},
    {INSTANT, NOSPECIES, "Neigh",
     "Play this card when another player tries to play a card. Stop their card "
     "from being played and send it to the discard pile.",
     66},
    {INSTANT, NOSPECIES, "Neigh",
     "Play this card when another player tries to play a card. Stop their card "
     "from being played and send it to the discard pile.",
     66},
    {INSTANT, NOSPECIES, "Neigh",
     "Play this card when another player tries to play a card. Stop their card "
     "from being played and send it to the discard pile.",
     66},
    {INSTANT, NOSPECIES, "Neigh",
     "Play this card when another player tries to play a card. Stop their card "
     "from being played and send it to the discard pile.",
     66},
    {INSTANT, NOSPECIES, "Neigh",
     "Play this card when another player tries to play a card. Stop their card "
     "from being played and send it to the discard pile.",
     66},
    {INSTANT, NOSPECIES, "Neigh",
     "Play this card when another player tries to play a card. Stop their card "
     "from being played and send it to the discard pile.",
     66},
    {INSTANT, NOSPECIES, "Neigh",
     "Play this card when another player tries to play a card. Stop their card "
     "from being played and send it to the discard pile.",
     66},
    {INSTANT, NOSPECIES, "Neigh",
     "Play this card when another player tries to play a card. Stop their card "
     "from being played and send it to the discard pile.",
     66},
    {INSTANT, NOSPECIES, "Neigh",
     "Play this card when another player tries to play a card. Stop their card "
     "from being played and send it to the discard pile.",
     66},
    {INSTANT, NOSPECIES, "Neigh",
     "Play this card when another player tries to play a card. Stop their card "
     "from being played and send it to the discard pile.",
     66},
    {INSTANT, NOSPECIES, "Neigh",
     "Play this card when another player tries to play a card. Stop their card "
     "from being played and send it to the discard pile.",
     66},
    {INSTANT, NOSPECIES, "Neigh",
     "Play this card when another player tries to play a card. Stop their card "
     "from being played and send it to the discard pile.",
     66},
    {INSTANT, NOSPECIES, "Neigh",
     "Play this card when another player tries to play a card. Stop their card "
     "from being played and send it to the discard pile.",
     66},
    {INSTANT, NOSPECIES, "Super Neigh",
     "Play this card when another player tries to play a card. Stop their card "
     "from being played and send it to the discard pile. This card cannot be "
     "Neigh'd.",
     67} };

// deck_index and nursery_index point to top-most card that is currently
// available; discard index is basically the size count
unsigned int deck_index = 0;
unsigned int discard_index = 0;
unsigned int nursery_index = 0;
unsigned int dnurse_size = NURSERY_SIZE;  // decreases by 1 after each player picks a baby unicorn
unsigned int isclient; // 0 = server, 1 = client
unsigned int WIN_CONDITION = 7;
unsigned int turn_count = 0;
int uni_lasso_flag[3] = { 0 };  // 0 = crd ind of p1, 1 = plyr stealer, 2 = victim
int puppicorn_index = -1;
unsigned char deck_flag = 0;    // toggles whether deck is printed out or not
unsigned char discard_flag = 0;
unsigned char nursery_flag = 0;
int deck_ref[] = {
    13,  14,  15,  16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,
    28,  29,  30,  31,  32,  33,  34,  35,  36,  37,  38,  39,  40,  41,  42,
    43,  44,  45,  46,  47,  48,  49,  50,  51,  52,  53,  54,  55,  56,  57,
    58,  59,  60,  61,  62,  63,  64,  65,  66,  67,  68,  69,  70,  71,  72,
    73,  74,  75,  76,  77,  78,  79,  80,  81,  82,  83,  84,  85,  86,  87,
    88,  89,  90,  91,  92,  93,  94,  95,  96,  97,  98,  99,  100, 101, 102,
    103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117,
    118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128 };
int discard_ref[DECK_SIZE] = { 0 };
int nursery_ref[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };

// ******************** Pretty Color Functions ********************

// https://www.codeproject.com/Tips/5255355/How-to-Put-Color-on-Windows-Console
// add: reg add HKEY_CURRENT_USER\Console /v VirtualTerminalLevel /t REG_DWORD /d 0x00000001 /f
// undo: reg add HKEY_CURRENT_USER\Console /v VirtualTerminalLevel /t REG_DWORD /d 0x00000000 /f
void red(void) { printf("\033[1;31m"); }
void yellow(void) { printf("\033[1;33m"); }
void green(void) { printf("\033[1;32m"); }
void cyan(void) { printf("\033[1;36m"); }
void blue(void) { printf("\033[1;34m"); }
void magenta(void) { printf("\033[1;35m"); }
void reset_col(void) { printf("\033[0m"); }

void rainbow(char* str) {
  unsigned int color_index = 0;
  for (size_t i = 0; i < strlen(str); i++) {
    if (color_index % 6 == 0)
      red();
    else if (color_index % 6 == 1)
      yellow();
    else if (color_index % 6 == 2)
      green();
    else if (color_index % 6 == 3)
      cyan();
    else if (color_index % 6 == 4)
      blue();
    else
      magenta();

    printf("%c", str[i]);

    if (str[i] != '\n' && str[i] != ' ') color_index++;
  }
  reset_col();
}
