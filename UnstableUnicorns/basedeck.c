#include "basedeck.h"
#include "gamemechanics.h"
#include "gamephase.h"
#include "networkevents.h"

// Base game deck (ver. 1 before the reprint)
struct Unicorn Base_DECK[] = {
    {BABYUNICORN, UNICORN, "Baby Unicorn (Red)",
     "If this card would be sacrificed, destroyed, or returned to your hand, "
     "return it to the Nursery instead.",
     NOTHING, 0, 0},
    {BABYUNICORN, UNICORN, "Baby Unicorn (Pink)",
     "If this card would be sacrificed, destroyed, or returned to your hand, "
     "return it to the Nursery instead.",
     NOTHING, 0, 0},
    {BABYUNICORN, UNICORN, "Baby Unicorn (Orange)",
     "If this card would be sacrificed, destroyed, or returned to your hand, "
     "return it to the Nursery instead.",
     NOTHING, 0, 0},
    {BABYUNICORN, UNICORN, "Baby Unicorn (Yellow)",
     "If this card would be sacrificed, destroyed, or returned to your hand, "
     "return it to the Nursery instead.",
     NOTHING, 0, 0},
    {BABYUNICORN, UNICORN, "Baby Unicorn (Green)",
     "If this card would be sacrificed, destroyed, or returned to your hand, "
     "return it to the Nursery instead.",
     NOTHING, 0, 0},
    {BABYUNICORN, UNICORN, "Baby Unicorn (Blue)",
     "If this card would be sacrificed, destroyed, or returned to your hand, "
     "return it to the Nursery instead.",
     NOTHING, 0, 0},
    {BABYUNICORN, UNICORN, "Baby Unicorn (Purple)",
     "If this card would be sacrificed, destroyed, or returned to your hand, "
     "return it to the Nursery instead.",
     NOTHING, 0, 0},
    {BABYUNICORN, UNICORN, "Baby Unicorn (Black)",
     "If this card would be sacrificed, destroyed, or returned to your hand, "
     "return it to the Nursery instead.",
     NOTHING, 0, 0},
    {BABYUNICORN, UNICORN, "Baby Unicorn (White)",
     "If this card would be sacrificed, destroyed, or returned to your hand, "
     "return it to the Nursery instead.",
     NOTHING, 0, 0},
    {BABYUNICORN, UNICORN, "Baby Unicorn (Brown)",
     "If this card would be sacrificed, destroyed, or returned to your hand, "
     "return it to the Nursery instead.",
     NOTHING, 0, 0},
    {BABYUNICORN, UNICORN, "Baby Unicorn (Rainbow)",
     "If this card would be sacrificed, destroyed, or returned to your hand, "
     "return it to the Nursery instead.",
     NOTHING, 0, 0},
    {BABYUNICORN, UNICORN, "Baby Unicorn (Death)",
     "If this card would be sacrificed, destroyed, or returned to your hand, "
     "return it to the Nursery instead.",
     NOTHING, 0, 0},
    {BABYUNICORN, NARWHAL, "Baby Narwhal",
     "If this card would be sacrificed, destroyed, or returned to your hand, "
     "return it to the Nursery instead.",
     NOTHING, 0, 0},
    {BASICUNICORN, UNICORN, "Basic Unicorn (Red)", "Beards are like, so hot.",
     NOTHING, 0, 0},
    {BASICUNICORN, UNICORN, "Basic Unicorn (Red)", "Beards are like, so hot.",
     NOTHING, 0, 0},
    {BASICUNICORN, UNICORN, "Basic Unicorn (Red)", "Beards are like, so hot.",
     NOTHING, 0, 0},
    {BASICUNICORN, UNICORN, "Basic Unicorn (Orange)",
     "Pumpkin spice is the pumpkin spice of life.", NOTHING, 0, 0},
    {BASICUNICORN, UNICORN, "Basic Unicorn (Orange)",
     "Pumpkin spice is the pumpkin spice of life.", NOTHING, 0, 0},
    {BASICUNICORN, UNICORN, "Basic Unicorn (Orange)",
     "Pumpkin spice is the pumpkin spice of life.", NOTHING, 0, 0},
    {BASICUNICORN, UNICORN, "Basic Unicorn (Yellow)",
     "Dance like nobody's watching.", NOTHING, 0, 0},
    {BASICUNICORN, UNICORN, "Basic Unicorn (Yellow)",
     "Dance like nobody's watching.", NOTHING, 0, 0},
    {BASICUNICORN, UNICORN, "Basic Unicorn (Yellow)",
     "Dance like nobody's watching.", NOTHING, 0, 0},
    {BASICUNICORN, UNICORN, "Basic Unicorn (Green)",
     "Vinyl records and mixtapes only.", NOTHING, 0, 0},
    {BASICUNICORN, UNICORN, "Basic Unicorn (Green)",
     "Vinyl records and mixtapes only.", NOTHING, 0, 0},
    {BASICUNICORN, UNICORN, "Basic Unicorn (Green)",
     "Vinyl records and mixtapes only.", NOTHING, 0, 0},
    {BASICUNICORN, UNICORN, "Basic Unicorn (Blue)",
     "Popped collars are for date nights only.", NOTHING, 0, 0},
    {BASICUNICORN, UNICORN, "Basic Unicorn (Blue)",
     "Popped collars are for date nights only.", NOTHING, 0, 0},
    {BASICUNICORN, UNICORN, "Basic Unicorn (Blue)",
     "Popped collars are for date nights only.", NOTHING, 0, 0},
    {BASICUNICORN, UNICORN, "Basic Unicorn (Indigo)", "[emojis]", NOTHING, 0, 0},
    {BASICUNICORN, UNICORN, "Basic Unicorn (Indigo)", "[emojis]", NOTHING, 0, 0},
    {BASICUNICORN, UNICORN, "Basic Unicorn (Indigo)", "[emojis]", NOTHING, 0, 0},
    {BASICUNICORN, UNICORN, "Basic Unicorn (Purple)",
     "#nomakeup #nofilter #sunnies #shameless #selfie #basic #TGIF # "
     "unicornhairdontcare",
     NOTHING, 0, 0},
    {BASICUNICORN, UNICORN, "Basic Unicorn (Purple)",
     "#nomakeup #nofilter #sunnies #shameless #selfie #basic #TGIF # "
     "unicornhairdontcare",
     NOTHING, 0, 0},
    {BASICUNICORN, UNICORN, "Basic Unicorn (Purple)",
     "#nomakeup #nofilter #sunnies #shameless #selfie #basic #TGIF # "
     "unicornhairdontcare",
     NOTHING, 0, 0},
    {BASICUNICORN, NARWHAL, "Narwhal",
     "This card has no special powers, but it sure is cute!", NOTHING, 0, 0},
    {BASICUNICORN, NARWHAL, "Narwhal",
     "This card has no special powers, but it sure is cute!", NOTHING, 0, 0},
    {BASICUNICORN, NARWHAL, "Narwhal",
     "This card has no special powers, but it sure is cute!", NOTHING, 0, 0},
    {MAGICALUNICORN, UNICORN, "Rhinocorn",
     "If this card is in your Stable at the beginning of your turn, you may "
     "DESTROY a Unicorn card. If you do, immediately end your turn.",
     RHINOCORN, 0, 0},
    {MAGICALUNICORN, UNICORN, "Extremely Fertile Unicorn",
     "If this card is in your Stable at the beginning of your turn, you may "
     "DISCARD a card. If you do, bring a Baby Unicorn card from the Nursery "
     "directly into your Stable.",
     EXTREMELY_FERTILE_UNICORN, 0, 0},
    {MAGICALUNICORN, UNICORN, "Magical Kittencorn",
     "This card cannot be destroyed by Magic cards.", MAGICAL_KITTENCORN, 0, 0},
    {MAGICALUNICORN, UNICORN, "Stabby the Unicorn",
     "If this card is sacrificed or destroyed, you may DESTROY a Unicorn card.",
     STABBY_THE_UNICORN, 0, 0},
    {MAGICALUNICORN, UNICORN, "Puppicorn",
     "At the end of your turn, move Puppicorn to the next player's Stable. "
     "This card cannot be sacrificed or destroyed.",
     PUPPICORN, 0, 0},
    {MAGICALUNICORN, UNICORN, "Rainbow Unicorn",
     "When this card enters your Stable, you may bring a Basic Unicorn card "
     "from your hand into your Stable.",
     RAINBOW_UNICORN, 0, 0},
    {MAGICALUNICORN, UNICORN, "Zombie Unicorn",
     "If this card is in your Stable at the beginning of your turn, you may "
     "DISCARD a Unicorn card. If you do, choose a Unicorn card from the "
     "discard pile and bring it directly into your Stable, then immediately "
     "skip to your End of Turn Phase",
     ZOMBIE_UNICORN, 0, 0},
    {MAGICALUNICORN, UNICORN, "Extremely Destructive Unicorn",
     "When this card enters your Stable, each player (including you) must "
     "SACRIFICE a Unicorn card.",
     EXTREMELY_DESTRUCTIVE_UNICORN, 0, 0},
    {MAGICALUNICORN, UNICORN, "Chainsaw Unicorn",
     "When this card enters your Stable, you may DESTROY an Upgrade card or "
     "SACRIFICE a Downgrade card.",
     CHAINSAW_UNICORN, 0, 0},
    {MAGICALUNICORN, UNICORN, "Llamacorn",
     "When this card enters your Stable, each player (including you) must "
     "DISCARD a card.",
     LLAMACORN, 0, 0},
    {MAGICALUNICORN, UNICORN, "Americorn",
     "When this card enters your Stable, you may pull a card at random from "
     "another player's hand.",
     AMERICORN, 0, 0},
    {MAGICALUNICORN, UNICORN, "Ginormous Unicorn",
     "This card counts for 2 Unicorns. You cannot play any Instant cards.", GINORMOUS_UNICORN_EFFECT, 0, 0},
    {MAGICALUNICORN, UNICORN, "Seductive Unicorn",
     "When this card enters your Stable, you may DISCARD a card, then STEAL a "
     "Unicorn card.",
     SEDUCTIVE_UNICORN, 0, 0},
    {MAGICALUNICORN, UNICORN, "Angel Unicorn",
     "If this card is in your Stable at the beginning of your turn, you may "
     "SACRIFICE this card. If you do, choose a Unicorn card from the discard "
     "pile and bring it directly into your Stable.",
     ANGEL_UNICORN, 0, 0},
    {MAGICALUNICORN, UNICORN, "Queen Bee Unicorn",
     "Basic Unicorn cards cannot enter any player's Stable except yours.", QUEEN_BEE_UNICORN_EFFECT, 0, 0},
    {MAGICALUNICORN, UNICORN, "Greedy Flying Unicorn",
     "When this card enters your Stable, DRAW a card. If this card is "
     "sacrificed or destroyed, return it to your hand.",
     GREEDY_FLYING_UNICORN, 0, 0},
    {MAGICALUNICORN, UNICORN, "Annoying Flying Unicorn",
     "When this card enters your Stable, you may force another player to "
     "DISCARD a card. If this card is sacrificed or destroyed, return it to "
     "your hand.",
     ANNOYING_FLYING_UNICORN, 0, 0},
    {MAGICALUNICORN, UNICORN, "Magical Flying Unicorn",
     "When this card enters your Stable, you may choose a Magic card from the "
     "discard pile and add it to your hand. If this card is sacrificed or destroyed, "
     "return it to your hand.",
     MAGICAL_FLYING_UNICORN, 0, 0},
    {MAGICALUNICORN, UNICORN, "Swift Flying Unicorn",
     "When this card enters your Stable, you may choose an Instant card from the "
     "discard pile and add it to your hand. If this card is sacrificed or destroyed, "
     "return it to your hand.",
     SWIFT_FLYING_UNICORN, 0, 0},
    {MAGICALUNICORN, UNICORN, "Majestic Flying Unicorn",
     "When this card enters your Stable, you may choose a Unicorn card from the "
     "discard pile and add it to your hand. If this card is sacrificed or destroyed, "
     "return it to your hand.",
     MAJESTIC_FLYING_UNICORN, 0, 0},
    {MAGICALUNICORN, UNICORN, "Unicorn Phoenix",
     "If this card would be sacrificed or destroyed, you may DISCARD a card "
     "instead.",
     UNICORN_PHOENIX, 0, 0},
    {MAGICALUNICORN, UNICORN, "Unicorn on the Cob",
     "When this card enters your Stable, DRAW 2 cards and DISCARD a card.", UNICORN_ON_THE_COB, 0, 0},
    {MAGICALUNICORN, UNICORN, "Black Knight Unicorn",
     "If a Unicorn card in your Stable would be destroyed, you may SACRIFICE "
     "this card instead.",
     BLACK_KNIGHT_UNICORN_EFFECT, 0, 0},
    {MAGICALUNICORN, UNICORN, "Shark With a Horn",
     "When this card enters your Stable, you may SACRIFICE this card, then "
     "DESTROY a Unicorn card.",
     SHARK_WITH_A_HORN, 0, 0},
    {MAGICALUNICORN, NARWHAL, "Shabby the Narwhal",
     "When this card enters your Stable, you may search the deck for a "
     "Downgrade card and add it to your hand, then shuffle the deck.",
     SHABBY_THE_NARWHAL, 0, 0},
    {MAGICALUNICORN, NARWHAL, "Narwhal Torpedo",
     "When this card enters your Stable, SACRIFICE all Downgrade cards in your "
     "Stable.",
     NARWHAL_TORPEDO, 0, 0},
    {MAGICALUNICORN, NARWHAL, "Alluring Narwhal",
     "When this card enters your Stable, you may STEAL an Upgrade card.", ALLURING_NARWHAL, 0, 0},
    {MAGICALUNICORN, UNICORN, "Mermaid Unicorn",
     "When this card enters your Stable, return a card in another player's "
     "Stable to their hand.",
     MERMAID_UNICORN, 0, 0},
    {MAGICALUNICORN, NARWHAL, "Classy Narwhal",
     "When this card enters your Stable, you may search the deck for an "
     "Upgrade card and add it to your hand, then shuffle the deck.",
     CLASSY_NARWHAL, 0, 0},
    {MAGICALUNICORN, NARWHAL, "The Great Narwhal",
     "When this card enters your Stable, you may search the deck for a card "
     "with \"Narwhal\" in its name and add it to your hand, then shuffle the "
     "deck.",
     THE_GREAT_NARWHAL, 0, 0},
    {MAGIC, NOSPECIES, "Unicorn Poison", "DESTROY a Unicorn card.", UNICORN_POISON, 0, 0},
    {MAGIC, NOSPECIES, "Unicorn Poison", "DESTROY a Unicorn card.", UNICORN_POISON, 0, 0},
    {MAGIC, NOSPECIES, "Unicorn Poison", "DESTROY a Unicorn card.", UNICORN_POISON, 0, 0},
    {MAGIC, NOSPECIES, "Back Kick",
     "Return a card in another player's Stable to their hand. That player must "
     "DISCARD a card.",
     BACK_KICK, 0, 0},
    {MAGIC, NOSPECIES, "Back Kick",
     "Return a card in another player's Stable to their hand. That player must "
     "DISCARD a card.",
     BACK_KICK, 0, 0},
    {MAGIC, NOSPECIES, "Back Kick",
     "Return a card in another player's Stable to their hand. That player must "
     "DISCARD a card.",
     BACK_KICK, 0, 0},
    {MAGIC, NOSPECIES, "Change of Luck",
     "DRAW 2 cards and DISCARD 3 cards, then take another turn.", CHANGE_OF_LUCK, 0, 0},
    {MAGIC, NOSPECIES, "Change of Luck",
     "DRAW 2 cards and DISCARD 3 cards, then take another turn.", CHANGE_OF_LUCK, 0, 0},
    {MAGIC, NOSPECIES, "Glitter Tornado",
     "Return a card in each player's Stable (including yours) to their hand.",
     GLITTER_TORNADO, 0, 0},
    {MAGIC, NOSPECIES, "Glitter Tornado",
     "Return a card in each player's Stable (including yours) to their hand.",
     GLITTER_TORNADO, 0, 0},
    {MAGIC, NOSPECIES, "Unicorn Swap",
     "Move a Unicorn card in your Stable to any other player's Stable, then "
     "STEAL a Unicorn card from that player's Stable.",
     UNICORN_SWAP, 0, 0},
    {MAGIC, NOSPECIES, "Unicorn Swap",
     "Move a Unicorn card in your Stable to any other player's Stable, then "
     "STEAL a Unicorn card from that player's Stable.",
     UNICORN_SWAP, 0, 0},
    {MAGIC, NOSPECIES, "Re-Target",
     "Move an Upgrade or Downgrade card from any player's Stable to any other "
     "player's Stable.",
     RE_TARGET, 0, 0},
    {MAGIC, NOSPECIES, "Re-Target",
     "Move an Upgrade or Downgrade card from any player's Stable to any other "
     "player's Stable.",
     RE_TARGET, 0, 0},
    {MAGIC, NOSPECIES, "Unfair Bargain", "Trade hands with any other player.",
     UNFAIR_BARGAIN, 0, 0},
    {MAGIC, NOSPECIES, "Unfair Bargain", "Trade hands with any other player.",
     UNFAIR_BARGAIN, 0, 0},
    {MAGIC, NOSPECIES, "Two-For-One", "SACRIFICE a card, then DESTROY 2 cards.",
     TWO_FOR_ONE, 0, 0},
    {MAGIC, NOSPECIES, "Two-For-One", "SACRIFICE a card, then DESTROY 2 cards.",
     TWO_FOR_ONE, 0, 0},
    {MAGIC, NOSPECIES, "Unicorn Shrinkray",
     "Choose any player. Move all of that player's Unicorn cards to the "
     "discard pile without triggering any of their effects, then bring the "
     "same number of Baby Unicorn cards from the Nursery directly into that "
     "player's Stable.",
     UNICORN_SHRINKRAY, 0, 0},
    {MAGIC, NOSPECIES, "Targeted Destruction",
     "DESTROY an Upgrade card or SACRIFICE a Downgrade card.", TARGETED_DESTRUCTION, 0, 0},
    {MAGIC, NOSPECIES, "Mystical Vortex",
     "Each player (including you) must DISCARD a card. Shuffle the discard "
     "pile into the deck.",
     MYSTICAL_VORTEX, 0, 0},
    {MAGIC, NOSPECIES, "Good Deal", "DRAW 3 cards and DISCARD a card", GOOD_DEAL, 0, 0},
    {MAGIC, NOSPECIES, "Shake Up",
     "Shuffle this card, your hand, and the discard pile into the deck. DRAW 5 "
     "cards.",
     SHAKE_UP, 0, 0},
    {MAGIC, NOSPECIES, "Blatant Thievery",
     "Look at another player's hand. Choose a card and add it to your hand.",
     BLATANT_THIEVERY, 0, 0},
    {MAGIC, NOSPECIES, "Reset Button",
     "Each player (including you) must SACRIFICE all Upgrade and Downgrade "
     "cards in their Stable. Shuffle the discard pile into the deck.",
     RESET_BUTTON, 0, 0},
    {UPGRADE, NOSPECIES, "Rainbow Mane",
     "This card can only enter a Stable if there is a Basic Unicorn card in "
     "that Stable. If this card is in your Stable at the beginning of your "
     "turn, you may bring a Basic Unicorn card from your hand directly into "
     "your Stable.",
     RAINBOW_MANE, 0, 0},
    {UPGRADE, NOSPECIES, "Rainbow Mane",
     "This card can only enter a Stable if there is a Basic Unicorn card in "
     "that Stable. If this card is in your Stable at the beginning of your "
     "turn, you may bring a Basic Unicorn card from your hand directly into "
     "your Stable.",
     RAINBOW_MANE, 0, 0},
    {UPGRADE, NOSPECIES, "Rainbow Mane",
     "This card can only enter a Stable if there is a Basic Unicorn card in "
     "that Stable. If this card is in your Stable at the beginning of your "
     "turn, you may bring a Basic Unicorn card from your hand directly into "
     "your Stable.",
     RAINBOW_MANE, 0, 0},
    {UPGRADE, NOSPECIES, "Extra Tail",
     "This card can only enter a Stable if there is a Basic Unicorn card in "
     "that Stable. If this card is in your Stable at the beginning of your "
     "turn, you may DRAW an extra card.",
     EXTRA_TAIL, 0, 0},
    {UPGRADE, NOSPECIES, "Extra Tail",
     "This card can only enter a Stable if there is a Basic Unicorn card in "
     "that Stable. If this card is in your Stable at the beginning of your "
     "turn, you may DRAW an extra card.",
     EXTRA_TAIL, 0, 0},
    {UPGRADE, NOSPECIES, "Extra Tail",
     "This card can only enter a Stable if there is a Basic Unicorn card in "
     "that Stable. If this card is in your Stable at the beginning of your "
     "turn, you may DRAW an extra card.",
     EXTRA_TAIL, 0, 0},
    {UPGRADE, NOSPECIES, "Glitter Bomb",
     "If this card is in your Stable at the beginning of your turn, you may "
     "SACRIFICE a card, then DESTROY a card.",
     GLITTER_BOMB, 0, 0},
    {UPGRADE, NOSPECIES, "Glitter Bomb",
     "If this card is in your Stable at the beginning of your turn, you may "
     "SACRIFICE a card, then DESTROY a card.",
     GLITTER_BOMB, 0, 0},
    {UPGRADE, NOSPECIES, "Yay", "Cards you play cannot be Neigh'd.", YAY_EFFECT, 0, 0},
    {UPGRADE, NOSPECIES, "Yay", "Cards you play cannot be Neigh'd.", YAY_EFFECT, 0, 0},
    {UPGRADE, NOSPECIES, "Unicorn Lasso",
     "If this card is in your Stable at the beginning of your turn, you may "
     "STEAL a Unicorn card. At the end of your turn, return that Unicorn card "
     "to the Stable from which you stole it.",
     UNICORN_LASSO, 0, 0},
    {UPGRADE, NOSPECIES, "Rainbow Aura",
     "Your Unicorn cards cannot be destroyed.", RAINBOW_AURA_EFFECT, 0, 0},
    {UPGRADE, NOSPECIES, "Double Dutch",
     "If this card is in your Stable at the beginning of your turn, you may "
     "play 2 cards during your Action phase.",
     DOUBLE_DUTCH, 0, 0},
    {UPGRADE, NOSPECIES, "Summoning Ritual",
     "If this card is in your Stable at the beginning of your turn, you may "
     "DISCARD 2 Unicorn cards. If you do, bring a Unicorn card directly from "
     "the discard pile into your Stable.",
     SUMMONING_RITUAL, 0, 0},
    {DOWNGRADE, NOSPECIES, "Barbed Wire",
     "Each time a Unicorn card enters or leaves your Stable, DISCARD a card.",
     BARBED_WIRE_EFFECT, 0, 0},
    {DOWNGRADE, NOSPECIES, "Pandamonium",
     "All of your Unicorns are considered Pandas. Cards that affect Unicorn "
     "cards do not affect your Pandas.",
     PANDAMONIUM_EFFECT, 0, 0},
    {DOWNGRADE, NOSPECIES, "Sadistic Ritual",
     "If this card is in your Stable at the beginning of your turn, SACRIFICE "
     "a Unicorn card, then DRAW a card.",
     SADISTIC_RITUAL, 0, 0},
    {DOWNGRADE, NOSPECIES, "Slowdown", "You cannot play Instant cards.", SLOWDOWN_EFFECT, 0, 0},
    {DOWNGRADE, NOSPECIES, "Nanny Cam",
     "Your hand must be visible to all players.", NANNY_CAM_EFFECT, 0, 0},
    {DOWNGRADE, NOSPECIES, "Broken Stable", "You cannot play Upgrade cards.",
     BROKEN_STABLE_EFFECT, 0, 0},
    {DOWNGRADE, NOSPECIES, "Blinding Light",
     "All of your Unicorn cards are considered Basic Unicorns with no effects.",
     BLINDING_LIGHT_EFFECT, 0, 0},
    {DOWNGRADE, NOSPECIES, "Tiny Stable",
     "If at any time you have more than 5 Unicorns in your Stable, SACRIFICE a "
     "Unicorn card.",
     TINY_STABLE_EFFECT, 0, 0},
    {INSTANT, NOSPECIES, "Neigh",
     "Play this card when another player tries to play a card. Stop their card "
     "from being played and send it to the discard pile.",
     NEIGH, 0, 0},
    {INSTANT, NOSPECIES, "Neigh",
     "Play this card when another player tries to play a card. Stop their card "
     "from being played and send it to the discard pile.",
     NEIGH, 0, 0},
    {INSTANT, NOSPECIES, "Neigh",
     "Play this card when another player tries to play a card. Stop their card "
     "from being played and send it to the discard pile.",
     NEIGH, 0, 0},
    {INSTANT, NOSPECIES, "Neigh",
     "Play this card when another player tries to play a card. Stop their card "
     "from being played and send it to the discard pile.",
     NEIGH, 0, 0},
    {INSTANT, NOSPECIES, "Neigh",
     "Play this card when another player tries to play a card. Stop their card "
     "from being played and send it to the discard pile.",
     NEIGH, 0, 0},
    {INSTANT, NOSPECIES, "Neigh",
     "Play this card when another player tries to play a card. Stop their card "
     "from being played and send it to the discard pile.",
     NEIGH, 0, 0},
    {INSTANT, NOSPECIES, "Neigh",
     "Play this card when another player tries to play a card. Stop their card "
     "from being played and send it to the discard pile.",
     NEIGH, 0, 0},
    {INSTANT, NOSPECIES, "Neigh",
     "Play this card when another player tries to play a card. Stop their card "
     "from being played and send it to the discard pile.",
     NEIGH, 0, 0},
    {INSTANT, NOSPECIES, "Neigh",
     "Play this card when another player tries to play a card. Stop their card "
     "from being played and send it to the discard pile.",
     NEIGH, 0, 0},
    {INSTANT, NOSPECIES, "Neigh",
     "Play this card when another player tries to play a card. Stop their card "
     "from being played and send it to the discard pile.",
     NEIGH, 0, 0},
    {INSTANT, NOSPECIES, "Neigh",
     "Play this card when another player tries to play a card. Stop their card "
     "from being played and send it to the discard pile.",
     NEIGH, 0, 0},
    {INSTANT, NOSPECIES, "Neigh",
     "Play this card when another player tries to play a card. Stop their card "
     "from being played and send it to the discard pile.",
     NEIGH, 0, 0},
    {INSTANT, NOSPECIES, "Neigh",
     "Play this card when another player tries to play a card. Stop their card "
     "from being played and send it to the discard pile.",
     NEIGH, 0, 0},
    {INSTANT, NOSPECIES, "Neigh",
     "Play this card when another player tries to play a card. Stop their card "
     "from being played and send it to the discard pile.",
     NEIGH, 0, 0},
    {INSTANT, NOSPECIES, "Super Neigh",
     "Play this card when another player tries to play a card. Stop their card "
     "from being played and send it to the discard pile. This card cannot be "
     "Neigh'd.",
     SUPER_NEIGH, 0, 0}
};

