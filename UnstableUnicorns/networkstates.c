#include "gamemechanics.h"
#include "networkevents.h"

// was unable to properly link this with an external file ;~;
#ifdef TEST_RUN
#define EndGame EndGame_mock

void EndGame_mock(int winningPnum) {
  // blank function to avoid ending the tests prematurely
  return;
}
#endif

void InitNetworkStates(void) {
  netStates[END_TURN]           = (NetworkStateManager){ ClientStateEndTurn,     ServerStateEndTurn };
  netStates[END_GAME]           = (NetworkStateManager){ ClientStateEndGame,     ServerStateEndGame };
  netStates[QUIT_LOOP]          = (NetworkStateManager){ ClientStateEndTurn,     ServerStateQuitLoop };
  netStates[NEIGH_EVENT]        = (NetworkStateManager){ ClientStateNeigh,       ServerStateNeigh };
  netStates[DISCARD_EVENT]      = (NetworkStateManager){ ClientStateDiscard,     ServerStateDiscard };
  netStates[SACRIFICE_EVENT]    = (NetworkStateManager){ ClientStateSacrifice,   ServerStateSacrifice };
  netStates[DESTROY_EVENT]      = (NetworkStateManager){ ClientStateDestroy,     ServerStateDestroy };
  netStates[ENTER_STABLE_EVENT] = (NetworkStateManager){ ClientStateEnterStable, ServerStateEnterStable };
}

// ********************************************************************************
// ********************************* Client States ********************************
// ********************************************************************************

void ClientSendEndGame(int winningPnum, int fd) {
  if (winningPnum == -1) {
    winningPnum = Tiebreaker();
  }

  SendInt(END_GAME, fd);
  SendInt(winningPnum, fd);
  SendGamePacket(fd);

  EndGame(winningPnum);
}

int ClientStateEndTurn(int origPnum, int fd) {
  ReceiveGamePacket(fd);

  return 1;
}

int ClientStateEndGame(int origPnum, int fd) {
  int winningPnum;

  ReceiveInt(&winningPnum, fd);
  ReceiveGamePacket(fd);
  EndGame(winningPnum);

  return winningPnum;
}

int ClientStateNeigh(int clientpnum, int fd) {
  int origPnum;
  int origCindex;

  ReceiveInt(&origPnum, fd);
  ReceiveInt(&origCindex, fd);
  ClientNeigh(clientpnum, origPnum, &origCindex);

  return 0;
}

int ClientStateDiscard(int clientpnum, int fd) {
  int target_player;
  int desiredType;

  ReceiveCardEffectPacket(&target_player, &desiredType, fd);
  ClientDiscard(clientpnum, target_player, desiredType);

  return 0;
}

int ClientStateSacrifice(int clientpnum, int fd) {
  int target_player;
  int desiredType;

  ReceiveCardEffectPacket(&target_player, &desiredType, fd);
  ClientSacrifice(clientpnum, target_player, desiredType);

  return 0;
}

// this is specifically for clients who's cards are getting destroyed
// so they can enter any necessary info in the sacrificeDestroyEffects function
int ClientStateDestroy(int clientpnum, int fd) {
  int origPnum;
  int cindex;

  ReceiveCardEffectPacket(&origPnum, &cindex, fd);

  printf("\n\033[1;31m%s\033[0m wants to destroy your card \033[1;31m'%s'\033[0m.\n",
    player[origPnum].username, player[clientpnum].stable.unicorns[cindex].name);

  Base_SacrificeDestroyEffects(clientpnum, cindex, player[clientpnum].stable.unicorns[cindex].effect);
  SendInt(QUIT_LOOP, fd);
  SendGamePacket(fd);

  return 0;
}

int ClientStateEnterStable(int clientpnum, int fd) {
  struct Unicorn corn;
  int origPnum;

  ReceiveEnterStablePacket(&corn, &origPnum, fd);

  printf("\n\033[1;31m%s\033[0m sent you the card \033[1;31m'%s'\033[0m.\n", player[origPnum].username, corn.name);
  AddStable(clientpnum, corn);

  if (!CheckWin(clientpnum)) {
    SendInt(QUIT_LOOP, fd);
    SendGamePacket(fd);
  }
  else {
    ClientSendEndGame(clientpnum, fd);
    return 1;
  }

  return 0;
}

