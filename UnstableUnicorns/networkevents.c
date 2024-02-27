#include "networkevents.h"
#include "gamemechanics.h"

#ifdef TEST_RUN
#include "../UnstableUnicornsTest/mockfns.h"
#endif

int ClientNeigh(int clientpnum, int origPnum, int *origCindex) {
  int pnum = origPnum;
  int cindex = *origCindex;
  int loopend = 0;
  int oddcheck = 0;

  // receive updated player list to include new/discarded cards since the start of the turn
  if (clientpnum != origPnum) {
    ReceivePlayers(sockfd);
  }

  do {
    if (pnum != clientpnum) {
      if (cindex != -1) {
        printf("\n\033[1;31m%s\033[0m is trying to play the card \033[1;31m'%s'\033[0m.\n",
          player[pnum].username, player[pnum].hand.cards[cindex].name);
      }
      else {
        printf("\n\033[1;31m%s\033[0m is trying to Neigh the last played card.\n",
          player[pnum].username);
      }
    }
    else {
      Red();
      printf("\nWaiting to see if other players will neigh in return...\n");
      ResetCol();
    }

    // only have to check if the current player can neigh others, because CanBeNeighed is checked
    // before the start of each cycle (e.g. at the end of the loop or before the function call)
    if (!CanNeighOthers(clientpnum)) {
      printf("You are unable to play neigh cards due to the card");
      if ((player[clientpnum].flags & GINORMOUS_UNICORN) == GINORMOUS_UNICORN) {
        printf("\033[1;31m Ginormous Unicorn's \033[0m");
      }
      else {
        printf("\033[1;31m Slowdown's \033[0m");
      }
      printf("effect.\n");

      SendInt(-1, sockfd);
    }
    else {

      if (pnum != clientpnum) {
        PrintHand(clientpnum);
        printf("Choose the neigh card to counter with, or enter 0 to skip: ");
      }

      int ret2;
      int selection;
      int isvalid = 0;
      char *end = NULL;
      char buf[LINE_MAX] = { 0 };
      int inputIndex = 0;

      WSAPOLLFD pfd;
      pfd.fd = sockfd;
      pfd.events = POLLIN | POLLOUT;

      for (;;) {
        ret2 = WSAPoll(&pfd, 1, NEIGHTIME);
        if (ret2 == SOCKET_ERROR) {
          fprintf(stderr, "ERROR: poll() failed. Error code : %d", WSAGetLastError());
          exit(2);
        }
        if (ret2 == 0) {
          // this shouldn't time out because the server socket pipe is looking out for POLLOUT
          fprintf(stderr, "ERROR: client timed out. Error code : %d", WSAGetLastError());
          exit(2);
        }

        // skip the neigh input selection if the player already made a valid choice
        // or if the player played the last card in the cycle
        if (WaitForSingleObject(GetStdHandle(STD_INPUT_HANDLE), 0) == WAIT_OBJECT_0 && pnum != clientpnum && !isvalid) {
          ProcessStdin(buf, &inputIndex);
          if (inputIndex >= (sizeof(buf) - 1) || buf[inputIndex - 1] == '\r' || buf[inputIndex - 1] == '\n') {

            buf[strlen(buf) - 1] = 0;
            selection = strtol(buf, &end, 10) - 1;

            if (selection == -1 && strlen(buf) > 0) {
              SendInt(selection, sockfd);
              isvalid = 1;
            }

            // index validation
            else if (selection < -1 || selection >= player[clientpnum].hand.numCards ||
                player[clientpnum].hand.cards[selection].cType != INSTANT) {
              printf("Choose the neigh card to counter with, or enter 0 to skip: ");
            }

            else {
              SendInt(selection, sockfd);
              isvalid = 1;
            }

            memset(buf, '\0', sizeof buf);
            inputIndex = 0;
          }
        }

        if (pfd.revents & POLLIN) {
          int quitter;
          ReceiveInt(&quitter, sockfd);

          if (quitter == QUIT_LOOP)
            break;
        }

        Sleep(20);
      }
    }

    ReceiveInt(&loopend, sockfd);
    ReceivePlayers(sockfd);

    if (!loopend) {
      ReceiveInt(&pnum, sockfd);
      cindex = -1;
    }

  } while (!loopend);

  ReceiveInt(&oddcheck, sockfd);
  ReceiveInt(origCindex, sockfd);
  ReceiveInt(&discardpile.size, sockfd);
  ReceiveUnicorns(discardpile.cards, discardpile.size, sockfd);

  if ((oddcheck & 1) == 0) {
    printf("\nA neigh did not go through, and");
    Red();
    printf(" %s ", player[origPnum].username);
    ResetCol();
    printf("successfully played their card.\n");
  }
  else {
    printf("\nThe card");
    Red();
    printf(" '%s' ", discardpile.cards[discardpile.size - 1].name);
    ResetCol();
    printf("was neighed, so");
    Red();
    printf(" %s ", player[origPnum].username);
    ResetCol();
    printf("was unable to play their card.\n");
  }

  return oddcheck;
}

