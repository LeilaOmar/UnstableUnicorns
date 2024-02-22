#include "client.h"
#include "gamemechanics.h"
#include "gamephase.h"

/**
 * @brief Connects to the server and verifies a valid username before fully connecting to the lobby
 * @return clientpnum The user/client's personal player number for the game
 */
static int ClientConnect(void) {
  int isvalid = 0;
  int clientpnum;
  short portno;
  char *end, buf[LINE_MAX];
  struct sockaddr_in server;

  for (;;) {
    do {
      printf("Enter a valid port number to connect to (between 1024 and 65535 inclusive): ");
      portno = NumInput(buf, &end, sizeof buf);
    } while (portno < 1024 || portno > 65535 || end != (buf + strlen(buf)));
    do {
      printf("Enter a valid IPv4 Address to join: ");
      fgets(buf, sizeof buf, stdin);
      buf[strlen(buf) - 1] = 0;
    } while (inet_pton(AF_INET, buf, &server.sin_addr.s_addr) != 1);

    server.sin_family = AF_INET;
    server.sin_port = htons(portno);

    if (connect(sockfd, (struct sockaddr*)&server, sizeof(server)) < 0) {
      fprintf(stderr, "Attempt to connect to the server was unsuccessful. Error code : %d\n", WSAGetLastError());
    }
    else {
      printf("Connection attempt successful! Yay\n");
      break;
    }
  }

  do {
    printf("\nEnter your desired username (max 32 chars): ");
    fgets(buf, sizeof buf, stdin);
    buf[strlen(buf) - 1] = 0;

    if (strlen(buf) > 0) {
      send(sockfd, buf, strlen(buf), 0);
      ReceiveInt(&isvalid, sockfd);

      if (!isvalid) {
        printf("The username '%s' is already taken by another player.\n", buf);
      }
    }
    else {
      isvalid = 0;
    }
  } while (!isvalid);

  ReceiveInt(&clientpnum, sockfd);
  isClient = 1;

  printf(
    "\n********************************************************************************"
    "\n********************************* Waiting Room *********************************"
    "\n********************************************************************************"
  );

  Red();
  printf("\n\nYou have successfully joined the lobby!"
    "\nType and enter any messages you wish to send to your party.");
  ResetCol();

  printf("\033[1;31m%s\033[0m: ", buf);

  return clientpnum;
}

