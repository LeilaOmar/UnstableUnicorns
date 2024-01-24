#pragma once
#include "basedeck.h"

// Base game deck (ver. 1 before the reprint)
struct Unicorn deck[] = {
    {BABYUNICORN, UNICORN, "Baby Unicorn (Red)",
     "If this card would be sacrificed, destroyed, or returned to your hand, "
     "return it to the Nursery instead.",
     NOTHING},
    {BABYUNICORN, UNICORN, "Baby Unicorn (Pink)",
     "If this card would be sacrificed, destroyed, or returned to your hand, "
     "return it to the Nursery instead.",
     NOTHING},
    {BABYUNICORN, UNICORN, "Baby Unicorn (Orange)",
     "If this card would be sacrificed, destroyed, or returned to your hand, "
     "return it to the Nursery instead.",
     NOTHING},
    {BABYUNICORN, UNICORN, "Baby Unicorn (Yellow)",
     "If this card would be sacrificed, destroyed, or returned to your hand, "
     "return it to the Nursery instead.",
     NOTHING},
    {BABYUNICORN, UNICORN, "Baby Unicorn (Green)",
     "If this card would be sacrificed, destroyed, or returned to your hand, "
     "return it to the Nursery instead.",
     NOTHING},
    {BABYUNICORN, UNICORN, "Baby Unicorn (Blue)",
     "If this card would be sacrificed, destroyed, or returned to your hand, "
     "return it to the Nursery instead.",
     NOTHING},
    {BABYUNICORN, UNICORN, "Baby Unicorn (Purple)",
     "If this card would be sacrificed, destroyed, or returned to your hand, "
     "return it to the Nursery instead.",
     NOTHING},
    {BABYUNICORN, UNICORN, "Baby Unicorn (Black)",
     "If this card would be sacrificed, destroyed, or returned to your hand, "
     "return it to the Nursery instead.",
     NOTHING},
    {BABYUNICORN, UNICORN, "Baby Unicorn (White)",
     "If this card would be sacrificed, destroyed, or returned to your hand, "
     "return it to the Nursery instead.",
     NOTHING},
    {BABYUNICORN, UNICORN, "Baby Unicorn (Brown)",
     "If this card would be sacrificed, destroyed, or returned to your hand, "
     "return it to the Nursery instead.",
     NOTHING},
    {BABYUNICORN, UNICORN, "Baby Unicorn (Rainbow)",
     "If this card would be sacrificed, destroyed, or returned to your hand, "
     "return it to the Nursery instead.",
     NOTHING},
    {BABYUNICORN, UNICORN, "Baby Unicorn (Death)",
     "If this card would be sacrificed, destroyed, or returned to your hand, "
     "return it to the Nursery instead.",
     NOTHING},
    {BABYUNICORN, NARWHAL, "Baby Narwhal",
     "If this card would be sacrificed, destroyed, or returned to your hand, "
     "return it to the Nursery instead.",
     NOTHING},
    {BASICUNICORN, UNICORN, "Basic Unicorn (Red)", "Beards are like, so hot.",
     NOTHING},
    {BASICUNICORN, UNICORN, "Basic Unicorn (Red)", "Beards are like, so hot.",
     NOTHING},
    {BASICUNICORN, UNICORN, "Basic Unicorn (Red)", "Beards are like, so hot.",
     NOTHING},
    {BASICUNICORN, UNICORN, "Basic Unicorn (Orange)",
     "Pumpkin spice is the pumpkin spice of life.", NOTHING},
    {BASICUNICORN, UNICORN, "Basic Unicorn (Orange)",
     "Pumpkin spice is the pumpkin spice of life.", NOTHING},
    {BASICUNICORN, UNICORN, "Basic Unicorn (Orange)",
     "Pumpkin spice is the pumpkin spice of life.", NOTHING},
    {BASICUNICORN, UNICORN, "Basic Unicorn (Yellow)",
     "Dance like nobody's watching.", NOTHING},
    {BASICUNICORN, UNICORN, "Basic Unicorn (Yellow)",
     "Dance like nobody's watching.", NOTHING},
    {BASICUNICORN, UNICORN, "Basic Unicorn (Yellow)",
     "Dance like nobody's watching.", NOTHING},
    {BASICUNICORN, UNICORN, "Basic Unicorn (Green)",
     "Vinyl records and mixtapes only.", NOTHING},
    {BASICUNICORN, UNICORN, "Basic Unicorn (Green)",
     "Vinyl records and mixtapes only.", NOTHING},
    {BASICUNICORN, UNICORN, "Basic Unicorn (Green)",
     "Vinyl records and mixtapes only.", NOTHING},
    {BASICUNICORN, UNICORN, "Basic Unicorn (Blue)",
     "Popped collars are for date nights only.", NOTHING},
    {BASICUNICORN, UNICORN, "Basic Unicorn (Blue)",
     "Popped collars are for date nights only.", NOTHING},
    {BASICUNICORN, UNICORN, "Basic Unicorn (Blue)",
     "Popped collars are for date nights only.", NOTHING},
    {BASICUNICORN, UNICORN, "Basic Unicorn (Indigo)", "[emojis]", NOTHING},
    {BASICUNICORN, UNICORN, "Basic Unicorn (Indigo)", "[emojis]", NOTHING},
    {BASICUNICORN, UNICORN, "Basic Unicorn (Indigo)", "[emojis]", NOTHING},
    {BASICUNICORN, UNICORN, "Basic Unicorn (Purple)",
     "#nomakeup #nofilter #sunnies #shameless #selfie #basic #TGIF # "
     "unicornhairdontcare",
     NOTHING},
    {BASICUNICORN, UNICORN, "Basic Unicorn (Purple)",
     "#nomakeup #nofilter #sunnies #shameless #selfie #basic #TGIF # "
     "unicornhairdontcare",
     NOTHING},
    {BASICUNICORN, UNICORN, "Basic Unicorn (Purple)",
     "#nomakeup #nofilter #sunnies #shameless #selfie #basic #TGIF # "
     "unicornhairdontcare",
     NOTHING},
    {BASICUNICORN, NARWHAL, "Narwhal",
     "This card has no special powers, but it sure is cute!", NOTHING},
    {BASICUNICORN, NARWHAL, "Narwhal",
     "This card has no special powers, but it sure is cute!", NOTHING},
    {BASICUNICORN, NARWHAL, "Narwhal",
     "This card has no special powers, but it sure is cute!", NOTHING},
    {MAGICUNICORN, UNICORN, "Rhinocorn",
     "If this card is in your Stable at the beginning of your turn, you may "
     "DESTROY a Unicorn card. If you do, immediately end your turn.",
     rhinocorn},
    {MAGICUNICORN, UNICORN, "Extremely Fertile Unicorn",
     "If this card is in your Stable at the beginning of your turn, you may "
     "DISCARD a card. If you do, bring a Baby Unicorn card from the Nursery "
     "directly into your Stable.",
     extremely_fertile_unicorn},
    {MAGICUNICORN, UNICORN, "Magical Kittencorn",
     "This card cannot be destroyed by Magic cards.", magical_kittencorn},
    {MAGICUNICORN, UNICORN, "Stabby the Unicorn",
     "If this card is sacrificed or destroyed, you may DESTROY a Unicorn card.",
     stabby_the_unicorn},
    {MAGICUNICORN, UNICORN, "Puppicorn",
     "Each time any player begins their turn, move this card to that player's "
     "Stable. This card cannot be sacrificed or destroyed.",
     puppicorn},
    {MAGICUNICORN, UNICORN, "Rainbow Unicorn",
     "When this card enters your Stable, you may bring a Basic Unicorn card "
     "from your hand into your Stable.",
     rainbow_unicorn},
    {MAGICUNICORN, UNICORN, "Zombie Unicorn",
     "If this card is in your Stable at the beginning of your turn, you may "
     "DISCARD a Unicorn card. If you do, choose a Unicorn card from the "
     "discard pile and bring it directly into your Stable, then immediately "
     "skip to your End of Turn Phase",
     zombie_unicorn},
    {MAGICUNICORN, UNICORN, "Extremely Destructive Unicorn",
     "When this card enters your Stable, each player (including you) must "
     "SACRIFICE a Unicorn card.",
     extremely_destructive_unicorn},
    {MAGICUNICORN, UNICORN, "Chainsaw Unicorn",
     "When this card enters your Stable, you may DESTROY an Upgrade card or "
     "SACRIFICE a Downgrade card.",
     chainsaw_unicorn},
    {MAGICUNICORN, UNICORN, "Llamacorn",
     "When this card enters your Stable, each player (including you) must "
     "DISCARD a card.",
     llamacorn},
    {MAGICUNICORN, UNICORN, "Americorn",
     "When this card enters your Stable, you may pull a card at random from "
     "another player's hand.",
     americorn},
    {MAGICUNICORN, UNICORN, "Ginormous Unicorn",
     "This card counts for 2 Unicorns. You cannot play any Neigh cards.", ginormous_unicorn_effect},
    {MAGICUNICORN, UNICORN, "Seductive Unicorn",
     "When this card enters your Stable, you may DISCARD a card, then STEAL a "
     "Unicorn card.",
     seductive_unicorn},
    {MAGICUNICORN, UNICORN, "Angel Unicorn",
     "If this card is in your Stable at the beginning of your turn, you may "
     "SACRIFICE this card. If you do, choose a Unicorn card from the discard "
     "pile and bring it directly into your Stable.",
     angel_unicorn},
    {MAGICUNICORN, UNICORN, "Queen Bee Unicorn",
     "Basic Unicorn cards cannot enter any player's Stable except yours.", queen_bee_unicorn_effect},
    {MAGICUNICORN, UNICORN, "Greedy Flying Unicorn",
     "When this card enters your Stable, DRAW a card. If this card is "
     "sacrificed or destroyed, return it to your hand.",
     greedy_flying_unicorn},
    {MAGICUNICORN, UNICORN, "Annoying Flying Unicorn",
     "When this card enters your Stable, you may force another player to "
     "DISCARD a card. If this card is sacrificed or destroyed, return it to "
     "your hand.",
     annoying_flying_unicorn},
    {MAGICUNICORN, UNICORN, "Magical Flying Unicorn",
     "When this card enters your Stable, you may add a Magic card from the "
     "discard pile to your hand. If this card is sacrificed or destroyed, "
     "return it to your hand.",
     magical_flying_unicorn},
    {MAGICUNICORN, UNICORN, "Swift Flying Unicorn",
     "When this card enters your Stable, you may add a Neigh card from the "
     "discard pile to your hand. If this card is sacrificed or destroyed, "
     "return it to your hand.",
     swift_flying_unicorn},
    {MAGICUNICORN, UNICORN, "Majestic Flying Unicorn",
     "When this card enters your Stable, you may add a Unicorn card from the "
     "discard pile to your hand. If this card is sacrificed or destroyed, "
     "return it to your hand.",
     majestic_flying_unicorn},
    {MAGICUNICORN, UNICORN, "Unicorn Phoenix",
     "If this card would be sacrificed or destroyed, you may DISCARD a card "
     "instead.",
     unicorn_phoenix},
    {MAGICUNICORN, UNICORN, "Unicorn on the Cob",
     "When this card enters your Stable, DRAW 2 cards and DISCARD a card.", unicorn_on_the_cob},
    {MAGICUNICORN, UNICORN, "Black Knight Unicorn",
     "If a Unicorn card in your Stable would be destroyed, you may SACRIFICE "
     "this card instead.",
     black_knight_unicorn_effect},
    {MAGICUNICORN, UNICORN, "Shark With a Horn",
     "When this card enters your Stable, you may SACRIFICE this card, then "
     "DESTROY a Unicorn card.",
     shark_with_a_horn},
    {MAGICUNICORN, NARWHAL, "Shabby the Narwhal",
     "When this card enters your Stable, you may search the deck for a "
     "Downgrade card and add it to your hand, then shuffle the deck.",
     shabby_the_narwhal},
    {MAGICUNICORN, NARWHAL, "Narwhal Torpedo",
     "When this card enters your Stable, SACRIFICE all Downgrade cards in your "
     "Stable.",
     narwhal_torpedo},
    {MAGICUNICORN, NARWHAL, "Alluring Narwhal",
     "When this card enters your Stable, you may STEAL an Upgrade card.", alluring_narwhal},
    {MAGICUNICORN, UNICORN, "Mermaid Unicorn",
     "When this card enters your Stable, return a card in another player's "
     "Stable to their hand.",
     mermaid_unicorn},
    {MAGICUNICORN, NARWHAL, "Classy Narwhal",
     "When this card enters your Stable, you may search the deck for an "
     "Upgrade card and add it to your hand, then shuffle the deck.",
     classy_narwhal},
    {MAGICUNICORN, NARWHAL, "The Great Narwhal",
     "When this card enters your Stable, you may search the deck for a card "
     "with \"Narwhal\" in its name and add it to your hand, then shuffle the "
     "deck.",
     the_great_narwhal},
    {MAGIC, NOSPECIES, "Unicorn Poison", "DESTROY a Unicorn card.", unicorn_poison},
    {MAGIC, NOSPECIES, "Unicorn Poison", "DESTROY a Unicorn card.", unicorn_poison},
    {MAGIC, NOSPECIES, "Unicorn Poison", "DESTROY a Unicorn card.", unicorn_poison},
    {MAGIC, NOSPECIES, "Back Kick",
     "Return a card in another player's Stable to their hand. That player must "
     "DISCARD a card.",
     back_kick},
    {MAGIC, NOSPECIES, "Back Kick",
     "Return a card in another player's Stable to their hand. That player must "
     "DISCARD a card.",
     back_kick},
    {MAGIC, NOSPECIES, "Back Kick",
     "Return a card in another player's Stable to their hand. That player must "
     "DISCARD a card.",
     back_kick},
    {MAGIC, NOSPECIES, "Change of Luck",
     "DRAW 2 cards and DISCARD 3 cards, then take another turn.", change_of_luck},
    {MAGIC, NOSPECIES, "Change of Luck",
     "DRAW 2 cards and DISCARD 3 cards, then take another turn.", change_of_luck},
    {MAGIC, NOSPECIES, "Glitter Tornado",
     "Return a card in each player's Stable (including yours) to their hand.",
     glitter_tornado},
    {MAGIC, NOSPECIES, "Glitter Tornado",
     "Return a card in each player's Stable (including yours) to their hand.",
     glitter_tornado},
    {MAGIC, NOSPECIES, "Unicorn Swap",
     "Move a Unicorn card in your Stable to any other player's Stable, then "
     "STEAL a Unicorn card from that player's Stable.",
     unicorn_swap},
    {MAGIC, NOSPECIES, "Unicorn Swap",
     "Move a Unicorn card in your Stable to any other player's Stable, then "
     "STEAL a Unicorn card from that player's Stable.",
     unicorn_swap},
    {MAGIC, NOSPECIES, "Re-Target",
     "Move an Upgrade or Downgrade card from any player's Stable to any other "
     "player's Stable.",
     re_target},
    {MAGIC, NOSPECIES, "Re-Target",
     "Move an Upgrade or Downgrade card from any player's Stable to any other "
     "player's Stable.",
     re_target},
    {MAGIC, NOSPECIES, "Unfair Bargain", "Trade hands with any other player.",
     unfair_bargain},
    {MAGIC, NOSPECIES, "Unfair Bargain", "Trade hands with any other player.",
     unfair_bargain},
    {MAGIC, NOSPECIES, "Two-For-One", "SACRIFICE a card, then DESTROY 2 cards.",
     two_for_one},
    {MAGIC, NOSPECIES, "Two-For-One", "SACRIFICE a card, then DESTROY 2 cards.",
     two_for_one},
    {MAGIC, NOSPECIES, "Unicorn Shrinkray",
     "Choose any player. Move all of that player's Unicorn cards to the "
     "discard pile without triggering any of their effects, then bring the "
     "same number of Baby Unicorn cards from the Nursery directly into that "
     "player's Stable.",
     unicorn_shrinkray},
    {MAGIC, NOSPECIES, "Targeted Destruction",
     "DESTROY an Upgrade card or SACRIFICE a Downgrade card.", targeted_destruction},
    {MAGIC, NOSPECIES, "Mystical Vortex",
     "Each player (including you) must DISCARD a card. Shuffle the discard "
     "pile into the deck.",
     mystical_vortex},
    {MAGIC, NOSPECIES, "Good Deal", "DRAW 3 cards and DISCARD a card", good_deal},
    {MAGIC, NOSPECIES, "Shake Up",
     "Shuffle this card, your hand, and the discard pile into the deck. DRAW 5 "
     "cards.",
     shake_up},
    {MAGIC, NOSPECIES, "Blatant Thievery",
     "Look at another player's hand. Choose a card and add it to your hand.",
     blatant_thievery},
    {MAGIC, NOSPECIES, "Reset Button",
     "Each player (including you) must SACRIFICE all Upgrade and Downgrade "
     "cards in their Stable. Shuffle the discard pile into the deck.",
     reset_button},
    {UPGRADE, NOSPECIES, "Rainbow Mane",
     "This card can only enter a Stable if there is a Basic Unicorn card in "
     "that Stable. If this card is in your Stable at the beginning of your "
     "turn, you may bring a Basic Unicorn card from your hand directly into "
     "your Stable.",
     rainbow_mane},
    {UPGRADE, NOSPECIES, "Rainbow Mane",
     "This card can only enter a Stable if there is a Basic Unicorn card in "
     "that Stable. If this card is in your Stable at the beginning of your "
     "turn, you may bring a Basic Unicorn card from your hand directly into "
     "your Stable.",
     rainbow_mane},
    {UPGRADE, NOSPECIES, "Rainbow Mane",
     "This card can only enter a Stable if there is a Basic Unicorn card in "
     "that Stable. If this card is in your Stable at the beginning of your "
     "turn, you may bring a Basic Unicorn card from your hand directly into "
     "your Stable.",
     rainbow_mane},
    {UPGRADE, NOSPECIES, "Extra Tail",
     "This card can only enter a Stable if there is a Basic Unicorn card in "
     "that Stable. If this card is in your Stable at the beginning of your "
     "turn, you may DRAW an extra card.",
     extra_tail},
    {UPGRADE, NOSPECIES, "Extra Tail",
     "This card can only enter a Stable if there is a Basic Unicorn card in "
     "that Stable. If this card is in your Stable at the beginning of your "
     "turn, you may DRAW an extra card.",
     extra_tail},
    {UPGRADE, NOSPECIES, "Extra Tail",
     "This card can only enter a Stable if there is a Basic Unicorn card in "
     "that Stable. If this card is in your Stable at the beginning of your "
     "turn, you may DRAW an extra card.",
     extra_tail},
    {UPGRADE, NOSPECIES, "Glitter Bomb",
     "If this card is in your Stable at the beginning of your turn, you may "
     "SACRIFICE a card, then DESTROY a card.",
     glitter_bomb},
    {UPGRADE, NOSPECIES, "Glitter Bomb",
     "If this card is in your Stable at the beginning of your turn, you may "
     "SACRIFICE a card, then DESTROY a card.",
     glitter_bomb},
    {UPGRADE, NOSPECIES, "Yay", "Cards you play cannot be Neigh'd.", yay_effect},
    {UPGRADE, NOSPECIES, "Yay", "Cards you play cannot be Neigh'd.", yay_effect},
    {UPGRADE, NOSPECIES, "Unicorn Lasso",
     "If this card is in your Stable at the beginning of your turn, you may "
     "STEAL a Unicorn card. At the end of your turn, return that Unicorn card "
     "to the Stable from which you stole it.",
     unicorn_lasso},
    {UPGRADE, NOSPECIES, "Rainbow Aura",
     "Your Unicorn cards cannot be destroyed.", rainbow_aura_effect},
    {UPGRADE, NOSPECIES, "Double Dutch",
     "If this card is in your Stable at the beginning of your turn, you may "
     "play 2 cards during your Action phase.",
     double_dutch},
    {UPGRADE, NOSPECIES, "Summoning Ritual",
     "If this card is in your Stable at the beginning of your turn, you may "
     "DISCARD 2 Unicorn cards. If you do, bring a Unicorn card directly from "
     "the discard pile into your Stable.",
     summoning_ritual},
    {DOWNGRADE, NOSPECIES, "Barbed Wire",
     "Each time a Unicorn card enters or leaves your Stable, DISCARD a card.",
     barbed_wire_effect},
    {DOWNGRADE, NOSPECIES, "Pandamonium",
     "All of your Unicorns are considered Pandas. Cards that affect Unicorn "
     "cards do not affect your Pandas.",
     pandamonium_effect},
    {DOWNGRADE, NOSPECIES, "Sadistic Ritual",
     "If this card is in your Stable at the beginning of your turn, SACRIFICE "
     "a Unicorn card, then DRAW a card.",
     sadistic_ritual},
    {DOWNGRADE, NOSPECIES, "Slowdown", "You cannot play Neigh cards.", slowdown_effect},
    {DOWNGRADE, NOSPECIES, "Nanny Cam",
     "Your hand must be visible to all players.", nanny_cam_effect},
    {DOWNGRADE, NOSPECIES, "Broken Stable", "You cannot play Upgrade cards.",
     broken_stable_effect},
    {DOWNGRADE, NOSPECIES, "Blinding Light",
     "All of your Unicorn cards are considered Basic Unicorns with no effects.",
     blinding_light_effect},
    {DOWNGRADE, NOSPECIES, "Tiny Stable",
     "If at any time you have more than 5 Unicorns in your Stable, SACRIFICE a "
     "Unicorn card.",
     tiny_stable_effect},
    {INSTANT, NOSPECIES, "Neigh",
     "Play this card when another player tries to play a card. Stop their card "
     "from being played and send it to the discard pile.",
     neigh},
    {INSTANT, NOSPECIES, "Neigh",
     "Play this card when another player tries to play a card. Stop their card "
     "from being played and send it to the discard pile.",
     neigh},
    {INSTANT, NOSPECIES, "Neigh",
     "Play this card when another player tries to play a card. Stop their card "
     "from being played and send it to the discard pile.",
     neigh},
    {INSTANT, NOSPECIES, "Neigh",
     "Play this card when another player tries to play a card. Stop their card "
     "from being played and send it to the discard pile.",
     neigh},
    {INSTANT, NOSPECIES, "Neigh",
     "Play this card when another player tries to play a card. Stop their card "
     "from being played and send it to the discard pile.",
     neigh},
    {INSTANT, NOSPECIES, "Neigh",
     "Play this card when another player tries to play a card. Stop their card "
     "from being played and send it to the discard pile.",
     neigh},
    {INSTANT, NOSPECIES, "Neigh",
     "Play this card when another player tries to play a card. Stop their card "
     "from being played and send it to the discard pile.",
     neigh},
    {INSTANT, NOSPECIES, "Neigh",
     "Play this card when another player tries to play a card. Stop their card "
     "from being played and send it to the discard pile.",
     neigh},
    {INSTANT, NOSPECIES, "Neigh",
     "Play this card when another player tries to play a card. Stop their card "
     "from being played and send it to the discard pile.",
     neigh},
    {INSTANT, NOSPECIES, "Neigh",
     "Play this card when another player tries to play a card. Stop their card "
     "from being played and send it to the discard pile.",
     neigh},
    {INSTANT, NOSPECIES, "Neigh",
     "Play this card when another player tries to play a card. Stop their card "
     "from being played and send it to the discard pile.",
     neigh},
    {INSTANT, NOSPECIES, "Neigh",
     "Play this card when another player tries to play a card. Stop their card "
     "from being played and send it to the discard pile.",
     neigh},
    {INSTANT, NOSPECIES, "Neigh",
     "Play this card when another player tries to play a card. Stop their card "
     "from being played and send it to the discard pile.",
     neigh},
    {INSTANT, NOSPECIES, "Neigh",
     "Play this card when another player tries to play a card. Stop their card "
     "from being played and send it to the discard pile.",
     neigh},
    {INSTANT, NOSPECIES, "Super Neigh",
     "Play this card when another player tries to play a card. Stop their card "
     "from being played and send it to the discard pile. This card cannot be "
     "Neigh'd.",
     super_neigh}
};

