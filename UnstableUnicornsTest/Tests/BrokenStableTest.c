#include "DowngradeTests.h"

// sanity test
int broken_basic_check() {
	int num_fails = 0;
	struct Unicorn broken_tmp = basedeck[111];
	struct Unicorn yay_tmp = basedeck[100];

	addStable(0, broken_tmp);
	player[0].hand.cards[player[0].hand.num_cards++] = yay_tmp;
	assert(player[0].flags == broken_stable);

	player[0].flags |= yay;
	playCard(0);

	if (turn_count != 2) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: turn count failed\n");
		reset_col();
	}

	if (player[0].hand.num_cards != 1 ||
			strcmp(player[0].hand.cards[0].name, yay_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: hand verification failed\n");
		reset_col();
	}

	reset_players();
	reset_discard();
	return num_fails;
}

// Broken Stable
//
// You cannot play Upgrade cards.
int broken_stable_tests() {
	int num_fails = 0;

	if (!isclient) {
		rainbow_error("\nStarting Broken Stable tests...\n");

		// file input stream setup
		FILE* fp;
		fopen_s(&fp, "Tests/Input/line_1_1.txt", "r");
		if (fp == NULL) {
			magenta();
			fprintf(stderr, "    file input failed :(");
			reset_col();
			return 1;
		}
		fpinput = fp;

		num_fails += broken_basic_check();

		fclose(fp);
	}
	return num_fails;
}
