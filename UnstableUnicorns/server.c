#include "server.h"
#include "gamemechanics.h"
#include "gamephase.h"

/**
 * @brief Establishes a new connection between the server and the client socket fd
 * @return 0 Success
 * @return 1 Failed to connect
 */
static int NewConnection(SOCKET *cfd) {
  struct sockaddr_in client_addr;
  socklen_t client_addr_size = sizeof(client_addr);
  char errormsg[DESC_SIZE];

  if ((cfd[currentPlayers - 1] = accept(sockfd, (struct sockaddr*)&client_addr, &client_addr_size)) == INVALID_SOCKET) {
    fprintf(stderr, "Accept failed with error code : %d", WSAGetLastError());
    return 1;
  }

  puts("Incoming connection accepted!");

  currentPlayers++;
  return 0;
}

/**
 * @brief Server socket set-up
 * @return 0 Success
 * @return 1 Failed to connect
 */
static int InitServer(void) {
  short portno;
  struct sockaddr_in addr;
  char *end, buf[LINE_MAX];

  printf("\nEnter your desired username (max 32 chars): ");
  fgets(player[0].username, sizeof player[0].username, stdin);
  player[0].username[strlen(player[0].username) - 1] = 0;
  currentPlayers = 1; // the host is player one :>

  do {
    printf("Enter a valid port number to connect to (between 1024 and 65535 inclusive): ");
    portno = NumInput(buf, &end, sizeof buf);
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

  Red();
  printf("\n\nOnce two or more people are in a lobby, you may type and enter 'start' to start the game!"
    "\nOther messages get sent to your entire party.\n\n");
  ResetCol();
  return 0;
}

int ServerMain(void) {
  int ret = InitServer();

  if (ret) return ret;

  // *****************************************************
  // ******************** Poll Set-up ********************
  // *****************************************************

  int isvalid;
  int connected_players = 1;
  int networkEvents;
  int bufIndex = 0;
  char stdinBuf[MSGBUF] = { 0 };
  char *end, buf[LINE_MAX];

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
      printf("ERROR: poll() failed. Error code : %d", WSAGetLastError());
      exit(2);
    }
    else if (ret == 0) {
      printf("ERROR: server timed out. Error code : %d", WSAGetLastError());
      exit(2);
    }

    // incoming connection
    if (pfd[1].revents & POLLIN) {
      // create a new connection given that there aren't already 10 clients
      if (currentPlayers < MAX_PLAYERS - 1) {
        NewConnection(clientsockfd);
        ioctlsocket(clientsockfd[currentPlayers - 2], FIONBIO, &on);
        pfd[currentPlayers].fd = clientsockfd[currentPlayers - 2];
        pfd[currentPlayers].events = POLLIN | POLLOUT;
      }
    }

    if (pfd[1].revents & (POLLHUP | POLLERR)) {
      puts("ERROR: Server received POLLHUP or POLLERR signal");
      exit(2);
    }

    // incoming signal from stdin (through handle, not pipe)
    if (WaitForSingleObject(GetStdHandle(STD_INPUT_HANDLE), 0) == WAIT_OBJECT_0) {
      ProcessStdin(stdinBuf, &bufIndex);
      if (bufIndex >= (sizeof(stdinBuf) - 1) || stdinBuf[bufIndex - 1] == '\r' || stdinBuf[bufIndex - 1] == '\n') {
        if (strncmp(stdinBuf, "start", 5) == 0 && currentPlayers >= 2 && connected_players == currentPlayers) {
          for (int i = 0; i < currentPlayers - 1; i++) {
            SendInt(START_GAME, clientsockfd[i]);
          }

          // erase the player's name and incomplete message; the extra 2 comes from the colon and space
          for (int i = 0; i < (int)strlen(player[0].username) + bufIndex + 2; i++) {
            printf("\b \b");
          }
          break;
        }

        for (int i = 0; i < currentPlayers - 1; i++) {
          if (player[i + 1].username[0] == '\0') continue;

          SendInt(INCOMING_MSG, clientsockfd[i]);
          SendInt(0, clientsockfd[i]);
          SendMsg(stdinBuf, sizeof stdinBuf, clientsockfd[i]);
        }

        memset(stdinBuf, '\0', sizeof stdinBuf);
        bufIndex = 0;

        printf("\033[1;31m%s\033[0m: ", player[0].username);
      }
    }

    // IO operation came in from a client socket (username check!)
    for (int i = 0; i < currentPlayers - 1; i++) {
      if (pfd[i + 2].revents & POLLIN && clientsockfd[i] != -1) {
        // new player
        if (player[i + 1].username[0] == '\0') {
          memset(buf, '\0', sizeof buf);
          recv(clientsockfd[i], buf, LINE_MAX, 0);
          isvalid = 1;

          for (int j = 0; j < currentPlayers; j++) {
            if (strcmp(buf, player[j].username) == 0) {
              isvalid = 0;
              break;
            }
          }

          SendInt(isvalid, clientsockfd[i]);

          if (isvalid) {
            connected_players++;
            SendInt(i + 1, clientsockfd[i]);

            // erase the player's name and incomplete message; the extra 2 comes from the colon and space
            for (int i = 0; i < (int)strlen(player[0].username) + bufIndex + 2; i++) {
              printf("\b \b");
            }

            strcpy_s(player[i + 1].username, sizeof player[i + 1].username, buf);
            printf("Player ");
            Red();
            printf("%s", player[i + 1].username);
            ResetCol();
            puts(" has joined the game!\n");

            // rewrite the typed message
            printf("\033[1;31m%s\033[0m: %s", player[0].username, stdinBuf);

            for (int j = 0; j < currentPlayers - 1; j++) {
              if (player[j + 1].username[0] == '\0') continue;

              SendInt(PLAYER_JOIN, clientsockfd[j]);
              SendPlayers(clientsockfd[j]);
            }
          }
        }
        else {
          ReceiveInt(&networkEvents, clientsockfd[i]);

          if (networkEvents == INCOMING_MSG) {
            int pnum;
            char recvbuf[MSGBUF] = { 0 };

            ReceiveInt(&pnum, clientsockfd[i]);
            ReceiveMsg(recvbuf, clientsockfd[i]);

            // erase the player's name and incomplete message; the extra 2 comes from the colon and space
            for (int i = 0; i < (int)strlen(player[0].username) + bufIndex + 2; i++) {
              printf("\b \b");
            }

            Red();
            printf("%s: ", player[pnum].username);
            ResetCol();
            printf("%s\n", recvbuf);

            // rewrite the typed message
            printf("\033[1;31m%s\033[0m: %s", player[0].username, stdinBuf);

            // the pnum in ReceiveMsg is the same as the client socket number (i.e. clientsockfd[i])
            for (int j = 0; j < currentPlayers - 1; j++) {
              if (j == i || player[j + 1].username[0] == '\0') continue;

              SendInt(networkEvents, clientsockfd[j]);
              SendInt(i + 1, clientsockfd[j]);
              SendMsg(recvbuf, sizeof recvbuf, clientsockfd[j]);
            }
          }
        }
      }

      if (pfd[i + 2].revents & (POLLHUP | POLLERR)) {
        // erase the player's name and incomplete message; the extra 2 comes from the colon and space
        for (int i = 0; i < (int)strlen(player[0].username) + bufIndex + 2; i++) {
          printf("\b \b");
        }

        printf("Player ");
        Red();
        printf("%s", player[i + 1].username);
        ResetCol();
        printf(" disconnected :(\n");

        // rewrite the typed message
        printf("\033[1;31m%s\033[0m: %s", player[0].username, stdinBuf);

        currentPlayers--;
        connected_players--;

        for (int j = i; j < currentPlayers - 1; j++) {
          clientsockfd[j] = clientsockfd[j + 1];
          player[j + 1] = player[j + 2];
          pfd[j + 2] = pfd[j + 3];
        }

        memset(player[currentPlayers].username, '\0', sizeof player[0].username);
        clientsockfd[currentPlayers - 1] = 0;
        closesocket(clientsockfd[currentPlayers - 1]);

        // update the rest of the party
        for (int j = 0; j < currentPlayers - 1; j++) {
          SendInt(PLAYER_DISCONNECT, clientsockfd[j]);
          SendInt(i + 1, clientsockfd[j]); // i + 1 == player that left
          SendPlayers(clientsockfd[j]);
        }
      }
    }

    Sleep(20);
  }

  // *****************************************************
  // ******************** Game Set-up ********************
  // *****************************************************

  int index;

  Rainbow(
    "\n*********************************"
    "\n*** Choose your baby unicorn! ***"
    "\n*********************************\n\n");

  PrintPile(nursery);
  do {
    Red();
    printf("%s: ", player[0].username);
    ResetCol();
    printf("Pick the index associated with your desired Baby Unicorn: ");
    index = NumInput(buf, &end, sizeof buf) - 1;
  } while (index < 0 || index >= nursery.size || end != (buf + strlen(buf)));

  player[0].stable.unicorns[0] = nursery.cards[index];
  player[0].stable.size = 1;
  player[0].stable.numUnicorns = 1;

  RearrangePile(&nursery, index);

  Red();
  puts("Waiting for other players to pick their Baby Unicorn...");
  ResetCol();

  for (int i = 1; i < currentPlayers; i++) {
    SendInt(currentPlayers, clientsockfd[i - 1]);
    SendPlayers(clientsockfd[i - 1]);
    SendInt(nursery.size, clientsockfd[i - 1]);
    SendUnicorns(nursery.cards, nursery.size, clientsockfd[i - 1]);

    ReceiveInt(&index, clientsockfd[i - 1]);
    player[i].stable.unicorns[0] = nursery.cards[index];
    player[i].stable.size = 1;
    player[i].stable.numUnicorns = 1;

    RearrangePile(&nursery, index);
  }

  // all of the baby unicorns have been chosen, now initialize the hands/deck
  ShuffleDeck(&deck);

  // initialize variables, draw 5 cards and "pick" username
  for (int i = 0; i < currentPlayers; i++) {
    player[i].hand.numCards = 0;
    Draw(i, HAND_START);
  }

  // just have to trust that pepole don't cheat by examining the code mid-game lol
  for (int i = 1; i < currentPlayers; i++) {
    SendPlayers(clientsockfd[i - 1]);
    SendInt(nursery.size, clientsockfd[i - 1]);
    SendUnicorns(nursery.cards, nursery.size, clientsockfd[i - 1]);
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
  // asynch stuff: http://archive.gamedev.net/archive/reference/articles/article1297.html22

  // loop until win condition occurs (7 unicorns in stable)
  do {
    printf("\n********************************************************************************\n");
    Red();
    printf("\nPlayer Stables\n");
    ResetCol();
    PrintStableGrid();

    Red();
    printf("\n*** %s's turn ***\n\n", player[counter].username);
    ResetCol();

    if (counter == 0) {
      // it's your turn! do your thing :>
      BeginningOfTurn(counter);

      ActionPhase(counter);

      if (EndOfTurn(counter)) {
        ServerSendEndGame(counter);
        // break just to avoid looping in case the function actually returns
        break;
      }

      // signal the next turn cycle and send the deck/player data to everyone else
      for (int i = 0; i < currentPlayers - 1; i++) {
        SendInt(END_TURN, clientsockfd[i]);
        SendGamePacket(clientsockfd[i]);
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

        for (int k = 0; k < currentPlayers - 1; k++) {
          if (pfd[k + 2].revents & POLLIN) {
            ReceiveInt(&networkEvents, clientsockfd[k]);

            // delayed input from timing issues
            if (networkEvents < 0) continue;

            eventloop = netStates[networkEvents].RecvServer(k + 1, clientsockfd[k]);
          }

          Sleep(20);
        }

      } while (!eventloop);
    }

    // print state of nursery and discard piles
#ifdef SHOWNURSERY
      Yellow();
      PrintPile(nursery);
      ResetCol();
#endif
      
#ifdef SHOWDISCARD
      Magenta();
      PrintPile(discardpile);
      ResetCol();
#endif

    counter = (counter + 1) % currentPlayers;
  } while (42);

  // this shouldn't happen
  return 1;
}
