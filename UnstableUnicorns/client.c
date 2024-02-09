#include "client.h"
#include "gamemechanics.h"
#include "gamephase.h"
#include <conio.h>
#include <windows.h>

int clientMain(void) {
  int isvalid, clientpnum, index;
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
    isvalid = 0;
    printf("\nEnter your desired username (max 32 chars): ");
    fgets(buf, sizeof buf, stdin);
    buf[strlen(buf) - 1] = 0;

    if (strlen(buf) > 0) {
      send(sockfd, buf, strlen(buf), 0);
      receiveInt(&isvalid, sockfd);

      if (isvalid == 1) {
        printf("The username '%s' is already taken by another player.\n", buf);
      }
    }
    else {
      isvalid = 1;
    }
  } while (isvalid == 1);

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
      closesocket(sockfd);
      return 2;
    }
    else if (ret == 0) {
      fprintf(stderr, "ERROR: server timed out. Error code : %d", WSAGetLastError());
      closesocket(sockfd);
      return 2;
    }


    if (WaitForSingleObject(GetStdHandle(STD_INPUT_HANDLE), 0) == WAIT_OBJECT_0) {
      processStdin(stdinbuf, &bufindex);
      if (bufindex >= (sizeof(stdinbuf) - 1) || stdinbuf[bufindex - 1] == '\r' || stdinbuf[bufindex - 1] == '\n') {

        sendInt(incoming_msg, sockfd);
        sendInt(clientpnum, sockfd);
        send(sockfd, stdinbuf, sizeof stdinbuf, 0);

        memset(stdinbuf, '\0', sizeof stdinbuf);
        bufindex = 0;
      }
    }

    if (pfd[1].revents & POLLIN) {
      receiveInt(&network_events, sockfd);

      if (network_events == player_join) {
        receivePlayers(sockfd);
        printf("\nCurrent list of players:\n");
        for (int i = 0; i < current_players; i++) {
          printf("  %d: %s\n", i + 1, player[i].username);
        }
      }
      else if (network_events == incoming_msg) {
        receiveMsg(stdinbuf, sizeof stdinbuf, sockfd);
        memset(stdinbuf, '\0', sizeof stdinbuf);
      }
      else if (network_events == start_game) {
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
  int didWin = 0, winningpnum = 0;

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
      beginningOfTurn(counter);

      actionPhase(counter);

      didWin = endOfTurn(counter);
      if (didWin == 1) {
        sendInt(end_game, sockfd);
        sendGamePacket(sockfd);
        winningpnum = counter;
        break;
      }

      // send updates of stuff
      sendInt(end_turn, sockfd);
      sendGamePacket(sockfd);
    }
    else {
      printf("waiting for %s to make a move...\n", player[counter].username);

      for (;;) {
        ret = WSAPoll(pfd, 2, -1);
        if (ret == SOCKET_ERROR) {
          fprintf(stderr, "ERROR: poll() failed. Error code : %d", WSAGetLastError());
          closesocket(sockfd);
          return 2;
        }
        else if (ret == 0) {
          fprintf(stderr, "ERROR: server timed out. Error code : %d", WSAGetLastError());
          closesocket(sockfd);
          return 2;
        }

        if (pfd[1].revents & POLLIN) {
          receiveInt(&network_events, sockfd);

          if (network_events == neigh_event) {
            int orig_pnum, orig_cindex;
            receiveInt(&orig_pnum, sockfd);
            receiveInt(&orig_cindex, sockfd);

            clientNeigh(clientpnum, orig_pnum, &orig_cindex);
          }
          else if (network_events == discard_event) {
            int target_player;
            int desired_class;
            receiveInt(&target_player, sockfd);
            receiveInt(&desired_class, sockfd);
            receivePlayers(sockfd);

            clientDiscard(clientpnum, target_player, desired_class);
          }
          else if (network_events == sacrifice_event) {
            int target_player;
            int desired_class;
            receiveInt(&target_player, sockfd);
            receiveInt(&desired_class, sockfd);

            clientSacrifice(clientpnum, target_player, desired_class);
          }
          else if (network_events == end_turn) {
            receiveGamePacket(sockfd);
            break;
          }
          else if (network_events == end_game) {
            receiveInt(&winningpnum, sockfd);
            receiveGamePacket(sockfd);
            didWin = 1;
            break;
          }
        }

        Sleep(20);
      }
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
  _getch();

	return 0;
}
