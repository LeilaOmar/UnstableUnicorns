#include "server.h"
#include "gamemechanics.h"
#include "gamephase.h"
#include "windowsapp.h"

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
    sprintf_s(errormsg, DESC_SIZE, "Accept failed with error code : %d", WSAGetLastError());
    MessageBoxA(NULL,
      errormsg,
      _T("Client Connection Set-up"),
      MB_ICONERROR | MB_OK);
    return 1;
  }

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
  fgets(player[currentPlayers].username, sizeof player[currentPlayers].username, stdin);
  player[currentPlayers].username[strlen(player[currentPlayers].username) - 1] = 0;
  currentPlayers++; // the host is player one :>

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
  int connectedPlayers = 1;
  int nEvents;
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
        if (strncmp(stdinBuf, "start", 5) == 0 && currentPlayers >= 2 && connectedPlayers == currentPlayers) {
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
            connectedPlayers++;
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
          ReceiveInt(&nEvents, clientsockfd[i]);

          if (nEvents == INCOMING_MSG) {
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

              SendInt(nEvents, clientsockfd[j]);
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
        connectedPlayers--;

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

  if (isLog) LogGameData(-1, START_GAME);

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

      if (isLog) LogGameData(counter, END_TURN);
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
            ReceiveInt(&nEvents, clientsockfd[k]);

            // delayed input from timing issues
            if (nEvents < 0) continue;

            eventloop = netStates[nEvents].RecvServer(k + 1, clientsockfd[k]);
          }

          Sleep(20);
        }

      } while (!eventloop);

      if (isLog) LogGameData(counter, END_TURN);
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

