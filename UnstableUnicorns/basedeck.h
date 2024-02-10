#pragma once
#include "globals.h"

enum baseflag {
  broken_stable = 1, slowdown = 2, yay = 4, rainbow_aura = 8,
  nanny_cam = 16, queen_bee_unicorn = 32, ginormous_unicorn = 64,
  blinding_light = 128, pandamonium = 256, tiny_stable = 512,
  black_knight_unicorn = 1024, barbed_wire = 2048
};

// switch table effects
enum BaseCardEffects {
                        NOTHING,  // nothing
  // card toggle effects
    black_knight_unicorn_effect,  // if a unicorn card in your stable would be destroyed, you may SACRIFICE this card instead
       ginormous_unicorn_effect,  // this card counts for 2 unicorns. you cannot play any neigh cards
       queen_bee_unicorn_effect,  // basic unicorn cards cannot enter any player's stable except yours
            rainbow_aura_effect,  // your unicorn cards cannot be destroyed
                     yay_effect,  // cards you play cannot be neigh'd
             barbed_wire_effect,  // each time a unicorn card enters or leaves your stable, discard a card [this is evil with the unicorn shrinkray lol]
          blinding_light_effect,  // all of your unicorn cards are considered basic unicorns with no effects
           broken_stable_effect,  // you cannot play any upgrade cards
               nanny_cam_effect,  // your hand must be visible to all players
             pandamonium_effect,  // all of your unicorns are considered pandas. cards that affect unicorn cards do not affect your pandas
                slowdown_effect,  // you cannot play any neigh cards
             tiny_stable_effect,  // if at any time you have more than 5 Unicorns in your Stable, SACRIFICE 1 Unicorn card
  // magical unicorns
               alluring_narwhal,  // STEAL 1 upgrade card (e.y.s.)
                      americorn,  // pull a card at random from another player's hand into your hand (e.y.s.)
                  angel_unicorn,  // SACRIFICE this card, then choose a unicorn from the discard pile and bring it directly into your stable (b.o.t.)
        annoying_flying_unicorn,  // force another player to DISCARD 1 card (e.y.s.). if this card is sacrificed or destroyed, return it to your hand
               chainsaw_unicorn,  // DESTROY an upgrade card or SACRIFICE a downgrade card (e.y.s.)
                 classy_narwhal,  // search the deck for an upgrade card and add it to your hand. shuffle the deck (e.y.s.)
  extremely_destructive_unicorn,  // each player must SACRIFICE 1 card (e.y.s.)
      extremely_fertile_unicorn,  // DISCARD 1 card, then bring a baby unicorn from the nursery directly into your stable (b.o.t.)
          greedy_flying_unicorn,  // DRAW 1 card (when this card enters your stable - > referred to as e.y.s.). if this card is sacrificed or destroyed, return it to your hand
                      llamacorn,  // each player must DISCARD 1 card. shuffle the discard pile into the deck (no trigger)
         magical_flying_unicorn,  // add a Magic card from the discard pile to your hand (e.y.s.). if this card is sacrificed or destroyed, return it to your hand
             magical_kittencorn,  // this card cannot be destroyed by magic cards
        majestic_flying_unicorn,  // add a Unicorn card from the discard pile to your hand (e.y.s.). if this card is sacrificed or destroyed, return it to your hand
                mermaid_unicorn,  // return a card in another player's stable to their hand (e.y.s.)
                      puppicorn,  // puppicorn nonsense
                rainbow_unicorn,  // bring a basic unicorn card from your hand directly into your stable (e.y.s.)
                      rhinocorn,  // DESTROY 1 unicorn card, then immediately end your turn (b.o.t.)
              seductive_unicorn,  // DISCARD 1 card, then STEAL a unicorn card (e.y.s.)
              shark_with_a_horn,  // SACRIFICE this card, then destroy a unicorn card (e.y.s.)
             stabby_the_unicorn,  // if this card is sacrificed or destroyed, you may DESTROY a unicorn card
           swift_flying_unicorn,  // add a Neigh card from the discard pile to your hand (e.y.s.). if this card is sacrificed or destroyed, return it to your hand
             unicorn_on_the_cob,  // DRAW 2 cards and DISCARD 1 card (e.y.s.)
                unicorn_phoenix,  // if this card is sacrificed or destroyed, you may DISCARD a card instead
                 zombie_unicorn,  // DISCARD 1 card, then choose a unicorn card from the discard pile and bring it directly into your stable, then immediately end your turn (b.o.t.)
  // magical narwhals (still magical unicorn cards)
              the_great_narwhal,  // search the deck for a card with "Narwhal" in its name and add it to your hand. shuffle the deck (e.y.s.)
                narwhal_torpedo,  // SACRIFICE all downgrade cards (e.y.s.)
             shabby_the_narwhal,  // search the deck for a downgrade card and add it to your hand. shuffle the deck (e.y.s.)
  // magic cards
                      back_kick,  // return a card in another player's stable to their hand. that player must DISCARD a card
               blatant_thievery,  // look at another player's hand. choose a card and add it to your hand
                 change_of_luck,  // DRAW 2 cards and DISCARD 3 cards (then take another turn)
                glitter_tornado,  // return a card in each player's Stable (including yours) to their hand
                      good_deal,  // DRAW 3 cards and DISCARD 1 card
                mystical_vortex,  // every player must DISCARD 1 card. shuffle the discard pile into the deck (e.y.s.)
                      re_target,  // move an upgrade or downgrade card from any player's stable to any other player's stable
                   reset_button,  // SACRIFICE all upgrade/downgrade cards (all players). shuffle discard pile into the deck (no trigger)
                       shake_up,  // shuffle your card, hand, and discard pile into the deck. DRAW 5 cards
           targeted_destruction,  // DESTROY an upgrade card or SACRIFICE a downgrade card (no trigger)
                    two_for_one,  // SACRIFICE 1 card, then DESTROY 2 cards (no trigger)
                 unfair_bargain,  // trade hands with any other player
                 unicorn_poison,  // DESTROY 1 unicorn card (no trigger)
              unicorn_shrinkray,  // move any player's unicorn cards to the discard pile without triggering any of their effects, then bring the same number of baby unicorns from the nursery directly into that player's stable
                   unicorn_swap,  // move a unicorn card from your stable to any other player's then STEAL a unicorn card from that player's stable
  // upgrade cards
                   double_dutch,  // play 2 cards during your action phase (b.o.t.)
                     extra_tail,  // DRAW 1 card (if this card is in your stable at the beginning of your turn -> referred to as b.o.t.); this card can only enter a stable if there is a basic unicorn card there
                   glitter_bomb,  // SACRIFICE 1 card, then DESTROY 1 card (b.o.t.)
                   rainbow_mane,  // this card can only enter a stable if there is a basic unicorn card there. second half of rainbow mane (bring in basic unicorn card...)
               summoning_ritual,  // DISCARD 2 cards, then bring a unicorn card directly from the discard pile into your stable (b.o.t.)
                  unicorn_lasso,  // STEAL 1 unicorn card, then return it at the end of your turn (b.o.t.)
  // downgrade cards
                sadistic_ritual,  // SACRIFICE 1 unicorn card, then DRAW a card (b.o.t.)
  // instant cards
                          neigh,  // neigh
                    super_neigh   // super neigh
};

