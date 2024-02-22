#pragma once

#include "basedeck.h"

// ********************************************************************************
// **************************** Print/Display Functions ***************************
// ********************************************************************************

void PrintPlayers(void);
void PrintPile(struct Deck d);
void PrintPileFilter(struct Deck d, int cType, int species);
void PrintHand(int pnum);
void PrintStable(int pnum);
void DisplayCardDesc(void);
void DisplayDesiredStable(void);
void PrintStableGrid(void); //!< prints stables of all players in two columns

// ********************************************************************************
// ******************************* Utility Functions ******************************
// ********************************************************************************

/**
 * @brief Connects to the server and verifies a valid username before fully connecting to the lobby
 * @param desiredType The required type for an action to take place
 * @param cardType The type of the player's card in question
 * @return FALSE The card's type does not equal the desired one
 * @return TRUE Card type matches
 */
int CheckType(int desiredType, int cardType);

/**
 * @brief Checks if player[pnum] satisfies the win condition given the number of unicorns and certain flags
 * @return FALSE The game goes on!
 * @return TRUE Game over, player[pnum] won
 */
int CheckWin(int pnum);

/**
 * @brief Shuffles the deck with the Fisher-Yates algorithm
 */
void ShuffleDeck(struct Deck *d);

/**
 * @brief Shuffles the Discard pile into the Deck, then shuffles the entire Deck
 */
void ShuffleDiscard(void);

/**
 * @brief Shuffles the indexed card out of the Deck, Nursery, or Discard pile's range
 */
void RearrangePile(struct Deck *d, int index);

/**
 * @brief Shuffles the indexed card out of the hand's range
 */
void RearrangeHand(int pnum, int index);

/**
 * @brief Shuffles the indexed card out of the stable's range
 */
void RearrangeStable(int pnum, int index);

/**
 * @brief Searches the Deck, Discard pile, or Nursery for a specific card that satisfies the conditional params
 * @param cType Required card type
 * @param species Required card species (e.g. "Narwhal")
 * @return TRUE Success
 * @return FALSE No available cards satisfying all conditions
 */
int SearchPile(int pnum, struct Deck *d, int cType, int species);

/**
 * @brief Adds the struct Unicorn card "corn" to the Nursery
 */
void AddNursery(struct Unicorn corn);

/**
 * @brief Adds the struct Unicorn card "corn" to the Discard pile
 */
void AddDiscard(struct Unicorn corn);

/**
 * @brief Adds the struct Unicorn card "corn" to player[pnum]'s stable and applies any toggle or "Enter Stable" effects
 */
void AddStable(int pnum, struct Unicorn corn);

/**
 * @brief Returns the card from player[pnum]'s Stable to their hand, or to the Nursery if it's a baby
 */
void ReturnCardToHand(int pnum, int cindex);

/**
 * @brief Utility tie-breaker func for games that end early
 * @return -1 Complete tie, nobody wins
 * @return [0-7] Winning player index
 */
int Tiebreaker(void);

// ********************************************************************************
// **************************** Boolean Check Functions ***************************
// ********************************************************************************

/**
 * @brief Checks for flags that make the player immune to Neigh cards
 * @return 0 False
 * @return 1 True
 */
int CanBeNeighed(int pnum);

/**
 * @brief Checks for flags that make the player unable to use Neigh cards
 * @return 0 False
 * @return 1 True
 */
int CanNeighOthers(int pnum);

/**
 * @brief Checks for all of the specific edge cases when destroying card
 * @param cType The type of cards that are allowed to be destroyed
 * @param isMagicCard Boolean check that's only relevant for Magical Kittencorn (as it cannot be destroyed by magic cards)
 * @return 0 False
 * @return 1 True
 */
int CanBeDestroyed(int pindex, int cindex, int cType, int isMagicCard);

/**
 * @brief Returns the number of available cards to destroy
 * @param cType The type of cards that are allowed to be destroyed
 * @param isMagicCard Boolean check that's only relevant for Magical Kittencorn (as it cannot be destroyed by magic cards)
 * @note This fn is only relevant for the card "Two-For-One," but it's used in many others; seems a bit wasteful tbh
 */
int CheckNumCardsToDestroy(int pnum, int cType, int isMagicCard);

/**
 * @brief Checks for all of the specific edge cases when sacrificing cards
 * @param cType The type of cards that are allowed to be destroyed
 * @return 0 False
 * @return 1 True
 */
int CanBeSacrificed(int pindex, int cindex, int cType);

// ********************************************************************************
// ************************** Basic Card Effect Functions *************************
// ********************************************************************************

/**
 * @return 0 Normal behavior; draw succeeded
 * @return 1 Deck ran out of cards; game over
 */
int Draw(int pnum, int numDrawn);

/**
 * @param cType The type of cards that are allowed to be discarded
 */
void Discard(int pnum, int numDiscard, int cType);

/**
 * @param cType The type of cards that are allowed to be sacrificed
 * @return 0 Fail; unable to sacrifice
 * @return 1 Success
 */
int Sacrifice(int pnum, int cType);

/**
 * @param cType The type of cards that are allowed to be destroyed
 * @param isMagicCard Boolean check that's only relevant for Magical Kittencorn (as it cannot be destroyed by magic cards)
 */
void Destroy(int pnum, int cType, int isMagicCard);

/**
 * @param cType The type of cards that are allowed to be stolen
 */
void Steal(int pnum, int cType);

// ********************************************************************************
// *************************** Core Game Loop Functions ***************************
// ********************************************************************************

/**
 * @brief Plays a card from player[pnum]'s hand during their turn
 */
void PlayCard(int pnum);

/**
 * @param winningPnum Value = [0-7]: The winning player number; Value = -1: tie
 */
void EndGame(int winningPnum);
