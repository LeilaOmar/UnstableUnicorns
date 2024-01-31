#include "MagicUnicornTests.h"

// upgrade check
int chainsaw_upgrade_check() {
	int num_fails = 0;
	struct Unicorn basic_tmp = basedeck[13];
	struct Unicorn yay_tmp = basedeck[100];
	struct Unicorn chainsaw_tmp = basedeck[45];

	current_players = 2;
	addStable(0, chainsaw_tmp);
	addStable(1, basic_tmp);
	addStable(1, yay_tmp);

	int ncorn0 = player[0].stable.num_unicorns;
	int nsize0 = player[0].stable.size;
	int ncorn1 = player[1].stable.num_unicorns;
	int nsize1 = player[1].stable.size;

	enterStableEffects(0, player[0].stable.unicorns[0].effect);

	if (player[0].stable.num_unicorns != ncorn0 || player[0].stable.size != nsize0 ||
			player[1].stable.num_unicorns != ncorn1 || player[1].stable.size != (nsize1 - 1)) {
		num_fails++;
		red();
		fprintf(stderr, "    upgrade test: stable size failed\n");
		reset_col();
	}

	if (strcmp(discardpile.cards[0].name, yay_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    upgrade test: card validation failed\n");
		reset_col();
	}

	reset_players();
	reset_deck();
	reset_discard();
	return num_fails;
}

// downgrade check
int chainsaw_downgrade_check() {
	int num_fails = 0;
	struct Unicorn barbed_wire_tmp = basedeck[106];
	struct Unicorn chainsaw_tmp = basedeck[45];

	addStable(0, chainsaw_tmp);
	addStable(0, barbed_wire_tmp);

	int ncorn0 = player[0].stable.num_unicorns;
	int nsize0 = player[0].stable.size;

	enterStableEffects(0, player[0].stable.unicorns[0].effect);

	if (player[0].stable.num_unicorns != ncorn0 || player[0].stable.size != (nsize0 - 1)) {
		num_fails++;
		red();
		fprintf(stderr, "    downgrade test: stable size failed\n");
		reset_col();
	}

	if (strcmp(discardpile.cards[0].name, barbed_wire_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    downgrade test: card validation failed\n");
		reset_col();
	}

	reset_players();
	reset_deck();
	reset_discard();
	return num_fails;
}

// empty check
int chainsaw_empty_check() {
	int num_fails = 0;
	struct Unicorn yay_tmp = basedeck[100];
	struct Unicorn barbed_wire_tmp = basedeck[106];
	struct Unicorn chainsaw_tmp = basedeck[45];

	addStable(0, chainsaw_tmp);
	addStable(0, yay_tmp);
	addStable(1, barbed_wire_tmp);

	int ncorn0 = player[0].stable.num_unicorns;
	int nsize0 = player[0].stable.size;
	int ncorn1 = player[1].stable.num_unicorns;
	int nsize1 = player[1].stable.size;

	// shouldn't require any input, but this keeps it from getting borked
	fpinput = stdin;

	enterStableEffects(0, player[0].stable.unicorns[0].effect);

	if (player[0].stable.num_unicorns != ncorn0 || player[0].stable.size != nsize0) {
		num_fails++;
		red();
		fprintf(stderr, "    empty test: player[0] stable size failed\n");
		reset_col();
	}

	if (player[1].stable.num_unicorns != ncorn1 || player[1].stable.size != nsize1) {
		num_fails++;
		red();
		fprintf(stderr, "    empty test: player[1] stable size failed\n");
		reset_col();
	}

	if (discardpile.size != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    empty test: discard pile size failed\n");
		reset_col();
	}

	reset_players();
	reset_deck();
	reset_discard();
	return num_fails;
}

// Chainsaw Unicorn
//
// When this card enters your Stable, you may DESTROY an Upgrade card
// or SACRIFICE a Downgrade card.
int chainsaw_unicorn_tests() {
	int num_fails = 0;

	rainbow_error("\nStarting Chainsaw Unicorn tests...\n");

	// file input stream setup
	FILE* fp;
	fopen_s(&fp, "Tests/Input/chainsawunicorn.txt", "r");
	if (fp == NULL) {
		magenta();
		fprintf(stderr, "    file input failed :(");
		reset_col();
		return 1;
	}
	fpinput = fp;

	num_fails += chainsaw_upgrade_check();
	num_fails += chainsaw_downgrade_check();
	num_fails += chainsaw_empty_check();

	fclose(fp);
	return num_fails;
}
