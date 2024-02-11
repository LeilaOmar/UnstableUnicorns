#include "MagicTests.h"
#include "networkevents.h"

// sanity check
int luck_basic_check() {
	int num_fails = 0;
	struct Unicorn luck_tmp = basedeck[73];
	struct Unicorn basic_tmp = basedeck[13];
	struct Unicorn super_tmp = basedeck[128];
	struct Unicorn neigh_tmp = basedeck[127];

	toggleFlags(0, yay_effect);
	player[0].hand.cards[player[0].hand.num_cards++] = luck_tmp;
	player[0].hand.cards[player[0].hand.num_cards++] = basic_tmp;

	assert(discardpile.size == 0);
	assert(player[0].hand.num_cards == 2);
	assert(player[0].flags == yay);
	playCard(0);

	if (turn_count != 2) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: turn count failed\n");
		reset_col();
	}

	// hand size is 1 because it restarted the beginning turn phase, so
	// the player drew again
	if (player[0].hand.num_cards != 1) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: hand size failed\n");
		reset_col();
	}

	if (discardpile.size != 4 ||
		strcmp(discardpile.cards[0].name, luck_tmp.name) != 0 ||
		strcmp(discardpile.cards[1].name, basic_tmp.name) != 0 ||
		strcmp(discardpile.cards[2].name, super_tmp.name) != 0 ||
		strcmp(discardpile.cards[3].name, neigh_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: discard verification failed\n");
		reset_col();
	}

	reset_players();
	reset_deck();
	reset_discard();
	return num_fails;
}

// check to see if the beginning turn effect gets triggered
int luck_special_check() {
	int num_fails = 0;
	struct Unicorn luck_tmp = basedeck[73];
	struct Unicorn basic_tmp = basedeck[13];
	struct Unicorn fertile_tmp = basedeck[38];
	struct Unicorn baby_tmp = basedeck[0];

	addStable(0, fertile_tmp);
	toggleFlags(0, yay_effect);
	player[0].hand.cards[player[0].hand.num_cards++] = luck_tmp;
	player[0].hand.cards[player[0].hand.num_cards++] = basic_tmp;
	player[0].hand.cards[player[0].hand.num_cards++] = basic_tmp;

	int tmp_size = nursery.size;
	assert(discardpile.size == 0);
	assert(player[0].hand.num_cards == 3);
	assert(player[0].stable.size == 1);
	assert(player[0].flags == yay);
	playCard(0);

	if (nursery.size != (tmp_size - 1) ||
			strcmp(player[0].stable.unicorns[1].name, baby_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    special test: extremely fertile unicorn effect failed\n");
		reset_col();
	}

	if (player[0].stable.size != 2 || player[0].stable.num_unicorns != 2) {
		num_fails++;
		red();
		fprintf(stderr, "    special test: stable size failed\n");
		reset_col();
	}

	reset_players();
	reset_nursery();
	reset_deck();
	reset_discard();
	return num_fails;
}

// no cards in hand before the effect takes place
int luck_empty_check() {
	int num_fails = 0;
	struct Unicorn luck_tmp = basedeck[73];

	player[0].hand.cards[player[0].hand.num_cards++] = luck_tmp;

	int ret;
	assert(player[0].hand.num_cards == 1);
	ret = conditionalEffects(0, luck_tmp, 0, 0);

	if (ret != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    empty hand test: conditional effect return failed\n");
		reset_col();
	}

	reset_players();
	reset_deck();
	reset_discard();
	return num_fails;
}

// compatibility w/ unicorn lasso & a magic unicorn that triggers upon entering the stable
int luck_unicorn_lasso_check() {
	int num_fails = 0;
	struct Unicorn luck_tmp = basedeck[73];
	struct Unicorn basic_tmp = basedeck[13];
	struct Unicorn basic_tmp2 = basedeck[17];
	struct Unicorn lasso_tmp = basedeck[102];
	struct Unicorn rainbow_tmp = basedeck[42]; // brings basic unicorn from hand to stable

	current_players = 2;
	player[0].hand.cards[player[0].hand.num_cards++] = luck_tmp;
	player[0].hand.cards[player[0].hand.num_cards++] = basic_tmp;
	player[0].hand.cards[player[0].hand.num_cards++] = basic_tmp2;

	draw(0, 1);
	addStable(0, lasso_tmp);
	addStable(1, rainbow_tmp);
	addStable(1, basic_tmp);
	addStable(1, basic_tmp);
	toggleFlags(0, yay_effect);

	assert(discardpile.size == 0);
	assert(player[0].hand.num_cards == 4);
	assert(player[0].stable.size == 1);
	assert(player[0].stable.num_unicorns == 0);
	assert(player[0].flags == yay);

	beginningTurnEffects(0, lasso_tmp);
	assert(uni_lasso_flag[0] != -1);
	playCard(0);

	// this is checking in the middle of the 2nd turn, so player[0] still
	// has player[1]'s card
	// 
	// beginning of turn #1:
	// stable[0] = unicorn lasso
	// unicorn lasso effect:
	// stable[0] = unicorn lasso
	// stable[1] = rainbow unicorn
	// enter stable effect: -- 4 cards in hand
	// stable[0] = unicorn lasso
	// stable[1] = rainbow unicorn
	// stable[2] = basic unicorn #1
	// change of luck: -- 3 cards in hand
	// --restart turn
	// end of turn #1: -- 1 card in hand
	// stable[0] = unicorn lasso
	// stable[1] = basic unicorn #1
	// beginning of turn #2, unicorn lasso effect + enter stable effect again: -- 0 cards in hand
	// stable[0] = unicorn lasso
	// stable[1] = basic unicorn #1
	// stable[2] = rainbow unicorn
	// stable[3] = basic unicorn #2
	// draw (after beginning turn effect) -- 1 card in hand
	// 
	if (uni_lasso_flag[0] != 2 ||
			strcmp(player[0].stable.unicorns[uni_lasso_flag[0]].name, rainbow_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    unicorn lasso test: unicorn swap failed\n");
		reset_col();
	}

	if (player[0].stable.size != 4 || player[0].stable.num_unicorns != 3) {
		num_fails++;
		red();
		fprintf(stderr, "    unicorn lasso test: stable size failed\n");
		reset_col();
	}

	// this is just the last card that was drawn during the 2nd beginning turn phase
	if (player[0].hand.num_cards != 1) {
		num_fails++;
		red();
		fprintf(stderr, "    unicorn lasso test: hand size failed\n");
		reset_col();
	}

	reset_players();
	reset_deck();
	reset_discard();
	return num_fails;
}

// Change of Luck
//
// DRAW 2 cards and DISCARD 3 cards, then take another turn.
int change_of_luck_tests() {
	int num_fails = 0;

	rainbow_error("\nStarting Change of Luck tests...\n");
	if (!isclient) {
		// file input stream setup
		FILE* fp;
		fopen_s(&fp, "Tests/Input/changeofluck.txt", "r");
		if (fp == NULL) {
			magenta();
			fprintf(stderr, "    file input failed :(");
			reset_col();
			return 1;
		}
		fpinput = fp;

		num_fails += luck_basic_check();
		num_fails += luck_special_check();
		num_fails += luck_empty_check();
		num_fails += luck_unicorn_lasso_check();

		fclose(fp);
	}
	else {
		// should be no input required
		int events;
		receiveInt(&events, sockfd);
		netStates[events].recvClient(1, sockfd);
	}
	return num_fails;
}
