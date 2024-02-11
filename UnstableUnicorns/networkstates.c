#include "gamemechanics.h"
#include "networkevents.h"

// was unable to properly link this with an external file ;~;
#ifdef TEST_RUN
#define endGame endGame_mock

void endGame_mock(int winningpnum) {
  // blank function to avoid ending the tests prematurely
  return;
}
#endif

void init_network_states() {
  netStates[end_turn]           = (NetworkStateManager){ clientStateEndTurn,     serverStateEndTurn };
  netStates[end_game]           = (NetworkStateManager){ clientStateEndGame,     serverStateEndGame };
  netStates[quit_loop]          = (NetworkStateManager){ clientStateEndTurn,     serverStateQuitLoop };
  netStates[neigh_event]        = (NetworkStateManager){ clientStateNeigh,       serverStateNeigh };
  netStates[discard_event]      = (NetworkStateManager){ clientStateDiscard,     serverStateDiscard };
  netStates[sacrifice_event]    = (NetworkStateManager){ clientStateSacrifice,   serverStateSacrifice };
  netStates[enter_stable_event] = (NetworkStateManager){ clientStateEnterStable, serverStateEnterStable };
}

// ********************************************************************************
// ********************************* Client States ********************************
// ********************************************************************************

void clientSendEndGame(int winningpnum, int fd) {
  if (winningpnum == -1) {
    winningpnum = tiebreaker();
  }

  sendInt(end_game, fd);
  sendInt(winningpnum, fd);
  sendGamePacket(fd);

  endGame(winningpnum);
}

int clientStateEndTurn(int orig_pnum, int fd) {
  receiveGamePacket(fd);

  return 1;
}

int clientStateEndGame(int orig_pnum, int fd) {
  int winningpnum;

  receiveInt(&winningpnum, fd);
  receiveGamePacket(fd);
  endGame(winningpnum);

  return winningpnum;
}

int clientStateNeigh(int clientpnum, int fd) {
  int orig_pnum;
  int orig_cindex;

  receiveInt(&orig_pnum, fd);
  receiveInt(&orig_cindex, fd);
  clientNeigh(clientpnum, orig_pnum, &orig_cindex);

  return 0;
}

int clientStateDiscard(int clientpnum, int fd) {
  int target_player;
  int desired_type;

  receiveCardEffectPacket(&target_player, &desired_type, fd);
  clientDiscard(clientpnum, target_player, desired_type);

  return 0;
}

int clientStateSacrifice(int clientpnum, int fd) {
  int target_player;
  int desired_type;

  receiveCardEffectPacket(&target_player, &desired_type, fd);
  clientSacrifice(clientpnum, target_player, desired_type);

  return 0;
}

int clientStateEnterStable(int clientpnum, int fd) {
  struct Unicorn corn;
  int orig_pnum;

  receiveEnterStablePacket(&corn, &orig_pnum, sockfd);

  printf("\n\033[1;31m%s\033[0m sent you the card \033[1;31m'%s'\033[0m.\n", player[orig_pnum].username, corn.name);
  addStable(clientpnum, corn);

  if (!checkWin(clientpnum)) {
    sendInt(quit_loop, sockfd);
    sendGamePacket(sockfd);
  }
  else {
    clientSendEndGame(clientpnum, sockfd);
    return 1;
  }

  return 0;
}

// ********************************************************************************
// ********************************* Server States ********************************
// ********************************************************************************

void serverSendEndGame(int winningpnum) {
  // check if winningpnum = -1 or not
  if (winningpnum == -1) {
    winningpnum = tiebreaker();
  }

  for (int i = 0; i < current_players - 1; i++) {
    sendInt(end_game, clientsockfd[i]);
    sendInt(winningpnum, clientsockfd[i]);
    sendGamePacket(clientsockfd[i]);
  }

  endGame(winningpnum);
}

int serverStateEndTurn(int orig_pnum, int fd) {
  receiveGamePacket(fd);

  for (int i = 0; i < current_players - 1; i++) {
    if (orig_pnum == (i + 1)) continue;

    sendInt(end_turn, clientsockfd[i]);
    sendGamePacket(clientsockfd[i]);
  }

  return 1;
}

int serverStateEndGame(int orig_pnum, int fd) {
  int winningpnum;

  receiveInt(&winningpnum, fd);
  receiveGamePacket(fd);

  for (int i = 0; i < current_players - 1; i++) {
    if (orig_pnum == (i + 1)) continue;

    sendInt(end_game, clientsockfd[i]);
    sendInt(winningpnum, clientsockfd[i]);
    sendGamePacket(clientsockfd[i]);
  }
  endGame(winningpnum);

  return winningpnum;
}

int serverStateQuitLoop(int orig_pnum, int fd) {
  receiveGamePacket(fd);

  return 1;
}

int serverStateNeigh(int orig_pnum, int fd) {
  int cindex;

  receiveInt(&cindex, fd);
  receivePlayers(fd); // this is for updating the current player's hand after the beginning stable effects and drawing
  serverNeigh(orig_pnum, &cindex);

  return 0;
}

int serverStateDiscard(int orig_pnum, int fd) {
  int target_player;
  int desired_type;

  receiveCardEffectPacket(&target_player, &desired_type, fd);
  serverDiscard(orig_pnum, target_player, desired_type);

  return 0;
}

int serverStateSacrifice(int orig_pnum, int fd) {
  int target_player;
  int desired_type;

  receiveCardEffectPacket(&target_player, &desired_type, fd);
  serverSacrifice(orig_pnum, target_player, desired_type);

  return 0;
}

int serverStateEnterStable(int orig_pnum, int fd) {
  struct Unicorn corn;
  int target_player;

  receiveEnterStablePacket(&corn, &target_player, fd);

  if (target_player != 0) {
    sendInt(enter_stable_event, clientsockfd[target_player - 1]);
    sendEnterStablePacket(corn, orig_pnum, clientsockfd[target_player - 1]);
    serverEnterStable(orig_pnum, target_player);
  }
  else {
    printf("\n\033[1;31m%s\033[0m sent you the card \033[1;31m'%s'\033[0m.\n", player[orig_pnum].username, corn.name);
    addStable(0, corn);

    if (!checkWin(0)) {
      sendInt(quit_loop, fd);
      sendGamePacket(fd);
    }
    else {
      // winningpnum = 0 since that's the host
      serverSendEndGame(0);
    }
  }

  return 0;
}
