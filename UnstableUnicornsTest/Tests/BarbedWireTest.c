#include "DowngradeTests.h"

// tests for when unicorns enter the stable
int barbed_enter_check() {
	int num_fails = 0;
	struct Unicorn barbed_tmp = basedeck[106];
	struct Unicorn basic_tmp = basedeck[13];
	struct Unicorn ginormous_tmp = basedeck[48];

	addStable(0, barbed_tmp);
	player[0].hand.cards[player[0].hand.num_cards++] = basic_tmp;

	assert(discardpile.size == 0);
	assert(player[0].hand.num_cards == 1);
	assert(player[0].stable.size == 1);
	assert(player[0].flags == barbed_wire);

	addStable(0, ginormous_tmp);

	if (player[0].hand.num_cards != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    enter test: hand size failed\n");
		reset_col();
	}

	if (player[0].stable.size != 2 || player[0].stable.num_unicorns != 1 ||
			strcmp(player[0].stable.unicorns[0].name, barbed_tmp.name) != 0 ||
			strcmp(player[0].stable.unicorns[1].name, ginormous_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    enter test: stable verification failed\n");
		reset_col();
	}

	if (discardpile.size != 1 ||
			strcmp(discardpile.cards[0].name, basic_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    enter test: discard pile verification failed\n");
		reset_col();
	}
	reset_players();
	reset_discard();

	// make sure it doesn't break if there are no cards to discard (a bit redundant given discard tests)
	addStable(0, barbed_tmp);

	assert(discardpile.size == 0);
	assert(player[0].hand.num_cards == 0);
	assert(player[0].stable.size == 1);
	assert(player[0].flags == barbed_wire);

	addStable(0, ginormous_tmp);

	if (player[0].hand.num_cards != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    enter test (empty hand): hand size failed\n");
		reset_col();
	}

	if (player[0].stable.size != 2 || player[0].stable.num_unicorns != 1 ||
			strcmp(player[0].stable.unicorns[0].name, barbed_tmp.name) != 0 ||
			strcmp(player[0].stable.unicorns[1].name, ginormous_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    enter test (empty hand): stable verification failed\n");
		reset_col();
	}

	if (discardpile.size != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    enter test (empty hand): discard pile size failed\n");
		reset_col();
	}

	reset_players();
	reset_discard();
	return num_fails;
}

// tests for when unicorns leave the stable
int barbed_exit_check() {
	int num_fails = 0;
	struct Unicorn barbed_tmp = basedeck[106];
	struct Unicorn basic_tmp = basedeck[13];
	struct Unicorn ginormous_tmp = basedeck[48];

	addStable(0, barbed_tmp);
	addStable(0, ginormous_tmp);
	player[0].hand.cards[player[0].hand.num_cards++] = basic_tmp;

	assert(discardpile.size == 0);
	assert(player[0].hand.num_cards == 1);
	assert(player[0].stable.size == 2);
	assert((player[0].flags & barbed_wire) == barbed_wire);

	// player[0][1] == ginourmous_tmp
	rearrangeStable(0, 1);

	if (player[0].hand.num_cards != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    exit test: hand size failed\n");
		reset_col();
	}

	if (player[0].stable.size != 1 || player[0].stable.num_unicorns != 0 ||
			strcmp(player[0].stable.unicorns[0].name, barbed_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    exit test: stable verification failed\n");
		reset_col();
	}

	if (discardpile.size != 1 ||
			strcmp(discardpile.cards[0].name, basic_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    exit test: discard pile verification failed\n");
		reset_col();
	}
	reset_players();
	reset_discard();

	// make sure it doesn't break if there are no cards to discard (a bit redundant given discard tests)
	addStable(0, barbed_tmp);
	addStable(0, ginormous_tmp);

	assert(discardpile.size == 0);
	assert(player[0].hand.num_cards == 0);
	assert(player[0].stable.size == 2);
	assert((player[0].flags & barbed_wire) == barbed_wire);

	// player[0][1] == ginourmous_tmp
	rearrangeStable(0, 1);

	if (player[0].hand.num_cards != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    exit test (empty hand): hand size failed\n");
		reset_col();
	}

	if (player[0].stable.size != 1 || player[0].stable.num_unicorns != 0 ||
			strcmp(player[0].stable.unicorns[0].name, barbed_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    exit test (empty hand): stable verification failed\n");
		reset_col();
	}

	if (discardpile.size != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    exit test (empty hand): discard pile verification failed\n");
		reset_col();
	}

	reset_players();
	reset_discard();
	return num_fails;
}

// compatibility w/ pandamonium;
// enter - should discard because they are entering as unicorn CARDS; did not "cross the gate" yet so to speak
// exit - should not discard because they are pandas, not unicorns
int barbed_pandamonium_check() {
	int num_fails = 0;
	struct Unicorn barbed_tmp = basedeck[106];
	struct Unicorn basic_tmp = basedeck[13];
	struct Unicorn ginormous_tmp = basedeck[48];
	struct Unicorn panda_tmp = basedeck[107];

	// entering the stable
	addStable(0, barbed_tmp);
	addStable(0, panda_tmp);
	player[0].hand.cards[player[0].hand.num_cards++] = basic_tmp;

	assert(discardpile.size == 0);
	assert(player[0].hand.num_cards == 1);
	assert(player[0].stable.size == 2);
	assert((player[0].flags & barbed_wire) == barbed_wire);
	assert((player[0].flags & pandamonium) == pandamonium);

	addStable(0, ginormous_tmp);

	if (player[0].hand.num_cards != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    pandamonium enter test: hand size failed\n");
		reset_col();
	}

	if (player[0].stable.size != 3 || player[0].stable.num_unicorns != 1 ||
			strcmp(player[0].stable.unicorns[0].name, barbed_tmp.name) != 0 ||
			strcmp(player[0].stable.unicorns[1].name, panda_tmp.name) != 0 ||
			strcmp(player[0].stable.unicorns[2].name, ginormous_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    pandamonium enter test: stable verification failed\n");
		reset_col();
	}

	if (discardpile.size != 1 ||
			strcmp(discardpile.cards[0].name, basic_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    pandamonium enter test: discard pile verification failed\n");
		reset_col();
	}
	reset_players();
	reset_discard();

	// leaving the stable
	addStable(0, barbed_tmp);
	addStable(0, panda_tmp);
	addStable(0, ginormous_tmp);

	assert(discardpile.size == 0);
	assert(player[0].hand.num_cards == 0);
	assert(player[0].stable.size == 3);
	assert((player[0].flags & barbed_wire) == barbed_wire);
	assert((player[0].flags & pandamonium) == pandamonium);

	// player[0][2] == ginourmous_tmp
	rearrangeStable(0, 2);

	if (player[0].hand.num_cards != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    pandamonium exit test: hand size failed\n");
		reset_col();
	}

	if (player[0].stable.size != 2 || player[0].stable.num_unicorns != 0 ||
			strcmp(player[0].stable.unicorns[0].name, barbed_tmp.name) != 0 ||
			strcmp(player[0].stable.unicorns[1].name, panda_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    pandamonium exit test: stable verification failed\n");
		reset_col();
	}

	if (discardpile.size != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    pandamonium exit test: discard pile verification failed\n");
		reset_col();
	}

	reset_players();
	reset_discard();
	return num_fails;
}

// Barbed Wire
//
// Each time a Unicorn card enters or leaves your Stable, DISCARD a card.
int barbed_wire_tests() {
	int num_fails = 0;

	if (!isclient) {
		rainbow_error("\nStarting Barbed Wire tests...\n");

		// file input stream setup
		FILE* fp;
		fopen_s(&fp, "Tests/Input/barbedwire.txt", "r");
		if (fp == NULL) {
			magenta();
			fprintf(stderr, "    file input failed :(");
			reset_col();
			return 1;
		}
		fpinput = fp;

		// already tested it w/ unicorn shrinkray
		num_fails += barbed_enter_check();
		num_fails += barbed_exit_check();
		num_fails += barbed_pandamonium_check();

		fclose(fp);
	}
	return num_fails;
}
