#include "MagicUnicornTests.h"

// basic check
int llamacorn_basic_check() {
	int num_fails = 0;
	struct Unicorn llamacorn_tmp = basedeck[46];

	current_players = 3;
	addStable(0, llamacorn_tmp);
	draw(0, 1);
	draw(2, 1);

	int nhand0 = player[0].hand.num_cards;
	int nhand1 = player[1].hand.num_cards;
	int nhand2 = player[2].hand.num_cards;

	enterStableEffects(0, player[0].stable.unicorns[0].effect);

	if (player[0].hand.num_cards != (nhand0 - 1) ||
			player[1].hand.num_cards != nhand1 || 
			player[2].hand.num_cards != (nhand2 - 1)) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: hand size failed\n");
		reset_col();
	}

	// only 2 cards should have been discarded because player[1] did not have any
	// in their hand to discard, and EVERY player including player[0] must discard
	// a card if they have one
	if (discardpile.size != 2) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: discard pile size failed\n");
		reset_col();
	}

	reset_players();
	reset_deck();
	reset_discard();
	return num_fails;
}

// Llamacorn
//
// When this card enters your Stable, each player (including you)
// must DISCARD a card.
int llamacorn_tests() {
	int num_fails = 0;

	rainbow_error("\nStarting Llamacorn tests...\n");

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

	num_fails += llamacorn_basic_check();

	fclose(fp);
	return num_fails;
}
