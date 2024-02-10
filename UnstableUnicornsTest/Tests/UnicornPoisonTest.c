#include "MagicTests.h"

// sanity check
int poison_basic_check() {
	int num_fails = 0;
	struct Unicorn poison_tmp = basedeck[67];
	struct Unicorn yay_tmp = basedeck[100];
	struct Unicorn basic_tmp = basedeck[13];

	current_players = 2;
	addStable(1, yay_tmp);
	addStable(1, basic_tmp);
	player[0].hand.cards[player[0].hand.num_cards++] = poison_tmp;

	int ret;
	assert(turn_count == 1);
	assert(discardpile.size == 0);
	assert(player[0].hand.num_cards == 1);
	ret = conditionalEffects(0, poison_tmp, 0, 0);

	if (turn_count != 1 || ret != 1) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: turn count failed\n");
		reset_col();
	}

	if (player[0].hand.num_cards != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: hand size failed\n");
		reset_col();
	}

	if (player[1].stable.size != 1 || player[1].stable.num_unicorns != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: stable size failed\n");
		reset_col();
	}

	if (discardpile.size != 2 ||
			strcmp(discardpile.cards[0].name, poison_tmp.name) != 0 ||
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

// destroying a card w/ a special effect upon getting destroyed
int poison_special_check() {
	int num_fails = 0;
	struct Unicorn poison_tmp = basedeck[67];
	struct Unicorn stabby_tmp = basedeck[40];
	struct Unicorn basic_tmp = basedeck[13];
	struct Unicorn ginormous_tmp = basedeck[48];

	// stabby the unicorn test
	current_players = 2;
	addStable(0, basic_tmp);
	addStable(1, stabby_tmp);
	player[0].hand.cards[player[0].hand.num_cards++] = poison_tmp;

	assert(discardpile.size == 0);
	conditionalEffects(0, poison_tmp, 0, 0);

	if (discardpile.size != 3 ||
			strcmp(discardpile.cards[0].name, poison_tmp.name) != 0 ||
			strcmp(discardpile.cards[1].name, basic_tmp.name) != 0 ||
			strcmp(discardpile.cards[2].name, stabby_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    special test: stabby unicorn effect failed\n");
		reset_col();
	}
	reset_players();
	reset_discard();

	// ginormous unicorn test (toggle flag)
	current_players = 2;
	addStable(0, basic_tmp);
	addStable(1, ginormous_tmp);
	player[0].hand.cards[player[0].hand.num_cards++] = poison_tmp;

	assert((player[1].flags & ginormous_unicorn) == ginormous_unicorn);
	conditionalEffects(0, poison_tmp, 0, 0);

	if ((player[1].flags & ginormous_unicorn) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    special test: toggle ginormous unicorn effect failed\n");
		reset_col();
	}

	reset_players();
	reset_discard();
	return num_fails;
}

// no unicorn cards to destroy
int poison_empty_check() {
	int num_fails = 0;
	struct Unicorn poison_tmp = basedeck[67];
	struct Unicorn yay_tmp = basedeck[100];
	struct Unicorn barbed_tmp = basedeck[106];

	current_players = 2;
	addStable(1, yay_tmp);
	addStable(1, barbed_tmp);
	player[0].hand.cards[player[0].hand.num_cards++] = poison_tmp;

	int ret;
	assert(turn_count == 1);
	assert(player[0].hand.num_cards == 1);
	ret = conditionalEffects(0, poison_tmp, 0, 0);

	if (turn_count != 2 || ret != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    non-unicorn test: turn count failed\n");
		reset_col();
	}

	if (discardpile.size != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    non-unicorn test: discard size failed\n");
		reset_col();
	}

	if (player[0].hand.num_cards != 1 ||
			strcmp(player[0].hand.cards[0].name, poison_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    non-unicorn test: hand verification failed\n");
		reset_col();
	}

	if (player[1].stable.size != 2 || player[1].stable.num_unicorns != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    non-unicorn test: stable size failed\n");
		reset_col();
	}

	reset_players();
	reset_discard();
	return num_fails;
}

// Unicorn Poison
//
// DESTROY a Unicorn card.
int unicorn_poison_tests() {
	int num_fails = 0;

	if (!isclient) {
		rainbow_error("\nStarting Unicorn Poison tests...\n");

		// file input stream setup
		FILE* fp;
		fopen_s(&fp, "Tests/Input/unicornpoison.txt", "r");
		if (fp == NULL) {
			magenta();
			fprintf(stderr, "    file input failed :(");
			reset_col();
			return 1;
		}
		fpinput = fp;

		num_fails += poison_basic_check();
		num_fails += poison_special_check();
		num_fails += poison_empty_check();

		fclose(fp);
	}
	return num_fails;
}
