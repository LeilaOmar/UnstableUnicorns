#include "server.h"
#include "gamemechanics.h"
#include "gamephase.h"
#include <windows.h>

int newConnection(SOCKET *cfd) {
  struct sockaddr_in client_addr;
  socklen_t client_addr_size = sizeof(client_addr);

  if ((cfd[current_players - 1] = accept(sockfd, (struct sockaddr*)&client_addr, &client_addr_size)) == INVALID_SOCKET) {
    fprintf(stderr, "Accept failed with error code : %d", WSAGetLastError());
    return 1;
  }

  puts("Incoming connection accepted!");

  current_players++;
  return 0;
}

int serverMain(void) {
  short portno;
  struct sockaddr_in addr;
  char *end, buf[LINE_MAX];

  // *****************************************************
  // ******************* Server Set-up *******************
  // *****************************************************

  printf("\nEnter your desired username (max 32 chars): ");
  fgets(player[current_players].username, sizeof player[current_players].username, stdin);
  player[current_players].username[strlen(player[current_players].username) - 1] = 0;
  current_players++; // the host is player one :>

  do {
    printf("Enter a valid port number to connect to (between 1024 and 65535 inclusive): ");
    portno = numinput(buf, &end, sizeof buf);
  } while (portno < 1024 || portno > 65535 || end != (buf + strlen(buf)));

  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = htons(portno);

  if (bind(sockfd, (struct sockaddr*)&addr, sizeof addr) == SOCKET_ERROR) {
    fprintf(stderr, "Bind failed with error code : %d.", WSAGetLastError());
    return 1;
  }

  listen(sockfd, MAX_PLAYERS);

  printf(
    "\n********************************************************************************"
    "\n********************************* Waiting Room *********************************"
    "\n********************************************************************************"
  );

  red();
  printf("\n\nOnce two or more people are in a lobby, you may type and enter 'start' to start the game!"
    "\nOther messages get sent to your entire party.\n\n");
  reset_col();

  // *****************************************************
  // ******************** Poll Set-up ********************
  // *****************************************************

  int ret, isvalid;
  char stdinbuf[MSGBUF] = { 0 };
  int bufindex = 0;

  // server should be in non-blocking mode
  unsigned long on = 1;
  ioctlsocket(sockfd, FIONBIO, &on);

  WSAPOLLFD pfd[MAX_PLAYERS + 1] = { -1 };  // stdin + original sockfd + 7 additional players
  pfd[0].fd = _fileno(stdin); // personally could not get this to work in windows :/
  pfd[0].events = POLLIN | POLLOUT;
  pfd[1].fd = sockfd;
  pfd[1].events = POLLIN | POLLOUT;

  for (;;) {
    // timeout after 150 seconds
    ret = WSAPoll(pfd, MAX_PLAYERS + 1, -1);
    if (ret == SOCKET_ERROR) {
      printf( "ERROR: poll() failed. Error code : %d", WSAGetLastError());
      exit(2);
    }
    else if (ret == 0) {
      printf("ERROR: server timed out. Error code : %d", WSAGetLastError());
      exit(2);
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
      exit(2);
    }

    // incoming signal from stdin (through handle, not pipe)
    if (WaitForSingleObject(GetStdHandle(STD_INPUT_HANDLE), 0) == WAIT_OBJECT_0) {
      processStdin(stdinbuf, &bufindex);
      if (bufindex >= (sizeof(stdinbuf) - 1) || stdinbuf[bufindex - 1] == '\r' || stdinbuf[bufindex - 1] == '\n') {
        if (strncmp(stdinbuf, "start", 5) == 0 && current_players >= 2) {
          network_events = start_game;
          for (int i = 0; i < current_players - 1; i++) {
            sendInt(network_events, clientsockfd[i]);
          }
          break;
        }

        network_events = incoming_msg;
        for (int i = 0; i < current_players - 1; i++) {
          sendInt(network_events, clientsockfd[i]);
          sendInt(0, clientsockfd[i]);
          send(clientsockfd[i], stdinbuf, MSGBUF, 0);
        }

        memset(stdinbuf, '\0', sizeof stdinbuf);
        bufindex = 0;
      }
    }

    // IO operation came in from a client socket (username check!)
    for (int i = 0; i < current_players - 1; i++) {
      if (pfd[i + 2].revents & POLLIN && clientsockfd[i] != -1) {
        // new player
        if (player[i + 1].username[0] == '\0') {
          memset(buf, '\0', sizeof buf);
          isvalid = 0;
          recv(clientsockfd[i], buf, LINE_MAX, 0);

          for (int j = 0; j < current_players; j++) {
            if (strcmp(buf, player[j].username) == 0) {
              isvalid = 1;
              break;
            }
          }

          sendInt(isvalid, clientsockfd[i]);

          if (!isvalid) {
            sendInt(i + 1, clientsockfd[i]);

            strcpy_s(player[i + 1].username, sizeof player[i + 1].username, buf);
            printf("Player ");
            red();
            printf("%s", player[i + 1].username);
            reset_col();
            puts(" has joined the game!\n");

            network_events = player_join;
            for (int j = 0; j < current_players - 1; j++) {
              sendInt(network_events, clientsockfd[j]);
              sendPlayers(clientsockfd[j]);
            }
          }
        }
        else {
          receiveInt(&network_events, clientsockfd[i]);
          if (network_events == incoming_msg) {
            receiveMsg(stdinbuf, sizeof stdinbuf, clientsockfd[i]);

            // the pnum in receiveMsg is the same as the client socket number (i.e. clientsockfd[i])
            for (int j = 0; j < current_players - 1; j++) {
              if (j == i) continue;
              sendInt(network_events, clientsockfd[j]);
              sendInt(i + 1, clientsockfd[j]);
              send(clientsockfd[j], stdinbuf, sizeof stdinbuf, 0);
            }

            memset(stdinbuf, '\0', sizeof stdinbuf);
          }
        }
      }

      if (pfd[i + 2].revents & (POLLHUP | POLLERR)) {
        printf("Player %s disconnected :(\n", player[i + 1].username);

        closesocket(clientsockfd[i]);
        current_players--;

        for (int j = i; j < current_players - 1; j++) {
          clientsockfd[j] = clientsockfd[j + 1];
        }

      }
    }

    Sleep(20);
  }

  // *****************************************************
  // ******************** Game Set-up ********************
  // *****************************************************

  int index;

  rainbow(
    "\n*********************************"
    "\n*** Choose your baby unicorn! ***"
    "\n*********************************\n\n");

  printPile(nursery);
  do {
    red();
    printf("%s: ", player[0].username);
    reset_col();
    printf("Pick the index associated with your desired Baby Unicorn: ");
    index = numinput(buf, &end, sizeof buf) - 1;
  } while (index < 0 || index >= nursery.size || end != (buf + strlen(buf)));

  player[0].stable.unicorns[0] = nursery.cards[index];
  player[0].stable.size = 1;
  player[0].stable.num_unicorns = 1;

  rearrangePile(&nursery, index);

  red();
  puts("Waiting for other players to pick their Baby Unicorn...");
  reset_col();

  // TODO: watch out for EWOULDBLOCK; this works on my local machine, but what about online?
  for (int i = 1; i < current_players; i++) {
    sendInt(current_players, clientsockfd[i - 1]);
    sendPlayers(clientsockfd[i - 1]);
    sendInt(nursery.size, clientsockfd[i - 1]);
    sendUnicorns(nursery.cards, nursery.size, clientsockfd[i - 1]);

    receiveInt(&index, clientsockfd[i - 1]);
    player[i].stable.unicorns[0] = nursery.cards[index];
    player[i].stable.size = 1;
    player[i].stable.num_unicorns = 1;
    
    rearrangePile(&nursery, index);
  }

  // all of the baby unicorns have been chosen, now initialize the hands/deck
  shuffleDeck(&deck);

  if (deck_flag) {
    blue();
    printPile(deck);
    reset_col();
  }

  // initialize variables, draw 5 cards and "pick" username
  for (int i = 0; i < current_players; i++) {
    player[i].hand.num_cards = 0;
    draw(i, HAND_START);
  }

  // just have to trust that pepole don't cheat by examining the code mid-game lol
  for (int i = 1; i < current_players; i++) {
    sendPlayers(clientsockfd[i - 1]);
    sendInt(nursery.size, clientsockfd[i - 1]);
    sendUnicorns(nursery.cards, nursery.size, clientsockfd[i - 1]);
  }

  // *****************************************************
  // ******************** Game Start! ********************
  // *****************************************************

  printf("\n\n"
    "                 &(,    _____                         _____ _             _     \n"
    "               &%%(     / ____|                       / ____| |           | |    \n"
    "         #&&&&,       | |  __  __ _ _ __ ___   ___  | (___ | |_ __ _ _ __| |_   \n"
    "         &&&&#(       | | |_ |/ _` | '_ ` _ \\ / _ \\  \\___ \\| __/ _` | '__| __|  \n"
    "  &&&&&&              | |__| | (_| | | | | | |  __/  ____) | || (_| | |  | |_   \n"
    "  (&&&&&&%%             \\_____|\\__,_|_| |_| |_|\\___| |_____/ \\__\\__,_|_|   \\__|  \n"
    "     &&&#(                                                                      \n"
    "\n");

  int counter = 0;
  int didWin = 0;
  int winningpnum = 0;
  // asynch stuff: http://archive.gamedev.net/archive/reference/articles/article1297.html22

  // loop until win condition occurs (7 unicorns in stable)
  do {
    printf("\n********************************************************************************\n");
    red();
    printf("\nPlayer Stables\n");
    reset_col();
    printStableGrid();

    red();
    printf("\n*** %s's turn ***\n\n", player[counter].username);
    reset_col();

    if (counter == 0) {
      // it's your turn! do your thing :>
      beginningOfTurn(counter);

      actionPhase(counter);

      didWin = endOfTurn(counter);
      if (didWin == 1) {
        for (int i = 0; i < current_players - 1; i++) {
          sendInt(end_game, clientsockfd[i]);
          sendInt(counter, clientsockfd[i]);
          sendGamePacket(clientsockfd[i]);
        }
        winningpnum = 0;
        break;
      }

      // signal the next turn cycle and send the deck/player data to everyone else
      for (int i = 0; i < current_players - 1; i++) {
        sendInt(end_turn, clientsockfd[i]);
        sendGamePacket(clientsockfd[i]);
      }
    }
    else {
      printf("waiting for %s to make a move...\n", player[counter].username);
      int eventloop = 0;

      do {
        ret = WSAPoll(pfd, MAX_PLAYERS + 1, -1);
        if (ret == SOCKET_ERROR) {
          fprintf(stderr, "ERROR: poll() failed. Error code : %d", WSAGetLastError());
          exit(2);
        }
        else if (ret == 0) {
          fprintf(stderr, "ERROR: server timed out. Error code : %d", WSAGetLastError());
          exit(2);
        }

        for (int k = 0; k < current_players - 1; k++) {
          if (pfd[k + 2].revents & POLLIN) {
            receiveInt(&network_events, clientsockfd[k]);

            // neigh stuff
            if (network_events == neigh_event) {
              int cindex;
              receiveInt(&cindex, clientsockfd[k]);
              receivePlayers(clientsockfd[k]); // this is for updating the current player's hand after the beginning stable effects and drawing
              serverNeigh(k + 1, &cindex);
            }
            else if (network_events == discard_event) {
              int target_player;
              int desired_type;
              receiveCardEffectPacket(&target_player, &desired_type, clientsockfd[k]);
              serverDiscard(k + 1, target_player, desired_type);
            }
            else if (network_events == sacrifice_event) {
              int target_player;
              int desired_type;
              receiveCardEffectPacket(&target_player, &desired_type, clientsockfd[k]);
              serverSacrifice(k + 1, target_player, desired_type);
            }
            else if (network_events == enter_stable_event) {
              struct Unicorn corn;
              int target_player;
              receiveEnterStablePacket(&corn, &target_player, clientsockfd[k]);
              
              if (target_player != 0) {
                sendInt(enter_stable_event, clientsockfd[target_player - 1]);
                // k + 1 represents the original player
                sendEnterStablePacket(corn, k + 1, clientsockfd[target_player - 1]);
                serverEnterStable(k + 1, target_player);
              }
              else {
                printf("\n\033[1;31m%s\033[0m sent you the card \033[1;31m'%s'\033[0m.\n", player[k + 1].username, corn.name);
                addStable(0, corn);

                if (!checkWin(0)) {
                  sendInt(quit_loop, clientsockfd[k]);
                  sendGamePacket(clientsockfd[k]);
                }
                else {
                  eventloop = 1;
                  didWin = 1;
                  winningpnum = 0;

                  for (int i = 0; i < current_players - 1; i++) {
                    sendInt(end_game, clientsockfd[i]);
                    sendInt(winningpnum, clientsockfd[i]);
                    sendGamePacket(clientsockfd[i]);
                  }
                  break;
                }
              }
            }
            else if (network_events == end_turn) {
              receiveGamePacket(clientsockfd[k]);
              eventloop = 1;

              for (int i = 0; i < current_players - 1; i++) {
                if (k == i) continue;

                sendInt(end_turn, clientsockfd[i]);
                sendGamePacket(clientsockfd[i]);
              }

              break;
            }
            else if (network_events == end_game) {
              receiveGamePacket(clientsockfd[k]);
              eventloop = 1;
              didWin = 1;
              winningpnum = k + 1;

              for (int i = 0; i < current_players - 1; i++) {
                if (k == i) continue;

                sendInt(end_game, clientsockfd[i]);
                sendInt(winningpnum, clientsockfd[i]);
                sendGamePacket(clientsockfd[i]);
              }
              break;
            }
          }

          Sleep(20);
        } // network_events polling

      } while (!eventloop);
    }

    // print state of nursery and discard piles
    if (nursery_flag) {
      yellow();
      printPile(nursery);
      reset_col();
    }
    if (discard_flag) {
      magenta();
      printPile(discardpile);
      reset_col();
    }

    counter = (counter < current_players - 1) ? counter + 1 : 0;
  } while (!didWin);

  printf("\n********************************************************************************\n");
  red();
  printf("\nPlayer Stables\n");
  reset_col();
  printStableGrid();

  // https://www.asciiart.eu/mythology/unicorns
  printf("\n\n"
    "                    /  \n"
    "               ,.. /   \n"
    "             ,'   ';        _____                         _____      _   _ \n"
    "  ,,.__    _,' /';  .      / ____|                       / ____|    | | | |\n"
    " :','  ~~~~    '. '~      | |  __  __ _ _ __ ___   ___  | (___   ___| |_| |\n"
    ":' (   )         )::,     | | |_ |/ _` | '_ ` _ \\ / _ \\  \\___ \\ / _ \\ __| |\n"
    "'. '. .=----=..-~  .;'    | |__| | (_| | | | | | |  __/  ____) |  __/ |_|_|\n"
    " '  ;'  ::   ':.  '\"       \\_____|\\__,_|_| |_| |_|\\___| |_____/ \\___|\\__(_)\n"
    "   (:   ':    ;)       \n"
    "    \\\\   '\"  ./        \n"
    "     '\"      '\"     *ASCII unicorn by Dr J   \n"
    "\n");

  char winmsg[DESC_SIZE];
  sprintf_s(winmsg, NAME_SIZE + 18, "%s won the game!!!\n", player[winningpnum].username);
  rainbow(winmsg);

  printf("\nPress any key to close the window...");

  return 0;
}
