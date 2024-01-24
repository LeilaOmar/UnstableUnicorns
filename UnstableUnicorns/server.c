#include "server.h"
#include "gamemechanics.h"

#define ERRORBUF 256

int newConnection(SOCKET *cfd) {
  struct sockaddr_in client_addr;
  socklen_t client_addr_size = sizeof(client_addr);
  char errormsg[ERRORBUF];

  if ((cfd[current_players - 1] = accept(sockfd, (struct sockaddr*)&client_addr, &client_addr_size)) == INVALID_SOCKET) {
    printf("Accept failed with error code : %d", WSAGetLastError());
    return 1;
  }

  puts("Incoming connection accepted!");

  current_players++;
  return 0;
}

void initGame(void) {
  return;
}

int serverMain(void) {
  short portno;
  char *end, buf[LINE_MAX];
  struct sockaddr_in addr;

  // *****************************************************
  // ******************* Server Set-up *******************
  // *****************************************************

  printf("\nEnter your desired username (max 32 chars): ");
  fgets(player[current_players].username, sizeof player[current_players].username, stdin);
  player[current_players].username[strlen(player[current_players].username) - 1] = 0;
  current_players++;

  do {
    printf("Enter a valid port number to connect to (between 1024 and 65535 inclusive): ");
    fgets(buf, sizeof buf, stdin);
    buf[strlen(buf) - 1] = 0;
    portno = strtol(buf, &end, 10);
  } while (portno < 1024 || portno > 65535 || end != (buf + strlen(buf)));

  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = htons(portno);

  if (bind(sockfd, (struct sockaddr*)&addr, sizeof addr) == SOCKET_ERROR) {
    printf("Bind failed with error code : %d.", WSAGetLastError());
    return 1;
  }

  listen(sockfd, MAX_PLAYERS);

  puts("Waiting for new players...");

  // *****************************************************
  // ******************** Poll Set-up ********************
  // *****************************************************

  SOCKET clientsockfd[MAX_PLAYERS - 1];

  // server should be in non-blocking mode
  unsigned long on = 1;
  ioctlsocket(sockfd, FIONBIO, &on);

  // read data from the connection
  char data[BUF_SIZE] = { 0 };
  int count, ret, isvalid;
  WSAPOLLFD pfd[MAX_PLAYERS + 1] = { -1 };  // stdin + original sockfd + 7 additional players
  pfd[0].fd = _fileno(stdin);
  pfd[0].events = POLLIN | POLLOUT;
  pfd[1].fd = sockfd;
  pfd[1].events = POLLIN | POLLOUT;

  for (;;) {
    // timeout after 150 seconds
    ret = WSAPoll(pfd, MAX_PLAYERS + 1, -1);
    if (ret == SOCKET_ERROR) {
      printf( "ERROR: poll() failed. Error code : %d", WSAGetLastError());
      closesocket(sockfd);
      return 2;
    }
    else if (ret == 0) {
      printf("ERROR: server timed out. Error code : %d", WSAGetLastError());
      closesocket(sockfd);
      return 2;
    }

    // incoming connection
    if (pfd[1].revents & POLLIN) {
      // create a new connection given that there aren't already 10 clients
      if (current_players < MAX_PLAYERS - 1) {
        newConnection(clientsockfd);
        ioctlsocket(clientsockfd[current_players - 2], FIONBIO, &on);
        pfd[current_players].fd = clientsockfd[current_players - 2];
        pfd[current_players].events = POLLIN | POLLOUT;
      }
    }

    if (pfd[1].revents & (POLLHUP | POLLERR)) {
      puts("ERROR: Server received POLLHUP or POLLERR signal");
      closesocket(sockfd);
      return 2;
    }

    // incoming signal from stdin
    //if (pfd[0].revents & POLLIN) {
    //if (ret) {
    //WaitForSingleObject(GetStdHandle(STD_INPUT_HANDLE), INFINITE);
    if (WaitForSingleObject(GetStdHandle(STD_INPUT_HANDLE), 1) == WAIT_OBJECT_0) {
      memset(buf, '\0', sizeof buf);
      fgets(buf, sizeof buf, stdin);
      buf[strlen(buf) - 1] = 0;

      //puts("inside stdin POLLIN");

      if (strncmp(buf, "start", 5) == 0 && current_players >= 2) {
        break;
      }
    }

    // IO operation came in from a client socket (username check!)
    for (int i = 0; i < current_players - 1; i++) {
      if (pfd[i + 2].revents & POLLIN && clientsockfd[i] != -1) {
        do {
          memset(buf, '\0', sizeof buf);
          isvalid = 0;
          recv(clientsockfd[i], buf, LINE_MAX, 0);

          for (int j = 0; j < current_players; j++) {
            if (strcmp(buf, player[j].username) == 0) {
              isvalid = 1;
              break;
            }
          }
          // send(clientsockfd[i], (char*)&isvalid, sizeof isvalid, 0);
          sendInt(isvalid, clientsockfd[i]);
        } while (isvalid == 1);

        strcpy_s(player[i + 1].username, sizeof player[i + 1].username, buf);
        printf("Player ");
        red();
        printf("%s", player[i + 1].username);
        reset_col();
        puts(" has joined the game!\n");
      }

      if (pfd[i + 2].revents & (POLLHUP | POLLERR)) {
        puts("ERROR: Server received POLLHUP or POLLERR signal");
        printf("Player %s disconnected :(\n", player[i].username);
        closesocket(clientsockfd[i]);
        // return 2;
      }
    }
  }
  // *****************************************************
  // ******************** Game Set-up ********************
  // *****************************************************

  int index, index2, result, rc;
  char *end2, buf2[LINE_MAX];

  rainbow("\n*******************"
    "\n*** Game Start! ***"
    "\n*******************\n\n");

  shuffleDeck(deck_index, DECK_SIZE);

  if (deck_flag) {
    blue();
    printDeck(deck_index, DECK_SIZE, ANY, ANY);
    reset_col();
  }

  // initialize variables, draw 5 cards and "pick" username
  for (int i = 0; i < current_players; i++) {
    player[i].hand.num_cards = 0;
    draw(i, HAND_START);
  }

  printNursery(nursery_index, NURSERY_SIZE);
  do {
    red();
    printf("%s: ", player[0].username);
    reset_col();
    printf("Pick the index associated with your desired Baby Unicorn: ");
    fgets(buf, sizeof buf, stdin);
    buf[strlen(buf) - 1] = 0;
    index = strtol(buf, &end, 10) - 1;
  } while (index < 0 || index >= (int)dnurse_size || end != (buf + strlen(buf)));

  player[0].stable.unicorns[0] = nursery_ref[index + nursery_index];
  player[0].stable.size = 1;
  player[0].stable.num_unicorns = 1;

  rearrangeNursery(index + nursery_index);

  red();
  puts("Waiting for other players to pick their Baby Unicorn...");
  reset_col();

  // TODO: serialize this for later; also check the amount of bytes sent!
  // TODO: watch out for EWOULDBLOCK; this works on my local but what about online?
  for (int i = 1; i < current_players; i++) {
    sendInt(current_players, clientsockfd[i - 1]);
    sendInt(i, clientsockfd[i - 1]);
    send(clientsockfd[i - 1], (void*)&player[i], sizeof(struct Player), 0);
    sendInt(nursery_index, clientsockfd[i - 1]);
    sendInt(dnurse_size, clientsockfd[i - 1]);
    send(clientsockfd[i - 1], (void*)nursery_ref, sizeof(nursery_ref), 0);
    
    receiveInt(&index, clientsockfd[i - 1]);
    player[i].stable.unicorns[0] = nursery_ref[index + nursery_index];
    player[i].stable.size = 1;
    player[i].stable.num_unicorns = 1;
    
    rearrangeNursery(index + nursery_index);
  }

  // player[0].hand.cards[player[0].hand.num_cards++] = 94;
  // player[1].hand.cards[player[1].hand.num_cards++] = 49;
  // player[2].hand.cards[player[2].hand.num_cards++] = 102;

  for (int i = 1; i < current_players; i++) {
    ret = send(clientsockfd[i - 1], (void*)player, sizeof(player), 0);
    if (ret != sizeof(player)) {
      printf("data wasn't fully sent for player[%d]\n", i);
    }
  }

  puts("\n**********\n");
  for (int i = 0; i < current_players; i++) {
    printHand(i);
    printStable(i);
    puts("\n**********\n");
  }

  // *****************************************************
  // ******************** Game Start! ********************
  // *****************************************************

  int counter = 0;
  // asynch stuff: http://archive.gamedev.net/archive/reference/articles/article1297.html22

  // loop until win condition occurs (7 unicorns in stable)
  for (;;) {
    red();
    printf("\n*** %s's turn ***\n\n", player[counter].username);
    reset_col();

    // it's your turn! do your thing :>
    //if (counter == 0) {
      // *********************************************
      // ********** beginning of turn phase **********
      // *********************************************

      draw(counter, 1);
      printHand(counter);
      turn_count = 1;
      uni_lasso_flag[0] = -1;

      // Nanny Cam check
      for (int i = 0; i < current_players; i++) {
        if (i != counter && (player[i].flags & nanny_cam) != 0)
          printHand(i);
      }

      for (int i = 0; i < player[counter].stable.size; i++) {
        beginningTurnEffects(counter,
          deck[player[counter].stable.unicorns[i]].effect);
      }

      // **********************************
      // ********** action phase **********
      // **********************************

      // choose between drawing or playing a card
      // OPTIMIZE: skip go straight to drawing another card in case all cards in
      // hand are impossible plays (e.g. you MUST [xxx] when there are no
      // available cards to be affected)

      while (turn_count > 0) {
        do {
          printf(
            "\nAction phase options:"
            "\n1. Draw a card"
            "\n2. Play a card"
            "\n3. Display card description"
            "\n4. Display a player's stable"
            "\n5. Display your own hand"
            "\n6. Display Nursery"
            "\n7. Display Discard Pile"
            "\nChoice: ");
          fgets(buf, sizeof buf, stdin);
          buf[strlen(buf) - 1] = 0;
          index = strtol(buf, &end, 10);
          if (index == 3)
            displayCardDesc();
          else if (index == 4)
            displayDesiredStable();
          else if (index == 5)
            printHand(counter);
          else if (index == 6) {
            yellow();
            printNursery(nursery_index, NURSERY_SIZE);
            reset_col();
          }
          else if (index == 7) {
            magenta();
            printDiscard(ANY);
            reset_col();
          }
        } while (index < 1 || index > 2 || end != (buf + strlen(buf)));
        if (index == 1)
          draw(counter, 1);
        else
          playCard(counter);

        turn_count--;
      }

      // ***************************************
      // ********** end of turn phase **********
      // ***************************************

      // discard extra cards since max hand limit in-game is 7
      while (player[counter].hand.num_cards > 7) {
        // optimize this later... maybe if statement and discard multiple
        // instead of using another loop
        discard(counter, 1, ANY);
      }

      // print stuff to see what's going on, duh
      printHand(counter);
      printStable(counter);

      // cycle between players; count after checking number of unicorns
      // maybe just check if the number is >= 7 every time a card enters your
      // stable; ginormous unicorn counts for 2 unless blinding light is in effect
      // pandamonium means you have 7+ pandas, not unicorns
      // TODO: player won with this stable??? only 6 unicorns; tested it in a different
      // game and rainbow mane bringing in cards did not make the game end prematurely
      // so maybe there is some kind of issue with accidentally using the wrong effect
      // that would trigger the ginormous unicorn flag?
      //  1. Baby Unicorn(Rainbow) [ID:10]
      //  2. Basic Unicorn(Green) [ID:23]
      //  3. Basic Unicorn(Purple) [ID:32]
      //  4. Rainbow Mane [ID:94]
      //  5. Basic Unicorn(Blue) [ID:26]
      //  6. Alluring Narwhal [ID:63]
      //  7. Black Knight Unicorn [ID:59]
      if ((player[counter].stable.num_unicorns >= WIN_CONDITION ||
          (player[counter].stable.num_unicorns >= (WIN_CONDITION - 1) &&
            (player[counter].flags & (ginormous_unicorn | blinding_light)) == ginormous_unicorn)) &&
          (player[counter].flags & pandamonium) == 0)
        break;

      // return any unicorns or pass any unicorns to the proper or next owner
      if (uni_lasso_flag[0] != -1) {
        // uni_lasso_flag[2] represents proper owner, and the thief [1] has the
        // card removed from their stable
        addStable(uni_lasso_flag[2], player[uni_lasso_flag[1]].stable.unicorns[uni_lasso_flag[0]]);
        // detoggle potential unicorn flags from the stealer once it leaves the
        // stable so that it isn't improperly left
        toggleFlags(
          uni_lasso_flag[1],
          deck[player[uni_lasso_flag[1]].stable.unicorns[uni_lasso_flag[0]]]
          .effect);
        rearrangeStable(uni_lasso_flag[1], uni_lasso_flag[0]);
        enterStableEffects(
          uni_lasso_flag[2],
          deck[player[uni_lasso_flag[2]]
          .stable.unicorns[player[uni_lasso_flag[2]].stable.size - 1]]
          .effect);
      }

      // puppicorn swap
      if (puppicorn_index != -1) {
        if (counter == current_players - 1) {
          addStable(0, player[counter].stable.unicorns[puppicorn_index]);
          rearrangeStable(counter, puppicorn_index);
          puppicorn_index = player[0].stable.size - 1;
        }
        else {
          addStable(counter + 1, player[counter].stable.unicorns[puppicorn_index]);
          rearrangeStable(counter, puppicorn_index);
          puppicorn_index = player[counter + 1].stable.size - 1;
        }
      }
    //}

    // print state of deck, nursery, and discard pile
    if (nursery_flag) {
      yellow();
      printNursery(nursery_index, NURSERY_SIZE);
      reset_col();
    }
    if (discard_flag) {
      magenta();
      printDiscard(ANY);
      reset_col();
    }

    counter = (counter < current_players - 1) ? counter + 1 : 0;
  }

  char winmsg[DESC_SIZE];
  sprintf_s(winmsg, NAME_SIZE + 18, "%s won the game!!!\n", player[counter].username);
  rainbow(winmsg);
  getch();

  return 0;
}
