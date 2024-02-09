#include "MagicUnicornTests.h"

// basic check
int llamacorn_basic_check() {
	int num_fails = 0;
	struct Unicorn llamacorn_tmp = basedeck[46];
	struct Unicorn basic_tmp = basedeck[13];

	current_players = 2;
	player[0].hand.cards[player[0].hand.num_cards++] = basic_tmp;

	assert(player[0].hand.num_cards == 1);
	assert(player[1].hand.num_cards == 0);
	addStable(0, llamacorn_tmp);

	if (player[0].hand.num_cards != 0 ||
			player[1].hand.num_cards != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: hand size failed\n");
		reset_col();
	}

	// only 1 card should have been discarded because player[1] did not have any
	// in their hand to discard, and EVERY player including player[0] must discard
	// a card if they have one
	if (discardpile.size != 1 ||
			strcmp(discardpile.cards[0].name, basic_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: discard pile size failed\n");
		reset_col();
	}

	reset_players();
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
	fopen_s(&fp, "Tests/Input/line_1.txt", "r");
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