int uniLassoIndex[3] = { 0 };
int puppicornIndex[2] = { -1, -1 };

// ********************************************************************************
// ***************************** Card Effect Functions ****************************
// ********************************************************************************

int Base_ToggleFlags(int pnum, int effect) {
  switch (effect) {
  case NOTHING:
    return 0;
  case BARBED_WIRE_EFFECT:
    // Playing Barbed Wire:
    // Each time a Unicorn card enters or leaves your Stable, DISCARD a card
    player[pnum].flags ^= BARBED_WIRE;
    return 1;
  case BLACK_KNIGHT_UNICORN_EFFECT:
    // Playing Black Knight Unicorn
    // if a unicorn card in your stable would be destroyed, you may SACRIFICE this card instead
    player[pnum].flags ^= BLACK_KNIGHT_UNICORN;
    return 1;
  case BLINDING_LIGHT_EFFECT:
    // Playing Blinding Light:
    // All of your unicorn cards are considered basic unicorns with no effects
    player[pnum].flags ^= BLINDING_LIGHT;
    return 1;
  case BROKEN_STABLE_EFFECT:
    // Playing Broken Stable:
    // You cannot play any upgrade cards
    player[pnum].flags ^= BROKEN_STABLE;
    return 1;
  case GINORMOUS_UNICORN_EFFECT:
    // Playing Ginormous Unicorn:
    // This card counts for 2 unicorns. you cannot play any instant cards
    player[pnum].flags ^= GINORMOUS_UNICORN;
    return 1;
  case NANNY_CAM_EFFECT:
    // Playing Nanny Cam:
    // Your hand must be visible to all players
    player[pnum].flags ^= NANNY_CAM;
    return 1;
  case PANDAMONIUM_EFFECT:
    // Playing PANDAMONIUM
    // All of your unicorns are considered pandas.
    // Cards that affect unicorn cards do not affect your pandas
    player[pnum].flags ^= PANDAMONIUM;
    return 1;
  case QUEEN_BEE_UNICORN_EFFECT:
    // Playing Queen Bee Unicorn:
    // Basic unicorn cards cannot enter any player's stable except yours
    for (int i = 0; i < currentPlayers; i++) {
      if (i != pnum) player[i].flags ^= QUEEN_BEE_UNICORN;
    }
    return 1;
  case RAINBOW_AURA_EFFECT:
    // Playing Rainbow Aura:
    // Your unicorn cards cannot be destroyed
    player[pnum].flags ^= RAINBOW_AURA;
    return 1;
  case SLOWDOWN_EFFECT:
    // Playing SLOWDOWN:
    // You cannot play any instant cards
    player[pnum].flags ^= SLOWDOWN;
    return 1;
  case TINY_STABLE_EFFECT:
    // Playing Tiny Stable:
    // If at any time you have more than 5 Unicorns in your Stable, SACRIFICE a Unicorn card
    player[pnum].flags ^= TINY_STABLE;
    return 1;
  case YAY_EFFECT:
    // Playing Yay:
    // Cards you play cannot be neigh'd
    player[pnum].flags ^= YAY;
    return 1;
  default:
    return 0;
  }

  return 0;
}

