#include "resetfns.h"
#include "basedeck.h"

// ********************************************************************************
// *************************** Save and Reset Functions ***************************
// ********************************************************************************

void reset_players() {
	for (int i = 0; i < current_players; i++) {
		player[i].hand.num_cards = 0;
		player[i].stable.num_unicorns = 0;
		player[i].stable.size = 0;
		player[i].flags = 0;
		memset(player[i].stable.unicorns, 0, sizeof player[i].stable.unicorns);
		memset(player[i].hand.cards, 0, sizeof player[i].hand.cards);
	}

	current_players = 1;
	turn_count = 1;
}

void reset_nursery() {
	int counter = 0;

	for (int i = 0; i < NURSERY_SIZE; i++) {
		nursery.cards[i] = basedeck[i];
		nursery.cards[i].id = counter;
		counter++;
	}
	nursery.size = NURSERY_SIZE;
}

void reset_deck() {
	int counter = NURSERY_SIZE;

	for (int i = 0; i < DECK_SIZE; i++) {
		deck.cards[i] = basedeck[i + NURSERY_SIZE];
		deck.cards[i].id = counter;
		counter++;
	}
	deck.size = DECK_SIZE;
}

void reset_discard() {
	memset(discardpile.cards, 0, DECK_SIZE);
	discardpile.size = 0;
}

void rainbow_error(char* str) {
	unsigned int color_index = 0;
	for (size_t i = 0; i < strlen(str); i++) {
		if (color_index % 6 == 0)
			red();
		else if (color_index % 6 == 1)
			yellow();
		else if (color_index % 6 == 2)
			green();
		else if (color_index % 6 == 3)
			cyan();
		else if (color_index % 6 == 4)
			blue();
		else
			magenta();

		fprintf(stderr, "%c", str[i]);

		if (str[i] != '\n' && str[i] != ' ') color_index++;
	}
	reset_col();
}