// deck_index and nursery_index point to top-most card that is currently available;
// discard index is basically the size count
unsigned int deck_index = 0;
unsigned int discard_index = 0;
unsigned int nursery_index = 0;
unsigned int dnurse_size = NURSERY_SIZE;  // decreases by 1 after each player picks a baby unicorn

int uni_lasso_flag[3] = { 0 };  // 0 = crd ind of p1, 1 = plyr stealer, 2 = victim
int puppicorn_index = -1;

unsigned int deck_ref[] = {
    13,  14,  15,  16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,
    28,  29,  30,  31,  32,  33,  34,  35,  36,  37,  38,  39,  40,  41,  42,
    43,  44,  45,  46,  47,  48,  49,  50,  51,  52,  53,  54,  55,  56,  57,
    58,  59,  60,  61,  62,  63,  64,  65,  66,  67,  68,  69,  70,  71,  72,
    73,  74,  75,  76,  77,  78,  79,  80,  81,  82,  83,  84,  85,  86,  87,
    88,  89,  90,  91,  92,  93,  94,  95,  96,  97,  98,  99,  100, 101, 102,
    103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117,
    118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128 };
unsigned int discard_ref[DECK_SIZE] = { 0 };
unsigned int nursery_ref[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };

// ********************************************************************************
// ***************************** Card Effect Functions ****************************
// ********************************************************************************

