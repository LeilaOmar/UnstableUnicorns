#include <stdint.h>
#include "networkfuncs.h"
#include "gamemechanics.h"

void serialize_int(unsigned char* buffer, int num) {
  buffer[0] = num >> 24;
  buffer[1] = num >> 16;
  buffer[2] = num >> 8;
  buffer[3] = num;
}

int deserialize_int(unsigned char* buffer) {
  int num = 0;

  num |= buffer[0] << 24;
  num |= buffer[1] << 16;
  num |= buffer[2] << 8;
  num |= buffer[3];
  return num;
}

void serialize_short(unsigned char* buffer, int num) {
  buffer[0] = num >> 8;
  buffer[1] = num;
}

short deserialize_short(unsigned char* buffer) {
  int num = 0;

  num |= buffer[0] << 8;
  num |= buffer[1];
  return num;
}

// unsigned char* serialize_long(unsigned char* buffer, int num) {
//   buffer[0] = num >> 56;
//   buffer[1] = num >> 48;
//   buffer[2] = num >> 40;
//   buffer[3] = num >> 32;
//   buffer[4] = num >> 24;
//   buffer[5] = num >> 16;
//   buffer[6] = num >> 8;
//   buffer[7] = num;
//   return buffer + 8;
// }
// 
// long deserialize_long(unsigned char* buffer) {
//   long num = 0;
// 
//   num |= buffer[0] << 56;
//   num |= buffer[1] << 48;
//   num |= buffer[2] << 40;
//   num |= buffer[3] << 32;
//   num |= buffer[4] << 24;
//   num |= buffer[5] << 16;
//   num |= buffer[6] << 8;
//   num |= buffer[7];
//   return num;
// }

int sendInt(int num, int fd) {
  int32_t tmp = htonl(num);
  int count = sizeof(tmp);
  char* data = (char*)&tmp;
  int rc;

  do {
    rc = send(fd, data, count, 0);
    if (rc >= 0) {
      data += rc;
      count -= rc;
    }
  } while (count > 0);

  return 0;
}

