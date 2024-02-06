#include "server.h"
#include "gamemechanics.h"
#include "gamephase.h"
#include "windowsapp.h"
#include <conio.h>

int newConnection(SOCKET *cfd) {
  struct sockaddr_in client_addr;
  socklen_t client_addr_size = sizeof(client_addr);

  if ((cfd[current_players - 1] = accept(sockfd, (struct sockaddr*)&client_addr, &client_addr_size)) == INVALID_SOCKET) {
    sprintf_s(errormsg, ERRORBUF, "Accept failed with error code : %d", WSAGetLastError());
    MessageBoxA(NULL,
      errormsg,
      _T("Client Connection Set-up"),
      NULL);
    return 1;
  }

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
            receiveMsg(stdinbuf, clientsockfd[i]);

            // the pnum in receiveMsg is the same as the client socket number (i.e. clientsockfd[i])
            for (int j = 0; j < current_players - 1; j++) {
              if (j == i) continue;
              sendInt(network_events, clientsockfd[j]);
              sendInt(i + 1, clientsockfd[j]);
              send(clientsockfd[j], stdinbuf, MSGBUF, 0);
            }

            memset(stdinbuf, '\0', MSGBUF);
          }
        }
      }

      if (pfd[i + 2].revents & (POLLHUP | POLLERR)) {
        puts("ERROR: Server received POLLHUP or POLLERR signal");
        printf("Player %s disconnected :(\n", player[i + 1].username);

        for (int j = i; j < current_players - 2; j++) {
          clientsockfd[j] = clientsockfd[j + 1];
        }
        current_players--;

        closesocket(clientsockfd[i]);
      }
    }
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
          closesocket(sockfd);
          return 2;
        }
        else if (ret == 0) {
          fprintf(stderr, "ERROR: server timed out. Error code : %d", WSAGetLastError());
          closesocket(sockfd);
          return 2;
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
            } // if network_events == neigh_event
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
              winningpnum = counter;

              for (int i = 0; i < current_players - 1; i++) {
                if (k == i) continue;

                sendInt(end_game, clientsockfd[i]);
                sendInt(k + 1, clientsockfd[i]);
                sendGamePacket(clientsockfd[i]);
              }
              break;
            }
          }
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
  _getch();

  return 0;
}