// TODO: consider creating onReturnHand, onDestroy, etc. functions for the struct Unicorn
// cards themselves because there are a lot of edge cases... it's potentially an issue
// because the functions may require variable length parameters (e.g. magical kittencorn
// would need to know which card triggered its destruction)

// TODO: add functions for checking if there are valid unicorns to destroy/sacrifice/etc.
// since that popped up among several cards


// switch cases for cards that may trigger specific player flags (e.g. cannot
// neigh)
int toggleFlags(int pnum, int effect) {
  switch (effect) {
  case NOTHING:
    return 0;
  case black_knight_unicorn_effect:
    // Playing Black Knight Unicorn
    // if a unicorn card in your stable would be destroyed, you may
    // SACRIFICE this card instead
    player[pnum].flags ^= black_knight_unicorn;
    return 1;
  case tiny_stable_effect:
    // Playing Tiny Stable:
    // If at any time you have more than 5 Unicorns in your Stable,
    // SACRIFICE a Unicorn card
    player[pnum].flags ^= tiny_stable;
    return 1;
  case barbed_wire_effect:
    // Playing Barbed Wire:
    // Each time a Unicorn card enters or leaves your Stable, DISCARD a
    // card. 
    player[pnum].flags ^= barbed_wire;
    return 1;
  case nanny_cam_effect:
    // Playing Nanny Cam:
    // your hand must be visible to all players
    player[pnum].flags ^= nanny_cam;
    return 1;
  case queen_bee_unicorn_effect:
    // Playing Queen Bee Unicorn:
    // basic unicorn cards cannot enter any player's stable except yours
    for (int i = 0; i < current_players; i++) {
      if (i != pnum) player[i].flags ^= queen_bee_unicorn;
    }
    return 1;
  case rainbow_aura_effect:
    // Playing Rainbow Aura:
    // your unicorn cards cannot be destroyed
    player[pnum].flags ^= rainbow_aura;
    return 1;
  case broken_stable_effect:
    // Playing Broken Stable:
    // you cannot play any upgrade cards
    player[pnum].flags ^= broken_stable;
    return 1;
  case slowdown_effect:
    // Playing Slowdown:
    // you cannot play any neigh cards
    player[pnum].flags ^= slowdown;
    return 1;
  case ginormous_unicorn_effect:
    // Playing Ginormous Unicorn:
    // this card counts for 2 unicorns. you cannot play any neigh cards
    player[pnum].flags ^= ginormous_unicorn;
    return 1;
  case yay_effect:
    // Playing Yay:
    // cards you play cannot be neigh'd
    player[pnum].flags ^= yay;
    return 1;
  case blinding_light_effect:
    // Playing Blinding Light:
    // all of your unicorn cards are considered basic unicorns with
    // no effects
    player[pnum].flags ^= blinding_light;
    return 1;
  case pandamonium_effect:
    // Playing Pandamonium
    // all of your unicorns are considered pandas. cards that
    // affect unicorn cards do not affect your pandas
    player[pnum].flags ^= pandamonium;
    return 1;
  default:
    return 0;
  }

  return 0;
}

