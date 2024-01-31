#include <stdint.h>
#include "networkfuncs.h"
#include "gamemechanics.h"

#define BUFSIZE 200

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

void deserialize_short(unsigned char* buffer) {
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

int sendPlayer(struct Player p, int fd) {

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
    serialize_int(data + offset, corns[i].effect);
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
    corns[i].effect = deserialize_int(data + offset);
    offset += 2;
    corns[i].id = deserialize_int(data + offset);
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
