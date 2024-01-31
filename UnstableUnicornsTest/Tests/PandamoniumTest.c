#include "DowngradeTests.h"

// tests for when unicorns enter the stable
int pandamonium_win_check() {
	int num_fails = 0;
	struct Unicorn panda_tmp = basedeck[107];
	struct Unicorn basic_tmp = basedeck[13];

	addStable(0, panda_tmp);
	addStable(0, basic_tmp);
	addStable(0, basic_tmp);
	addStable(0, basic_tmp);
	addStable(0, basic_tmp);
	addStable(0, basic_tmp);
	addStable(0, basic_tmp);
	addStable(0, basic_tmp);
	toggleFlags(0, panda_tmp.effect);

	assert(player[0].stable.size == 8);
	assert(player[0].stable.num_unicorns == 7);
	assert(player[0].stable.num_unicorns >= WIN_CONDITION);
	assert(player[0].flags == pandamonium);

	// endOfTurn returns 1 if the player won, or 0 if the game continues
	if (endOfTurn(0)) {
		num_fails++;
		red();
		fprintf(stderr, "    win condition test: endOfTurn failed\n");
		reset_col();
	}

	reset_players();
	reset_discard();
	return num_fails;
}

int pandamonium_destroy_check() {
	int num_fails = 0;
	struct Unicorn panda_tmp = basedeck[107];
	struct Unicorn basic_tmp = basedeck[13];

	addStable(0, panda_tmp);
	addStable(0, basic_tmp);
	toggleFlags(0, panda_tmp.effect);
	assert(player[0].stable.size == 2);
	assert(player[0].flags == pandamonium);

	// player[0][1] == basic_tmp
	if (canBeDestroyed(0, 1, ANYUNICORN, FALSE)) {
		num_fails++;
		red();
		fprintf(stderr, "    destroy test: canBeDestroyed ANYUNICORN class failed\n");
		reset_col();
	}

	if (!canBeDestroyed(0, 1, ANY, FALSE)) {
		num_fails++;
		red();
		fprintf(stderr, "    destroy test: canBeDestroyed ANY class failed\n");
		reset_col();
	}

	reset_players();
	return num_fails;
}

int pandamonium_rainbow_mane_check() {
	int num_fails = 0;
	struct Unicorn panda_tmp = basedeck[107];
	struct Unicorn rainbow_tmp = basedeck[42];
	struct Unicorn basic_tmp = basedeck[13];

	addStable(0, panda_tmp);
	addStable(0, rainbow_tmp);
	toggleFlags(0, panda_tmp.effect);
	player[0].hand.cards[player[0].hand.num_cards++] = basic_tmp;

	assert(player[0].stable.size == 2);
	assert(player[0].hand.num_cards == 1);
	assert(player[0].flags == pandamonium);
	enterStableEffects(0, rainbow_tmp.effect);

	if (player[0].hand.num_cards != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    rainbow mane test: hand size failed\n");
		reset_col();
	}

	if (player[0].stable.size != 3 || player[0].stable.num_unicorns != 2 ||
			strcmp(player[0].stable.unicorns[2].name, basic_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    rainbow mane test: stable verification failed\n");
		reset_col();
	}

	reset_players();
	return num_fails;
}

// Pandamonium
//
// All of your Unicorns are considered Pandas. Cards that affect
// Unicorn cards do not affect your Pandas.
int pandamonium_tests() {
	int num_fails = 0;

	rainbow_error("\nStarting Pandamonium tests...\n");

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

	// already tested it w/ barbed wire, rainbow aura, black knight unicorn, extremely destructive unicorn, sadistic ritual, tiny stable
	num_fails += pandamonium_win_check();
	num_fails += pandamonium_destroy_check();
	num_fails += pandamonium_rainbow_mane_check(); // just making sure that cards are only pandas when inside the stable, not in the hand

	fclose(fp);
	return num_fails;
}
