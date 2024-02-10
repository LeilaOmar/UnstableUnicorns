#include "MagicTests.h"

// sanity check
int twofer_basic_check() {
	int num_fails = 0;
	struct Unicorn twofer_tmp = basedeck[83];
	struct Unicorn basic_tmp = basedeck[13];
	struct Unicorn basic_tmp2 = basedeck[17];
	struct Unicorn basic_tmp3 = basedeck[20];
	struct Unicorn yay_tmp = basedeck[100];

	current_players = 3;
	addStable(0, basic_tmp);
	addStable(0, yay_tmp);
	addStable(1, basic_tmp2);
	addStable(1, basic_tmp3);
	player[0].hand.cards[player[0].hand.num_cards++] = twofer_tmp;

	int ret;
	assert(discardpile.size == 0);
	assert(player[0].hand.num_cards == 1);
	assert(player[0].stable.size == 2);
	assert(player[1].stable.size == 2);
	ret = conditionalEffects(0, twofer_tmp, 0, 0);

	if (turn_count != 1 || ret != 1) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: turn count failed\n");
		reset_col();
	}

	if (player[0].stable.size != 1 || player[0].stable.num_unicorns != 0 ||
			player[1].stable.size != 0 || player[1].stable.num_unicorns != 0 ) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: stable size failed\n");
		reset_col();
	}

	if (discardpile.size != 4 ||
			strcmp(discardpile.cards[0].name, twofer_tmp.name) != 0 ||
			strcmp(discardpile.cards[1].name, basic_tmp.name) != 0 ||
			strcmp(discardpile.cards[2].name, basic_tmp2.name) != 0 ||
			strcmp(discardpile.cards[3].name, basic_tmp3.name) != 0) {
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

// no cards to sacrifice or destroy
int twofer_empty_check() {
	int num_fails = 0;
	struct Unicorn twofer_tmp = basedeck[83];
	struct Unicorn puppicorn_tmp = basedeck[41]; // cannot be destroyed, periodt.
	struct Unicorn kitten_tmp = basedeck[39]; // cannot be destroyed by magic cards
	struct Unicorn basic_tmp = basedeck[13]; // [emojis]

	// no cards to sacrifice
	current_players = 2;
	addStable(0, puppicorn_tmp);
	addStable(1, basic_tmp);
	addStable(1, basic_tmp);
	player[0].hand.cards[player[0].hand.num_cards++] = twofer_tmp;

	int ret;
	assert(discardpile.size == 0);
	assert(player[0].hand.num_cards == 1);
	assert(player[0].stable.size == 1);
	assert(player[1].stable.size == 2);
	ret = conditionalEffects(0, twofer_tmp, 0, 0);

	if (turn_count != 2 || ret != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    empty sacrifice test: turn count failed\n");
		reset_col();
	}

	if (player[0].hand.num_cards != 1 ||
			strcmp(player[0].hand.cards[0].name, twofer_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    empty sacrifice test: hand verification failed\n");
		reset_col();
	}

	if (player[0].stable.size != 1 || player[0].stable.num_unicorns != 1 ||
			player[1].stable.size != 2 || player[1].stable.num_unicorns != 2 ||
			strcmp(player[0].stable.unicorns[0].name, puppicorn_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    empty sacrifice test: stable verification failed\n");
		reset_col();
	}

	if (discardpile.size != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    empty sacrifice test: discard size failed\n");
		reset_col();
	}
	reset_players();
	reset_discard();

	// no cards to destroy
	current_players = 2;
	addStable(0, basic_tmp);
	addStable(1, puppicorn_tmp);
	addStable(1, kitten_tmp);
	player[0].hand.cards[player[0].hand.num_cards++] = twofer_tmp;

	assert(discardpile.size == 0);
	assert(player[0].hand.num_cards == 1);
	assert(player[0].stable.size == 1);
	assert(player[1].stable.size == 2);
	ret = conditionalEffects(0, twofer_tmp, 0, 0);

	if (turn_count != 2 || ret != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    empty destroy test: turn count failed\n");
		reset_col();
	}

	if (player[0].hand.num_cards != 1 ||
			strcmp(player[0].hand.cards[0].name, twofer_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    empty destroy test: hand verification failed\n");
		reset_col();
	}

	if (player[0].stable.size != 1 || player[0].stable.num_unicorns != 1 ||
			player[1].stable.size != 2 || player[1].stable.num_unicorns != 2 ||
			strcmp(player[0].stable.unicorns[0].name, basic_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    empty destroy test: stable verification failed\n");
		reset_col();
	}

	if (discardpile.size != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    empty destroy test: discard size failed\n");
		reset_col();
	}
	reset_players();
	reset_discard();

	// only one card available to destroy
	current_players = 2;
	addStable(0, kitten_tmp);
	addStable(1, puppicorn_tmp);
	addStable(1, basic_tmp);
	player[0].hand.cards[player[0].hand.num_cards++] = twofer_tmp;

	assert(discardpile.size == 0);
	assert(player[0].hand.num_cards == 1);
	assert(player[0].stable.size == 1);
	assert(player[1].stable.size == 2);
	ret = conditionalEffects(0, twofer_tmp, 0, 0);

	if (turn_count != 2 || ret != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    incomplete destroy test: turn count failed\n");
		reset_col();
	}

	if (player[0].hand.num_cards != 1 ||
			strcmp(player[0].hand.cards[0].name, twofer_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    incomplete destroy test: hand verification failed\n");
		reset_col();
	}

	if (player[0].stable.size != 1 || player[0].stable.num_unicorns != 1 ||
			player[1].stable.size != 2 || player[1].stable.num_unicorns != 2 ||
			strcmp(player[0].stable.unicorns[0].name, kitten_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    incomplete destroy test: stable verification failed\n");
		reset_col();
	}

	if (discardpile.size != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    incomplete destroy test: discard size failed\n");
		reset_col();
	}

	reset_players();
	reset_discard();
	return num_fails;
}

// unique edge-case where cards like Unicorn Phoenix can bypass destruction,
// so two-for-one probably shouldn't activate
int twofer_unicorn_phoenix_check() {
	int num_fails = 0;
	struct Unicorn twofer_tmp = basedeck[83];
	struct Unicorn phoenix_tmp = basedeck[57];
	struct Unicorn kitten_tmp = basedeck[39];
	struct Unicorn basic_tmp = basedeck[13];

	// 1 card can be destroyed, 1 card has a special effect that allows it
	// to bypass getting destroyed
	current_players = 2;
	addStable(0, kitten_tmp);
	addStable(1, phoenix_tmp);
	addStable(1, basic_tmp);
	player[0].hand.cards[player[0].hand.num_cards++] = twofer_tmp;
	player[1].hand.cards[player[1].hand.num_cards++] = twofer_tmp;

	int ret;
	assert(discardpile.size == 0);
	assert(player[0].hand.num_cards == 1);
	assert(player[0].stable.size == 1);
	assert(player[1].stable.size == 2);
	assert(player[1].hand.num_cards == 1);
	ret = conditionalEffects(0, twofer_tmp, 0, 0);

	if (turn_count != 2 || ret != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    unicorn phoenix test: turn count failed\n");
		reset_col();
	}

	if (player[0].hand.num_cards != 1 ||
			strcmp(player[0].hand.cards[0].name, twofer_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    unicorn phoenix test: hand verification failed\n");
		reset_col();
	}

	if (player[0].stable.size != 1 || player[0].stable.num_unicorns != 1 ||
			player[1].stable.size != 2 || player[1].stable.num_unicorns != 2 ||
			strcmp(player[0].stable.unicorns[0].name, kitten_tmp.name) != 0 ||
			strcmp(player[1].stable.unicorns[0].name, phoenix_tmp.name) != 0 ||
			strcmp(player[1].stable.unicorns[1].name, basic_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    unicorn phoenix test: stable verification failed\n");
		reset_col();
	}

	if (discardpile.size != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    unicorn phoenix test: discard size failed\n");
		reset_col();
	}

	reset_players();
	reset_discard();
	return num_fails;
}

// Two-For-One
//
// SACRIFICE a card, then DESTROY 2 cards.
int two_for_one_tests() {
	int num_fails = 0;

	if (!isclient) {
		rainbow_error("\nStarting Two-For-One tests...\n");

		// file input stream setup
		FILE* fp;
		fopen_s(&fp, "Tests/Input/twoforone.txt", "r");
		if (fp == NULL) {
			magenta();
			fprintf(stderr, "    file input failed :(");
			reset_col();
			return 1;
		}
		fpinput = fp;

		num_fails += twofer_basic_check();
		num_fails += twofer_empty_check();
		num_fails += twofer_unicorn_phoenix_check();

		fclose(fp);
	}
	return num_fails;
}
