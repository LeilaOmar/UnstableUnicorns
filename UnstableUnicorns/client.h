#pragma once

#include "networkevents.h"

/**
 * @brief The main client loop for Console applications
 */
int ClientMain(void);

/**
 * @brief The main client loop for Windows applications
 * @param p Pointer rep of the port number
 * @note This executes in a separate thread from the UI code
 */
int ClientJoin(LPVOID p);
