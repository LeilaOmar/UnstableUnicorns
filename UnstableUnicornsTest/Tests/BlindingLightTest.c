#include "DowngradeTests.h"

// checking for beginning of turn effects
int blinding_beginning_turn_check() {
	int num_fails = 0;
	struct Unicorn blinding_tmp = basedeck[112];
	struct Unicorn fertile_tmp = basedeck[38];
	struct Unicorn dutch_tmp = basedeck[104];

	// test w/ unicorn card
	draw(0, 1);
	int tmp_size = nursery.size;
	addStable(0, blinding_tmp);
	addStable(0, fertile_tmp);
	toggleFlags(0, blinding_tmp.effect);

	assert(discardpile.size == 0);
	assert(player[0].hand.num_cards == 1);
	assert(player[0].flags == blinding_light);

	// can't discard to bring baby unicorns into stable
	beginningTurnEffects(0, fertile_tmp);

	if (player[0].hand.num_cards != 1) {
		num_fails++;
		red();
		fprintf(stderr, "    beginning turn (unicorn) test: hand size failed\n");
		reset_col();
	}

	if (player[0].stable.size != 2 || player[0].stable.num_unicorns != 1) {
		num_fails++;
		red();
		fprintf(stderr, "    beginning turn (unicorn) test: hand size failed\n");
		reset_col();
	}

	if (discardpile.size != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    beginning turn (unicorn) test: discard size failed\n");
		reset_col();
	}

	if (nursery.size != tmp_size) {
		num_fails++;
		red();
		fprintf(stderr, "    beginning turn (unicorn) test: nursery size failed\n");
		reset_col();
	}
	reset_players();
	reset_deck();
	reset_nursery();
	reset_discard();

	// test w/ non-unicorn card
	addStable(0, dutch_tmp);
	addStable(0, blinding_tmp);
	toggleFlags(0, blinding_tmp.effect);
	assert(player[0].flags == blinding_light);

	beginningTurnEffects(0, dutch_tmp);

	if (turn_count != 2) {
		num_fails++;
		red();
		fprintf(stderr, "    beginning turn (non-unicorn) test: turn count failed\n");
		reset_col();
	}

	reset_players();
	return num_fails;
}