int ServerNeigh(int origPnum, int *origCindex) {

  for (int i = 0; i < currentPlayers - 1; i++) {
    if (origPnum == (i + 1)) continue;

    SendInt(NEIGH_EVENT, clientsockfd[i]);
    SendInt(origPnum, clientsockfd[i]);
    SendInt(*origCindex, clientsockfd[i]);
    SendPlayers(clientsockfd[i]);
  }

  int pnum = origPnum;
  int cindex = *origCindex;
  int loopend = 0;
  int oddcheck = 0;

  if (isLog && pnum == 0) LogMove(pnum, player[pnum].hand.cards[cindex], "play");

  WSAPOLLFD pfd[MAX_PLAYERS + 1] = { -1 };  // stdin + original sockfd + 7 additional players
  pfd[0].fd = _fileno(stdin);
  pfd[0].events = POLLIN | POLLOUT;
  pfd[1].fd = sockfd;
  pfd[1].events = POLLIN | POLLOUT;
  for (int i = 0; i < currentPlayers - 1; i++) {
    pfd[i + 2].fd = clientsockfd[i];
    pfd[i + 2].events = POLLIN | POLLOUT;
  }

  do {
    int ret2;
    int selection;
    int neigh_cindex = 0;
    int playerneigh = -1;
    int playerneighflag = 0;
    char *end = NULL;
    char buf[LINE_MAX] = { 0 };
    int inputIndex = 0;

    if (pnum != 0) {
      if (cindex != -1) {
        printf("\n\033[1;31m%s\033[0m is trying to play the card \033[1;31m'%s'\033[0m.\n",
          player[pnum].username, player[pnum].hand.cards[cindex].name);

        if (isLog) LogMove(pnum, player[pnum].hand.cards[cindex], "play");
      }
      else {
        printf("\n\033[1;31m%s\033[0m is trying to Neigh the last played card.\n",
          player[pnum].username);

        if (isLog) LogMove(pnum, player[pnum].hand.cards[cindex], "neigh");
      }

      if (CanNeighOthers(0)) {
        PrintHand(0);
        printf("Choose the neigh card to counter with, or enter 0 to skip: ");
      }
      else {
        printf("You are unable to play neigh cards due to the card");
        if ((player[0].flags & GINORMOUS_UNICORN) == GINORMOUS_UNICORN) {
          printf("\033[1;31m Ginormous Unicorn's \033[0m");
        }
        else {
          printf("\033[1;31m Slowdown's \033[0m");
        }
        printf("effect.\n");

        playerneighflag = 1;
      }
    }
    else {
      Red();
      printf("\nWaiting to see if other players will neigh in return...\n");
      ResetCol();
    }

    time_t endTime = time(NULL) + (NEIGHTIME / 1000);
    while (endTime > time(NULL)) {
      ret2 = WSAPoll(pfd, MAX_PLAYERS + 1, NEIGHTIME);
      if (ret2 == SOCKET_ERROR) {
        fprintf(stderr, "ERROR: poll() failed. Error code : %d", WSAGetLastError());
        exit(2);
      }
      if (ret2 == 0) {
        // this shouldn't time out because the client socket pipes are looking out for POLLOUT
        fprintf(stderr, "ERROR: client timed out. Error code : %d", WSAGetLastError());
        exit(2);
      }

      // skip the neigh input selection if the player already made a valid choice
      // or if the player played the last card in the cycle
      if (WaitForSingleObject(GetStdHandle(STD_INPUT_HANDLE), 0) == WAIT_OBJECT_0 && pnum != 0 && ((playerneighflag & 1) == 0)) {
        ProcessStdin(buf, &inputIndex);
        if (inputIndex >= (sizeof(buf) - 1) || buf[inputIndex - 1] == '\r' || buf[inputIndex - 1] == '\n') {

          buf[strlen(buf) - 1] = 0;
          selection = strtol(buf, &end, 10) - 1;

          if (selection == -1 && strlen(buf) > 0) {
            playerneighflag |= (1 << 0);
          }

          // index validation
          else if (selection < -1 || selection >= player[0].hand.numCards ||
              player[0].hand.cards[selection].cType != INSTANT) {
            printf("Choose the neigh card to counter with, or enter 0 to skip: ");
          }

          else {
            if (playerneigh == -1) {
              playerneigh = 0;
              neigh_cindex = selection;
            }
            playerneighflag |= (1 << 0);
            break;
          }

          memset(buf, '\0', sizeof buf);
          inputIndex = 0;
        }
      }

      for (int j = 0; j < currentPlayers - 1; j++) {
        if (pfd[j + 2].revents & POLLIN && pnum != (j + 1)) {
          ReceiveInt(&selection, clientsockfd[j]);

          if (selection > -1 && playerneigh == -1) {
            playerneigh = j + 1;
            neigh_cindex = selection;
          }

          playerneighflag |= (1 << (j + 1));
        }
      }

      if (playerneigh != -1 || playerneighflag == ((1 << currentPlayers) - 1 - (1 << pnum))) {
        break;
      }

      Sleep(20);
    }

    for (int i = 0; i < currentPlayers - 1; i++) {
      // send it to every client that did not skip the polling process
      // this is sent in order to trigger the end of the loop
      if (CanNeighOthers(i + 1)) {
        SendInt(QUIT_LOOP, clientsockfd[i]);
      }
    }

    if (playerneigh != -1) {
      struct Unicorn tmp = player[playerneigh].hand.cards[neigh_cindex];
      AddDiscard(tmp);
      RearrangeHand(playerneigh, neigh_cindex);
      oddcheck++;

      // adjust the original card index appropriately in the event of chain neigh cards
      if (playerneigh == origPnum && neigh_cindex < *origCindex)
        (*origCindex)--;

      if (CanBeNeighed(playerneigh) && tmp.effect != SUPER_NEIGH) {
        for (int i = 0; i < currentPlayers - 1; i++) {
          SendInt(loopend, clientsockfd[i]);
          SendPlayers(clientsockfd[i]);

          SendInt(playerneigh, clientsockfd[i]);
        }

        pnum = playerneigh;
        cindex = -1;
      }
      else {
        loopend = 1;
      }
    }
    else {
      loopend = 1;
    }

  } while (!loopend);

  if ((oddcheck & 1) == 0) {
    printf("\nA neigh did not go through, and");
    Red();
    printf(" %s ", player[origPnum].username);
    ResetCol();
    printf("successfully played their card.\n");
  }
  else {
    printf("\nThe card");
    Red();
    printf(" '%s' ", player[origPnum].hand.cards[*origCindex].name);
    ResetCol();
    printf("was neighed, so");
    Red();
    printf(" %s ", player[origPnum].username);
    ResetCol();
    printf("was unable to play their card.\n");

    AddDiscard(player[origPnum].hand.cards[*origCindex]);
    RearrangeHand(origPnum, *origCindex);
  }

  for (int i = 0; i < currentPlayers - 1; i++) {
    SendInt(loopend, clientsockfd[i]);
    SendPlayers(clientsockfd[i]);

    SendInt(oddcheck, clientsockfd[i]);
    SendInt(*origCindex, clientsockfd[i]);
    SendInt(discardpile.size, clientsockfd[i]);
    SendUnicorns(discardpile.cards, discardpile.size, clientsockfd[i]);
  }

  return oddcheck;
}

