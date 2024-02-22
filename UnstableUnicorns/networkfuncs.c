#include <stdint.h>
#include "networkfuncs.h"
#include "gamemechanics.h"

// ********************************************************************************
// ****************************** Serialize Functions *****************************
// ********************************************************************************

/**
 * @brief Enforces Big Endian byte order by right shifting the integer value of num into 4 individual bytes and saving it into buffer
 * @param[out] buffer Unsigned char byte stream to send across a TCP socket
 * @param[in] num The number that's getting serialized
 * @note This assumes integers are stored as 4 bytes
 */
static void SerializeInt(unsigned char *buffer, int num) {
  buffer[0] = num >> 24;
  buffer[1] = num >> 16;
  buffer[2] = num >> 8;
  buffer[3] = num;
}

/**
 * @brief Left shifts the Big Endian-stored value of buffer into 4 individual bytes and stores it as an integer
 * @param[in] buffer Unsigned char byte stream that's getting processed and deserialized
 * @return num Value of the deserialized integer
 * @note This assumes integers are stored as 4 bytes
 */
static int DeserializeInt(unsigned char *buffer) {
  int num = 0;

  num |= buffer[0] << 24;
  num |= buffer[1] << 16;
  num |= buffer[2] << 8;
  num |= buffer[3];
  return num;
}

/**
 * @brief Enforces Big Endian byte order by shifting the short value of num into 2 individual bytes and saving it into buffer
 * @param[out] buffer Unsigned char byte stream to send across a TCP socket
 * @param[in] num The number that's getting serialized
 */
static void SerializeShort(unsigned char *buffer, int num) {
  buffer[0] = num >> 8;
  buffer[1] = num;
}

/**
 * @brief Left shifts the Big Endian-stored value of buffer into 2 individual bytes and stores it as a short
 * @param[in] buffer Unsigned char byte stream that's getting processed and deserialized
 * @return num Value of the deserialized short
 */
static short DeserializeShort(unsigned char *buffer) {
  int num = 0;

  num |= buffer[0] << 8;
  num |= buffer[1];
  return num;
}

// ********************************************************************************
// *************************** Send/Recv Data Functions ***************************
// ********************************************************************************

int SendInt(int num, int fd) {
  int32_t tmp = htonl(num);
  int count = sizeof(tmp);
  char *data = (char*)&tmp;
  int rc;

  do {
    rc = send(fd, data, count, 0);
    if (rc > 0) {
      data += rc;
      count -= rc;
    }
  } while (count > 0);

  return 0;
}

int ReceiveInt(int *num, int fd) {
  int32_t ret;
  int count = sizeof(ret);
  char *data = (char*)&ret;
  int rc;

  do {
    rc = recv(fd, data, count, 0);
    if (rc > 0) {
      data += rc;
      count -= rc;
    }
  } while (count > 0);

  *num = ntohl(ret);

  return 0;
}

int SendPlayers(int fd) {
  int count = sizeof(struct Player) * currentPlayers;
  int rc;

  static char data[sizeof(struct Player) * MAX_PLAYERS];
  int offset = 0;

  SendInt(currentPlayers, fd);

  for (int i = 0; i < currentPlayers; i++) {
    // cards in hand
    SendInt(player[i].hand.numCards, fd);
    SendUnicorns(player[i].hand.cards, player[i].hand.numCards, fd);

    // stable
    SendInt(player[i].stable.size, fd);
    SendInt(player[i].stable.numUnicorns, fd);
    SendUnicorns(player[i].stable.unicorns, player[i].stable.size, fd);

    // username
    for (int j = 0; j < sizeof player[i].username; j++) {
      data[offset++] = player[i].username[j];
    }

    // flags
    SerializeShort(data + offset, player[i].flags);
    offset += 2;
  }

  count = offset;
  SendInt(count, fd);
  offset = 0;
  do {
    rc = send(fd, data + offset, count, 0);
    if (rc > 0) {
      offset += rc;
      count -= rc;
    }
  } while (count > 0);

  return 0;
}

