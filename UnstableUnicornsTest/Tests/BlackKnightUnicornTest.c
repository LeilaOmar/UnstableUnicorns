#include "MagicUnicornTests.h"
#include "networkevents.h"

// sanity check
int black_knight_basic_check() {
	int num_fails = 0;
	struct Unicorn basic_tmp = basedeck[13];
	struct Unicorn knight_tmp = basedeck[59];

	current_players = 2;
	addStable(1, knight_tmp);
	addStable(1, basic_tmp);

	assert(discardpile.size == 0);
	assert(player[1].stable.num_unicorns == 2);
	assert(player[1].flags == black_knight_unicorn);

	destroy(0, ANY, FALSE);

	// this test assumes that the 1st player initially tries to destroy
	// the basic unicorn; this is reflected in the input file
	if (discardpile.size != 1 ||
			strcmp(discardpile.cards[0].name, knight_tmp.name) != 0 ||
			strcmp(player[1].stable.unicorns[0].name, basic_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: sacrifice failed\n");
		reset_col();
	}

	if ((player[1].flags & black_knight_unicorn) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: toggle flags failed\n");
		reset_col();
	}

	reset_players();
	reset_discard();
	return num_fails;
}

// no other unicorns in the stable to sacrifice for
int black_knight_empty_check() {
	int num_fails = 0;
	struct Unicorn yay_tmp = basedeck[100];
	struct Unicorn knight_tmp = basedeck[59];

	current_players = 2;
	addStable(0, knight_tmp);
	addStable(0, yay_tmp);

	assert(discardpile.size == 0);
	assert(player[0].flags == black_knight_unicorn);

	sacrificeDestroyEffects(0, 1, knight_tmp.effect);

	// this test assumes that there's no stdout prompting the self-sacrifice
	if (discardpile.size != 1 ||
			strcmp(discardpile.cards[0].name, yay_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    non-unicorn test: destroy failed\n");
		reset_col();
	}

	reset_players();
	reset_discard();
	return num_fails;
}

// no other "unicorns" in the stable to sacrifice for
int black_knight_pandamonium_check() {
	int num_fails = 0;
	struct Unicorn panda_tmp = basedeck[107];
	struct Unicorn basic_tmp = basedeck[13];
	struct Unicorn knight_tmp = basedeck[59];

	current_players = 2;
	addStable(0, basic_tmp);
	addStable(0, panda_tmp);
	addStable(0, knight_tmp);
	toggleFlags(0, panda_tmp.effect);

	assert(discardpile.size == 0);
	assert((player[0].flags & pandamonium) == pandamonium);
	assert((player[0].flags & black_knight_unicorn) == black_knight_unicorn);

	sacrificeDestroyEffects(0, 0, knight_tmp.effect);

	// this test assumes that there's no stdout prompting the self-sacrifice
	if (discardpile.size != 1 ||
			strcmp(discardpile.cards[0].name, basic_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    pandamonium test: destroy failed\n");
		reset_col();
	}

	reset_players();
	reset_discard();
	return num_fails;
}

// blinding light disables the black knight check
int black_knight_blinding_light_check() {
	int num_fails = 0;
	struct Unicorn blinding_tmp = basedeck[112];
	struct Unicorn basic_tmp = basedeck[13];
	struct Unicorn knight_tmp = basedeck[59];

	current_players = 2;
	addStable(0, basic_tmp);
	addStable(0, blinding_tmp);
	addStable(0, knight_tmp);
	toggleFlags(0, blinding_tmp.effect);

	assert(discardpile.size == 0);
	assert((player[0].flags & blinding_light) == blinding_light);

	sacrificeDestroyEffects(0, 0, knight_tmp.effect);

	// this test assumes that there's no stdout prompting the self-sacrifice
	if (discardpile.size != 1 ||
			strcmp(discardpile.cards[0].name, basic_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    pandamonium test: destroy failed\n");
		reset_col();
	}

	reset_players();
	reset_discard();
	return num_fails;
}

// Black Knight Unicorn
//
// If a Unicorn card in your Stable would be destroyed, you may SACRIFICE this card instead.
int black_knight_unicorn_tests() {
	int num_fails = 0;

	rainbow_error("\nStarting Black Knight Unicorn tests...\n");
	FILE* fp;

	if (!isclient) {
		// file input stream setup
		fopen_s(&fp, "Tests/Input/blackknightunicorn.txt", "r");
		if (fp == NULL) {
			magenta();
			fprintf(stderr, "    file input failed :(");
			reset_col();
			return 1;
		}
		fpinput = fp;

		// the test is set up to prepare for the fail scenario with y inputs
		num_fails += black_knight_basic_check();
		num_fails += black_knight_empty_check();
		num_fails += black_knight_pandamonium_check();
		num_fails += black_knight_blinding_light_check();

	}
	else {
		// file input stream setup
		fopen_s(&fp, "Tests/Input/line_y_1.txt", "r"); // technically only the y part is necessary
		if (fp == NULL) {
			magenta();
			fprintf(stderr, "    file input failed :(");
			reset_col();
			return 1;
		}
		fpinput = fp;

		// basic check
		int events;
		receiveInt(&events, sockfd);
		netStates[events].recvClient(1, sockfd);
	}

	fclose(fp);
	return num_fails;
}
