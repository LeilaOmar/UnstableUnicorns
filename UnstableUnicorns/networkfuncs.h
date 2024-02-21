#pragma once

#include <time.h>
#include "globals.h"

#define MSGBUF 1024
#define NEIGHTIME 4000 //!< 4000 ms

// ********************************************************************************
// ****************************** Lobby Code Hashing ******************************
// ********************************************************************************

/**
 * @brief Returns the external IP request to api.ipify.org
 *
 * https://stackoverflow.com/questions/39566240/how-to-get-the-external-ip-address-in-c
 * 
 * @param ip_address IPv4 address as a C string (e.g. "127.0.0.1")
 * @note Unfortunately the internetopen way didn't work :(
 */
void GetIPreq(char *ip_address);

/**
 * @brief Returns the local IP from sockaddr
 * 
 * https://stackoverflow.com/questions/19817425/how-to-get-ip-addresses-from-sockaddr
 * 
 * @param ip_address IPv4 address as a C string (e.g. "127.0.0.1")
 * @return 0 Success
 * @return 1 Fail
 */
int GetLocalIP(char *ip_address);

/**
 * @brief Uses the Hash() fn to return a number representation of the ip address
 * @param ip IPv4 address as a C string (e.g. "127.0.0.1")
 * @return hexcode (uint)Number rep. of the IP in host byte order
 */
unsigned int IPtoHexCode(char *ip);

/**
 * @brief Uses the Unhash() fn to decode a hex value into an IPv4 ip address
 * @param[in] code Hex string of IPv4 address (e.g. "142B8F0B")
 * @param[out] dest IPv4 address as a C string (e.g. "127.0.0.1")
 */
void HexCodeToIP(char *code, char *dest);

// ********************************************************************************
// *************************** Send/Recv Data Functions ***************************
// ********************************************************************************

/**
 * @brief Serializes and sends an integer to the given socket file descriptor
 * @param[in] num The int sent in network byte order
 */
int SendInt(int num, int fd);

/**
 * @brief Receives and deserializes an integer from the given socket file descriptor
 * @param[out] num The int saved in host byte order
 */
int ReceiveInt(int *num, int fd);

/**
 * @brief Sends the entire global player structure up to currentPlayers
 */
int SendPlayers(int fd);

/**
 * @brief Receives the entire global player structure up to currentPlayers
 */
int ReceivePlayers(int fd);

/**
 * @brief Sends size number of elements in the struct Unicorn array corns
 */
int SendUnicorns(struct Unicorn *corns, int size, int fd);

/**
 * @brief Receives size number of elements in the struct Unicorn array corns
 */
int ReceiveUnicorns(struct Unicorn *corns, int size, int fd);

// ********************************************************************************
// ******************************* Packet Functions *******************************
// ********************************************************************************

/**
 * @brief Sends the number of players, client player number, player usernames (partyMems), and selected Baby Unicorns (pselect)
 */
int SendLobbyPacket(int numPlayers, int clientpnum, int fd);

/**
 * @brief Receives the number of players, client player number, player usernames (partyMems), and selected Baby Unicorns (pselect)
 */
int ReceiveLobbyPacket(int *numPlayers, int *clientpnum, int fd);

/**
 * @brief Sends the Deck, Nursery, and Discard piles, the whole global player struct, and any misc. standalone card flags (e.g. puppicorn)
 */
int SendGamePacket(int fd);

/**
 * @brief Receives the Deck, Nursery, and Discard piles, the whole global player struct, and any misc. standalone card flags (e.g. puppicorn)
 */
int ReceiveGamePacket(int fd);

/**
 * @brief Sends data during Discard and Sacrifice events
 * @param targetPnum Target player number for the effect
 * @param desiredType Required card type
 */
int SendCardEffectPacket(int targetPnum, int desiredType, int fd);

/**
 * @brief Receives data during Discard and Sacrifice events
 * @param[out] targetPnum Target player number for the effect
 * @param[out] desiredType Required card type
 */
int ReceiveCardEffectPacket(int *targetPnum, int *desiredType, int fd);

/**
 * @brief Sends data during Enter Stable events
 * @param pnum (client) sends targetPnum; (server) sends origPnum
 */
int SendEnterStablePacket(struct Unicorn corn, int pnum, int fd);

/**
 * @brief Receives data during Enter Stable events
 * @param pnum (client) receives origPnum; (server) receives targetPnum
 */
int ReceiveEnterStablePacket(struct Unicorn* corn, int *pnum, int fd);

// ********************************************************************************
// **************************** Message/Input Functions ***************************
// ********************************************************************************

/**
 * @brief Sends the size and contents of the message string
 */
int SendMsg(char *str, int count, int fd);

/**
 * @brief Receives a C string message from the socket file descriptor
 */
int ReceiveMsg(char *str, int fd);

/**
 * @brief Handles stdin events from the console and filters everything but KeyDown presses
 * 
 * https://stackoverflow.com/questions/19955617/win32-read-from-stdin-with-timeout
 * 
 * @param[out] stdinBuf Char* buffer that contains the user's console input
 * @param[out] bufIndex Current index number/length of stdinBuf
 */
void ProcessStdin(char *stdinBuf, int *bufIndex);
