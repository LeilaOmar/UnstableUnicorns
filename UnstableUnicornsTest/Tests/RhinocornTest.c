#include "MagicUnicornTests.h"
#include "networkevents.h"

int rhinocorn_basic_check() {
	int num_fails = 0;
	struct Unicorn basic_tmp = basedeck[13];
	struct Unicorn rhinocorn_tmp = basedeck[37];

	turn_count = 1;
	current_players = 2;
	addStable(0, rhinocorn_tmp);
	addStable(1, basic_tmp);

	beginningTurnEffects(0, player[0].stable.unicorns[0]);

	// destroy unicorn test
	if (player[1].stable.size != 0 || player[1].stable.num_unicorns != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: destroy unicorn failed\n");
		reset_col();
	}

	// assert end of turn phase
	if (turn_count > 0) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: end of turn failed\n");
		reset_col();
	}

	reset_players();
	return num_fails;
}

// Rhinocorn
// 
// If this card is in your Stable at the beginning of your turn, you may DESTROY a Unicorn card.
// If you do, immediately skip to your End of Turn phase.
int rhinocorn_tests() {
	int num_fails = 0;

	rainbow_error("\nStarting Rhinocorn tests...\n");
	if (!isclient) {
		// file input stream setup
		FILE* fp;
		fopen_s(&fp, "Tests/Input/rhinocorn.txt", "r");
		if (fp == NULL) {
			magenta();
			fprintf(stderr, "    file input failed :(");
			reset_col();
			return 1;
		}
		fpinput = fp;

		num_fails += rhinocorn_basic_check();

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