void Base_SacrificeDestroyEffects(int pnum, int cindex, int effect) {
  int isvalid = 0;
  char ans, buf[LINE_MAX];

  // Blinding Light makes it so that all unicorn cards have no effects;
  // pandamonium (and other potential future masquerade cards?) bypasses that
  if ((player[pnum].flags & BLINDING_LIGHT) == BLINDING_LIGHT &&
      (player[pnum].flags & PANDAMONIUM) == 0)
    effect = NOTHING;

  // black unicorn knight check; make sure that pandamonium is not active
  if (player[pnum].stable.unicorns[cindex].species != NOSPECIES && 
      (player[pnum].flags & BLACK_KNIGHT_UNICORN) != 0 &&
      (player[pnum].flags & BLINDING_LIGHT) == 0 &&
      (player[pnum].flags & PANDAMONIUM) == 0 &&
    strcmp(player[pnum].stable.unicorns[cindex].name, "Black Knight Unicorn") != 0) {
    // ask player pindex if they'd like to sacrifice bku instead of cindex (unicorn)
    do {
      printf(
        "Would you like to sacrifice 'Black Knight Unicorn' instead of "
        "card '%s' (y/n)?: ",
        player[pnum].stable.unicorns[cindex].name);
      ans = CharInput(buf, sizeof buf);
    } while (ans != 'y' && ans != 'n' && strlen(buf) != 2);

    if (ans == 'y') {
      for (int i = 0; i < player[pnum].stable.size; i++) {
        if (strcmp(player[pnum].stable.unicorns[i].name,
          "Black Knight Unicorn") == 0) {
          cindex = i;
          effect = NOTHING;
          break;
        }
      }
    }
  }

  switch (effect) {
  case NOTHING:
    break;
  case GREEDY_FLYING_UNICORN:
  case ANNOYING_FLYING_UNICORN:
  case MAJESTIC_FLYING_UNICORN:
  case MAGICAL_FLYING_UNICORN:
  case SWIFT_FLYING_UNICORN:
    // Playing XYZ Flying Unicorn
    // If this card is sacrificed or destroyed, return it to your hand
    player[pnum].hand.cards[player[pnum].hand.numCards++] =
      player[pnum].stable.unicorns[cindex];
    RearrangeStable(pnum, cindex);
    return;  // leave the function early
  case STABBY_THE_UNICORN:
  {
    // Playing Stabby the Unicorn
    // If this card is sacrificed or destroyed, you may DESTROY a unicorn card

    // check if there are unicorn cards to destroy
    isvalid = CheckNumCardsToDestroy(pnum, ANYUNICORN, FALSE);
    if (!isvalid)
      break;

    do {
      printf("\033[1;31mStabby the Unicorn\033[0m has been destroyed.\nWould you like to destroy a Unicorn card (y/n)?: ");
      ans = CharInput(buf, sizeof buf);
    } while ((ans != 'y' && ans != 'n') || strlen(buf) != 2);
    if (ans == 'y')
      Destroy(pnum, ANYUNICORN, FALSE);
    break;
  }
  case UNICORN_PHOENIX:
  {
    // Playing Unicorn Phoenix
    // If this card would be sacrificed or destroyed, you may DISCARD a card instead

    // check if there are any cards to actually discard
    if (player[pnum].hand.numCards <= 0)
      break;

    do {
      printf(
        "Would you like to discard a card instead of disposing "
        "the card \033[1;31m'Unicorn Phoenix'\033[0m (y/n)?: ");
      ans = CharInput(buf, sizeof buf);
    } while ((ans != 'y' && ans != 'n') || strlen(buf) != 2);

    if (ans == 'y') {
      Discard(pnum, 1, ANY);
      return;
    }
    break;
  }
  default:
    break;
  }

  if (player[pnum].stable.unicorns[cindex].cType == BABYUNICORN)
    AddNursery(player[pnum].stable.unicorns[cindex]);
  else
    AddDiscard(player[pnum].stable.unicorns[cindex]);

  RearrangeStable(pnum, cindex);
}

