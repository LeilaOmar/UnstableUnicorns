#include "MagicTests.h"
#include "networkevents.h"

// sanity test
int mystical_vortex_basic_check() {
	int num_fails = 0;
	struct Unicorn vortex_tmp = basedeck[87];

	current_players = 2;
	player[0].hand.cards[player[0].hand.num_cards++] = vortex_tmp;
	deck.size--; // to compensate for the vortex draw
	draw(0, 2);
	draw(1, 2);
	toggleFlags(0, yay_effect);
	int tmp_size = deck.size;

	assert(discardpile.size == 0);
	assert(player[0].hand.num_cards == 3);
	assert(player[1].hand.num_cards == 2);
	assert(player[0].flags == yay);
	playCard(0);

	if (turn_count != 1) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: turn count failed\n");
		reset_col();
	}

	if (player[0].hand.num_cards != 1 ||
			player[1].hand.num_cards != 1) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: hand size failed\n");
		reset_col();
	}

	if (discardpile.size != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: discard size failed\n");
		reset_col();
	}

	// deck.size = tmp_size + 1 (mystical vortex) + 1 (player[0]'s discarded card) + 1 (player[1]'s discarded card)
	if (deck.size != (tmp_size + 3)) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: deck size failed\n");
		reset_col();
	}

	reset_players();
	reset_deck();
	reset_discard();
	return num_fails;
}

// no cards to discard; the current player must have another card to discard
// in order to play this card
int mystical_vortex_empty_check() {
	int num_fails = 0;
	struct Unicorn vortex_tmp = basedeck[87];

	current_players = 2;
	draw(1, 1);
	int tmp_size = deck.size;
	player[0].hand.cards[player[0].hand.num_cards++] = vortex_tmp;

	int ret;
	assert(player[0].hand.num_cards == 1);
	ret = conditionalEffects(0, vortex_tmp, 0, 0);

	if (turn_count != 2 || ret != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    empty test: turn count failed\n");
		reset_col();
	}

	reset_players();
	reset_deck();
	reset_discard();
	return num_fails;
}

// Mystical Vortex
//
// Each player (including you) must DISCARD a card. Shuffle the discard pile into the deck.
int mystical_vortex_tests() {
	int num_fails = 0;

	rainbow_error("\nStarting Mystical Vortex tests...\n");

	// file input stream setup
	FILE* fp;
	fopen_s(&fp, "Tests/Input/line_1_1.txt", "r");
	if (fp == NULL) {
		magenta();
		fprintf(stderr, "    file input failed :(");
		reset_col();
		return 1;
	}
	fpinput = fp;

	if (!isclient) {
		num_fails += mystical_vortex_basic_check();
		num_fails += mystical_vortex_empty_check();
	}
	else {
		// input = 1; card index is actually 0
		int events;
		receiveInt(&events, sockfd);
		netStates[events].recvClient(1, sockfd);
	}

	fclose(fp);
	return num_fails;
}
