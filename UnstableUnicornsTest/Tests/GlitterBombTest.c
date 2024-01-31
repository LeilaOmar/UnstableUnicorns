#include "UpgradeTests.h"

// sanity test
int glitter_bomb_basic_check() {
	int num_fails = 0;
	struct Unicorn bomb_tmp = basedeck[98];
	struct Unicorn basic_tmp = basedeck[13];

	current_players = 3;
	addStable(0, bomb_tmp);
	addStable(0, basic_tmp);
	addStable(1, basic_tmp);

	assert(player[0].stable.size == 2);
	assert(player[1].stable.size == 1);
	beginningTurnEffects(0, bomb_tmp);

	if (player[0].stable.size != 1 || player[0].stable.num_unicorns != 0 ||
			player[1].stable.size != 0 || player[1].stable.num_unicorns != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: stable size failed\n");
		reset_col();
	}

	if (discardpile.size != 2 ||
			strcmp(discardpile.cards[0].name, basic_tmp.name) != 0 ||
			strcmp(discardpile.cards[1].name, basic_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: discard verification failed\n");
		reset_col();
	}

	reset_players();
	reset_discard();
	return num_fails;
}

// no cards to destroy; you can always sacrifice glitter bomb itself!
int glitter_bomb_empty_check() {
	int num_fails = 0;
	struct Unicorn bomb_tmp = basedeck[98];
	struct Unicorn puppicorn_tmp = basedeck[41];
	struct Unicorn basic_tmp = basedeck[13];

	// no cards to destroy
	current_players = 2;
	addStable(0, bomb_tmp);
	addStable(0, basic_tmp);
	addStable(1, puppicorn_tmp);

	assert(player[0].stable.size == 2);
	assert(player[1].stable.size == 1);
	beginningTurnEffects(0, bomb_tmp);

	if (player[0].stable.size != 2 || player[0].stable.num_unicorns != 1 ||
			player[1].stable.size != 1 || player[1].stable.num_unicorns != 1) {
		num_fails++;
		red();
		fprintf(stderr, "    empty test: stable size failed\n");
		reset_col();
	}

	if (discardpile.size != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    empty test: discard size failed\n");
		reset_col();
	}

	reset_players();
	reset_discard();
	return num_fails;
}

// Glitter Bomb
//
// If this card is in your Stable at the beginning of your turn,
// you may SACRIFICE a card, then DESTROY a card.
int glitter_bomb_tests() {
	int num_fails = 0;

	rainbow_error("\nStarting Glitter Bomb tests...\n");

	// file input stream setup
	FILE* fp;
	fopen_s(&fp, "Tests/Input/glitterbomb.txt", "r");
	if (fp == NULL) {
		magenta();
		fprintf(stderr, "    file input failed :(");
		reset_col();
		return 1;
	}
	fpinput = fp;

	num_fails += glitter_bomb_basic_check();
	num_fails += glitter_bomb_empty_check();

	fclose(fp);
	return num_fails;
}
