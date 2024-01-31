#include "MagicUnicornTests.h"

// basic check
int americorn_basic_check() {
	int num_fails = 0;
	struct Unicorn americorn_tmp = basedeck[47];

	current_players = 2;
	draw(0, 1); 
	addStable(1, americorn_tmp);

	int nhand0 = player[0].hand.num_cards;
	int nhand1 = player[1].hand.num_cards;
	struct Unicorn tmp_card = player[0].hand.cards[0];

	enterStableEffects(1, player[1].stable.unicorns[0].effect);

	if (player[0].hand.num_cards != (nhand0 - 1) ||
			player[1].hand.num_cards != (nhand1 + 1)) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: num cards failed\n");
		reset_col();
	}

	// since the deck is unshuffled and being picked from the back, tmp_card
	// should be unique (Super Neigh)
	assert(strcmp(tmp_card.name, "Super Neigh") == 0);
	if (strcmp(player[1].hand.cards[0].name, tmp_card.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: unicorn verification failed\n");
		reset_col();
	}

	reset_players();
	reset_deck();
	return num_fails;
}

// empty check
int americorn_empty_check() {
	int num_fails = 0;
	struct Unicorn americorn_tmp = basedeck[47];

	current_players = 2;
	addStable(0, americorn_tmp);

	int nhand0 = player[0].hand.num_cards;
	int nhand1 = player[1].hand.num_cards;

	enterStableEffects(0, player[0].stable.unicorns[0].effect);

	if (player[0].hand.num_cards != nhand0 ||
			player[1].hand.num_cards != nhand1) {
		num_fails++;
		red();
		fprintf(stderr, "    empty test: num cards failed\n");
		reset_col();
	}


	reset_players();
	reset_deck();
	return num_fails;
}

// check if the player can choose the card to pull when the other person
// is under the nanny cam effect (all cards in hand are visible at all times)
int americorn_nanny_cam_check() {
	int num_fails = 0;
	struct Unicorn americorn_tmp = basedeck[47];

	current_players = 2;
	struct Unicorn corn = deck.cards[deck.size - 1];
	draw(1, 7); // the first card drawn is "Super Neigh" since it's the last card when unshuffled
	addStable(0, americorn_tmp);
	toggleFlags(1, nanny_cam_effect);

	assert(player[0].hand.num_cards == 0);
	assert((player[1].flags & nanny_cam) == nanny_cam);
	enterStableEffects(0, player[0].stable.unicorns[0].effect);

	// a random pick would take the 6th card w/ seed(0), so
	// an incorrect result should actually be different from corn.name/Super Neigh
	if (strcmp(player[0].hand.cards[0].name, corn.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    nanny came test: hand pick failed\n");
		reset_col();
	}

	reset_players();
	reset_deck();
	return num_fails;
}

// Americorn
//
// When this card enters your Stable, you may pull a card at random
// from another player's hand.
int americorn_tests() {
	int num_fails = 0;

	rainbow_error("\nStarting Americorn tests...\n");

	// file input stream setup
	FILE* fp;
	fopen_s(&fp, "Tests/Input/americorn.txt", "r");
	if (fp == NULL) {
		magenta();
		fprintf(stderr, "    file input failed :(");
		reset_col();
		return 1;
	}
	fpinput = fp;

	num_fails += americorn_basic_check();
	num_fails += americorn_empty_check();
	num_fails += americorn_nanny_cam_check();

	fclose(fp);
	return num_fails;
}
