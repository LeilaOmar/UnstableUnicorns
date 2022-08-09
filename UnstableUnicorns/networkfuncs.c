#include <stdint.h>
#include "networkfuncs.h"
#include "gamemechanics.h"
#include "windowsapp.h"

#define BUFSIZE 200

// hash functions for scrambling IP addresses
// https://stackoverflow.com/questions/664014/what-integer-hash-function-are-good-that-accepts-an-integer-hash-key
unsigned int hash(unsigned int x) {
  x = ((x >> 16) ^ x) * 0x45d9f3b;
  x = ((x >> 16) ^ x) * 0x45d9f3b;
  x = (x >> 16) ^ x;
  return x;
}
unsigned int unhash(unsigned int x) {
  x = ((x >> 16) ^ x) * 0x119de1f3;
  x = ((x >> 16) ^ x) * 0x119de1f3;
  x = (x >> 16) ^ x;
  return x;
}

// IP stuff
// https://stackoverflow.com/questions/39566240/how-to-get-the-external-ip-address-in-c
// unfortunately the internetopen way didn't work :(
void getIPreq(char* ip_address) {
  //WSADATA wsaData;
  struct addrinfo hints;
  struct addrinfo* result = NULL;
  int sd, err;
  char buffer[BUFSIZE] = { 0 };

  // api.ipify.org is only for IPv4; api64.ipify.org is both v4 and v6 w/ priority for v6
  // **don't forget to change it in getaddrinfo too if ever**
  char* httpreq = "GET / HTTP/1.1\r\nHost: api.ipify.org\r\nConnection: close\r\n\r\n";

  //if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
  //  // error handling
  //  return;
  //  //cout << "WSAStartup failed.\n";
  //  //system("pause");
  //}

  ZeroMemory(&hints, sizeof(hints));
  hints.ai_family = AF_UNSPEC; // allows for both ipv4 and ipv6
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;
  err = getaddrinfo("api.ipify.org", "80", &hints, &result);
  if (err != 0)
  {
    // error handling
    abort();
    //fprintf(stderr, "%s: %s\n", url, gai_strerror(err));
  }


  for (struct addrinfo* addr = result; addr != NULL; addr = addr->ai_next)
  {
    sd = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
    if (sd == -1)
    {
      err = errno;
      continue; // if using AF_UNSPEC above instead of AF_INET/6 specifically,
                // replace this 'break' with 'continue' instead, as the 'ai_family'
                // may be different on the next iteration...
    }

    if (connect(sd, addr->ai_addr, addr->ai_addrlen) == 0)
      break;

    err = errno;
  }

  send(sd, httpreq, strlen(httpreq), 0);
  recv(sd, buffer, BUFSIZE, 0);

  freeaddrinfo(result);
  closesocket(sd);
  //WSACleanup();

  // this only works because the text is the only output from the site
  // therefore it's the last line and does not have a newline after it
  int index = 0;
  for (int i = BUFSIZE - 1; i >= 0; i--) {
    if (buffer[i] == '\n') {
      index = i + 1;
      break;
    }
  }

  // using sizeof(ip) for MaxCount may not be appropriate since the length is variable
  // it still works though since everything afterwards is \0 *shrugs in laziness*
  strncpy_s(ip_address, 16, buffer + index, 16);
}

// https://stackoverflow.com/questions/19817425/how-to-get-ip-addresses-from-sockaddr
int getLocalIP(char* ip_address)
{
  char szBuffer[64];

  if (gethostname(szBuffer, sizeof(szBuffer)) == SOCKET_ERROR)
  {
    return 1;
  }

  int sd, err;
  struct addrinfo hints, * addrs;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  // hints.ai_socktype = SOCK_DGRAM;
  // hints.ai_protocol = IPPROTO_UDP;
  err = getaddrinfo(szBuffer, NULL, &hints, &addrs);
  if (err != 0)
  {
    // TODO: winapi exit thingamabob
  }

  // for (struct addrinfo* addr = addrs; addr != NULL; addr = addr->ai_next)
  // {
  //   char ipv4[16];
  //   inet_ntop(AF_INET, &((struct sockaddr_in*)addr->ai_addr)->sin_addr, ipv4, INET_ADDRSTRLEN);
  //   printf("local ip from code: %s; hex code: %08X\n", ipv4, IPtoHexCode(ipv4));
  // }

  // only returns the first result, which should suffice anyways; think default 1st is ethernet
  // then 2nd is wlan0 and any vpn stuff, but could just be dependent on the computer/network
  inet_ntop(AF_INET, &((struct sockaddr_in*)addrs->ai_addr)->sin_addr, ip_address, INET_ADDRSTRLEN);

  freeaddrinfo(addrs);

  return 0;
}

// uses hash function to get code for ip (and vice versa); only for IPv4
unsigned int IPtoHexCode(char* ip) {
  struct in_addr addr;
  inet_pton(AF_INET, ip, &addr);
  unsigned int num = ntohl(addr.s_addr); // convert network byte to machine byte order

  return hash(num);
}

void HexCodetoIP(char* code, char* dest) {
  memset(dest, 0, 16); // reset ip in case it was entered before somehow
  unsigned int tmp = unhash(strtoul(code, NULL, 16)); // 16 represents base
  sprintf_s(dest, 16, "%d.%d.%d.%d", (tmp & 0xFF000000) >> 24, (tmp & 0x00FF0000) >> 16, (tmp & 0x0000FF00) >> 8, tmp & 0x000000FF);
}

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
