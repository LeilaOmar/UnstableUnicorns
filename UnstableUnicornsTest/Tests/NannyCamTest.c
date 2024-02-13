#include "DowngradeTests.h"

// sanity check
int nanny_cam_basic_check() {
	int num_fails = 0;
	struct Unicorn nanny_tmp = basedeck[110];

	addStable(0, nanny_tmp);

	// a proper test would have to check against stdout during the beginning of turn phase
	// this just checks if the flag works LOL
	if (player[0].flags != nanny_cam) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: toggle flags failed\n");
		reset_col();
	}

	toggleFlags(0, nanny_tmp.effect);

	if (player[0].flags == nanny_cam) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: toggle flags pt 2 failed\n");
		reset_col();
	}

	reset_players();
	return num_fails;
}

// Nanny Cam
//
// Your hand must be visible to all players.
int nanny_cam_tests() {
	int num_fails = 0;

	if (!isclient) {
		rainbow_error("\nStarting Nanny Cam tests...\n");

		num_fails += nanny_cam_basic_check();
	}
	return num_fails;
}
