#include "MagicUnicornTests.h"

// sanity test
int queen_bee_basic_check() {
	int num_fails = 0;
	struct Unicorn basic_tmp = basedeck[13];
	struct Unicorn queen_tmp = basedeck[51];

	current_players = 3;
	addStable(1, queen_tmp);

	player[0].hand.cards[player[0].hand.num_cards++] = basic_tmp;
	player[2].hand.cards[player[2].hand.num_cards++] = basic_tmp;

	int nsize0 = player[0].stable.size;
	int ncorn0 = player[0].stable.num_unicorns;
	int nsize2 = player[2].stable.size;
	int ncorn2 = player[2].stable.num_unicorns;

	enterStableEffects(1, player[1].stable.unicorns[0].effect);

	assert(turn_count == 1);
	player[0].flags |= yay;
	player[2].flags |= yay;

	playCard(0);
	playCard(2);

	// it should go up twice because both player[0] and player[2] were unable to play basic unicorns
	if (turn_count != 3) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: turn count failed\n");
		reset_col();
	}

	if (player[0].stable.num_unicorns != ncorn0 || player[0].stable.size != nsize0 ||
			player[2].stable.num_unicorns != ncorn2 || player[2].stable.size != nsize2) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: stable size test failed\n");
		reset_col();
	}

	if ((player[0].flags & queen_bee_unicorn) != queen_bee_unicorn ||
			(player[2].flags & queen_bee_unicorn) != queen_bee_unicorn) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: flag check test failed\n");
		reset_col();
	}

	reset_players();
	reset_deck();
	reset_discard();
	return num_fails;
}

// Queen Bee Unicorn
//
// Basic Unicorn cards cannot enter any player's Stable except yours.
int queen_bee_unicorn_tests() {
	int num_fails = 0;

	rainbow_error("\nStarting Queen Bee Unicorn tests...\n");

	// file input stream setup
	FILE* fp;
	fopen_s(&fp, "Tests/Input/queenbeeunicorn.txt", "r");
	if (fp == NULL) {
		magenta();
		fprintf(stderr, "    file input failed :(");
		reset_col();
		return 1;
	}
	fpinput = fp;

	num_fails += queen_bee_basic_check();

	fclose(fp);
	return num_fails;
}
