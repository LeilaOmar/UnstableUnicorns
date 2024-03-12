#pragma once

#include "gamemechanics.h"

/**
 * @brief Beginning Turn Phase: Activates all valid "beginning of turn" card effects
 * 
 * Beginning turn effects all trigger at once, so any cards that enter before
 * the action phase do not count towards these card effects 
 * (e.g. gained a card through unicorn lasso or puppicorn)
 */
void BeginningOfTurn(int pnum);

/**
 * @brief Action Phase: Allows the player to choose between playing or drawing a card
 * @return -1 No action took place, keep looping
 * @return  0 player[pnum] finished their Action phase
 * @note this loops externally to avoid blocking UI code during single-threaded debug mode tests
 */
int ActionPhase(int pnum);

/**
 * @brief End Turn Phase: Discards all of the extra cards, checks if player[pnum] won the game, and resolves all active effect chains
 * @return 0 Game continues to the next player's turn
 * @return 1 Game over, player[pnum] won
 */
int EndOfTurn(int pnum);
