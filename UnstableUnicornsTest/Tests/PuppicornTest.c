#include "MagicUnicornTests.h"

// sanity check
int puppicorn_basic_check() {
	int num_fails = 0;
	struct Unicorn basic_tmp = basedeck[13];
	struct Unicorn puppicorn_tmp = basedeck[41];

	reset_players();

	// does it move?
	current_players = 2;
	addStable(0, puppicorn_tmp);
	addStable(1, basic_tmp);
	addStable(1, basic_tmp);

	endOfTurn(0);

	if (player[0].stable.num_unicorns != 0 || player[0].stable.size != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: player[0] stable size failed\n");
		reset_col();
	}

	if (player[1].stable.num_unicorns != 3 || player[1].stable.size != 3) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: player[1] stable size failed\n");
		reset_col();
	}

	if (puppicorn_index[0] != 2 || puppicorn_index[1] != 1 ||
			strcmp(player[1].stable.unicorns[2].name, puppicorn_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: puppicorn index failed\n");
		reset_col();
	}

	reset_players();
	return num_fails;
}

int puppicorn_sacrifice_destroy_check() {
	int num_fails = 0;
	struct Unicorn puppicorn_tmp = basedeck[41];

	addStable(0, puppicorn_tmp);

	// destroy test
	canBeDestroyed(0, 0, ANY, FALSE);

	if (canBeDestroyed(0, 0, ANY, FALSE)) {
		num_fails++;
		red();
		fprintf(stderr, "    destroy test: canBeDestroyed failed\n");
		reset_col();
	}

	// sacrifice test
	canBeSacrificed(0, 0, ANY);

	if (canBeSacrificed(0, 0, ANY)) {
		num_fails++;
		red();
		fprintf(stderr, "    sacrifice test: canBeSacrificed failed\n");
		reset_col();
	}

	reset_players();
	return num_fails;
}

// win condition should be checked before it moves to the next player
int puppicorn_win_check() {
	int num_fails = 0;
	struct Unicorn puppicorn_tmp = basedeck[41];

	current_players = 2;
	for (int i = 0; i < 6; i++) {
		addStable(0, basedeck[i]);
	}
	addStable(0, puppicorn_tmp);

	int win = endOfTurn(0);

	if (win != 1) {
		num_fails++;
		red();
		fprintf(stderr, "    win check: endOfTurn failed\n");
		reset_col();
	}

	reset_players();
	return num_fails;
}

// puppicorn_index[0] should be -1 once it's returned to someone's hand
int puppicorn_reset_check() {
	int num_fails = 0;
	struct Unicorn basic_tmp = basedeck[13];
	struct Unicorn puppicorn_tmp = basedeck[41];

	current_players = 2;
	addStable(0, puppicorn_tmp);
	addStable(1, basic_tmp);

	endOfTurn(0);
	returnCardToHand(1, 1);

	if (puppicorn_index[0] != -1) {
		num_fails++;
		red();
		fprintf(stderr, "    reset test: puppicorn index failed\n");
		reset_col();
	}

	if (player[0].stable.num_unicorns != 0 || player[0].stable.size != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    reset test: stable size failed\n");
		reset_col();
	}


	return num_fails;
}

// puppicorn should loop around the players
int puppicorn_swap_check() {
	int num_fails = 0;
	struct Unicorn basic_tmp = basedeck[13];
	struct Unicorn puppicorn_tmp = basedeck[41];

	current_players = 3;
	addStable(0, basic_tmp);
	addStable(0, basic_tmp);
	addStable(0, basic_tmp);
	addStable(2, puppicorn_tmp);

	endOfTurn(2);

	if (player[0].stable.num_unicorns != 4 || player[0].stable.size != 4 ||
			puppicorn_index[0] != 3 ||
			strcmp(player[0].stable.unicorns[3].name, puppicorn_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    swap test: stable size & puppicorn name check failed\n");
		reset_col();
	}

	reset_players();
	return num_fails;
}

// puppicorn_index[0] should adjust accordingly to any stable changes
int puppicorn_rearrange_check() {
	int num_fails = 0;
	struct Unicorn basic_tmp = basedeck[13];
	struct Unicorn puppicorn_tmp = basedeck[41];

	addStable(0, basic_tmp);
	addStable(0, puppicorn_tmp);

	assert(puppicorn_index[0] == 1);
	sacrifice(0, ANY);

	// puppicorn_index[0] should become 0 if the first index gets sacrificed or destroyed
	if (puppicorn_index[0] != 0 ||
			strcmp(player[0].stable.unicorns[puppicorn_index[0]].name, puppicorn_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    rearrange test: puppicorn index failed\n");
		reset_col();
	}

	reset_players();
	reset_discard();
	return num_fails;
}

// Puppicorn
//
// Each time any player begins their turn, move this card to that player's Stable.
// This card cannot be sacrificed or destroyed.
int puppicorn_tests() {
	int num_fails = 0;

	rainbow_error("\nStarting Puppicorn tests...\n");

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

	num_fails += puppicorn_basic_check();
	num_fails += puppicorn_sacrifice_destroy_check();
	num_fails += puppicorn_win_check();
	num_fails += puppicorn_reset_check();
	num_fails += puppicorn_swap_check();
	num_fails += puppicorn_rearrange_check();

	return num_fails;
}
