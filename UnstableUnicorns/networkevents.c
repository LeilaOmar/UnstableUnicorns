#include "networkevents.h"
#include "gamemechanics.h"
#include <conio.h>

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

      WSAPOLLFD pfd;
      pfd.fd = sockfd;
      pfd.events = POLLIN | POLLOUT;

      for (;;) {
        ret2 = WSAPoll(&pfd, 1, NEIGHTIME);
        if (ret2 == SOCKET_ERROR) {
          fprintf(stderr, "ERROR: poll() failed. Error code : %d", WSAGetLastError());
          closesocket(sockfd);
          _getch();
          exit(2);
        }
        if (ret2 == 0) {
          // this shouldn't time out because the server socket pipe is looking out for POLLOUT
          fprintf(stderr, "ERROR: client timed out. Error code : %d", WSAGetLastError());
          closesocket(sockfd);
          _getch();
          exit(2);
        }

        // skip the neigh input selection if the player already made a valid choice
        // or if the player played the last card in the cycle
        if (WaitForSingleObject(GetStdHandle(STD_INPUT_HANDLE), 0) == WAIT_OBJECT_0 && pnum != clientpnum && !isvalid) {
          processStdin(stdinbuf, &bufindex);
          if (bufindex >= (sizeof(stdinbuf) - 1) || stdinbuf[bufindex - 1] == '\r' || stdinbuf[bufindex - 1] == '\n') {

            stdinbuf[strlen(stdinbuf) - 1] = 0;
            selection = strtol(stdinbuf, &end, 10) - 1;

            memset(stdinbuf, '\0', sizeof stdinbuf);
            bufindex = 0;

            if (selection == -1) {
              sendInt(selection, sockfd);
              isvalid = 1;
            }

            // index validation
            else if (selection < -1 || selection >= player[clientpnum].hand.num_cards ||
                player[clientpnum].hand.cards[selection].class != INSTANT) {
              printf("Choose the neigh card to counter with, or enter 0 to skip: ");
            }

            else {
              sendInt(selection, sockfd);
              isvalid = 1;
            }
          }
        }

        if (pfd.revents & POLLIN) {
          int quitter;
          receiveInt(&quitter, sockfd);

          if (quitter == 1)
            break;
        }
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

    memset(stdinbuf, '\0', sizeof stdinbuf);
    bufindex = 0;

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
        closesocket(sockfd);
        _getch();
        exit(2);
      }
      if (ret2 == 0) {
        // this shouldn't time out because the client socket pipes are looking out for POLLOUT
        fprintf(stderr, "ERROR: client timed out. Error code : %d", WSAGetLastError());
        closesocket(sockfd);
        _getch();
        exit(2);
      }

      // skip the neigh input selection if the player already made a valid choice
      // or if the player played the last card in the cycle
      if (WaitForSingleObject(GetStdHandle(STD_INPUT_HANDLE), 0) == WAIT_OBJECT_0 && pnum != 0 && ((playerneighflag & 1) == 0)) {
        processStdin(stdinbuf, &bufindex);
        if (bufindex >= (sizeof(stdinbuf) - 1) || stdinbuf[bufindex - 1] == '\r' || stdinbuf[bufindex - 1] == '\n') {

          stdinbuf[strlen(stdinbuf) - 1] = 0;
          selection = strtol(stdinbuf, &end, 10) - 1;

          memset(stdinbuf, '\0', sizeof stdinbuf);
          bufindex = 0;

          if (selection == -1) {
            playerneighflag |= (1 << 0);
          }

          // index validation
          else if (selection < -1 || selection >= player[0].hand.num_cards ||
              player[0].hand.cards[selection].class != INSTANT) {
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

      if (playerneighflag == ((1 << current_players) - 1 - (1 << pnum))) {
        break;
      }
    }

    for (int i = 0; i < current_players - 1; i++) {
      // send it to every client that did not skip the polling process
      // this is sent in order to trigger the end of the loop
      if (canNeighOthers(i + 1)) {
        sendInt(TRUE, clientsockfd[i]);
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
