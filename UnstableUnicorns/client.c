#include "client.h"
#include "gamemechanics.h"
#include "windowsapp.h"

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
      fgets(buf, sizeof buf, stdin);
      buf[strlen(buf) - 1] = 0;
      portno = strtol(buf, &end, 10);
    } while (portno < 1024 || portno > 65535 || end != (buf + strlen(buf)));
    do {
      printf("Enter a valid IPv4 Address to join: ");
      fgets(buf, sizeof buf, stdin);
      buf[strlen(buf) - 1] = 0;
    } while (inet_pton(AF_INET, buf, &server.sin_addr.s_addr) != 1);

    server.sin_family = AF_INET;
    server.sin_port = htons(portno);

    if (connect(sockfd, (struct sockaddr*)&server, sizeof(server)) < 0) {
      printf("Attempt to connect to the server was unsuccessful. Error code : %d\n", WSAGetLastError());
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

  red();
  puts("You have successfully joined the party!"
    "\nPlease wait until the host starts the game...");
  reset_col();

  // *****************************************************
  // ******************** Game Set-up ********************
  // *****************************************************

  int count, ret, rc;
  WSAPOLLFD pfd[2] = { -1 };  // stdin + original sockfd
  pfd[0].fd = _fileno(stdin);
  pfd[0].events = POLLIN | POLLOUT;
  pfd[1].fd = sockfd;
  pfd[1].events = POLLIN | POLLOUT;

  // TODO: serialize this for later; also check the amount of bytes received!
  receiveInt(&current_players, sockfd);
  receiveInt(&clientpnum, sockfd);
  recv(sockfd, (void*)&player[clientpnum], sizeof(struct Player), 0);
  receiveInt(&nursery_index, sockfd);
  receiveInt(&dnurse_size, sockfd);
  recv(sockfd, (void*)nursery_ref, sizeof(nursery_ref), 0);

  rainbow("\n*******************"
          "\n*** Game Start! ***"
          "\n*******************\n\n");

  // pick your chosen baby unicorn :D
  printNursery(nursery_index, NURSERY_SIZE);
  do {
    red();
    printf("%s: ", player[clientpnum].username);
    reset_col();
    printf("Pick the index associated with your desired Baby Unicorn: ");
    fgets(buf, sizeof buf, stdin);
    buf[strlen(buf) - 1] = 0;
    index = strtol(buf, &end, 10) - 1;
  } while (index < 0 || index >= (int)dnurse_size || end != (buf + strlen(buf)));

  sendInt(index, sockfd);

  if (clientpnum < current_players - 1) {
    red();
    puts("Waiting for other players to pick their Baby Unicorn...");
    reset_col();
  }

  rc = recv(sockfd, (void*)player, sizeof(player), 0);
  if (rc != sizeof(player)) {
    puts("data wasn't fully received for the full player structure");
  }

  // *****************************************************
  // ******************** Game Start! ********************
  // *****************************************************

  puts("\n**********\n");
  for (int i = 0; i < current_players; i++) {
    printHand(i);
    printStable(i);
    puts("\n**********\n");
  }

  int counter = 0, isevent = 0;

  // loop until win condition occurs (7 unicorns in stable)
  for (;;) {
    red();
    printf("\n*** %s's turn ***\n\n", player[counter].username);
    reset_col();

    // it's your turn! do your thing :>
    if (counter == clientpnum) {
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
        ((player[counter].stable.num_unicorns >= WIN_CONDITION - 1) &&
          (player[counter].flags & ginormous_unicorn) != 0 &&
          (player[counter].flags & blinding_light) == 0)) &&
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

      // send updates of stuff
    }
    else {
      printf("waiting for %s to make a move...\n", player[counter].username);

      // checks to see if it's possible to neigh the card
      // receiveInt(&isevent, sockfd);
      // 
      // // timeout after 5 seconds
      // ret = WSAPoll(pfd, MAX_PLAYERS + 1, 5000);
      // if (pfd[1].revents & POLLOUT) {
      // 
      // }
    }
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

int clientJoin(short portno) {
  struct sockaddr_in server;
  int clientpnum, versioncheck;

  // *****************************************************
  // ******************* Client Set-up *******************
  // *****************************************************

  if (inet_pton(AF_INET, ip, &server.sin_addr.s_addr) != 1) {
    // oopsie
    return 1;
  }
  
  server.sin_family = AF_INET;
  server.sin_port = htons(portno);
  
  int tmp;
  if (tmp = connect(sockfd, (struct sockaddr*)&server, sizeof(server)) < 0) {
    versioncheck = WSAGetLastError();
    MessageBoxA(NULL,
      _T("Attempt to connect to the server was unsuccessful. Error code : %d"),
      _T("Client Connection Set-up"),
      NULL);
    menustate = TITLEBLANK;
    return 1;
  }

  //struct addrinfo* result = NULL, hints;
  //ZeroMemory(&hints, sizeof(hints));
  //hints.ai_family = AF_INET;
  //hints.ai_socktype = SOCK_STREAM;
  //hints.ai_protocol = IPPROTO_TCP;
  //
  //// Resolve the server address and port
  //result = getaddrinfo(ip, htons(portno), &hints, &result);
  //if (result != 0)
  //{
  //  //printf("getaddrinfo failed with error: %d\n", result);
  //  WSACleanup();
  //  return 1;
  //}
  //
  //// Attempt to connect to an address until one succeeds
  //for (struct addrinfo *ptr = result; ptr != NULL; ptr = ptr->ai_next)
  //{
  //
  //  // Create a SOCKET for connecting to server
  //  sockfd = socket(ptr->ai_family, ptr->ai_socktype,
  //    ptr->ai_protocol);
  //  if (sockfd == INVALID_SOCKET)
  //  {
  //    //printf("socket failed with error: %ld\n", WSAGetLastError());
  //    WSACleanup();
  //    return 1;
  //  }
  //
  //  // Connect to server.
  //  result = connect(sockfd, ptr->ai_addr, (int)ptr->ai_addrlen);
  //  if (result == SOCKET_ERROR)
  //  {
  //    closesocket(sockfd);
  //    sockfd = INVALID_SOCKET;
  //    continue;
  //  }
  //  break;
  //}
  //
  //freeaddrinfo(result);

  // success! ^.^
  // using player[0] just for the start; will be overwritten later
  send(sockfd, player[0].username, strlen(player[0].username), 0);
  receiveLobbyPacket(&current_players, &clientpnum, sockfd);

  // ***** test *****

  int ret, isvalid = 0;
  char buf[BUF_SIZE];
  POINT pnt;

  // client should be in non-blocking mode
  unsigned long on = 1;
  ioctlsocket(sockfd, FIONBIO, &on);

  WSAPOLLFD pfd = { -1 };
  pfd.fd = sockfd;
  pfd.events = POLLIN | POLLOUT;

  for (;;) {
    // timeout after 150 seconds
    ret = WSAPoll(&pfd, 1, -1);
    if (ret == SOCKET_ERROR) {
      MessageBoxA(NULL,
        _T("ERROR: poll() failed. Error code : %d", WSAGetLastError()),
        _T("Client Connection"),
        NULL);
      closesocket(sockfd);
      WSACleanup();
      menustate = TITLEBLANK;
      return 2;
    }
    else if (ret == 0) {
      MessageBoxA(NULL,
        _T("ERROR: server timed out. Error code : %d", WSAGetLastError()),
        _T("Client Connection"),
        NULL);
      closesocket(sockfd);
      WSACleanup();
      menustate = TITLEBLANK;
      return 2;
    }

    // incoming IO
    if (pfd.revents & POLLIN) {
      // character selection update or the game is about to start
      receiveLobbyPacket(&current_players, &clientpnum, sockfd);
    }

    // Connection lost [server dieded uh-oh]
    if (pfd.revents & (POLLHUP | POLLERR)) {
      MessageBoxA(NULL,
        _T("ERROR: Client received POLLHUP or POLLERR signal (Host disconnected)", WSAGetLastError()),
        _T("Client Connection"),
        NULL);
      closesocket(sockfd);
      WSACleanup();
      menustate = TITLEBLANK;
      return 2;
    }

    GetCursorPos(&pnt);
    ScreenToClient(webhwnd, &pnt);
    
    if (GetAsyncKeyState(VK_LBUTTON) < 0 && is_active) {
      if (pnt.x >= 360 && pnt.x <= 549 && pnt.y >= 590 && pnt.y <= 639) {
        // user clicked the leave button
        closesocket(sockfd);
        WSACleanup();
        menustate = TITLEBLANK;
        return 1;
      }
      sendInt(pnt.x, sockfd);
      sendInt(pnt.y, sockfd);
    }

    Sleep(15);
  }

  return 0;
}
