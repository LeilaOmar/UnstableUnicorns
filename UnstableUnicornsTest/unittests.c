#include "resetfns.h"
#include "Tests/MagicUnicornTests.h"
#include "Tests/MagicTests.h"
#include "Tests/UpgradeTests.h"
#include "Tests/DowngradeTests.h"
#include "Tests/InstantTests.h"
#include "networkstates.h"
#include <conio.h>

// void processStdin_mock(char* stdinbuf, int* bufindex) {
// 
// 	fgets(stdinbuf, 1024, fpinput);
// 	*bufindex = strlen(stdinbuf);
// 
// }
// 
// DWORD simple(HANDLE hHandle, DWORD dwMilliseconds) {
// 	return WAIT_OBJECT_0;
// }
// 
// #define processStdin(a, b) processStdin_mock(a, b)
// #define WaitForSingleObject(a, b) simple(a, b)
// // #include "networkevents.c"
// #undef processStdin
// #undef WaitForSingleObject

// #pragma comment(lib, "MockLib.lib")

int num_fails = 0;

// ********************************************************************************
// **************************** Utility Function Tests ****************************
// ********************************************************************************

void check_class_tests() {
	rainbow_error("\nStarting checkType tests...\n");

	// test for ANY cards
	if (checkType(ANY, BABYUNICORN) == 0) num_fails++;
	if (checkType(ANY, BASICUNICORN) == 0) num_fails++;
	if (checkType(ANY, MAGICUNICORN) == 0) num_fails++;
	if (checkType(ANY, UPGRADE) == 0) num_fails++;
	if (checkType(ANY, DOWNGRADE) == 0) num_fails++;
	if (checkType(ANY, MAGIC) == 0) num_fails++;
	if (checkType(ANY, INSTANT) == 0) num_fails++;

	// test for ANYUNICORN
	if (checkType(ANYUNICORN, BABYUNICORN) == 0) num_fails++;
	if (checkType(ANYUNICORN, BASICUNICORN) == 0) num_fails++;
	if (checkType(ANYUNICORN, MAGICUNICORN) == 0) num_fails++;

	// test for compatible classes
	if (checkType(BASICUNICORN, BASICUNICORN) == 0) num_fails++;
	if (checkType(BASICUNICORN, UPGRADE) == 1) num_fails++;

	if (num_fails > 0) {
		fprintf(stderr, "    checkType failed?!?!!! wow lol\n");
	}
}

// a lot of potential checks here are actually checked for in the code surrounding
// the function call
void rearrange_pile_tests() {
	int tmp_size = nursery.size;
	int start = 5;

	rainbow_error("\nStarting rearrangePile tests...\n");

	// sanity check
	rearrangePile(&nursery, start);

	if ((tmp_size - 1) != nursery.size) {
		num_fails++;
		red();
		fprintf(stderr, "    basic sanity check test 1 failed\n");
		reset_col();
	}

	// using the nursery here because each card is unique
	int found = 0;
	for (int i = 0; i < nursery.size; i++) {
		if (strcmp(nursery.cards[i].name, basedeck[start].name) == 0)
			found++;
	}

	if (found > 0) {
		num_fails++;
		red();
		fprintf(stderr, "    basic sanity check test 2 failed\n");
		reset_col();
	}
	reset_nursery();

	// empty pile test (this shouldn't really happen in the first place)
	nursery.size = 0;
	tmp_size = nursery.size;

	rearrangePile(&nursery, 0);

	if (tmp_size != nursery.size) {
		num_fails++;
		red();
		fprintf(stderr, "    empty pile test failed\n");
		reset_col();
	}

	reset_nursery();
}