// switch cases for when a card has a special effect upon being sacrificed or
// destroyed
void sacrificeDestroyEffects(int pnum, int cindex, int effect) {
  int isvalid = 0;
  char ans, buf[LINE_MAX];

  // Blinding Light makes it so that all unicorn cards have no effects;
  // the other special cards still need to toggle their flags
  if ((player[pnum].flags & blinding_light) == blinding_light ||
      toggleFlags(pnum, effect))
    effect = NOTHING;

  switch (effect) {
  case NOTHING:
    break;
  case greedy_flying_unicorn:
  case annoying_flying_unicorn:
  case majestic_flying_unicorn:
  case magical_flying_unicorn:
  case swift_flying_unicorn:
    // Playing XYZ Flying Unicorn
    // if this card is sacrificed or destroyed, return it to your hand
    player[pnum].hand.cards[player[pnum].hand.num_cards++] =
      player[pnum].stable.unicorns[cindex];
    rearrangeStable(pnum, cindex);
    return;  // leave the function early
  case stabby_the_unicorn:
  {
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
  }
  case unicorn_phoenix:
  {
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
    if (ans == 'y') {
      discard(pnum, 1, ANYUNICORN);
      return;
    }
    break;
  }
  default:
    break;
  }

  if (deck[player[pnum].stable.unicorns[cindex]].class == BABYUNICORN)
    addNursery(player[pnum].stable.unicorns[cindex]);
  else
    discard_ref[discard_index++] = player[pnum].stable.unicorns[cindex];

  rearrangeStable(pnum, cindex);
}