int ReceivePlayers(int fd) {
  int count;
  int rc;

  static char data[sizeof(struct Player) * MAX_PLAYERS];
  int offset = 0;

  ReceiveInt(&currentPlayers, fd);

  for (int i = 0; i < currentPlayers; i++) {
    // cards in hand
    ReceiveInt(&player[i].hand.numCards, fd);
    ReceiveUnicorns(player[i].hand.cards, player[i].hand.numCards, fd);

    // stable
    ReceiveInt(&player[i].stable.size, fd);
    ReceiveInt(&player[i].stable.numUnicorns, fd);
    ReceiveUnicorns(player[i].stable.unicorns, player[i].stable.size, fd);
  }

  ReceiveInt(&count, fd);
  while (count > 0) {
    rc = recv(fd, data + offset, count, 0);
    if (rc > 0) {
      offset += rc;
      count -= rc;
    }
  }

  offset = 0;

  for (int i = 0; i < currentPlayers; i++) {
    for (int j = 0; j < sizeof player[i].username; j++) {
      player[i].username[j] = data[offset++];
    }
    player[i].flags = DeserializeShort(data + offset);
    offset += 2;
  }

  return 0;
}

int SendUnicorns(struct Unicorn *corns, int size, int fd) {
  int count = sizeof(struct Unicorn) * size;
  int rc;

  static char data[sizeof(struct Unicorn) * (DECK_SIZE + NURSERY_SIZE)];
  int offset = 0;

  for (int i = 0; i < size; i++) {
    data[offset++] = corns[i].cType;
    data[offset++] = corns[i].species;
    for (int j = 0; j < sizeof corns[i].name; j++) {
      data[offset++] = corns[i].name[j];
    }
    for (int j = 0; j < sizeof corns[i].description; j++) {
      data[offset++] = corns[i].description[j];
    }
    SerializeShort(data + offset, corns[i].effect);
    offset += 2;
    SerializeInt(data + offset, corns[i].id);
    offset += 4;
  }

  // send the size of the array before sending the data
  SendInt(count, fd);

  offset = 0;
  do {
    rc = send(fd, data + offset, count, 0);
    if (rc > 0) {
      offset += rc;
      count -= rc;
    }
  } while (count > 0);

  return 0;
}

int ReceiveUnicorns(struct Unicorn *corns, int size, int fd) {
  int count, rc, offset = 0;
  static char data[sizeof(struct Unicorn) * (DECK_SIZE + NURSERY_SIZE)];
  ReceiveInt(&count, fd);

  while (count > 0) {
    rc = recv(fd, data + offset, count, 0);
    if (rc > 0) {
      offset += rc;
      count -= rc;
    }
  }

  offset = 0;
  for (int i = 0; i < size; i++) {
    corns[i].cType = data[offset++];
    corns[i].species = data[offset++];
    for (int j = 0; j < sizeof corns[i].name; j++) {
      corns[i].name[j] = data[offset++];
    }
    for (int j = 0; j < sizeof corns[i].description; j++) {
      corns[i].description[j] = data[offset++];
    }
    corns[i].effect = DeserializeShort(data + offset);
    offset += 2;
    corns[i].id = DeserializeInt(data + offset);
    offset += 4;
  }

  return 0;
}

// ********************************************************************************
// ******************************* Packet Functions *******************************
// ********************************************************************************

int SendGamePacket(int fd) {
  SendInt(deck.size, fd);
  SendUnicorns(deck.cards, deck.size, fd);

  SendInt(nursery.size, fd);
  SendUnicorns(nursery.cards, nursery.size, fd);

  SendInt(discardpile.size, fd);
  SendUnicorns(discardpile.cards, discardpile.size, fd);

  SendInt(uniLassoIndex[0], fd);
  SendInt(uniLassoIndex[1], fd);
  SendInt(uniLassoIndex[2], fd);

  SendInt(puppicornIndex[0], fd);
  SendInt(puppicornIndex[1], fd);

  SendPlayers(fd);

  return 0;
}

