#include "MagicUnicornTests.h"

// sanity check
int cob_basic_check() {
	int num_fails = 0;
	struct Unicorn basic_tmp = basedeck[13];
	struct Unicorn cob_tmp = basedeck[58];

	int tmp_size = deck.size;

	addStable(0, cob_tmp);
	player[0].hand.cards[player[0].hand.num_cards++] = basic_tmp;

	assert(discardpile.size == 0);
	assert(player[0].hand.num_cards == 1);

	enterStableEffects(0, cob_tmp.effect);

	if (player[0].hand.num_cards != 2) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: draw failed\n");
		reset_col();
	}

	if (deck.size != (tmp_size - 2) ||
			discardpile.size != 1 ||
			strcmp(discardpile.cards[0].name, basic_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: hand discard failed\n");
		reset_col();
	}

	reset_players();
	reset_discard();
	reset_deck();
	return num_fails;
}

// Unicorn on the Cob
//
// When this card enters your Stable, DRAW 2 cards and DISCARD a card.
int unicorn_on_the_cob_tests() {
	int num_fails = 0;

	rainbow_error("\nStarting Unicorn on the Cob tests...\n");

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

	num_fails += cob_basic_check();

	fclose(fp);
	return num_fails;
}
