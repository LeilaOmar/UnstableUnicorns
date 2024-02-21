#include <stdint.h>
#include "networkfuncs.h"
#include "gamemechanics.h"
#include "windowsapp.h"

#define BUF_HTTP 256

// ********************************************************************************
// ****************************** Lobby Code Hashing ******************************
// ********************************************************************************

/**
 * @brief Encodes IPv4 addresses into hexadecimal numbers
 *
 * https://stackoverflow.com/questions/664014/what-integer-hash-function-are-good-that-accepts-an-integer-hash-key
 * @return <unsigned int> Obfuscated hex code of the given IP address 
 */
static unsigned int Hash(unsigned int x) {
  x = ((x >> 16) ^ x) * 0x45d9f3b;
  x = ((x >> 16) ^ x) * 0x45d9f3b;
  x = (x >> 16) ^ x;
  return x;
}

/**
 * @brief Decodes hashed IPv4 addresses
 *
 * https://stackoverflow.com/questions/664014/what-integer-hash-function-are-good-that-accepts-an-integer-hash-key
 * @return <unsigned int> Hexadecimal representation of the actual IP address
 */
static unsigned int Unhash(unsigned int x) {
  x = ((x >> 16) ^ x) * 0x119de1f3;
  x = ((x >> 16) ^ x) * 0x119de1f3;
  x = (x >> 16) ^ x;
  return x;
}

void GetIPreq(char *ip) {
  //WSADATA wsaData;
  struct addrinfo hints;
  struct addrinfo* result = NULL;
  int sd, err;
  char buffer[BUF_HTTP] = { 0 };

  // api.ipify.org is only for IPv4; api64.ipify.org is both v4 and v6 w/ priority for v6
  // **don't forget to change it in getaddrinfo too if ever**
  char *httpreq = "GET / HTTP/1.1\r\nHost: api.ipify.org\r\nConnection: close\r\n\r\n";

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
  recv(sd, buffer, BUF_HTTP, 0);

  freeaddrinfo(result);
  closesocket(sd);
  //WSACleanup();

  // this only works because the text is the only output from the site
  // therefore it's the last line and does not have a newline after it
  int index = 0;
  for (int i = BUF_HTTP - 1; i >= 0; i--) {
    if (buffer[i] == '\n') {
      index = i + 1;
      break;
    }
  }

  // using sizeof(ip) for MaxCount may not be appropriate since the length is variable
  // it still works though since everything afterwards is \0 *shrugs in laziness*
  strncpy_s(ip, 16, buffer + index, 16);
}

int GetLocalIP(char *ip)
{
  char szBuffer[64];

  if (gethostname(szBuffer, sizeof(szBuffer)) == SOCKET_ERROR)
  {
    return 1;
  }

  int err;
  struct addrinfo hints, *addrs;
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
  inet_ntop(AF_INET, &((struct sockaddr_in*)addrs->ai_addr)->sin_addr, ip, INET_ADDRSTRLEN);

  freeaddrinfo(addrs);

  return 0;
}

unsigned int IPtoHexCode(char *ip) {
  struct in_addr addr;
  inet_pton(AF_INET, ip, &addr);
  unsigned int num = ntohl(addr.s_addr); // convert network byte to machine byte order

  return Hash(num);
}

void HexCodeToIP(char *code, char *dest) {
  memset(dest, 0, 16); // reset ip in case it was entered before somehow
  unsigned int tmp = Unhash(strtoul(code, NULL, 16)); // 16 represents base
  sprintf_s(dest, 16, "%d.%d.%d.%d", (tmp & 0xFF000000) >> 24, (tmp & 0x00FF0000) >> 16, (tmp & 0x0000FF00) >> 8, tmp & 0x000000FF);
}

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

int SendLobbyPacket(int numPlayers, int clientpnum, int fd) {
  int count = sizeof(int) + sizeof(int) + strlen(partyMems) + (2 * sizeof(long) * MAX_PLAYERS);
  int rc;
  static char data[512];
  int offset = 8;
  SerializeInt(data, numPlayers);
  SerializeInt(data + 4, clientpnum);
  for (int i = 0; i < (int)strlen(partyMems); i++) {
    data[offset] = partyMems[i];
    offset++;
  }
  for (int i = 0; i < MAX_PLAYERS; i++) {
    SerializeInt(data + offset, (int)pselect[i].left);
    offset += 4;
    SerializeInt(data + offset, (int)pselect[i].top);
    offset += 4;
  }

  // send data count size before sending data packet
  SendInt(count, fd);

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

int ReceiveLobbyPacket(int *numPlayers, int *clientpnum, int fd) {
  int count, rc, offset = 0;
  static char data[512];
  ReceiveInt(&count, fd);

  while (count > 0) {
    rc = recv(fd, data + offset, count, 0);
    if (rc >= 0) {
      offset += rc;
      count -= rc;
    }
  }

  count = offset;
  offset = 8;
  *numPlayers = DeserializeInt(data);
  *clientpnum = DeserializeInt(data + 4);
  int strcount = count - (2 * sizeof(int)) - (2 * sizeof(long) * MAX_PLAYERS);

  // TODO: take care of these buffer overrun warnings before more out of range errors occur <_<
  for (int i = 0; i < strcount; i++) {
    partyMems[i] = data[offset];
    offset++;
  }
  partyMems[strcount] = '\0';

  for (int i = 0; i < MAX_PLAYERS; i++) {
    (int)pselect[i].left = DeserializeInt(data + offset);
    offset += 4;
    (int)pselect[i].top = DeserializeInt(data + offset);
    offset += 4;
  }

  return 0;
}

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
