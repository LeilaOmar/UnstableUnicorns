#include "MagicUnicornTests.h"

// sanity check; this doubles as a check for toggled flags
int great_narwhal_basic_check() {
	int num_fails = 0;
	struct Unicorn great_tmp = basedeck[66];
	struct Unicorn classy_tmp = basedeck[65];
	struct Unicorn corn = deck.cards[42];
	// corn is magical flying unicorn w/ the base deck; the index is notably
	// before any upgrade cards, and the shuffling assumes the same seed each time

	int tmp_size = deck.size;
	assert(player[0].hand.num_cards == 0);

	addStable(0, great_tmp);

	if (deck.size != (tmp_size - 1)) {
		num_fails++;
		red();
		fprintf(stderr, "\n    sanity test: deck size failed\n");
		reset_col();
	}

	if (strcmp(corn.name, deck.cards[42].name) == 0) {
		num_fails++;
		red();
		fprintf(stderr, "\n    sanity test: deck shuffle failed\n");
		reset_col();
	}

	if (player[0].hand.num_cards != 1 ||
			strcmp(player[0].hand.cards[0].name, classy_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: hand return failed\n");
		reset_col();
	}

	reset_players();
	reset_deck();
	return num_fails;
}

// The Great Narwhal
//
// When this card enters your Stable, you may search the deck for a card with
// "Narwhal" in its name and add it to your hand, then shuffle the deck.
int the_great_narwhal_tests() {
	int num_fails = 0;

	if (!isclient) {
		rainbow_error("\nStarting The Great Narwhal tests...\n");

		// file input stream setup
		FILE* fp;
		fopen_s(&fp, "Tests/Input/thegreatnarwhal.txt", "r");
		if (fp == NULL) {
			magenta();
			fprintf(stderr, "    file input failed :(");
			reset_col();
			return 1;
		}
		fpinput = fp;

		num_fails += great_narwhal_basic_check();

		fclose(fp);
	}
	return num_fails;
}