// switch cases for cards that may be unable to be played due to certain circumstances;
// this is separated so that the hand is not rearranged unnecessarily;
// 0 = returned prematurely, conditions not met (FALSE)
// 1 = success (TRUE)
// 2 = effect is not conditional/relevant; skip
int conditionalEffects(int pnum, int effect, int cardid, int hindex, int upgrade_target) {
  int index, index2, index3, isvalid = 0, isvalid2 = 0, tmp_hand[HAND_SIZE];
  char ans, * end, buf[LINE_MAX];

  switch (effect) {
  case NOTHING:
    break;
  case extra_tail:
  {
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
  }
  case two_for_one:
  {
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
  }
  case targeted_destruction:
  {
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
  }
  case unicorn_poison:
  {
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
  }
  case unicorn_swap:
  {
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
  }
  case unicorn_shrinkray:
  {
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
  }
  case re_target:
  {
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
  }
  case back_kick:
  {
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
  }
  case blatant_thievery:
  {
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
  }
  case unfair_bargain:
  {
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
  }
  case rainbow_mane:
  {
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
  }
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
  case NOTHING:
    break;
  case greedy_flying_unicorn:
  {
    // Playing Greedy Flying Unicorn:
    // DRAW 1 card
    draw(pnum, 1);
    break;
  }
  case unicorn_on_the_cob:
  {
    // Playing Unicorn on the Cob:
    // DRAW 2 cards and DISCARD 1 card
    draw(pnum, 2);
    discard(pnum, 1, ANY);
    break;
  }
  case annoying_flying_unicorn:
  {
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
  }
  case seductive_unicorn:
  {
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
  }
  case llamacorn:
  {
    // Playing Llamacorn:
    // each player must DISCARD 1 card.
    for (int i = 0; i < current_players; i++) {
      discard(i, 1, ANY);
    }
    break;
  }
  case extremely_destructive_unicorn:
  {
    // Playing Extremely Destructive Unicorn:
    // each player must SACRIFICE 1 unicorn card

    for (int i = 0; i < current_players; i++) {
      sacrifice(i, ANYUNICORN);
    }
    break;
  }
  case narwhal_torpedo:
  {
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
  }
  case shark_with_a_horn:
  {
    // Playing Shark With a Horn
    // you may SACRIFICE this card, then destroy a unicorn card

    // check if there are unicorn cards to destroy; rainbow aura protects
    // unicorn cards and pandemonium makes it so that your "pandas" don't
    // classify as unicorn cards; inner loop for puppicorn *blegh*
    // 
    // TODO: i don't remember the circumstances behind why i wrote this bug... :/ figure it out
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
  }
  case chainsaw_unicorn:
  {
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
  }
  case alluring_narwhal:
  {
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
  }
  case black_knight_unicorn_effect:
  {
    // Playing Black Knight Unicorn
    // if a unicorn card in your stable would be destroyed, you may
    // SACRIFICE this card instead
    player[pnum].flags |= 1024;
    break;
  }
  case majestic_flying_unicorn:
  {
    // Playing Majestic Flying Unicorn:
    // you may add a Unicorn card from the discard pile to your hand
    searchDiscard(pnum, ANYUNICORN);
    break;
  }
  case magical_flying_unicorn:
  {
    // Playing Magical Flying Unicorn:
    // you may add a Magic card from the discard pile to your hand
    searchDiscard(pnum, MAGIC);
    break;
  }
  case swift_flying_unicorn:
  {
    // Playing Swift Flying Unicorn:
    // you may add a Neigh card from the discard pile to your hand
    searchDiscard(pnum, INSTANT);
    break;
  }
  case mermaid_unicorn:
  {
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
  }
  case americorn:
  {
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
  }
  case classy_narwhal:
  {
    // Playing Classy Narwhal
    // search the deck for an upgrade card and add it to your hand. shuffle
    // the deck
    searchDeck(pnum, UPGRADE, ANY);
    break;
  }
  case shabby_the_narwhal:
  {
    // Playing Shabby the Narwhal
    // search the deck for a downgrade card and add it to your hand. shuffle
    // the deck
    searchDeck(pnum, DOWNGRADE, ANY);
    break;
  }
  case the_great_narwhal:
  {
    // Playing The Great Narwhal
    // search the deck for a card with "Narwhal" in its name and add it to
    // your hand. shuffle the deck
    searchDeck(pnum, ANY, NARWHAL);
    break;
  }
  case queen_bee_unicorn_effect:
  {
    // Playing Queen Bee Unicorn:
    // basic unicorn cards cannot enter any player's stable except yours
    for (int i = 0; i < current_players; i++) {
      if (i != pnum) player[i].flags ^= 32;
    }
    break;
  }
  case ginormous_unicorn_effect:
  {
    // Playing Ginormous Unicorn:
    // this card counts for 2 unicorns. you cannot play any neigh cards
    player[pnum].flags ^= 64;
    break;
  }
  case rainbow_unicorn:
  {
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
  }
  default:
    break;
  }
}

