#include "MagicUnicornTests.h"

// sanity check
int shabby_basic_check() {
	int num_fails = 0;
	struct Unicorn shabby_tmp = basedeck[61];
	int tmp_size = deck.size;

	enterStableEffects(0, shabby_tmp.effect);

	if (player[0].hand.num_cards != 1) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: hand size failed\n");
		reset_col();
	}

	if (player[0].hand.cards[0].cType != DOWNGRADE) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: downgrade check failed\n");
		reset_col();
	}

	if (deck.size != (tmp_size - 1)) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: deck shuffle size failed\n");
		reset_col();
	}

	reset_players();
	reset_deck();
	return num_fails;
}

// Shabby the Narwhal
//
// When this card enters your Stable, you may search the deck for a
// Downgrade card and add it to your hand, then shuffle the deck.
int shabby_the_narwhal_tests() {
	int num_fails = 0;

	if (!isclient) {
		rainbow_error("\nStarting Shabby the Narwhal tests...\n");

		// file input stream setup
		FILE* fp;
		fopen_s(&fp, "Tests/Input/shabbythenarwhal.txt", "r");
		if (fp == NULL) {
			magenta();
			fprintf(stderr, "    file input failed :(");
			reset_col();
			return 1;
		}
		fpinput = fp;

		num_fails += shabby_basic_check();

		fclose(fp);
	}
	return num_fails;
}
