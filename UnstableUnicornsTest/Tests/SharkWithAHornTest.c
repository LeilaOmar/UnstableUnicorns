#include "MagicUnicornTests.h"

// sanity check
int shark_basic_check() {
	int num_fails = 0;
	struct Unicorn basic_tmp = basedeck[13];
	struct Unicorn shark_tmp = basedeck[60];

	current_players = 2;
	addStable(0, shark_tmp);
	addStable(1, basic_tmp);

	assert(discardpile.size == 0);
	assert(player[0].stable.num_unicorns == 1);
	assert(player[1].stable.num_unicorns == 1);
	enterStableEffects(0, shark_tmp.effect);

	if (player[0].stable.size != 0 || player[0].stable.num_unicorns != 0 ||
			player[1].stable.size != 0 || player[1].stable.num_unicorns != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: stable size failed\n");
		reset_col();
	}

	if (discardpile.size != 2 ||
			strcmp(discardpile.cards[0].name, shark_tmp.name) != 0 ||
			strcmp(discardpile.cards[1].name, basic_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: sacrifice failed\n");
		reset_col();
	}

	reset_players();
	reset_discard();
	return num_fails;
}

// no available unicorns to destroy
int shark_empty_check() {
	int num_fails = 0;
	struct Unicorn yay_tmp = basedeck[100];
	struct Unicorn shark_tmp = basedeck[60];

	current_players = 2;
	addStable(0, shark_tmp);
	addStable(1, yay_tmp);

	assert(discardpile.size == 0);
	assert(player[0].stable.num_unicorns == 1);
	assert(player[1].stable.size == 1);
	enterStableEffects(0, shark_tmp.effect);

	if (player[0].stable.size != 1 || player[0].stable.num_unicorns != 1 ||
			player[1].stable.size != 1 || player[1].stable.num_unicorns != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    non-unicorn test: stable size failed\n");
		reset_col();
	}

	if (discardpile.size != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    non-unicorn test: discard size failed\n");
		reset_col();
	}

	reset_players();
	reset_discard();
	return num_fails;
}

// Shark With a Horn
//
// When this card enters your Stable, you may SACRIFICE this card,
// then DESTROY a Unicorn card.
int shark_with_a_horn_tests() {
	int num_fails = 0;

	rainbow_error("\nStarting Shark With a Horn tests...\n");

	// file input stream setup
	FILE* fp;
	fopen_s(&fp, "Tests/Input/sharkwithahorn.txt", "r");
	if (fp == NULL) {
		magenta();
		fprintf(stderr, "    file input failed :(");
		reset_col();
		return 1;
	}
	fpinput = fp;

	num_fails += shark_basic_check();
	num_fails += shark_empty_check();

	fclose(fp);
	return num_fails;
}