// switch cases for Magic cards
void magicEffects(int pnum, int effect) {
  int index;
  char* end, buf[LINE_MAX];

  switch (effect) {
  case NOTHING:
    break;
  case change_of_luck:
  {
    // Playing Change of Luck:
    // DRAW 2 cards and DISCARD 3 cards; take another turn
    draw(pnum, 2);
    discard(pnum, 3, ANY);
    turn_count++;
    break;
  }
  case good_deal:
  {
    // Playing Good Deal:
    // DRAW 3 cards and DISCARD 1 card
    draw(pnum, 3);
    discard(pnum, 1, ANY);
    break;
  }
  case shake_up:
  {
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
  }
  case mystical_vortex:
  {
    // Playing Mystical Vortex:
    // every player must DISCARD 1 card. shuffle the discard pile into the
    // deck
    for (int i = 0; i < current_players; i++) {
      discard(i, 1, ANY);
    }
    shuffleDiscard();
    break;
  }
  case reset_button:
  {
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
  }
  case glitter_tornado:
  {
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
  }
  default:
    break;
  }
}

// switch cases for beginning of your turn effects
void beginningTurnEffects(int pnum, int effect) {
  int index, index2 = 0, isvalid = 0, isvalid2 = 0;
  char ans, * end, buf[LINE_MAX];

  // TODO: add the card index as a parameter for the blinding_light check
  // and for cards that get sacrificed

  // Blinding Light makes it so that all unicorn cards have no effects;
  // the listed effects are upgrade/downgrade cards
  // OPTIMIZE: put this in a different function so there's no need to check
  // all those different effects?
  if ((player[pnum].flags & blinding_light) != 0 &&
    effect != 13 && effect != 15 && effect != 21 &&
    effect != 28 && effect != 57 && effect != 58)
    return;

  switch (effect) {
  case NOTHING:
    break;
  case extra_tail:
  {
    // Playing Extra Tail:
    // you may draw 1 card
    draw(pnum, 1);
    break;
  }
  case extremely_fertile_unicorn:
  {
    // Playing Extremely Fertile Unicorn:
    // you may discard 1 card to bring a baby unicorn directly to your
    // stable

    // check if there are actually cards to discard and receive, otherwise break;
    if (player[pnum].hand.num_cards == 0) break;
    if (nursery_index >= NURSERY_SIZE) break;

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
  }
  case zombie_unicorn:
  {
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
  }
  case summoning_ritual:
  {
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
  }
  case angel_unicorn:
  {
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
  }
  case sadistic_ritual:
  {
    // Playing Sadistic Ritual:
    // you must sacrifice 1 unicorn card, then draw a card

    // TODO: can't draw cards if there's nothing to sacrifice, make sacrifice return an int to confirm
    sacrifice(pnum, ANYUNICORN);
    draw(pnum, 1);
    break;
  }
  case glitter_bomb:
  {
    // Playing Glitter Bomb:
    // you may sacrifice a card, then destroy a card


    // TODO: can't destroy cards if there's nothing to sacrifice, make sacrifice return an int to confirm

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
  }
  case rhinocorn:
  {
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
  }
  case unicorn_lasso:
  {
    // Playing Unicorn Lasso:
    // you may steal a unicorn card, and then return the unicorn card
    // at the end of your turn

    // TODO: technically speaking, if the stealing player has pandamonium,
    // then they do not have to return the unicorn because it's now a "panda"
    // instead
    //
    // but then you could also argue that the second conditional is impossible,
    // so you can't play unicorn lasso. most people seem to think stealing
    // under a masquerade effect is fair game though

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
  }
  case double_dutch:
  {
    // Playing Double Dutch:
    // you may play 2 cards during your Action phase

    turn_count++;
    break;
  }
  case rainbow_mane:
  {
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
  }
  default:
    break;
  }
}