// checking for enter stable effects
int blinding_enter_stable_check() {
	int num_fails = 0;
	struct Unicorn blinding_tmp = basedeck[112];
	struct Unicorn llama_tmp = basedeck[46];
	struct Unicorn tail_tmp = basedeck[95];

	// test w/ unicorn card
	draw(0, 1);
	addStable(0, blinding_tmp);
	toggleFlags(0, blinding_tmp.effect);

	assert(discardpile.size == 0);
	assert(player[0].hand.num_cards == 1);
	assert(player[0].flags == blinding_light);

	// can't discard cards
	enterStableEffects(0, llama_tmp.effect);

	if (player[0].hand.num_cards != 1) {
		num_fails++;
		red();
		fprintf(stderr, "    enter stable (unicorn) test: hand size failed\n");
		reset_col();
	}

	if (discardpile.size != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    enter stable (unicorn) test: discard size failed\n");
		reset_col();
	}

	reset_players();
	reset_deck();
	reset_discard();

	// test w/ non-unicorn card; tail still can't enter
	addStable(0, blinding_tmp);
	toggleFlags(0, blinding_tmp.effect);
	player[0].hand.cards[player[0].hand.num_cards++] = tail_tmp;

	int ret;
	assert(discardpile.size == 0);
	assert(player[0].hand.num_cards == 1);
	assert(player[0].flags == blinding_light);
	ret = conditionalEffects(0, tail_tmp, 0, 0);

	if (turn_count != 2 || ret != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    enter stable (non-unicorn) test: turn count failed\n");
		reset_col();
	}

	if (player[0].hand.num_cards != 1 ||
			strcmp(player[0].hand.cards[0].name, tail_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    enter stable (non-unicorn) test: hand verification failed\n");
		reset_col();
	}

	reset_players();
	return num_fails;
}

// checking for sacrifice & destroy effects
int blinding_sacrifice_destroy_check() {
	int num_fails = 0;
	struct Unicorn blinding_tmp = basedeck[112];
	struct Unicorn majestic_tmp = basedeck[56];

	addStable(0, majestic_tmp);
	addStable(0, blinding_tmp);
	toggleFlags(0, blinding_tmp.effect);

	assert(discardpile.size == 0);
	assert(player[0].hand.num_cards == 0);
	assert(player[0].flags == blinding_light);

	// card doesn't return to hand
	// player[0][0] == majestic_tmp
	sacrificeDestroyEffects(0, 0, majestic_tmp.effect);

	if (player[0].hand.num_cards != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    sacrifice test: hand size failed\n");
		reset_col();
	}

	if (discardpile.size != 1 ||
		strcmp(discardpile.cards[0].name, majestic_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    sacrifice test: discard pile verification failed\n");
		reset_col();
	}

	reset_players();
	reset_discard();
	return num_fails;
}

// blinding light and pandamonium
//	- entering stable (no effect)
//	- beginning of next turn (effect should work)
//	- sacrificing/destroying (effect should work)
int blinding_pandamonium_check() {
	int num_fails = 0;
	struct Unicorn blinding_tmp = basedeck[112];
	struct Unicorn panda_tmp = basedeck[107];

	// entering the stable
	struct Unicorn llama_tmp = basedeck[46];

	addStable(0, blinding_tmp);
	addStable(0, panda_tmp);
	toggleFlags(0, blinding_tmp.effect);
	toggleFlags(0, panda_tmp.effect);

	draw(0, 1);
	int tmp_size_deck = deck.size;

	assert(discardpile.size == 0);
	assert(player[0].hand.num_cards == 1);
	assert((player[0].flags & blinding_light) == blinding_light);
	assert((player[0].flags & pandamonium) == pandamonium);

	// can't discard cards
	enterStableEffects(0, llama_tmp.effect);

	if (player[0].hand.num_cards != 1) {
		num_fails++;
		red();
		fprintf(stderr, "    pandamonium enter stable test: hand size failed\n");
		reset_col();
	}

	if (discardpile.size != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    pandamoniums enter stable test: discard size failed\n");
		reset_col();
	}

	reset_players();
	reset_deck();
	reset_discard();

	// beginning of turn
	struct Unicorn fertile_tmp = basedeck[38];

	draw(0, 1);
	int tmp_size = nursery.size;
	addStable(0, blinding_tmp);
	addStable(0, fertile_tmp);
	addStable(0, panda_tmp);
	toggleFlags(0, blinding_tmp.effect);
	toggleFlags(0, panda_tmp.effect);

	assert(discardpile.size == 0);
	assert(player[0].hand.num_cards == 1);
	assert(player[0].stable.size == 3);
	assert((player[0].flags & blinding_light) == blinding_light);
	assert((player[0].flags & pandamonium) == pandamonium);

	// should be able to discard to bring baby unicorns into stable
	beginningTurnEffects(0, fertile_tmp);

	if (player[0].hand.num_cards != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    pandamonium beginning turn test: hand size failed\n");
		reset_col();
	}

	if (player[0].stable.size != 4 || player[0].stable.num_unicorns != 2) {
		num_fails++;
		red();
		fprintf(stderr, "    pandamonium beginning turn test: stable size failed\n");
		reset_col();
	}

	if (discardpile.size != 1) {
		num_fails++;
		red();
		fprintf(stderr, "    pandamonium beginning turn test: discard size failed\n");
		reset_col();
	}

	if (nursery.size != (tmp_size - 1)) {
		num_fails++;
		red();
		fprintf(stderr, "    pandamonium beginning turn test: nursery size failed\n");
		reset_col();
	}
	reset_players();
	reset_deck();
	reset_nursery();
	reset_discard();

	// card does return to hand because they're actually pandas
	struct Unicorn majestic_tmp = basedeck[56];

	addStable(0, majestic_tmp);
	addStable(0, blinding_tmp);
	addStable(0, panda_tmp);
	toggleFlags(0, blinding_tmp.effect);
	toggleFlags(0, panda_tmp.effect);

	assert(discardpile.size == 0);
	assert(player[0].hand.num_cards == 0);
	assert((player[0].flags & blinding_light) == blinding_light);
	assert((player[0].flags & pandamonium) == pandamonium);

	// player[0][0] == majestic_tmp
	sacrificeDestroyEffects(0, 0, majestic_tmp.effect);

	if (player[0].hand.num_cards != 1 ||
			strcmp(player[0].hand.cards[0].name, majestic_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    pandamonium sacrifice test: hand verification failed\n");
		reset_col();
	}

	if (player[0].stable.size != 2 || player[0].stable.num_unicorns != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    pandamonium sacrifice test: stable size failed\n");
		reset_col();
	}

	if (discardpile.size != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    pandamonium sacrifice test: discard pile size failed\n");
		reset_col();
	}

	reset_players();
	reset_discard();
	return num_fails;
}

// check for the win condition w/ ginormous unicorn
int blinding_ginormous_check() {
	int num_fails = 0;
	struct Unicorn blinding_tmp = basedeck[112];
	struct Unicorn ginormous_tmp = basedeck[48];
	struct Unicorn basic_tmp = basedeck[13];

	addStable(0, blinding_tmp);
	addStable(0, basic_tmp);
	addStable(0, basic_tmp);
	addStable(0, basic_tmp);
	addStable(0, basic_tmp);
	addStable(0, basic_tmp);
	toggleFlags(0, blinding_tmp.effect);

	assert((player[0].flags & blinding_light) == blinding_light);

	addStable(0, ginormous_tmp);
	enterStableEffects(0, ginormous_tmp.effect);

	assert(discardpile.size == 0);
	assert(player[0].stable.num_unicorns == 6);
	assert((player[0].flags & ginormous_unicorn) == ginormous_unicorn);

	// player shouldn't win; i.e. endOfTurn returns 0 instead of 1
	int ret = endOfTurn(0);

	if (ret != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    ginormous unicorn test: (pre-flag) win check failed\n");
		reset_col();
	}

	// taking away blinding light should allow the check to pass now

	// player[0][0] == blinding_tmp
	rearrangeStable(0, 0);
	ret = endOfTurn(0);

	if (ret != 1) {
		num_fails++;
		red();
		fprintf(stderr, "    ginormous unicorn test: (post-flag) win check failed\n");
		reset_col();
	}

	reset_players();
	reset_discard();
	return num_fails;
}

// Blinding Light
//
// All of your Unicorn cards are considered Basic Unicorns with no effects.
int blinding_light_tests() {
	int num_fails = 0;

	rainbow_error("\nStarting Blinding Light tests...\n");

	// file input stream setup
	FILE* fp;
	fopen_s(&fp, "Tests/Input/blindinglight.txt", "r");
	if (fp == NULL) {
		magenta();
		fprintf(stderr, "    file input failed :(");
		reset_col();
		return 1;
	}
	fpinput = fp;

	num_fails += blinding_beginning_turn_check();
	num_fails += blinding_enter_stable_check();
	num_fails += blinding_sacrifice_destroy_check();
	num_fails += blinding_pandamonium_check();
	num_fails += blinding_ginormous_check();

	fclose(fp);
	return num_fails;
}
