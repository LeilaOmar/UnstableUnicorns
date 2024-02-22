#pragma once

#include "networkfuncs.h"
#include "networkstates.h"

/**
 * @brief Client: Loops Neigh card chains whenever player[origPnum] plays a card during their Action phase
 * @param[in] clientpnum The user/client's own player index
 * @param[in] origPnum The player who played the original card
 * @param[out] origCindex The card index in player[origPnum]'s hand; this may get updated after rearranging their hand
 * @return 0 Neigh(s) did not go through; card is in play
 * @return 1 Neigh(s) succeeded; card went to the Discard pile
 */
int ClientNeigh(int clientpnum, int origPnum, int *origCindex);

/**
 * @brief Server: Loops Neigh card chains whenever player[origPnum] plays a card during their Action phase
 * @param[in] origPnum The player who played the original card
 * @param[out] origCindex The card index in player[origPnum]'s hand; this may get updated after rearranging their hand
 * @return 0 Neigh(s) did not go through; card is in play
 * @return 1 Neigh(s) succeeded; card went to the Discard pile
 */
int ServerNeigh(int origPnum, int *origCindex);

/**
 * @brief Client: Handles discard card effect events outside of another player's normal turn
 * @param clientpnum The user/client's own player index
 * @param targetPnum The player target index who must discard a card; ANY (-1) = everyone
 * @param cType The type of cards that are allowed to be discarded
 */
void ClientDiscard(int clientpnum, int targetPnum, int cType);

/**
 * @brief Server: Handles discard card effect events outside of another player's normal turn
 * @param origPnum The player who initiated the event
 * @param targetPnum The player target index who must discard a card; ANY (-1) = everyone
 * @param cType The type of cards that are allowed to be discarded
 */
void ServerDiscard(int origPnum, int targetPnum, int cType);

/**
 * @brief Client: Handles sacrifice card effect events outside of another player's normal turn
 * @param clientpnum The user/client's own player index
 * @param targetPnum The player target index who must discard a card; ANY (-1) = everyone
 * @param cType The type of cards that are allowed to be sacrificed
 */
void ClientSacrifice(int clientpnum, int targetPnum, int cType);

/**
 * @brief Server: Handles sacrifice card effect events outside of another player's normal turn
 * @param origPnum The player who initiated the event
 * @param targetPnum The player target index who must sacrifice a card; ANY (-1) = everyone
 * @param cType The type of cards that are allowed to be sacrificed
 */
void ServerSacrifice(int origPnum, int targetPnum, int cType);

/**
 * @brief Client: Idle function that handles nested network events upon entering/leaving a stable (sans returning to the hand)
 * @param clientpnum The user/client's own player index
 * @note This essentially works for both "enter stable" effects and unicorn "SacrificeDestroy" effects
 * @note This only runs whenever the client initiated the event, and exists solely for polling other events
 */
void ClientEnterLeaveStable(int clientpnum);

/**
 * @brief Server: Idle function that handles nested network events upon entering/leaving a stable (sans returning to the hand)
 * @param origPnum The player who initiated the event
 * @param targetPnum The player target index who has to act on the triggered event
 * @note This essentially works for both "enter stable" effects and unicorn "SacrificeDestroy" effects
 */
void ServerEnterLeaveStable(int origPnum, int targetPnum);
