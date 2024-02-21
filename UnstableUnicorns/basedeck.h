#pragma once

#include "globals.h"

enum BaseFlags {
  BROKEN_STABLE = 1,      //!< Can't play upgrade cards
  SLOWDOWN = 2,           //!< Can't neigh others
  YAY = 4,                //!< Can't be neighed
  RAINBOW_AURA = 8,       //!< Unicorn cards can't be destroyed
  NANNY_CAM = 16,         //!< Card is visible to other players
  QUEEN_BEE_UNICORN = 32, //!< Can't play Basic Unicorn cards
  GINORMOUS_UNICORN = 64, //!< Card counts as 2 Unicorns; can't neigh others
  BLINDING_LIGHT = 128,   //!< All Unicorns have no special effects
  PANDAMONIUM = 256,      //!< Unicorns are now considered pandas
  TINY_STABLE = 512,      //!< Can't have more than 5 Unicorns in stable
  BLACK_KNIGHT_UNICORN = 1024,  //!< Can sacrifice BKU instead if another Unicorn is about to be destroyed
  BARBED_WIRE = 2048      //!< Discard a card each time a Unicorn enters/leaves the player's stable
};

// switch table effects
enum BaseCardEffects {
  NOTHING,  //!< No effect

  // card toggle effects
  BLACK_KNIGHT_UNICORN_EFFECT,  //!< If a unicorn card in your stable would be destroyed, you may SACRIFICE this card instead
  GINORMOUS_UNICORN_EFFECT,     //!< This card counts for 2 unicorns. You cannot play any neigh cards
  QUEEN_BEE_UNICORN_EFFECT,     //!< Basic unicorn cards cannot enter any player's stable except yours
  RAINBOW_AURA_EFFECT,          //!< Your unicorn cards cannot be destroyed
  YAY_EFFECT,                   //!< Cards you play cannot be neigh'd
  BARBED_WIRE_EFFECT,           //!< Each time a unicorn card enters or leaves your stable, discard a card
  BLINDING_LIGHT_EFFECT,        //!< All of your unicorn cards are considered basic unicorns with no effects
  BROKEN_STABLE_EFFECT,         //!< You cannot play any upgrade cards
  NANNY_CAM_EFFECT,             //!< Your hand must be visible to all players
  PANDAMONIUM_EFFECT,           //!< All of your unicorns are considered pandas. Cards that affect unicorn cards do not affect your pandas
  SLOWDOWN_EFFECT,              //!< You cannot play any neigh cards
  TINY_STABLE_EFFECT,           //!< If at any time you have more than 5 Unicorns in your Stable, SACRIFICE 1 Unicorn card

  // magical unicorns
  AMERICORN,                      //!< Pull a card at random from another player's hand into your hand (e.y.s.)
  ANGEL_UNICORN,                  //!< SACRIFICE this card, then choose a unicorn from the discard pile and bring it directly into your stable (b.o.t.)
  ANNOYING_FLYING_UNICORN,        //!< Force another player to DISCARD 1 card (e.y.s.). if this card is sacrificed or destroyed, return it to your hand
  GREEDY_FLYING_UNICORN,          //!< DRAW 1 card (when this card enters your stable - > referred to as e.y.s.). if this card is sacrificed or destroyed, return it to your hand
  MAGICAL_FLYING_UNICORN,         //!< Add a Magic card from the discard pile to your hand (e.y.s.). if this card is sacrificed or destroyed, return it to your hand
  MAJESTIC_FLYING_UNICORN,        //!< Add a Unicorn card from the discard pile to your hand (e.y.s.). if this card is sacrificed or destroyed, return it to your hand
  CHAINSAW_UNICORN,               //!< DESTROY an upgrade card or SACRIFICE a downgrade card (e.y.s.)
  EXTREMELY_DESTRUCTIVE_UNICORN,  //!< Each player must SACRIFICE 1 card (e.y.s.)
  EXTREMELY_FERTILE_UNICORN,      //!< DISCARD 1 card, then bring a baby unicorn from the nursery directly into your stable (b.o.t.)
  LLAMACORN,                      //!< Each player must DISCARD 1 card. shuffle the discard pile into the deck (no trigger)
  MAGICAL_KITTENCORN,             //!< This card cannot be destroyed by magic cards
  MERMAID_UNICORN,                //!< Return a card in another player's stable to their hand (e.y.s.)
  PUPPICORN,                      //!< This card moves to the next player's stable at the start of a turn
  RAINBOW_UNICORN,                //!< Bring a basic unicorn card from your hand directly into your stable (e.y.s.)
  RHINOCORN,                      //!< DESTROY 1 unicorn card, then immediately end your turn (b.o.t.)
  SEDUCTIVE_UNICORN,              //!< DISCARD 1 card, then STEAL a unicorn card (e.y.s.)
  SHARK_WITH_A_HORN,              //!< SACRIFICE this card, then destroy a unicorn card (e.y.s.)
  STABBY_THE_UNICORN,             //!< If this card is sacrificed or destroyed, you may DESTROY a unicorn card
  SWIFT_FLYING_UNICORN,           //!< Add a Neigh card from the discard pile to your hand (e.y.s.). if this card is sacrificed or destroyed, return it to your hand
  UNICORN_ON_THE_COB,             //!< DRAW 2 cards and DISCARD 1 card (e.y.s.)
  UNICORN_PHOENIX,                //!< If this card is sacrificed or destroyed, you may DISCARD a card instead
  ZOMBIE_UNICORN,                 //!< DISCARD 1 card, then choose a unicorn card from the discard pile and bring it directly into your stable, then immediately end your turn (b.o.t.)

