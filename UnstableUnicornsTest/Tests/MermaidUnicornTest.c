#include "MagicUnicornTests.h"

// sanity check; this doubles as a check for toggled flags
int mermaid_basic_check() {
	int num_fails = 0;
	struct Unicorn mermaid_tmp = basedeck[64];
	struct Unicorn yay_tmp = basedeck[100];

	current_players = 2;
	addStable(1, mermaid_tmp);
	addStable(0, yay_tmp);
	toggleFlags(0, yay_tmp.effect);

	assert(player[0].stable.size == 1);
	assert(player[0].hand.num_cards == 0);
	assert((player[0].flags & yay) == yay);

	assert(player[1].stable.size == 1);
	assert(player[1].stable.num_unicorns == 1);
	enterStableEffects(1, mermaid_tmp.effect);

	if (player[0].stable.size != 0 || player[0].stable.num_unicorns != 0 ||
			player[1].stable.size != 1 || player[1].stable.num_unicorns != 1) {
		num_fails++;
		red();
		fprintf(stderr, "\n    sanity test: stable size failed\n");
		reset_col();
	}

	if (player[0].hand.num_cards != 1 ||
			strcmp(player[0].hand.cards[0].name, yay_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: hand return failed\n");
		reset_col();
	}

	// check to make sure that the yay flag was disabled for player[0]
	if ((player[0].flags & yay) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: toggle flag check failed\n");
		reset_col();
	}

	reset_players();
	return num_fails;
}

// Mermaid Unicorn
//
// When this card enters your Stable, return a card in another player's Stable to their hand.
int mermaid_unicorn_tests() {
	int num_fails = 0;

	if (!isclient) {
		rainbow_error("\nStarting Mermaid Unicorn tests...\n");

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

		num_fails += mermaid_basic_check();

		fclose(fp);
	}
	return num_fails;
}