void ClientSacrifice(int clientpnum, int targetPnum, int cType) {
  int ret2;
  int selection;
  int isvalid = 1;
  char *end = NULL;
  char buf[LINE_MAX] = { 0 };
  int inputIndex = 0;

  if (targetPnum == ANY || targetPnum == clientpnum) {
    // check if clientpnum even has any cards to sacrifice
    for (int i = 0; i < player[clientpnum].stable.size; i++) {
      if (CanBeSacrificed(clientpnum, i, cType)) {
        isvalid = 0;
        PrintStable(clientpnum);
        printf("Pick a valid card number to sacrifice: ");
        break;
      }
    }

    if (isvalid) {
      SendInt(-1, sockfd); // no available cards to sacrifice
    }
  }

  WSAPOLLFD pfd;
  pfd.fd = sockfd;
  pfd.events = POLLIN | POLLOUT;

  for (;;) {
    ret2 = WSAPoll(&pfd, 1, NEIGHTIME);
    if (ret2 == SOCKET_ERROR) {
      fprintf(stderr, "ERROR: poll() failed. Error code : %d", WSAGetLastError());
      exit(2);
    }
    if (ret2 == 0) {
      // this shouldn't time out because the server socket pipe is looking out for POLLOUT
      fprintf(stderr, "ERROR: client timed out. Error code : %d", WSAGetLastError());
      exit(2);
    }

    // skip the neigh input selection if the player already made a valid choice
    // or if the player played the last card in the cycle
    if (WaitForSingleObject(GetStdHandle(STD_INPUT_HANDLE), 0) == WAIT_OBJECT_0 &&
        (targetPnum == clientpnum || targetPnum == ANY) && !isvalid) {
      ProcessStdin(buf, &inputIndex);
      if (inputIndex >= (sizeof(buf) - 1) || buf[inputIndex - 1] == '\r' || buf[inputIndex - 1] == '\n') {

        buf[strlen(buf) - 1] = 0;
        selection = strtol(buf, &end, 10) - 1;

        memset(buf, '\0', sizeof buf);
        inputIndex = 0;

        // index validation
        if (selection < 0 || selection >= player[clientpnum].stable.size ||
            !CanBeSacrificed(clientpnum, selection, cType) ||
            !CheckType(cType, player[clientpnum].stable.unicorns[selection].cType)) {
          printf("Pick a valid card number to sacrifice: ");
        }

        else {
          SendInt(selection, sockfd);
          isvalid = 1;
        }
      }
    }

    if (pfd.revents & POLLIN) {
      int signal;
      ReceiveInt(&signal, sockfd);

      if (signal == QUIT_LOOP) {
        // all eligible players sent in their choice
        break;
      }
      else if (signal == INCOMING_MSG) {
        char stdinBuf[DESC_SIZE] = { 0 };

        ReceiveMsg(stdinBuf, sockfd);
        printf("\n%s\n", stdinBuf);
      }
    }

    Sleep(20);
  }

  // that's all!
  ReceiveGamePacket(sockfd);
}

