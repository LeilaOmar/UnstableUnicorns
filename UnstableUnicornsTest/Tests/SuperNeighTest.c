#include "InstantTests.h"

// odd number of neighs, the card goes to the discard pile
int super_neigh_odd_check() {
	int num_fails = 0;
	struct Unicorn neigh_tmp = basedeck[115];
	struct Unicorn super_neigh_tmp = basedeck[128];
	struct Unicorn basic_tmp = basedeck[13];

	player[0].hand.cards[player[0].hand.num_cards++] = basic_tmp;
	player[0].hand.cards[player[0].hand.num_cards++] = neigh_tmp;

	player[1].hand.cards[player[1].hand.num_cards++] = super_neigh_tmp;
	current_players = 2;

	assert(discardpile.size == 0);
	assert(player[0].hand.num_cards == 2);
	assert(player[1].hand.num_cards == 1);
	assert(player[0].stable.size == 0);
	assert(player[1].stable.size == 0);

	playCard(0);

	// player[0] lost the card they originally tried to play
	// player[1] lost 1 neigh
	if (player[0].hand.num_cards != 1 || player[1].hand.num_cards != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    odd test: hand size failed\n");
		reset_col();
	}

	if (strcmp(player[0].hand.cards[0].name, neigh_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    odd test: player[1] hand verification failed\n");
		reset_col();
	}

	if (player[0].stable.size != 0 || player[0].stable.num_unicorns != 0 ||
			player[1].stable.size != 0 || player[1].stable.num_unicorns != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    odd test: stable size failed\n");
		reset_col();
	}

	if (discardpile.size != 2 ||
			strcmp(discardpile.cards[0].name, super_neigh_tmp.name) != 0 ||
			strcmp(discardpile.cards[1].name, basic_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    odd test: discard verification failed\n");
		reset_col();
	}

	reset_players();
	reset_discard();
	return num_fails;
}

// even number of neighs, the card is in play and goes to the stable
int super_neigh_even_check() {
	int num_fails = 0;
	struct Unicorn neigh_tmp = basedeck[115];
	struct Unicorn super_neigh_tmp = basedeck[128];

	struct Unicorn basic_tmp = basedeck[13];

	player[0].hand.cards[player[0].hand.num_cards++] = basic_tmp;
	player[0].hand.cards[player[0].hand.num_cards++] = super_neigh_tmp;

	player[1].hand.cards[player[1].hand.num_cards++] = neigh_tmp;
	player[1].hand.cards[player[1].hand.num_cards++] = neigh_tmp;
	current_players = 2;

	assert(discardpile.size == 0);
	assert(player[0].hand.num_cards == 2);
	assert(player[1].hand.num_cards == 2);
	assert(player[0].stable.size == 0);
	assert(player[1].stable.size == 0);

	playCard(0);

	// player[0] lost 1 neigh and the card they originally tried to play
	// player[1] lost 1 neigh
	if (player[0].hand.num_cards != 0 || player[1].hand.num_cards != 1) {
		num_fails++;
		red();
		fprintf(stderr, "    even test: hand size failed\n");
		reset_col();
	}

	if (strcmp(player[1].hand.cards[0].name, neigh_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    even test: player[1] hand verification failed\n");
		reset_col();
	}

	if (player[0].stable.size != 1 || player[0].stable.num_unicorns != 1 ||
			strcmp(player[0].stable.unicorns[0].name, basic_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    even test: stable size failed\n");
		reset_col();
	}

	if (discardpile.size != 2 ||
			strcmp(discardpile.cards[0].name, neigh_tmp.name) != 0 ||
			strcmp(discardpile.cards[1].name, super_neigh_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    even test: discard verification failed\n");
		reset_col();
	}

	reset_players();
	reset_discard();
	return num_fails;
}

// Super Neigh
//
// Play this card when another player tries to play a card.
// Stop their card from being played and send it to the discard pile.
// This card cannot be Neigh'd.
int super_neigh_tests() {
	int num_fails = 0;

	rainbow_error("\nStarting Super Neigh tests...\n");

	// file input stream setup
	FILE* fp;
	fopen_s(&fp, "Tests/Input/superneigh.txt", "r");
	if (fp == NULL) {
		magenta();
		fprintf(stderr, "    file input failed :(");
		reset_col();
		return 1;
	}
	fpinput = fp;

	// already tested with yay, slowdown, and ginormous unicorn
	num_fails += super_neigh_odd_check();
	num_fails += super_neigh_even_check();

	fclose(fp);
	return num_fails;
}
