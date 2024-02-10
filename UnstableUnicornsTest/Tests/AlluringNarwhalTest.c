#include "MagicUnicornTests.h"

// sanity check
int alluring_basic_check() {
	int num_fails = 0;
	struct Unicorn alluring_tmp = basedeck[63];
	struct Unicorn yay_tmp = basedeck[100];

	current_players = 2;
	addStable(0, yay_tmp);

	assert(player[0].stable.size == 1);
	assert(player[1].stable.num_unicorns == 0);
	assert(player[1].stable.size == 0);
	addStable(1, alluring_tmp);

	if (player[0].stable.size != 0 || player[0].stable.num_unicorns != 0 ||
			player[1].stable.size != 2 || player[1].stable.num_unicorns != 1) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: stable size failed\n");
		reset_col();
	}

	// sacrifices in this case are in reverse order
	if (strcmp(player[1].stable.unicorns[1].name, yay_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: unicorn verification failed\n");
		reset_col();
	}

	reset_players();
	return num_fails;
}

// no available upgrade cards to steal
int alluring_empty_check() {
	int num_fails = 0;
	struct Unicorn alluring_tmp = basedeck[63];
	struct Unicorn yay_tmp = basedeck[100];
	struct Unicorn basic_tmp = basedeck[13];

	current_players = 2;
	addStable(0, yay_tmp);
	addStable(1, basic_tmp);

	assert(player[0].stable.num_unicorns == 0);
	assert(player[0].stable.size == 1);
	assert(player[1].stable.size == 1);
	addStable(0, alluring_tmp);

	if (player[0].stable.size != 2 || player[0].stable.num_unicorns != 1 ||
			player[1].stable.size != 1 || player[1].stable.num_unicorns != 1) {
		num_fails++;
		red();
		fprintf(stderr, "    non-upgrade test: stable size failed\n");
		reset_col();
	}

	reset_players();
	return num_fails;
}

// Alluring Narwhal
//
// When this card enters your Stable, you may STEAL an Upgrade card.
int alluring_narwhal_tests() {
	int num_fails = 0;

	if (!isclient) {
		rainbow_error("\nStarting Alluring Narwhal tests...\n");

		// file input stream setup
		FILE* fp;
		fopen_s(&fp, "Tests/Input/line_1_1.txt", "r");
		if (fp == NULL) {
			magenta();
			fprintf(stderr, "    file input failed :(");
			reset_col();
			return 1;
		}
		fpinput = fp;

		num_fails += alluring_basic_check();
		num_fails += alluring_empty_check();

		fclose(fp);
	}
	return num_fails;
}
