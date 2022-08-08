#include <stdint.h>
#include "networkfuncs.h"
#include "gamemechanics.h"
#include "windowsapp.h"

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

// sends the following info, * denotes global variables:
// 1. number of current players
// 2. client player number (unique to csockfd)
// 3. *partymems (i.e. the player username list)
// 4. *pselect (notes the corner of every player's selected unicorn; 0 means nothing selected)
int sendLobbyPacket(int num_players, int clientpnum, int fd) {
  int count = sizeof(int) + sizeof(int) + strlen(partymems) + (2 * sizeof(long) * MAX_PLAYERS);
  int rc;
  static char data[512];
  int offset = 8;
  serialize_int(data, num_players);
  serialize_int(data + 4, clientpnum);
  for (int i = 0; i < strlen(partymems); i++) {
    //serialize_char(data + offset, partymems[i]);
    data[offset] = partymems[i];
    offset++;
  }
  for (int i = 0; i < MAX_PLAYERS; i++) {
    serialize_int(data + offset, (int)pselect[i].left);
    offset += 4;
    serialize_int(data + offset, (int)pselect[i].top);
    offset += 4;
  }

  // send data count size before sending data packet
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

// receives the following info, * denotes global variables:
// 1. number of current players
// 2. client player number (unique to csockfd)
// 3. *partymems (i.e. the player username list)
// 4. *pselect (notes the corner of every player's selected unicorn; 0 means nothing selected)
int receiveLobbyPacket(int *num_players, int *clientpnum, int fd) {
  int count, rc, offset = 0;
  static char data[512];
  receiveInt(&count, fd);

  while (count > 0) {
    rc = recv(fd, data + offset, count, 0);
    if (rc >= 0) {
      offset += rc;
      count -= rc;
    }
  }

  count = offset;
  offset = 8;
  *num_players = deserialize_int(data);
  *clientpnum = deserialize_int(data + 4);
  int strcount = count - (2 * sizeof(int)) - (2 * sizeof(long) * MAX_PLAYERS);

  for (int i = 0; i < strcount; i++) {
    partymems[i] = data[offset];
    offset++;
  }
  partymems[strcount] = '\0';

  for (int i = 0; i < MAX_PLAYERS; i++) {
    (int)pselect[i].left = deserialize_int(data + offset);
    offset += 4;
    (int)pselect[i].top = deserialize_int(data + offset);
    offset += 4;
  }

  return 0;
}

int sendNurseryInfo(struct nurseryPacket info, int fd) {
  int32_t tmp_index = htonl(info.index);
  int32_t tmp_size = htonl(info.size);
  int count = sizeof(tmp_index) + sizeof(tmp_size) + sizeof(info.ref);
  return 0;
}

int receiveNurseryInfo(struct nurseryPacket info, int fd) {
  return 0;
}

int sendDiscardInfo(struct discardPacket info, int fd) {
  return 0;
}

int receiveDiscardInfo(struct discardPacket info, int fd) {
  return 0;
}

int sendDeckInfo(struct deckPacket info, int fd) {
  return 0;
}

int receiveDeckInfo(struct deckPacket info, int fd) {
  return 0;
}