int ReceiveGamePacket(int fd) {
  ReceiveInt(&deck.size, fd);
  ReceiveUnicorns(deck.cards, deck.size, fd);

  ReceiveInt(&nursery.size, fd);
  ReceiveUnicorns(nursery.cards, nursery.size, fd);

  ReceiveInt(&discardpile.size, fd);
  ReceiveUnicorns(discardpile.cards, discardpile.size, fd);

  ReceiveInt(&uniLassoIndex[0], fd);
  ReceiveInt(&uniLassoIndex[1], fd);
  ReceiveInt(&uniLassoIndex[2], fd);

  ReceiveInt(&puppicornIndex[0], fd);
  ReceiveInt(&puppicornIndex[1], fd);

  ReceivePlayers(fd);

  return 0;
}

int SendCardEffectPacket(int targetPnum, int desiredType, int fd) {
  SendInt(targetPnum, fd);
  SendInt(desiredType, fd);
  SendGamePacket(fd);

  return 0;
}

int ReceiveCardEffectPacket(int *targetPnum, int *desiredType, int fd) {
  ReceiveInt(targetPnum, fd);
  ReceiveInt(desiredType, fd);
  ReceiveGamePacket(fd);

  return 0;
}

int SendEnterStablePacket(struct Unicorn corn, int pnum, int fd) {
  SendInt(pnum, fd);
  SendUnicorns(&corn, 1, fd);
  SendGamePacket(fd);

  return 0;
}

int ReceiveEnterStablePacket(struct Unicorn* corn, int *pnum, int fd) {
  ReceiveInt(pnum, fd);
  ReceiveUnicorns(corn, 1, fd);
  ReceiveGamePacket(fd);

  return 0;
}

// ********************************************************************************
// **************************** Message/Input Functions ***************************
// ********************************************************************************

int SendMsg(char *str, int count, int fd) {
  int rc;

  SendInt(count, fd);
  do {
    rc = send(fd, str, count, 0);
    if (rc > 0) {
      str += rc;
      count -= rc;
    }
  } while (count > 0);

  return 0;
}

int ReceiveMsg(char *str, int fd) {
  int count;
  int offset = 0;
  int rc;

  ReceiveInt(&count, fd);

  while (count > 0) {
    rc = recv(fd, str + offset, count, 0);
    if (rc > 0) {
      offset += rc;
      count -= rc;
    }
  }

  return 0;
}

void ProcessStdin(char *stdinBuf, int *bufIndex) {
  INPUT_RECORD record;
  DWORD numRead;

  // (bug fix \o/ ) stack corruption bug where the struct members were shifted by 2 bytes;
  // fix => change struct member alignment from 1 byte (what??) to default (i.e. usually 2 or 4)
  if (!ReadConsoleInput(GetStdHandle(STD_INPUT_HANDLE), &record, 1, &numRead)) {
    return;
  }

  if (record.EventType != KEY_EVENT) {
    // don't care about mouse move
    return;
  }

  if (!record.Event.KeyEvent.bKeyDown) {
    // only care about key down
    return;
  }

  // use printf \b when backspacing
  if (record.Event.KeyEvent.wVirtualKeyCode == VK_BACK) {
    if (*bufIndex > 0) {
      printf("\b \b");
      stdinBuf[*bufIndex] = '\0';
      (*bufIndex)--;
    }
    return;
  }

  // ascii input
  if (record.Event.KeyEvent.uChar.AsciiChar) {
    stdinBuf[*bufIndex] = record.Event.KeyEvent.uChar.AsciiChar;

    if (stdinBuf[*bufIndex] == '\r' || stdinBuf[*bufIndex] == '\n') {
      printf("\n");
    }
    else {
      printf("%c", stdinBuf[*bufIndex]);
    }
    (*bufIndex)++;
  }
}
