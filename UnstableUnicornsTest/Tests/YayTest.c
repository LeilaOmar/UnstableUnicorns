#include "UpgradeTests.h"

// sanity test
int yay_basic_check() {
	int num_fails = 0;
	struct Unicorn yay_tmp = basedeck[100];

	addStable(0, yay_tmp);

	toggleFlags(0, yay_tmp.effect);
	assert(player[0].flags == yay);

	if (canBeNeighed(0)) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: canBeNeighed failed\n");
		reset_col();
	}

	reset_players();
	return num_fails;
}

// Yay
//
// Cards you play cannot be Neigh'd.
int yay_tests() {
	int num_fails = 0;

	rainbow("\nStarting Yay tests...\n");

	num_fails += yay_basic_check();

	return num_fails;
}
