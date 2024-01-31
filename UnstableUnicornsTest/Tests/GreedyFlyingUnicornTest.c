#include "MagicUnicornTests.h"

// sanity check
int greedy_basic_check() {
	int num_fails = 0;
	struct Unicorn greedy_tmp = basedeck[52];

	addStable(0, greedy_tmp);

	assert(player[0].hand.num_cards == 0);

	enterStableEffects(0, greedy_tmp.effect);

	if (player[0].hand.num_cards != 1) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: draw failed\n");
		reset_col();
	}
	reset_players();

	// check the sacrifice/destroy effects
	addStable(0, greedy_tmp);

	assert(player[0].hand.num_cards == 0);
	sacrificeDestroyEffects(0, 0, greedy_tmp.effect);

	if (player[0].hand.num_cards != 1 ||
			strcmp(player[0].hand.cards[0].name, greedy_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: hand return failed\n");
		reset_col();
	}

	reset_players();
	return num_fails;
}

// Greedy Flying Unicorn
//
// When this card enters your Stable, DRAW a card.
// If this card is sacrificed or destroyed, return it to your hand.
int greedy_flying_unicorn_tests() {
	int num_fails = 0;

	rainbow_error("\nStarting Greedy Flying Unicorn tests...\n");

	num_fails += greedy_basic_check();

	return num_fails;
}