void ServerSacrifice(int origPnum, int targetPnum, int cType) {
  int ret2;
  int selection;
  int isvalid = 1;
  int playerSacrificeFlag = 0;
  int endCondition = 0;
  char *end = NULL;
  char buf[LINE_MAX] = { 0 };
  int inputIndex = 0;

  // send sacrifice event to all other players
  for (int i = 0; i < currentPlayers - 1; i++) {
    if (origPnum == (i + 1)) continue;

    SendInt(SACRIFICE_EVENT, clientsockfd[i]);
    SendCardEffectPacket(targetPnum, cType, clientsockfd[i]);
  }

  if (targetPnum == ANY) endCondition = (1 << currentPlayers) - 1;
  else endCondition = (1 << targetPnum);

  if (targetPnum == ANY || targetPnum == 0) {
    // check if the host even has any cards to sacrifice
    for (int i = 0; i < player[0].stable.size; i++) {
      if (CanBeSacrificed(0, i, cType)) {
        isvalid = 0;
        break;
      }
    }

    if (isvalid) {
      playerSacrificeFlag = 1;
    }
    else {
      PrintStable(0);
      printf("Pick a valid card number to sacrifice: ");
    }
  }

  WSAPOLLFD pfd[MAX_PLAYERS + 1] = { -1 };  // stdin + original sockfd + 7 additional players
  pfd[0].fd = _fileno(stdin);
  pfd[0].events = POLLIN | POLLOUT;
  pfd[1].fd = sockfd;
  pfd[1].events = POLLIN | POLLOUT;
  for (int i = 0; i < currentPlayers - 1; i++) {
    pfd[i + 2].fd = clientsockfd[i];
    pfd[i + 2].events = POLLIN | POLLOUT;
  }

  for (;;) {
    ret2 = WSAPoll(pfd, MAX_PLAYERS + 1, NEIGHTIME);
    if (ret2 == SOCKET_ERROR) {
      fprintf(stderr, "ERROR: poll() failed. Error code : %d", WSAGetLastError());
      exit(2);
    }
    if (ret2 == 0) {
      // this shouldn't time out because the server socket pipe is looking out for POLLOUT
      fprintf(stderr, "ERROR: client timed out. Error code : %d", WSAGetLastError());
      exit(2);
    }

    // skip the neigh input selection if the player already made a valid choice
    // or if the player played the last card in the cycle
    if (WaitForSingleObject(GetStdHandle(STD_INPUT_HANDLE), 0) == WAIT_OBJECT_0 &&
        (targetPnum == 0 || targetPnum == ANY) && !isvalid) {
      ProcessStdin(buf, &inputIndex);
      if (inputIndex >= (sizeof(buf) - 1) || buf[inputIndex - 1] == '\r' || buf[inputIndex - 1] == '\n') {

        buf[strlen(buf) - 1] = 0;
        selection = strtol(buf, &end, 10) - 1;

        memset(buf, '\0', sizeof buf);
        inputIndex = 0;

        // index validation
        if (selection < 0 || selection >= player[0].stable.size ||
            !CanBeSacrificed(0, selection, cType) ||
            !CheckType(cType, player[0].stable.unicorns[selection].cType)) {
          printf("Pick a valid card number to sacrifice: ");
        }

        else {
          playerSacrificeFlag |= 1;
          isvalid = 1;

          char stdinBuf[DESC_SIZE] = { 0 };
          sprintf_s(stdinBuf, sizeof stdinBuf, "\033[1;31m%s\033[0m sacrificed the card '\033[1;31m%s\033[0m'.",
            player[0].username, player[0].stable.unicorns[selection].name);

          if (isLog) LogMove(0, player[0].stable.unicorns[selection], "sacrifice");

          Base_SacrificeDestroyEffects(0, selection, player[0].stable.unicorns[selection].effect);

          for (int j = 0; j < currentPlayers - 1; j++) {
            SendInt(INCOMING_MSG, clientsockfd[j]);
            SendMsg(stdinBuf, sizeof stdinBuf, clientsockfd[j]);
          }
        }
      }
    }

    for (int i = 0; i < currentPlayers - 1; i++) {
      if (pfd[i + 2].revents & POLLIN) {
        int cindex;
        ReceiveInt(&cindex, clientsockfd[i]);

        playerSacrificeFlag |= (1 << (i + 1));

        if (cindex == -1) continue;

        char stdinBuf[DESC_SIZE] = { 0 };
        sprintf_s(stdinBuf, sizeof stdinBuf, "\033[1;31m%s\033[0m sacrificed the card '\033[1;31m%s\033[0m'.",
          player[i + 1].username, player[i + 1].stable.unicorns[cindex].name);

        if (isLog) LogMove(i + 1, player[i + 1].stable.unicorns[selection], "sacrifice");

        Base_SacrificeDestroyEffects(i + 1, cindex, player[i + 1].stable.unicorns[cindex].effect);
        
        for (int j = 0; j < currentPlayers - 1; j++) {
          if (j == i) continue;

          SendInt(INCOMING_MSG, clientsockfd[j]);
          SendMsg(stdinBuf, sizeof stdinBuf, clientsockfd[j]);
        }

        printf("\n%s\n", stdinBuf);
      }
    }

    if (playerSacrificeFlag == endCondition) {
      for (int i = 0; i < currentPlayers - 1; i++) {
        SendInt(QUIT_LOOP, clientsockfd[i]);
      }
      break;
    }

    Sleep(20);
  }

  // that's all!
  for (int i = 0; i < currentPlayers - 1; i++) {
    SendGamePacket(clientsockfd[i]);
  }
}