int receiveInt(int* num, int fd) {
  int32_t ret;
  int count = sizeof(ret);
  char* data = (char*)&ret;
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

int sendPlayers(int fd) {
  int count = sizeof(struct Player) * current_players;
  int rc;

  static char data[sizeof(struct Player) * MAX_PLAYERS];
  int offset = 0;

  sendInt(current_players, fd);

  for (int i = 0; i < current_players; i++) {
    // cards in hand
    sendInt(player[i].hand.num_cards, fd);
    sendUnicorns(player[i].hand.cards, player[i].hand.num_cards, fd);

    // stable
    sendInt(player[i].stable.size, fd);
    sendInt(player[i].stable.num_unicorns, fd);
    sendUnicorns(player[i].stable.unicorns, player[i].stable.size, fd);

    // username
    for (int j = 0; j < sizeof player[i].username; j++) {
      data[offset++] = player[i].username[j];
    }

    // flags
    serialize_short(data + offset, player[i].flags);
    offset += 2;
  }

  count = offset;
  sendInt(count, fd);
  offset = 0;
  do {
    rc = send(fd, data + offset, count, 0);
    if (rc >= 0) {
      offset += rc;
      count -= rc;
    }
  } while (count > 0);

  return 0;
}

int receivePlayers(int fd) {
  int count;
  int rc;

  static char data[sizeof(struct Player) * MAX_PLAYERS];
  int offset = 0;

  receiveInt(&current_players, fd);

  for (int i = 0; i < current_players; i++) {
    // cards in hand
    receiveInt(&player[i].hand.num_cards, fd);
    receiveUnicorns(player[i].hand.cards, player[i].hand.num_cards, fd);

    // stable
    receiveInt(&player[i].stable.size, fd);
    receiveInt(&player[i].stable.num_unicorns, fd);
    receiveUnicorns(player[i].stable.unicorns, player[i].stable.size, fd);
  }

  receiveInt(&count, fd);
  while (count > 0) {
    rc = recv(fd, data + offset, count, 0);
    if (rc >= 0) {
      offset += rc;
      count -= rc;
    }
  }

  offset = 0;

  for (int i = 0; i < current_players; i++) {
    for (int j = 0; j < sizeof player[i].username; j++) {
      player[i].username[j] = data[offset++];
    }
    player[i].flags = deserialize_short(data + offset);
    offset += 2;
  }

  return 0;
}

int sendUnicorns(struct Unicorn* corns, int size, int fd) {
  int count = sizeof(struct Unicorn) * size;
  int rc;

  static char data[sizeof(struct Unicorn) * (DECK_SIZE + NURSERY_SIZE)];
  int offset = 0;

  for (int i = 0; i < size; i++) {
    data[offset++] = corns[i].class;
    data[offset++] = corns[i].species;
    for (int j = 0; j < sizeof corns[i].name; j++) {
      data[offset++] = corns[i].name[j];
    }
    for (int j = 0; j < sizeof corns[i].description; j++) {
      data[offset++] = corns[i].description[j];
    }
    serialize_short(data + offset, corns[i].effect);
    offset += 2;
    serialize_int(data + offset, corns[i].id);
    offset += 4;
  }

  // send the size of the array before sending the data
  sendInt(count, fd);

  offset = 0;
  do {
    rc = send(fd, data + offset, count, 0);
    if (rc >= 0) {
      offset += rc;
      count -= rc;
    }
  } while (count > 0);

  return 0;
}

int receiveUnicorns(struct Unicorn* corns, int size, int fd) {
  int count, rc, offset = 0;
  static char data[sizeof(struct Unicorn) * (DECK_SIZE + NURSERY_SIZE)];
  receiveInt(&count, fd);

  while (count > 0) {
    rc = recv(fd, data + offset, count, 0);
    if (rc >= 0) {
      offset += rc;
      count -= rc;
    }
  }

  offset = 0;
  for (int i = 0; i < size; i++) {
    corns[i].class = data[offset++];
    corns[i].species = data[offset++];
    for (int j = 0; j < sizeof corns[i].name; j++) {
      corns[i].name[j] = data[offset++];
    }
    for (int j = 0; j < sizeof corns[i].description; j++) {
      corns[i].description[j] = data[offset++];
    }
    corns[i].effect = deserialize_short(data + offset);
    offset += 2;
    corns[i].id = deserialize_int(data + offset);
    offset += 4;
  }

  return 0;
}

int sendGamePacket(int fd) {
  sendInt(deck.size, fd);
  sendUnicorns(deck.cards, deck.size, fd);

  sendInt(nursery.size, fd);
  sendUnicorns(nursery.cards, nursery.size, fd);

  sendInt(discardpile.size, fd);
  sendUnicorns(discardpile.cards, discardpile.size, fd);

  sendInt(puppicorn_index[0], fd);
  sendInt(puppicorn_index[1], fd);

  sendPlayers(fd);

  return 0;
}

int receiveGamePacket(int fd) {
  receiveInt(&deck.size, fd);
  receiveUnicorns(deck.cards, deck.size, fd);

  receiveInt(&nursery.size, fd);
  receiveUnicorns(nursery.cards, nursery.size, fd);

  receiveInt(&discardpile.size, fd);
  receiveUnicorns(discardpile.cards, discardpile.size, fd);

  receiveInt(&puppicorn_index[0], fd);
  receiveInt(&puppicorn_index[1], fd);

  receivePlayers(fd);

  return 0;
}

void receiveMsg(char* str, int count, int fd) {
  int pnum;
  int offset = 0;
  int rc;

  receiveInt(&pnum, fd);

  while (count > 0) {
    rc = recv(fd, str + offset, count, 0);
    if (rc >= 0) {
      offset += rc;
      count -= rc;
    }
  }

  red();
  printf("%s: ", player[pnum].username);
  reset_col();
  printf("%s\n", str);
}

// handles stdin events and filters everything but keydown presses
// https://stackoverflow.com/questions/19955617/win32-read-from-stdin-with-timeout
void processStdin(char* stdinbuf, int* bufindex) {
  INPUT_RECORD record[8];
  DWORD numRead;

  // TODO: (bug) try and analyze this another day
  // stack corruption occurs if record is only a singular static struct;
  // this is related to the wVirtualKeyCode issue :/
  if (!ReadConsoleInput(GetStdHandle(STD_INPUT_HANDLE), record, 1, &numRead)) {
    return;
  }

  if (record->EventType != KEY_EVENT) {
    // don't care about mouse move
    return;
  }

  if (!record->Event.KeyEvent.bKeyDown) {
    // only care about key down
    return;
  }
  
  if (record->Event.KeyEvent.wVirtualKeyCode == VK_LBUTTON) {
    // for whatever reason, record's struct members get displaced/shifted by 1
    // and this ONLY happens in the main program, not the test one
    record->Event.KeyEvent.wVirtualKeyCode = record->Event.KeyEvent.wVirtualScanCode;
    record[0].Event.KeyEvent.uChar.AsciiChar = (char)record[0].Event.KeyEvent.dwControlKeyState;
  }

  // use printf \b when backspacing
  if (record->Event.KeyEvent.wVirtualKeyCode == VK_BACK) {
    if (*bufindex > 0) {
      printf("\b \b");
      stdinbuf[*bufindex] = '\0';
      (*bufindex)--;
    }
    return;
  }

  // ascii input
  if (record->Event.KeyEvent.uChar.AsciiChar) {
    stdinbuf[*bufindex] = record[0].Event.KeyEvent.uChar.AsciiChar;

    if (stdinbuf[*bufindex] == '\r' || stdinbuf[*bufindex] == '\n') {
      printf("\n");
    }
    else {
      printf("%c", stdinbuf[*bufindex]);
    }
    (*bufindex)++;
  }
}
