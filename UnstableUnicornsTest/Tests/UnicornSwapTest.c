#include "MagicTests.h"

// sanity check
int unicorn_swap_basic_check() {
	int num_fails = 0;
	struct Unicorn swap_tmp = basedeck[77];
	struct Unicorn basic_tmp = basedeck[13];
	struct Unicorn basic_tmp2 = basedeck[17];

	current_players = 2;
	addStable(0, basic_tmp);
	addStable(1, basic_tmp2);
	addStable(1, basic_tmp2);
	addStable(1, basic_tmp2);
	player[0].hand.cards[player[0].hand.num_cards++] = swap_tmp;

	int ret;
	assert(player[0].hand.num_cards == 1);
	assert(player[0].stable.size == 1);
	assert(player[1].stable.size == 3);
	ret = conditionalEffects(0, swap_tmp, 0, 0);

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

	if (player[0].stable.size != 1 || player[0].stable.num_unicorns != 1 ||
			player[1].stable.size != 3 || player[1].stable.num_unicorns != 3 ||
			strcmp(player[0].stable.unicorns[0].name, basic_tmp2.name) != 0 ||
			strcmp(player[1].stable.unicorns[2].name, basic_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: stable verification failed\n");
		reset_col();
	}

	reset_players();
	reset_discard();
	return num_fails;
}

// check to see if enter stable effects get triggered when cards swapped stables
int unicorn_swap_special_check() {
	int num_fails = 0;
	struct Unicorn swap_tmp = basedeck[77];
	struct Unicorn rainbow_tmp = basedeck[42]; // brings basic unicorn from hand to stable
	struct Unicorn americorn_tmp = basedeck[47]; // picks random card from another player's hand
	struct Unicorn neigh_tmp = basedeck[128];
	struct Unicorn basic_tmp = basedeck[13];

	current_players = 2;
	addStable(0, rainbow_tmp);
	addStable(1, americorn_tmp);
	player[0].hand.cards[player[0].hand.num_cards++] = swap_tmp;
	player[1].hand.cards[player[1].hand.num_cards++] = neigh_tmp;
	player[1].hand.cards[player[1].hand.num_cards++] = basic_tmp;

	assert(player[0].hand.num_cards == 1);
	assert(player[1].hand.num_cards == 2);
	conditionalEffects(0, swap_tmp, 0, 0);

	if (player[0].stable.size != 1 || player[0].stable.num_unicorns != 1 ||
			player[1].stable.size != 2 || player[1].stable.num_unicorns != 2 ||
			strcmp(player[0].stable.unicorns[0].name, americorn_tmp.name) != 0 ||
			strcmp(player[1].stable.unicorns[0].name, rainbow_tmp.name) != 0 ||
			strcmp(player[1].stable.unicorns[1].name, basic_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    special test: stable verification failed\n");
		reset_col();
	}

	if (player[0].hand.num_cards != 1 || player[1].hand.num_cards != 0 ||
			strcmp(player[0].hand.cards[0].name, neigh_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    special test: hand verification failed\n");
		reset_col();
	}

	reset_players();
	reset_discard();
	return num_fails;
}

// check to see if flags get toggled when cards swapped stables
int unicorn_swap_toggle_check() {
	int num_fails = 0;
	struct Unicorn swap_tmp = basedeck[77];
	struct Unicorn queen_tmp = basedeck[51];
	struct Unicorn ginormous_tmp = basedeck[48];

	current_players = 3;
	addStable(0, queen_tmp);
	addStable(1, ginormous_tmp);
	player[0].hand.cards[player[0].hand.num_cards++] = swap_tmp;

	assert((player[0].flags & queen_bee_unicorn) == 0); // this flag is set backwards
	assert((player[1].flags & queen_bee_unicorn) == queen_bee_unicorn);
	assert((player[2].flags & queen_bee_unicorn) == queen_bee_unicorn);
	assert((player[1].flags & ginormous_unicorn) == ginormous_unicorn);
	assert(player[0].hand.num_cards == 1);
	conditionalEffects(0, swap_tmp, 0, 0);

	if ((player[0].flags & queen_bee_unicorn) != queen_bee_unicorn || (player[0].flags & ginormous_unicorn) != ginormous_unicorn ||
			(player[1].flags & queen_bee_unicorn) != 0 || (player[1].flags & ginormous_unicorn) != 0 ||
			(player[2].flags & queen_bee_unicorn) != queen_bee_unicorn || (player[2].flags & ginormous_unicorn) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    special test: toggle flags failed\n");
		reset_col();
	}

	reset_players();
	reset_discard();
	return num_fails;
}

// either no unicorn cards in your own stable or no unicorns is any other stable,
// so unicorn swap cannot be played
int unicorn_swap_empty_check() {
	int num_fails = 0;
	struct Unicorn swap_tmp = basedeck[77];
	struct Unicorn yay_tmp = basedeck[100];
	struct Unicorn barbed_tmp = basedeck[106];
	struct Unicorn basic_tmp = basedeck[13];

	// no unicorn card in player[0]'s stable
	current_players = 2;
	addStable(0, barbed_tmp);
	addStable(1, basic_tmp);
	player[0].hand.cards[player[0].hand.num_cards++] = swap_tmp;

	int ret;
	assert(discardpile.size == 0);
	assert(player[0].hand.num_cards == 1);
	ret = conditionalEffects(0, swap_tmp, 0, 0);

	if (turn_count != 2 || ret != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    empty stable test (player[0]): turn count failed\n");
		reset_col();
	}

	if (player[0].hand.num_cards != 1 ||
			strcmp(player[0].hand.cards[0].name, swap_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    empty stable test (player[0]): hand verification failed\n");
		reset_col();
	}

	if (discardpile.size != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    empty stable test (player[0]): discard size failed\n");
		reset_col();
	}
	reset_players();
	reset_discard();

	// no unicorn card in player[1]'s stable
	current_players = 2;
	addStable(0, basic_tmp);
	addStable(1, yay_tmp);
	player[0].hand.cards[player[0].hand.num_cards++] = swap_tmp;

	assert(discardpile.size == 0);
	assert(player[0].hand.num_cards == 1);
	ret = conditionalEffects(0, swap_tmp, 0, 0);

	if (turn_count != 2 || ret != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    empty stable test (player[1]): turn count failed\n");
		reset_col();
	}

	if (player[0].hand.num_cards != 1 ||
		strcmp(player[0].hand.cards[0].name, swap_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    empty stable test (player[1]): hand verification failed\n");
		reset_col();
	}

	if (discardpile.size != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    empty stable test (player[1]): discard size failed\n");
		reset_col();
	}

	reset_players();
	reset_discard();
	return num_fails;
}

// compatibility w/ unicorn lasso & swapping stolen cards
int unicorn_swap_unicorn_lasso_check() {
	int num_fails = 0;
	struct Unicorn swap_tmp = basedeck[77];
	struct Unicorn queen_tmp = basedeck[51];
	struct Unicorn ginormous_tmp = basedeck[48];
	struct Unicorn basic_tmp = basedeck[13];
	struct Unicorn basic_tmp2 = basedeck[17];
	struct Unicorn lasso_tmp = basedeck[102];

	// playing hot potato with the stolen lasso card
	// i.e. player[0] takes card (ginormous_tmp) from player[1]
	// player[0] activates unicorn swap and moves card (ginormous_tmp) to player[2]'s stable
	// player[0] then steals some random card (queen_tmp) from player[2]'s stable
	// player[1] gets nothing back at the end of player[0]'s turn
	current_players = 3;
	player[0].hand.cards[player[0].hand.num_cards++] = swap_tmp;

	addStable(0, lasso_tmp);
	addStable(0, basic_tmp);
	addStable(1, ginormous_tmp);
	addStable(2, basic_tmp2);
	addStable(2, queen_tmp);

	assert(player[0].hand.num_cards == 1);
	assert(player[0].stable.size == 2);
	assert(player[0].stable.num_unicorns == 1);
	assert(player[0].flags == queen_bee_unicorn);

	assert(player[1].stable.size == 1);
	assert(player[1].stable.num_unicorns == 1);
	assert((player[1].flags & ginormous_unicorn) == ginormous_unicorn);
	assert((player[1].flags & queen_bee_unicorn) == queen_bee_unicorn);

	assert(player[2].stable.size == 2);
	assert(player[2].stable.num_unicorns == 2);
	assert(player[2].flags == 0);

	beginningTurnEffects(0, lasso_tmp);
	assert(uni_lasso_flag[0] != -1);
	conditionalEffects(0, swap_tmp, 0, 0);

	// check if the lasso flag has been reset before the turn is over
	if (uni_lasso_flag[0] != -1) {
		num_fails++;
		red();
		fprintf(stderr, "    unicorn lasso test: flag reset failed\n");
		reset_col();
	}

	// player[0]'s size and number of unicorns should go up by 1 because of unicorn lasso
	// player[1]'s size and number of unicorns should go down by 1 because of unicorn lasso
	// player[2]'s size and number of unicorns stay the same since unicorn swap is an
	// equivalent exchange number-wise
	if (player[0].stable.size != 3 || player[0].stable.num_unicorns != 2 ||
			player[1].stable.size != 0 || player[1].stable.num_unicorns != 0 ||
			player[2].stable.size != 2 || player[2].stable.num_unicorns != 2) {
		num_fails++;
		red();
		fprintf(stderr, "    unicorn lasso test: stable size pre-end of turn failed\n");
		reset_col();
	}

	// checks the last unicorn in player[0] (index 2, 3rd card) against the swapped
	// card from player[2] (queen_tmp)
	// the stolen/swapped card (ginormous_tmp) is the last card of player[2] at index 1
	if (strcmp(player[0].stable.unicorns[2].name, queen_tmp.name) != 0 ||
			strcmp(player[2].stable.unicorns[0].name, basic_tmp2.name) != 0 ||
			strcmp(player[2].stable.unicorns[1].name, ginormous_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    unicorn lasso test: stable unicorn check pre-end of turn failed\n");
		reset_col();
	}
	
	// player[0] has queen bee unicorn, so that flag should be set for every /other/ player but them
	// player[2] has ginormous unicorn, so that flag should be set for themself
	if ((player[0].flags & ginormous_unicorn) != 0 || (player[0].flags & queen_bee_unicorn) != 0 ||
			(player[1].flags & ginormous_unicorn) != 0 || (player[1].flags & queen_bee_unicorn) != queen_bee_unicorn ||
			(player[2].flags & ginormous_unicorn) != ginormous_unicorn || (player[2].flags & queen_bee_unicorn) != queen_bee_unicorn) {
		num_fails++;
		red();
		fprintf(stderr, "    unicorn lasso test: toggle flags check pre-end of turn failed\n");
		reset_col();
	}

	endOfTurn(0);

	// the unicorn locations, stable sizes, and flags should all stay the same at the end of the turn
	if (player[0].stable.size != 3 || player[0].stable.num_unicorns != 2 ||
			player[1].stable.size != 0 || player[1].stable.num_unicorns != 0 ||
			player[2].stable.size != 2 || player[2].stable.num_unicorns != 2) {
		num_fails++;
		red();
		fprintf(stderr, "    unicorn lasso test: stable size post-end of turn failed\n");
		reset_col();
	}
	
	if (strcmp(player[0].stable.unicorns[2].name, queen_tmp.name) != 0 ||
			strcmp(player[2].stable.unicorns[0].name, basic_tmp2.name) != 0 ||
			strcmp(player[2].stable.unicorns[1].name, ginormous_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    unicorn lasso test: stable unicorn check post-end of turn failed\n");
		reset_col();
	}
	
	// queen bee unicorn is in the same spot as the stolen unicorn from unicorn lasso,
	// so the end of turn phase mistakenly swapped unicorns again when the original lasso'd unicorn is gone,
	// then everyone's flags should change as if player[1] has queen bee unicorn now
	if ((player[0].flags & ginormous_unicorn) != 0 || (player[0].flags & queen_bee_unicorn) != 0 ||
			(player[1].flags & ginormous_unicorn) != 0 || (player[1].flags & queen_bee_unicorn) != queen_bee_unicorn ||
			(player[2].flags & ginormous_unicorn) != ginormous_unicorn || (player[2].flags & queen_bee_unicorn) != queen_bee_unicorn) {
		num_fails++;
		red();
		fprintf(stderr, "    unicorn lasso test: toggle flags check post-end of turn failed\n");
		reset_col();
	}

	reset_players();
	reset_discard();
	return num_fails;
}

int unicorn_swap_puppicorn_check() {
	int num_fails = 0;
	struct Unicorn swap_tmp = basedeck[77];
	struct Unicorn yay_tmp = basedeck[100];
	struct Unicorn basic_tmp = basedeck[13];
	struct Unicorn basic_tmp2 = basedeck[17];
	struct Unicorn puppicorn_tmp = basedeck[41];

	current_players = 2;
	player[0].hand.cards[player[0].hand.num_cards++] = swap_tmp;

	addStable(0, basic_tmp);
	addStable(0, basic_tmp);
	addStable(0, basic_tmp);
	addStable(0, basic_tmp);
	addStable(0, puppicorn_tmp);
	addStable(0, basic_tmp);
	addStable(1, yay_tmp);
	addStable(1, basic_tmp2);

	assert(player[0].hand.num_cards == 1);
	assert(player[0].stable.size == 6);
	assert(player[1].stable.size == 2);
	assert(puppicorn_index[0] == 4); // card index spot
	assert(puppicorn_index[1] == 0); // player number index

	conditionalEffects(0, swap_tmp, 0, 0);

	// puppicorn should have moved from player[0][4] to player[1][1]
	if (puppicorn_index[0] != 1 || puppicorn_index[1] != 1) {
		num_fails++;
		red();
		fprintf(stderr, "    puppicorn test: puppicorn index failed\n");
		reset_col();
	}

	if (player[0].stable.size != 6 || player[0].stable.num_unicorns != 6 ||
			player[1].stable.size != 2 || player[1].stable.num_unicorns != 1 ||
			strcmp(player[0].stable.unicorns[5].name, basic_tmp2.name) != 0 ||
			strcmp(player[1].stable.unicorns[0].name, yay_tmp.name) != 0 ||
			strcmp(player[1].stable.unicorns[puppicorn_index[0]].name, puppicorn_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    puppicorn test: stable verification failed\n");
		reset_col();
	}

	reset_players();
	reset_discard();
	return num_fails;
}

// Unicorn Swap
//
// Move a Unicorn card in your Stable to any other player's Stable,
// then STEAL a Unicorn card from that player's Stable.
int unicorn_swap_tests() {
	int num_fails = 0;

	rainbow("\nStarting Unicorn Swap tests...\n");

	// file input stream setup
	FILE* fp;
	fopen_s(&fp, "Tests/Input/unicornswap.txt", "r");
	if (fp == NULL) {
		magenta();
		fprintf(stderr, "    file input failed :(");
		reset_col();
		return 1;
	}
	fpinput = fp;

	num_fails += unicorn_swap_basic_check();
	num_fails += unicorn_swap_special_check();
	num_fails += unicorn_swap_toggle_check();
	num_fails += unicorn_swap_empty_check();
	num_fails += unicorn_swap_unicorn_lasso_check();
	num_fails += unicorn_swap_puppicorn_check();

	fclose(fp);
	return num_fails;
}
