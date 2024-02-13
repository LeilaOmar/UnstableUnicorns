#include "MagicUnicornTests.h"
#include "networkevents.h"

// sanity check
int kittencorn_basic_check() {
	int num_fails = 0;
	struct Unicorn kitten_tmp = basedeck[39];

	current_players = 2;
	addStable(1, kitten_tmp);

	// destroyed by a magic card; should not work
	if (canBeDestroyed(1, 0, ANY, TRUE)) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: canBeDestroyed failed\n");
		reset_col();
	}
	reset_players();

	// destroyed by a non-magic card
	current_players = 2;
	addStable(1, kitten_tmp);

	destroy(0, ANY, FALSE);

	if (player[1].stable.size != 0 || player[1].stable.num_unicorns != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: destruction by non-magic card failed\n");
		reset_col();
	}

	reset_players();
	return num_fails;
}

// Magical Kittencorn
//
// This card cannot be destroyed by Magic cards.
int magical_kittencorn_tests() {
	int num_fails = 0;

	rainbow_error("\nStarting Magical Kittencorn tests...\n");
	if (!isclient) {

		// file input stream setup
		FILE* fp;
		fopen_s(&fp, "Tests/Input/line_2_1.txt", "r");
		if (fp == NULL) {
			magenta();
			fprintf(stderr, "    file input failed :(");
			reset_col();
			return 1;
		}
		fpinput = fp;

		num_fails += kittencorn_basic_check();

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
