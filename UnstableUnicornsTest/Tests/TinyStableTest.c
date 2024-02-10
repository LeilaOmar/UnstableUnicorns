#include "DowngradeTests.h"

// sanity test
int tiny_stable_basic_check() {
	int num_fails = 0;
	struct Unicorn tiny_tmp = basedeck[113];
	struct Unicorn fertile_tmp = basedeck[38];
	struct Unicorn basic_tmp = basedeck[13];
	struct Unicorn dutch_tmp = basedeck[104];

	// adding a unicorn card
	addStable(0, fertile_tmp);
	addStable(0, basic_tmp);
	addStable(0, basic_tmp);
	addStable(0, basic_tmp);
	addStable(0, basic_tmp);
	addStable(0, tiny_tmp);
	toggleFlags(0, tiny_tmp.effect);

	assert(discardpile.size == 0);
	assert(player[0].stable.size == 6);
	assert(player[0].stable.num_unicorns == 5);
	assert(player[0].flags == tiny_stable);

	addStable(0, basic_tmp);

	if (player[0].stable.size != 6 || player[0].stable.num_unicorns != 5) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test (unicorn): hand size failed\n");
		reset_col();
	}

	if (discardpile.size != 1 ||
			strcmp(discardpile.cards[0].name, fertile_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test (unicorn): discard size failed\n");
		reset_col();
	}
	reset_players();
	reset_discard();
	
	// adding a non-unicorn card
	addStable(0, fertile_tmp);
	addStable(0, basic_tmp);
	addStable(0, basic_tmp);
	addStable(0, basic_tmp);
	addStable(0, basic_tmp);
	addStable(0, tiny_tmp);
	toggleFlags(0, tiny_tmp.effect);

	assert(discardpile.size == 0);
	assert(player[0].stable.size == 6);
	assert(player[0].stable.num_unicorns == 5);
	assert(player[0].flags == tiny_stable);

	addStable(0, dutch_tmp);

	if (player[0].stable.size != 7 || player[0].stable.num_unicorns != 5) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test (non-unicorn): hand size failed\n");
		reset_col();
	}

	if (discardpile.size != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test (non-unicorn): discard size failed\n");
		reset_col();
	}

	reset_players();
	reset_discard();
	return num_fails;
}

// tiny stable should trigger when puppicorn enters your stable, and
// once it leaves by the end of your turn, you should have 4 unicorns left
int tiny_stable_puppicorn_check() {
	int num_fails = 0;
	struct Unicorn tiny_tmp = basedeck[113];
	struct Unicorn basic_tmp = basedeck[13];
	struct Unicorn basic_tmp2 = basedeck[17];
	struct Unicorn puppicorn_tmp = basedeck[41];

	current_players = 2;
	addStable(0, basic_tmp2);
	addStable(0, basic_tmp);
	addStable(0, basic_tmp);
	addStable(0, basic_tmp);
	addStable(0, basic_tmp);
	addStable(0, tiny_tmp);
	toggleFlags(0, tiny_tmp.effect);

	assert(discardpile.size == 0);
	assert(player[0].stable.size == 6);
	assert(player[0].stable.num_unicorns == 5);
	assert(player[0].flags == tiny_stable);

	addStable(0, puppicorn_tmp);
	endOfTurn(0);

	if (player[0].stable.size != 5 || player[0].stable.num_unicorns != 4) {
		num_fails++;
		red();
		fprintf(stderr, "    puppicorn test: hand size failed\n");
		reset_col();
	}

	if (discardpile.size != 1 ||
			strcmp(discardpile.cards[0].name, basic_tmp2.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    puppicorn test: discard size failed\n");
		reset_col();
	}

	reset_players();
	reset_discard();
	return num_fails;
}

// do not have to sacrifice cards because they're pandas
int tiny_stable_pandamonium_check() {
	int num_fails = 0;
	struct Unicorn tiny_tmp = basedeck[113];
	struct Unicorn basic_tmp = basedeck[13];
	struct Unicorn panda_tmp = basedeck[107];

	// adding a unicorn card
	addStable(0, tiny_tmp);
	addStable(0, panda_tmp);
	addStable(0, basic_tmp);
	addStable(0, basic_tmp);
	addStable(0, basic_tmp);
	addStable(0, basic_tmp);
	addStable(0, basic_tmp);
	toggleFlags(0, tiny_tmp.effect);
	toggleFlags(0, panda_tmp.effect);

	assert(discardpile.size == 0);
	assert(player[0].stable.size == 7);
	assert(player[0].stable.num_unicorns == 5);
	assert((player[0].flags & tiny_stable) == tiny_stable);
	assert((player[0].flags & pandamonium) == pandamonium);

	addStable(0, basic_tmp);

	if (player[0].stable.size != 8 || player[0].stable.num_unicorns != 6) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test (unicorn): hand size failed\n");
		reset_col();
	}

	if (discardpile.size != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test (unicorn): discard size failed\n");
		reset_col();
	}

	reset_players();
	reset_discard();
	return num_fails;
}

// Tiny Stable
//
// If at any time you have more than 5 Unicorns in your Stable, SACRIFICE a Unicorn card.
int tiny_stable_tests() {
	int num_fails = 0;

	if (!isclient) {
		rainbow_error("\nStarting Tiny Stable tests...\n");

		// file input stream setup
		FILE* fp;
		fopen_s(&fp, "Tests/Input/line_1_1.txt", "r");
		if (fp == NULL) {
			magenta();
			fprintf(stderr, "    file input failed :(");
			reset_col();
			return 1;
		}
		fpinput = fp;

		num_fails += tiny_stable_basic_check();
		num_fails += tiny_stable_puppicorn_check();
		num_fails += tiny_stable_pandamonium_check();

		fclose(fp);
	}
	return num_fails;
}
