#include "networkevents.h"
#include "gamemechanics.h"
#include <windows.h>

// was unable to properly link this with an external file ;~;
#ifdef TEST_RUN
#define processStdin(a, b) processStdin_mock(a, b)
#define WaitForSingleObject(a, b) simple(a, b)

void processStdin_mock(char* stdinbuf, int* bufindex) {

  fgets(stdinbuf, 1024, fpinput);
  *bufindex = strlen(stdinbuf);

}

DWORD simple(HANDLE hHandle, DWORD dwMilliseconds) {
  return WAIT_OBJECT_0;
}
#endif

// codes the part where players are able to use an instant card against a play
// 0 = nobody used Neigh/Super Neigh cards, or Neigh cards cancelled out
// 1 = card is gone
int clientNeigh(int clientpnum, int orig_pnum, int *orig_cindex) {
  int pnum = orig_pnum;
  int cindex = *orig_cindex;
  int loopend = 0;
  int oddcheck = 0;

  // receive updated player list to include new/discarded cards since the start of the turn
  if (clientpnum != orig_pnum) {
    receivePlayers(sockfd);
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
      red();
      printf("\nWaiting to see if other players will neigh in return...\n");
      reset_col();
    }

    // only have to check if the current player can neigh others, because canBeNeighed is checked
    // before the start of each cycle (e.g. at the end of the loop or before the function call)
    if (!canNeighOthers(clientpnum)) {
      printf("You are unable to play neigh cards due to the card");
      if ((player[clientpnum].flags & ginormous_unicorn) == ginormous_unicorn) {
        printf("\033[1;31m Ginormous Unicorn's \033[0m");
      }
      else {
        printf("\033[1;31m Slowdown's \033[0m");
      }
      printf("effect.\n");

      sendInt(-1, sockfd);
    }
    else {

      if (pnum != clientpnum) {
        printHand(clientpnum);
        printf("Choose the neigh card to counter with, or enter 0 to skip: ");
      }

      int ret2;
      int selection;
      int isvalid = 0;
      char* end = NULL;
      char buf[LINE_MAX] = { 0 };
      int input_index = 0;

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
          processStdin(buf, &input_index);
          if (input_index >= (sizeof(buf) - 1) || buf[input_index - 1] == '\r' || buf[input_index - 1] == '\n') {

            buf[strlen(buf) - 1] = 0;
            selection = strtol(buf, &end, 10) - 1;

            if (selection == -1 && strlen(buf) > 0) {
              sendInt(selection, sockfd);
              isvalid = 1;
            }

            // index validation
            else if (selection < -1 || selection >= player[clientpnum].hand.num_cards ||
                player[clientpnum].hand.cards[selection].cType != INSTANT) {
              printf("Choose the neigh card to counter with, or enter 0 to skip: ");
            }

            else {
              sendInt(selection, sockfd);
              isvalid = 1;
            }

            memset(buf, '\0', sizeof buf);
            input_index = 0;
          }
        }

        if (pfd.revents & POLLIN) {
          int quitter;
          receiveInt(&quitter, sockfd);

          if (quitter == quit_loop)
            break;
        }

        Sleep(20);
      }
    }

    receiveInt(&loopend, sockfd);
    receivePlayers(sockfd);

    if (!loopend) {
      receiveInt(&pnum, sockfd);
      cindex = -1;
    }

  } while (!loopend);

  receiveInt(&oddcheck, sockfd);
  receiveInt(orig_cindex, sockfd);
  receiveInt(&discardpile.size, sockfd);
  receiveUnicorns(discardpile.cards, discardpile.size, sockfd);

  if ((oddcheck & 1) == 0) {
    printf("\nA neigh did not go through, and");
    red();
    printf(" %s ", player[orig_pnum].username);
    reset_col();
    printf("successfully played their card.\n");
  }
  else {
    printf("\nThe card");
    red();
    printf(" '%s' ", discardpile.cards[discardpile.size - 1].name);
    reset_col();
    printf("was neighed, so");
    red();
    printf(" %s ", player[orig_pnum].username);
    reset_col();
    printf("was unable to play their card.\n");
  }

  return oddcheck;
}

