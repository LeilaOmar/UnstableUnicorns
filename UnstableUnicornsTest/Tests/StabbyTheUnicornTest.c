#include "MagicUnicornTests.h"
#include "networkevents.h"

// sanity check
int stabby_basic_check() {
	int num_fails = 0;
	struct Unicorn basic_tmp = basedeck[13]; // unicorn card
	struct Unicorn barbed_wire_tmp = basedeck[106]; // non-unicorn card
	struct Unicorn stabby_tmp = basedeck[40];

	// destroy worked
	current_players = 2;
	addStable(1, basic_tmp);
	sacrificeDestroyEffects(0, 0, stabby_tmp.effect);

	if (player[1].stable.num_unicorns != 0 || player[1].stable.size != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: destroy failed\n");
		reset_col();
	}
	reset_players();

	// no valid cards to destroy
	current_players = 2;
	addStable(1, barbed_wire_tmp);

	sacrificeDestroyEffects(0, 0, stabby_tmp.effect);
	if (player[1].stable.size != 1) {
		num_fails++;
		red();
		fprintf(stderr, "    non-unicorn test: destroy failed\n");
		reset_col();
	}

	reset_players();
	reset_discard();
	return num_fails;
}

// Stabby the Unicorn
//
// If this card is sacrificed or destroyed, you may DESTROY a Unicorn card
int stabby_the_unicorn_tests() {
	int num_fails = 0;

	rainbow_error("\nStarting Stabby the Unicorn tests...\n");
	if (!isclient) {
		// file input stream setup
		FILE* fp;
		fopen_s(&fp, "Tests/Input/stabbytheunicorn.txt", "r");
		if (fp == NULL) {
			magenta();
			fprintf(stderr, "    file input failed :(");
			reset_col();
			return 1;
		}
		fpinput = fp;

		num_fails += stabby_basic_check();

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
