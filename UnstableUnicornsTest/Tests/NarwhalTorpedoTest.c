#include "MagicUnicornTests.h"

// sanity check
int torpedo_basic_check() {
	int num_fails = 0;
	struct Unicorn torpedo_tmp = basedeck[62];
	struct Unicorn barbed_tmp = basedeck[106];
	struct Unicorn pandamonium_tmp = basedeck[107];
	struct Unicorn sadistic_tmp = basedeck[108];

	// this also checks to see if it works when
	// adding downgrade cards in the first and last indices,
	// even though the last should theoretically be impossible
	addStable(0, barbed_tmp);
	addStable(0, pandamonium_tmp);
	addStable(0, sadistic_tmp);

	assert(discardpile.size == 0);
	assert(player[0].stable.num_unicorns == 0);
	assert(player[0].stable.size == 3);
	addStable(0, torpedo_tmp);

	if (player[0].stable.size != 1 || player[0].stable.num_unicorns != 1) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: stable size failed\n");
		reset_col();
	}

	// sacrifices in this case are in reverse order
	if (discardpile.size != 3 ||
			strcmp(discardpile.cards[0].name, sadistic_tmp.name) != 0 ||
			strcmp(discardpile.cards[1].name, pandamonium_tmp.name) != 0 ||
			strcmp(discardpile.cards[2].name, barbed_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: sacrifice downgrade cards failed\n");
		reset_col();
	}

	// torpedo should be moved over to the first index in the stable
	if (strcmp(player[0].stable.unicorns[0].name, torpedo_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: stable unicorn[0] verification failed\n");
		reset_col();
	}

	reset_players();
	reset_discard();
	return num_fails;
}

// no available downgrade cards to destroy
int torpedo_empty_check() {
	int num_fails = 0;
	struct Unicorn torpedo_tmp = basedeck[62];
	struct Unicorn barbed_tmp = basedeck[106];
	struct Unicorn yay_tmp = basedeck[100];

	current_players = 2;
	addStable(0, yay_tmp);
	addStable(1, barbed_tmp);

	assert(discardpile.size == 0);
	assert(player[0].stable.num_unicorns == 0);
	assert(player[1].stable.size == 1);
	addStable(0, torpedo_tmp);

	if (player[0].stable.size != 2 || player[0].stable.num_unicorns != 1 ||
			player[1].stable.size != 1 || player[1].stable.num_unicorns != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    non-downgrade test: stable size failed\n");
		reset_col();
	}

	if (discardpile.size != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    non-downgrade test: discard size failed\n");
		reset_col();
	}

	reset_players();
	reset_discard();
	return num_fails;
}

// Narwhal Torpedo
//
// When this card enters your Stable, SACRIFICE all Downgrade cards in your Stable.
int narwhal_torpedo_tests() {
	int num_fails = 0;

	if (!isclient) {
		rainbow_error("\nStarting Narwhal Torpedo tests...\n");

		num_fails += torpedo_basic_check();
		num_fails += torpedo_empty_check();
	}
	return num_fails;
}
