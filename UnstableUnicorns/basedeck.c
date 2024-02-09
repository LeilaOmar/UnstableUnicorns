#pragma once
#include "basedeck.h"
#include "gamemechanics.h"
#include "gamephase.h"
#include "networkevents.h"

// Base game deck (ver. 1 before the reprint)
struct Unicorn basedeck[] = {
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
     "This card counts for 2 Unicorns. You cannot play any Instant cards.", ginormous_unicorn_effect},
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
     "When this card enters your Stable, you may choose a Magic card from the "
     "discard pile and add it to your hand. If this card is sacrificed or destroyed, "
     "return it to your hand.",
     magical_flying_unicorn},
    {MAGICUNICORN, UNICORN, "Swift Flying Unicorn",
     "When this card enters your Stable, you may choose an Instant card from the "
     "discard pile and add it to your hand. If this card is sacrificed or destroyed, "
     "return it to your hand.",
     swift_flying_unicorn},
    {MAGICUNICORN, UNICORN, "Majestic Flying Unicorn",
     "When this card enters your Stable, you may choose a Unicorn card from the "
     "discard pile and add it to your hand. If this card is sacrificed or destroyed, "
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
    {DOWNGRADE, NOSPECIES, "Slowdown", "You cannot play Instant cards.", slowdown_effect},
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

int uni_lasso_flag[3] = { 0 };  // 0 = crd ind of p1, 1 = plyr stealer, 2 = victim
int puppicorn_index[2] = { -1, -1 };

// ********************************************************************************
// ***************************** Card Effect Functions ****************************
// ********************************************************************************

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
    // you cannot play any instant cards
    player[pnum].flags ^= slowdown;
    return 1;
  case ginormous_unicorn_effect:
    // Playing Ginormous Unicorn:
    // this card counts for 2 unicorns. you cannot play any instant cards
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
  // pandamonium (and other potential future masquerade cards?) bypasses that
  if ((player[pnum].flags & blinding_light) == blinding_light &&
      (player[pnum].flags & pandamonium) == 0)
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

    // check if there are unicorn cards to destroy
    isvalid = checkNumCardsToDestroy(pnum, ANYUNICORN, FALSE);
    if (!isvalid)
      break;

    do {
      printf("Would you like to destroy a Unicorn card (y/n)?: ");
      ans = charinput(buf, sizeof buf);
    } while ((ans != 'y' && ans != 'n') || strlen(buf) != 2);
    if (ans == 'y')
      destroy(pnum, ANYUNICORN, FALSE);
    break;
  }
  case unicorn_phoenix:
  {
    // Playing Unicorn Phoenix
    // if this card would be sacrificed or destroyed, you may DISCARD a
    // unicorn card instead

    // check if there are unicorn cards to discard
    for (int i = 0; i < player[pnum].hand.num_cards; i++) {
      if (player[pnum].hand.cards[i].species != NOSPECIES) {
        isvalid = 1;
        break;
      }
    }
    if (!isvalid)
      break;

    do {
      printf(
        "Would you like to discard a Unicorn card instead of disposing "
        "the card 'Unicorn Phoenix' (y/n)?: ");
      ans = charinput(buf, sizeof buf);
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

  if (player[pnum].stable.unicorns[cindex].class == BABYUNICORN)
    addNursery(player[pnum].stable.unicorns[cindex]);
  else
    addDiscard(player[pnum].stable.unicorns[cindex]);

  rearrangeStable(pnum, cindex);
}

// switch cases for cards that may be unable to be played due to certain circumstances;
// this is separated so that the hand is not rearranged unnecessarily;
// 0 = returned prematurely, conditions not met (FALSE)
// 1 = success (TRUE)
// 2 = effect is not conditional/relevant; skip
int conditionalEffects(int pnum, struct Unicorn corn, int hindex, int upgrade_target) {
  int index, index2, index3, isvalid = 0, isvalid2 = 0;
  char ans, * end, buf[LINE_MAX];
  struct Unicorn tmp_hand[HAND_SIZE];
  int tmp_size;

  switch (corn.effect) {
  case NOTHING:
    break;
  case extra_tail:
  {
    // Playing Extra Tail:
    // can only enter a stable if there is a Basic Unicorn card there
    for (int i = 0; i < player[upgrade_target].stable.size; i++) {
      if (player[upgrade_target].stable.unicorns[i].class == BASICUNICORN) {
        addStable(upgrade_target, corn);
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

    // check if there is at least 1 card to sacrifice; some cards *cough puppicorn cough*
    // are impervious to getting sacrificed
    for (int i = 0; i < player[pnum].stable.size; i++) {
      if (canBeSacrificed(pnum, i, ANY)) {
        isvalid = 1;
        break;
      }
    }

    // check if there are at least 2 cards to destroy
    isvalid2 = checkNumCardsToDestroy(pnum, ANY, TRUE);

    // quit and reassign the card to the player's hand
    if (!isvalid || isvalid2 < 2) {
      printf(
        "You are unable to play 'Two-For-One' because there are "
        "not enough cards to Sacrifice and/or Destroy\n");
      turn_count++;
      return 0;
    }

    // dispose of two-for-one
    addDiscard(corn);
    rearrangeHand(pnum, hindex);

    // card effect
    sacrifice(pnum, ANY);
    destroy(pnum, ANY, TRUE);
    destroy(pnum, ANY, TRUE);
    return 1;
  }
  case targeted_destruction:
  {
    // Playing Targeted Destruction:
    // you must DESTROY an upgrade card or SACRIFICE a downgrade card

    for (int j = 0; j < current_players; j++) {
      for (int i = 0; i < player[j].stable.size; i++) {
        if (j != pnum && player[j].stable.unicorns[i].class == UPGRADE)
          isvalid = 1;
        if (j == pnum &&
          player[j].stable.unicorns[i].class == DOWNGRADE)
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

    addDiscard(corn);
    rearrangeHand(pnum, hindex);

    do {
      printf(
        "Pick between the following options:"
        "\n  1. Destroy an upgrade card"
        "\n  2. Sacrifice a downgrade card"
        "\nChoice: ");
      ans = charinput(buf, sizeof buf);
    } while ((ans != '1' && ans != '2') || (ans == '1' && isvalid != 1) ||
      (ans == '2' && isvalid2 != 1) || strlen(buf) != 2);

    if (ans == '1') {
      // destroy an upgrade card (this doesn't need to be in the special function)
      destroy(pnum, UPGRADE, TRUE);
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
    isvalid = checkNumCardsToDestroy(pnum, ANYUNICORN, TRUE);

    // quit and reassign the card to the player's hand
    if (!isvalid) {
      printf(
        "You are unable to play 'Unicorn Poison' because there are "
        "no available cards to Destroy\n");
      turn_count++;
      return 0;
    }

    // dispose of card
    addDiscard(corn);
    rearrangeHand(pnum, hindex);

    // card effect
    destroy(pnum, ANYUNICORN, TRUE);
    return 1;
  }
  case change_of_luck:
  {
    // Playing Change of Luck:
    // DRAW 2 cards and DISCARD 3 cards; take another turn

    // need to discard 3 cards
    // 3 cards - 2 drawn + 1 (change of luck) = 2 cards in hand needed, including this card
    if (player[pnum].hand.num_cards < 2) {
      printf(
        "You are unable to play 'Change of Luck' because you would not "
        "have 3 cards in total to Discard\n");
      turn_count++;
      return 0;
    }

    // dispose of card
    addDiscard(corn);
    rearrangeHand(pnum, hindex);

    // card effect
    draw(pnum, 2);
    discard(pnum, 3, ANY);

    // the whole turn is reset, so the player gets to do their
    // end of turn phase and restart a new beginning of turn phase
    endOfTurn(pnum);
    beginningOfTurn(pnum);
    turn_count++; // turn_count++ is for restarting the action phase too
    return 1;
  }
  case glitter_tornado:
  {
    // Playing Glitter Tornado
    // Return a card in each player's Stable (including yours) to their
    // hand.

    // check if every player has a card to return
    for (int i = 0; i < current_players; i++) {
      if (player[i].stable.size <= 0) {
        isvalid = 1;
        break;
      }
    }

    if (isvalid) {
      printf(
        "You are unable to play 'Glitter Tornado' because all players do "
        "not have at least 1 card in their stables\n");
      turn_count++;
      return 0;
    }

    // dispose of card
    addDiscard(corn);
    rearrangeHand(pnum, hindex);

    // card effect
    // 
    // important to note that the current player (pnum) is the one to choose
    // every card that gets returned
    for (int i = 0; i < current_players; i++) {
      printStable(i);
      do {
        printf("Choose a valid card number to return: ");
        index = numinput(buf, &end, sizeof buf) - 1;
      } while (index < 0 || index >= player[i].stable.size || end != (buf + strlen(buf)));

      returnCardToHand(i, index);
    }
    return 1;
  }
  case unicorn_swap:
  {
    // Playing Unicorn Swap:
    // move a unicorn card from your stable to any other player's then STEAL
    // a unicorn card from that player's stable

    // check if there are unicorn cards to steal
    // 
    // TODO: can you use this even when their stable is empty just to trigger the
    // entering stable effect? maybe this check isn't necessary; the following code
    // doesn't even prevent the player from stealing their card back anyways
    for (int i = 0; i < current_players; i++) {
      if (i == pnum && player[i].stable.num_unicorns > 0) {
        isvalid = 1;
      }
      if (i != pnum && player[i].stable.num_unicorns > 0) {
        isvalid2 = 1;
      }
      if (isvalid && isvalid2) break;
    }

    if (!isvalid || !isvalid2) {
      printf(
        "You are unable to play 'Unicorn Swap' because there aren't enough "
        "Unicorn cards to swap\n");
      turn_count++;
      return 0;
    }

    // dispose of card
    addDiscard(corn);
    rearrangeHand(pnum, hindex);

    // card effect
    printPlayers();
    for (;;) {
      printf("Choose a player to swap unicorn cards with: ");
      index = numinput(buf, &end, sizeof buf) - 1;

      // index validation
      if (index < 0 || index >= current_players || index == pnum || end != (buf + strlen(buf)))
        continue;

      // check if chosen player actually has a unicorn card
      if (player[index].stable.num_unicorns > 0)
        break;
    }

    printStable(pnum);
    for (;;) {
      printf("Choose a valid unicorn card number to move to %s's stable: ",
        player[index].username);
      index2 = numinput(buf, &end, sizeof buf) - 1;

      // index validation
      if (index2 < 0 || index2 >= player[pnum].stable.size || end != (buf + strlen(buf)))
        continue;
      
      // make sure it's actually a unicorn card
      if (player[pnum].stable.unicorns[index2].species != NOSPECIES)
        break;
    }

    // rearrange the player's stable and toggle any flags before swapping the card to the other stable;
    // this will (hopefully) prevent dupes from card effect shenanigans
    struct Unicorn tmp = player[pnum].stable.unicorns[index2];
    rearrangeStable(pnum, index2);

    addStable(index, tmp);

    // steal a unicorn from the chosen player's stable
    printStable(index);
    for (;;) {
      printf("Choose the card number to steal: ");
      index2 = numinput(buf, &end, sizeof buf) - 1;

      // index validation
      if (index2 < 0 || index2 >= player[index].stable.size || end != (buf + strlen(buf)))
        continue;

      // make sure it's actually a unicorn card
      if (player[index].stable.unicorns[index2].species != NOSPECIES)
        break;
    }

    // swap the unicorn from the chosen player's stable to the current player (pnum)
    tmp = player[index].stable.unicorns[index2];
    rearrangeStable(index, index2);

    addStable(pnum, tmp);
    return 1;
  }
  case mystical_vortex:
  {
    // Playing Mystical Vortex:
    // every player must DISCARD 1 card. shuffle the discard pile into the
    // deck

    // check if the current player has any cards to dispose of
    if (player[pnum].hand.num_cards < 2) {
      printf(
        "You are unable to play 'Mystical Vortex' because you don't have any "
        "cards to Discard\n");
      turn_count++;
      return 0;
    }

    // dispose of card
    addDiscard(corn);
    rearrangeHand(pnum, hindex);

    // card effect
    for (int i = 0; i < current_players; i++) {
      discard(i, 1, ANY);
    }
    shuffleDiscard();
    return 1;
  }
  case unicorn_shrinkray:
  {
    // Playing Unicorn Shrinkray:
    // move all player's unicorn cards to the discard pile without
    // triggering any of their effects, then bring the same number of baby
    // unicorns from the nursery directly into that player's stable

    for (int i = 0; i < current_players; i++) {
      if (i == pnum) continue;
      if ((player[i].flags & pandamonium) != 0) {
        // pandas aren't unicorns
        isvalid2++;
        continue;
      }

      for (int j = 0; j < player[i].stable.size; j++) {
        if (player[i].stable.unicorns[j].class == BASICUNICORN ||
            player[i].stable.unicorns[j].class == MAGICUNICORN) {
          isvalid++;
        }
      }
      // adds to a counter check if nursery size is less than the amount of non-baby unicorns
      if (nursery.size < isvalid) {
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

    // dispose of card
    addDiscard(corn);
    rearrangeHand(pnum, hindex);

    // card effect
    printPlayers();
    for (;;) {
      printf("Choose a player to swap their stable with Baby Unicorns: ");
      index = numinput(buf, &end, sizeof buf) - 1;

      // index validation
      if (index < 0 || index >= current_players || index == pnum || end != (buf + strlen(buf)))
        continue;

      for (int i = 0; i < player[index].stable.size; i++) {
        if (player[index].stable.unicorns[i].class == BASICUNICORN ||
            player[index].stable.unicorns[i].class == MAGICUNICORN) {
          isvalid++;
        }
      }
      if (isvalid > 0 && isvalid <= nursery.size)
        break;

      isvalid = 0;
    }

    for (int i = 0; i < player[index].stable.size; i++) {
      if ((player[index].stable.unicorns[i].class == BASICUNICORN ||
            player[index].stable.unicorns[i].class == MAGICUNICORN)) {
        // get rid of the puppicorn index if applicable
        if (strcmp(player[index].stable.unicorns[i].name, "Puppicorn") == 0) {
          puppicorn_index[0] = -1;
        }

        // some unicorns may have flag effects that need to be removed
        toggleFlags(index, player[index].stable.unicorns[i].effect);
        addDiscard(player[index].stable.unicorns[i]);

        // no point in using addStable here when the cards are just getting replaced;
        // that means less function calls to discard() too
        player[index].stable.unicorns[i] = nursery.cards[nursery.size - 1];
        nursery.size--;
      }
    }

    // barbed wire check; that's a lot of unicorns to discard!
    // should this stay? this seems evil LOL
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
        if (player[j].stable.unicorns[i].class == UPGRADE ||
            player[j].stable.unicorns[i].class == DOWNGRADE) {
          // quit if either condition was met
          isvalid = 1;
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

    // dispose of card
    addDiscard(corn);
    rearrangeHand(pnum, hindex);

    // card effect
    isvalid = 0;
    printPlayers();
    do {
      printf("Choose a player to take an upgrade or downgrade card from: ");
      index = numinput(buf, &end, sizeof buf) - 1;

      // index validation
      if (index < 0 || index >= current_players || end != (buf + strlen(buf)))
        continue;

      // make sure chosen player has an upgrade or downgrade card
      for (int i = 0; i < player[index].stable.size; i++) {
        if (player[index].stable.unicorns[i].class == UPGRADE ||
            player[index].stable.unicorns[i].class == DOWNGRADE) {
          // quit if either condition was met
          isvalid = 1;
          break;
        }
      }
    } while (!isvalid);

    printStable(index);
    for (;;) {
      printf("Choose a valid card number to take: ");
      index2 = numinput(buf, &end, sizeof buf) - 1;

      // index validation
      if (index2 < 0 || index2 >= player[index].stable.size || end != (buf + strlen(buf)))
        continue;

      // check if the class aligns
      if (player[index].stable.unicorns[index2].class == UPGRADE ||
          player[index].stable.unicorns[index2].class == DOWNGRADE)
        break;
    }

    printPlayers();
    do {
      printf("Choose a player to give the taken upgrade or downgrade card: ");
      index3 = numinput(buf, &end, sizeof buf) - 1;
      // can't be the same as the player it was taken from (i.e. player # index)
    } while (index3 < 0 || index3 >= current_players || index3 == index || end != (buf + strlen(buf)));

    // add the card to the new stable, alongside any potential toggle switch
    addStable(index3, player[index].stable.unicorns[index2]);
    toggleFlags(index3, player[index3].stable.unicorns[player[index3].stable.size - 1].effect);

    rearrangeStable(index, index2);
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

    // dispose of card
    addDiscard(corn);
    rearrangeHand(pnum, hindex);

    // card effect
    printPlayers();
    for (;;) {
      printf(
        "Choose a player to return a card from their stable to their "
        "hand: ");
      index = numinput(buf, &end, sizeof buf) - 1;

      // index validation
      if (index < 0 || index >= current_players || index == pnum || end != (buf + strlen(buf)))
        continue;

      // passed the stable size check
      if (player[index].stable.size > 0)
        break;
    }

    printStable(index);
    do {
      printf("Choose a valid card number to return: ");
      index2 = numinput(buf, &end, sizeof buf) - 1;
    } while (index2 < 0 || index2 >= player[index].stable.size || end != (buf + strlen(buf)));

    returnCardToHand(index, index2);

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

    // dispose of card
    addDiscard(corn);
    rearrangeHand(pnum, hindex);

    // card effect
    printPlayers();
    for (;;) {
      printf("Choose a player to steal a random card from their hand: ");
      index = numinput(buf, &end, sizeof buf) - 1;

      // index validation
      if (index < 0 || index >= current_players || index == pnum || end != (buf + strlen(buf)))
        continue;

      if (player[index].hand.num_cards > 0)
        break;
    }

    printHand(index);
    do {
      printf("Choose a card to steal: ");
      index2 = numinput(buf, &end, sizeof buf) - 1;
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

    // dispose of card
    addDiscard(corn);
    rearrangeHand(pnum, hindex);

    // card effect
    printPlayers();
    for (;;) {
      printf("Choose a player to trade hands with: ");
      index = numinput(buf, &end, sizeof buf) - 1;

      // index validation
      if (index < 0 || index >= current_players || index == pnum || end != (buf + strlen(buf)))
        continue;

      if (player[index].hand.num_cards > 0)
        break;
    }

    for (int i = 0;
      i < (player[index].hand.num_cards *
        (player[index].hand.num_cards > player[pnum].hand.num_cards)) +
      (player[pnum].hand.num_cards *
        (player[pnum].hand.num_cards > player[index].hand.num_cards));
      i++) {
      // swap each element until i hits the max number between the highest
      // number of cards in each player's hand
      tmp_hand[i] = player[index].hand.cards[i];
      player[index].hand.cards[i] = player[pnum].hand.cards[i];
      player[pnum].hand.cards[i] = tmp_hand[i];
    }
    // swap num_cards info too
    tmp_size = player[index].hand.num_cards;
    player[index].hand.num_cards = player[pnum].hand.num_cards;
    player[pnum].hand.num_cards = tmp_size;
    return 1;
  }
  case reset_button:
  {
    // Playing Reset Button:
    // each player must SACRIFICE all upgrade/downgrade cards. shuffle
    // discard pile into the deck
    int isdowngrade = 0;
    int isupgrade = 0;

    // checks if the current player has any upgrade/downgrade cards to sacrifice,
    // then checks if other people have cards to sacrifice

    for (int j = 0; j < current_players; j++) {
      // current player (pnum)
      if (j == pnum) {
        for (int i = 0; i < player[j].stable.size; i++) {
          if (player[j].stable.unicorns[i].class == DOWNGRADE) {
            isdowngrade = 1;
          }
          else if (player[j].stable.unicorns[i].class == UPGRADE) {
            isupgrade = 1;
          }
        }
      }
      // everyone else
      else {
        for (int i = 0; i < player[j].stable.size; i++) {
          if (player[j].stable.unicorns[i].class == DOWNGRADE ||
              player[j].stable.unicorns[i].class == UPGRADE) {
            isvalid = 1;
          }
        }
      }
    }

    // need both an upgrade and downgrade card to play it, and then another player
    // also has to have at least one of the two
    if (!isupgrade || !isdowngrade || !isvalid) {
      printf(
        "You are unable to play 'Reset Button' because there are not enough Upgrade "
        "or Downgrade cards in your own stable and/or other people's stables to sacrifice.\n");
      turn_count++;
      return 0;
    }

    // dispose of card
    addDiscard(corn);
    rearrangeHand(pnum, hindex);

    // card effect
    for (int j = 0; j < current_players; j++) {
      // start from the end so that cards/indices don't get skipped upon
      // rearranging the stable
      for (int i = player[j].stable.size - 1; i >= 0; i--) {
        if (player[j].stable.unicorns[i].class == DOWNGRADE ||
            player[j].stable.unicorns[i].class == UPGRADE) {
          addDiscard(player[j].stable.unicorns[i]);
          rearrangeStable(j, i);
        }
      }
    }
    shuffleDiscard();
    return 1;
  }
  case rainbow_mane:
  {
    // Playing Rainbow Mane:
    // can only enter a stable if there is a Basic Unicorn card there
    for (int i = 0; i < player[upgrade_target].stable.size; i++) {
      if (player[upgrade_target].stable.unicorns[i].class == BASICUNICORN) {
        addStable(upgrade_target, corn);
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
  // 
  // TODO: should probably just move these three to the toggle flags function, and
  // then always put toggle flags after enter stable effects/add stable
  if ((player[pnum].flags & blinding_light) != 0 &&
      effect != queen_bee_unicorn_effect &&
      effect != ginormous_unicorn_effect &&
      effect != black_knight_unicorn_effect)
    return;

  switch (effect) {
  case NOTHING:
    break;
  case queen_bee_unicorn_effect:
  {
    // Playing Queen Bee Unicorn:
    // basic unicorn cards cannot enter any player's stable except yours
    for (int i = 0; i < current_players; i++) {
      if (i != pnum) player[i].flags |= 32;
    }
    break;
  }
  case ginormous_unicorn_effect:
  {
    // Playing Ginormous Unicorn:
    // this card counts for 2 unicorns. you cannot play any instant cards
    player[pnum].flags |= 64;
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
        isvalid = 1;
        break;
      }
    }

    if (!isvalid)
      break;

    printPlayers();
    for (;;) {
      printf("Choose a player other than yourself to discard a card: ");
      index = numinput(buf, &end, sizeof buf) - 1;

      // index validation
      if (index < 0 || index >= current_players || index == pnum || end != (buf + strlen(buf)))
        continue;

      if (player[index].hand.num_cards > 0)
        break;
    }

    discard(index, 1, ANY);
    break;
  }
  case seductive_unicorn:
  {
    // Playing Seductive Unicorn:
    // you may DISCARD 1 card, then STEAL a unicorn card

    // check if there are unicorn cards to steal
    for (int i = 0; i < current_players; i++) {
      if (i != pnum && player[i].stable.num_unicorns > 0) {
        isvalid = 1;
        break;
      }
    }

    if (!isvalid)
      break;

    do {
      printf(
        "Would you like to discard a card in order to steal a unicorn "
        "card (y/n)?: ");
      ans = charinput(buf, sizeof buf);
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

    // TODO: if there end up being too many isclient vs server checks, then maybe
    // it's worth using vtables and sending their respective lookup table references?
    if (isclient) {
      sendInt(sacrifice_event, sockfd);
      sendInt(ANY, sockfd);  // any being referred to all here, or the number "-1"
      sendInt(ANYUNICORN, sockfd);
      clientSacrifice(pnum, ANY, ANYUNICORN);
    }
    else {
      for (int i = 0; i < current_players - 1; i++) {
        sendInt(sacrifice_event, clientsockfd[i]);
        sendInt(ANY, clientsockfd[i]); // any being referred to all here, or the number "-1"
        sendInt(ANYUNICORN, clientsockfd[i]);
      }
      serverSacrifice(pnum, ANY, ANYUNICORN);
    }

    break;
  }
  case narwhal_torpedo:
  {
    // Playing Narwhal Torpedo
    // SACRIFICE all downgrade cards
    for (int i = player[pnum].stable.size - 1; i >= 0; i--) {
      // start from the end so that cards/indices don't get skipped upon
      // rearranging the stable
      if (player[pnum].stable.unicorns[i].class == DOWNGRADE) {
        addDiscard(player[pnum].stable.unicorns[i]);
        rearrangeStable(pnum, i);
      }
    }
    break;
  }
  case shark_with_a_horn:
  {
    // Playing Shark With a Horn
    // you may SACRIFICE this card, then destroy a unicorn card

    // check if there are unicorn cards to destroy
    isvalid = checkNumCardsToDestroy(pnum, ANYUNICORN, FALSE);
    if (!isvalid)
      break;

    do {
      printf(
        "Would you like to sacrifice this card in order to destroy a "
        "Unicorn card (y/n)?: ");
      ans = charinput(buf, sizeof buf);
    } while ((ans != 'y' && ans != 'n') || strlen(buf) != 2);

    if (ans == 'y') {
      addDiscard(player[pnum].stable.unicorns[player[pnum].stable.size - 1]);
      // player[pnum].stable.size--; <--- this is why we can't have nice things...
      rearrangeStable(pnum, player[pnum].stable.size - 1);
      destroy(pnum, ANYUNICORN, FALSE);
    }
    break;
  }
  case chainsaw_unicorn:
  {
    // Playing Chainsaw Unicorn:
    // you may DESTROY an upgrade card or SACRIFICE a downgrade card
    for (int j = 0; j < current_players; j++) {
      for (int i = 0; i < player[j].stable.size; i++) {
        if (j != pnum && player[j].stable.unicorns[i].class == UPGRADE)
          isvalid = 1;
        if (j == pnum && player[j].stable.unicorns[i].class == DOWNGRADE)
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
      ans = charinput(buf, sizeof buf);
    } while ((ans != '1' && ans != '2' && ans != '3') || strlen(buf) != 2 ||
      (ans == '1' && isvalid != 1) || (ans == '2' && isvalid2 != 1));

    if (ans == '1') {
      // destroy an upgrade card
      destroy(pnum, UPGRADE, FALSE);
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
        if (player[i].stable.unicorns[j].class == UPGRADE && i != pnum) {
          isvalid++;
          i = DESC_SIZE;
          break;
        }
      }
    }

    if (!isvalid)
      return;

    steal(pnum, UPGRADE);
    break;
  }
  case majestic_flying_unicorn:
  {
    // Playing Majestic Flying Unicorn:
    // you may add a Unicorn card from the discard pile to your hand
    searchPile(pnum, &discardpile, ANYUNICORN, ANY);
    break;
  }
  case magical_flying_unicorn:
  {
    // Playing Magical Flying Unicorn:
    // you may add a Magic card from the discard pile to your hand
    searchPile(pnum, &discardpile, MAGIC, ANY);
    break;
  }
  case swift_flying_unicorn:
  {
    // Playing Swift Flying Unicorn:
    // you may add an Instant card from the discard pile to your hand
    searchPile(pnum, &discardpile, INSTANT, ANY);
    break;
  }
  case mermaid_unicorn:
  {
    // Playing Mermaid Unicorn:
    // return a card in another player's stable to their hand
    for (int i = 0; i < current_players; i++) {
      if (i != pnum && player[i].stable.size > 0) {
        isvalid = 1;
        break;
      }
    }

    if (!isvalid)
      break;

    printPlayers();
    for (;;) {
      printf(
        "Choose a player to return a card from their stable to their "
        "hand: ");
      index = numinput(buf, &end, sizeof buf) - 1;

      // index validation
      if (index < 0 || index >= current_players || index == pnum || end != (buf + strlen(buf)))
        continue;

      if (player[index].stable.size > 0)
        break;
    }

    printStable(index);
    do {
      printf("Choose a valid card number to return: ");
      index2 = numinput(buf, &end, sizeof buf) - 1;
    } while (index2 < 0 || index2 >= player[index].stable.size || end != (buf + strlen(buf)));

    returnCardToHand(index, index2);
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
        break;
      }
    }

    if (!isvalid)
      break;

    printPlayers();
    for (;;) {
      printf("Choose a player to steal a random card from their hand: ");
      index = numinput(buf, &end, sizeof buf) - 1;

      // index validation
      if (index < 0 || index >= current_players || index == pnum || end != (buf + strlen(buf)))
        continue;

      if (player[index].hand.num_cards > 0)
        break;
    }

    if ((player[index].flags & nanny_cam) == nanny_cam) {
      // pick the card if the chosen player has the nanny cam downgrade card
      printHand(index);
      for (;;) {
        printf("Player #%d: %s has Nanny Cam in effect, so you may choose a card "
               "to pull from their hand: ", index + 1, player[index].username);
        index2 = numinput(buf, &end, sizeof buf) - 1;

        // index validation
        if (index2 >= 0 && index2 < player[index].hand.num_cards && end == (buf + strlen(buf)))
          break;
      }
    }
    else {
      // set index2 to be some random card within the chosen player's range
      index2 = rand() % player[index].hand.num_cards;
    }

    // discard time! pnum is current player, index is the stolen player
    // number and index2 is the card item being transferred
    player[pnum].hand.cards[player[pnum].hand.num_cards] =
      player[index].hand.cards[index2];
    player[pnum].hand.num_cards++;
    rearrangeHand(index, index2);
    break;
  }
  case classy_narwhal:
  {
    // Playing Classy Narwhal
    // search the deck for an upgrade card and add it to your hand. shuffle
    // the deck
    searchPile(pnum, &deck, UPGRADE, ANY);
    shuffleDeck(&deck);
    break;
  }
  case shabby_the_narwhal:
  {
    // Playing Shabby the Narwhal
    // search the deck for a downgrade card and add it to your hand. shuffle
    // the deck
    searchPile(pnum, &deck, DOWNGRADE, ANY);
    shuffleDeck(&deck);
    break;
  }
  case the_great_narwhal:
  {
    // Playing The Great Narwhal
    // search the deck for a card with "Narwhal" in its name and add it to
    // your hand. shuffle the deck
    searchPile(pnum, &deck, ANY, NARWHAL);
    shuffleDeck(&deck);
    break;
  }
  case rainbow_unicorn:
  {
    // Playing Rainbow Unicorn
    // bring a basic unicorn card from your hand directly into your
    // stable

    // someone else has a queen bee unicorn, so the player is unable to play
    // basic unicorns
    if ((player[pnum].flags & queen_bee_unicorn) != 0)
      break;

    for (int i = 0; i < player[pnum].hand.num_cards; i++) {
      // use the player's card ID (taken from deck_ref) to access the main
      // deck to check the original cards classification
      if (player[pnum].hand.cards[i].class == BASICUNICORN) {
        isvalid = 1;
        break;
      }
    }

    // don't have to check before because this is an optional effect
    if (isvalid == 0) break;

    printHand(pnum);
    for (;;) {
      printf("Choose a Basic Unicorn to bring into your stable: ");
      index = numinput(buf, &end, sizeof buf) - 1;

      // index validation
      if (index < 0 || index >= player[pnum].hand.num_cards || end != (buf + strlen(buf)))
        continue;

      // make sure it's actually a basic unicorn
      if (player[pnum].hand.cards[index].class == BASICUNICORN)
        break;
    }

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

  switch (effect) {
  case NOTHING:
    break;
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
    for (int i = player[pnum].hand.num_cards - 1; i >= 0; i--) {
      // move all cards from hand to the discard pile
      addDiscard(player[pnum].hand.cards[i]);
      player[pnum].hand.num_cards--;
    }
    shuffleDiscard();
    draw(pnum, 5);
    break;
  }
  default:
    break;
  }
}

// switch cases for beginning of your turn effects
void beginningTurnEffects(int pnum, struct Unicorn corn) {
  int index, index2 = 0, isvalid = 0, isvalid2 = 0;
  char ans, * end, buf[LINE_MAX];

  // Blinding Light makes it so that all unicorn cards have no effects
  if ((player[pnum].flags & blinding_light) != 0 &&
      (player[pnum].flags & pandamonium) == 0 &&
      corn.species != NOSPECIES)
    return;

  switch (corn.effect) {
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
    if (nursery.size == 0) break;

    do {
      printf(
        "Would you like to discard 1 card in order to bring a baby "
        "unicorn directly to your stable (y/n)?: ");
      ans = charinput(buf, sizeof buf);
    } while ((ans != 'y' && ans != 'n') || strlen(buf) != 2);
    if (ans == 'y') {
      // discard then bring in a baby unicorn :D
      discard(pnum, 1, ANY);
      printPile(nursery);
      do {
        printf("Pick the index associated with your desired Baby Unicorn: ");
        index = numinput(buf, &end, sizeof buf) - 1;
      } while (index < 0 || index >= nursery.size || end != (buf + strlen(buf)));

      addStable(pnum, nursery.cards[index]);
      rearrangePile(&nursery, index);
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
      if (player[pnum].hand.cards[i].class == BASICUNICORN ||
          player[pnum].hand.cards[i].class == MAGICUNICORN) {
        isvalid = 1;
        break;
      }
    }

    // check if there are actually cards available to take
    for (int i = 0; i < discardpile.size; i++) {
      if (discardpile.cards[i].class == BASICUNICORN ||
          discardpile.cards[i].class == MAGICUNICORN) {
        isvalid2 = 1;
        break;
      }
    }

    // no valid cards are available
    if (isvalid == 0 || isvalid2 == 0) break;

    do {
      printf(
        "Would you like to discard a unicorn card in order to bring a "
        "unicorn card from the discard pile directly to your stable at "
        "the expense of immediately ending your turn (y/n)?: ");
      ans = charinput(buf, sizeof buf);
    } while ((ans != 'y' && ans != 'n') || strlen(buf) != 2);

    if (ans == 'y') {
      discard(pnum, 1, ANYUNICORN);

      printPileFilter(discardpile, ANYUNICORN, ANY);
      for (;;) {
        printf("Pick a valid card number to add to your stable: ");
        index = numinput(buf, &end, sizeof buf) - 1;

        // index validation
        if (index < 0 || index >= discardpile.size || end != (buf + strlen(buf)))
          continue;

        // unicorn card check
        if (discardpile.cards[index].class == BASICUNICORN ||
            discardpile.cards[index].class == MAGICUNICORN)
          break;

      }

      addStable(pnum, discardpile.cards[index]);
      rearrangePile(&discardpile, index);

      // reduce turn_count in favor of "ending turn," especially since there might
      // be multiple "beginning of turn" effects that could be played
      turn_count = -1;
    }
    break;
  }
  case summoning_ritual:
  {
    // Playing Summoning Ritual:
    // you may discard 2 unicorn cards to bring a unicorn card directly from
    // the discard pile into your stable

    // check if there are unicorn cards to discard
    for (int i = 0; i < player[pnum].hand.num_cards; i++) {
      if (player[pnum].hand.cards[i].class == BASICUNICORN ||
          player[pnum].hand.cards[i].class == MAGICUNICORN) {
        isvalid++;
      }
    }

    // conditions weren't met
    if (isvalid < 2)
      break;

    do {
      printf(
        "Would you like to discard 2 unicorn cards in order to bring a "
        "unicorn card from the discard pile directly to your stable "
        "(y/n)?: ");
      ans = charinput(buf, sizeof buf);
    } while ((ans != 'y' && ans != 'n') || strlen(buf) != 2);

    if (ans == 'y') {
      discard(pnum, 2, ANYUNICORN);
      printPileFilter(discardpile, ANYUNICORN, ANY);

      for (;;) {
        printf("Pick a valid card number to add to your stable: ");
        index = numinput(buf, &end, sizeof buf) - 1;

        // index validation
        if (index < 0 || index >= discardpile.size|| end != (buf + strlen(buf)))
          continue;

        if (discardpile.cards[index].class == BASICUNICORN ||
            discardpile.cards[index].class == MAGICUNICORN)
          break;
      }
      struct Unicorn tmp = discardpile.cards[index];
      rearrangePile(&discardpile, index);
      addStable(pnum, tmp);
    }
    break;
  }
  case angel_unicorn:
  {
    // Playing Angel Unicorn:
    // you may sacrifice this card and bring a unicorn card from the discard
    // pile directly into your stable

    // check if there are actually cards available to take
    for (int i = 0; i < discardpile.size; i++) {
      if (discardpile.cards[i].class == BASICUNICORN ||
          discardpile.cards[i].class == MAGICUNICORN) {
        isvalid = 1;
        break;
      }
    }

    // no valid cards are available
    if (!isvalid)
      break;

    do {
      printf(
        "Would you like to sacrifice this card, the Angel Unicorn, in "
        "order to bring a unicorn card from the discard pile directly to "
        "your stable (y/n)?: ");
      ans = charinput(buf, sizeof buf);
    } while ((ans != 'y' && ans != 'n') || strlen(buf) != 2);

    if (ans == 'y') {
      for (int i = 0; i < player[pnum].stable.size; i++) {
        // ID of Angel Unicorn is 51
        if (strcmp(player[pnum].stable.unicorns[i].name, "Angel Unicorn") == 0) {
          index2 = i;
          break;
        }
      }

      // keep track of the angel unicorn card so that it's filtered outside of
      // the discard pile. there would be no point in taking the card back
      struct Unicorn angel_tmp = player[pnum].stable.unicorns[index2];
      rearrangeStable(pnum, index2);

      printPileFilter(discardpile, ANYUNICORN, ANY);
      for (;;) {
        printf("Pick a valid card number to add to your stable: ");
        index = numinput(buf, &end, sizeof buf) - 1;

        // index validation
        if (index < 0 || index >= discardpile.size || end != (buf + strlen(buf)))
          continue;

        if (discardpile.cards[index].class == BASICUNICORN ||
            discardpile.cards[index].class == MAGICUNICORN)
          break;
      }
      // add it to the discard before bringing the other unicorn over in case
      // it has a special enter stable effect that can bring unicorns from
      // the discard pile
      addDiscard(angel_tmp);

      addStable(pnum, discardpile.cards[index]);
      rearrangePile(&discardpile, index);
    }
    break;
  }
  case sadistic_ritual:
  {
    // Playing Sadistic Ritual:
    // you must sacrifice 1 unicorn card, then draw a card

    // can't draw cards if there's nothing to sacrifice
    isvalid = sacrifice(pnum, ANYUNICORN);
    if (!isvalid) {
      break;
    }
    draw(pnum, 1);
    break;
  }
  case glitter_bomb:
  {
    // Playing Glitter Bomb:
    // you may sacrifice a card, then destroy a card

    // check if there are cards to sacrifice and destroy first; only worry
    // about /that/ card in terms of sacrificing here since this can only
    // be the current player's turn
    isvalid = checkNumCardsToDestroy(pnum, ANY, FALSE);

    if (!isvalid)
      break;

    do {
      printf(
        "Would you like to sacrifice a card in order to destroy another "
        "card (y/n)?: ");
      ans = charinput(buf, sizeof buf);
    } while ((ans != 'y' && ans != 'n') || strlen(buf) != 2);

    if (ans == 'y') {
      sacrifice(pnum, ANY);
      destroy(pnum, ANY, FALSE);
    }
    break;
  }
  case rhinocorn:
  {
    // Playing Rhinocorn:
    // you may destroy a unicorn card, then immediately end your turn

    // check if there are unicorn cards to destroy
    isvalid = checkNumCardsToDestroy(pnum, ANYUNICORN, FALSE);
    if (!isvalid)
      break;

    do {
      printf(
        "Would you like to destroy a unicorn card at the expense of "
        "ending your turn (y/n)?: ");
      ans = charinput(buf, sizeof buf);
    } while ((ans != 'y' && ans != 'n') || strlen(buf) != 2);

    if (ans == 'y') {
      destroy(pnum, ANYUNICORN, FALSE);
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
        isvalid = 1;
        break;
      }
    }

    if (!isvalid)
      break;

    do {
      printf(
        "Would you like to steal a unicorn card and then return it at "
        "the end of your turn (y/n)?: ");
      ans = charinput(buf, sizeof buf);
    } while ((ans != 'y' && ans != 'n') || strlen(buf) != 2);

    // just copied the steal function because it has to be modified to fit
    // in the special flag
    if (ans == 'y') {
      printPlayers();
      for (;;) {
        printf("Choose a player to steal from: ");
        index = numinput(buf, &end, sizeof buf) - 1;

        // index validation
        if (index < 0 || index >= current_players || index == pnum || end != (buf + strlen(buf)))
          continue;

        // double checks that the chosen player actually has a unicorn card
        if (player[index].stable.num_unicorns > 0)
          break;
      }

      printStable(index);
      for (;;) {
        printf("Choose the card number to steal: ");
        index2 = numinput(buf, &end, sizeof buf) - 1;

        // index validation
        if (index2 < 0 || index2 >= player[index].stable.size || end != (buf + strlen(buf)))
          continue;

        // check that the chosen card is actually a unicorn
        if (checkClass(ANYUNICORN, player[index].stable.unicorns[index2].class)) {
          break;
        }
      }

      // keep track of the card index for pnum (since it's the last entry,
      // it should equal the current stable size), current player ID,
      // and stolen player's ID in order to return the card at the end of
      // the turn
      uni_lasso_flag[0] = player[pnum].stable.size;
      uni_lasso_flag[1] = pnum;
      uni_lasso_flag[2] = index;

      // rearrange the player's stable and toggle any flags before swapping the card to the other stable;
      // this will (hopefully) prevent dupes from card effect shenanigans
      struct Unicorn tmp = player[index].stable.unicorns[index2];
      rearrangeStable(index, index2);

      addStable(pnum, tmp);
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

    // check if Basic Unicorns are even in the player's hand
    for (int i = 0; i < player[pnum].hand.num_cards; i++) {
      if (player[pnum].hand.cards[i].class == BASICUNICORN) {
        isvalid = 1;
        break;
      }
    }

    if (!isvalid)
      break;

    for (;;) {
      printf("Choose a valid card number to place into your stable: \n");
      for (int i = 0; i < player[pnum].hand.num_cards; i++) {
        if (player[pnum].hand.cards[i].class == BASICUNICORN) {
          printf("    %d. %s [ID: %d]\n", i + 1, player[pnum].hand.cards[i].name, player[pnum].hand.cards[i].id);
        }
      }
      printf("Choice: ");
      index = numinput(buf, &end, sizeof buf) - 1;

      // index validation
      if (index < 0 || index >= player[pnum].hand.num_cards || end != (buf + strlen(buf)))
        continue;

      if (player[pnum].hand.cards[index].class == BASICUNICORN)
        break;
    }

    addStable(pnum, player[pnum].hand.cards[index]);
    rearrangeHand(pnum, index);
    break;
  }
  default:
    break;
  }
}
