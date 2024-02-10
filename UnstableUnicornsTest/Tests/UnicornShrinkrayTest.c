#include "MagicTests.h"

// sanity check
int shrinkray_basic_check() {
	int num_fails = 0;
	struct Unicorn shrinkray_tmp = basedeck[85];
	struct Unicorn basic_tmp = basedeck[13]; // [emojis]
	struct Unicorn yay_tmp = basedeck[100]; // non-unicorn card
	struct Unicorn ginormous_tmp = basedeck[48]; // speccial effect 1
	struct Unicorn queen_tmp = basedeck[51]; // special effect 2

	// no cards to sacrifice
	current_players = 3;
	addStable(1, basic_tmp);
	addStable(1, basic_tmp);
	addStable(1, ginormous_tmp);
	addStable(1, yay_tmp);
	addStable(1, queen_tmp);
	enterStableEffects(1, ginormous_tmp.effect);
	enterStableEffects(1, queen_tmp.effect);

	nursery.size = 4;
	int tmp_size = nursery.size;
	player[0].hand.cards[player[0].hand.num_cards++] = shrinkray_tmp;

	int ret;
	assert(discardpile.size == 0);
	assert(player[0].hand.num_cards == 1);
	assert(player[1].stable.size == 5);
	assert(player[1].stable.num_unicorns == 4);
	assert(player[2].stable.size == 0);

	assert(player[0].flags == queen_bee_unicorn);
	assert(player[1].flags == ginormous_unicorn);
	assert(player[2].flags == queen_bee_unicorn);
	ret = conditionalEffects(0, shrinkray_tmp, 0, 0);
	if (ret) magicEffects(0, shrinkray_tmp.effect);

	if (turn_count != 1 || ret != 1) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: turn count failed\n");
		reset_col();
	}

	if (player[1].stable.size != 5 || player[1].stable.num_unicorns != 4 ||
			player[1].stable.unicorns[0].cType != BABYUNICORN ||
			strcmp(player[1].stable.unicorns[3].name, yay_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: stable verification failed\n");
		reset_col();
	}

	// discardpile.size = player[1].stable.num_unicorns
	// normally it would include the shrinkray card, but magicEffects is simpler
	if (discardpile.size != 4 ||
			strcmp(discardpile.cards[0].name, basic_tmp.name) != 0 ||
			strcmp(discardpile.cards[1].name, basic_tmp.name) != 0 ||
			strcmp(discardpile.cards[2].name, ginormous_tmp.name) != 0 ||
			strcmp(discardpile.cards[3].name, queen_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: discard verification failed\n");
		reset_col();
	}

	// nursery.size = tmp_size - player[1].stable.num_unicorns
	if (nursery.size != (tmp_size - 4)) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: nursery size failed\n");
		reset_col();
	}

	// flags should be disabled
	if (player[0].flags != 0 ||
			player[1].flags != 0 ||
			player[2].flags != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: toggle flags failed\n");
		reset_col();
	}

	reset_players();
	reset_nursery();
	reset_discard();
	return num_fails;
}

// not enough baby unicorns to swap
int shrinkray_empty_check() {
	int num_fails = 0;
	struct Unicorn shrinkray_tmp = basedeck[85];
	struct Unicorn basic_tmp = basedeck[13];
	struct Unicorn yay_tmp = basedeck[100];

	// no cards to sacrifice
	current_players = 2;
	addStable(1, basic_tmp);
	addStable(1, basic_tmp);
	addStable(1, basic_tmp);
	addStable(1, yay_tmp);

	nursery.size = 2;
	int tmp_size = nursery.size;
	player[0].hand.cards[player[0].hand.num_cards++] = shrinkray_tmp;

	int ret;
	assert(player[1].stable.size == 4);
	assert(player[1].stable.num_unicorns == 3);
	ret = conditionalEffects(0, shrinkray_tmp, 0, 0);

	if (turn_count != 2 || ret != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    empty nursery test: turn count failed\n");
		reset_col();
	}

	reset_players();
	reset_nursery();
	reset_discard();
	return num_fails;
}

// checks if the puppicorn index gets adjusted
int shrinkray_puppicorn_check() {
	int num_fails = 0;
	struct Unicorn shrinkray_tmp = basedeck[85];
	struct Unicorn puppicorn_tmp = basedeck[41];
	struct Unicorn basic_tmp = basedeck[13];

	current_players = 2;
	int tmp_size = nursery.size;
	addStable(1, puppicorn_tmp);
	addStable(1, basic_tmp);
	player[0].hand.cards[player[0].hand.num_cards++] = shrinkray_tmp;

	assert(discardpile.size == 0);
	assert(player[0].hand.num_cards == 1);
	assert(player[0].stable.size == 0);
	assert(player[1].stable.size == 2);
	assert(player[1].stable.num_unicorns == 2);
	magicEffects(0, shrinkray_tmp.effect);

	if (player[1].stable.size != 2 || player[1].stable.num_unicorns != 2 ||
			player[1].stable.unicorns[0].cType != BABYUNICORN ||
			player[1].stable.unicorns[1].cType != BABYUNICORN) {
		num_fails++;
		red();
		fprintf(stderr, "    puppicorn test: stable verification failed\n");
		reset_col();
	}

	if (puppicorn_index[0] != -1) {
		num_fails++;
		red();
		fprintf(stderr, "    puppicorn test: puppicorn index failed\n");
		reset_col();
	}

	// does it move?
	endOfTurn(1);

	if (player[0].stable.size != 0 || player[0].stable.num_unicorns != 0 ||
			player[1].stable.size != 2 || player[1].stable.num_unicorns != 2 ||
			player[1].stable.unicorns[0].cType != BABYUNICORN ||
			player[1].stable.unicorns[1].cType != BABYUNICORN) {
		num_fails++;
		red();
		fprintf(stderr, "    puppicorn test: end of turn stable verification failed\n");
		reset_col();
	}

	reset_players();
	reset_nursery();
	reset_discard();
	return num_fails;
}