  // magical narwhals (still magical unicorn cards)
  ALLURING_NARWHAL,   //!< STEAL 1 upgrade card (e.y.s.)
  CLASSY_NARWHAL,     //!< Search the deck for an upgrade card and add it to your hand. shuffle the deck (e.y.s.)
  THE_GREAT_NARWHAL,  //!< Search the deck for a card with "Narwhal" in its name and add it to your hand. shuffle the deck (e.y.s.)
  NARWHAL_TORPEDO,    //!< SACRIFICE all downgrade cards (e.y.s.)
  SHABBY_THE_NARWHAL, //!< Search the deck for a downgrade card and add it to your hand. shuffle the deck (e.y.s.)

  // magic cards
  BACK_KICK,            //!< Return a card in another player's stable to their hand. that player must DISCARD a card
  BLATANT_THIEVERY,     //!< Look at another player's hand. choose a card and add it to your hand
  CHANGE_OF_LUCK,       //!< DRAW 2 cards and DISCARD 3 cards (then take another turn)
  GLITTER_TORNADO,      //!< Return a card in each player's Stable (including yours) to their hand
  GOOD_DEAL,            //!< DRAW 3 cards and DISCARD 1 card
  MYSTICAL_VORTEX,      //!< Every player must DISCARD 1 card. shuffle the discard pile into the deck (e.y.s.)
  RE_TARGET,            //!< Move an upgrade or downgrade card from any player's stable to any other player's stable
  RESET_BUTTON,         //!< SACRIFICE all upgrade/downgrade cards (all players). shuffle discard pile into the deck (no trigger)
  SHAKE_UP,             //!< Shuffle your card, hand, and discard pile into the deck. DRAW 5 cards
  TARGETED_DESTRUCTION, //!< DESTROY an upgrade card or SACRIFICE a downgrade card (no trigger)
  TWO_FOR_ONE,          //!< SACRIFICE 1 card, then DESTROY 2 cards (no trigger)
  UNFAIR_BARGAIN,       //!< Trade hands with any other player
  UNICORN_POISON,       //!< DESTROY 1 unicorn card (no trigger)
  UNICORN_SHRINKRAY,    //!< Move any player's unicorn cards to the discard pile without triggering any of their effects, then bring the same number of baby unicorns from the nursery directly into that player's stable
  UNICORN_SWAP,         //!< Move a unicorn card from your stable to any other player's then STEAL a unicorn card from that player's stable

  // upgrade cards
  DOUBLE_DUTCH,     //!< Play 2 cards during your action phase (b.o.t.)
  EXTRA_TAIL,       //!< DRAW 1 card (if this card is in your stable at the beginning of your turn -> referred to as b.o.t.); this card can only enter a stable if there is a basic unicorn card there
  GLITTER_BOMB,     //!< SACRIFICE 1 card, then DESTROY 1 card (b.o.t.)
  RAINBOW_MANE,     //!< This card can only enter a stable if there is a basic unicorn card there. second half of rainbow mane (bring in basic unicorn card...)
  SUMMONING_RITUAL, //!< DISCARD 2 cards, then bring a unicorn card directly from the discard pile into your stable (b.o.t.)
  UNICORN_LASSO,    //!< STEAL 1 unicorn card, then return it at the end of your turn (b.o.t.)

  // downgrade cards
  SADISTIC_RITUAL,  //!< SACRIFICE 1 unicorn card, then DRAW a card (b.o.t.)

  // instant cards
  NEIGH,        //!< Stop a player from playing a card
  SUPER_NEIGH   //!< Stop a player from playing a card; this cannot be Neighed
};

extern struct Unicorn Base_DECK[129];

extern int uniLassoIndex[3];   //!< 0 = card index of p1, 1 = player stealer, 2 = victim
extern int puppicornIndex[2];  //!< 0 = card index, 1 = player number of owner

// ********************************************************************************
// ***************************** Card Effect Functions ****************************
// ********************************************************************************

/**
 * @brief Switch cases for cards that may trigger specific player flags (e.g. cannot neigh)
 * @param effect The enum BaseFlags effect of the card that may be toggled
 * @return 0 Not a card w/ a togglable effect
 * @return 1 Toggle occurred
 */
int Base_ToggleFlags(int pnum, int effect);

/**
 * @brief Switch cases for when a card has a special effect upon being sacrificed or destroyed
 * @param cindex The index of the card in player[pnum]'s stable
 */
void Base_SacrificeDestroyEffects(int pnum, int cindex, int effect);

/**
 * @brief Switch cases for cards that may be unable to be played due to certain circumstances
 * @param hindex The index of the card in player[pnum]'s hand
 * @return 0 Conditions not met (FALSE)
 * @return 1 Success (TRUE)
 * @return 2 Effect is not conditional/relevant; skip
 * @note This is separated so that the hand is not rearranged unnecessarily
 */
int Base_ConditionalEffects(int pnum, struct Unicorn corn, int hindex, int upgrade_target);

/**
 * @brief Switch cases for "enter your stable" effects with Magical Unicorn cards
 */
void Base_EnterStableEffects(int pnum, int effect);

/**
 * @brief Switch cases for Magic cards
 */
void Base_MagicEffects(int pnum, int effect);

/**
 * @brief Switch cases for "beginning of your turn" effects
 */
void Base_BeginningTurnEffects(int pnum, struct Unicorn corn);
