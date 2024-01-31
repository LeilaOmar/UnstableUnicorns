#include "client.h"
#include "gamemechanics.h"
#include "gamephase.h"

#define ERRORBUF 256

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
  receiveInt(&nursery.size, sockfd);
  // recv(sockfd, (void*)nursery.cards, sizeof(struct Unicorn) * NURSERY_SIZE, 0);
  receiveUnicorns(nursery.cards, nursery.size, sockfd);

  rainbow("\n*******************"
          "\n*** Game Start! ***"
          "\n*******************\n\n");

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

      beginningOfTurn(counter);

      actionPhase(counter);

      int didWin = 0;
      didWin = endOfTurn(counter);
      if (didWin == 1)
        break;

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
  }

  char winmsg[DESC_SIZE];
  sprintf_s(winmsg, NAME_SIZE + 18, "%s won the game!!!\n", player[counter].username);
  rainbow(winmsg);
  getch();

	return 0;
}