// ********************************************************************************
// ********************************* Server States ********************************
// ********************************************************************************

void ServerSendEndGame(int winningPnum) {
  // check if winningPnum = -1 or not
  if (winningPnum == -1) {
    winningPnum = Tiebreaker();
  }

  for (int i = 0; i < currentPlayers - 1; i++) {
    SendInt(END_GAME, clientsockfd[i]);
    SendInt(winningPnum, clientsockfd[i]);
    SendGamePacket(clientsockfd[i]);
  }

  EndGame(winningPnum);
}

int ServerStateEndTurn(int origPnum, int fd) {
  ReceiveGamePacket(fd);

  for (int i = 0; i < currentPlayers - 1; i++) {
    if (origPnum == (i + 1)) continue;

    SendInt(END_TURN, clientsockfd[i]);
    SendGamePacket(clientsockfd[i]);
  }

  return 1;
}

int ServerStateEndGame(int origPnum, int fd) {
  int winningPnum;

  ReceiveInt(&winningPnum, fd);
  ReceiveGamePacket(fd);

  for (int i = 0; i < currentPlayers - 1; i++) {
    if (origPnum == (i + 1)) continue;

    SendInt(END_GAME, clientsockfd[i]);
    SendInt(winningPnum, clientsockfd[i]);
    SendGamePacket(clientsockfd[i]);
  }
  EndGame(winningPnum);

  return winningPnum;
}

int ServerStateQuitLoop(int origPnum, int fd) {
  ReceiveGamePacket(fd);

  return 1;
}

int ServerStateNeigh(int origPnum, int fd) {
  int cindex;

  ReceiveInt(&cindex, fd);
  ReceivePlayers(fd); // this is for updating the current player's hand after the beginning stable effects and drawing
  ServerNeigh(origPnum, &cindex);

  return 0;
}

int ServerStateDiscard(int origPnum, int fd) {
  int target_player;
  int desiredType;

  ReceiveCardEffectPacket(&target_player, &desiredType, fd);
  ServerDiscard(origPnum, target_player, desiredType);

  return 0;
}

int ServerStateSacrifice(int origPnum, int fd) {
  int target_player;
  int desiredType;

  ReceiveCardEffectPacket(&target_player, &desiredType, fd);
  ServerSacrifice(origPnum, target_player, desiredType);

  return 0;
}

// this is specifically for when the host's cards are getting destroyed
// so they can enter any necessary info in the sacrificeDestroyEffects function
int ServerStateDestroy(int origPnum, int fd) {
  int target_player;
  int cindex;

  ReceiveCardEffectPacket(&target_player, &cindex, fd);

  if (target_player != 0) {
    SendInt(DESTROY_EVENT, clientsockfd[target_player - 1]);
    SendCardEffectPacket(target_player, cindex, clientsockfd[target_player - 1]);
    ServerEnterLeaveStable(origPnum, target_player);
  }
  else {
    printf("\n\033[1;31m%s\033[0m wants to destroy your card \033[1;31m'%s'\033[0m.\n",
      player[origPnum].username, player[0].stable.unicorns[cindex].name);

    Base_SacrificeDestroyEffects(0, cindex, player[0].stable.unicorns[cindex].effect);

    SendInt(QUIT_LOOP, fd);
    SendGamePacket(fd);
  }

  return 0;
}

int ServerStateEnterStable(int origPnum, int fd) {
  struct Unicorn corn;
  int target_player;

  ReceiveEnterStablePacket(&corn, &target_player, fd);

  if (target_player != 0) {
    SendInt(ENTER_STABLE_EVENT, clientsockfd[target_player - 1]);
    SendEnterStablePacket(corn, origPnum, clientsockfd[target_player - 1]);
    ServerEnterLeaveStable(origPnum, target_player);
  }
  else {
    printf("\n\033[1;31m%s\033[0m sent you the card \033[1;31m'%s'\033[0m.\n", player[origPnum].username, corn.name);
    AddStable(0, corn);

    if (!CheckWin(0)) {
      SendInt(QUIT_LOOP, fd);
      SendGamePacket(fd);
    }
    else {
      // winningPnum = 0 since that's the host
      ServerSendEndGame(0);
    }
  }

  return 0;
}
