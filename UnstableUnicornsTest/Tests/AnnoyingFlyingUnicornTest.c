#include "MagicUnicornTests.h"

// sanity check
int annoying_basic_check() {
	int num_fails = 0;
	struct Unicorn basic_tmp = basedeck[13];
	struct Unicorn basic_tmp2 = basedeck[17];
	struct Unicorn annoying_tmp = basedeck[53];

	current_players = 2;
	player[0].hand.cards[player[0].hand.num_cards++] = basic_tmp2;
	player[1].hand.cards[player[1].hand.num_cards++] = basic_tmp;

	assert(player[0].hand.num_cards == 1);
	assert(player[1].hand.num_cards == 1);
	assert(discardpile.size == 0);
	addStable(0, annoying_tmp);

	if (player[0].hand.num_cards != 1 || player[1].hand.num_cards != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: hand size failed\n");
		reset_col();
	}
	if (discardpile.size != 1 ||
			strcmp(discardpile.cards[0].name, basic_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: discard size failed\n");
		reset_col();
	}
	reset_players();
	reset_discard();

	// no players to discard; a proper test would check if stdout is empty
	current_players = 2;
	player[0].hand.num_cards = 0;
	player[1].hand.num_cards = 0;

	addStable(0, annoying_tmp);

	if (player[1].hand.num_cards != 0 || discardpile.size != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: discard skip failed\n");
		reset_col();
	}
	reset_players();

	// check the sacrifice/destroy effects

	player[0].stable.unicorns[0] = annoying_tmp;

	assert(player[0].hand.num_cards == 0);
	sacrificeDestroyEffects(0, 0, annoying_tmp.effect);

	if (player[0].hand.num_cards != 1 ||
			strcmp(player[0].hand.cards[0].name, annoying_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: hand return failed\n");
		reset_col();
	}

	reset_players();
	return num_fails;
}

// Annoying Flying Unicorn
//
// When this card enters your Stable, you may force another player to DISCARD a card.
// If this card is sacrificed or destroyed, return it to your hand.
int annoying_flying_unicorn_tests() {
	int num_fails = 0;

	rainbow_error("\nStarting Annoying Flying Unicorn tests...\n");

	// file input stream setup
	FILE* fp;
	fopen_s(&fp, "Tests/Input/annoyingflyingunicorn.txt", "r");
	if (fp == NULL) {
		magenta();
		fprintf(stderr, "    file input failed :(");
		reset_col();
		return 1;
	}
	fpinput = fp;

	num_fails += annoying_basic_check();

	fclose(fp);
	return num_fails;
}
