#include "MagicUnicornTests.h"

// sanity check
int zombie_basic_check() {
	int num_fails = 0;
	struct Unicorn basic_tmp = basedeck[13];
	struct Unicorn zombie_tmp = basedeck[43];

	addStable(0, zombie_tmp);
	addDiscard(basic_tmp);
	draw(0, 1);
	player[0].hand.cards[player[0].hand.num_cards++] = basic_tmp;

	int tmp_stable_size = player[0].stable.size;
	int tmp_discard_size = discardpile.size;

	beginningTurnEffects(0, player[0].stable.unicorns[0]);

	if (discardpile.size != tmp_discard_size) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: discard pile size failed\n");
		reset_col();
	}

	if (player[0].stable.num_unicorns != (tmp_stable_size + 1)) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: stable size failed\n");
		reset_col();
	}

	if (turn_count != -1) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: turn count failed\n");
		reset_col();
	}

	reset_players();
	reset_deck();
	reset_discard();
	return num_fails;
}

int zombie_invalid_hand_check() {
	int num_fails = 0;
	struct Unicorn basic_tmp = basedeck[13];
	struct Unicorn zombie_tmp = basedeck[43];

	addStable(0, zombie_tmp);
	addDiscard(basic_tmp);
	draw(0, 1); // this is a super neigh and not a unicorn card

	int tmp_stable_size = player[0].stable.size;
	int tmp_discard_size = discardpile.size;

	beginningTurnEffects(0, player[0].stable.unicorns[0]);

	if (discardpile.size != tmp_discard_size) {
		num_fails++;
		red();
		fprintf(stderr, "    invalid hand test: discard pile size failed\n");
		reset_col();
	}

	if (player[0].stable.num_unicorns != tmp_stable_size) {
		num_fails++;
		red();
		fprintf(stderr, "    invalid hand test: unicorn size failed\n");
		reset_col();
	}

	if (turn_count <= 0) {
		num_fails++;
		red();
		fprintf(stderr, "    invalid hand test: turn count failed\n");
		reset_col();
	}

	reset_players();
	reset_deck();
	reset_discard();
	return num_fails;
}

int zombie_invalid_discard_check() {
	int num_fails = 0;
	struct Unicorn basic_tmp = basedeck[13];
	struct Unicorn yay_tmp = basedeck[100];
	struct Unicorn zombie_tmp = basedeck[43];

	addStable(0, zombie_tmp);
	addDiscard(yay_tmp);
	player[0].hand.cards[player[0].hand.num_cards++] = basic_tmp;

	int tmp_stable_size = player[0].stable.size;
	int tmp_discard_size = discardpile.size;

	beginningTurnEffects(0, player[0].stable.unicorns[0]);

	if (discardpile.size != tmp_discard_size) {
		num_fails++;
		red();
		fprintf(stderr, "    invalid discard pile test 1 failed\n");
		reset_col();
	}

	if (player[0].stable.num_unicorns != tmp_stable_size) {
		num_fails++;
		red();
		fprintf(stderr, "    invalid discard pile test 2 failed\n");
		reset_col();
	}

	if (turn_count <= 0) {
		num_fails++;
		red();
		fprintf(stderr, "    invalid discard pile test 3 failed\n");
		reset_col();
	}

	reset_players();
	reset_deck();
	reset_discard();
	return num_fails;
}

// Zombie Unicorn
//
// If this card is in your Stable at the beginning of your turn, you may DISCARD a Unicorn card.
// If you do, choose a Unicorn card from the discard pile and bring it directly into your Stable,
// then immediately skip to your End of Turn Phase
int zombie_unicorn_tests() {
	int num_fails = 0;

	if (!isclient) {
		rainbow_error("\nStarting Zombie Unicorn tests...\n");

		// file input stream setup
		FILE* fp;
		fopen_s(&fp, "Tests/Input/zombieunicorn.txt", "r");
		if (fp == NULL) {
			magenta();
			fprintf(stderr, "    file input failed :(");
			reset_col();
			return 1;
		}
		fpinput = fp;

		num_fails += zombie_basic_check();
		num_fails += zombie_invalid_hand_check();
		num_fails += zombie_invalid_discard_check();

		fclose(fp);
	}
	return num_fails;
}
