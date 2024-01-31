#include "UpgradeTests.h"

// sanity test
int lasso_basic_check() {
	int num_fails = 0;
	struct Unicorn lasso_tmp = basedeck[102];
	struct Unicorn rainbow_tmp = basedeck[42]; // brings basic unicorn from hand to stable
	struct Unicorn basic_tmp = basedeck[13];
	struct Unicorn basic_tmp2 = basedeck[17];

	current_players = 2;
	addStable(0, lasso_tmp);
	addStable(1, rainbow_tmp);
	player[0].hand.cards[player[0].hand.num_cards++] = basic_tmp;
	player[1].hand.cards[player[1].hand.num_cards++] = basic_tmp2;

	assert(player[0].stable.size == 1);
	assert(player[1].stable.size == 1);
	assert(player[0].hand.num_cards == 1);
	assert(player[1].hand.num_cards == 1);
	beginningTurnEffects(0, lasso_tmp);

	// mid-turn
	if (player[0].stable.size != 3 || player[0].stable.num_unicorns != 2 ||
			player[1].stable.size != 0 || player[1].stable.num_unicorns != 0 ||
			strcmp(player[0].stable.unicorns[1].name, rainbow_tmp.name) != 0 ||
			strcmp(player[0].stable.unicorns[2].name, basic_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: stealing stable verification failed\n");
		reset_col();
	}

	endOfTurn(0);

	// post returning by end-of-turn; they should have additional basic unicorns
	if (player[0].stable.size != 2 || player[0].stable.num_unicorns != 1 ||
			player[1].stable.size != 2 || player[1].stable.num_unicorns != 2 ||
			strcmp(player[0].stable.unicorns[1].name, basic_tmp.name) != 0 ||
			strcmp(player[1].stable.unicorns[1].name, basic_tmp2.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: returning stable verification failed\n");
		reset_col();
	}

	reset_players();
	return num_fails;
}

int lasso_empty_check() {
	int num_fails = 0;
	struct Unicorn lasso_tmp = basedeck[102];

	current_players = 2;
	addStable(0, lasso_tmp);

	assert(player[0].stable.size == 1);
	assert(player[1].stable.size == 0);
	beginningTurnEffects(0, lasso_tmp);

	// this should skip over the question asking whether or not they'd like
	// to proc unicorn lasso
	if (player[0].stable.size != 1 || player[0].stable.num_unicorns != 0 ||
			player[1].stable.size != 0 || player[1].stable.num_unicorns != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    empty test: stable size failed\n");
		reset_col();
	}

	reset_players();
	return num_fails;
}

// Unicorn Lasso
//
// If this card is in your Stable at the beginning of your turn, you may STEAL a Unicorn card.
// At the end of your turn, return that Unicorn card to the Stable from which you stole it.
int unicorn_lasso_tests() {
	int num_fails = 0;

	rainbow_error("\nStarting Unicorn Lasso tests...\n");

	// file input stream setup
	FILE* fp;
	fopen_s(&fp, "Tests/Input/unicornlasso.txt", "r");
	if (fp == NULL) {
		magenta();
		fprintf(stderr, "    file input failed :(");
		reset_col();
		return 1;
	}
	fpinput = fp;

	// there are other tests featuring unique edge cases with other cards
	// in their own respective card tests
	num_fails += lasso_basic_check();
	num_fails += lasso_empty_check();

	fclose(fp);
	return num_fails;
}
