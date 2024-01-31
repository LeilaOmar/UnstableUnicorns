#include "MagicUnicornTests.h"

// sanity check
int destructive_basic_check() {
	int num_fails = 0;
	struct Unicorn basic_tmp = basedeck[13];
	struct Unicorn barbed_wire_tmp = basedeck[106];
	struct Unicorn destructive_tmp = basedeck[44];

	current_players = 3;
	addStable(1, basic_tmp);
	addStable(2, barbed_wire_tmp);

	assert(player[0].stable.size == 0);
	assert(player[1].stable.size == 1);
	assert(player[2].stable.size == 1);
	assert(player[0].stable.num_unicorns == 0);
	assert(player[1].stable.num_unicorns == 1);
	assert(player[2].stable.num_unicorns == 0);
	addStable(0, destructive_tmp);

	if (player[0].stable.num_unicorns != 0 || player[0].stable.size != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: player[0] stable size failed\n");
		reset_col();
	}

	if (player[1].stable.num_unicorns != 0 || player[1].stable.size != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: player[1] stable size failed\n");
		reset_col();
	}

	if (player[2].stable.num_unicorns != 0 || player[2].stable.size != 1) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: player[2] stable size failed\n");
		reset_col();
	}

	if (discardpile.size != 2 ||
			strcmp(discardpile.cards[0].name, destructive_tmp.name) != 0 ||
			strcmp(discardpile.cards[0].name, destructive_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: discard pile verification failed\n");
		reset_col();
	}

	reset_players();
	reset_deck();
	reset_discard();
	return num_fails;
}

// pandas are not unicorns
int destructive_pandamonium_check() {
	int num_fails = 0;
	struct Unicorn basic_tmp = basedeck[13];
	struct Unicorn destructive_tmp = basedeck[44];
	struct Unicorn panda_tmp = basedeck[107];

	current_players = 3;
	addStable(0, panda_tmp);
	addStable(1, basic_tmp);
	toggleFlags(0, panda_tmp.effect);

	assert(player[0].stable.size == 1);
	assert(player[1].stable.size == 1);
	assert(player[2].stable.size == 0);
	assert(player[0].flags == pandamonium);
	addStable(0, destructive_tmp);

	if (player[0].stable.num_unicorns != 1 || player[0].stable.size != 2 ||
			strcmp(player[0].stable.unicorns[0].name, panda_tmp.name) != 0 ||
			strcmp(player[0].stable.unicorns[1].name, destructive_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    pandamonium test: player[0] stable size failed\n");
		reset_col();
	}

	if (player[1].stable.num_unicorns != 0 || player[1].stable.size != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    pandamonium test: player[1] stable size failed\n");
		reset_col();
	}

	if (player[1].stable.num_unicorns != 0 || player[1].stable.size != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    pandamonium test: player[2] stable size failed\n");
		reset_col();
	}

	if (discardpile.size != 1 ||
			strcmp(discardpile.cards[0].name, basic_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: discard pile verification failed\n");
		reset_col();
	}

	reset_players();
	reset_discard();
	return num_fails;
}

// Extremely Destructive Unicorn
//
// When this card enters your Stable, each player (including you)
// must SACRIFICE a Unicorn card.
int extremely_destructive_unicorn_tests() {
	int num_fails = 0;

	rainbow_error("\nStarting Extremely Destructive Unicorn tests...\n");

	// file input stream setup
	FILE* fp;
	fopen_s(&fp, "Tests/Input/line_1_1_1.txt", "r");
	if (fp == NULL) {
		magenta();
		fprintf(stderr, "    file input failed :(");
		reset_col();
		return 1;
	}
	fpinput = fp;

	num_fails += destructive_basic_check();
	num_fails += destructive_pandamonium_check();

	fclose(fp);
	return num_fails;
}