int serverInit(short portno) {
  // *****************************************************
  // ******************* Server Set-up *******************
  // *****************************************************

  struct sockaddr_in addr;
  char errormsg[ERRORBUF];

  // TODO: isclient is kind of superfluous when i could just use the player number to check instead
  isclient = 0;

  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = htons(portno);

  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
    sprintf_s(errormsg, ERRORBUF, "Could not create socket. Error code : % d", WSAGetLastError());
    MessageBoxA(NULL,
      errormsg,
      _T("Server Set-up"),
      NULL);
    return 1;
  }

  if (bind(sockfd, (struct sockaddr*)&addr, sizeof addr) == SOCKET_ERROR) {
    sprintf_s(errormsg, ERRORBUF, "Bind failed with error code : %d.", WSAGetLastError());
    MessageBoxA(NULL,
      errormsg,
      _T("Server Set-up"),
      NULL);
    closesocket(sockfd);
    return 1;
  }

  listen(sockfd, MAX_PLAYERS - 1);


  // *****************************************************
  // ***************** UDP Socket Set-up *****************
  // *****************************************************

  // dummy/unbound udp socket used for the purpose of interrupting WSAPoll
  if ((udpfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == INVALID_SOCKET) {
    sprintf_s(errormsg, ERRORBUF, "Could not create udp socket. Error code : % d", WSAGetLastError());
    MessageBoxA(NULL,
      errormsg,
      _T("Server Set-up"),
      NULL);
    return 1;
  }

  // *****************************************************
  // ******************** Poll Set-up ********************
  // *****************************************************

  SOCKET clientsockfd[MAX_PLAYERS - 1];

  // server should be in non-blocking mode
  unsigned long on = 1;
  ioctlsocket(sockfd, FIONBIO, &on);

  // read data from the connection
  char buf[BUF_SIZE];
  int count, ret, isvalid;
  POINT pnt;

  // count for the number of times server has been officially updated; this is for cases where
  // two clients may send data at (near) the same time before getting updated about the other one
  // that was just slightly slower. if the version is not the same then the server must either
  // merge data or force the client to re-enter/send the data
  int versioncheck = 0;
  WSAPOLLFD pfd[MAX_PLAYERS + 1] = { -1 };  // original sockfd + udp socket fd + 7 additional players
  pfd[0].fd = sockfd;
  pfd[0].events = POLLIN | POLLOUT;
  pfd[1].fd = udpfd;

  for (;;) {
    // timeout after 150 seconds
    ret = WSAPoll(pfd, MAX_PLAYERS, -1);
    if (ret == SOCKET_ERROR) {
      sprintf_s(errormsg, ERRORBUF, "ERROR: poll() failed. Error code : %d", WSAGetLastError());
      MessageBoxA(NULL,
        errormsg,
        _T("Server Set-up"),
        NULL);
      closesocket(udpfd);
      closesocket(sockfd);
      menustate = TITLEBLANK;
      return 2;
    }
    else if (ret == 0) {
      sprintf_s(errormsg, ERRORBUF, "ERROR: server timed out. Error code : %d", WSAGetLastError());
      MessageBoxA(NULL,
        errormsg,
        _T("Server Set-up"),
        NULL);
      closesocket(udpfd);
      closesocket(sockfd);
      menustate = TITLEBLANK;
      return 2;
    }

    // incoming connection
    if (pfd[0].revents & POLLIN) {
      // create a new connection given that there aren't already 7 clients
      if (current_players < MAX_PLAYERS) {
        if (newConnection(clientsockfd) != 0)
        {
          // shouldn't update sockfd stuff if accept() failed
          continue;
        }
        ioctlsocket(clientsockfd[current_players - 1], FIONBIO, &on);
        pfd[current_players + 1].fd = clientsockfd[current_players - 1];
        pfd[current_players + 1].events = POLLIN | POLLOUT;
        current_players++;

        // // TODO: moving this here just delayed the aforementioned client bug (that has since been hopefully fixed)
        // // the 2nd client to join was stalled until an IO event happened
        // 
        // recv(clientsockfd[current_players - 2], player[current_players - 1].username, NAME_SIZE, 0);
        // sprintf_s(buf, BUF_SIZE, "\n%d. %s", current_players, player[current_players - 1].username);
        // strncat_s(partymems, PARTYSTRSIZE, buf, 36); // 36 is NAME_SIZE + 4 for the '\n%d. ' part
        // 
        // // send full lobby code to the player that just joined
        // send(clientsockfd[current_players - 2], hexcode, sizeof(hexcode), 0);
        // 
        // // send an update on party size/names/unicorns to every player
        // for (int j = 0; j < current_players - 2; j++) {
        //   sendLobbyPacket(current_players, j + 1, clientsockfd[j]);
        // }
        // memset(buf, 0, BUF_SIZE); // reset buf
      }
    }

    if (pfd[0].revents & (POLLHUP | POLLERR)) {
      sprintf_s(errormsg, ERRORBUF, "ERROR: Server received POLLHUP or POLLERR signal. Error code : %d", WSAGetLastError());
      MessageBoxA(NULL,
        errormsg,
        _T("Server Set-up"),
        NULL);
      closesocket(udpfd);
      closesocket(sockfd);
      menustate = TITLEBLANK;
      return 2;
    }

    // IO operation came in from a client socket
    for (int i = 0; i < current_players - 1; i++) {
      if (pfd[i + 2].revents & POLLIN) {
        if (player[i + 1].username[0] == '\0') {
          // TODO: move this condition (without the if statement) to the socket POLLIN w/ new connections
          recv(clientsockfd[i], player[i + 1].username, NAME_SIZE, 0);
          sprintf_s(buf, BUF_SIZE, "\n%d. %s", i + 2, player[i + 1].username);
          strncat_s(partymems, PARTYSTRSIZE, buf, 36); // 36 is NAME_SIZE + 4 for the '\n%d. ' part

          // send full lobby code to the player that just joined
          send(clientsockfd[i], hexcode, sizeof(hexcode), 0);

          // send an update on party size/names/unicorns to every player
          for (int j = 0; j < current_players - 1; j++) {
            sendLobbyPacket(current_players, j + 1, clientsockfd[j]);
          }
          memset(buf, 0, BUF_SIZE); // reset buf
        }
        else {
          // TODO: (maybe) combine POINT into one recv/send instead of sending two halves
          receiveInt(&pnt.x, clientsockfd[i]);
          receiveInt(&pnt.y, clientsockfd[i]);
          ret = SelectBabyUnicorn(i + 1, pnt);
          if (ret) {
            for (int j = 0; j < current_players - 1; j++) {
              sendLobbyPacket(current_players, j + 1, clientsockfd[j]);
            }
          }
        }
      }

      // client player disconnected; update existing lobby to boot player
      if (pfd[i + 2].revents & (POLLHUP | POLLERR)) {
        // close/reset client-specific data; the baby unicorn toggle is just a simple 1 or 0 depending
        // on whether or not the specific unicorn was selected, so it shouldn't be shifted over and once
        // this player leaves, it should go back to 0
        closesocket(clientsockfd[i]);
        babytoggle[player[i + 1].stable.unicorns[0]] = 0;
        current_players--;

        // shift all data from client[i] (i.e. player[i + 1]) onwards down a step to replace the disconnected client
        for (int j = i; j < current_players - 1; j++) {
          clientsockfd[j] = clientsockfd[j + 1];
          pfd[j + 2] = pfd[j + 3];
          // deselect the unicorn
          pselect[j + 1].left = pselect[j + 2].left;
          pselect[j + 1].top = pselect[j + 2].top;
          player[j + 1].stable.unicorns[0] = player[j + 2].stable.unicorns[0];
          strcpy_s(player[j + 1].username, NAME_SIZE, player[j + 2].username);
        }

        // reset the previous last player index to zero since one person left;
        // current_players has an offset of 1, so the current player count (after being decremented earlier)
        // would be the same as the previous last valid index
        // technically the unicorn in the stable should reset too, but it doesn't matter because it would get
        // overwritten later on, and '0' would mean that it is a baby red unicorn anyways which would be wrong
        pselect[current_players].left = 0;
        pselect[current_players].top = 0;
        memset(player[current_players].username, '\0', NAME_SIZE);

        // change lobby username list too...
        memset(partymems, 0, PARTYSTRSIZE);
        count = 0;
        for (int k = 0; k < current_players; k++) {
          count += sprintf_s(partymems + count, PARTYSTRSIZE - count, "\n%d. %s", k + 1, player[k].username);
        }

        // send data to remaining clients
        for (int j = 0; j < current_players - 1; j++) {
          sendLobbyPacket(current_players, j + 1, clientsockfd[j]);
        }
      }
    }

    // this should only activate once per action, so toggle the var again at the end
    // theoretically, only one flag should be active at a time; both being active at the same time signifies some serious issues (like the out of range bug)
    if (networktoggle & 1) {
      // clicked the leave button; udpfd was already closed in the main thread
      closesocket(sockfd);
      menustate = TITLEBLANK;
      networktoggle ^= 1;

      // close all client fd's
      for (int i = 0; i < current_players - 1; i++) {
        closesocket(clientsockfd[i]);
      }

      // wipe all relevant lobby global variables and return to title screen
      memset(babytoggle, 0, sizeof babytoggle);
      memset(pselect, 0, sizeof pselect);
      memset(partymems, '\0', sizeof partymems);
      for (int i = 0; i < MAX_PLAYERS; i++) {
        memset(player[i].username, '\0', sizeof player[i].username);
      }
      current_players = 1;
      return 1;
    }
    else if (networktoggle & 2) {
      // host changed their baby unicorn and must send the updated list to every client
      for (int i = 0; i < current_players - 1; i++) {
        sendLobbyPacket(current_players, i + 1, clientsockfd[i]);
      }
      networktoggle ^= 2;
    }

    Sleep(20);
  }

  if (current_players >= 6) WIN_CONDITION = 6;
}
