#include "MagicUnicornTests.h"

// sanity check
int rainbow_unicorn_basic_check() {
	int num_fails = 0;
	struct Unicorn basic_tmp = basedeck[13];
	struct Unicorn barbed_wire_tmp = basedeck[106];
	struct Unicorn rainbow_tmp = basedeck[42];

	// stable thing went through
	addStable(0, rainbow_tmp);
	player[0].hand.cards[0] = basic_tmp;
	player[0].hand.num_cards++;
	enterStableEffects(0, rainbow_tmp.effect);

	if (player[0].stable.num_unicorns != 2 || player[0].stable.size != 2) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: stable size failed\n");
		reset_col();
	}

	if (player[0].hand.num_cards != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: hand size failed\n");
		reset_col();
	}
	reset_players();

	// no basic unicorns
	addStable(0, rainbow_tmp);
	player[0].hand.cards[0] = barbed_wire_tmp;
	player[0].hand.num_cards++;
	enterStableEffects(0, rainbow_tmp.effect);

	if (player[0].stable.num_unicorns != 1 || player[0].stable.size != 1) {
		num_fails++;
		red();
		fprintf(stderr, "    non-basic unicorn test: stable size failed\n");
		reset_col();
	}

	if (player[0].hand.num_cards != 1) {
		num_fails++;
		red();
		fprintf(stderr, "    non-basic unicorn test: hand size failed\n");
		reset_col();
	}

	reset_players();
	return num_fails;
}

// Rainbow Unicorn
//
// When this card enters your Stable, you may bring a Basic Unicorn card 
// from your hand into your Stable
int rainbow_unicorn_tests() {
	int num_fails = 0;

	if (!isclient) {
		rainbow_error("\nStarting Rainbow Unicorn tests...\n");

		// file input stream setup
		FILE* fp;
		fopen_s(&fp, "Tests/Input/line_1.txt", "r");
		if (fp == NULL) {
			magenta();
			fprintf(stderr, "    file input failed :(");
			reset_col();
			return 1;
		}
		fpinput = fp;

		num_fails += rainbow_unicorn_basic_check();

		fclose(fp);
	}
	return num_fails;
}
