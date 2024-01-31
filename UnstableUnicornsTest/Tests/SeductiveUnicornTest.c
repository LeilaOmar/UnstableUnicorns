#include "MagicUnicornTests.h"

// stealing a regular card
int seductive_basic_check() {
	int num_fails = 0;
	struct Unicorn basic_tmp = basedeck[13];
	struct Unicorn seductive_tmp = basedeck[49];

	current_players = 2;
	addStable(0, seductive_tmp);
	draw(0, 1);
	addStable(1, basic_tmp);

	int nhand0 = player[0].hand.num_cards;
	int nsize0 = player[0].stable.size;
	int ncorn0 = player[0].stable.num_unicorns;

	int nsize1 = player[1].stable.size;
	int ncorn1 = player[1].stable.num_unicorns;

	enterStableEffects(0, player[0].stable.unicorns[0].effect);

	assert(player[0].stable.num_unicorns == player[0].stable.size);
	assert(player[1].stable.num_unicorns == player[1].stable.size);

	if (player[0].stable.num_unicorns != (ncorn0 + 1) ||
			player[0].stable.size != (nsize0 + 1)) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: player[0] stable size failed\n");
		reset_col();
	}

	if (player[1].stable.num_unicorns != (ncorn1 - 1) ||
			player[1].stable.size != (nsize1 - 1)) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: player[1] stable size failed\n");
		reset_col();
	}

	assert(discardpile.size == 1);
	if (player[0].hand.num_cards != (nhand0 - 1)) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: player[0] hand size failed\n");
		reset_col();
	}

	reset_players();
	reset_deck();
	reset_discard();
	return num_fails;
}

// stealing a magic unicorn w/ an "entering your stable" effect
// this should probably fall under the steal tests...
int seductive_special_check() {
	int num_fails = 0;
	struct Unicorn basic_tmp = basedeck[13];
	struct Unicorn rainbow_tmp = basedeck[42]; // brings basic unicorn from hand to stable
	struct Unicorn seductive_tmp = basedeck[49];

	current_players = 2;
	addStable(0, seductive_tmp);
	addStable(1, rainbow_tmp);

	draw(0, 1);
	player[0].hand.cards[player[0].hand.num_cards++] = basic_tmp;

	int nhand0 = player[0].hand.num_cards;
	int nsize0 = player[0].stable.size;
	int ncorn0 = player[0].stable.num_unicorns;

	enterStableEffects(0, player[0].stable.unicorns[0].effect);

	assert(player[0].stable.num_unicorns == player[0].stable.size);

	if (player[0].stable.num_unicorns != (ncorn0 + 2) ||
			player[0].stable.size != (nsize0 + 2)) {
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

// unable to steal anything because the other player has non-unicorns
int seductive_empty_check() {
	int num_fails = 0;
	struct Unicorn yay_tmp = basedeck[100];
	struct Unicorn barbed_wire_tmp = basedeck[106];
	struct Unicorn seductive_tmp = basedeck[49];

	current_players = 2;
	draw(0, 1);
	addStable(0, seductive_tmp);
	addStable(1, barbed_wire_tmp);
	addStable(1, yay_tmp);

	int nhand0 = player[0].hand.num_cards;
	int nsize0 = player[0].stable.size;
	int ncorn0 = player[0].stable.num_unicorns;

	enterStableEffects(0, player[0].stable.unicorns[0].effect);

	assert(player[0].stable.num_unicorns == player[0].stable.size);
	assert(discardpile.size == 0);

	if (player[0].stable.num_unicorns != ncorn0 ||
			player[0].stable.size != nsize0) {
		num_fails++;
		red();
		fprintf(stderr, "    non-unicorn test: stable size failed\n");
		reset_col();
	}

	reset_players();
	reset_deck();
	reset_discard();
	return num_fails;
}

// Seductive Unicorn
//
// When this card enters your Stable, you may DISCARD a card,
// then STEAL a Unicorn card.
int seductive_unicorn_tests() {
	int num_fails = 0;

	rainbow_error("\nStarting Seductive Unicorn tests...\n");

	// file input stream setup
	FILE* fp;
	fopen_s(&fp, "Tests/Input/seductiveunicorn.txt", "r");
	if (fp == NULL) {
		magenta();
		fprintf(stderr, "    file input failed :(");
		reset_col();
		return 1;
	}
	fpinput = fp;

	num_fails += seductive_basic_check();
	num_fails += seductive_special_check();
	num_fails += seductive_empty_check();

	fclose(fp);
	return num_fails;
}
