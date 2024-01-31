#include "MagicTests.h"

// sanity check
int glitter_tornado_basic_check() {
	int num_fails = 0;
	struct Unicorn tornado_tmp = basedeck[75];
	struct Unicorn basic_tmp = basedeck[13];
	struct Unicorn basic_tmp2 = basedeck[17];
	struct Unicorn baby_tmp = basedeck[12];

	nursery.size = 0;
	int tmp_size = nursery.size;
	current_players = 3;
	addStable(0, basic_tmp);
	addStable(1, basic_tmp2);
	addStable(2, baby_tmp);
	player[0].hand.cards[player[0].hand.num_cards++] = tornado_tmp;

	int ret;
	assert(player[0].hand.num_cards == 1);
	assert(player[1].hand.num_cards == 0);
	assert(player[2].hand.num_cards == 0);

	assert(player[0].stable.size == 1);
	assert(player[1].stable.size == 1);
	assert(player[2].stable.size == 1);
	ret = conditionalEffects(0, tornado_tmp, 0, 0);

	if (turn_count != 1 || ret != 1) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: turn count failed\n");
		reset_col();
	}

	if (player[0].stable.size != 0 || player[0].stable.num_unicorns != 0 ||
			player[1].stable.size != 0 ||	player[1].stable.num_unicorns != 0 ||
			player[2].stable.size != 0 ||	player[2].stable.num_unicorns != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: stable size failed\n");
		reset_col();
	}

	if (player[0].hand.num_cards != 1 ||
			player[1].hand.num_cards != 1 || 
			player[2].hand.num_cards != 0 ||
			strcmp(player[0].hand.cards[0].name, basic_tmp.name) != 0 ||
			strcmp(player[1].hand.cards[0].name, basic_tmp2.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: hand verification failed\n");
		reset_col();
	}

	// the baby narwhal goes back to index 0 because nursery.size was set to 0 earlier
	if (nursery.size != (tmp_size + 1) ||
			strcmp(nursery.cards[0].name, baby_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: nursery verification failed\n");
		reset_col();
	}

	reset_players();
	reset_nursery();
	reset_discard();
	return num_fails;
}

// check to see if flags get toggled when cards return to hand
int glitter_tornado_special_check() {
	int num_fails = 0;
	struct Unicorn tornado_tmp = basedeck[75];
	struct Unicorn yay_tmp = basedeck[100];
	struct Unicorn ginormous_tmp = basedeck[48];

	current_players = 2;
	addStable(0, yay_tmp);
	addStable(1, ginormous_tmp);
	toggleFlags(0, yay_tmp.effect);
	toggleFlags(1, ginormous_tmp.effect);
	player[0].hand.cards[player[0].hand.num_cards++] = tornado_tmp;

	assert((player[0].flags & yay) == yay);
	assert((player[1].flags & ginormous_unicorn) == ginormous_unicorn);
	assert(player[0].hand.num_cards == 1);
	conditionalEffects(0, tornado_tmp, 0, 0);

	if ((player[0].flags & yay) != 0 ||
			(player[1].flags & ginormous_unicorn) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    special test: toggle flags failed\n");
		reset_col();
	}

	reset_players();
	reset_discard();
	return num_fails;
}

// no cards in every player's stable, so glitter tornado cannot be played
int glitter_tornado_empty_check() {
	int num_fails = 0;
	struct Unicorn tornado_tmp = basedeck[75];
	struct Unicorn basic_tmp = basedeck[13];

	current_players = 2;
	addStable(1, basic_tmp);
	player[0].hand.cards[player[0].hand.num_cards++] = tornado_tmp;

	int ret;
	assert(discardpile.size == 0);
	assert(player[0].hand.num_cards == 1);
	ret = conditionalEffects(0, tornado_tmp, 0, 0);

	if (turn_count != 2 || ret != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    empty stable test: turn count failed\n");
		reset_col();
	}

	if (player[0].hand.num_cards != 1 ||
			strcmp(player[0].hand.cards[0].name, tornado_tmp.name) != 0) {
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

// compatibility w/ unicorn lasso & returning a stolen card to their own hand
int glitter_tornado_unicorn_lasso_check() {
	int num_fails = 0;
	struct Unicorn tornado_tmp = basedeck[75];
	struct Unicorn basic_tmp = basedeck[13];
	struct Unicorn basic_tmp2 = basedeck[17];
	struct Unicorn lasso_tmp = basedeck[102];

	current_players = 2;
	player[0].hand.cards[player[0].hand.num_cards++] = tornado_tmp;

	addStable(0, lasso_tmp);
	addStable(1, basic_tmp);
	addStable(1, basic_tmp2);

	assert(player[0].hand.num_cards == 1);
	assert(player[0].stable.size == 1);
	assert(player[0].stable.num_unicorns == 0);

	assert(player[1].hand.num_cards == 0);
	assert(player[1].stable.size == 2);
	assert(player[1].stable.num_unicorns == 2);

	beginningTurnEffects(0, lasso_tmp);
	assert(uni_lasso_flag[0] != -1);
	conditionalEffects(0, tornado_tmp, 0, 0);

	// check if the lasso flag has been reset before the turn is over
	if (uni_lasso_flag[0] != -1) {
		num_fails++;
		red();
		fprintf(stderr, "\n    unicorn lasso test: flag reset failed\n");
		reset_col();
	}

	if (player[0].hand.num_cards != 1 || player[1].hand.num_cards != 1 ||
			strcmp(player[0].hand.cards[0].name, basic_tmp.name) != 0 ||
			strcmp(player[1].hand.cards[0].name, basic_tmp2.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    unicorn lasso test: hand verification pre-end of turn failed\n");
		reset_col();
	}

	if (player[0].stable.size != 1 || player[0].stable.num_unicorns != 0 ||
			player[1].stable.size != 0 || player[1].stable.num_unicorns != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    unicorn lasso test: stable size pre-end of turn failed\n");
		reset_col();
	}

	endOfTurn(0);

	// check if the stolen card is still in their hand and the stable sizes are the same
	if (player[0].hand.num_cards != 1 || player[1].hand.num_cards != 1 ||
		strcmp(player[0].hand.cards[0].name, basic_tmp.name) != 0 ||
		strcmp(player[1].hand.cards[0].name, basic_tmp2.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    unicorn lasso test: hand verification post-end of turn failed\n");
		reset_col();
	}

	if (player[0].stable.size != 1 || player[0].stable.num_unicorns != 0 ||
			player[1].stable.size != 0 || player[1].stable.num_unicorns != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    unicorn lasso test: stable size post-end of turn failed\n");
		reset_col();
	}

	reset_players();
	reset_discard();
	return num_fails;
}

// Glitter Tornado
//
// Return a card in each player's Stable (including yours) to their hand.
int glitter_tornado_tests() {
	int num_fails = 0;

	rainbow_error("\nStarting Glitter Tornado tests...\n");

	// file input stream setup
	FILE* fp;
	fopen_s(&fp, "Tests/Input/glittertornado.txt", "r");
	if (fp == NULL) {
		magenta();
		fprintf(stderr, "    file input failed :(");
		reset_col();
		return 1;
	}
	fpinput = fp;

	num_fails += glitter_tornado_basic_check();
	num_fails += glitter_tornado_special_check();
	num_fails += glitter_tornado_empty_check();
	num_fails += glitter_tornado_unicorn_lasso_check();

	fclose(fp);
	return num_fails;
}
