#pragma once

enum NetworkEvents {
  PLAYER_JOIN = 1,      //!< A new player joined the lobby
  PLAYER_DISCONNECT,    //!< Someone left, so the player struct needs to be adjusted
  INCOMING_MSG,         //!< Incoming player chat msg or game text
  START_GAME,           //!< Leave the lobby and start the game
  END_TURN,             //!< End the turn and continue to the next player
  NEIGH_EVENT,          //!< Neigh chain loop
  DISCARD_EVENT,        //!< Choose a card to discard
  SACRIFICE_EVENT,      //!< Choose a card to sacrifice
  DESTROY_EVENT,        //!< Trigger a "sacrificeDestroy" effect
  ENTER_STABLE_EVENT,   //!< Trigger an "enter stable" effect
  QUIT_LOOP,            //!< Quit the event loop
  END_GAME,             //!< End the game
  NUM_NETWORK_EVENTS    //!< Total size
};

typedef struct {
  /**
   * @brief Runs the Game State logic of an event to do [State] action outside of the client's normal turn
   * @param pnum Situationally acts as either the target player number or the original player number
   */
  int (*RecvClient)(int pnum, int fd);

  /**
   * @brief Runs the Game State logic of an event to do [State] action outside of the server/host's normal turn
   * @param pnum Situationally acts as either the target player number or the original player number
   */
  int (*RecvServer)(int pnum, int fd);
} NetworkStateManager;

NetworkStateManager netStates[NUM_NETWORK_EVENTS];

/**
 * @brief Initializes the network state machine manager "netStates"
 */
void InitNetworkStates(void);

void ClientSendEndGame(int winningPnum, int fd);
int ClientStateEndTurn(int origPnum, int fd);
int ClientStateEndGame(int origPnum, int fd);
int ClientStateNeigh(int origPnum, int fd);
int ClientStateDiscard(int origPnum, int fd);
int ClientStateSacrifice(int origPnum, int fd);
int ClientStateDestroy(int origPnum, int fd);
int ClientStateEnterStable(int origPnum, int fd);

void ServerSendEndGame(int winningPnum);
int ServerStateEndTurn(int origPnum, int fd);
int ServerStateEndGame(int origPnum, int fd);
int ServerStateQuitLoop(int origPnum, int fd);
int ServerStateNeigh(int origPnum, int fd);
int ServerStateDiscard(int origPnum, int fd);
int ServerStateSacrifice(int origPnum, int fd);
int ServerStateDestroy(int origPnum, int fd);
int ServerStateEnterStable(int origPnum, int fd);
