#pragma once

#include "networkevents.h"

/**
 * @brief The main server/host loop for Console applications
 */
int ServerMain(void);

/**
 * @brief The main server/host loop for Windows applications
 * @param p Pointer rep of the port number
 * @note This executes in a separate thread from the UI code
 */
int ServerHost(LPVOID p);
