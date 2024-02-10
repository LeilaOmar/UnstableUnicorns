#include "MagicTests.h"

// destroying an upgrade card
int targeted_destruction_upgrade_check() {
	int num_fails = 0;
	struct Unicorn targeted_tmp = basedeck[86];
	struct Unicorn basic_tmp = basedeck[13];
	struct Unicorn yay_tmp = basedeck[100];

	current_players = 3;
	addStable(1, yay_tmp);
	addStable(2, basic_tmp);
	toggleFlags(1, yay_tmp.effect);
	player[0].hand.cards[player[0].hand.num_cards++] = targeted_tmp;

	int ret;
	assert(discardpile.size == 0);
	assert(player[0].hand.num_cards == 1);
	assert(player[1].stable.size == 1);
	assert(player[1].stable.num_unicorns == 0);
	assert(player[2].stable.size == 1);
	assert(player[1].flags == yay);
	ret = conditionalEffects(0, targeted_tmp, 0, 0);

	if (turn_count != 1 || ret != 1) {
		num_fails++;
		red();
		fprintf(stderr, "    upgrade test: turn count failed\n");
		reset_col();
	}

	if (player[0].hand.num_cards != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    upgrade test: hand size failed\n");
		reset_col();
	}

	if (player[1].stable.size != 0 || player[1].stable.num_unicorns != 0 ||
			player[2].stable.size != 1 || player[2].stable.num_unicorns != 1) {
		num_fails++;
		red();
		fprintf(stderr, "    upgrade test: stable size failed\n");
		reset_col();
	}

	if (discardpile.size != 2 ||
			strcmp(discardpile.cards[0].name, targeted_tmp.name) != 0 ||
			strcmp(discardpile.cards[1].name, yay_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    upgrade test: discard verification failed\n");
		reset_col();
	}

	// flags should be disabled
	if (player[1].flags != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: toggle flags failed\n");
		reset_col();
	}

	reset_players();
	reset_discard();
	return num_fails;
}

// sacrificing a downgrade card
int targeted_destruction_downgrade_check() {
	int num_fails = 0;
	struct Unicorn targeted_tmp = basedeck[86];
	struct Unicorn basic_tmp = basedeck[13];
	struct Unicorn barbed_tmp = basedeck[106];

	current_players = 3;
	addStable(0, barbed_tmp);
	addStable(2, basic_tmp);
	toggleFlags(0, barbed_tmp.effect);
	player[0].hand.cards[player[0].hand.num_cards++] = targeted_tmp;

	int ret;
	assert(discardpile.size == 0);
	assert(player[0].hand.num_cards == 1);
	assert(player[0].stable.size == 1);
	assert(player[0].stable.num_unicorns == 0);
	assert(player[2].stable.size == 1);
	assert(player[0].flags == barbed_wire);
	ret = conditionalEffects(0, targeted_tmp, 0, 0);

	if (turn_count != 1 || ret != 1) {
		num_fails++;
		red();
		fprintf(stderr, "    downgrade test: turn count failed\n");
		reset_col();
	}

	if (player[0].hand.num_cards != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    downgrade test: hand size failed\n");
		reset_col();
	}

	if (player[0].stable.size != 0 || player[0].stable.num_unicorns != 0 ||
			player[2].stable.size != 1 || player[2].stable.num_unicorns != 1) {
		num_fails++;
		red();
		fprintf(stderr, "    downgrade test: stable size failed\n");
		reset_col();
	}

	if (discardpile.size != 2 ||
			strcmp(discardpile.cards[0].name, targeted_tmp.name) != 0 ||
			strcmp(discardpile.cards[1].name, barbed_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    downgrade test: discard verification failed\n");
		reset_col();
	}

	// flags should be disabled
	if (player[0].flags != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    downgrade test: toggle flags failed\n");
		reset_col();
	}

	reset_players();
	reset_discard();
	return num_fails;
}


// no upgrade/downgrade cards to destroy/sacrifice respectively
int targeted_destruction_empty_check() {
	int num_fails = 0;
	struct Unicorn targeted_tmp = basedeck[86];
	struct Unicorn barbed_tmp = basedeck[106];
	struct Unicorn yay_tmp = basedeck[100];

	current_players = 2;
	addStable(0, yay_tmp);
	addStable(1, barbed_tmp);
	player[0].hand.cards[player[0].hand.num_cards++] = targeted_tmp;

	int ret;
	assert(discardpile.size == 0);
	assert(player[0].hand.num_cards == 1);
	assert(player[0].stable.size == 1);
	assert(player[1].stable.size == 1);
	ret = conditionalEffects(0, targeted_tmp, 0, 0);

	if (turn_count != 2 || ret != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    empty test: turn count failed\n");
		reset_col();
	}

	if (player[0].hand.num_cards != 1 ||
		strcmp(player[0].hand.cards[0].name, targeted_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    empty test: hand verification failed\n");
		reset_col();
	}

	if (player[0].stable.size != 1 || player[0].stable.num_unicorns != 0 ||
			player[1].stable.size != 1 || player[1].stable.num_unicorns != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    empty test: stable size failed\n");
		reset_col();
	}

	if (discardpile.size != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    empty test: discard size failed\n");
		reset_col();
	}

	reset_players();
	reset_discard();
	return num_fails;
}

// Targeted Destruction
//
// DESTROY an Upgrade card or SACRIFICE a Downgrade card.
int targeted_destruction_tests() {
	int num_fails = 0;

	if (!isclient) {
		rainbow_error("\nStarting Targeted Destruction tests...\n");

		// file input stream setup
		FILE* fp;
		fopen_s(&fp, "Tests/Input/targeteddestruction.txt", "r");
		if (fp == NULL) {
			magenta();
			fprintf(stderr, "    file input failed :(");
			reset_col();
			return 1;
		}
		fpinput = fp;

		num_fails += targeted_destruction_upgrade_check();
		num_fails += targeted_destruction_downgrade_check();
		num_fails += targeted_destruction_empty_check();

		fclose(fp);
	}
	return num_fails;
}