int Base_ConditionalEffects(int pnum, struct Unicorn corn, int hindex, int upgrade_target) {
  int isvalid = 0, isvalid2 = 0;

  switch (corn.effect) {
  case NOTHING:
    break;
  case BACK_KICK:
  {
    // Playing Back Kick:
    // Return a card in another player's stable to their hand. That player must DISCARD a card

    for (int i = 0; i < currentPlayers; i++) {
      if (player[i].stable.size > 0) {
        isvalid = 1;
        break;
      }
    }

    if (isvalid == 0) {
      printf(
        "You are unable to play 'Back Kick' because there are no cards "
        "in other player's stables\n");
      turnCount++;
      return 0;
    }

    return 1;
  }
  case BLATANT_THIEVERY:
  {
    // Playing Blatant Thievery:
    // Look at another player's hand. Choose a card and add it to your hand

    for (int i = 0; i < currentPlayers; i++) {
      if (i != pnum && player[i].hand.numCards > 0) {
        isvalid = 1;
        i = currentPlayers;  // quit the for loop
      }
    }

    if (isvalid == 0) {
      printf(
        "You are unable to play 'Blatant Thievery' because no other "
        "players have any cards in their hands\n");
      turnCount++;
      return 0;
    }

    return 1;
  }
  case CHANGE_OF_LUCK:
  {
    // Playing Change of Luck:
    // DRAW 2 cards and DISCARD 3 cards, then take another turn

    // need to discard 3 cards
    // 3 cards - 2 drawn + 1 (change of luck) = 2 cards in hand needed, including this card
    if (player[pnum].hand.numCards < 2) {
      printf(
        "You are unable to play 'Change of Luck' because you would not "
        "have 3 cards in total to Discard\n");
      turnCount++;
      return 0;
    }

    return 1;
  }
  case EXTRA_TAIL:
  {
    // Playing Extra Tail:
    // Can only enter a stable if there is a Basic Unicorn card there
    for (int i = 0; i < player[upgrade_target].stable.size; i++) {
      if (player[upgrade_target].stable.unicorns[i].cType == BASICUNICORN) {
        return 1;
      }
    }

    printf(
      "You are unable to play 'Extra Tail' because there aren't any "
      "Basic Unicorn cards in the chosen player's stable\n");
    turnCount++;
    return 0;
  }
  case GLITTER_TORNADO:
  {
    // Playing Glitter Tornado
    // Return a card in each player's Stable (including yours) to their hand

    // check if every player has a card to return
    for (int i = 0; i < currentPlayers; i++) {
      if (player[i].stable.size <= 0) {
        isvalid = 1;
        break;
      }
    }

    if (isvalid) {
      printf(
        "You are unable to play 'Glitter Tornado' because all players do "
        "not have at least 1 card in their stables\n");
      turnCount++;
      return 0;
    }

    return 1;
  }
  case MYSTICAL_VORTEX:
  {
    // Playing Mystical Vortex:
    // Every player must DISCARD 1 card. Shuffle the discard pile into the deck

    // check if the current player has any cards to dispose of
    if (player[pnum].hand.numCards < 2) {
      printf(
        "You are unable to play 'Mystical Vortex' because you don't have any "
        "cards to Discard\n");
      turnCount++;
      return 0;
    }

    return 1;
  }
  case RAINBOW_MANE:
  {
    // Playing Rainbow Mane:
    // Can only enter a stable if there is a Basic Unicorn card there
    for (int i = 0; i < player[upgrade_target].stable.size; i++) {
      if (player[upgrade_target].stable.unicorns[i].cType == BASICUNICORN) {
        return 1;
      }
    }

    printf(
      "You are unable to play 'Rainbow Mane' because there aren't any "
      "Basic Unicorn cards in the chosen player's stable\n");
    turnCount++;
    return 0;
  }
  case RE_TARGET:
  {
    // Playing Re-Target:
    // Move an Upgrade or Downgrade card from any player's stable to any other player's stable

    for (int j = 0; j < currentPlayers; j++) {
      for (int i = 0; i < player[j].stable.size; i++) {
        if (player[j].stable.unicorns[i].cType == UPGRADE ||
          player[j].stable.unicorns[i].cType == DOWNGRADE) {
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
      turnCount++;
      return 0;
    }

    return 1;
  }
  case RESET_BUTTON:
  {
    // Playing Reset Button:
    // Each player must SACRIFICE all upgrade/downgrade cards. Shuffle discard pile into the deck
    int isdowngrade = 0;
    int isupgrade = 0;

    // checks if the current player has any upgrade/downgrade cards to sacrifice,
    // then checks if other people have cards to sacrifice

    for (int j = 0; j < currentPlayers; j++) {
      // current player (pnum)
      if (j == pnum) {
        for (int i = 0; i < player[j].stable.size; i++) {
          if (player[j].stable.unicorns[i].cType == DOWNGRADE) {
            isdowngrade = 1;
          }
          else if (player[j].stable.unicorns[i].cType == UPGRADE) {
            isupgrade = 1;
          }
        }
      }
      // everyone else
      else {
        for (int i = 0; i < player[j].stable.size; i++) {
          if (player[j].stable.unicorns[i].cType == DOWNGRADE ||
            player[j].stable.unicorns[i].cType == UPGRADE) {
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
      turnCount++;
      return 0;
    }

    return 1;
  }
  case TARGETED_DESTRUCTION:
  {
    // Playing Targeted Destruction:
    // You must DESTROY an Upgrade card or SACRIFICE a Downgrade card

    for (int j = 0; j < currentPlayers; j++) {
      for (int i = 0; i < player[j].stable.size; i++) {
        if (j != pnum && player[j].stable.unicorns[i].cType == UPGRADE)
          isvalid = 1;
        if (j == pnum &&
          player[j].stable.unicorns[i].cType == DOWNGRADE)
          isvalid2 = 1;
      }
      // quit if both conditions have been met
      if (isvalid == 1 && isvalid2 == 1) j = currentPlayers;
    }
    // quit if there isn't an upgrade card or downgrade card available
    if (isvalid == 0 && isvalid2 == 0) {
      printf(
        "You are unable to play 'Targeted Destruction' because there are "
        "no Upgrade or Downgrade cards in place\n");
      turnCount++;
      return 0;
    }

    return 1;
  }
  case TWO_FOR_ONE:
  {
    // Playing Two-For-One:
    // SACRIFICE 1 card, then DESTROY 2 cards

    // check if there is at least 1 card to sacrifice; some cards *cough puppicorn cough*
    // are impervious to getting sacrificed
    for (int i = 0; i < player[pnum].stable.size; i++) {
      if (CanBeSacrificed(pnum, i, ANY)) {
        isvalid = 1;
        break;
      }
    }

    // check if there are at least 2 cards to destroy
    isvalid2 = CheckNumCardsToDestroy(pnum, ANY, TRUE);

    // quit and reassign the card to the player's hand
    if (!isvalid || isvalid2 < 2) {
      printf(
        "You are unable to play 'Two-For-One' because there are "
        "not enough cards to Sacrifice and/or Destroy\n");
      turnCount++;
      return 0;
    }

    return 1;
  }
  case UNFAIR_BARGAIN:
  {
    // Playing Unfair Bargain:
    // Trade hands with any other player

    for (int i = 0; i < currentPlayers; i++) {
      if (i != pnum && player[i].hand.numCards > 0) {
        isvalid = 1;
        break;
      }
    }

    if (isvalid == 0) {
      printf(
        "You are unable to play 'Unfair Bargain' because no other "
        "players have any cards in their hands\n");
      turnCount++;
      return 0;
    }

    return 1;
  }
  case UNICORN_POISON:
  {
    // Playing Unicorn Poison:
    // DESTROY 1 unicorn card

    // check if there's at least 1 unicorn card to destroy first
    isvalid = CheckNumCardsToDestroy(pnum, ANYUNICORN, TRUE);

    // quit and reassign the card to the player's hand
    if (!isvalid) {
      printf(
        "You are unable to play 'Unicorn Poison' because there are "
        "no available cards to Destroy\n");
      turnCount++;
      return 0;
    }

    return 1;
  }
  case UNICORN_SHRINKRAY:
  {
    // Playing Unicorn Shrinkray:
    // Move all player's unicorn cards to the discard pile without triggering any of their effects,
    // then bring the same number of Baby Unicorns from the nursery directly into that player's stable

    for (int i = 0; i < currentPlayers; i++) {
      if (i == pnum) continue;
      if ((player[i].flags & PANDAMONIUM) != 0) {
        // pandas aren't unicorns
        isvalid2++;
        continue;
      }

      for (int j = 0; j < player[i].stable.size; j++) {
        if (player[i].stable.unicorns[j].cType == BASICUNICORN ||
            player[i].stable.unicorns[j].cType == MAGICALUNICORN) {
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
    if (isvalid2 == currentPlayers - 1) {
      printf(
        "You are unable to play 'Unicorn Shrinkray' because there aren't "
        "enough available Baby Unicorns to replace any player's stable\n");
      turnCount++;
      return 0;
    }

    return 1;
  }
  case UNICORN_SWAP:
  {
    // Playing Unicorn Swap:
    // Move a Unicorn card from your stable to any other player's,
    // then STEAL a Unicorn card from that player's stable

    // you can steal the card back, so the size of any other player's stable doesn't matter
    // so long as the current player has 1 unicorn card and there are no masquerade effects in play
    for (int i = 0; i < currentPlayers; i++) {
      if (i == pnum) continue;

      if ((player[i].flags & PANDAMONIUM) == 0) {
        isvalid = 1;
        break;
      }
    }

    if (player[pnum].stable.numUnicorns <= 0 || (player[pnum].flags & PANDAMONIUM) == PANDAMONIUM ||
        !isvalid) {
      printf(
        "You are unable to play 'Unicorn Swap' because there are not enough Unicorn cards to swap.\n");
      turnCount++;
      return 0;
    }

    return 1;
  }
  default:
    break;
  }

  // queen bee unicorn flag check
  if (corn.cType == BASICUNICORN &&
    (player[pnum].flags & QUEEN_BEE_UNICORN) != 0) {
    printf("%s is unable to play Basic Unicorns due to someone else's "
      "Queen Bee Unicorn\n", player[pnum].username);
    turnCount++;
    return 0;
  }
  // broken stable flag check
  else if (corn.cType == UPGRADE &&
    (player[pnum].flags & BROKEN_STABLE) != 0) {
    printf(
      "%s is unable to play Upgrade cards due to the card "
      "Broken Stable's effect\n", player[pnum].username);
    turnCount++;
    return 0;
  }

  return 2;
}

void Base_EnterStableEffects(int pnum, int effect) {
  int index, index2;
  char ans, * end, buf[LINE_MAX];
  int isvalid = 0, isvalid2 = 0;

  // Blinding Light makes it so that all unicorn cards have no effects
  if ((player[pnum].flags & BLINDING_LIGHT) != 0)
    return;

  switch (effect) {
  case NOTHING:
    break;
  case ALLURING_NARWHAL:
  {
    // Playing Alluring Narwhal:
    // STEAL 1 upgrade card (e.y.s.)
    for (int i = 0; i < currentPlayers; i++) {
      for (int j = 0; j < player[i].stable.size; j++) {
        if (player[i].stable.unicorns[j].cType == UPGRADE && i != pnum) {
          isvalid++;
          i = DESC_SIZE;
          break;
        }
      }
    }

    if (!isvalid)
      return;

    Steal(pnum, UPGRADE);
    break;
  }
  case AMERICORN:
  {
    // Playing Americorn:
    // You may pull a card at random from another player's hand into your hand

    // check if other players actually have at least one card
    for (int i = 0; i < currentPlayers; i++) {
      if (i != pnum && player[i].hand.numCards > 0) {
        isvalid = 1;
        break;
      }
    }

    if (!isvalid)
      break;

    PrintPlayers();
    for (;;) {
      printf("Choose a player to steal a random card from their hand: ");
      index = NumInput(buf, &end, sizeof buf) - 1;

      // index validation
      if (index < 0 || index >= currentPlayers || index == pnum || end != (buf + strlen(buf)))
        continue;

      if (player[index].hand.numCards > 0)
        break;
    }

    if ((player[index].flags & NANNY_CAM) == NANNY_CAM) {
      // pick the card if the chosen player has the nanny cam downgrade card
      PrintHand(index);
      for (;;) {
        printf("Player #%d: %s has Nanny Cam in effect, so you may choose a card "
          "to pull from their hand: ", index + 1, player[index].username);
        index2 = NumInput(buf, &end, sizeof buf) - 1;

        // index validation
        if (index2 >= 0 && index2 < player[index].hand.numCards && end == (buf + strlen(buf)))
          break;
      }
    }
    else {
      // set index2 to be some random card within the chosen player's range
      index2 = rand() % player[index].hand.numCards;
    }

    // discard time! pnum is current player, index is the stolen player
    // number and index2 is the card item being transferred
    player[pnum].hand.cards[player[pnum].hand.numCards] =
      player[index].hand.cards[index2];
    player[pnum].hand.numCards++;
    RearrangeHand(index, index2);
    break;
  }
  case ANNOYING_FLYING_UNICORN:
  {
    // Playing Annoying Flying Unicorn:
    // You may force another player to DISCARD 1 card

    // check if other players have at least 1 card
    for (int i = 0; i < currentPlayers; i++) {
      if (player[i].hand.numCards > 0 && i != pnum) {
        isvalid = 1;
        break;
      }
    }

    if (!isvalid)
      break;

    PrintPlayers();
    for (;;) {
      printf("Choose a player other than yourself to discard a card: ");
      index = NumInput(buf, &end, sizeof buf) - 1;

      // index validation
      if (index < 0 || index >= currentPlayers || index == pnum || end != (buf + strlen(buf)))
        continue;

      if (player[index].hand.numCards > 0)
        break;
    }

    // make the chosen player discard a card
    if (isClient) {
      SendInt(DISCARD_EVENT, sockfd);
      // index = player target; discard ANY card class
      SendCardEffectPacket(index, ANY, sockfd);
      ClientDiscard(pnum, index, ANY);
    }
    else {
      ServerDiscard(pnum, index, ANY);
    }
    break;
  }
  case CHAINSAW_UNICORN:
  {
    // Playing Chainsaw Unicorn:
    // You may DESTROY an upgrade card or SACRIFICE a downgrade card
    for (int j = 0; j < currentPlayers; j++) {
      for (int i = 0; i < player[j].stable.size; i++) {
        if (j != pnum && player[j].stable.unicorns[i].cType == UPGRADE)
          isvalid = 1;
        if (j == pnum && player[j].stable.unicorns[i].cType == DOWNGRADE)
          isvalid2 = 1;
      }
      // quit if both conditions have been met
      if (isvalid == 1 && isvalid2 == 1) j = currentPlayers;
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
      ans = CharInput(buf, sizeof buf);
    } while ((ans != '1' && ans != '2' && ans != '3') || strlen(buf) != 2 ||
      (ans == '1' && isvalid != 1) || (ans == '2' && isvalid2 != 1));

    if (ans == '1') {
      // destroy an upgrade card
      Destroy(pnum, UPGRADE, FALSE);
    }
    else if (ans == '2') {
      // sacrifice a downgrade card
      Sacrifice(pnum, DOWNGRADE);
    }
    break;
  }
  case CLASSY_NARWHAL:
  {
    // Playing Classy Narwhal
    // Search the deck for an upgrade card and add it to your hand. Shuffle the deck
    SearchPile(pnum, &deck, UPGRADE, ANY);
    ShuffleDeck(&deck);
    break;
  }
  case EXTREMELY_DESTRUCTIVE_UNICORN:
  {
    // Playing Extremely Destructive Unicorn:
    // Each player must SACRIFICE 1 unicorn card

    if (isClient) {
      SendInt(SACRIFICE_EVENT, sockfd);
      // (targetPnum = ANY) represents all players
      SendCardEffectPacket(ANY, ANYUNICORN, sockfd);
      ClientSacrifice(pnum, ANY, ANYUNICORN);
    }
    else {
      ServerSacrifice(pnum, ANY, ANYUNICORN);
    }

    break;
  }
  case THE_GREAT_NARWHAL:
  {
    // Playing The Great Narwhal
    // Search the deck for a card with "Narwhal" in its name and add it to your hand. Shuffle the deck
    SearchPile(pnum, &deck, ANY, NARWHAL);
    ShuffleDeck(&deck);
    break;
  }
  case GREEDY_FLYING_UNICORN:
  {
    // Playing Greedy Flying Unicorn:
    // DRAW 1 card
    Draw(pnum, 1);
    break;
  }
  case LLAMACORN:
  {
    // Playing Llamacorn:
    // Each player must DISCARD 1 card.

    if (isClient) {
      SendInt(DISCARD_EVENT, sockfd);
      // (targetPnum = ANY) represents all players
      SendCardEffectPacket(ANY, ANY, sockfd);
      ClientDiscard(pnum, ANY, ANY);
    }
    else {
      ServerDiscard(pnum, ANY, ANY);
    }

    break;
  }
  case MAGICAL_FLYING_UNICORN:
  {
    // Playing Magical Flying Unicorn:
    // You may add a Magic card from the discard pile to your hand
    SearchPile(pnum, &discardpile, MAGIC, ANY);
    break;
  }
  case MAJESTIC_FLYING_UNICORN:
  {
    // Playing Majestic Flying Unicorn:
    // You may add a Unicorn card from the discard pile to your hand
    SearchPile(pnum, &discardpile, ANYUNICORN, ANY);
    break;
  }
  case MERMAID_UNICORN:
  {
    // Playing Mermaid Unicorn:
    // Return a card in another player's stable to their hand
    for (int i = 0; i < currentPlayers; i++) {
      if (i != pnum && player[i].stable.size > 0) {
        isvalid = 1;
        break;
      }
    }

    if (!isvalid)
      break;

    PrintPlayers();
    for (;;) {
      printf(
        "Choose a player to return a card from their stable to their "
        "hand: ");
      index = NumInput(buf, &end, sizeof buf) - 1;

      // index validation
      if (index < 0 || index >= currentPlayers || index == pnum || end != (buf + strlen(buf)))
        continue;

      if (player[index].stable.size > 0)
        break;
    }

    PrintStable(index);
    do {
      printf("Choose a valid card number to return: ");
      index2 = NumInput(buf, &end, sizeof buf) - 1;
    } while (index2 < 0 || index2 >= player[index].stable.size || end != (buf + strlen(buf)));

    ReturnCardToHand(index, index2);
    break;
  }
  case NARWHAL_TORPEDO:
  {
    // Playing Narwhal Torpedo
    // SACRIFICE all Downgrade cards
    for (int i = player[pnum].stable.size - 1; i >= 0; i--) {
      // start from the end so that cards/indices don't get skipped upon
      // rearranging the stable
      if (player[pnum].stable.unicorns[i].cType == DOWNGRADE) {
        AddDiscard(player[pnum].stable.unicorns[i]);
        RearrangeStable(pnum, i);
      }
    }
    break;
  }
  case RAINBOW_UNICORN:
  {
    // Playing Rainbow Unicorn
    // Bring a basic unicorn card from your hand directly into your stable

    // someone else has a queen bee unicorn, so the player is unable to play
    // basic unicorns
    if ((player[pnum].flags & QUEEN_BEE_UNICORN) != 0)
      break;

    for (int i = 0; i < player[pnum].hand.numCards; i++) {
      // use the player's card ID (taken from deck_ref) to access the main
      // deck to check the original cards cType
      if (player[pnum].hand.cards[i].cType == BASICUNICORN) {
        isvalid = 1;
        break;
      }
    }

    // don't have to check before because this is an optional effect
    if (isvalid == 0) break;

    PrintHand(pnum);
    for (;;) {
      printf("Choose a Basic Unicorn to bring into your stable: ");
      index = NumInput(buf, &end, sizeof buf) - 1;

      // index validation
      if (index < 0 || index >= player[pnum].hand.numCards || end != (buf + strlen(buf)))
        continue;

      // make sure it's actually a basic unicorn
      if (player[pnum].hand.cards[index].cType == BASICUNICORN)
        break;
    }

    AddStable(pnum, player[pnum].hand.cards[index]);
    RearrangeHand(pnum, index);
    break;
  }
  case SEDUCTIVE_UNICORN:
  {
    // Playing Seductive Unicorn:
    // You may DISCARD 1 card, then STEAL a unicorn card

    // check if there are unicorn cards to steal
    for (int i = 0; i < currentPlayers; i++) {
      if (i != pnum && player[i].stable.numUnicorns > 0) {
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
      ans = CharInput(buf, sizeof buf);
    } while ((ans != 'y' && ans != 'n') || strlen(buf) != 2);

    if (ans == 'y') {
      Discard(pnum, 1, ANY);
      Steal(pnum, ANYUNICORN);
    }
    break;
  }
  case SHABBY_THE_NARWHAL:
  {
    // Playing Shabby the Narwhal
    // Search the deck for a downgrade card and add it to your hand. Shuffle the deck
    SearchPile(pnum, &deck, DOWNGRADE, ANY);
    ShuffleDeck(&deck);
    break;
  }
  case SHARK_WITH_A_HORN:
  {
    // Playing Shark With a Horn
    // You may SACRIFICE this card, then destroy a Unicorn card

    // check if there are unicorn cards to destroy
    isvalid = CheckNumCardsToDestroy(pnum, ANYUNICORN, FALSE);
    if (!isvalid)
      break;

    do {
      printf(
        "Would you like to sacrifice this card in order to destroy a "
        "Unicorn card (y/n)?: ");
      ans = CharInput(buf, sizeof buf);
    } while ((ans != 'y' && ans != 'n') || strlen(buf) != 2);

    if (ans == 'y') {
      AddDiscard(player[pnum].stable.unicorns[player[pnum].stable.size - 1]);
      // player[pnum].stable.size--; <--- this is why we can't have nice things...
      RearrangeStable(pnum, player[pnum].stable.size - 1);
      Destroy(pnum, ANYUNICORN, FALSE);
    }
    break;
  }
  case SWIFT_FLYING_UNICORN:
  {
    // Playing Swift Flying Unicorn:
    // You may add an Instant card from the discard pile to your hand
    SearchPile(pnum, &discardpile, INSTANT, ANY);
    break;
  }
  case UNICORN_ON_THE_COB:
  {
    // Playing Unicorn on the Cob:
    // DRAW 2 cards and DISCARD 1 card
    Draw(pnum, 2);
    Discard(pnum, 1, ANY);
    break;
  }
  default:
    break;
  }
}

void Base_MagicEffects(int pnum, int effect) {
  int index, index2, index3;
  int isvalid = 0, isvalid2 = 0;
  char ans, *end, buf[LINE_MAX];

  // could probably add the conditional effect here just to prevent infinite loops
  // on the rare case that this gets called outside of PlayCard and various tests,
  // but that doesn't seem worth it...

  switch (effect) {
  case NOTHING:
    break;
  case BACK_KICK:
  {
    // Playing Back Kick:
    // Return a card in another player's stable to their hand. That player must DISCARD a card

    PrintPlayers();
    for (;;) {
      printf(
        "Choose a player to return a card from their stable to their "
        "hand: ");
      index = NumInput(buf, &end, sizeof buf) - 1;

      // index validation
      if (index < 0 || index >= currentPlayers || index == pnum || end != (buf + strlen(buf)))
        continue;

      // passed the stable size check
      if (player[index].stable.size > 0)
        break;
    }

    PrintStable(index);
    do {
      printf("Choose a valid card number to return: ");
      index2 = NumInput(buf, &end, sizeof buf) - 1;
    } while (index2 < 0 || index2 >= player[index].stable.size || end != (buf + strlen(buf)));

    ReturnCardToHand(index, index2);

    // make the chosen player discard a card
    if (isClient) {
      SendInt(DISCARD_EVENT, sockfd);
      // index = player target; discard ANY card class
      SendCardEffectPacket(index, ANY, sockfd);
      ClientDiscard(pnum, index, ANY);
    }
    else {
      ServerDiscard(pnum, index, ANY);
    }
    break;
  }
  case BLATANT_THIEVERY:
  {
    // Playing Blatant Thievery:
    // Look at another player's hand. Choose a card and add it to your hand

    PrintPlayers();
    for (;;) {
      printf("Choose a player to steal a random card from their hand: ");
      index = NumInput(buf, &end, sizeof buf) - 1;

      // index validation
      if (index < 0 || index >= currentPlayers || index == pnum || end != (buf + strlen(buf)))
        continue;

      if (player[index].hand.numCards > 0)
        break;
    }

    PrintHand(index);
    do {
      printf("Choose a card to steal: ");
      index2 = NumInput(buf, &end, sizeof buf) - 1;
    } while (index2 < 0 || index2 >= player[index].hand.numCards || end != (buf + strlen(buf)));

    // readjust hands and num_card lengths
    player[pnum].hand.cards[player[pnum].hand.numCards++] =
      player[index].hand.cards[index2];
    RearrangeHand(index, index2);
    break;
  }
  case CHANGE_OF_LUCK:
  {
    // Playing Change of Luck:
    // DRAW 2 cards and DISCARD 3 cards, then take another turn

    Draw(pnum, 2);
    Discard(pnum, 3, ANY);

    // the whole turn is reset, so the player gets to do their
    // end of turn phase and restart a new beginning of turn phase
    EndOfTurn(pnum);
    BeginningOfTurn(pnum);
    turnCount++; // turnCount++ is for restarting the action phase too
    break;
  }
  case GLITTER_TORNADO:
  {
    // Playing Glitter Tornado
    // Return a card in each player's Stable (including yours) to their hand.

    // important to note that the current player (pnum) is the one to choose
    // every card that gets returned
    for (int i = 0; i < currentPlayers; i++) {
      PrintStable(i);
      do {
        printf("Choose a valid card number to return: ");
        index = NumInput(buf, &end, sizeof buf) - 1;
      } while (index < 0 || index >= player[i].stable.size || end != (buf + strlen(buf)));

      ReturnCardToHand(i, index);
    }
    break;
  }
  case GOOD_DEAL:
  {
    // Playing Good Deal:
    // DRAW 3 cards and DISCARD 1 card
    Draw(pnum, 3);
    Discard(pnum, 1, ANY);
    break;
  }
  case MYSTICAL_VORTEX:
  {
    // Playing Mystical Vortex:
    // Every player must DISCARD 1 card. Shuffle the discard pile into the deck

    if (isClient) {
      SendInt(DISCARD_EVENT, sockfd);
      // (targetPnum = ANY) represents all players
      SendCardEffectPacket(ANY, ANY, sockfd);
      ClientDiscard(pnum, ANY, ANY);
    }
    else {
      ServerDiscard(pnum, ANY, ANY);
    }
    ShuffleDiscard();
    break;
  }
  case RE_TARGET:
  {
    // Playing Re-Target:
    // Move an upgrade or downgrade card from any player's stable to any other player's stable

    PrintPlayers();
    do {
      printf("Choose a player to take an upgrade or downgrade card from: ");
      index = NumInput(buf, &end, sizeof buf) - 1;

      // index validation
      if (index < 0 || index >= currentPlayers || end != (buf + strlen(buf)))
        continue;

      // make sure chosen player has an upgrade or downgrade card
      for (int i = 0; i < player[index].stable.size; i++) {
        if (player[index].stable.unicorns[i].cType == UPGRADE ||
            player[index].stable.unicorns[i].cType == DOWNGRADE) {
          // quit if either condition was met
          isvalid = 1;
          break;
        }
      }
    } while (!isvalid);

    PrintStable(index);
    for (;;) {
      printf("Choose a valid card number to take: ");
      index2 = NumInput(buf, &end, sizeof buf) - 1;

      // index validation
      if (index2 < 0 || index2 >= player[index].stable.size || end != (buf + strlen(buf)))
        continue;

      // check if the card type aligns
      if (player[index].stable.unicorns[index2].cType == UPGRADE ||
          player[index].stable.unicorns[index2].cType == DOWNGRADE)
        break;
    }

    PrintPlayers();
    do {
      printf("Choose a player to give the taken upgrade or downgrade card: ");
      index3 = NumInput(buf, &end, sizeof buf) - 1;
      // can't be the same as the player it was taken from (i.e. player # index)
    } while (index3 < 0 || index3 >= currentPlayers || index3 == index || end != (buf + strlen(buf)));

    // add the card to the new stable, alongside any potential toggle switch
    struct Unicorn corn = player[index].stable.unicorns[index2];

    RearrangeStable(index, index2);
    AddStable(index3, corn);
    break;
  }
  case RESET_BUTTON:
  {
    // Playing Reset Button:
    // Each player must SACRIFICE all Upgrade/Downgrade cards. Shuffle discard pile into the deck

    for (int j = 0; j < currentPlayers; j++) {
      // start from the end so that cards/indices don't get skipped upon
      // rearranging the stable
      for (int i = player[j].stable.size - 1; i >= 0; i--) {
        if (player[j].stable.unicorns[i].cType == DOWNGRADE ||
          player[j].stable.unicorns[i].cType == UPGRADE) {
          AddDiscard(player[j].stable.unicorns[i]);
          RearrangeStable(j, i);
        }
      }
    }
    ShuffleDiscard();
    break;
  }
  case SHAKE_UP:
  {
    // Playing Shake Up:
    // Shuffle your card, hand, and discard pile into the deck. DRAW 5 cards
    for (int i = player[pnum].hand.numCards - 1; i >= 0; i--) {
      // move all cards from hand to the discard pile
      AddDiscard(player[pnum].hand.cards[i]);
      player[pnum].hand.numCards--;
    }
    ShuffleDiscard();
    Draw(pnum, 5);
    break;
  }
  case TARGETED_DESTRUCTION:
  {
    // Playing Targeted Destruction:
    // You must DESTROY an Upgrade card or SACRIFICE a Downgrade card

    // this one has to check for upgrade/downgrade cards AGAIN because
    // the effect is situational
    for (int j = 0; j < currentPlayers; j++) {
      for (int i = 0; i < player[j].stable.size; i++) {
        if (j != pnum && player[j].stable.unicorns[i].cType == UPGRADE)
          isvalid = 1;
        if (j == pnum &&
          player[j].stable.unicorns[i].cType == DOWNGRADE)
          isvalid2 = 1;
      }
      // quit if both conditions have been met
      if (isvalid == 1 && isvalid2 == 1) j = currentPlayers;
    }

    do {
      printf(
        "Pick between the following options:"
        "\n  1. Destroy an upgrade card"
        "\n  2. Sacrifice a downgrade card"
        "\nChoice: ");
      ans = CharInput(buf, sizeof buf);
    } while ((ans != '1' && ans != '2') || (ans == '1' && isvalid != 1) ||
      (ans == '2' && isvalid2 != 1) || strlen(buf) != 2);

    if (ans == '1') {
      // destroy an upgrade card (this doesn't need to be in the special function)
      Destroy(pnum, UPGRADE, TRUE);
    }
    else if (ans == '2') {
      // sacrifice a downgrade card
      Sacrifice(pnum, DOWNGRADE);
    }
    break;
  }
  case TWO_FOR_ONE:
  {
    // Playing Two-For-One:
    // SACRIFICE 1 card, then DESTROY 2 cards

    Sacrifice(pnum, ANY);
    Destroy(pnum, ANY, TRUE);
    Destroy(pnum, ANY, TRUE);
    break;
  }
  case UNFAIR_BARGAIN:
  {
    // Playing Unfair Bargain:
    // Trade hands with any other player

    struct Unicorn tmp_hand[HAND_SIZE];
    int tmp_size;

    PrintPlayers();
    for (;;) {
      printf("Choose a player to trade hands with: ");
      index = NumInput(buf, &end, sizeof buf) - 1;

      // index validation
      if (index < 0 || index >= currentPlayers || index == pnum || end != (buf + strlen(buf)))
        continue;

      if (player[index].hand.numCards > 0)
        break;
    }

    for (int i = 0;
      i < (player[index].hand.numCards *
        (player[index].hand.numCards > player[pnum].hand.numCards)) +
      (player[pnum].hand.numCards *
        (player[pnum].hand.numCards > player[index].hand.numCards));
      i++) {
      // swap each element until i hits the max number between the highest
      // number of cards in each player's hand
      tmp_hand[i] = player[index].hand.cards[i];
      player[index].hand.cards[i] = player[pnum].hand.cards[i];
      player[pnum].hand.cards[i] = tmp_hand[i];
    }

    // swap numCards info too
    tmp_size = player[index].hand.numCards;
    player[index].hand.numCards = player[pnum].hand.numCards;
    player[pnum].hand.numCards = tmp_size;
    break;
  }
  case UNICORN_POISON:
  {
    // Playing Unicorn Poison:
    // DESTROY 1 Unicorn card

    Destroy(pnum, ANYUNICORN, TRUE);
    break;
  }
  case UNICORN_SHRINKRAY:
  {
    // Playing Unicorn Shrinkray:
    // Move all player's unicorn cards to the discard pile without triggering any of their effects,
    // then bring the same number of Baby Unicorns from the nursery directly into that player's stable

    // this one has to check for Unicorn cards AGAIN because
    // the effect is situational
    for (int i = 0; i < currentPlayers; i++) {
      if (i == pnum) continue;
      if ((player[i].flags & PANDAMONIUM) != 0) {
        // pandas aren't unicorns
        isvalid2++;
        continue;
      }

      for (int j = 0; j < player[i].stable.size; j++) {
        if (player[i].stable.unicorns[j].cType == BASICUNICORN ||
          player[i].stable.unicorns[j].cType == MAGICALUNICORN) {
          isvalid++;
        }
      }
      // adds to a counter check if nursery size is less than the amount of non-baby unicorns
      if (nursery.size < isvalid) {
        isvalid2++;
      }
      isvalid = 0;
    }

    // card effect
    PrintPlayers();
    for (;;) {
      printf("Choose a player to swap their stable with Baby Unicorns: ");
      index = NumInput(buf, &end, sizeof buf) - 1;

      // index validation
      if (index < 0 || index >= currentPlayers || index == pnum || end != (buf + strlen(buf)))
        continue;

      for (int i = 0; i < player[index].stable.size; i++) {
        if (player[index].stable.unicorns[i].cType == BASICUNICORN ||
          player[index].stable.unicorns[i].cType == MAGICALUNICORN) {
          isvalid++;
        }
      }
      if (isvalid > 0 && isvalid <= nursery.size)
        break;

      isvalid = 0;
    }

    for (int i = 0; i < player[index].stable.size; i++) {
      if ((player[index].stable.unicorns[i].cType == BASICUNICORN ||
        player[index].stable.unicorns[i].cType == MAGICALUNICORN)) {
        // get rid of the puppicorn index if applicable
        if (player[index].stable.unicorns[i].effect == PUPPICORN) {
          puppicornIndex[0] = -1;
        }

        // some unicorns may have flag effects that need to be removed
        Base_ToggleFlags(index, player[index].stable.unicorns[i].effect);
        AddDiscard(player[index].stable.unicorns[i]);

        // no point in using AddStable here when the cards are just getting replaced;
        // that means less function calls to Discard() too
        player[index].stable.unicorns[i] = nursery.cards[nursery.size - 1];
        nursery.size--;
      }
    }

    // barbed wire check; that's a lot of unicorns to discard!
    // should this stay? this seems evil LOL
    if ((player[index].flags & BARBED_WIRE) != 0)
      Discard(index, player[index].stable.numUnicorns * 2, ANY);

    break;
  }
  case UNICORN_SWAP:
  {
    // Playing Unicorn Swap:
    // Move a Unicorn card from your stable to any other player's,
    // then STEAL a Unicorn card from that player's stable

    PrintPlayers();
    for (;;) {
      printf("Choose a player to swap unicorn cards with: ");
      index = NumInput(buf, &end, sizeof buf) - 1;

      // index validation
      if (index < 0 || index >= currentPlayers || index == pnum || end != (buf + strlen(buf)))
        continue;

      // check if chosen player has "pandas" instead of unicorns
      if ((player[index].flags & PANDAMONIUM) == 0)
        break;
    }

    PrintStable(pnum);
    for (;;) {
      printf("Choose a valid unicorn card number to move to %s's stable: ",
        player[index].username);
      index2 = NumInput(buf, &end, sizeof buf) - 1;

      // index validation
      if (index2 < 0 || index2 >= player[pnum].stable.size || end != (buf + strlen(buf)))
        continue;

      // make sure it's actually a unicorn card
      if (player[pnum].stable.unicorns[index2].species != NOSPECIES)
        break;
    }

    // rearrange the player's stable before swapping the card to the other stable;
    // this will (hopefully) prevent dupes from card effect shenanigans
    struct Unicorn tmp = player[pnum].stable.unicorns[index2];
    RearrangeStable(pnum, index2);

    // send a notice to add a card to the person's stable outside of their normal turn
    if (isClient) {
      SendInt(ENTER_STABLE_EVENT, sockfd);
      SendEnterStablePacket(tmp, index, sockfd); // index = target player index

      // need to look out for nested network events
      ClientEnterLeaveStable(pnum);
    }
    else {
      SendInt(ENTER_STABLE_EVENT, clientsockfd[index - 1]);
      SendEnterStablePacket(tmp, pnum, clientsockfd[index - 1]); // pnum = original player index

      // need to look out for nested network events
      ServerEnterLeaveStable(pnum, index);
    }

    // steal a unicorn from the chosen player's stable
    PrintStable(index);
    for (;;) {
      printf("Choose the card number to steal: ");
      index2 = NumInput(buf, &end, sizeof buf) - 1;

      // index validation
      if (index2 < 0 || index2 >= player[index].stable.size || end != (buf + strlen(buf)))
        continue;

      // make sure it's actually a unicorn card
      if (player[index].stable.unicorns[index2].species != NOSPECIES)
        break;
    }

    // swap the unicorn from the chosen player's stable to the current player (pnum)
    tmp = player[index].stable.unicorns[index2];
    RearrangeStable(index, index2);

    AddStable(pnum, tmp);
    break;
  }
  default:
    break;
  }
}

// switch cases for beginning of your turn effects
void Base_BeginningTurnEffects(int pnum, struct Unicorn corn) {
  int index, index2 = 0;
  int isvalid = 0, isvalid2 = 0;
  char ans, *end, buf[LINE_MAX];

  // Blinding Light makes it so that all unicorn cards have no effects
  if ((player[pnum].flags & BLINDING_LIGHT) != 0 &&
      (player[pnum].flags & PANDAMONIUM) == 0 &&
      corn.species != NOSPECIES)
    return;

  switch (corn.effect) {
  case NOTHING:
    break;
  case ANGEL_UNICORN:
  {
    // Playing Angel Unicorn:
    // You may sacrifice this card and bring a unicorn card from the discard pile directly into your stable

    // check if there are actually cards available to take
    for (int i = 0; i < discardpile.size; i++) {
      if (discardpile.cards[i].cType == BASICUNICORN ||
          discardpile.cards[i].cType == MAGICALUNICORN) {
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
        "order to bring a unicorn card \nfrom the discard pile directly to "
        "your stable (y/n)?: ");
      ans = CharInput(buf, sizeof buf);
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
      RearrangeStable(pnum, index2);

      PrintPileFilter(discardpile, ANYUNICORN, ANY);
      for (;;) {
        printf("Pick a valid card number to add to your stable: ");
        index = NumInput(buf, &end, sizeof buf) - 1;

        // index validation
        if (index < 0 || index >= discardpile.size || end != (buf + strlen(buf)))
          continue;

        if (discardpile.cards[index].cType == BASICUNICORN ||
            discardpile.cards[index].cType == MAGICALUNICORN)
          break;
      }
      // add it to the discard before bringing the other unicorn over in case
      // it has a special enter stable effect that can bring unicorns from
      // the discard pile
      AddDiscard(angel_tmp);

      AddStable(pnum, discardpile.cards[index]);
      RearrangePile(&discardpile, index);
    }
    break;
  }
  case DOUBLE_DUTCH:
  {
    // Playing Double Dutch:
    // You may play 2 cards during your Action phase

    turnCount++;
    break;
  }
  case EXTRA_TAIL:
  {
    // Playing Extra Tail:
    // You may draw 1 card
    Draw(pnum, 1);
    break;
  }
  case EXTREMELY_FERTILE_UNICORN:
  {
    // Playing Extremely Fertile Unicorn:
    // You may discard 1 card to bring a baby unicorn directly to your stable

    // check if there are actually cards to discard and receive, otherwise break;
    if (player[pnum].hand.numCards == 0) break;
    if (nursery.size == 0) break;

    do {
      printf(
        "Would you like to discard 1 card in order to bring a baby "
        "unicorn directly to your stable (y/n)?: ");
      ans = CharInput(buf, sizeof buf);
    } while ((ans != 'y' && ans != 'n') || strlen(buf) != 2);
    if (ans == 'y') {
      // discard then bring in a baby unicorn :D
      Discard(pnum, 1, ANY);
      PrintPile(nursery);
      do {
        printf("Pick the index associated with your desired Baby Unicorn: ");
        index = NumInput(buf, &end, sizeof buf) - 1;
      } while (index < 0 || index >= nursery.size || end != (buf + strlen(buf)));

      AddStable(pnum, nursery.cards[index]);
      RearrangePile(&nursery, index);
    }
    break;
  }
  case GLITTER_BOMB:
  {
    // Playing Glitter Bomb:
    // You may sacrifice a card, then destroy a card

    // check if there are cards to sacrifice and destroy first; only worry
    // about /that/ card in terms of sacrificing here since this can only
    // be the current player's turn
    isvalid = CheckNumCardsToDestroy(pnum, ANY, FALSE);

    if (!isvalid)
      break;

    do {
      printf(
        "Would you like to sacrifice a card in order to destroy another "
        "card (y/n)?: ");
      ans = CharInput(buf, sizeof buf);
    } while ((ans != 'y' && ans != 'n') || strlen(buf) != 2);

    if (ans == 'y') {
      Sacrifice(pnum, ANY);
      Destroy(pnum, ANY, FALSE);
    }
    break;
  }
  case RAINBOW_MANE:
  {
    // Playing Rainbow Mane:
    // You may bring a Basic Unicorn card from your hand directly into your stable

    // check if Basic Unicorns are even in the player's hand
    for (int i = 0; i < player[pnum].hand.numCards; i++) {
      if (player[pnum].hand.cards[i].cType == BASICUNICORN) {
        isvalid = 1;
        break;
      }
    }

    // check if there's a basic unicorn card to play and whether or not the player is blocked from playing basic unicorns
    if (!isvalid || (player[pnum].flags & QUEEN_BEE_UNICORN) != 0)
      break;

    printf("Choose a valid card number to place into your stable: \n");
    for (int i = 0; i < player[pnum].hand.numCards; i++) {
      if (player[pnum].hand.cards[i].cType == BASICUNICORN) {
        printf("    %d. %s [ID: %d]\n", i + 1, player[pnum].hand.cards[i].name, player[pnum].hand.cards[i].id);
      }
    }
    for (;;) {
      printf("Choice: ");
      index = NumInput(buf, &end, sizeof buf) - 1;

      // index validation
      if (index < 0 || index >= player[pnum].hand.numCards || end != (buf + strlen(buf)))
        continue;

      if (player[pnum].hand.cards[index].cType == BASICUNICORN)
        break;
    }

    AddStable(pnum, player[pnum].hand.cards[index]);
    RearrangeHand(pnum, index);
    break;
  }
  case RHINOCORN:
  {
    // Playing Rhinocorn:
    // You may destroy a unicorn card, then immediately end your turn

    // check if there are unicorn cards to destroy
    isvalid = CheckNumCardsToDestroy(pnum, ANYUNICORN, FALSE);
    if (!isvalid)
      break;

    do {
      printf(
        "Would you like to destroy a unicorn card at the expense of "
        "ending your turn (y/n)?: ");
      ans = CharInput(buf, sizeof buf);
    } while ((ans != 'y' && ans != 'n') || strlen(buf) != 2);

    if (ans == 'y') {
      Destroy(pnum, ANYUNICORN, FALSE);
      turnCount = -1;
    }
    break;
  }
  case SADISTIC_RITUAL:
  {
    // Playing Sadistic Ritual:
    // You must sacrifice 1 unicorn card, then draw a card

    // can't draw cards if there's nothing to sacrifice
    isvalid = Sacrifice(pnum, ANYUNICORN);
    if (!isvalid) {
      break;
    }
    Draw(pnum, 1);
    break;
  }
  case SUMMONING_RITUAL:
  {
    // Playing Summoning Ritual:
    // You may discard 2 Unicorn cards to bring a Unicorn card directly from the discard pile into your stable

    // check if there are unicorn cards to discard
    for (int i = 0; i < player[pnum].hand.numCards; i++) {
      if (player[pnum].hand.cards[i].cType == BASICUNICORN ||
          player[pnum].hand.cards[i].cType == MAGICALUNICORN) {
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
      ans = CharInput(buf, sizeof buf);
    } while ((ans != 'y' && ans != 'n') || strlen(buf) != 2);

    if (ans == 'y') {
      Discard(pnum, 2, ANYUNICORN);
      PrintPileFilter(discardpile, ANYUNICORN, ANY);

      for (;;) {
        printf("Pick a valid card number to add to your stable: ");
        index = NumInput(buf, &end, sizeof buf) - 1;

        // index validation
        if (index < 0 || index >= discardpile.size|| end != (buf + strlen(buf)))
          continue;

        if (discardpile.cards[index].cType == BASICUNICORN ||
            discardpile.cards[index].cType == MAGICALUNICORN)
          break;
      }
      struct Unicorn tmp = discardpile.cards[index];
      RearrangePile(&discardpile, index);
      AddStable(pnum, tmp);
    }
    break;
  }
  case UNICORN_LASSO:
  {
    // Playing Unicorn Lasso:
    // You may steal a unicorn card, and then return the unicorn card at the end of your turn

    // check if there are unicorn cards to steal
    for (int i = 0; i < currentPlayers; i++) {
      if (i != pnum && player[i].stable.numUnicorns > 0) {
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
      ans = CharInput(buf, sizeof buf);
    } while ((ans != 'y' && ans != 'n') || strlen(buf) != 2);

    // just copied the steal function because it has to be modified to fit
    // in the special flag
    if (ans == 'y') {
      PrintPlayers();
      for (;;) {
        printf("Choose a player to steal from: ");
        index = NumInput(buf, &end, sizeof buf) - 1;

        // index validation
        if (index < 0 || index >= currentPlayers || index == pnum || end != (buf + strlen(buf)))
          continue;

        // double checks that the chosen player actually has a unicorn card
        if (player[index].stable.numUnicorns > 0)
          break;
      }

      PrintStable(index);
      for (;;) {
        printf("Choose the card number to steal: ");
        index2 = NumInput(buf, &end, sizeof buf) - 1;

        // index validation
        if (index2 < 0 || index2 >= player[index].stable.size || end != (buf + strlen(buf)))
          continue;

        // check that the chosen card is actually a unicorn
        if (CheckType(ANYUNICORN, player[index].stable.unicorns[index2].cType)) {
          break;
        }
      }

      // keep track of the card index for pnum (since it's the last entry,
      // it should equal the current stable size), current player ID,
      // and stolen player's ID in order to return the card at the end of
      // the turn
      uniLassoIndex[0] = player[pnum].stable.size;
      uniLassoIndex[1] = pnum;
      uniLassoIndex[2] = index;

      // rearrange the player's stable and toggle any flags before swapping the card to the other stable;
      // this will (hopefully) prevent dupes from card effect shenanigans
      struct Unicorn tmp = player[index].stable.unicorns[index2];
      RearrangeStable(index, index2);

      AddStable(pnum, tmp);
    }
    break;
  }
  case ZOMBIE_UNICORN:
  {
    // Playing Zombie Unicorn:
    // You may discard 1 Unicorn card to bring a Unicorn card from the discard pile directly to your stable,
    // then immediately skip to the end of turn phase

    // check if there are valid unicorn cards to discard
    for (int i = 0; i < player[pnum].hand.numCards; i++) {
      if (player[pnum].hand.cards[i].cType == BASICUNICORN ||
          player[pnum].hand.cards[i].cType == MAGICALUNICORN) {
        isvalid = 1;
        break;
      }
    }

    // check if there are actually cards available to take
    for (int i = 0; i < discardpile.size; i++) {
      if (discardpile.cards[i].cType == BASICUNICORN ||
          discardpile.cards[i].cType == MAGICALUNICORN) {
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
      ans = CharInput(buf, sizeof buf);
    } while ((ans != 'y' && ans != 'n') || strlen(buf) != 2);

    if (ans == 'y') {
      Discard(pnum, 1, ANYUNICORN);

      PrintPileFilter(discardpile, ANYUNICORN, ANY);
      for (;;) {
        printf("Pick a valid card number to add to your stable: ");
        index = NumInput(buf, &end, sizeof buf) - 1;

        // index validation
        if (index < 0 || index >= discardpile.size || end != (buf + strlen(buf)))
          continue;

        // unicorn card check
        if (discardpile.cards[index].cType == BASICUNICORN ||
            discardpile.cards[index].cType == MAGICALUNICORN)
          break;

      }

      AddStable(pnum, discardpile.cards[index]);
      RearrangePile(&discardpile, index);

      // reduce turnCount in favor of "ending turn," especially since there might
      // be multiple "beginning of turn" effects that could be played
      turnCount = -1;
    }
    break;
  }
  default:
    break;
  }
}
