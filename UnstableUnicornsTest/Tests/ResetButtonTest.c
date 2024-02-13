#include "MagicTests.h"

// sanity test
int reset_button_basic_check() {
	int num_fails = 0;
	struct Unicorn reset_tmp = basedeck[91];
	struct Unicorn yay_tmp = basedeck[100];
	struct Unicorn rainaura_tmp = basedeck[103];
	struct Unicorn dutch_tmp = basedeck[104];
	struct Unicorn barbed_tmp = basedeck[106];
	struct Unicorn panda_tmp = basedeck[107];
	struct Unicorn basic_tmp = basedeck[13];

	current_players = 3;
	addStable(0, yay_tmp);
	addStable(0, barbed_tmp);

	addStable(1, basic_tmp);

	addStable(2, rainaura_tmp);
	addStable(2, dutch_tmp);
	addStable(2, panda_tmp);
	addStable(2, basic_tmp);
	player[0].hand.cards[player[0].hand.num_cards++] = reset_tmp;

	deck.size -= 7; // to accomodate for the added stable cards and reset button
	int tmp_size = deck.size;

	int ret;
	assert(discardpile.size == 0);
	assert(player[0].stable.size == 2);
	assert(player[1].stable.size == 1);
	assert(player[2].stable.size == 4);
	assert(player[0].hand.num_cards == 1);

	assert((player[0].flags & yay) == yay);
	assert((player[0].flags & barbed_wire) == barbed_wire);
	assert((player[2].flags & rainbow_aura) == rainbow_aura);
	assert((player[2].flags & pandamonium) == pandamonium);
	ret = conditionalEffects(0, reset_tmp, 0, 0);
	if (ret) magicEffects(0, reset_tmp.effect);

	if (turn_count != 1 || ret != 1) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: turn count failed\n");
		reset_col();
	}

	if (player[0].stable.size != 0 ||
			player[1].stable.size != 1 || player[1].stable.num_unicorns != 1 ||
			player[2].stable.size != 1 || player[2].stable.num_unicorns != 1) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: stable size failed\n");
		reset_col();
	}

	// discard pile gets shuffled into the deck
	if (discardpile.size != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: discard size failed\n");
		reset_col();
	}

	// 1 reset button + 2 downgrade cards + 3 upgrade cards = 6 (in-game)
	// this actually isn't including reset button though, so it should check for 5
	if (deck.size != (tmp_size + 5)) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: deck size failed\n");
		reset_col();
	}

	// flags should be gone
	if (player[0].flags != 0 ||
			player[2].flags != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: toggle flags failed\n");
		reset_col();
	}

	reset_players();
	reset_deck();
	reset_discard();
	return num_fails;
}

// no downgrade cards in player[0]'s stable
int reset_button_empty_upgrade_check() {
	int num_fails = 0;
	struct Unicorn reset_tmp = basedeck[91];
	struct Unicorn yay_tmp = basedeck[100];
	struct Unicorn barbed_tmp = basedeck[106];
	struct Unicorn basic_tmp = basedeck[13];

	// no upgrade
	current_players = 2;
	addStable(0, basic_tmp);
	addStable(0, barbed_tmp);
	addStable(1, yay_tmp);
	addStable(1, barbed_tmp);
	player[0].hand.cards[player[0].hand.num_cards++] = reset_tmp;

	deck.size -= 5; // to accomodate for the added stable cards and reset button
	int tmp_size = deck.size;

	int ret;
	assert(player[0].stable.size == 2);
	assert(player[1].stable.size == 2);
	ret = conditionalEffects(0, reset_tmp, 0, 0);

	if (turn_count != 2 || ret != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    empty upgrade test (player[0]): turn count failed\n");
		reset_col();
	}

	reset_players();
	reset_deck();
	reset_discard();
	return num_fails;
}

// no downgrade cards in player[0]'s stable
int reset_button_empty_downgrade_check() {
	int num_fails = 0;
	struct Unicorn reset_tmp = basedeck[91];
	struct Unicorn yay_tmp = basedeck[100];
	struct Unicorn barbed_tmp = basedeck[106];
	struct Unicorn basic_tmp = basedeck[13];

	// no upgrade
	current_players = 3;
	addStable(0, basic_tmp);
	addStable(0, yay_tmp);
	addStable(1, yay_tmp);
	addStable(1, barbed_tmp);
	player[0].hand.cards[player[0].hand.num_cards++] = reset_tmp;

	int ret;
	assert(player[0].stable.size == 2);
	assert(player[0].stable.num_unicorns == 1);
	assert(player[1].stable.size == 2);
	assert(player[1].stable.num_unicorns == 0);
	ret = conditionalEffects(0, reset_tmp, 0, 0);

	if (turn_count != 2 || ret != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    empty downgrade test (player[0]): turn count failed\n");
		reset_col();
	}

	reset_players();
	reset_deck();
	reset_discard();
	return num_fails;
}

// no upgrade or downgrade cards in other players' stables
int reset_button_empty_other_check() {
	int num_fails = 0;
	struct Unicorn reset_tmp = basedeck[91];
	struct Unicorn yay_tmp = basedeck[100];
	struct Unicorn barbed_tmp = basedeck[106];
	struct Unicorn basic_tmp = basedeck[13];

	// no upgrade
	current_players = 2;
	addStable(0, yay_tmp);
	addStable(0, barbed_tmp);
	addStable(1, basic_tmp);
	player[0].hand.cards[player[0].hand.num_cards++] = reset_tmp;

	deck.size -= 4; // to accomodate for the added stable cards and reset button
	int tmp_size = deck.size;

	int ret;
	assert(player[0].stable.size == 2);
	assert(player[1].stable.size == 1);
	ret = conditionalEffects(0, reset_tmp, 0, 0);

	if (turn_count != 2 || ret != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    empty stable test (player[1]): turn count failed\n");
		reset_col();
	}

	reset_players();
	reset_deck();
	reset_discard();
	return num_fails;
}

// Reset Button
//
// Each player (including you) must SACRIFICE all Upgrade and Downgrade cards
// in their Stable. Shuffle the discard pile into the deck.
int reset_button_tests() {
	int num_fails = 0;

	if (!isclient) {
		rainbow_error("\nStarting Reset Button tests...\n");

		num_fails += reset_button_basic_check();
		num_fails += reset_button_empty_upgrade_check();
		num_fails += reset_button_empty_downgrade_check();
		num_fails += reset_button_empty_other_check();
	}
	return num_fails;
}