extern struct Unicorn basedeck[129];

extern int uni_lasso_flag[3];   // 0 = card index of p1, 1 = player stealer, 2 = victim
extern int puppicorn_index[2];  // 0 = card index, 1 = player number of owner

// ********************************************************************************
// ***************************** Card Effect Functions ****************************
// ********************************************************************************


// switch cases for cards that may trigger specific player flags (e.g. cannot neigh)
// 0 = not a card w/ a togglable effect
// 1 = toggle did happen
int toggleFlags(int pnum, int effect);

// switch cases for when a card has a special effect upon being sacrificed or destroyed
void sacrificeDestroyEffects(int pnum, int cindex, int effect);

// switch cases for cards that may be unable to be played due to certain circumstances;
// this is separated so that the hand is not rearranged unnecessarily;
// 0 = conditions not met (FALSE)
// 1 = success (TRUE)
// 2 = effect is not conditional/relevant; skip
int conditionalEffects(int pnum, struct Unicorn corn, int hindex, int upgrade_target);

// switch cases for enter your stable effects with magical unicorn cards
void enterStableEffects(int pnum, int effect);

// switch cases for Magic cards
void magicEffects(int pnum, int effect);

// switch cases for beginning of your turn effects
void beginningTurnEffects(int pnum, struct Unicorn corn);
