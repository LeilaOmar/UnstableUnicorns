#include "UpgradeTests.h"

// sanity test
int summoning_basic_check() {
	int num_fails = 0;
	struct Unicorn summon_tmp = basedeck[105];
	struct Unicorn rainbow_tmp = basedeck[42];
	struct Unicorn basic_tmp = basedeck[13];
	struct Unicorn basic_tmp2 = basedeck[17]; // basic unicorn
	struct Unicorn ginormous_tmp = basedeck[48]; // magic unicorn

	addStable(0, summon_tmp);
	addDiscard(rainbow_tmp);
	player[0].hand.cards[player[0].hand.num_cards++] = basic_tmp;
	player[0].hand.cards[player[0].hand.num_cards++] = basic_tmp2;
	player[0].hand.cards[player[0].hand.num_cards++] = ginormous_tmp;

	assert(discardpile.size == 1);
	assert(player[0].hand.num_cards == 3);
	assert(player[0].stable.size == 1);
	beginningTurnEffects(0, summon_tmp);

	if (player[0].hand.num_cards != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: hand size failed\n");
		reset_col();
	}

	if (player[0].stable.size != 3 || player[0].stable.num_unicorns != 2 ||
			strcmp(player[0].stable.unicorns[0].name, summon_tmp.name) != 0 ||
			strcmp(player[0].stable.unicorns[1].name, rainbow_tmp.name) != 0 ||
			strcmp(player[0].stable.unicorns[2].name, basic_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: stable verification failed\n");
		reset_col();
	}

	if (discardpile.size != 2 ||
			strcmp(discardpile.cards[0].name, basic_tmp2.name) != 0 ||
			strcmp(discardpile.cards[1].name, ginormous_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: discard pile verification failed\n");
		reset_col();
	}

	reset_players();
	reset_discard();
	return num_fails;
}

// no cards in discard pile; this should still work because the cards you discard
// are eventually included
int summoning_empty_discard_check() {
	int num_fails = 0;
	struct Unicorn summon_tmp = basedeck[105];
	struct Unicorn basic_tmp = basedeck[13];
	struct Unicorn ginormous_tmp = basedeck[48]; // magic unicorn

	addStable(0, summon_tmp);
	player[0].hand.cards[player[0].hand.num_cards++] = basic_tmp;
	player[0].hand.cards[player[0].hand.num_cards++] = ginormous_tmp;

	assert(discardpile.size == 0);
	assert(player[0].hand.num_cards == 2);
	assert(player[0].stable.size == 1);
	assert(player[0].flags == 0);
	beginningTurnEffects(0, summon_tmp);

	if (player[0].hand.num_cards != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    empty discard test: hand size failed\n");
		reset_col();
	}

	if (player[0].stable.size != 2 || player[0].stable.num_unicorns != 1 ||
			strcmp(player[0].stable.unicorns[0].name, summon_tmp.name) != 0 ||
			strcmp(player[0].stable.unicorns[1].name, ginormous_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    empty discard test: stable verification failed\n");
		reset_col();
	}

	if (discardpile.size != 1 ||
			strcmp(discardpile.cards[0].name, basic_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    empty discard test: discard pile verification failed\n");
		reset_col();
	}

	if (player[0].flags != ginormous_unicorn) {
		num_fails++;
		red();
		fprintf(stderr, "    empty discard test: toggle flags failed\n");
		reset_col();
	}

	reset_players();
	reset_discard();
	return num_fails;
}

// not enough unicorn cards in hand
int summoning_empty_hand_check() {
	int num_fails = 0;
	struct Unicorn summon_tmp = basedeck[105];
	struct Unicorn basic_tmp = basedeck[13];
	struct Unicorn yay_tmp = basedeck[100];

	addStable(0, summon_tmp);
	player[0].hand.cards[player[0].hand.num_cards++] = basic_tmp;
	player[0].hand.cards[player[0].hand.num_cards++] = yay_tmp;

	assert(discardpile.size == 0);
	assert(player[0].hand.num_cards == 2);
	assert(player[0].stable.size == 1);
	beginningTurnEffects(0, summon_tmp);

	if (player[0].hand.num_cards != 2) {
		num_fails++;
		red();
		fprintf(stderr, "    empty hand test: hand size failed\n");
		reset_col();
	}

	if (player[0].stable.size != 1 || player[0].stable.num_unicorns != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    empty hand test: stable size failed\n");
		reset_col();
	}

	if (discardpile.size != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    empty hand test: discard pile size failed\n");
		reset_col();
	}

	reset_players();
	reset_discard();
	return num_fails;
}

// Summoning Ritual
//
// If this card is in your Stable at the beginning of your turn, you may DISCARD 2 Unicorn cards.
// If you do, bring a Unicorn card directly from the discard pile into your Stable.
// (note) you can choose to directly bring one of the two discarded cards into your stable
int summoning_ritual_tests() {
	int num_fails = 0;

	rainbow_error("\nStarting Summoning Ritual tests...\n");

	// file input stream setup
	FILE* fp;
	fopen_s(&fp, "Tests/Input/summoningritual.txt", "r");
	if (fp == NULL) {
		magenta();
		fprintf(stderr, "    file input failed :(");
		reset_col();
		return 1;
	}
	fpinput = fp;

	num_fails += summoning_basic_check();
	num_fails += summoning_empty_discard_check();
	num_fails += summoning_empty_hand_check();

	fclose(fp);
	return num_fails;
}