void ClientDiscard(int clientpnum, int targetPnum, int cType) {
  int ret2;
  int selection;
  int isvalid = 1;
  char *end = NULL;
  char buf[LINE_MAX] = { 0 };
  int inputIndex = 0;

  if (targetPnum == ANY || targetPnum == clientpnum) {
    if (player[clientpnum].hand.numCards <= 0) {
      SendInt(-1, sockfd); // no available cards to discard
    }
    else {
      isvalid = 0;
      PrintHand(clientpnum);
      printf("Pick a valid card number to discard: ");
    }
  }


  WSAPOLLFD pfd;
  pfd.fd = sockfd;
  pfd.events = POLLIN | POLLOUT;

  for (;;) {
    ret2 = WSAPoll(&pfd, 1, NEIGHTIME);
    if (ret2 == SOCKET_ERROR) {
      fprintf(stderr, "ERROR: poll() failed. Error code : %d", WSAGetLastError());
      exit(2);
    }
    if (ret2 == 0) {
      // this shouldn't time out because the server socket pipe is looking out for POLLOUT
      fprintf(stderr, "ERROR: client timed out. Error code : %d", WSAGetLastError());
      exit(2);
    }

    // skip the neigh input selection if the player already made a valid choice
    // or if the player played the last card in the cycle
    if (WaitForSingleObject(GetStdHandle(STD_INPUT_HANDLE), 0) == WAIT_OBJECT_0 &&
        (targetPnum == clientpnum || targetPnum == ANY) && !isvalid) {
      ProcessStdin(buf, &inputIndex);
      if (inputIndex >= (sizeof(buf) - 1) || buf[inputIndex - 1] == '\r' || buf[inputIndex - 1] == '\n') {

        buf[strlen(buf) - 1] = 0;
        selection = strtol(buf, &end, 10) - 1;

        memset(buf, '\0', sizeof buf);
        inputIndex = 0;

        // index validation
        if (selection < 0 || selection >= player[clientpnum].hand.numCards ||
            !CheckType(cType, player[clientpnum].hand.cards[selection].cType)) {
          printf("Pick a valid card number to discard: ");
        }

        else {
          SendInt(selection, sockfd);
          isvalid = 1;
        }
      }
    }

    if (pfd.revents & POLLIN) {
      int signal;
      ReceiveInt(&signal, sockfd);

      if (signal == QUIT_LOOP) {
        // all eligible players sent in their choice
        break;
      }
      else if (signal == INCOMING_MSG) {
        char stdinBuf[DESC_SIZE] = { 0 };

        ReceiveMsg(stdinBuf, sockfd);
        printf("\n%s\n", stdinBuf);
      }
    }

    Sleep(20);
  }

  // that's all!
  ReceiveGamePacket(sockfd);
}

