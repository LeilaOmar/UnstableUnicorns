#include "MagicTests.h"

// sanity test
int shake_up_basic_check() {
	int num_fails = 0;
	struct Unicorn shake_tmp = basedeck[89];
	struct Unicorn basic_tmp = basedeck[13];

	player[0].hand.cards[player[0].hand.num_cards++] = shake_tmp;
	draw(0, 3);
	deck.size -= 2;
	addDiscard(basic_tmp);

	int tmp_size = deck.size;
	assert(discardpile.size == 1);
	assert(player[0].hand.num_cards == 4);
	magicEffects(0, shake_tmp.effect);

	if (player[0].hand.num_cards != 5) {
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

	// deck.size = tmp_size + 1 (prev discardpile.size) + 1 (shake up) + 3 (rest of player[0]'s hand) - 5 (drawn cards)
	// deck.size = tmp_size + 5 - 5 = tmp_size
	if (deck.size != tmp_size) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: deck size failed\n");
		reset_col();
	}
	reset_players();
	reset_deck();
	reset_discard();

	// with shake up as the only card in your hand
	player[0].hand.cards[player[0].hand.num_cards++] = shake_tmp;
	deck.size -= 2;
	addDiscard(basic_tmp);

	tmp_size = deck.size;
	assert(discardpile.size == 1);
	assert(player[0].hand.num_cards == 1);
	magicEffects(0, shake_tmp.effect);

	if (player[0].hand.num_cards != 5) {
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

	// deck.size = tmp_size + 1 (prev discardpile.size) + 1 (shake up) + 0 (rest of player[0]'s hand) - 5 (drawn cards)
	// deck.size = tmp_size + 2 - 5 = tmp_size - 3
	if (deck.size != (tmp_size - 3)) {
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

// Shake Up
//
// Shuffle this card, your hand, and the discard pile into the deck. DRAW 5 cards.
int shake_up_tests() {
	int num_fails = 0;

	if (!isclient) {
		rainbow_error("\nStarting Shake Up tests...\n");

		num_fails += shake_up_basic_check();
	}
	return num_fails;
}