int ServerHost(LPVOID p) {
  // *****************************************************
  // ******************* Server Set-up *******************
  // *****************************************************

  short portno = *((short*)p);
  struct sockaddr_in addr;
  char errormsg[DESC_SIZE];

  // TODO: isClient is kind of superfluous when i could just use the player number to check instead
  isClient = 0;

  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = htons(portno);

  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
    sprintf_s(errormsg, DESC_SIZE, "Could not create socket. Error code : % d", WSAGetLastError());
    MessageBoxA(NULL,
      errormsg,
      _T("Server Set-up"),
      MB_ICONERROR | MB_OK);
    return 1;
  }

  if (bind(sockfd, (struct sockaddr*)&addr, sizeof addr) == SOCKET_ERROR) {
    sprintf_s(errormsg, DESC_SIZE, "Bind failed with error code : %d.", WSAGetLastError());
    MessageBoxA(NULL,
      errormsg,
      _T("Server Set-up"),
      MB_ICONERROR | MB_OK);
    closesocket(sockfd);
    return 1;
  }

  listen(sockfd, MAX_PLAYERS - 1);


  // *****************************************************
  // ***************** UDP Socket Set-up *****************
  // *****************************************************

  // dummy/unbound udp socket used for the purpose of interrupting WSAPoll
  if ((udpfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == INVALID_SOCKET) {
    sprintf_s(errormsg, DESC_SIZE, "Could not create udp socket. Error code : % d", WSAGetLastError());
    MessageBoxA(NULL,
      errormsg,
      _T("Server Set-up"),
      MB_ICONERROR | MB_OK);
    return 1;
  }

  // *****************************************************
  // ******************** Poll Set-up ********************
  // *****************************************************

  // SOCKET clientsockfd[MAX_PLAYERS - 1];

  // server should be in non-blocking mode
  unsigned long on = 1;
  ioctlsocket(sockfd, FIONBIO, &on);

  // read data from the connection
  char buf[BUF_SIZE];
  int count, ret;
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
    ret = WSAPoll(pfd, MAX_PLAYERS + 1, -1);
    if (ret == SOCKET_ERROR) {
      sprintf_s(errormsg, DESC_SIZE, "ERROR: poll() failed. Error code : %d", WSAGetLastError());
      MessageBoxA(NULL,
        errormsg,
        _T("Server Set-up"),
        MB_ICONERROR | MB_OK);
      closesocket(udpfd);
      closesocket(sockfd);
      menuState = TITLEBLANK;
      return 2;
    }
    else if (ret == 0) {
      sprintf_s(errormsg, DESC_SIZE, "ERROR: server timed out. Error code : %d", WSAGetLastError());
      MessageBoxA(NULL,
        errormsg,
        _T("Server Set-up"),
        MB_ICONERROR | MB_OK);
      closesocket(udpfd);
      closesocket(sockfd);
      menuState = TITLEBLANK;
      return 2;
    }

    // incoming connection
    if (pfd[0].revents & POLLIN) {
      // create a new connection given that there aren't already 7 clients
      if (currentPlayers < MAX_PLAYERS) {
        if (NewConnection(clientsockfd) != 0)
        {
          // shouldn't update sockfd stuff if accept() failed
          continue;
        }
        ioctlsocket(clientsockfd[currentPlayers - 1], FIONBIO, &on);
        pfd[currentPlayers + 1].fd = clientsockfd[currentPlayers - 1];
        pfd[currentPlayers + 1].events = POLLIN | POLLOUT;
        currentPlayers++;

        // // TODO: moving this here just delayed the aforementioned client bug (that has since been hopefully fixed)
        // // the 2nd client to join was stalled until an IO event happened
        // 
        // recv(clientsockfd[currentPlayers - 2], player[currentPlayers - 1].username, LINE_MAX, 0);
        // sprintf_s(buf, BUF_SIZE, "\n%d. %s", currentPlayers, player[currentPlayers - 1].username);
        // strncat_s(partyMems, PARTYSTRSIZE, buf, 36); // 36 is LINE_MAX + 4 for the '\n%d. ' part
        // 
        // // send full lobby code to the player that just joined
        // send(clientsockfd[currentPlayers - 2], hexcode, sizeof(hexcode), 0);
        // 
        // // send an update on party size/names/unicorns to every player
        // for (int j = 0; j < currentPlayers - 2; j++) {
        //   SendLobbyPacket(currentPlayers, j + 1, clientsockfd[j]);
        // }
        // memset(buf, 0, BUF_SIZE); // reset buf
      }
    }

    if (pfd[0].revents & (POLLHUP | POLLERR)) {
      sprintf_s(errormsg, DESC_SIZE, "ERROR: Server received POLLHUP or POLLERR signal. Error code : %d", WSAGetLastError());
      MessageBoxA(NULL,
        errormsg,
        _T("Server Set-up"),
        MB_ICONERROR | MB_OK);
      closesocket(udpfd);
      closesocket(sockfd);
      menuState = TITLEBLANK;
      return 2;
    }

    // IO operation came in from a client socket
    for (int i = 0; i < currentPlayers - 1; i++) {
      if (pfd[i + 2].revents & POLLIN) {
        if (player[i + 1].username[0] == '\0') {
          // TODO: move this condition (without the if statement) to the socket POLLIN w/ new connections
          recv(clientsockfd[i], player[i + 1].username, LINE_MAX, 0);
          sprintf_s(buf, BUF_SIZE, "\n%d. %s", i + 2, player[i + 1].username);
          strncat_s(partyMems, PARTYSTRSIZE, buf, LINE_MAX + 4); // +4 for the "\n%d. " part

          // send full lobby code to the player that just joined
          send(clientsockfd[i], hexcode, sizeof(hexcode), 0);

          // send an update on party size/names/unicorns to every player
          for (int j = 0; j < currentPlayers - 1; j++) {
            SendInt(INCOMING_MSG, clientsockfd[j]);
            SendLobbyPacket(currentPlayers, j + 1, clientsockfd[j]);
          }
          memset(buf, 0, BUF_SIZE); // reset buf

          PlaySound(TEXT("Assets\\Audio\\player-join.wav"), NULL, SND_FILENAME | SND_ASYNC);
        }
        else {
          // TODO: (maybe) combine POINT into one recv/send instead of sending two halves
          ReceiveInt(&pnt.x, clientsockfd[i]);
          ReceiveInt(&pnt.y, clientsockfd[i]);
          ret = SelectBabyUnicorn(i + 1, pnt);
          if (ret) {
            for (int j = 0; j < currentPlayers - 1; j++) {
              SendInt(INCOMING_MSG, clientsockfd[j]);
              SendLobbyPacket(currentPlayers, j + 1, clientsockfd[j]);
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
        babyToggle[babyMap[i + 1]] = 0;
        currentPlayers--;

        // shift all data from client[i] (i.e. player[i + 1]) onwards down a step to replace the disconnected client
        for (int j = i; j < currentPlayers - 1; j++) {
          clientsockfd[j] = clientsockfd[j + 1];
          pfd[j + 2] = pfd[j + 3];
          // deselect the unicorn
          pselect[j + 1].left = pselect[j + 2].left;
          pselect[j + 1].top = pselect[j + 2].top;
          babyMap[j + 1] = babyMap[j + 2];
          player[j + 1].stable.unicorns[0] = player[j + 2].stable.unicorns[0];
          strcpy_s(player[j + 1].username, LINE_MAX, player[j + 2].username);
        }

        // reset pselect in case other players join afterwards
        pselect[currentPlayers].left = 0;

        // reset the previous last player index to zero since one person left;
        // currentPlayers has an offset of 1, so the current player count (after being decremented earlier)
        // would be the same as the previous last valid index
        // technically the unicorn in the stable should reset too, but it doesn't matter because it would get
        // overwritten later on, and '0' would mean that it is a baby red unicorn anyways which would be wrong
        pselect[currentPlayers].left = 0;
        pselect[currentPlayers].top = 0;
        memset(player[currentPlayers].username, '\0', LINE_MAX);

        // change lobby username list too...
        memset(partyMems, 0, PARTYSTRSIZE);
        count = 0;
        for (int k = 0; k < currentPlayers; k++) {
          count += sprintf_s(partyMems + count, PARTYSTRSIZE - count, "\n%d. %s", k + 1, player[k].username);
        }

        // send data to remaining clients
        for (int j = 0; j < currentPlayers - 1; j++) {
          SendInt(INCOMING_MSG, clientsockfd[j]);
          SendLobbyPacket(currentPlayers, j + 1, clientsockfd[j]);
        }
      }
    }

    // this should only activate once per action, so toggle the var again at the end
    // theoretically, only one flag should be active at a time; both being active at the same time signifies some serious issues (like the out of range bug)
    if (networkToggle & 1) {
      // clicked the leave button; udpfd was already closed in the main thread
      closesocket(sockfd);
      menuState = TITLEBLANK;
      networkToggle ^= 1;

      // close all client fd's
      for (int i = 0; i < currentPlayers - 1; i++) {
        closesocket(clientsockfd[i]);
      }

      // wipe all relevant lobby global variables and return to title screen
      memset(babyToggle, 0, sizeof babyToggle);
      memset(pselect, 0, sizeof pselect);
      memset(partyMems, '\0', sizeof partyMems);
      for (int i = 0; i < MAX_PLAYERS; i++) {
        memset(player[i].username, '\0', sizeof player[i].username);
      }
      currentPlayers = 1;
      return 1;
    }
    else if (networkToggle & 2) {
      // host changed their baby unicorn and must send the updated list to every client
      for (int i = 0; i < currentPlayers - 1; i++) {
        SendInt(INCOMING_MSG, clientsockfd[i]);
        SendLobbyPacket(currentPlayers, i + 1, clientsockfd[i]);
      }
      networkToggle ^= 2;
    }
    else if (networkToggle & 4) {
      networkToggle ^= 4;

      int isvalid = 1;
      for (int i = 0; i < currentPlayers; i++) {
        if (pselect[i].left == 0) {
          isvalid = 0;
          break;
        }
      }
      if (isvalid) {
        menuState = GAMESTART;
        SetTabs(0);
        SetClientPnum(0);
        break;
      }
    }

    Sleep(20);
  }

  // *****************************************************
  // ******************** Game Set-up ********************
  // *****************************************************

  if (currentPlayers >= 6) WIN_CONDITION = 6;

  for (int i = 0; i < currentPlayers; i++) {
    int tmp = babyMap[i];

    AddStable(i, nursery.cards[tmp]);
    RearrangePile(&nursery, tmp);

    for (int j = 0; j < currentPlayers; j++) {
      if (babyMap[j] > tmp) {
        babyMap[j]--;
      }
    }
  }

  // seed for randomized pulls
  srand((unsigned int)time(NULL));

  // all of the baby unicorns have been chosen, now initialize the hands/deck
  ShuffleDeck(&deck);

  // initialize variables, draw 5 cards and "pick" username
  for (int i = 0; i < currentPlayers; i++) {
    player[i].hand.numCards = 0;
    Draw(i, HAND_START);
  }

  // just have to trust that pepole don't cheat by examining the code mid-game lol
  for (int i = 0; i < currentPlayers - 1; i++) {
    SendInt(START_GAME, clientsockfd[i]);

    SendPlayers(clientsockfd[i]);
    for (int j = 0; j < currentPlayers; j++) {
      SendInt(player[j].icon, clientsockfd[i]);
    }
    SendInt(nursery.size, clientsockfd[i]);
    SendUnicorns(nursery.cards, nursery.size, clientsockfd[i]);
  }

  if (isLog) LogGameData(-1, START_GAME);

  // *****************************************************
  // ******************** Game Start! ********************
  // *****************************************************

  int counter = 0;
  int nEvents;

  // loop until win condition occurs (7 unicorns in stable)
  do {
    // printf("\n*** %s's turn ***\n\n", player[counter].username);
    DisplayMessage("It's the host's turn!!");

    SetCurrPnum(counter);
    networkToggle = 0; // reset this to avoid accidentally queueing up input outside of the player's turn
    totalTurns++;

    if (counter == 0) {
      // it's your turn! do your thing :>
      BeginningOfTurn(counter);

      // TODO: update UI and hand/stable count stuff after each action?

      while (ActionPhase(counter) == -1) {
        Sleep(20);
      }

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

      if (isLog) LogGameData(counter, END_TURN);
    }
    else {
      // printf("waiting for %s to make a move...\n", player[counter].username);
      int eventloop = 0;

      do {
        ret = WSAPoll(pfd, MAX_PLAYERS + 1, -1);
        if (ret == SOCKET_ERROR) {
          sprintf_s(errormsg, DESC_SIZE, "ERROR: server timed out. Error code : %d", WSAGetLastError());
          MessageBoxA(NULL,
            errormsg,
            _T("Server Set-up"),
            MB_ICONERROR | MB_OK);
          closesocket(sockfd);
          menuState = TITLEBLANK;
          return 2;
        }
        else if (ret == 0) {
          sprintf_s(errormsg, DESC_SIZE, "ERROR: server timed out. Error code : %d", WSAGetLastError());
          MessageBoxA(NULL,
            errormsg,
            _T("Server Set-up"),
            MB_ICONERROR | MB_OK);
          closesocket(sockfd);
          menuState = TITLEBLANK;
          return 2;
        }

        for (int k = 0; k < currentPlayers - 1; k++) {
          if (pfd[k + 2].revents & POLLIN) {
            ReceiveInt(&nEvents, clientsockfd[k]);

            // delayed input from timing issues
            if (nEvents < 0) continue;

            eventloop = netStates[nEvents].RecvServer(k + 1, clientsockfd[k]);
          }

          Sleep(20);
        }

      } while (!eventloop);

      if (isLog) LogGameData(counter, END_TURN);
    }

    counter = (counter + 1) % currentPlayers;
  } while (42);

  // this shouldn't happen
  return 1;
}
