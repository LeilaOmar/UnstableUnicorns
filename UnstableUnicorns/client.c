#include "client.h"
#include "gamemechanics.h"
#include "gamephase.h"
#include <windows.h>

int clientMain(void) {
  int isvalid = 0, clientpnum, index;
  short portno;
  char *end, buf[BUF_SIZE];
  struct sockaddr_in server;

  // *****************************************************
  // ******************* Client Set-up *******************
  // *****************************************************

  for (;;) {
    do {
      printf("Enter a valid port number to connect to (between 1024 and 65535 inclusive): ");
      portno = numinput(buf, &end, sizeof buf);
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
      receiveInt(&isvalid, sockfd);

      if (!isvalid) {
        printf("The username '%s' is already taken by another player.\n", buf);
      }
    }
    else {
      isvalid = 0;
    }
  } while (!isvalid);

  receiveInt(&clientpnum, sockfd);
  isclient = 1;

  printf(
    "\n********************************************************************************"
    "\n********************************* Waiting Room *********************************"
    "\n********************************************************************************"
  );

  red();
  printf("\n\nYou have successfully joined the lobby!"
    "\nType and enter any messages you wish to send to your party.");
  reset_col();

  printf("\033[1;31m%s\033[0m: ", buf);

  // *****************************************************
  // ******************** Game Set-up ********************
  // *****************************************************

  char stdinbuf[MSGBUF] = { 0 };
  int bufindex = 0;

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
      processStdin(stdinbuf, &bufindex);
      if (bufindex >= (sizeof(stdinbuf) - 1) || stdinbuf[bufindex - 1] == '\r' || stdinbuf[bufindex - 1] == '\n') {

        sendInt(incoming_msg, sockfd);
        sendInt(clientpnum, sockfd);
        sendMsg(stdinbuf, sizeof stdinbuf, sockfd);

        memset(stdinbuf, '\0', sizeof stdinbuf);
        bufindex = 0;

        printf("\033[1;31m%s\033[0m: ", player[clientpnum].username);
      }
    }

    if (pfd[1].revents & POLLIN) {
      receiveInt(&network_events, sockfd);

      if (network_events == player_join) {
        receivePlayers(sockfd);

        // erase the player's name and incomplete message; the extra 2 comes from the colon and space
        for (int i = 0; i < strlen(player[clientpnum].username) + bufindex + 2; i++) {
          printf("\b \b");
        }

        printf("\nCurrent list of players:\n");
        for (int i = 0; i < current_players; i++) {
          printf("  %d: %s\n", i + 1, player[i].username);
        }

        // rewrite the typed message
        printf("\033[1;31m%s\033[0m: %s", player[clientpnum].username, stdinbuf);
      }
      else if (network_events == player_disconnect) {
        int tmp;
        receiveInt(&tmp, sockfd);

        // erase the player's name and incomplete message; the extra 2 comes from the colon and space
        for (int i = 0; i < strlen(player[clientpnum].username) + bufindex + 2; i++) {
          printf("\b \b");
        }

        printf("\nPlayer ");
        red();
        printf("%s", player[tmp].username);
        reset_col();
        printf(" disconnected :(\n");

        receivePlayers(sockfd);
        printf("\nCurrent list of players:\n");
        for (int i = 0; i < current_players; i++) {
          printf("  %d: %s\n", i + 1, player[i].username);
        }

        // update clientpnum
        if (tmp < clientpnum)
          clientpnum--;

        // rewrite the typed message
        printf("\033[1;31m%s\033[0m: %s", player[clientpnum].username, stdinbuf);
      }
      else if (network_events == incoming_msg) {
        int pnum;
        char recvbuf[MSGBUF] = { 0 };

        receiveInt(&pnum, sockfd);
        receiveMsg(recvbuf, sockfd);

        // erase the player's name and incomplete message; the extra 2 comes from the colon and space
        for (int i = 0; i < strlen(player[clientpnum].username) + bufindex + 2; i++) {
          printf("\b \b");
        }

        red();
        printf("%s: ", player[pnum].username);
        reset_col();
        printf("%s\n", recvbuf);

        // rewrite the typed message
        printf("\033[1;31m%s\033[0m: %s", player[clientpnum].username, stdinbuf);
      }
      else if (network_events == start_game) {
        // erase the player's name and incomplete message; the extra 2 comes from the colon and space
        for (int i = 0; i < strlen(player[clientpnum].username) + bufindex + 2; i++) {
          printf("\b \b");
        }
        break;
      }
    }

    Sleep(20);
  }

  rainbow(
    "\n*********************************"
    "\n*** Choose your baby unicorn! ***"
    "\n*********************************\n\n");

  red();
  puts("Waiting for other players to pick their Baby Unicorn...");
  reset_col();

  receiveInt(&current_players, sockfd);
  receivePlayers(sockfd);
  receiveInt(&nursery.size, sockfd);
  receiveUnicorns(nursery.cards, nursery.size, sockfd);

  // pick your chosen baby unicorn :D
  printPile(nursery);
  do {
    red();
    printf("%s: ", player[clientpnum].username);
    reset_col();
    printf("Pick the index associated with your desired Baby Unicorn: ");
    index = numinput(buf, &end, sizeof buf) - 1;
  } while (index < 0 || index >= nursery.size || end != (buf + strlen(buf)));

  sendInt(index, sockfd);

  if (clientpnum < current_players - 1) {
    red();
    puts("Waiting for other players to pick their Baby Unicorn...");
    reset_col();
  }

  // receive final list of players/nursery
  receivePlayers(sockfd);
  receiveInt(&nursery.size, sockfd);
  receiveUnicorns(nursery.cards, nursery.size, sockfd);

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
    red();
    printf("\nPlayer Stables\n");
    reset_col();
    printStableGrid();

    red();
    printf("\n*** %s's turn ***\n\n", player[counter].username);
    reset_col();

    if (counter == clientpnum) {
      // it's your turn! do your thing :>
      beginningOfTurn(clientpnum);

      actionPhase(clientpnum);

      if (endOfTurn(clientpnum)) {
        clientSendEndGame(clientpnum, sockfd);
        // break just to avoid looping in case the function actually returns
        break;
      }

      // send updates of stuff
      sendInt(end_turn, sockfd);
      sendGamePacket(sockfd);
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
          receiveInt(&network_events, sockfd);

          eventloop = netStates[network_events].recvClient(clientpnum, sockfd);
        }

        Sleep(20);
      } while (!eventloop);
    }

    // print state of nursery and discard piles
    #ifdef SHOWNURSERY
      yellow();
      printPile(nursery);
      reset_col();
    #endif
      
    #ifdef SHOWDISCARD
      magenta();
      printPile(discardpile);
      reset_col();
    #endif

    counter = (counter + 1) % current_players;
  } while (42);

  // this shouldn't happen
	return 1;
}
