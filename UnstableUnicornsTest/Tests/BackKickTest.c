#include "MagicTests.h"

// sanity check
int back_kick_basic_check() {
	int num_fails = 0;
	struct Unicorn back_kick_tmp = basedeck[70];
	struct Unicorn rainbow_tmp = basedeck[42];
	struct Unicorn barbed_tmp = basedeck[106];
	struct Unicorn basic_tmp = basedeck[13];
	struct Unicorn basic_tmp2 = basedeck[17];

	current_players = 2;
	addStable(1, rainbow_tmp);
	addStable(1, basic_tmp);
	addStable(1, barbed_tmp);
	player[0].hand.cards[player[0].hand.num_cards++] = back_kick_tmp;
	player[1].hand.cards[player[1].hand.num_cards++] = basic_tmp2;

	assert(discardpile.size == 0);
	assert(player[0].hand.num_cards == 1);
	assert(player[1].hand.num_cards == 1);
	assert(player[1].stable.size == 3);
	assert(player[1].stable.num_unicorns == 2);
	conditionalEffects(0, back_kick_tmp, 0, 0);

	if (player[0].hand.num_cards != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: player[0] hand size failed\n");
		reset_col();
	}

	if (player[1].hand.num_cards != 1) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: player[1] hand size failed\n");
		reset_col();
	}

	if (player[1].stable.size != 2 || player[1].stable.num_unicorns != 1 ||
			strcmp(player[1].stable.unicorns[1].name, barbed_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: stable verification failed\n");
		reset_col();
	}

	if (discardpile.size != 2 ||
			strcmp(discardpile.cards[0].name, back_kick_tmp.name) != 0 ||
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

// check to make sure toggles are applied
int back_kick_special_check() {
	int num_fails = 0;
	struct Unicorn back_kick_tmp = basedeck[70];
	struct Unicorn yay_tmp = basedeck[100];

	current_players = 2;
	player[0].hand.cards[player[0].hand.num_cards++] = back_kick_tmp;
	addStable(1, yay_tmp);
	toggleFlags(1, yay_effect);

	assert(player[0].hand.num_cards == 1);
	assert((player[1].flags & yay) == yay);
	conditionalEffects(0, back_kick_tmp, 0, 0);


	if ((player[1].flags & yay) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    toggle test: yay flag toggle failed\n");
		reset_col();
	}

	reset_players();
	reset_discard();
	return num_fails;
}

// no cards in hand before the effect takes place
int back_kick_empty_hand_check() {
	int num_fails = 0;
	struct Unicorn back_kick_tmp = basedeck[70];
	struct Unicorn basic_tmp = basedeck[13];

	current_players = 2;
	addStable(1, basic_tmp);
	player[0].hand.cards[player[0].hand.num_cards++] = back_kick_tmp;

	assert(discardpile.size == 0);
	assert(player[0].hand.num_cards == 1);
	assert(player[1].hand.num_cards == 0);
	assert(player[1].stable.size == 1);
	conditionalEffects(0, back_kick_tmp, 0, 0);


	if (player[1].hand.num_cards != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    empty hand test: player[1] hand size failed\n");
		reset_col();
	}

	if (player[1].stable.size != 0 || player[1].stable.num_unicorns != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    empty hand test: stable size failed\n");
		reset_col();
	}

	if (discardpile.size != 2 ||
		strcmp(discardpile.cards[0].name, back_kick_tmp.name) != 0 ||
		strcmp(discardpile.cards[1].name, basic_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    empty hand test: discard verification failed\n");
		reset_col();
	}

	reset_players();
	reset_discard();
	return num_fails;
}

// no cards in stable before the effect takes place
int back_kick_empty_stable_check() {
	int num_fails = 0;
	struct Unicorn back_kick_tmp = basedeck[70];

	current_players = 2;
	player[0].hand.cards[player[0].hand.num_cards++] = back_kick_tmp;

	int ret;
	assert(player[0].hand.num_cards == 1);
	assert(player[1].stable.size == 0);
	assert(player[1].stable.num_unicorns == 0);
	ret = conditionalEffects(0, back_kick_tmp, 0, 0);

	if (turn_count != 2 || ret != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    empty stable test: turn count failed\n");
		reset_col();
	}

	if (player[0].hand.num_cards != 1 ||
			strcmp(player[0].hand.cards[0].name, back_kick_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    empty stable test: hand verification failed\n");
		reset_col();
	}

	if (discardpile.size != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    empty stable test: discard size failed\n");
		reset_col();
	}

	reset_players();
	reset_discard();
	return num_fails;
}

// Back Kick
//
// Return a card in another player's Stable to their hand.
// That player must DISCARD a card.
int back_kick_tests() {
	int num_fails = 0;

	rainbow_error("\nStarting Back Kick tests...\n");

	// file input stream setup
	FILE* fp;
	fopen_s(&fp, "Tests/Input/backkick.txt", "r");
	if (fp == NULL) {
		magenta();
		fprintf(stderr, "    file input failed :(");
		reset_col();
		return 1;
	}
	fpinput = fp;

	num_fails += back_kick_basic_check();
	num_fails += back_kick_special_check();
	num_fails += back_kick_empty_hand_check();
	num_fails += back_kick_empty_stable_check();

	fclose(fp);
	return num_fails;
}
