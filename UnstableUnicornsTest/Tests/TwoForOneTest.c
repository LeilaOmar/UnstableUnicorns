#include "MagicTests.h"
#include "networkevents.h"

// sanity check
int twofer_basic_check() {
	int num_fails = 0;
	struct Unicorn twofer_tmp = basedeck[83];
	struct Unicorn basic_tmp = basedeck[13];
	struct Unicorn panda_tmp = basedeck[107];
	struct Unicorn basic_tmp3 = basedeck[20];
	struct Unicorn yay_tmp = basedeck[100];

	current_players = 3;
	addStable(0, basic_tmp);
	addStable(0, yay_tmp);
	addStable(1, panda_tmp);
	addStable(1, basic_tmp3);
	toggleFlags(0, yay_effect);
	player[0].hand.cards[player[0].hand.num_cards++] = twofer_tmp;

	assert(discardpile.size == 0);
	assert(player[0].hand.num_cards == 1);
	assert(player[0].stable.size == 2);
	assert(player[1].stable.size == 2);
	assert(player[0].flags == yay);
	playCard(0);

	if (turn_count != 1) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: turn count failed\n");
		reset_col();
	}

	if (player[0].stable.size != 1 || player[0].stable.num_unicorns != 0 ||
			player[1].stable.size != 0 || player[1].stable.num_unicorns != 0 ) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: stable size failed\n");
		reset_col();
	}

	if (discardpile.size != 4 ||
			strcmp(discardpile.cards[0].name, twofer_tmp.name) != 0 ||
			strcmp(discardpile.cards[1].name, basic_tmp.name) != 0 ||
			strcmp(discardpile.cards[2].name, panda_tmp.name) != 0 ||
			strcmp(discardpile.cards[3].name, basic_tmp3.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: discard pile verification failed\n");
		reset_col();
	}

	reset_players();
	reset_deck();
	reset_discard();
	return num_fails;
}

// no cards to sacrifice or destroy
int twofer_empty_check() {
	int num_fails = 0;
	struct Unicorn twofer_tmp = basedeck[83];
	struct Unicorn puppicorn_tmp = basedeck[41]; // cannot be destroyed, periodt.
	struct Unicorn kitten_tmp = basedeck[39]; // cannot be destroyed by magic cards
	struct Unicorn basic_tmp = basedeck[13]; // [emojis]

	// no cards to sacrifice
	current_players = 2;
	addStable(0, puppicorn_tmp);
	addStable(1, basic_tmp);
	addStable(1, basic_tmp);
	player[0].hand.cards[player[0].hand.num_cards++] = twofer_tmp;

	int ret;
	assert(player[0].stable.size == 1);
	assert(player[1].stable.size == 2);
	ret = conditionalEffects(0, twofer_tmp, 0, 0);

	if (turn_count != 2 || ret != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    empty sacrifice test: turn count failed\n");
		reset_col();
	}
	reset_players();
	reset_discard();

	// no cards to destroy
	current_players = 2;
	addStable(0, basic_tmp);
	addStable(1, puppicorn_tmp);
	addStable(1, kitten_tmp);
	player[0].hand.cards[player[0].hand.num_cards++] = twofer_tmp;

	assert(player[0].stable.size == 1);
	assert(player[1].stable.size == 2);
	ret = conditionalEffects(0, twofer_tmp, 0, 0);

	if (turn_count != 2 || ret != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    empty destroy test: turn count failed\n");
		reset_col();
	}
	reset_players();
	reset_discard();

	// only one card available to destroy
	current_players = 2;
	addStable(0, kitten_tmp);
	addStable(1, puppicorn_tmp);
	addStable(1, basic_tmp);
	player[0].hand.cards[player[0].hand.num_cards++] = twofer_tmp;

	assert(player[0].stable.size == 1);
	assert(player[1].stable.size == 2);
	ret = conditionalEffects(0, twofer_tmp, 0, 0);

	if (turn_count != 2 || ret != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    incomplete destroy test: turn count failed\n");
		reset_col();
	}

	reset_players();
	reset_discard();
	return num_fails;
}

// Two-For-One
//
// SACRIFICE a card, then DESTROY 2 cards.
int two_for_one_tests() {
	int num_fails = 0;

	rainbow_error("\nStarting Two-For-One tests...\n");
	if (!isclient) {
		// file input stream setup
		FILE* fp;
		fopen_s(&fp, "Tests/Input/twoforone.txt", "r");
		if (fp == NULL) {
			magenta();
			fprintf(stderr, "    file input failed :(");
			reset_col();
			return 1;
		}
		fpinput = fp;

		num_fails += twofer_basic_check();
		num_fails += twofer_empty_check();

		fclose(fp);
	}
	else {
		// basic check, no input
		int events;
		receiveInt(&events, sockfd);
		netStates[events].recvClient(1, sockfd);
	}
	return num_fails;
}
