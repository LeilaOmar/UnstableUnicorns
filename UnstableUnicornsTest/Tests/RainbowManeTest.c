#include "UpgradeTests.h"

// sanity test
int rainbow_mane_basic_check() {
	int num_fails = 0;
	struct Unicorn rainmane_tmp = basedeck[92];
	struct Unicorn basic_tmp = basedeck[13];

	addStable(0, basic_tmp);
	player[0].hand.cards[player[0].hand.num_cards++] = rainmane_tmp;
	player[0].hand.cards[player[0].hand.num_cards++] = basic_tmp;

	int ret;
	assert(player[0].stable.size == 1);
	assert(player[0].hand.num_cards == 2);
	ret = conditionalEffects(0, rainmane_tmp, 0, 0);

	if (turn_count != 1 || ret != 1) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: turn count failed\n");
		reset_col();
	}

	if (player[0].stable.size != 2 || player[0].stable.num_unicorns != 1) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: (pre-beginning turn) stable size failed\n");
		reset_col();
	}

	beginningTurnEffects(0, rainmane_tmp);

	if (player[0].stable.size != 3 || player[0].stable.num_unicorns != 2) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: (post-beginning turn) stable size failed\n");
		reset_col();
	}

	if (player[0].hand.num_cards != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: (post-beginning turn) hand size failed\n");
		reset_col();
	}

	reset_players();
	return num_fails;
}

// no basic unicorn in your stable
int rainbow_mane_empty_check() {
	int num_fails = 0;
	struct Unicorn rainmane_tmp = basedeck[92];

	player[0].hand.cards[player[0].hand.num_cards++] = rainmane_tmp;

	int ret;
	assert(discardpile.size == 0);
	assert(player[0].stable.size == 0);
	assert(player[0].hand.num_cards == 1);
	ret = conditionalEffects(0, rainmane_tmp, 0, 0);

	if (turn_count != 2 || ret != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    empty stable test: turn count failed\n");
		reset_col();
	}

	if (player[0].hand.num_cards != 1 ||
		strcmp(player[0].hand.cards[0].name, rainmane_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    empty stable test: hand verification failed\n");
		reset_col();
	}

	if (player[0].stable.size != 0 || player[0].stable.num_unicorns != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    empty stable test: stable size failed\n");
		reset_col();
	}

	reset_players();
	return num_fails;
}

// Rainbow Mane
//
// This card can only enter a Stable if there is a Basic Unicorn card in that Stable.
// If this card is in your Stable at the beginning of your turn, you may bring a
// Basic Unicorn card from your hand directly into your Stable.
int rainbow_mane_tests() {
	int num_fails = 0;

	rainbow_error("\nStarting Rainbow Mane tests...\n");

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

	num_fails += rainbow_mane_basic_check();
	num_fails += rainbow_mane_empty_check();

	fclose(fp);
	return num_fails;
}
