#include "MagicTests.h"

// trading hands where player[0].num_cards > player[1].num_cards
int bargain_greater_check() {
	int num_fails = 0;
	struct Unicorn bargain_tmp = basedeck[81];

	current_players = 3;
	player[0].hand.cards[player[0].hand.num_cards++] = bargain_tmp;

	struct Unicorn corn = deck.cards[deck.size - 1];
	draw(0, 4);
	draw(1, 2);
	toggleFlags(0, yay_effect);

	assert(player[0].hand.num_cards == 5);
	assert(player[1].hand.num_cards == 2);
	assert(player[2].hand.num_cards == 0);
	assert(player[0].flags == yay);
	playCard(0);

	if (turn_count != 1) {
		num_fails++;
		red();
		fprintf(stderr, "    greater test: turn count failed\n");
		reset_col();
	}

	if (player[0].hand.num_cards != 2 ||
			player[1].hand.num_cards != 4 ||
			strcmp(player[1].hand.cards[0].name, corn.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    greater test: hand verification failed\n");
		reset_col();
	}

	if (discardpile.size != 1 ||
			strcmp(discardpile.cards[0].name, bargain_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    greater test: discard pile verification failed\n");
		reset_col();
	}

	reset_players();
	reset_deck();
	reset_discard();
	return num_fails;
}

// trading hands where player[0].num_cards < player[1].num_cards
int bargain_lesser_check() {
	int num_fails = 0;
	struct Unicorn bargain_tmp = basedeck[81];

	current_players = 3;
	player[0].hand.cards[player[0].hand.num_cards++] = bargain_tmp;

	struct Unicorn corn = deck.cards[deck.size - 1];
	draw(1, 5);
	draw(0, 1);
	toggleFlags(0, yay_effect);

	assert(player[0].hand.num_cards == 2);
	assert(player[1].hand.num_cards == 5);
	assert(player[2].hand.num_cards == 0);
	assert(player[0].flags == yay);
	playCard(0);

	if (turn_count != 1) {
		num_fails++;
		red();
		fprintf(stderr, "    lesser test: turn count failed\n");
		reset_col();
	}

	if (player[0].hand.num_cards != 5 ||
			player[1].hand.num_cards != 1 ||
			strcmp(player[0].hand.cards[0].name, corn.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    lesser test: hand verification failed\n");
		reset_col();
	}

	if (discardpile.size != 1 ||
		strcmp(discardpile.cards[0].name, bargain_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    lesser test: discard pile verification failed\n");
		reset_col();
	}

	reset_players();
	reset_deck();
	reset_discard();
	return num_fails;
}

// no cards in player[0]'s or other player's hands
int bargain_empty_check() {
	int num_fails = 0;
	struct Unicorn bargain_tmp = basedeck[81];
	struct Unicorn basic_tmp = basedeck[13];

	// no cards in other player's hands
	current_players = 2;
	player[0].hand.cards[player[0].hand.num_cards++] = bargain_tmp;
	player[0].hand.cards[player[0].hand.num_cards++] = basic_tmp;

	int ret;
	assert(player[0].hand.num_cards == 2);
	assert(player[1].hand.num_cards == 0);
	ret = conditionalEffects(0, bargain_tmp, 0, 0);

	if (turn_count != 2 || ret != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    empty test (player[1]): turn count failed\n");
		reset_col();
	}
	reset_players();
	reset_discard();

	// no cards in player[0]'s hands; should still work, actually :p
	current_players = 2;
	player[0].hand.cards[player[0].hand.num_cards++] = bargain_tmp;
	player[1].hand.cards[player[1].hand.num_cards++] = basic_tmp;

	toggleFlags(0, yay_effect);
	assert(discardpile.size == 0);
	assert(player[0].hand.num_cards == 1);
	assert(player[1].hand.num_cards == 1);
	assert(player[0].flags == yay);
	playCard(0);

	if (turn_count != 1) {
		num_fails++;
		red();
		fprintf(stderr, "    empty test (player[0]): turn count failed\n");
		reset_col();
	}

	if (player[0].hand.num_cards != 1 ||
			player[1].hand.num_cards != 0 ||
			strcmp(player[0].hand.cards[0].name, basic_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    empty test (player[0]): hand verification failed\n");
		reset_col();
	}

	if (discardpile.size != 1 ||
			strcmp(discardpile.cards[0].name, bargain_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    empty test (player[0]): discard size failed\n");
		reset_col();
	}

	reset_players();
	reset_discard();
	return num_fails;
}


// Unfair Bargain
//
// Trade hands with any other player.
int unfair_bargain_tests() {
	int num_fails = 0;

	if (!isclient) {
		rainbow_error("\nStarting Unfair Bargain tests...\n");

		// file input stream setup
		FILE* fp;
		fopen_s(&fp, "Tests/Input/unfairbargain.txt", "r");
		if (fp == NULL) {
			magenta();
			fprintf(stderr, "    file input failed :(");
			reset_col();
			return 1;
		}
		fpinput = fp;

		num_fails += bargain_greater_check();
		num_fails += bargain_lesser_check();
		num_fails += bargain_empty_check();

		fclose(fp);
	}
	return num_fails;
}
