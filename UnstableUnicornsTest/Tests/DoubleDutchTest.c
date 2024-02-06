#include "UpgradeTests.h"

// sanity test
int dutch_basic_check() {
	int num_fails = 0;
	struct Unicorn dutch_tmp = basedeck[104];
	struct Unicorn basic_tmp = basedeck[13];

	addStable(0, dutch_tmp);
	player[0].flags = yay;
	assert(player[0].stable.size == 1);
	beginningTurnEffects(0, dutch_tmp);

	// first, check the turn count
	if (turn_count != 2) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: pre-phase turn count failed\n");
		reset_col();
	}

	// now check if it actually plays 2 cards during the action phase
	player[0].hand.cards[player[0].hand.num_cards++] = basic_tmp;
	player[0].hand.cards[player[0].hand.num_cards++] = basic_tmp;

	assert(player[0].hand.num_cards == 2);

	actionPhase(0);

	if (turn_count != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: post-phase turn count failed\n");
		reset_col();
	}

	if (player[0].hand.num_cards != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: hand size failed\n");
		reset_col();
	}

	if (player[0].stable.size != 3 || player[0].stable.num_unicorns != 2 ||
			strcmp(player[0].stable.unicorns[0].name, dutch_tmp.name) != 0 ||
			strcmp(player[0].stable.unicorns[1].name, basic_tmp.name) != 0 ||
			strcmp(player[0].stable.unicorns[2].name, basic_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: stable verification failed\n");
		reset_col();
	}

	reset_players();
	return num_fails;
}

// Double Dutch
//
// If this card is in your Stable at the beginning of your turn,
// you may play 2 cards during your Action phase.
int double_dutch_tests() {
	int num_fails = 0;

	rainbow_error("\nStarting Double Dutch tests...\n");

	// file input stream setup
	FILE* fp;
	fopen_s(&fp, "Tests/Input/doubledutch.txt", "r");
	if (fp == NULL) {
		magenta();
		fprintf(stderr, "    file input failed :(");
		reset_col();
		return 1;
	}
	fpinput = fp;

	num_fails += dutch_basic_check();

	fclose(fp);
	return num_fails;
}
