#include "MagicUnicornTests.h"

// stealing a regular card
int angel_basic_check() {
	int num_fails = 0;
	struct Unicorn basic_tmp = basedeck[13];
	struct Unicorn angel_tmp = basedeck[50];

	addStable(0, angel_tmp);
	addDiscard(basic_tmp);

	int nsize0 = player[0].stable.size;
	int ncorn0 = player[0].stable.num_unicorns;

	assert(discardpile.size == 1);

	beginningTurnEffects(0, player[0].stable.unicorns[0]);

	assert(player[0].stable.num_unicorns == player[0].stable.size);

	if (player[0].stable.num_unicorns != ncorn0 ||
			player[0].stable.size != nsize0) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: player size failed\n");
		reset_col();
	}

	// the angel unicorn card is discarded after being sacrificed
	if (discardpile.size != 1) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: discard pile size failed\n");
		reset_col();
	}

	// angel and basic basically swapped places, so the basic unicorn should
	// now be in the player's stable, and angel unicorn should be in the discard pile
	if (strcmp(player[0].stable.unicorns[0].name, basic_tmp.name) != 0 ||
			strcmp(discardpile.cards[0].name, angel_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: unicorn verification failed\n");
		reset_col();
	}

	reset_players();
	reset_deck();
	reset_discard();
	return num_fails;
}

// taking a magic unicorn w/ an "entering your stable" effect
int angel_special_check() {
	int num_fails = 0;
	struct Unicorn basic_tmp = basedeck[13];
	struct Unicorn rainbow_tmp = basedeck[42]; // brings basic unicorn from hand to stable
	struct Unicorn angel_tmp = basedeck[50];

	addStable(0, angel_tmp);
	addDiscard(rainbow_tmp);

	player[0].hand.cards[player[0].hand.num_cards++] = basic_tmp;

	int nhand0 = player[0].hand.num_cards;
	int nsize0 = player[0].stable.size;
	int ncorn0 = player[0].stable.num_unicorns;

	beginningTurnEffects(0, player[0].stable.unicorns[0]);

	assert(player[0].stable.num_unicorns == player[0].stable.size);

	if (player[0].stable.num_unicorns != (ncorn0 + 1) ||
			player[0].stable.size != (nsize0 + 1) ||
			player[0].hand.num_cards != (nhand0 - 1)) {
		num_fails++;
		red();
		fprintf(stderr, "    special test: rainbow unicorn effect failed\n");
		reset_col();
	}

	reset_players();
	reset_deck();
	reset_discard();
	return num_fails;
}

// unable to sacrifice because there are no unicorns in the discard pile
int angel_empty_check() {
	int num_fails = 0;
	struct Unicorn yay_tmp = basedeck[100];
	struct Unicorn barbed_wire_tmp = basedeck[106];
	struct Unicorn angel_tmp = basedeck[50];

	addStable(0, angel_tmp);
	addDiscard(barbed_wire_tmp);
	addDiscard(yay_tmp);

	int nhand0 = player[0].hand.num_cards;
	int nsize0 = player[0].stable.size;
	int ncorn0 = player[0].stable.num_unicorns;

	beginningTurnEffects(0, player[0].stable.unicorns[0]);

	assert(player[0].stable.num_unicorns == player[0].stable.size);

	if (player[0].stable.num_unicorns != ncorn0 ||
			player[0].stable.size != nsize0) {
		num_fails++;
		red();
		fprintf(stderr, "    non-unicorn test: stable size failed\n");
		reset_col();
	}

	if (strcmp(player[0].stable.unicorns[0].name, angel_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    non-unicorn test: unicorn verification failed\n");
		reset_col();
	}

	reset_players();
	reset_deck();
	reset_discard();
	return num_fails;
}

// Angel Unicorn
//
// If this card is in your Stable at the beginning of your turn, you may SACRIFICE this card.
// If you do, choose a Unicorn card from the discard pile and bring it directly into your Stable.
int angel_unicorn_tests() {
	int num_fails = 0;

	if (!isclient) {
		rainbow_error("\nStarting Angel Unicorn tests...\n");

		// file input stream setup
		FILE* fp;
		fopen_s(&fp, "Tests/Input/angelunicorn.txt", "r");
		if (fp == NULL) {
			magenta();
			fprintf(stderr, "    file input failed :(");
			reset_col();
			return 1;
		}
		fpinput = fp;

		num_fails += angel_basic_check();
		num_fails += angel_special_check();
		num_fails += angel_empty_check();

		fclose(fp);
	}
	return num_fails;
}
