#include "gamemechanics.h"

int num_fails = 0;

void save_array(int *tmp, int *arr, int size) {
	for (int i = 0; i < size; i++) {
		tmp[i] = arr[i];
	}
}

void restore_array(int* tmp, int* arr, int size) {
	for (int i = 0; i < size; i++) {
		arr[i] = tmp[i];
	}

	deck_index = 0;
	discard_index = 0;
	nursery_index = 0;
	dnurse_size = NURSERY_SIZE;
}

void reset_player(int pnum) {
	player[pnum].hand.num_cards = 0;
	player[pnum].stable.num_unicorns = 0;
	player[pnum].stable.size = 0;
	player[pnum].flags = 0;
}

void check_class_tests() {
	printf("\nStarting checkClass tests...");

	// test for ANY cards
	if (checkClass(ANY, BABYUNICORN) == 0) num_fails++;
	if (checkClass(ANY, BASICUNICORN) == 0) num_fails++;
	if (checkClass(ANY, MAGICUNICORN) == 0) num_fails++;
	if (checkClass(ANY, UPGRADE) == 0) num_fails++;
	if (checkClass(ANY, DOWNGRADE) == 0) num_fails++;
	if (checkClass(ANY, MAGIC) == 0) num_fails++;
	if (checkClass(ANY, INSTANT) == 0) num_fails++;

	// test for ANYUNICORN
	if (checkClass(ANYUNICORN, BABYUNICORN) == 0) num_fails++;
	if (checkClass(ANYUNICORN, BASICUNICORN) == 0) num_fails++;
	if (checkClass(ANYUNICORN, MAGICUNICORN) == 0) num_fails++;

	// test for compatible classes
	if (checkClass(BASICUNICORN, BASICUNICORN) == 0) num_fails++;
	if (checkClass(BASICUNICORN, UPGRADE) == 1) num_fails++;

	if (num_fails > 0) {
		printf("\n    checkClass failed?!?!!! wow lol\n");
	}
}

// why are shuffleDeck parameters (start, size) when size is meant to be the end...
void shuffle_deck_tests() {
	int start = 10, size = 20, tmp[DECK_SIZE];
	int upperbound = deck_ref[start - 1], lowerbound = deck_ref[start + size + 1];

	printf("\nStarting shuffleDeck tests...");
	save_array(tmp, deck_ref, DECK_SIZE);

	// test to see if it respects the proper bounds (i.e. it should only affect the start index by size # of cards
	shuffleDeck(start, size);

	if (deck_ref[start - 1] != upperbound || deck_ref[start + size + 1] != lowerbound) {
		num_fails++;
		red();
		printf("\n    deck bound test failed");
		reset_col();
	}
	restore_array(tmp, deck_ref, DECK_SIZE);

	// test to see that nothing changes when size = zero
	int start_card = deck_ref[start];
	shuffleDeck(start, 0);

	if (deck_ref[start] != start_card) {
		num_fails++;
		red();
		printf("\n    size = 0 test failed");
		reset_col();
	}
	restore_array(tmp, deck_ref, DECK_SIZE);

	// test to make sure that the program doesn't crash when start is negative
	shuffleDeck(-1, size);

	for (int i = 0; i < DECK_SIZE; i++) {
		if (tmp[i] != deck_ref[i]) {
			num_fails++;
			red();
			printf("\n    negative start test failed");
			reset_col();
			break;
		}
	}
	restore_array(tmp, deck_ref, DECK_SIZE);

	// test to make sure that the program doesn't crash when size is negative
	shuffleDeck(start, -1);

	for (int i = 0; i < DECK_SIZE; i++) {
		if (tmp[i] != deck_ref[i]) {
			num_fails++;
			red();
			printf("\n    negative size test failed");
			reset_col();
			break;
		}
	}
	restore_array(tmp, deck_ref, DECK_SIZE);

}

