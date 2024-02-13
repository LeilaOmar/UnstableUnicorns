#include "MagicUnicornTests.h"

// sanity check
int ginormous_basic_check() {
	int num_fails = 0;
	struct Unicorn basic_tmp = basedeck[13];
	struct Unicorn ginormous_tmp = basedeck[48];

	current_players = 2;
	addStable(0, ginormous_tmp);
	addStable(0, basic_tmp);
	addStable(0, basic_tmp);
	addStable(0, basic_tmp);
	addStable(0, basic_tmp);
	addStable(0, basic_tmp);

	int ret = endOfTurn(0);

	assert(WIN_CONDITION == 7);
	assert(player[0].stable.num_unicorns == 6);
	assert((player[0].flags & ginormous_unicorn) == ginormous_unicorn);

	if (ret != 1) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: win condition failed\n");
		reset_col();
	}

	if (canNeighOthers(0)) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: canNeighOthers failed\n");
		reset_col();
	}

	toggleFlags(0, ginormous_tmp.effect);
	ret = endOfTurn(0);

	if (ret != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: toggle flag failed\n");
		reset_col();
	}

	reset_players();
	reset_deck();
	return num_fails;
}

// Ginormous Unicorn
//
// This card counts for 2 Unicorns. You cannot play any Neigh cards.
int ginormous_unicorn_tests() {
	int num_fails = 0;

	if (!isclient) {
		rainbow_error("\nStarting Ginormous Unicorn tests...\n");

		num_fails += ginormous_basic_check();
	}
	return num_fails;
}