void ServerDiscard(int origPnum, int targetPnum, int cType) {
  int ret2;
  int selection;
  int isvalid = 1;
  int playerDiscardFlag = 0;
  int endCondition = 0;
  char *end = NULL;
  char buf[LINE_MAX] = { 0 };
  int inputIndex = 0;

  // send discard event to all other players
  for (int i = 0; i < currentPlayers - 1; i++) {
    if (origPnum == (i + 1)) continue;

    SendInt(DISCARD_EVENT, clientsockfd[i]);
    SendCardEffectPacket(targetPnum, cType, clientsockfd[i]);
  }

  if (targetPnum == ANY) endCondition = (1 << currentPlayers) - 1;
  else endCondition = (1 << targetPnum);

  if (targetPnum == ANY || targetPnum == 0) {
    if (player[0].hand.numCards <= 0) {
      playerDiscardFlag = 1;
    }
    else {
      isvalid = 0;
      PrintHand(0);
      printf("Pick a valid card number to discard: ");
    }
  }

  WSAPOLLFD pfd[MAX_PLAYERS + 1] = { -1 };  // stdin + original sockfd + 7 additional players
  pfd[0].fd = _fileno(stdin);
  pfd[0].events = POLLIN | POLLOUT;
  pfd[1].fd = sockfd;
  pfd[1].events = POLLIN | POLLOUT;
  for (int i = 0; i < currentPlayers - 1; i++) {
    pfd[i + 2].fd = clientsockfd[i];
    pfd[i + 2].events = POLLIN | POLLOUT;
  }

  for (;;) {
    ret2 = WSAPoll(pfd, MAX_PLAYERS + 1, NEIGHTIME);
    if (ret2 == SOCKET_ERROR) {
      fprintf(stderr, "ERROR: poll() failed. Error code : %d", WSAGetLastError());
      exit(2);
    }
    if (ret2 == 0) {
      // this shouldn't time out because the server socket pipe is looking out for POLLOUT
      fprintf(stderr, "ERROR: client timed out. Error code : %d", WSAGetLastError());
      exit(2);
    }

    // skip the neigh input selection if the player already made a valid choice
    // or if the player played the last card in the cycle
    if (WaitForSingleObject(GetStdHandle(STD_INPUT_HANDLE), 0) == WAIT_OBJECT_0 &&
        (targetPnum == 0 || targetPnum == ANY) && !isvalid) {
      ProcessStdin(buf, &inputIndex);
      if (inputIndex >= (sizeof(buf) - 1) || buf[inputIndex - 1] == '\r' || buf[inputIndex - 1] == '\n') {

        buf[strlen(buf) - 1] = 0;
        selection = strtol(buf, &end, 10) - 1;

        memset(buf, '\0', sizeof buf);
        inputIndex = 0;

        // index validation
        if (selection < 0 || selection >= player[0].hand.numCards ||
            !CheckType(cType, player[0].hand.cards[selection].cType)) {
          printf("Pick a valid card number to discard: ");
        }

        else {
          playerDiscardFlag |= 1;
          isvalid = 1;

          char stdinBuf[DESC_SIZE] = { 0 };
          sprintf_s(stdinBuf, sizeof stdinBuf, "\033[1;31m%s\033[0m discarded the card '\033[1;31m%s\033[0m'.",
            player[0].username, player[0].hand.cards[selection].name);

          if (isLog) LogMove(0, player[0].stable.unicorns[selection], "discard");

          AddDiscard(player[0].hand.cards[selection]);
          RearrangeHand(0, selection);

          for (int j = 0; j < currentPlayers - 1; j++) {
            SendInt(INCOMING_MSG, clientsockfd[j]);
            SendMsg(stdinBuf, sizeof stdinBuf, clientsockfd[j]);
          }
        }
      }
    }

    for (int i = 0; i < currentPlayers - 1; i++) {
      if (pfd[i + 2].revents & POLLIN) {
        int cindex;
        ReceiveInt(&cindex, clientsockfd[i]);

        playerDiscardFlag |= (1 << (i + 1));

        if (cindex == -1) continue;

        char stdinBuf[DESC_SIZE] = { 0 };
        sprintf_s(stdinBuf, sizeof stdinBuf, "\033[1;31m%s\033[0m discarded the card '\033[1;31m%s\033[0m'.",
          player[i + 1].username, player[i + 1].hand.cards[cindex].name);

        if (isLog) LogMove(i + 1, player[i + 1].stable.unicorns[selection], "discard");

        AddDiscard(player[i + 1].hand.cards[cindex]);
        RearrangeHand(i + 1, cindex);

        for (int j = 0; j < currentPlayers - 1; j++) {
          if (j == i) continue;

          SendInt(INCOMING_MSG, clientsockfd[j]);
          SendMsg(stdinBuf, sizeof stdinBuf, clientsockfd[j]);
        }

        printf("\n%s\n", stdinBuf);
      }
    }

    if (playerDiscardFlag == endCondition) {
      for (int i = 0; i < currentPlayers - 1; i++) {
        SendInt(QUIT_LOOP, clientsockfd[i]);
      }
      break;
    }

    Sleep(20);
  }

  // that's all!
  for (int i = 0; i < currentPlayers - 1; i++) {
    SendGamePacket(clientsockfd[i]);
  }
}