// a lot of potential checks here are actually checked for in code surrounding
// the function call
void rearrange_nursery_tests() {
	int tmp[NURSERY_SIZE], tmp_index = nursery_index, tmp_size = dnurse_size;

	printf("\nStarting rearrangeNursery tests...");
	save_array(tmp, nursery_ref, NURSERY_SIZE);

	// negative index test
	for (int i = 0; i < NURSERY_SIZE; i++) {
		tmp[i] = nursery_ref[i];
	}
	rearrangeNursery(-1);

	for (int i = 0; i < NURSERY_SIZE; i++) {
		if (tmp[i] != nursery_ref[i]) {
			num_fails++;
			red();
			printf("\n    negative index pt 1 test failed");
			reset_col();
		}
	}

	if (tmp_index != nursery_index || tmp_size != dnurse_size) {
		num_fails++;
		red();
		printf("\n    negative index pt 2 test failed");
		reset_col();
	}
	restore_array(tmp, nursery_ref, NURSERY_SIZE);

	// sanity check
	int start = 5, start_card = nursery_ref[start];
	for (int i = 0; i < NURSERY_SIZE; i++) {
		tmp[i] = nursery_ref[i];
	}

	rearrangeNursery(start);

	if ((tmp_index + 1) != nursery_index || (tmp_size - 1) != dnurse_size || nursery_ref[0] != start_card) {
		num_fails++;
		red();
		printf("\n    basic sanity check test failed");
		reset_col();
	}
	restore_array(tmp, nursery_ref, NURSERY_SIZE);
}

void add_nursery_tests() {
	// basic check
	int tmp[NURSERY_SIZE], start_index = nursery_index, start_size = dnurse_size, cardid = 12;

	printf("\nStarting addNursery tests...");
	save_array(tmp, nursery_ref, NURSERY_SIZE);

	addNursery(cardid);
	if ((start_size + 1) != dnurse_size || (start_index - 1) != nursery_index || nursery_ref[nursery_index] != cardid) {
		num_fails++;
		red();
		printf("\n    basic sanity check test failed");
		reset_col();
	}
	restore_array(tmp, nursery_ref, NURSERY_SIZE);
}

void add_stable_tests() {
	int narwhal_tmp = 34, broken_stable_tmp = 111, basic_tmp = 13; // cardid doesn't matter

	printf("\nStarting addStable tests...");

	// adding unicorn (technically a Narwhal :V )
	addStable(0, narwhal_tmp);

	if (player[0].stable.num_unicorns != 1 || player[0].stable.size != 1) {
		num_fails++;
		red();
		printf("\n    adding unicorn test failed");
		reset_col();
	}
	reset_player(0);

	// adding magic/upgrade/downgrade card [broken stable]
	addStable(0, broken_stable_tmp);

	if (player[0].stable.num_unicorns != 0 || player[0].stable.size != 1) {
		num_fails++;
		red();
		printf("\n    adding non-unicorn test failed");
		reset_col();
	}
	reset_player(0);

	// barbed wire test with adding a non-unicorn card (it shouldn't activate)
	player[0].hand.cards[player[0].hand.num_cards++] = narwhal_tmp;
	player[0].flags |= barbed_wire;
	addStable(0, broken_stable_tmp);

	if (player[0].hand.num_cards != 1) {
		num_fails++;
		red();
		printf("\n    barbed wire test w/ non-unicorn failed");
		reset_col();
	}
	reset_player(0);

	// barbed wire test with adding a unicorn card (it should activate)
	player[0].hand.cards[player[0].hand.num_cards++] = narwhal_tmp;
	player[0].flags |= barbed_wire;

	FILE* test1;
	freopen_s(&test1, "tests/line_1.txt", "r", stdin);

	addStable(0, narwhal_tmp);

	if (player[0].hand.num_cards != 0) {
		num_fails++;
		red();
		printf("\n    barbed wire test w/ unicorn failed");
		reset_col();
	}
	reset_player(0);

	// tiny stable sanity check w/ 5 unicorns and 1 "other"
	player[0].flags |= tiny_stable;
	for (int i = 0; i < 5; i++) {
		addStable(0, narwhal_tmp);
	}
	addStable(0, broken_stable_tmp);

	if (player[0].stable.size != 6 || player[0].stable.num_unicorns != 5) {
		num_fails++;
		red();
		printf("\n    tiny stable pre-limit check failed");
		reset_col();
	}

	// tiny stable cont. check
	FILE* test2;
	freopen_s(&test2, "tests/line_1.txt", "r", stdin);
	player[0].stable.unicorns[1] = basic_tmp; // replace the second unicorn since the first will be sacrificed
	addStable(0, narwhal_tmp);

	// check if the previous second becomes the first alongside the basic size/count checks
	if (player[0].stable.size != 6 || player[0].stable.num_unicorns != 5 || player[0].stable.unicorns[0] != basic_tmp) {
		num_fails++;
		red();
		printf("\n    tiny stable sacrifice test failed");
		reset_col();
	}
	reset_player(0);

	// tiny stable w/ pandamonium
	player[0].flags |= tiny_stable;
	player[0].flags |= pandamonium;
	for (int i = 0; i < 7; i++) {
		addStable(0, narwhal_tmp);
	}

	if (player[0].stable.size != 7 || player[0].stable.num_unicorns != 7) {
		num_fails++;
		red();
		printf("\n    tiny stable test w/ pandamonium failed");
		reset_col();
	}
	reset_player(0);
}

