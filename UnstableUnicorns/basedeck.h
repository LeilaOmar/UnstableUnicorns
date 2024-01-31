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
                        NOTHING,  // 0 = nothing
          greedy_flying_unicorn,  // 1 = DRAW 1 card (when this card enters your stable - > referred to as e.y.s.). if this card is sacrificed or destroyed, return it to your hand
             unicorn_on_the_cob,  // 2 = DRAW 2 cards and DISCARD 1 card (e.y.s.)
                 change_of_luck,  // 3 = DRAW 2 cards and DISCARD 3 cards (then take another turn)
                      good_deal,  // 4 = DRAW 3 cards and DISCARD 1 card
                       shake_up,  // 5 = shuffle your card, hand, and discard pile into the deck. DRAW 5 cards
                     extra_tail,  // 6 = DRAW 1 card (if this card is in your stable at the beginning of your turn -> referred to as b.o.t.); this card can only enter a stable if there is a basic unicorn card there
        annoying_flying_unicorn,  // 7 = force another player to DISCARD 1 card (e.y.s.). if this card is sacrificed or destroyed, return it to your hand
      extremely_fertile_unicorn,  // 8 = DISCARD 1 card, then bring a baby unicorn from the nursery directly into your stable (b.o.t.)
                mystical_vortex,  // 9 = every player must DISCARD 1 card. shuffle the discard pile into the deck (e.y.s.)
              seductive_unicorn,  // 10 = DISCARD 1 card, then STEAL a unicorn card (e.y.s.)
                 zombie_unicorn,  // 11 = DISCARD 1 card, then choose a unicorn card from the discard pile and bring it directly into your stable, then immediately end your turn (b.o.t.)
                      llamacorn,  // 12 = each player must DISCARD 1 card. shuffle the discard pile into the deck (no trigger)
               summoning_ritual,  // 13 = DISCARD 2 cards, then bring a unicorn card directly from the discard pile into your stable (b.o.t.)
                  angel_unicorn,  // 14 = SACRIFICE this card, then choose a unicorn from the discard pile and bring it directly into your stable (b.o.t.)
                sadistic_ritual,  // 15 = SACRIFICE 1 unicorn card, then DRAW a card (b.o.t.)
  extremely_destructive_unicorn,  // 16 = each player must SACRIFICE 1 card (e.y.s.)
                narwhal_torpedo,  // 17 = SACRIFICE all downgrade cards (e.y.s.)
              shark_with_a_horn,  // 18 = SACRIFICE this card, then destroy a unicorn card (e.y.s.)
                   reset_button,  // 19 = SACRIFICE all upgrade/downgrade cards (all players). shuffle discard pile into the deck (no trigger)
                    two_for_one,  // 20 = SACRIFICE 1 card, then DESTROY 2 cards (no trigger)
                   glitter_bomb,  // 21 = SACRIFICE 1 card, then DESTROY 1 card (b.o.t.)
               chainsaw_unicorn,  // 23 = DESTROY an upgrade card or SACRIFICE a downgrade card (e.y.s.)
           targeted_destruction,  // 24 = DESTROY an upgrade card or SACRIFICE a downgrade card (no trigger)
                      rhinocorn,  // 25 = DESTROY 1 unicorn card, then immediately end your turn (b.o.t.)
                 unicorn_poison,  // 26 = DESTROY 1 unicorn card (no trigger)
               alluring_narwhal,  // 27 = STEAL 1 upgrade card (e.y.s.)
                  unicorn_lasso,  // 28 = STEAL 1 unicorn card, then return it at the end of your turn (b.o.t.)
                   unicorn_swap,  // 29 = move a unicorn card from your stable to any other player's then STEAL a unicorn card from that player's stable
              unicorn_shrinkray,  // 30 = move any player's unicorn cards to the discard pile without triggering any of their effects, then bring the same number of baby unicorns from the nursery directly into that player's stable
                      re_target,  // 31 = move an upgrade or downgrade card from any player's stable to any other player's stable
             stabby_the_unicorn,  // 32 = if this card is sacrificed or destroyed, you may DESTROY a unicorn card
                unicorn_phoenix,  // 33 = if this card is sacrificed or destroyed, you may DISCARD a card instead
    black_knight_unicorn_effect,  // 35 = if a unicorn card in your stable would be destroyed, you may SACRIFICE this card instead
             tiny_stable_effect,  // 36 = if at any time you have more than 5 Unicorns in your Stable, SACRIFICE 1 Unicorn card
             barbed_wire_effect,  // 37 = each time a unicorn card enters or leaves your stable, discard a card [this is evil with the unicorn shrinkray lol]
        majestic_flying_unicorn,  // 38 = add a Unicorn card from the discard pile to your hand (e.y.s.). if this card is sacrificed or destroyed, return it to your hand
         magical_flying_unicorn,  // 39 = add a Magic card from the discard pile to your hand (e.y.s.). if this card is sacrificed or destroyed, return it to your hand
           swift_flying_unicorn,  // 40 = add a Neigh card from the discard pile to your hand (e.y.s.). if this card is sacrificed or destroyed, return it to your hand
                      back_kick,  // 41 = return a card in another player's stable to their hand. that player must DISCARD a card
                mermaid_unicorn,  // 42 = return a card in another player's stable to their hand (e.y.s.)
                      americorn,  // 43 = pull a card at random from another player's hand into your hand (e.y.s.)
               blatant_thievery,  // 44 = look at another player's hand. choose a card and add it to your hand
                 unfair_bargain,  // 45 = trade hands with any other player
               nanny_cam_effect,  // 46 = your hand must be visible to all players
                 classy_narwhal,  // 47 = search the deck for an upgrade card and add it to your hand. shuffle the deck (e.y.s.)
             shabby_the_narwhal,  // 48 = search the deck for a downgrade card and add it to your hand. shuffle the deck (e.y.s.)
              the_great_narwhal,  // 49 = search the deck for a card with "Narwhal" in its name and add it to your hand. shuffle the deck (e.y.s.)
       queen_bee_unicorn_effect,  // 50 = basic unicorn cards cannot enter any player's stable except yours
             magical_kittencorn,  // 51 = this card cannot be destroyed by magic cards
            rainbow_aura_effect,  // 52 = your unicorn cards cannot be destroyed
           broken_stable_effect,  // 53 = you cannot play any upgrade cards
                slowdown_effect,  // 54 = you cannot play any neigh cards
       ginormous_unicorn_effect,  // 55 = this card counts for 2 unicorns. you cannot play any neigh cards
                     yay_effect,  // 56 = cards you play cannot be neigh'd
                   double_dutch,  // 57 = play 2 cards during your action phase (b.o.t.)
                   rainbow_mane,  // 58 = this card can only enter a stable if there is a basic unicorn card there. second half of rainbow mane (bring in basic unicorn card...)
                rainbow_unicorn,  // 59 = bring a basic unicorn card from your hand directly into your stable (e.y.s.)
          blinding_light_effect,  // 60 = all of your unicorn cards are considered basic unicorns with no effects
             pandamonium_effect,  // 61 = all of your unicorns are considered pandas. cards that affect unicorn cards do not affect your pandas
                      puppicorn,  // 62 = puppicorn nonsense
                glitter_tornado,  // 63 = glitter tornado [forgot...]
                          neigh,  // 66 = neigh
                    super_neigh   // 67 = super neigh
};

extern struct Unicorn basedeck[129];

extern int uni_lasso_flag[3];  // 0 = crd ind of p1, 1 = plyr stealer, 2 = victim
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
// 0 = returned prematurely, conditions not met (FALSE)
// 1 = success (TRUE)
// 2 = effect is not conditional/relevant; skip
int conditionalEffects(int pnum, struct Unicorn corn, int hindex, int upgrade_target);

// switch cases for enter your stable effects with magical unicorn cards
void enterStableEffects(int pnum, int effect);

// switch cases for Magic cards
void magicEffects(int pnum, int effect);

// switch cases for beginning of your turn effects
void beginningTurnEffects(int pnum, struct Unicorn corn);
