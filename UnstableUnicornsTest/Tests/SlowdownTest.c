#include "DowngradeTests.h"

// sanity check
int slowdown_basic_check() {
	int num_fails = 0;
	struct Unicorn slowdown_tmp = basedeck[109];

	addStable(0, slowdown_tmp);
	toggleFlags(0, slowdown_tmp.effect);
	assert(player[0].flags == slowdown);

	if (canNeighOthers(0)) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: canNeighOthers failed\n");
		reset_col();
	}

	reset_players();
	return num_fails;
}

// Slowdown
//
// You cannot play Instant cards.
int slowdown_tests() {
	int num_fails = 0;

	if (!isclient) {
		rainbow_error("\nStarting Slowdown tests...\n");

		num_fails += slowdown_basic_check();
	}
	return num_fails;
}
