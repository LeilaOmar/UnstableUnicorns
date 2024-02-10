#include "UpgradeTests.h"

// sanity test
int rainbow_aura_basic_check() {
	int num_fails = 0;
	struct Unicorn rainaura_tmp = basedeck[103];
	struct Unicorn basic_tmp = basedeck[13];
	struct Unicorn poison_tmp = basedeck[67];

	current_players = 2;
	player[1].hand.cards[player[1].hand.num_cards++] = poison_tmp;
	addStable(0, rainaura_tmp);
	addStable(0, basic_tmp);
	toggleFlags(0, rainaura_tmp.effect);

	// try destroying it with unicorn poison
	int ret;
	assert(player[0].stable.size == 2);
	assert(player[0].flags == rainbow_aura);
	ret = conditionalEffects(1, poison_tmp, 0, 1);

	if (turn_count != 2 || ret != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: turn count failed\n");
		reset_col();
	}

	// check canBeDestroyed; card referenced is player[0][1]
	if (canBeDestroyed(0, 1, ANY, FALSE)) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: canBeDestroyed failed\n");
		reset_col();
	}


	reset_players();
	reset_discard();
	return num_fails;
}

int rainbow_aura_non_unicorn_check() {
	int num_fails = 0;
	struct Unicorn rainaura_tmp = basedeck[103];
	struct Unicorn panda_tmp = basedeck[107];
	struct Unicorn bomb_tmp = basedeck[98];

	current_players = 2;
	addStable(0, rainaura_tmp);
	addStable(0, panda_tmp);
	addStable(1, bomb_tmp);
	toggleFlags(0, rainaura_tmp.effect);
	assert(player[0].stable.size == 2);
	assert(player[0].flags == rainbow_aura);

	// first just check canBeDestroyed; card referenced is player[0][1]
	if (!canBeDestroyed(0, 1, ANY, FALSE)) {
		num_fails++;
		red();
		fprintf(stderr, "    non-unicorn test: canBeDestroyed failed\n");
		reset_col();
	}

	// try destroying it with glitter bomb
	assert(discardpile.size == 0);
	assert(player[1].stable.size == 1);
	beginningTurnEffects(1, bomb_tmp);

	if (player[0].stable.size != 1 || player[0].stable.num_unicorns != 0 ||
			player[1].stable.size != 0 || player[1].stable.num_unicorns != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    non-unicorn test: stable size failed\n");
		reset_col();
	}

	if (discardpile.size != 2 ||
			strcmp(discardpile.cards[0].name, bomb_tmp.name) != 0 ||
			strcmp(discardpile.cards[1].name, panda_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    non-unicorn test: discard pile verification failed\n");
		reset_col();
	}

	reset_players();
	reset_discard();
	return num_fails;
}

int rainbow_aura_pandamonium_check() {
	int num_fails = 0;
	struct Unicorn rainaura_tmp = basedeck[103];
	struct Unicorn basic_tmp = basedeck[13];
	struct Unicorn bomb_tmp = basedeck[98];
	struct Unicorn panda_tmp = basedeck[107];

	current_players = 2;
	addStable(0, rainaura_tmp);
	addStable(0, panda_tmp);
	addStable(0, basic_tmp);
	addStable(1, bomb_tmp);
	toggleFlags(0, rainaura_tmp.effect);
	toggleFlags(0, panda_tmp.effect);
	assert(player[0].stable.size == 3);
	assert((player[0].flags & rainbow_aura) == rainbow_aura);
	assert((player[0].flags & pandamonium) == pandamonium);

	// first just check canBeDestroyed; card referenced is player[0][1]
	if (!canBeDestroyed(0, 2, ANY, FALSE)) {
		num_fails++;
		red();
		fprintf(stderr, "    non-unicorn test: canBeDestroyed failed\n");
		reset_col();
	}

	// try destroying it with glitter bomb
	assert(discardpile.size == 0);
	assert(player[1].stable.size == 1);
	beginningTurnEffects(1, bomb_tmp);


	if (player[0].stable.size != 2 || player[0].stable.num_unicorns != 0 ||
			player[1].stable.size != 0 || player[1].stable.num_unicorns != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    pandamonium test: stable size failed\n");
		reset_col();
	}

	if (discardpile.size != 2 ||
			strcmp(discardpile.cards[0].name, bomb_tmp.name) != 0 ||
			strcmp(discardpile.cards[1].name, basic_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    pandamonium test: discard pile verification failed\n");
		reset_col();
	}

	reset_players();
	reset_discard();
	return num_fails;
}

// Rainbow Aura
//
// Your Unicorn cards cannot be destroyed.
int rainbow_aura_tests() {
	int num_fails = 0;

	if (!isclient) {
		rainbow_error("\nStarting Rainbow Aura tests...\n");

		// file input stream setup
		FILE* fp;
		fopen_s(&fp, "Tests/Input/rainbowaura.txt", "r");
		if (fp == NULL) {
			magenta();
			fprintf(stderr, "    file input failed :(");
			reset_col();
			return 1;
		}
		fpinput = fp;

		num_fails += rainbow_aura_basic_check();
		num_fails += rainbow_aura_non_unicorn_check();
		num_fails += rainbow_aura_pandamonium_check();

		fclose(fp);
	}
	return num_fails;
}
