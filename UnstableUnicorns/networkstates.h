#pragma once

enum NetworkEvents { player_join = 1, incoming_msg, start_game, end_turn, neigh_event, discard_event, sacrifice_event, enter_stable_event, quit_loop, end_game, num_network_events };

typedef struct {
  int (*recvClient)(int pnum, int fd);
  int (*recvServer)(int pnum, int fd);
} NetworkStateManager;

NetworkStateManager netStates[num_network_events];

void init_network_states();

void clientSendEndGame(int winningpnum, int fd);
int clientStateEndTurn(int orig_pnum, int fd);
int clientStateEndGame(int orig_pnum, int fd);
int clientStateNeigh(int orig_pnum, int fd);
int clientStateDiscard(int orig_pnum, int fd);
int clientStateSacrifice(int orig_pnum, int fd);
int clientStateEnterStable(int orig_pnum, int fd);

void serverSendEndGame(int winningpnum);
int serverStateEndTurn(int orig_pnum, int fd);
int serverStateEndGame(int orig_pnum, int fd);
int serverStateQuitLoop(int orig_pnum, int fd);
int serverStateNeigh(int orig_pnum, int fd);
int serverStateDiscard(int orig_pnum, int fd);
int serverStateSacrifice(int orig_pnum, int fd);
int serverStateEnterStable(int orig_pnum, int fd);