void rearrange_stable_tests() {
	int narwhal_tmp = 34, basic_tmp = 13, broken_stable_tmp = 111;

	printf("\nStarting rearrangeStable tests...");

	// barbed wire w/o pandamonium (general test pt 1)
	addStable(0, basic_tmp);
	addStable(0, narwhal_tmp);
	addStable(0, broken_stable_tmp);
	player[0].hand.cards[player[0].hand.num_cards++] = narwhal_tmp;
	player[0].flags |= barbed_wire;

	rearrangeStable(0, 2); // index 2 represents the 3rd card added [i.e. broken stable]

	if (player[0].stable.num_unicorns != 2 || player[0].stable.size != 2 || player[0].hand.num_cards != 1) {
		num_fails++;
		red();
		printf("\n    barbed wire basic non-unicorn check failed");
		reset_col();
	}
	reset_player(0);

	// barbed wire w/o pandamonium (general test pt 2)
	addStable(0, basic_tmp);
	addStable(0, narwhal_tmp);
	player[0].hand.cards[player[0].hand.num_cards++] = narwhal_tmp;
	player[0].flags |= barbed_wire;

	FILE* test1;
	freopen_s(&test1, "tests/line_1.txt", "r", stdin);
	rearrangeStable(0, 0); // take out the first card

	if (player[0].stable.num_unicorns != 1 || player[0].stable.size != 1 || player[0].hand.num_cards != 0) {
		num_fails++;
		red();
		printf("\n    barbed wire basic unicorn check failed");
		reset_col();
	}
	reset_player(0);

	// barbed wire w/ pandamonium
	addStable(0, basic_tmp);
	addStable(0, narwhal_tmp);
	player[0].hand.cards[player[0].hand.num_cards++] = narwhal_tmp;
	player[0].flags |= barbed_wire;
	player[0].flags |= pandamonium;

	rearrangeStable(0, 0); // take out the first card

	if (player[0].stable.num_unicorns != 1 || player[0].stable.size != 1 || player[0].hand.num_cards != 1) {
		num_fails++;
		red();
		printf("\n    barbed wire w/ pandamonium check failed");
		reset_col();
	}
	reset_player(0);
}

void draw_tests() {
	int tmp[DECK_SIZE], narwhal_tmp = 34;

	printf("\nStarting draw tests...");
	save_array(tmp, deck_ref, DECK_SIZE);

	// check if it correctly draws when the deck is about to be full
	deck_index = DECK_SIZE - 1;
	discard_ref[discard_index++] = narwhal_tmp; // add a dummy card in the discard pile so that shuffleDiscard will work properly
	draw(0, 2);

	// the second card drawn should be the dummy narwhal card from the discard pile, and the discard pile should be reset to zero
	if (player[0].hand.num_cards != 2 || discard_index != 0 || player[0].hand.cards[1] != narwhal_tmp) {
		num_fails++;
		red();
		printf("\n    multi-draw w/ empty deck test failed");
		reset_col();
	}
	reset_player(0);
	restore_array(tmp, deck_ref, DECK_SIZE);
}

int test_main() {
	// wheeeee~
	// searching and most rearrange/shuffle functions will be skipped because *snore*
	// and most of them are the same

	check_class_tests();
	shuffle_deck_tests();
	rearrange_nursery_tests();
	add_nursery_tests();
	add_stable_tests();
	rearrange_stable_tests();
	draw_tests();

	return num_fails;
}