// codes the part where players are able to use an instant card against a play
// 0 = nobody used Neigh/Super Neigh cards, or Neigh cards cancelled out
// 1 = card is gone
int serverNeigh(int orig_pnum, int *orig_cindex) {

  for (int i = 0; i < current_players - 1; i++) {
    if (orig_pnum == (i + 1)) continue;

    sendInt(neigh_event, clientsockfd[i]);
    sendInt(orig_pnum, clientsockfd[i]);
    sendInt(*orig_cindex, clientsockfd[i]);
    sendPlayers(clientsockfd[i]);
  }

  int pnum = orig_pnum;
  int cindex = *orig_cindex;
  int loopend = 0;
  int oddcheck = 0;

  WSAPOLLFD pfd[MAX_PLAYERS + 1] = { -1 };  // stdin + original sockfd + 7 additional players
  pfd[0].fd = _fileno(stdin);
  pfd[0].events = POLLIN | POLLOUT;
  pfd[1].fd = sockfd;
  pfd[1].events = POLLIN | POLLOUT;
  for (int i = 0; i < current_players - 1; i++) {
    pfd[i + 2].fd = clientsockfd[i];
    pfd[i + 2].events = POLLIN | POLLOUT;
  }

  do {
    int ret2;
    int selection;
    int neigh_cindex = 0;
    int playerneigh = -1;
    int playerneighflag = 0;
    char* end = NULL;
    char buf[LINE_MAX] = { 0 };
    int input_index = 0;

    if (pnum != 0) {
      if (cindex != -1) {
        printf("\n\033[1;31m%s\033[0m is trying to play the card \033[1;31m'%s'\033[0m.\n",
          player[pnum].username, player[pnum].hand.cards[cindex].name);
      }
      else {
        printf("\n\033[1;31m%s\033[0m is trying to Neigh the last played card.\n",
          player[pnum].username);
      }

      if (canNeighOthers(0)) {
        printHand(0);
        printf("Choose the neigh card to counter with, or enter 0 to skip: ");
      }
      else {
        printf("You are unable to play neigh cards due to the card");
        if ((player[0].flags & ginormous_unicorn) == ginormous_unicorn) {
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
      red();
      printf("\nWaiting to see if other players will neigh in return...\n");
      reset_col();
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
        processStdin(buf, &input_index);
        if (input_index >= (sizeof(buf) - 1) || buf[input_index - 1] == '\r' || buf[input_index - 1] == '\n') {

          buf[strlen(buf) - 1] = 0;
          selection = strtol(buf, &end, 10) - 1;

          if (selection == -1 && strlen(buf) > 0) {
            playerneighflag |= (1 << 0);
          }

          // index validation
          else if (selection < -1 || selection >= player[0].hand.num_cards ||
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
          input_index = 0;
        }
      }

      for (int j = 0; j < current_players - 1; j++) {
        if (pfd[j + 2].revents & POLLIN && pnum != (j + 1)) {
          receiveInt(&selection, clientsockfd[j]);

          if (selection > -1 && playerneigh == -1) {
            playerneigh = j + 1;
            neigh_cindex = selection;
          }

          playerneighflag |= (1 << (j + 1));
        }
      }

      if (playerneigh != -1 || playerneighflag == ((1 << current_players) - 1 - (1 << pnum))) {
        break;
      }

      Sleep(20);
    }

    for (int i = 0; i < current_players - 1; i++) {
      // send it to every client that did not skip the polling process
      // this is sent in order to trigger the end of the loop
      if (canNeighOthers(i + 1)) {
        sendInt(quit_loop, clientsockfd[i]);
      }
    }

    if (playerneigh != -1) {
      struct Unicorn tmp = player[playerneigh].hand.cards[neigh_cindex];
      addDiscard(tmp);
      rearrangeHand(playerneigh, neigh_cindex);
      oddcheck++;

      // adjust the original card index appropriately in the event of chain neigh cards
      if (playerneigh == orig_pnum && neigh_cindex < *orig_cindex)
        (*orig_cindex)--;

      if (canBeNeighed(playerneigh) && strcmp(tmp.name, "Super Neigh") != 0) {
        for (int i = 0; i < current_players - 1; i++) {
          sendInt(loopend, clientsockfd[i]);
          sendPlayers(clientsockfd[i]);

          sendInt(playerneigh, clientsockfd[i]);
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
    red();
    printf(" %s ", player[orig_pnum].username);
    reset_col();
    printf("successfully played their card.\n");
  }
  else {
    printf("\nThe card");
    red();
    printf(" '%s' ", player[orig_pnum].hand.cards[*orig_cindex].name);
    reset_col();
    printf("was neighed, so");
    red();
    printf(" %s ", player[orig_pnum].username);
    reset_col();
    printf("was unable to play their card.\n");

    addDiscard(player[orig_pnum].hand.cards[*orig_cindex]);
    rearrangeHand(orig_pnum, *orig_cindex);
  }

  for (int i = 0; i < current_players - 1; i++) {
    sendInt(loopend, clientsockfd[i]);
    sendPlayers(clientsockfd[i]);

    sendInt(oddcheck, clientsockfd[i]);
    sendInt(*orig_cindex, clientsockfd[i]);
    sendInt(discardpile.size, clientsockfd[i]);
    sendUnicorns(discardpile.cards, discardpile.size, clientsockfd[i]);
  }

  return oddcheck;
}

void clientSacrifice(int clientpnum, int target_pnum, int cType) {
  int ret2;
  int selection;
  int isvalid = 1;
  char* end = NULL;
  char buf[LINE_MAX] = { 0 };
  int input_index = 0;

  if (target_pnum == ANY || target_pnum == clientpnum) {
    // check if clientpnum even has any cards to sacrifice
    for (int i = 0; i < player[clientpnum].stable.size; i++) {
      if (canBeSacrificed(clientpnum, i, cType)) {
        isvalid = 0;
        printStable(clientpnum);
        printf("Pick a valid card number to sacrifice: ");
        break;
      }
    }

    if (isvalid) {
      sendInt(-1, sockfd); // no available cards to sacrifice
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
        (target_pnum == clientpnum || target_pnum == ANY) && !isvalid) {
      processStdin(buf, &input_index);
      if (input_index >= (sizeof(buf) - 1) || buf[input_index - 1] == '\r' || buf[input_index - 1] == '\n') {

        buf[strlen(buf) - 1] = 0;
        selection = strtol(buf, &end, 10) - 1;

        memset(buf, '\0', sizeof buf);
        input_index = 0;

        // index validation
        if (selection < 0 || selection >= player[clientpnum].stable.size ||
            !canBeSacrificed(clientpnum, selection, cType) ||
            !checkType(cType, player[clientpnum].stable.unicorns[selection].cType)) {
          printf("Pick a valid card number to sacrifice: ");
        }

        else {
          sendInt(selection, sockfd);
          isvalid = 1;
        }
      }
    }

    if (pfd.revents & POLLIN) {
      int signal;
      receiveInt(&signal, sockfd);

      if (signal == quit_loop) {
        // all eligible players sent in their choice
        break;
      }
      else if (signal == incoming_msg) {
        char stdinbuf[DESC_SIZE] = { 0 };

        receiveMsg(stdinbuf, sockfd);
        printf("\n%s\n", stdinbuf);
      }
    }

    Sleep(20);
  }

  // that's all!
  receiveGamePacket(sockfd);
}

void serverSacrifice(int orig_pnum, int target_pnum, int cType) {
  int ret2;
  int selection;
  int isvalid = 1;
  int playersacrificeflag = 0;
  int end_condition = 0;
  char* end = NULL;
  char buf[LINE_MAX] = { 0 };
  int input_index = 0;

  // send sacrifice event to all other players
  for (int i = 0; i < current_players - 1; i++) {
    if (orig_pnum == (i + 1)) continue;

    sendInt(sacrifice_event, clientsockfd[i]);
    sendCardEffectPacket(target_pnum, cType, clientsockfd[i]);
  }

  if (target_pnum == ANY) end_condition = (1 << current_players) - 1;
  else end_condition = (1 << target_pnum);

  if (target_pnum == ANY || target_pnum == 0) {
    // check if the host even has any cards to sacrifice
    for (int i = 0; i < player[0].stable.size; i++) {
      if (canBeSacrificed(0, i, cType)) {
        isvalid = 0;
        break;
      }
    }

    if (isvalid) {
      playersacrificeflag = 1;
    }
    else {
      printStable(0);
      printf("Pick a valid card number to sacrifice: ");
    }
  }

  WSAPOLLFD pfd[MAX_PLAYERS + 1] = { -1 };  // stdin + original sockfd + 7 additional players
  pfd[0].fd = _fileno(stdin);
  pfd[0].events = POLLIN | POLLOUT;
  pfd[1].fd = sockfd;
  pfd[1].events = POLLIN | POLLOUT;
  for (int i = 0; i < current_players - 1; i++) {
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
        (target_pnum == 0 || target_pnum == ANY) && !isvalid) {
      processStdin(buf, &input_index);
      if (input_index >= (sizeof(buf) - 1) || buf[input_index - 1] == '\r' || buf[input_index - 1] == '\n') {

        buf[strlen(buf) - 1] = 0;
        selection = strtol(buf, &end, 10) - 1;

        memset(buf, '\0', sizeof buf);
        input_index = 0;

        // index validation
        if (selection < 0 || selection >= player[0].stable.size ||
            !canBeSacrificed(0, selection, cType) ||
            !checkType(cType, player[0].stable.unicorns[selection].cType)) {
          printf("Pick a valid card number to sacrifice: ");
        }

        else {
          playersacrificeflag |= 1;
          isvalid = 1;

          char stdinbuf[DESC_SIZE] = { 0 };
          sprintf_s(stdinbuf, sizeof stdinbuf, "\033[1;31m%s\033[0m sacrificed the card '\033[1;31m%s\033[0m'.",
            player[0].username, player[0].stable.unicorns[selection].name);

          sacrificeDestroyEffects(0, selection, player[0].stable.unicorns[selection].effect);

          for (int j = 0; j < current_players - 1; j++) {
            sendInt(incoming_msg, clientsockfd[j]);
            sendMsg(stdinbuf, sizeof stdinbuf, clientsockfd[j]);
          }
        }
      }
    }

    for (int i = 0; i < current_players - 1; i++) {
      if (pfd[i + 2].revents & POLLIN) {
        int cindex;
        receiveInt(&cindex, clientsockfd[i]);

        playersacrificeflag |= (1 << (i + 1));

        if (cindex == -1) continue;

        char stdinbuf[DESC_SIZE] = { 0 };
        sprintf_s(stdinbuf, sizeof stdinbuf, "\033[1;31m%s\033[0m sacrificed the card '\033[1;31m%s\033[0m'.",
          player[i + 1].username, player[i + 1].stable.unicorns[cindex].name);

        sacrificeDestroyEffects(i + 1, cindex, player[i + 1].stable.unicorns[cindex].effect);
        
        for (int j = 0; j < current_players - 1; j++) {
          if (j == i) continue;

          sendInt(incoming_msg, clientsockfd[j]);
          sendMsg(stdinbuf, sizeof stdinbuf, clientsockfd[j]);
        }

        printf("\n%s\n", stdinbuf);
      }
    }

    if (playersacrificeflag == end_condition) {
      for (int i = 0; i < current_players - 1; i++) {
        sendInt(quit_loop, clientsockfd[i]);
      }
      break;
    }

    Sleep(20);
  }

  // that's all!
  for (int i = 0; i < current_players - 1; i++) {
    sendGamePacket(clientsockfd[i]);
  }
}

void clientDiscard(int clientpnum, int target_pnum, int cType) {
  int ret2;
  int selection;
  int isvalid = 1;
  char* end = NULL;
  char buf[LINE_MAX] = { 0 };
  int input_index = 0;

  if (target_pnum == ANY || target_pnum == clientpnum) {
    if (player[clientpnum].hand.num_cards <= 0) {
      sendInt(-1, sockfd); // no available cards to discard
    }
    else {
      isvalid = 0;
      printHand(clientpnum);
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
        (target_pnum == clientpnum || target_pnum == ANY) && !isvalid) {
      processStdin(buf, &input_index);
      if (input_index >= (sizeof(buf) - 1) || buf[input_index - 1] == '\r' || buf[input_index - 1] == '\n') {

        buf[strlen(buf) - 1] = 0;
        selection = strtol(buf, &end, 10) - 1;

        memset(buf, '\0', sizeof buf);
        input_index = 0;

        // index validation
        if (selection < 0 || selection >= player[clientpnum].hand.num_cards ||
            !checkType(cType, player[clientpnum].hand.cards[selection].cType)) {
          printf("Pick a valid card number to discard: ");
        }

        else {
          sendInt(selection, sockfd);
          isvalid = 1;
        }
      }
    }

    if (pfd.revents & POLLIN) {
      int signal;
      receiveInt(&signal, sockfd);

      if (signal == quit_loop) {
        // all eligible players sent in their choice
        break;
      }
      else if (signal == incoming_msg) {
        char stdinbuf[DESC_SIZE] = { 0 };

        receiveMsg(stdinbuf, sockfd);
        printf("\n%s\n", stdinbuf);
      }
    }

    Sleep(20);
  }

  // that's all!
  receiveGamePacket(sockfd);
}

void serverDiscard(int orig_pnum, int target_pnum, int cType) {
  int ret2;
  int selection;
  int isvalid = 1;
  int playerdiscardflag = 0;
  int end_condition = 0;
  char* end = NULL;
  char buf[LINE_MAX] = { 0 };
  int input_index = 0;

  // send discard event to all other players
  for (int i = 0; i < current_players - 1; i++) {
    if (orig_pnum == (i + 1)) continue;

    sendInt(discard_event, clientsockfd[i]);
    sendCardEffectPacket(target_pnum, cType, clientsockfd[i]);
  }

  if (target_pnum == ANY) end_condition = (1 << current_players) - 1;
  else end_condition = (1 << target_pnum);

  if (target_pnum == ANY || target_pnum == 0) {
    if (player[0].hand.num_cards <= 0) {
      playerdiscardflag = 1;
    }
    else {
      isvalid = 0;
      printHand(0);
      printf("Pick a valid card number to discard: ");
    }
  }

  WSAPOLLFD pfd[MAX_PLAYERS + 1] = { -1 };  // stdin + original sockfd + 7 additional players
  pfd[0].fd = _fileno(stdin);
  pfd[0].events = POLLIN | POLLOUT;
  pfd[1].fd = sockfd;
  pfd[1].events = POLLIN | POLLOUT;
  for (int i = 0; i < current_players - 1; i++) {
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
        (target_pnum == 0 || target_pnum == ANY) && !isvalid) {
      processStdin(buf, &input_index);
      if (input_index >= (sizeof(buf) - 1) || buf[input_index - 1] == '\r' || buf[input_index - 1] == '\n') {

        buf[strlen(buf) - 1] = 0;
        selection = strtol(buf, &end, 10) - 1;

        memset(buf, '\0', sizeof buf);
        input_index = 0;

        // index validation
        if (selection < 0 || selection >= player[0].hand.num_cards ||
            !checkType(cType, player[0].hand.cards[selection].cType)) {
          printf("Pick a valid card number to discard: ");
        }

        else {
          playerdiscardflag |= 1;
          isvalid = 1;

          char stdinbuf[DESC_SIZE] = { 0 };
          sprintf_s(stdinbuf, sizeof stdinbuf, "\033[1;31m%s\033[0m discarded the card '\033[1;31m%s\033[0m'.",
            player[0].username, player[0].hand.cards[selection].name);

          addDiscard(player[0].hand.cards[selection]);
          rearrangeHand(0, selection);

          for (int j = 0; j < current_players - 1; j++) {
            sendInt(incoming_msg, clientsockfd[j]);
            sendMsg(stdinbuf, sizeof stdinbuf, clientsockfd[j]);
          }
        }
      }
    }

    for (int i = 0; i < current_players - 1; i++) {
      if (pfd[i + 2].revents & POLLIN) {
        int cindex;
        receiveInt(&cindex, clientsockfd[i]);

        playerdiscardflag |= (1 << (i + 1));

        if (cindex == -1) continue;

        char stdinbuf[DESC_SIZE] = { 0 };
        sprintf_s(stdinbuf, sizeof stdinbuf, "\033[1;31m%s\033[0m discarded the card '\033[1;31m%s\033[0m'.",
          player[i + 1].username, player[i + 1].hand.cards[cindex].name);

        addDiscard(player[i + 1].hand.cards[cindex]);
        rearrangeHand(i + 1, cindex);

        for (int j = 0; j < current_players - 1; j++) {
          if (j == i) continue;

          sendInt(incoming_msg, clientsockfd[j]);
          sendMsg(stdinbuf, sizeof stdinbuf, clientsockfd[j]);
        }

        printf("\n%s\n", stdinbuf);
      }
    }

    if (playerdiscardflag == end_condition) {
      for (int i = 0; i < current_players - 1; i++) {
        sendInt(quit_loop, clientsockfd[i]);
      }
      break;
    }

    Sleep(20);
  }

  // that's all!
  for (int i = 0; i < current_players - 1; i++) {
    sendGamePacket(clientsockfd[i]);
  }
}

// idle function that handles nested network events;
// this works for both enter stable effects and unicorn sacrificeDestroy effects
void clientEnterLeaveStable(int clientpnum) {
  int ret2;
  int eventloop = 0;

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
      receiveInt(&network_events, sockfd);

      eventloop = netStates[network_events].recvClient(clientpnum, sockfd);
    }

    Sleep(20);

  } while (!eventloop);
}

// idle function that handles nested network events;
// this works for both enter stable effects and unicorn sacrificeDestroy effects
// 
// TODO: check if the target player wins the game inadvertently through
// an enter stable effect sacrificing masquerade cards
void serverEnterLeaveStable(int orig_pnum, int target_pnum) {
  // can always assume that target_pnum is a separate player from the host (or else this function wouldn't be called);
  // orig_pnum can either be the host or a player that's different from target_pnum
  int ret2;
  int eventloop = 0;

  WSAPOLLFD pfd[2] = { -1 };  // original sockfd + relevant client
  pfd[0].fd = sockfd;
  pfd[0].events = POLLIN | POLLOUT;
  pfd[1].fd = clientsockfd[target_pnum - 1];
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
      // target_pnum becomes the new "orig_pnum" because that player is
      // the catalyst that starts the next nested loop
      receiveInt(&network_events, clientsockfd[target_pnum - 1]);

      eventloop = netStates[network_events].recvServer(target_pnum, clientsockfd[target_pnum - 1]);

      // this is all to avoid adding a 3rd parameter just for one instance...
      if (eventloop && orig_pnum != 0) {
        sendInt(quit_loop, clientsockfd[orig_pnum - 1]);
        sendGamePacket(clientsockfd[orig_pnum - 1]);
      }
    }

    Sleep(20);

  } while (!eventloop);
}