void ClientEnterLeaveStable(int clientpnum) {
  int ret2;
  int eventloop = 0;
  int nEvents;

  WSAPOLLFD pfd;
  pfd.fd = sockfd;
  pfd.events = POLLIN | POLLOUT;

  do {
    ret2 = WSAPoll(&pfd, 1, -1);
    if (ret2 == SOCKET_ERROR) {
      fprintf(stderr, "ERROR: poll() failed. Error code : %d", WSAGetLastError());
      exit(2);
    }
    else if (ret2 == 0) {
      fprintf(stderr, "ERROR: server timed out. Error code : %d", WSAGetLastError());
      exit(2);
    }

    if (pfd.revents & POLLIN) {
      ReceiveInt(&nEvents, sockfd);

      eventloop = netStates[nEvents].RecvClient(clientpnum, sockfd);
    }

    Sleep(20);

  } while (!eventloop);
}

void ServerEnterLeaveStable(int origPnum, int targetPnum) {
  // can always assume that targetPnum is a separate player from the host (or else this function wouldn't be called);
  // origPnum can either be the host or a player that's different from targetPnum
  int ret2;
  int eventloop = 0;
  int nEvents;

  WSAPOLLFD pfd[2] = { -1 };  // original sockfd + relevant client
  pfd[0].fd = sockfd;
  pfd[0].events = POLLIN | POLLOUT;
  pfd[1].fd = clientsockfd[targetPnum - 1];
  pfd[1].events = POLLIN | POLLOUT;

  do {
    ret2 = WSAPoll(pfd, 2, -1);
    if (ret2 == SOCKET_ERROR) {
      fprintf(stderr, "ERROR: poll() failed. Error code : %d", WSAGetLastError());
      closesocket(sockfd);
      exit(2);
    }
    else if (ret2 == 0) {
      fprintf(stderr, "ERROR: server timed out. Error code : %d", WSAGetLastError());
      closesocket(sockfd);
      exit(2);
    }

    if (pfd[1].revents & POLLIN) {
      // targetPnum becomes the new "origPnum" because that player is
      // the catalyst that starts the next nested loop
      ReceiveInt(&nEvents, clientsockfd[targetPnum - 1]);

      eventloop = netStates[nEvents].RecvServer(targetPnum, clientsockfd[targetPnum - 1]);

      // this is all to avoid adding a 3rd parameter just for one instance...
      if (eventloop && origPnum != 0) {
        SendInt(QUIT_LOOP, clientsockfd[origPnum - 1]);
        SendGamePacket(clientsockfd[origPnum - 1]);
      }
    }

    Sleep(20);

  } while (!eventloop);
}