void add_nursery_tests() {
	struct Unicorn corn = basedeck[12];
	int tmp_size = nursery.size;

	rainbow_error("\nStarting addNursery tests...\n");

	// capacity check; this really shouldn't be called erroneously in the first place,
	// but at least it wouldn't completely break if sometihng else duplicated or went
	// wrong in some other fashion
	addNursery(corn);
	if (nursery.size != tmp_size) {
		num_fails++;
		red();
		fprintf(stderr, "    full nursery size check test failed\n");
		reset_col();
	}
	reset_nursery();

	// basic check
	nursery.size = 5;
	tmp_size = nursery.size;

	addNursery(corn);

	if (nursery.size != (tmp_size + 1)) {
		num_fails++;
		red();
		fprintf(stderr, "    basic sanity check test 1 failed\n");
		reset_col();
	}
	if (strcmp(nursery.cards[nursery.size - 1].name, corn.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    basic sanity check test 2 failed\n");
		reset_col();
	}

	reset_nursery();
}

void add_stable_tests() {
	struct Unicorn narwhal_tmp = basedeck[34];
	struct Unicorn broken_stable_tmp = basedeck[111];
	struct Unicorn basic_tmp = basedeck[13];

	rainbow_error("\nStarting addStable tests...\n");

	// adding unicorn (technically a Narwhal :V )
	addStable(0, narwhal_tmp);

	if (player[0].stable.num_unicorns != 1 || player[0].stable.size != 1 || 
			strcmp(player[0].stable.unicorns[0].name, narwhal_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    adding unicorn test failed\n");
		reset_col();
	}
	reset_players();

	// adding magic/upgrade/downgrade card [broken stable]
	addStable(0, broken_stable_tmp);

	if (player[0].stable.num_unicorns != 0 || player[0].stable.size != 1) {
		num_fails++;
		red();
		fprintf(stderr, "    adding non-unicorn test failed\n");
		reset_col();
	}
	reset_players();

	// barbed wire test with adding a non-unicorn card (it shouldn't activate)
	player[0].hand.cards[player[0].hand.num_cards++] = narwhal_tmp;
	player[0].flags |= barbed_wire;
	addStable(0, broken_stable_tmp);

	if (player[0].hand.num_cards != 1) {
		num_fails++;
		red();
		fprintf(stderr, "    barbed wire test w/ non-unicorn failed\n");
		reset_col();
	}
	reset_players();

	// barbed wire test with adding a unicorn card (it should activate)
	player[0].hand.cards[player[0].hand.num_cards++] = narwhal_tmp;
	player[0].flags |= barbed_wire;

	FILE* fp;
	fopen_s(&fp, "Tests/Input/line_1.txt", "r");
	if (fp == NULL) {
		num_fails++;
		magenta();
		fprintf(stderr, "    file input failed :(\n");
		reset_col();
		return;
	}
	fpinput = fp;

	addStable(0, narwhal_tmp);

	if (player[0].hand.num_cards != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    barbed wire test w/ unicorn failed\n");
		reset_col();
	}
	reset_players();
	fclose(fp);

	// barbed wire + pandamonium test (it should activate because it enters as a unicorn)
	player[0].hand.cards[player[0].hand.num_cards++] = narwhal_tmp;
	player[0].flags |= barbed_wire;
	player[0].flags |= pandamonium;

	fopen_s(&fp, "Tests/Input/line_1.txt", "r");
	if (fp == NULL) {
		num_fails++;
		magenta();
		fprintf(stderr, "    file input failed :(\n");
		reset_col();
		return;
	}
	fpinput = fp;

	addStable(0, narwhal_tmp);

	if (player[0].hand.num_cards != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    barbed wire test w/ pandamonium failed\n");
		reset_col();
	}
	reset_players();

	// tiny stable sanity check w/ 5 unicorns and 1 "other"
	player[0].flags |= tiny_stable;
	for (int i = 0; i < 5; i++) {
		addStable(0, narwhal_tmp);
	}
	addStable(0, broken_stable_tmp);

	if (player[0].stable.size != 6 || player[0].stable.num_unicorns != 5) {
		num_fails++;
		red();
		fprintf(stderr, "    tiny stable pre-limit check failed\n");
		reset_col();
	}
	fclose(fp);

	// tiny stable cont. check
	fopen_s(&fp, "Tests/Input/line_1.txt", "r");
	if (fp == NULL) {
		num_fails++;
		magenta();
		fprintf(stderr, "    file input failed :(\n");
		reset_col();
		return;
	}
	fpinput = fp;

	player[0].stable.unicorns[1] = basic_tmp; // replace the second unicorn since the first will be sacrificed
	addStable(0, narwhal_tmp);

	// check if the previous second becomes the first alongside the basic size/count checks
	if (player[0].stable.size != 6 || player[0].stable.num_unicorns != 5 ||
			strcmp(player[0].stable.unicorns[0].name, basic_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    tiny stable sacrifice test failed\n");
		reset_col();
	}
	reset_players();
	fclose(fp);

	fpinput = stdin; // to prevent an infinite loop due to some error

	// tiny stable w/ pandamonium
	player[0].flags |= tiny_stable;
	player[0].flags |= pandamonium;
	for (int i = 0; i < 7; i++) {
		addStable(0, narwhal_tmp);
	}

	if (player[0].stable.size != 7 || player[0].stable.num_unicorns != 7) {
		num_fails++;
		red();
		fprintf(stderr, "    tiny stable test w/ pandamonium failed\n");
		reset_col();
	}
	reset_players();
}

void rearrange_stable_tests() {
	struct Unicorn narwhal_tmp = basedeck[34];
	struct Unicorn broken_stable_tmp = basedeck[111];
	struct Unicorn basic_tmp = basedeck[13];

	rainbow_error("\nStarting rearrangeStable tests...\n");

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
		fprintf(stderr, "    barbed wire basic non-unicorn check failed\n");
		reset_col();
	}
	reset_players();

	// barbed wire w/o pandamonium (general test pt 2)
	addStable(0, basic_tmp);
	addStable(0, narwhal_tmp);
	player[0].hand.cards[player[0].hand.num_cards++] = narwhal_tmp;
	player[0].flags |= barbed_wire;

	FILE* fp;
	fopen_s(&fp, "Tests/Input/line_1.txt", "r");
	if (fp == NULL) {
		num_fails++;
		magenta();
		fprintf(stderr, "    file input failed :(\n");
		reset_col();
		return;
	}
	fpinput = fp;
	rearrangeStable(0, 0); // take out the first card

	if (player[0].stable.num_unicorns != 1 || player[0].stable.size != 1 || player[0].hand.num_cards != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    barbed wire basic unicorn check failed\n");
		reset_col();
	}
	reset_players();
	fclose(fp);

	// barbed wire w/ pandamonium
	addStable(0, basic_tmp);
	addStable(0, narwhal_tmp);
	player[0].hand.cards[player[0].hand.num_cards++] = narwhal_tmp;
	player[0].flags |= barbed_wire;
	player[0].flags |= pandamonium;

	fopen_s(&fp, "Tests/Input/line_1.txt", "r");
	if (fp == NULL) {
		num_fails++;
		magenta();
		fprintf(stderr, "    file input failed :(\n");
		reset_col();
		return;
	}
	fpinput = fp;

	rearrangeStable(0, 0); // take out the first card

	if (player[0].stable.num_unicorns != 1 || player[0].stable.size != 1 || player[0].hand.num_cards != 1) {
		num_fails++;
		red();
		fprintf(stderr, "    barbed wire w/ pandamonium check failed\n");
		reset_col();
	}
	reset_players();

	fclose(fp);
}

// ********************************************************************************
// **************************** Basic Card Effect Tests ***************************
// ********************************************************************************

void draw_tests() {

	rainbow_error("\nStarting draw tests...\n");

	// TODO: the game should actually end at this point
	// check if it correctly draws when the deck is about to be empty
	deck.size = 1;
	int tmp_hand_size = player[0].hand.num_cards;
	int ret = draw(0, 1);

	if (player[0].hand.num_cards != (tmp_hand_size + 1) || deck.size != 0 || ret != -1) {
		num_fails++;
		red();
		fprintf(stderr, "    draw until empty deck test failed\n");
		reset_col();
	}
	reset_players();
	reset_deck();

	// check if it skips drawing when the number of cards goes past the size
	deck.size = 1;
	tmp_hand_size = player[0].hand.num_cards;
	ret = draw(0, 2);

	if (player[0].hand.num_cards != tmp_hand_size || deck.size != 1 || ret != -1) {
		num_fails++;
		red();
		fprintf(stderr, "    draw past empty deck test failed\n");
		reset_col();
	}
	reset_players();
	reset_deck();

	// normal draw
	int tmp_deck_size = deck.size;
	tmp_hand_size = player[0].hand.num_cards;
	ret = draw(0, 1);

	if (player[0].hand.num_cards != (tmp_hand_size + 1) || deck.size != (tmp_deck_size - 1) || ret != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test 1 failed\n");
		reset_col();
	}

	// it isn't shuffled, so the player is drawing from the end
	if (strcmp(player[0].hand.cards[0].name, basedeck[NURSERY_SIZE + tmp_deck_size - 1].name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test 2 failed\n");
		reset_col();
	}
	reset_players();
	reset_deck();
	reset_discard();
}

void discard_tests() {

	rainbow_error("\nStarting discard tests...\n");

	// simple check
	draw(0, 1);

	FILE* fp;
	fopen_s(&fp, "Tests/Input/line_1.txt", "r");
	if (fp == NULL) {
		num_fails++;
		magenta();
		fprintf(stderr, "    file input failed :(\n");
		reset_col();
		return;
	}
	fpinput = fp;

	assert(discardpile.size == 0);
	assert(player[0].hand.num_cards == 1);
	discard(0, 1, ANY);

	if (player[0].hand.num_cards != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: hand size failed\n");
		reset_col();
	}

	if (discardpile.size != 1) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: discard size failed\n");
		reset_col();
	}

	reset_players();
	reset_deck();
	reset_discard();
}

void sacrifice_tests() {
	struct Unicorn ginormous_tmp = basedeck[48];
	rainbow_error("\nStarting sacrifice tests...\n");

	// simple check
	addStable(0, ginormous_tmp);
	enterStableEffects(0, ginormous_tmp.effect);

	FILE* fp;
	fopen_s(&fp, "Tests/Input/line_1.txt", "r");
	if (fp == NULL) {
		num_fails++;
		magenta();
		fprintf(stderr, "    file input failed :(\n");
		reset_col();
		return;
	}
	fpinput = fp;

	assert(discardpile.size == 0);
	assert(player[0].stable.size == 1);
	assert(player[0].flags == ginormous_unicorn);
	sacrifice(0, ANY);

	if (player[0].stable.size != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: stable size failed\n");
		reset_col();
	}

	if (discardpile.size != 1 ||
			strcmp(discardpile.cards[0].name, ginormous_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: discard verification failed\n");
		reset_col();
	}

	if (player[0].flags == ginormous_unicorn) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: toggle flags failed\n");
		reset_col();
	}

	reset_players();
	reset_deck();
	reset_discard();
}

void destroy_tests() {
	struct Unicorn ginormous_tmp = basedeck[48];
	rainbow_error("\nStarting destroy tests...\n");

	// simple check
	current_players = 2;
	addStable(0, ginormous_tmp);
	enterStableEffects(0, ginormous_tmp.effect);

	FILE* fp;
	fopen_s(&fp, "Tests/Input/line_1_1.txt", "r");
	if (fp == NULL) {
		num_fails++;
		magenta();
		fprintf(stderr, "    file input failed :(\n");
		reset_col();
		return;
	}
	fpinput = fp;

	assert(discardpile.size == 0);
	assert(player[0].stable.size == 1);
	assert(player[0].flags == ginormous_unicorn);
	destroy(1, ANY, FALSE);

	if (player[0].stable.size != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: stable size failed\n");
		reset_col();
	}

	if (discardpile.size != 1 ||
			strcmp(discardpile.cards[0].name, ginormous_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: discard verification failed\n");
		reset_col();
	}

	if (player[0].flags == ginormous_unicorn) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: toggle flags failed\n");
		reset_col();
	}

	reset_players();
	reset_deck();
	reset_discard();
}

void steal_tests() {
	struct Unicorn ginormous_tmp = basedeck[48];
	rainbow_error("\nStarting steal tests...\n");

	// simple check
	current_players = 2;
	addStable(0, ginormous_tmp);
	enterStableEffects(0, ginormous_tmp.effect);

	FILE* fp;
	fopen_s(&fp, "Tests/Input/line_1_1.txt", "r");
	if (fp == NULL) {
		num_fails++;
		magenta();
		fprintf(stderr, "    file input failed :(\n");
		reset_col();
		return;
	}
	fpinput = fp;

	assert(discardpile.size == 0);
	assert(player[0].stable.size == 1);
	assert(player[0].flags == ginormous_unicorn);
	steal(1, ANY);

	if (player[0].stable.size != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: player[0] stable size failed\n");
		reset_col();
	}

	if (player[1].stable.size != 1 || player[1].stable.num_unicorns != 1 ||
			strcmp(player[1].stable.unicorns[0].name, ginormous_tmp.name) != 0) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: player[1] stable size failed\n");
		reset_col();
	}

	if (player[0].flags == ginormous_unicorn) {
		num_fails++;
		red();
		fprintf(stderr, "    sanity test: toggle flags failed\n");
		reset_col();
	}

	reset_players();
	reset_deck();
	reset_discard();
}

// ********************************************************************************
// ********************************* Main Function ********************************
// ********************************************************************************

int main(int argc, char* argv[]) {

	// just in case file i/o fails
	fpinput = stdin;

	// client/server variables
	isclient = 0;
	WSADATA wsa;
	short portno = 1234;
	struct sockaddr_in addr;
	struct sockaddr_in server;

	// additional information
	STARTUPINFOA si;
	PROCESS_INFORMATION pi;

	// set the size of the structures
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	// set up winsock with TDP communication (SOCK_STREAM) on IPv4 (AF_INET)
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		printf("Failed. Error Code : %d", WSAGetLastError());
		exit(1);
	}

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
		printf("Could not create socket : %d", WSAGetLastError());
		exit(1);
	}

	if (argc > 1 && strcmp(argv[1], "client") == 0) {

		// ********************************************************************************
		// ********************************* Client Set-up ********************************
		// ********************************************************************************

		server.sin_family = AF_INET;
		server.sin_port = htons(portno);
		inet_pton(AF_INET, "127.0.0.1", &server.sin_addr.s_addr);

		while (connect(sockfd, (struct sockaddr*)&server, sizeof(server)) < 0) {
			fprintf(stderr, "Attempt to connect to the server was unsuccessful. Error code : %d\n", WSAGetLastError());
		}
		
		isclient = 1;

	}
	else {

		// ********************************************************************************
		// ********************************* Server Set-up ********************************
		// ********************************************************************************

		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = INADDR_ANY;
		addr.sin_port = htons(portno);

		if (bind(sockfd, (struct sockaddr*)&addr, sizeof addr) == SOCKET_ERROR) {
			fprintf(stderr, "Bind failed with error code : %d.", WSAGetLastError());
			return 1;
		}

		listen(sockfd, MAX_PLAYERS);

		// start the program up
		CreateProcessA (
			"..\\Debug\\UnstableUnicornsTest.exe",   // the path
			"..\\Debug\\UnstableUnicornsTest.exe client",		// Full command line (includes argv[0])
			NULL,                 // Process handle not inheritable
			NULL,                 // Thread handle not inheritable
			FALSE,                // Set handle inheritance to FALSE
			CREATE_NO_WINDOW,			// Process Create Flags; could do CREATE_NEW_CONSOLE to show separately
			NULL,									// Use parent's environment block
			NULL,									// Use parent's starting directory 
			&si,									// Pointer to STARTUPINFO structure
			&pi										// Pointer to PROCESS_INFORMATION structure
		);

		struct sockaddr_in client_addr;
		socklen_t client_addr_size = sizeof(client_addr);

		clientsockfd[0] = accept(sockfd, (struct sockaddr*)&client_addr, &client_addr_size);

	}

	// ********************************************************************************
	// **************************** Unit/Integration Tests ****************************
	// ********************************************************************************

	// redirect general output to make the cmd line cleaner and easier to parse through
	FILE* fp;
	freopen_s(&fp, "NUL", "w", stdout);

	// initialize the deck
	init_deck(&nursery, &deck, &discardpile);
	srand(0);

	// initialize the network states too
	init_network_states();

	// wheeeee~
	current_players = 1;
	strcpy_s(player[0].username, sizeof player[0].username, "one");
	strcpy_s(player[1].username, sizeof player[1].username, "two");
	strcpy_s(player[2].username, sizeof player[2].username, "three");

	fprintf(stderr, "Commencing testing for the ");
	rainbow_error("Unstable Unicorns");
	fprintf(stderr, " application demo! :D\n");

	if (!isclient) {
		// utility function test
		check_class_tests();
		rearrange_pile_tests();
		add_nursery_tests();
		add_stable_tests();
		rearrange_stable_tests();

		// basic card effect tests
		draw_tests();
		discard_tests();
		sacrifice_tests();
		destroy_tests();
		steal_tests();
	}

	// specific card functionality tests

	// magic unicorn cards
	num_fails += rhinocorn_tests();
	num_fails += extremely_fertile_unicorn_tests();
	num_fails += magical_kittencorn_tests();
	num_fails += stabby_the_unicorn_tests();
	num_fails += puppicorn_tests();
	num_fails += rainbow_unicorn_tests();
	num_fails += zombie_unicorn_tests();
	num_fails += extremely_destructive_unicorn_tests();
	num_fails += chainsaw_unicorn_tests();
	num_fails += llamacorn_tests();
	num_fails += americorn_tests();
	num_fails += ginormous_unicorn_tests();
	num_fails += seductive_unicorn_tests();
	num_fails += angel_unicorn_tests();
	num_fails += queen_bee_unicorn_tests();
	num_fails += greedy_flying_unicorn_tests();
	num_fails += annoying_flying_unicorn_tests();
	num_fails += magical_flying_unicorn_tests();
	num_fails += swift_flying_unicorn_tests();
	num_fails += majestic_flying_unicorn_tests();
	num_fails += unicorn_phoenix_tests();
	num_fails += unicorn_on_the_cob_tests();
	num_fails += black_knight_unicorn_tests();
	num_fails += shark_with_a_horn_tests();
	num_fails += shabby_the_narwhal_tests();
	num_fails += narwhal_torpedo_tests();
	num_fails += alluring_narwhal_tests();
	num_fails += mermaid_unicorn_tests();
	num_fails += classy_narwhal_tests();
	num_fails += the_great_narwhal_tests();

	// magic cards
	num_fails += unicorn_poison_tests();
	num_fails += back_kick_tests();
	num_fails += change_of_luck_tests();
	num_fails += glitter_tornado_tests();
	num_fails += unicorn_swap_tests();
	num_fails += re_target_tests();
	num_fails += unfair_bargain_tests();
	num_fails += two_for_one_tests();
	num_fails += unicorn_shrinkray_tests();
	num_fails += targeted_destruction_tests();
	num_fails += mystical_vortex_tests();
	num_fails += good_deal_tests();
	num_fails += shake_up_tests();
	num_fails += blatant_thievery_tests();
	num_fails += reset_button_tests();
	
	// upgrade cards
	num_fails += rainbow_mane_tests();
	num_fails += extra_tail_tests();
	num_fails += glitter_bomb_tests();
	num_fails += yay_tests();
	num_fails += unicorn_lasso_tests();
	num_fails += rainbow_aura_tests();
	num_fails += double_dutch_tests();
	num_fails += summoning_ritual_tests();
	
	// downgrade cards
	num_fails += barbed_wire_tests();
	num_fails += pandamonium_tests();
	num_fails += sadistic_ritual_tests();
	num_fails += slowdown_tests();
	num_fails += nanny_cam_tests();
	num_fails += broken_stable_tests();
	num_fails += blinding_light_tests();
	num_fails += tiny_stable_tests();
	
	// instant cards
	num_fails += neigh_tests();
	num_fails += super_neigh_tests();

	// ********************************************************************************
	// ***************************** Results and Clean-up *****************************
	// ********************************************************************************

	if (isclient) {
		char data[1024];
		int rc;
		while (rc = recv(sockfd, data, sizeof data, 0) > 0) {
			// loop until socket closes or server disconnects in some way
		}
		
		closesocket(sockfd);
		exit(1);
	}

	if (num_fails == 0) {
		green();
		fprintf(stderr, "\nAll tests have successfully passed! :D\n");
		reset_col();
	}
	else {
		red();
		fprintf(stderr, "\nA total of %d tests have failed :(\n", num_fails);
		reset_col();
	}

	// Close process and thread handles. 
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	closesocket(clientsockfd[0]);
	closesocket(sockfd);
	if (fp != NULL)
		fclose(fp);
	WSACleanup();

	_getch();
	exit(0);
}