int ClientMain(void) {
  int isvalid = 0, index;
  int clientpnum;

  clientpnum = ClientConnect();

  // *****************************************************
  // ******************** Game Set-up ********************
  // *****************************************************

  int nEvents;
  int bufIndex = 0;
  char stdinBuf[MSGBUF] = { 0 };

  int ret;
  WSAPOLLFD pfd[2] = { -1 };  // stdin + original sockfd
  pfd[0].fd = _fileno(stdin);
  pfd[0].events = POLLIN | POLLOUT;
  pfd[1].fd = sockfd;
  pfd[1].events = POLLIN | POLLOUT;

  for (;;) {
    ret = WSAPoll(pfd, 2, -1);
    if (ret == SOCKET_ERROR) {
      fprintf(stderr, "ERROR: poll() failed. Error code : %d", WSAGetLastError());
      exit(2);
    }
    else if (ret == 0) {
      fprintf(stderr, "ERROR: server timed out. Error code : %d", WSAGetLastError());
      exit(2);
    }

    if (WaitForSingleObject(GetStdHandle(STD_INPUT_HANDLE), 0) == WAIT_OBJECT_0) {
      ProcessStdin(stdinBuf, &bufIndex);
      if (bufIndex >= (sizeof(stdinBuf) - 1) || stdinBuf[bufIndex - 1] == '\r' || stdinBuf[bufIndex - 1] == '\n') {

        SendInt(INCOMING_MSG, sockfd);
        SendInt(clientpnum, sockfd);
        SendMsg(stdinBuf, sizeof stdinBuf, sockfd);

        memset(stdinBuf, '\0', sizeof stdinBuf);
        bufIndex = 0;

        printf("\033[1;31m%s\033[0m: ", player[clientpnum].username);
      }
    }

    if (pfd[1].revents & POLLIN) {
      ReceiveInt(&nEvents, sockfd);

      if (nEvents == PLAYER_JOIN) {
        ReceivePlayers(sockfd);

        // erase the player's name and incomplete message; the extra 2 comes from the colon and space
        for (int i = 0; i < (int)strlen(player[clientpnum].username) + bufIndex + 2; i++) {
          printf("\b \b");
        }

        printf("\nCurrent list of players:\n");
        for (int i = 0; i < currentPlayers; i++) {
          printf("  %d: %s\n", i + 1, player[i].username);
        }

        // rewrite the typed message
        printf("\033[1;31m%s\033[0m: %s", player[clientpnum].username, stdinBuf);
      }
      else if (nEvents == PLAYER_DISCONNECT) {
        int tmp;
        ReceiveInt(&tmp, sockfd);

        // erase the player's name and incomplete message; the extra 2 comes from the colon and space
        for (int i = 0; i < (int)strlen(player[clientpnum].username) + bufIndex + 2; i++) {
          printf("\b \b");
        }

        printf("\nPlayer ");
        Red();
        printf("%s", player[tmp].username);
        ResetCol();
        printf(" disconnected :(\n");

        ReceivePlayers(sockfd);
        printf("\nCurrent list of players:\n");
        for (int i = 0; i < currentPlayers; i++) {
          printf("  %d: %s\n", i + 1, player[i].username);
        }

        // update clientpnum
        if (tmp < clientpnum)
          clientpnum--;

        // rewrite the typed message
        printf("\033[1;31m%s\033[0m: %s", player[clientpnum].username, stdinBuf);
      }
      else if (nEvents == INCOMING_MSG) {
        int pnum;
        char recvbuf[MSGBUF] = { 0 };

        ReceiveInt(&pnum, sockfd);
        ReceiveMsg(recvbuf, sockfd);

        // erase the player's name and incomplete message; the extra 2 comes from the colon and space
        for (int i = 0; i < (int)strlen(player[clientpnum].username) + bufIndex + 2; i++) {
          printf("\b \b");
        }

        Red();
        printf("%s: ", player[pnum].username);
        ResetCol();
        printf("%s\n", recvbuf);

        // rewrite the typed message
        printf("\033[1;31m%s\033[0m: %s", player[clientpnum].username, stdinBuf);
      }
      else if (nEvents == START_GAME) {
        // erase the player's name and incomplete message; the extra 2 comes from the colon and space
        for (int i = 0; i < (int)strlen(player[clientpnum].username) + bufIndex + 2; i++) {
          printf("\b \b");
        }
        break;
      }
    }

    Sleep(20);
  }

  Rainbow(
    "\n*********************************"
    "\n*** Choose your baby unicorn! ***"
    "\n*********************************\n\n");

  Red();
  puts("Waiting for other players to pick their Baby Unicorn...");
  ResetCol();

  ReceiveInt(&currentPlayers, sockfd);
  ReceivePlayers(sockfd);
  ReceiveInt(&nursery.size, sockfd);
  ReceiveUnicorns(nursery.cards, nursery.size, sockfd);

  // pick your chosen baby unicorn :D
  char *end, buf[BUF_SIZE];

  PrintPile(nursery);
  do {
    Red();
    printf("%s: ", player[clientpnum].username);
    ResetCol();
    printf("Pick the index associated with your desired Baby Unicorn: ");
    index = NumInput(buf, &end, sizeof buf) - 1;
  } while (index < 0 || index >= nursery.size || end != (buf + strlen(buf)));

  SendInt(index, sockfd);

  if (clientpnum < currentPlayers - 1) {
    Red();
    puts("Waiting for other players to pick their Baby Unicorn...");
    ResetCol();
  }

  // receive final list of players/nursery
  ReceivePlayers(sockfd);
  ReceiveInt(&nursery.size, sockfd);
  ReceiveUnicorns(nursery.cards, nursery.size, sockfd);

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

    if (counter == clientpnum) {
      // it's your turn! do your thing :>
      BeginningOfTurn(clientpnum);

      ActionPhase(clientpnum);

      if (EndOfTurn(clientpnum)) {
        ClientSendEndGame(clientpnum, sockfd);
        // break just to avoid looping in case the function actually returns
        break;
      }

      // send updates of stuff
      SendInt(END_TURN, sockfd);
      SendGamePacket(sockfd);
    }
    else {
      printf("waiting for %s to make a move...\n", player[counter].username);
      int eventloop = 0;

      do {
        ret = WSAPoll(pfd, 2, -1);
        if (ret == SOCKET_ERROR) {
          fprintf(stderr, "ERROR: poll() failed. Error code : %d", WSAGetLastError());
          exit(2);
        }
        else if (ret == 0) {
          fprintf(stderr, "ERROR: server timed out. Error code : %d", WSAGetLastError());
          exit(2);
        }

        if (pfd[1].revents & POLLIN) {
          ReceiveInt(&nEvents, sockfd);

          eventloop = netStates[nEvents].RecvClient(clientpnum, sockfd);
        }

        Sleep(20);
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
