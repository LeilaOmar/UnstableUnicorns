#include "UpgradeTests.h"

// sanity test
int extra_tail_basic_check() {
	int num_fails = 0;
	struct Unicorn tail_tmp = basedeck[95];
	struct Unicorn basic_tmp = basedeck[13];

	addStable(0, basic_tmp);
	player[0].hand.cards[player[0].hand.num_cards++] = tail_tmp;

	int ret;
	assert(player[0].stable.size == 1);
	assert(player[0].hand.num_cards == 1);
	ret = conditionalEffects(0, tail_tmp, 0, 0);

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

	beginningTurnEffects(0, tail_tmp);

	if (player[0].stable.size != 2 || player[0].stable.num_unicorns != 1) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: (post-beginning turn) stable size failed\n");
		reset_col();
	}

	if (player[0].hand.num_cards != 1) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: (post-beginning turn) draw/hand size failed\n");
		reset_col();
	}

	reset_players();
	return num_fails;
}

// no basic unicorn in your stable
int extra_tail_empty_check() {
	int num_fails = 0;
	struct Unicorn tail_tmp = basedeck[95];

	player[0].hand.cards[player[0].hand.num_cards++] = tail_tmp;

	int ret;
	assert(discardpile.size == 0);
	assert(player[0].stable.size == 0);
	assert(player[0].hand.num_cards == 1);
	ret = conditionalEffects(0, tail_tmp, 0, 0);

	if (turn_count != 2 || ret != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    empty stable test: turn count failed\n");
		reset_col();
	}

	if (player[0].hand.num_cards != 1 ||
		strcmp(player[0].hand.cards[0].name, tail_tmp.name) != 0) {
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

// Extra Tail
//
// This card can only enter a Stable if there is a Basic Unicorn card in that Stable.
// If this card is in your Stable at the beginning of your turn, you may DRAW an extra card.
int extra_tail_tests() {
	int num_fails = 0;

	rainbow_error("\nStarting Extra Tail tests...\n");

	num_fails += extra_tail_basic_check();
	num_fails += extra_tail_empty_check();

	return num_fails;
}
