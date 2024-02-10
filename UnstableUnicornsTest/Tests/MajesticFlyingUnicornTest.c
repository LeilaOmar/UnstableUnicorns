#include "MagicUnicornTests.h"

// sanity check
int majestic_flying_basic_check() {
	int num_fails = 0;
	struct Unicorn basic_tmp = basedeck[13];
	struct Unicorn majestic_tmp = basedeck[56];

	addStable(0, majestic_tmp);
	addDiscard(basic_tmp);

	assert(discardpile.size == 1);
	assert(player[0].hand.num_cards == 0);

	enterStableEffects(0, majestic_tmp.effect);

	if (discardpile.size != 0 ||
		player[0].hand.num_cards != 1 ||
		strcmp(player[0].hand.cards[0].name, basic_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: pulling unicorn card failed\n");
		reset_col();
	}
	reset_players();
	reset_discard();

	// check the sacrifice/destroy effects
	addStable(0, majestic_tmp);

	assert(player[0].hand.num_cards == 0);
	sacrificeDestroyEffects(0, 0, majestic_tmp.effect);

	if (player[0].hand.num_cards != 1 ||
		strcmp(player[0].hand.cards[0].name, majestic_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: hand return failed\n");
		reset_col();
	}

	reset_players();
	return num_fails;
}

// non-unicorn card in the discard pile
int majestic_flying_empty_check() {
	int num_fails = 0;
	struct Unicorn neigh_tmp = basedeck[128];
	struct Unicorn majestic_tmp = basedeck[56];

	assert(player[0].hand.num_cards == 0);

	addStable(0, majestic_tmp);
	addDiscard(neigh_tmp);

	enterStableEffects(0, majestic_tmp.effect);

	if (player[0].hand.num_cards != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    non-unicorn test: hand size failed\n");
		reset_col();
	}

	reset_players();
	reset_discard();
	return num_fails;
}

// Majestic Flying Unicorn
//
// When this card enters your Stable, you may add a Unicorn card from the discard pile to your hand.
// If this card is sacrificed or destroyed, return it to your hand.
int majestic_flying_unicorn_tests() {
	int num_fails = 0;

	if (!isclient) {
		rainbow_error("\nStarting Majestic Flying Unicorn tests...\n");

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

		num_fails += majestic_flying_basic_check();
		num_fails += majestic_flying_empty_check();

		fclose(fp);
	}
	return num_fails;
}
