#include "MagicTests.h"

// sanity test
int blatant_basic_check() {
	int num_fails = 0;
	struct Unicorn blatant_tmp = basedeck[90];
	struct Unicorn basic_tmp = basedeck[13];

	current_players = 3;
	toggleFlags(0, yay_effect);
	player[0].hand.cards[player[0].hand.num_cards++] = blatant_tmp;
	player[1].hand.cards[player[1].hand.num_cards++] = basic_tmp;

	assert(discardpile.size == 0);
	assert(player[0].hand.num_cards == 1);
	assert(player[1].hand.num_cards == 1);
	assert(player[2].hand.num_cards == 0);
	assert(player[0].flags == yay);
	playCard(0);

	if (turn_count != 1) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: turn count failed\n");
		reset_col();
	}

	if (player[0].hand.num_cards != 1 ||
			strcmp(player[0].hand.cards[0].name, basic_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: hand verification failed\n");
		reset_col();
	}

	if (discardpile.size != 1 ||
			strcmp(discardpile.cards[0].name, blatant_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: discard verification failed\n");
		reset_col();
	}

	reset_players();
	reset_discard();
	return num_fails;
}

// no cards to steal from hands
int blatant_empty_check() {
	int num_fails = 0;
	struct Unicorn blatant_tmp = basedeck[90];

	current_players = 2;
	player[0].hand.cards[player[0].hand.num_cards++] = blatant_tmp;

	int ret;
	assert(player[0].hand.num_cards == 1);
	assert(player[1].hand.num_cards == 0);
	ret = conditionalEffects(0, blatant_tmp, 0, 0);

	if (turn_count != 2 || ret != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: turn count failed\n");
		reset_col();
	}

	reset_players();
	reset_discard();
	return num_fails;
}

// Blatant Thievery
//
// Look at another player's hand. Choose a card and add it to your hand.
int blatant_thievery_tests() {
	int num_fails = 0;

	if (!isclient) {
		rainbow_error("\nStarting Blatant Thievery tests...\n");

		// file input stream setup
		FILE* fp;
		fopen_s(&fp, "Tests/Input/blatantthievery.txt", "r");
		if (fp == NULL) {
			magenta();
			fprintf(stderr, "    file input failed :(");
			reset_col();
			return 1;
		}
		fpinput = fp;

		num_fails += blatant_basic_check();
		num_fails += blatant_empty_check();

		fclose(fp);
	}
	return num_fails;
}
