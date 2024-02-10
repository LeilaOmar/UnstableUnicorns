#include "MagicUnicornTests.h"

// sanity check
int phoenix_basic_check() {
	int num_fails = 0;
	struct Unicorn basic_tmp = basedeck[13];
	struct Unicorn phoenix_tmp = basedeck[57];

	// this would only get triggered if i disabled the hand size check in
	// canBeDestroyed
	assert(discardpile.size == 0);
	assert(player[0].hand.num_cards == 0);
	assert(player[0].stable.size == 0);

	addStable(0, phoenix_tmp);
	player[0].hand.cards[player[0].hand.num_cards++] = basic_tmp;

	sacrificeDestroyEffects(0, 0, phoenix_tmp.effect);

	if (player[0].stable.num_unicorns != 1 || player[0].stable.size != 1 ||
			strcmp(player[0].stable.unicorns[0].name, phoenix_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: stable size failed\n");
		reset_col();
	}

	if (discardpile.size != 1 ||
			player[0].hand.num_cards != 0 ||
			strcmp(discardpile.cards[0].name, basic_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: hand discard failed\n");
		reset_col();
	}
	reset_players();
	reset_discard();

	// check if it can be destroyed when there are no cards in the hand
	addStable(1, phoenix_tmp);
	assert(player[1].hand.num_cards == 0);
	assert(player[1].stable.size == 1);

	if (!canBeDestroyed(0, 0, ANY, FALSE)) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: canBeDestroyed failed\n");
		reset_col();
	}

	reset_players();
	reset_discard();
	return num_fails;
}

// no cards in hand to discard
int phoenix_empty_check() {
	int num_fails = 0;
	struct Unicorn phoenix_tmp = basedeck[57];

	assert(discardpile.size == 0);
	assert(player[0].hand.num_cards == 0);
	assert(player[0].stable.size == 0);

	addStable(0, phoenix_tmp);

	sacrificeDestroyEffects(0, 0, phoenix_tmp.effect);

	if (player[0].stable.size != 0 || player[0].stable.num_unicorns != 0 ||
			strcmp(discardpile.cards[0].name, phoenix_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    empty hand test: sacrifice/destroy failed\n");
		reset_col();
	}

	reset_players();
	reset_discard();
	return num_fails;
}

// Unicorn Phoenix
//
// If this card would be sacrificed or destroyed, you may DISCARD a card instead.
int unicorn_phoenix_tests() {
	int num_fails = 0;

	if (!isclient) {
		rainbow_error("\nStarting Unicorn Phoenix tests...\n");

		// file input stream setup
		FILE* fp;
		fopen_s(&fp, "Tests/Input/line_y_1.txt", "r");
		if (fp == NULL) {
			magenta();
			fprintf(stderr, "    file input failed :(");
			reset_col();
			return 1;
		}
		fpinput = fp;

		num_fails += phoenix_basic_check();
		num_fails += phoenix_empty_check();

		fclose(fp);
	}
	return num_fails;
}
