#include "MagicTests.h"

// sanity check for upgrade cards
int re_target_upgrade_check() {
	int num_fails = 0;
	struct Unicorn target_tmp = basedeck[79];
	struct Unicorn yay_tmp = basedeck[100];

	current_players = 3;
	addStable(1, yay_tmp);
	toggleFlags(1, yay_tmp.effect);
	player[0].hand.cards[player[0].hand.num_cards++] = target_tmp;

	int ret;
	assert(player[0].hand.num_cards == 1);
	assert(player[1].stable.size == 1);
	assert(player[1].flags == yay);
	assert(player[2].stable.size == 0);
	ret = conditionalEffects(0, target_tmp, 0, 0);

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

	if (discardpile.size != 1 ||
			strcmp(discardpile.cards[0].name, target_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    upgrade test: discard pile verification failed\n");
		reset_col();
	}

	if ((player[2].flags & yay) != yay ||
			player[1].flags != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    upgrade test: toggle flags failed\n");
		reset_col();
	}

	if (player[2].stable.size != 1 || player[2].stable.num_unicorns != 0 ||
			player[1].stable.size != 0 || player[1].stable.num_unicorns != 0 ||
			strcmp(player[2].stable.unicorns[0].name, yay_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    upgrade test: stable verification failed\n");
		reset_col();
	}

	reset_players();
	reset_discard();
	return num_fails;
}

// sanity check for downgrade cards
int re_target_downgrade_check() {
	int num_fails = 0;
	struct Unicorn target_tmp = basedeck[79];
	struct Unicorn barbed_tmp = basedeck[106];

	current_players = 2;
	addStable(1, barbed_tmp);
	toggleFlags(1, barbed_tmp.effect);
	player[0].hand.cards[player[0].hand.num_cards++] = target_tmp;

	int ret;
	assert(player[0].hand.num_cards == 1);
	assert(player[0].stable.size == 0);
	assert(player[1].stable.size == 1);
	assert(player[1].flags == barbed_wire);
	ret = conditionalEffects(0, target_tmp, 0, 0);

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

	if (discardpile.size != 1 ||
			strcmp(discardpile.cards[0].name, target_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    downgrade test: discard pile verification failed\n");
		reset_col();
	}

	if ((player[0].flags & barbed_wire) != barbed_wire ||
			player[1].flags != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    downgrade test: toggle flags failed\n");
		reset_col();
	}

	if (player[0].stable.size != 1 || player[0].stable.num_unicorns != 0 ||
			player[1].stable.size != 0 || player[1].stable.num_unicorns != 0 ||
			strcmp(player[1].stable.unicorns[0].name, barbed_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    downgrade test: stable verification failed\n");
		reset_col();
	}

	reset_players();
	reset_discard();
	return num_fails;
}

// no upgrades or downgrades at all
int re_target_empty_check() {
	int num_fails = 0;
	struct Unicorn target_tmp = basedeck[79];
	struct Unicorn basic_tmp = basedeck[13];

	// no unicorn card in player[0]'s stable
	current_players = 2;
	addStable(0, basic_tmp);
	addStable(1, basic_tmp);
	player[0].hand.cards[player[0].hand.num_cards++] = target_tmp;

	int ret;
	assert(discardpile.size == 0);
	assert(player[0].hand.num_cards == 1);
	ret = conditionalEffects(0, target_tmp, 0, 0);

	if (turn_count != 2 || ret != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    empty test: turn count failed\n");
		reset_col();
	}

	if (player[0].hand.num_cards != 1 ||
			strcmp(player[0].hand.cards[0].name, target_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    empty test: hand verification failed\n");
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


// Re-Target
//
// Move an Upgrade or Downgrade card from any player's Stable
// to any other player's Stable.
int re_target_tests() {
	int num_fails = 0;

	rainbow_error("\nStarting Re-Target tests...\n");

	// file input stream setup
	FILE* fp;
	fopen_s(&fp, "Tests/Input/retarget.txt", "r");
	if (fp == NULL) {
		magenta();
		fprintf(stderr, "    file input failed :(");
		reset_col();
		return 1;
	}
	fpinput = fp;

	num_fails += re_target_upgrade_check();
	num_fails += re_target_downgrade_check();
	num_fails += re_target_empty_check();

	fclose(fp);
	return num_fails;
}
