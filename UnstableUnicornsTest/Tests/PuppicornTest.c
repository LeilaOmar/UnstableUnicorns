#include "MagicUnicornTests.h"
#include "networkevents.h"

// sanity check
int puppicorn_basic_check() {
	int num_fails = 0;
	struct Unicorn basic_tmp = basedeck[13];
	struct Unicorn basic_tmp2 = basedeck[17];
	struct Unicorn basic_tmp3 = basedeck[20];
	struct Unicorn basic_tmp4 = basedeck[23];
	struct Unicorn basic_tmp5 = basedeck[26];
	struct Unicorn basic_tmp6 = basedeck[29];
	struct Unicorn puppicorn_tmp = basedeck[41];

	reset_players();

	// does it move?
	current_players = 2;
	addStable(0, basic_tmp);
	addStable(0, basic_tmp2);
	addStable(0, basic_tmp3);
	addStable(0, puppicorn_tmp);
	addStable(1, basic_tmp4);
	addStable(1, basic_tmp5);

	assert(player[0].stable.size == 4);
	assert(player[1].stable.size == 2);
	assert(puppicorn_index[0] == 3);
	assert(puppicorn_index[1] == 0);
	endOfTurn(0);

	if (player[0].stable.num_unicorns != 3 || player[0].stable.size != 3) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: player[0] stable size failed\n");
		reset_col();
	}

	if (player[1].stable.num_unicorns != 3 || player[1].stable.size != 3) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: player[1] stable size failed\n");
		reset_col();
	}

	if (strcmp(player[0].stable.unicorns[0].name, basic_tmp.name) != 0 ||
			strcmp(player[0].stable.unicorns[1].name, basic_tmp2.name) != 0 ||
			strcmp(player[0].stable.unicorns[2].name, basic_tmp3.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: player[0] stable verification failed\n");
		reset_col();
	}

	if (strcmp(player[1].stable.unicorns[0].name, basic_tmp4.name) != 0 ||
			strcmp(player[1].stable.unicorns[1].name, basic_tmp5.name) != 0 ||
			strcmp(player[1].stable.unicorns[2].name, puppicorn_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: player[1] stable verification failed\n");
		reset_col();
	}

	if (puppicorn_index[0] != 2 || puppicorn_index[1] != 1 ||
			strcmp(player[1].stable.unicorns[2].name, puppicorn_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: puppicorn index failed\n");
		reset_col();
	}

	// cycle 2
	int events;
	receiveInt(&events, clientsockfd[0]);
	netStates[events].recvServer(1, clientsockfd[0]);

	if (player[0].stable.num_unicorns != 4 || player[0].stable.size != 4) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test (round 2): player[0] stable size failed\n");
		reset_col();
	}

	if (player[1].stable.num_unicorns != 3 || player[1].stable.size != 3) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test (round 2): player[1] stable size failed\n");
		reset_col();
	}

	if (strcmp(player[0].stable.unicorns[0].name, basic_tmp.name) != 0 ||
			strcmp(player[0].stable.unicorns[1].name, basic_tmp2.name) != 0 ||
			strcmp(player[0].stable.unicorns[2].name, basic_tmp3.name) != 0 ||
			strcmp(player[0].stable.unicorns[3].name, puppicorn_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test (round 2): player[0] stable verification failed\n");
		reset_col();
	}

	if (strcmp(player[1].stable.unicorns[0].name, basic_tmp4.name) != 0 ||
			strcmp(player[1].stable.unicorns[1].name, basic_tmp5.name) != 0 ||
			strcmp(player[1].stable.unicorns[2].name, basic_tmp6.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test (round 2): player[1] stable verification failed\n");
		reset_col();
	}

	if (puppicorn_index[0] != 3 || puppicorn_index[1] != 0 ||
			strcmp(player[0].stable.unicorns[3].name, puppicorn_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test (round 2): puppicorn index failed\n");
		reset_col();
	}

	reset_players();
	return num_fails;
}

int puppicorn_sacrifice_destroy_check() {
	int num_fails = 0;
	struct Unicorn puppicorn_tmp = basedeck[41];

	addStable(0, puppicorn_tmp);

	// destroy test
	canBeDestroyed(0, 0, ANY, FALSE);

	if (canBeDestroyed(0, 0, ANY, FALSE)) {
		num_fails++;
		red();
		fprintf(stderr, "    destroy test: canBeDestroyed failed\n");
		reset_col();
	}

	// sacrifice test
	canBeSacrificed(0, 0, ANY);

	if (canBeSacrificed(0, 0, ANY)) {
		num_fails++;
		red();
		fprintf(stderr, "    sacrifice test: canBeSacrificed failed\n");
		reset_col();
	}

	reset_players();
	return num_fails;
}

// win condition should be checked before it moves to the next player
int puppicorn_win_check() {
	int num_fails = 0;
	struct Unicorn puppicorn_tmp = basedeck[41];

	current_players = 2;
	for (int i = 0; i < 6; i++) {
		addStable(0, basedeck[i]);
	}
	addStable(0, puppicorn_tmp);

	int win = endOfTurn(0);

	if (win != 1) {
		num_fails++;
		red();
		fprintf(stderr, "    win check: endOfTurn failed\n");
		reset_col();
	}

	reset_players();
	return num_fails;
}

// puppicorn_index[0] should be -1 once it's returned to someone's hand
int puppicorn_reset_check() {
	int num_fails = 0;
	struct Unicorn basic_tmp = basedeck[13];
	struct Unicorn puppicorn_tmp = basedeck[41];

	current_players = 2;
	addStable(0, puppicorn_tmp);
	addStable(1, basic_tmp);

	endOfTurn(0);
	returnCardToHand(1, 1);

	if (puppicorn_index[0] != -1) {
		num_fails++;
		red();
		fprintf(stderr, "    reset test: puppicorn index failed\n");
		reset_col();
	}

	if (player[0].stable.num_unicorns != 0 || player[0].stable.size != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    reset test: stable size failed\n");
		reset_col();
	}


	return num_fails;
}

// puppicorn_index[0] should adjust accordingly to any stable changes
int puppicorn_rearrange_check() {
	int num_fails = 0;
	struct Unicorn basic_tmp = basedeck[13];
	struct Unicorn puppicorn_tmp = basedeck[41];

	addStable(0, basic_tmp);
	addStable(0, puppicorn_tmp);

	assert(puppicorn_index[0] == 1);
	sacrifice(0, ANY);

	// puppicorn_index[0] should become 0 if the first index gets sacrificed or destroyed
	if (puppicorn_index[0] != 0 ||
			strcmp(player[0].stable.unicorns[puppicorn_index[0]].name, puppicorn_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    rearrange test: puppicorn index failed\n");
		reset_col();
	}

	reset_players();
	reset_discard();
	return num_fails;
}

// Puppicorn
// 
// (original print text)
// At the end of your turn, move Puppicorn to the Stable of the player on your left.
// This card cannot be sacrificed or destroyed.
//
// (2nd print text: i overlooked this when configuring unicorn swap, but now it
// will be defunct)
// Each time any player begins their turn, move this card to that player's Stable.
// This card cannot be sacrificed or destroyed.
int puppicorn_tests() {
	int num_fails = 0;

	rainbow_error("\nStarting Puppicorn tests...\n");
	if (!isclient) {
		// file input stream setup
		FILE* fp;
		fopen_s(&fp, "Tests/Input/line_1.txt", "r");
		if (fp == NULL) {
			magenta();
			fprintf(stderr, "    file input failed :(");
			reset_col();
			return 1;
		}
		fpinput = fp;

		num_fails += puppicorn_basic_check();
		num_fails += puppicorn_sacrifice_destroy_check();
		num_fails += puppicorn_win_check();
		num_fails += puppicorn_reset_check();
		num_fails += puppicorn_rearrange_check();

		fclose(fp);
	}
	else {
		// basic check, no input
		int events;
		receiveInt(&events, sockfd);
		netStates[events].recvClient(1, sockfd);

		// cycle 2 of basic check
		struct Unicorn basic_tmp6 = basedeck[29];
		addStable(1, basic_tmp6);

		assert(player[0].stable.size == 3);
		assert(player[1].stable.size == 4);
		assert(puppicorn_index[0] == 2);
		assert(puppicorn_index[1] == 1);
		endOfTurn(1);

		// win check
		// this should get skipped over

		// reset check, no input
		receiveInt(&events, sockfd);
		netStates[events].recvClient(1, sockfd);
	}
	return num_fails;
}