// essentially discard ALL of the hand
int shrinkray_barbed_wire_check() {
	int num_fails = 0;
	struct Unicorn shrinkray_tmp = basedeck[85];
	struct Unicorn basic_tmp = basedeck[13];
	struct Unicorn yay_tmp = basedeck[100];
	struct Unicorn barbed_tmp = basedeck[106];

	// no cards to sacrifice
	current_players = 2;
	addStable(1, basic_tmp);
	addStable(1, basic_tmp);
	addStable(1, basic_tmp);
	addStable(1, basic_tmp);
	addStable(1, basic_tmp);
	addStable(1, basic_tmp);

	draw(1, 6);
	player[1].hand.cards[player[1].hand.num_cards++] = yay_tmp;
	player[0].hand.cards[player[0].hand.num_cards++] = shrinkray_tmp;
	toggleFlags(1, barbed_tmp.effect);

	assert(discardpile.size == 0);
	assert(player[0].hand.num_cards == 1);
	assert(player[1].stable.size == 6);
	assert(player[1].stable.num_unicorns == 6);
	assert(player[1].hand.num_cards == 7);
	magicEffects(0, shrinkray_tmp.effect);

	if (player[1].hand.num_cards != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    barbed wire test: hand size failed\n");
		reset_col();
	}

	// discardpile.size = 1 (unicorn shrinkray) + player[1].stable.num_unicorns + min((player[1].stable.num_unicorns * 2), player[1].hand.num_cards)
	// discardpile.size = 1 + 6 + min(12, 7) = 1 + 6 + 7 = 14
	//
	// since this is excluding unicorn shrinkray, it should check for 13 cards
	if (discardpile.size != 13) {
		num_fails++;
		red();
		fprintf(stderr, "    barbed wire test: discard size failed\n");
		reset_col();
	}
	reset_players();
	reset_deck();
	reset_nursery();
	reset_discard();

	// test w/ cards still in hand by the end
	current_players = 2;
	addStable(1, basic_tmp);
	addStable(1, basic_tmp);

	draw(1, 4);
	player[1].hand.cards[player[1].hand.num_cards++] = yay_tmp;
	player[0].hand.cards[player[0].hand.num_cards++] = shrinkray_tmp;
	toggleFlags(1, barbed_tmp.effect);

	assert(discardpile.size == 0);
	assert(player[0].hand.num_cards == 1);
	assert(player[1].stable.size == 2);
	assert(player[1].stable.num_unicorns == 2);
	assert(player[1].hand.num_cards == 5);
	magicEffects(0, shrinkray_tmp.effect);

	if (player[1].hand.num_cards != 1 ||
			strcmp(player[1].hand.cards[0].name, yay_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    barbed wire test 2: hand verification failed\n");
		reset_col();
	}

	// discardpile.size = 1 (unicorn shrinkray) + player[1].stable.num_unicorns + min((player[1].stable.num_unicorns * 2), player[1].hand.num_cards)
	// discardpile.size = 1 + 2 + min(4, 5) = 1 + 2 + 4 = 7
	//
	// as explained above, just subtract 1 due to excluding unicorn shrinkray
	if (discardpile.size != 6) {
		num_fails++;
		red();
		fprintf(stderr, "    barbed wire test 2: discard size failed\n");
		reset_col();
	}

	reset_players();
	reset_deck();
	reset_nursery();
	reset_discard();
	return num_fails;
}

// compatibility w/ pandamonium
int shrinkray_pandamonium_check() {
	int num_fails = 0;
	struct Unicorn shrinkray_tmp = basedeck[85];
	struct Unicorn basic_tmp = basedeck[13];
	struct Unicorn panda_tmp = basedeck[107];

	current_players = 2;
	addStable(1, panda_tmp);
	addStable(1, basic_tmp);
	addStable(1, basic_tmp);
	addStable(1, basic_tmp);
	toggleFlags(1, panda_tmp.effect);

	int tmp_size = nursery.size;
	player[0].hand.cards[player[0].hand.num_cards++] = shrinkray_tmp;

	int ret;
	assert(player[1].stable.size == 4);
	assert(player[1].stable.num_unicorns == 3);
	assert(player[1].flags == pandamonium);
	ret = conditionalEffects(0, shrinkray_tmp, 0, 0);

	if (turn_count != 2 || ret != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    pandamonium test: turn count failed\n");
		reset_col();
	}

	reset_players();
	reset_nursery();
	reset_discard();
	return num_fails;
}

// Unicorn Shrinkray
//
// Choose any player. Move all of that player's Unicorn cards to the discard pile
// without triggering any of their effects, then bring the same number of Baby Unicorn
// cards from the Nursery directly into that player's Stable.
int unicorn_shrinkray_tests() {
	int num_fails = 0;

	if (!isclient) {
		rainbow_error("\nStarting Unicorn Shrinkray tests...\n");

		// file input stream setup
		FILE* fp;
		fopen_s(&fp, "Tests/Input/unicornshrinkray.txt", "r");
		if (fp == NULL) {
			magenta();
			fprintf(stderr, "    file input failed :(");
			reset_col();
			return 1;
		}
		fpinput = fp;

		num_fails += shrinkray_basic_check();
		num_fails += shrinkray_empty_check();
		num_fails += shrinkray_puppicorn_check();
		num_fails += shrinkray_barbed_wire_check();
		num_fails += shrinkray_pandamonium_check();

		fclose(fp);
	}
	return num_fails;
}
