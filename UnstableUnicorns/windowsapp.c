#include "windowsapp.h"
#include "server.h"
#include "client.h"
#include "gamemechanics.h"

#pragma comment(lib,"msimg32.lib") // Transparent blt

// ********************************************************************************
// ************************** super global variables TM ***************************
// ********************************************************************************

HINSTANCE hInstanceGlob;
char ip[16], hexcode[43];
enum GameState menustate = 0;
char partymems[PARTYSTRSIZE] = { 0 };
HWND webhwnd;
RECT pselect[MAX_PLAYERS];
// toggle to see whether or not a baby unicorn is taken during lobby selection; 0 = free, 1 = taken
BOOL babytoggle[13] = { FALSE };
// switch to activate certain networking features in the network thread to avoid improperly closing a socket
// this is super ugly :/
unsigned char networktoggle = 0;

// ********************************************************************************
// ************** private global variables for this source file only **************
// ********************************************************************************

enum tab {UNICORN_TAB, UPGRADE_TAB, HAND_TAB, PAGE_LEFT, PAGE_RIGHT, NURSERY_TAB, DECK_TAB, DISCARD_TAB};
enum fonts { OLDFONT, CHONKYFONT, BOLDFONT, ITALICFONT, FANCYFONT, FANCYITALICS, NUMCUSTOMFONTS };

HWND gamethread, networkthread, textNameHwnd, portHwnd, codeHwnd;
DWORD tId, tIdweb;
WNDCLASSEX wcexHost;
WNDCLASSEX wcexJoin;
HBITMAP hBitmapBG[NUMSTATES], hBitmapBorder[MAX_PLAYERS], hBitmapCard[15], hBitmapTab[3];
BOOL windowOpen[2] = { FALSE };
BOOL childWindow[2] = { FALSE };
HFONT fonts[NUMCUSTOMFONTS] = { NULL };

enum tab tabnum;    // the tab number representation of the window to display on the bottom of the in-game screen
int tabsize;        // number of total cards within the current tab view array (e.g. if you're looking at the deck, then tabsize could be anywhere from 1 to 115)
int pagenum = 1;    // the page number within the current tab view array
int pnumindex = 0;  // used to switch between player indices to show their respective stables inside the DisplayCardWindow function

struct ToolTip hoverTip;
int stablePadding = 15; // arbitrary number of pixels to pad between the displayed list of cards in their respective stables/hands/decks

// these are in the same order as UnicornDatatypes, which are used to represent the card classes
const COLORREF cardColors[] = {
  RGB(185, 109, 171),   // baby unicorn color     - purple
  RGB(107, 116, 181),   // basic unicorn color    - dark periwinkle (blue)
  RGB( 32, 189, 242),   // magical unicorn color  - aqua blue
  RGB(242, 128,  38),   // upgrade color          - orange
  RGB(255, 180,  23),   // downgrade color        - gold
  RGB(134, 200,  80),   // magic color            - lime green
  RGB(244,  29,  37),   // instant color          - red
};

// ********************************************************************************
// ****************************** button management *******************************
// ********************************************************************************

struct Button hornbutton;
struct Button titleButtons[3];
struct Button ruleButtons[3];
struct Button lobbyButtons[2];
struct Button debugButtons[8];
struct Button cardslots[7];

// TODO: potentially rename these to icon[1-13]?
struct Button icons[] = {
  { 1064, 28, 95, 81,     "Assets\\icon_red.bmp", NULL, NULL, NULL },
  { 1064, 28, 95, 81,    "Assets\\icon_pink.bmp", NULL, NULL, NULL },
  { 1064, 28, 95, 81,  "Assets\\icon_orange.bmp", NULL, NULL, NULL },
  { 1064, 28, 95, 81,  "Assets\\icon_yellow.bmp", NULL, NULL, NULL },
  { 1064, 28, 95, 81,   "Assets\\icon_green.bmp", NULL, NULL, NULL },
  { 1064, 28, 95, 81,    "Assets\\icon_blue.bmp", NULL, NULL, NULL },
  { 1064, 28, 95, 81,  "Assets\\icon_purple.bmp", NULL, NULL, NULL },
  { 1064, 28, 95, 81,   "Assets\\icon_black.bmp", NULL, NULL, NULL },
  { 1064, 28, 95, 81,   "Assets\\icon_white.bmp", NULL, NULL, NULL },
  { 1064, 28, 95, 81,   "Assets\\icon_brown.bmp", NULL, NULL, NULL },
  { 1064, 28, 95, 81, "Assets\\icon_rainbow.bmp", NULL, NULL, NULL },
  { 1064, 28, 95, 81,   "Assets\\icon_death.bmp", NULL, NULL, NULL },
  { 1064, 28, 95, 81, "Assets\\icon_narwhal.bmp", NULL, NULL, NULL }
};
struct Button player_nums[] = {
  { 1064, 28,  95, 81, "Assets\\player1.bmp", NULL, NULL, NULL },
  { 1175, 28,  95, 81, "Assets\\player2.bmp", NULL, NULL, NULL },
  { 1064, 118, 95, 81, "Assets\\player3.bmp", NULL, NULL, NULL },
  { 1175, 118, 95, 81, "Assets\\player4.bmp", NULL, NULL, NULL },
  { 1064, 208, 95, 81, "Assets\\player5.bmp", NULL, NULL, NULL },
  { 1175, 208, 95, 81, "Assets\\player6.bmp", NULL, NULL, NULL },
  { 1064, 298, 95, 81, "Assets\\player7.bmp", NULL, NULL, NULL },
  { 1175, 298, 95, 81, "Assets\\player8.bmp", NULL, NULL, NULL }
};
struct Button stable_nums[8];

// baby unicorn point coordinates in the lobby selection
// ordered: left, top, right, bottom
const RECT babies[] = {
  {  659,  243,  748,  332 },   // BABYRED
  {  779,  243,  868,  332 },   // BABYPINK
  {  899,  243,  988,  332 },   // BABYORANGE
  { 1019,  243, 1108,  332 },   // BABYYELLOW

  {  599,  358,  688,  447 },   // BABYGREEN
  {  719,  358,  808,  447 },   // BABYBLUE
  {  839,  358,  928,  447 },   // BABYPURPLE
  {  959,  358, 1048,  447 },   // BABYBLACK
  { 1079,  358, 1168,  447 },   // BABYWHITE

  {  659,  473,  748,  562 },   // BABYBROWN
  {  779,  473,  868,  562 },   // BABYRAINBOW
  {  899,  473,  988,  562 },   // BABYDEATH
  { 1019,  473, 1108,  562 },   // BABYNARWHAL
};

// for the player number border surrounding the baby unicorn selection in the lobby
#define BORDERWIDTH 97

// calculates the position for the horn select bitmap that centers around the hovered button
void HornPosition(struct Button b) {
  HGDIOBJ oldSprite;

  int xdiff = -93;
  int ydiff = 17;

  hornbutton.x = b.x + xdiff;
  hornbutton.y = b.y + ydiff;
  hornbutton.source = TRUE;
}

void InitTitleButtons(struct Button *b, HWND hWnd, int size) {
  // horn is at point 435, 402 for the first button
  for (int i = 0; i < size; i++) {
    b[i].x = 528;
    b[i].y = 385 + (i * 100);
    b[i].width = 230;
    b[i].height = 75;
  }

  b[0].source = hWnd;
  b[1].source = hWnd;
  b[2].source = RULESONE;
  b[0].onClick = HostGeneration;
  b[1].onClick = JoinGeneration;
  b[2].onClick = SwitchState;

  // TODO: incorporate error msg in case of failure
  // horn select
  hornbutton.width = 416;
  hornbutton.height = 40;
  hornbutton.bitmap = (HBITMAP)LoadImage(NULL, "Assets\\horn_select.bmp",
    IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
}

void InitRuleButtons(struct Button *b, int size) {
  for (int i = 0; i < size; i++) {
    b[i].y = 619;
    b[i].width = 32;
    b[i].height = 32;
    b[i].onClick = SwitchState;
  }

  b[0].width = 100;
  b[0].x = 975;
  b[1].x = 1087;
  b[2].x = 1129;

  // source is just storing the triggered game state that should occur when clicking the button
  b[0].source = TITLEBLANK;
  b[1].source = RULESONE;
  b[2].source = RULESTWO;
}

void InitLobbyButtons(struct Button *b) {
  // start game
  b[0].x = 120;
  b[0].y = 590;
  b[0].width = 190;
  b[0].height = 50;
  b[0].source = GAMESTART;
  b[0].onClick = StartGame;

  // leave lobby
  b[1].x = 360;
  b[1].y = 590;
  b[1].width = 190;
  b[1].height = 50;
  b[0].source = TITLEBLANK;
  b[1].onClick = LeaveLobby;
}

void InitCardWindowButtons(struct Button *b) {
  // unicorn stable tab
  b[UNICORN_TAB].x = 0;
  b[UNICORN_TAB].y = 463;
  b[UNICORN_TAB].width = 233;
  b[UNICORN_TAB].height = 29;
  b[UNICORN_TAB].source = UNICORN_TAB;
  b[UNICORN_TAB].onClick = SwitchTab;

  // upgrade/downgrade tab
  b[UPGRADE_TAB].x = 233;
  b[UPGRADE_TAB].y = 463;
  b[UPGRADE_TAB].width = 273;
  b[UPGRADE_TAB].height = 29;
  b[UPGRADE_TAB].source = UPGRADE_TAB;
  b[UPGRADE_TAB].onClick = SwitchTab;

  // hand tab
  b[HAND_TAB].x = 506;
  b[HAND_TAB].y = 463;
  b[HAND_TAB].width = 272;
  b[HAND_TAB].height = 29;
  b[HAND_TAB].source = HAND_TAB;
  b[HAND_TAB].onClick = SwitchTab;

  // TODO: incorporate error msg in case of failure
  // page left
  b[PAGE_LEFT].x = 3;
  b[PAGE_LEFT].y = 636;
  b[PAGE_LEFT].width = 80;
  b[PAGE_LEFT].height = 59;
  b[PAGE_LEFT].source = PAGE_LEFT;
  b[PAGE_LEFT].onClick = TurnPage;
  strcpy_s(b[PAGE_LEFT].filename, sizeof b[PAGE_LEFT].filename, "Assets\\pageleft.bmp");
  b[PAGE_LEFT].bitmap = (HBITMAP)LoadImage(NULL, b[PAGE_LEFT].filename,
    IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

  // page right
  b[PAGE_RIGHT].x = 1196;
  b[PAGE_RIGHT].y = 636;
  b[PAGE_RIGHT].width = 80;
  b[PAGE_RIGHT].height = 59;
  b[PAGE_RIGHT].source = PAGE_RIGHT;
  b[PAGE_RIGHT].onClick = TurnPage;
  strcpy_s(b[PAGE_RIGHT].filename, sizeof b[PAGE_RIGHT].filename, "Assets\\pageright.bmp");
  b[PAGE_RIGHT].bitmap = (HBITMAP)LoadImage(NULL, b[PAGE_RIGHT].filename,
    IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
}

void InitCardButtons(struct Button *b, int size) {
  // start is the starting point for the display, of course, with the scroll bar it could go to 0
  POINT start = { 87, 507 };
  BITMAP bm;
  GetObject(hBitmapCard[0], (int)sizeof bm, &bm); // fetches width/height; all cards have the same w/h

  int distance = stablePadding + bm.bmWidth;

  for (int i = 0; i < size; i++) {
    b[i].x = start.x + (distance * i);
    b[i].y = start.y;
    b[i].width = bm.bmWidth;
    b[i].height = bm.bmHeight;
    b[i].filename[0] = '\0';
    b[i].bitmap = NULL;
    b[i].source = NULL;
    b[i].onClick = NULL;
  }
}

void InitDeckButtons(struct Button *b) {
  // TODO: the onClick function will likely be multi-purpose, so this will need to be rewritten

  // nursery pile
  b[NURSERY_TAB].x = 346;
  b[NURSERY_TAB].y = 132;
  b[NURSERY_TAB].width = 145;
  b[NURSERY_TAB].height = 200;
  b[NURSERY_TAB].source = NURSERY_TAB;
  b[NURSERY_TAB].onClick = SwitchTab;

  // deck pile
  b[DECK_TAB].x = 521;
  b[DECK_TAB].y = 132;
  b[DECK_TAB].width = 145;
  b[DECK_TAB].height = 200;
  b[DECK_TAB].source = DECK_TAB;
  b[DECK_TAB].onClick = SwitchTab;

  // discard pile
  b[DISCARD_TAB].x = 696;
  b[DISCARD_TAB].y = 132;
  b[DISCARD_TAB].width = 145;
  b[DISCARD_TAB].height = 200;
  b[DISCARD_TAB].source = DISCARD_TAB;
  b[DISCARD_TAB].onClick = SwitchTab;
}

void InitButtonManager(HWND hWnd) {
  InitTitleButtons(titleButtons, hWnd, sizeof titleButtons / sizeof titleButtons[0]);
  InitRuleButtons(ruleButtons, sizeof ruleButtons / sizeof ruleButtons[0]);
  InitLobbyButtons(lobbyButtons);
  InitCardWindowButtons(debugButtons);
  InitCardButtons(cardslots, sizeof cardslots / sizeof cardslots[0]);
  InitDeckButtons(debugButtons);
}

// ********************************************************************************
// ************************ window and general UI creation ************************
// ********************************************************************************

enum { ID_BUTTON, ID_TEXT };

void CreateHostWindow(HWND hwnd) {
  if (childWindow[0] == FALSE) {
    ZeroMemory(&wcexHost, sizeof(WNDCLASSEX)); // TODO: figure out what this does
    wcexHost.cbSize = sizeof(WNDCLASSEX);
    wcexHost.cbClsExtra = 0;
    wcexHost.cbWndExtra = 0;
    wcexHost.style = CS_HREDRAW | CS_VREDRAW;
    wcexHost.hInstance = hInstanceGlob;
    wcexHost.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcexHost.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
    wcexHost.lpszMenuName = NULL;
    wcexHost.lpszClassName = _T("HostParameters");
    wcexHost.lpfnWndProc = WndProcHost;
    wcexHost.hIcon = NULL;
    wcexHost.hIconSm = NULL;

    if (!RegisterClassEx(&wcexHost))
    {
      MessageBox(NULL,
        _T("Call to RegisterClassEx failed!"),
        _T("Windows Desktop Guided Tour"),
        NULL);
      return;
    }
    else {
      childWindow[0] = TRUE;
    }
  }

  HWND hWndHost = CreateWindowEx(
    WS_EX_OVERLAPPEDWINDOW,
    wcexHost.lpszClassName,
    "Hostie Toastie",
    WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT, CW_USEDEFAULT,
    400, 300,
    hwnd, // parent/owner window; using the WS_CHILD flag w/ WS_OVERLAPPEDWINDOW or SetParent function makes the window bound to the dimensions of the game window
    NULL,
    hInstanceGlob,
    NULL
  );

  if (!hWndHost)
  {
    MessageBox(NULL,
      _T("Host window creation failed!"),
      _T("Windows Desktop Guided Tour"),
      NULL);
    return;
  }

  ShowWindow(hWndHost, SW_NORMAL); // nCmdShow [flag SW...] controls how the window is shown
}

void CreateJoinWindow(HWND hwnd) {
  if (childWindow[1] == FALSE) {
    ZeroMemory(&wcexJoin, sizeof(WNDCLASSEX)); // TODO: figure out what this does
    wcexJoin.cbSize = sizeof(WNDCLASSEX);
    wcexJoin.cbClsExtra = 0;
    wcexJoin.cbWndExtra = 0;
    wcexJoin.style = CS_HREDRAW | CS_VREDRAW;
    wcexJoin.hInstance = hInstanceGlob;
    wcexJoin.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcexJoin.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
    wcexJoin.lpszMenuName = NULL;
    wcexJoin.lpszClassName = _T("JoinParameters");
    wcexJoin.lpfnWndProc = WndProcJoin;
    wcexJoin.hIcon = NULL;
    wcexJoin.hIconSm = NULL;

    if (!RegisterClassEx(&wcexJoin))
    {
      MessageBox(NULL,
        _T("Call to RegisterClassEx failed!"),
        _T("Windows Desktop Guided Tour"),
        NULL);
      return;
    }
    else {
      childWindow[1] = TRUE;
    }
  }

  HWND hWndJoin = CreateWindowEx(
    WS_EX_OVERLAPPEDWINDOW,
    wcexJoin.lpszClassName,
    "Join me, Jimmy",
    WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT, CW_USEDEFAULT,
    400, 300,
    hwnd, // parent/owner window; using the WS_CHILD flag w/ WS_OVERLAPPEDWINDOW or SetParent function makes the window bound to the dimensions of the game window
    NULL,
    hInstanceGlob,
    NULL
  );

  if (!hWndJoin)
  {
    MessageBox(NULL,
      _T("Join window creation failed!"),
      _T("Windows Desktop Guided Tour"),
      NULL);
    return;
  }

  ShowWindow(hWndJoin, SW_NORMAL);
}

void DisplayCardWindow(HDC* hdcMem, HDC* hdcSprite) {
  // use pages to view the card window in lieu of scroll bars :)

  char windowtxt[64]; // tab window info to show the name of the currently viewed stable/deck
  HGDIOBJ oldSprite;
  POINT start = { 87, 507 }; // starting point for the display
  BITMAP bm;

  GetObject(hBitmapCard[0], (int)sizeof bm, &bm); // fetches width/height; all cards have the same w/h
  int distance = stablePadding + bm.bmWidth;

  // cards are just rectangles, so transparentblt isn't necessary
  // this relies on an accurate unicorn/stable/card count; the pages will bork otherwise
  int index_start = (pagenum - 1) * 7;

  // the actual index for the cards, accounting for filtered class and species types
  int skip = 0;

  switch (tabnum) {
  case UNICORN_TAB:
  {
    // draw the background for ease of view
    oldSprite = SelectObject(*hdcSprite, hBitmapTab[tabnum]);
    BitBlt(*hdcMem, 0, 497, BWIDTH, 223, *hdcSprite, 0, 0, SRCCOPY);
    SelectObject(*hdcSprite, oldSprite);

    tabsize = player[pnumindex].stable.num_unicorns;
    snprintf(windowtxt, sizeof windowtxt, "%s's Stable", player[pnumindex].username);

    // TODO: using the original index_start wouldn't account for skipped upgrade/downgrade cards,
    // but adding an extra array to check for offset cards within extra pages seems extremely overkill...
    // 
    // in any case, filtering is still necessary for specific deck functions, and fixing this one by
    // grouping the unicorns w/ upgrade/downgrade cards in a stable wouldn't fully solve the problem
    int count = 0;
    for (int k = 0; count < player[pnumindex].stable.size && k < (pagenum - 1) * 7; count++) {
      if (checkClass(ANYUNICORN, deck[player[pnumindex].stable.unicorns[count]].class))
        k++;
    }
    index_start = count;

    for (int i = index_start; i < player[pnumindex].stable.size && skip < 7; i++) {
      // for now, this cycles through hBitmapCard which only has the super neigh card and the back design
      // in the future, hBitmapCard could be replaced/assimilated by an updated Unicorn or Deck structure featuring the file name and the loaded hBitmap
      if (checkClass(ANYUNICORN, deck[player[pnumindex].stable.unicorns[i]].class)) {
        // save the source into cardslots
        cardslots[skip].source = &deck[player[pnumindex].stable.unicorns[i]];

        // draw the sprite
        oldSprite = SelectObject(*hdcSprite, hBitmapCard[13 + i % 2]);
        BitBlt(*hdcMem, start.x + (distance * skip), start.y, bm.bmWidth, bm.bmHeight, *hdcSprite, 0, 0, SRCCOPY);
        SelectObject(*hdcSprite, oldSprite);
        skip++;
      }
    }
    break;
  }
  case UPGRADE_TAB:
  {
    // draw the background for ease of view
    oldSprite = SelectObject(*hdcSprite, hBitmapTab[tabnum]);
    BitBlt(*hdcMem, 0, 497, BWIDTH, 223, *hdcSprite, 0, 0, SRCCOPY);
    SelectObject(*hdcSprite, oldSprite);

    // TODO: see unicorn_tab for the indexing issues. this is unlikely to happen in a normal game though due to most players having less than 8 up/downgrades
    tabsize = player[pnumindex].stable.size - player[pnumindex].stable.num_unicorns;
    snprintf(windowtxt, sizeof windowtxt, "%s's Stable", player[pnumindex].username);

    int j = 0;
    for (int count = 0; j < player[pnumindex].stable.size && count < (pagenum - 1) * 7; j++) {
      if (!checkClass(ANYUNICORN, deck[player[pnumindex].stable.unicorns[j]].class))
        count++;
    }
    index_start = j;

    for (int i = index_start; i < player[pnumindex].stable.size && skip < 7; i++) {
      // for now, this cycles through hBitmapCard which only has the super neigh card and the back design
      // in the future, hBitmapCard could be replaced/assimilated by an updated Unicorn or Deck structure featuring the file name and the loaded hBitmap
      if (deck[player[pnumindex].stable.unicorns[i]].class == UPGRADE || deck[player[pnumindex].stable.unicorns[i]].class == DOWNGRADE) {
        // save the source into cardslots
        cardslots[skip].source = &deck[player[pnumindex].stable.unicorns[i]];

        // draw the sprite
        oldSprite = SelectObject(*hdcSprite, hBitmapCard[13 + i % 2]);
        BitBlt(*hdcMem, start.x + (distance * skip), start.y, bm.bmWidth, bm.bmHeight, *hdcSprite, 0, 0, SRCCOPY);
        SelectObject(*hdcSprite, oldSprite);
        skip++;
      }
    }
    break;
  }
  case HAND_TAB:
  {
    // draw the background for ease of view
    oldSprite = SelectObject(*hdcSprite, hBitmapTab[tabnum]);
    BitBlt(*hdcMem, 0, 497, BWIDTH, 223, *hdcSprite, 0, 0, SRCCOPY);
    SelectObject(*hdcSprite, oldSprite);

    tabsize = player[pnumindex].hand.num_cards;
    snprintf(windowtxt, sizeof windowtxt, "%s's Hand", player[pnumindex].username);

    for (int i = index_start; i < player[pnumindex].hand.num_cards && i < index_start + 7; i++) {
      // save the source into cardslots
      cardslots[skip++].source = &deck[player[pnumindex].hand.cards[i]];

      // for now, this cycles through hBitmapCard which only has the super neigh card and the back design
      // in the future, hBitmapCard could be replaced/assimilated by an updated Unicorn or Deck structure featuring the file name and the loaded hBitmap
      oldSprite = SelectObject(*hdcSprite, hBitmapCard[13 + i % 2]);
      BitBlt(*hdcMem, start.x + (distance * (i % 7)), start.y, bm.bmWidth, bm.bmHeight, *hdcSprite, 0, 0, SRCCOPY);
      SelectObject(*hdcSprite, oldSprite);
    }
    break;
  }
  case NURSERY_TAB:
  {
    tabsize = NURSERY_SIZE - nursery_index;
    strcpy_s(windowtxt, sizeof windowtxt, "Nursery");

    for (int i = nursery_index + index_start; i < NURSERY_SIZE && i < index_start + 7; i++) {
      // save the source into cardslots
      cardslots[skip++].source = &deck[nursery_ref[i]];

      // for now, this cycles through hBitmapCard which only has the super neigh card and the back design
      // in the future, hBitmapCard could be replaced/assimilated by an updated Unicorn or Deck structure featuring the file name and the loaded hBitmap
      oldSprite = SelectObject(*hdcSprite, hBitmapCard[i]);
      BitBlt(*hdcMem, start.x + (distance * (i % 7)), start.y, bm.bmWidth, bm.bmHeight, *hdcSprite, 0, 0, SRCCOPY);
      SelectObject(*hdcSprite, oldSprite);
    }
    break;
  }
  case DECK_TAB:
  {
    // TODO: ifdef, or put this check earlier?
    if (menustate != DEBUGMODE) {
      break;
    }

    // TODO: include a check for card effects that allow deck viewing, and then include a check for specific cards
    tabsize = DECK_SIZE - deck_index;
    strcpy_s(windowtxt, sizeof windowtxt, "Deck");

    for (int i = deck_index + index_start; i < DECK_SIZE && i < index_start + 7; i++) {
      // save the source into cardslots
      cardslots[skip++].source = &deck[deck_ref[i]];

      // for now, this cycles through hBitmapCard which only has the super neigh card and the back design
      // in the future, hBitmapCard could be replaced/assimilated by an updated Unicorn or Deck structure featuring the file name and the loaded hBitmap
      oldSprite = SelectObject(*hdcSprite, hBitmapCard[13 + i % 2]);
      BitBlt(*hdcMem, start.x + (distance * (i % 7)), start.y, bm.bmWidth, bm.bmHeight, *hdcSprite, 0, 0, SRCCOPY);
      SelectObject(*hdcSprite, oldSprite);
    }
    break;
  }
  case DISCARD_TAB:
  {
    // TODO: (maybe) include a check for specific cards
    tabsize = discard_index;
    strcpy_s(windowtxt, sizeof windowtxt, "Discard Pile");

    for (int i = index_start; i < discard_index && i < index_start + 7; i++) {
      // save the source into cardslots
      cardslots[skip++].source = &deck[discard_ref[i]];

      // for now, this cycles through hBitmapCard which only has the super neigh card and the back design
      // in the future, hBitmapCard could be replaced/assimilated by an updated Unicorn or Deck structure featuring the file name and the loaded hBitmap
      oldSprite = SelectObject(*hdcSprite, hBitmapCard[i % 2]);
      BitBlt(*hdcMem, start.x + (distance * (i % 7)), start.y, bm.bmWidth, bm.bmHeight, *hdcSprite, 0, 0, SRCCOPY);
      SelectObject(*hdcSprite, oldSprite);
    }
    break;
  }
  default:
    // what happened???
    break;
  }

  // reset the card button sources so that they don't erroneously trigger the hover/click actions on empty spaces for different tab views
  while (skip < 7) {
    cardslots[skip++].source = NULL;
  }

  // show the page arrow icons if applicable
  if (tabsize > pagenum * 7) {
    oldSprite = SelectObject(*hdcSprite, debugButtons[PAGE_RIGHT].bitmap);
    TransparentBlt(*hdcMem, debugButtons[PAGE_RIGHT].x, debugButtons[PAGE_RIGHT].y, debugButtons[PAGE_RIGHT].width, debugButtons[PAGE_RIGHT].height, *hdcSprite, 0, 0, debugButtons[PAGE_RIGHT].width, debugButtons[PAGE_RIGHT].height, RGB(0, 255, 0));
    SelectObject(*hdcSprite, oldSprite);
  }
  if (pagenum > 1) {
    oldSprite = SelectObject(*hdcSprite, debugButtons[PAGE_LEFT].bitmap);
    TransparentBlt(*hdcMem, debugButtons[PAGE_LEFT].x, debugButtons[PAGE_LEFT].y, debugButtons[PAGE_LEFT].width, debugButtons[PAGE_LEFT].height, *hdcSprite, 0, 0, debugButtons[PAGE_LEFT].width, debugButtons[PAGE_LEFT].height, RGB(0, 255, 0));
    SelectObject(*hdcSprite, oldSprite);
  }

  // display the window tab as text
  SelectObject(*hdcMem, fonts[FANCYITALICS]);
  SetBkMode(*hdcMem, TRANSPARENT); // box surrounding text is transparent instead of white
  SetTextColor(*hdcMem, RGB(255, 255, 255));
  RECT rc = { 5, 425, 935, 457 }; // left side directly above the tabs
  DrawText(*hdcMem, windowtxt, strlen(windowtxt), &rc, DT_LEFT | DT_NOCLIP | DT_WORDBREAK);
}

struct ToolTip ReturnCardHoverTip(struct Button self) {
  struct ToolTip tippy;
  struct Unicorn corn = *(struct Unicorn*)(self.source);

  int width = 250;
  int height = 150;
  int padding = 8;

  strcpy_s(tippy.title, sizeof tippy.title, corn.name);
  strcpy_s(tippy.msg, sizeof tippy.msg, corn.description);
  tippy.fonttitle = FANCYFONT;
  tippy.fonttxt   = OLDFONT;

  tippy.x = self.x + 45;
  tippy.y = self.y - height - 25;
  tippy.width   = width;
  tippy.height  = height;
  tippy.ishover = TRUE;
  tippy.bgcolor = cardColors[corn.class];

  // update edge
  if (tippy.x + tippy.width > BWIDTH) {
    tippy.x = BWIDTH - tippy.width - (padding * 2);
  }

  switch (corn.class) {
  case BABYUNICORN:
    strcpy_s(tippy.subtitle, sizeof tippy.subtitle, "Baby Unicorn card");
    break;
  case BASICUNICORN:
    strcpy_s(tippy.subtitle, sizeof tippy.subtitle, "Basic Unicorn card");
    break;
  case MAGICUNICORN:
    strcpy_s(tippy.subtitle, sizeof tippy.subtitle, "Magical Unicorn card");
    break;
  case UPGRADE:
    strcpy_s(tippy.subtitle, sizeof tippy.subtitle, "Upgrade card");
    break;
  case DOWNGRADE:
    strcpy_s(tippy.subtitle, sizeof tippy.subtitle, "Downgrade card");
    break;
  case MAGIC:
    strcpy_s(tippy.subtitle, sizeof tippy.subtitle, "Magic card");
    break;
  case INSTANT:
    strcpy_s(tippy.subtitle, sizeof tippy.subtitle, "Instant card");
    break;
  }

  return tippy;
}

struct ToolTip ReturnPlayerHoverTip(int pnum, int buttonX, int buttonY) {
  struct ToolTip tippy;

  int width = 200;
  int height = 60;

  // TODO: update size for longer names
  strcpy_s(tippy.title, sizeof tippy.title, player[pnum].username);
  tippy.subtitle[0] = '\0';
  snprintf(tippy.msg, sizeof tippy.msg, "# of cards in hand: %d\n# of unicorns in stable: %d",
    player[pnum].hand.num_cards, player[pnum].stable.num_unicorns);
  tippy.fonttitle = BOLDFONT;
  tippy.fonttxt   = OLDFONT;

  tippy.x = buttonX - width - 20;
  tippy.y = buttonY - 10;
  tippy.width   = width;
  tippy.height  = height;
  tippy.ishover = TRUE;
  tippy.bgcolor = RGB(255, 0, 150);

  return tippy;
}

void CreateCustomToolTip(HDC *hdcMem, struct ToolTip hoverTip) {
  RECT rc;
  int padding = 8;

  // TODO: mess around with colors later to make it look prettier :3

  // draw the outer box
  HBRUSH brush = CreateSolidBrush(hoverTip.bgcolor);
  HBRUSH old_brush = (HBRUSH) SelectObject(*hdcMem, brush);

  // add some padding to the rectangle area instead of applying it to the text
  RoundRect(*hdcMem, hoverTip.x - padding, hoverTip.y - padding, hoverTip.x + hoverTip.width + (padding * 2), hoverTip.y + hoverTip.height + (padding * 2), 15, 15);

  SelectObject(*hdcMem, old_brush);
  DeleteObject(brush);

  // prepare the message text :)

  SetBkMode(*hdcMem, TRANSPARENT); // box surrounding text is transparent instead of white

  int title_offset = 0;
  SetRect(&rc, hoverTip.x, hoverTip.y, hoverTip.x + hoverTip.width, hoverTip.y + hoverTip.width);
  SetTextColor(*hdcMem, RGB(255, 255, 255));
  SelectObject(*hdcMem, fonts[hoverTip.fonttitle]);
  DrawText(*hdcMem, hoverTip.title, strlen(hoverTip.title), &rc, DT_LEFT | DT_WORDBREAK);

  // calculate the vertical space taken by the title
  SIZE size;
  GetTextExtentPoint(*hdcMem, hoverTip.title, strlen(hoverTip.title), &size);
  title_offset = size.cy + (padding / 2);

  // double the vertical space if the title is long enough to wrap around the rect border
  if (strlen(hoverTip.title) >= 24) {
    title_offset = (title_offset * 2) - (padding / 2);
  }

  // TODO: (maybe) calculate horizontal space too for the more verbose titles?
  // GetTextExtentPoint only works after the text has been drawn, but then the title and bg rectangle wouldn't be updateable...
  
  // hoverTip.width = (size.cx > hoverTip.width) ? size.cx : hoverTip.width;
  // 
  // if (hoverTip.x + hoverTip.width > BWIDTH) {
  //   hoverTip.x = BWIDTH - hoverTip.width - (padding * 2);
  // }


  // type out the subtitle for the card info (i.e. the class/type of card)
  if (hoverTip.subtitle[0] != '\0') {
    SetRect(&rc, hoverTip.x, hoverTip.y + title_offset, hoverTip.x + hoverTip.width, hoverTip.y + hoverTip.width);
    SetTextColor(*hdcMem, RGB(255, 255, 255));
    SelectObject(*hdcMem, fonts[ITALICFONT]);
    DrawText(*hdcMem, hoverTip.subtitle, strlen(hoverTip.subtitle), &rc, DT_LEFT | DT_WORDBREAK);

    // calculate the vertical space taken by the title
    SIZE size;
    GetTextExtentPoint(*hdcMem, hoverTip.title, strlen(hoverTip.title), &size);
    title_offset += size.cy + (padding / 2);
  }

  // type out message
  SetRect(&rc, hoverTip.x, hoverTip.y + title_offset, hoverTip.x + hoverTip.width, hoverTip.y + hoverTip.width + title_offset);
  SetTextColor(*hdcMem, RGB(255, 255, 255));
  SelectObject(*hdcMem, fonts[hoverTip.fonttxt]);
  DrawText(*hdcMem, hoverTip.msg, strlen(hoverTip.msg), &rc, DT_LEFT | DT_WORDBREAK);
}

// ********************************************************************************
// *************************** initializing misc. data ****************************
// ********************************************************************************

void LoadImages(HWND hWnd) {
  char errors[1024] = "Failed to load image(s) ";
  BOOL issuccess = TRUE;

  // TODO: (maybe) use a default error texture for files that couldn't load in the case of missing files
  // ideally the error texture(s) should just be a resource file tbh

  // hBitmapBG contains all of the background bitmaps for each gamestate
  hBitmapBG[TITLEBLANK] = (HBITMAP)LoadImage(NULL, "Assets\\titleblank.bmp",
    IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
  if (hBitmapBG[TITLEBLANK] == NULL) {
    issuccess = FALSE;
    strcat_s(errors, sizeof errors, "titleblank.bmp ");
  }

  hBitmapBG[RULESONE] = (HBITMAP)LoadImage(NULL, "Assets\\rules1.bmp",
    IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
  if (hBitmapBG[RULESONE] == NULL) {
    issuccess = FALSE;
    strcat_s(errors, sizeof errors, "rules1.bmp ");
  }

  hBitmapBG[RULESTWO] = (HBITMAP)LoadImage(NULL, "Assets\\rules2.bmp",
    IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
  if (hBitmapBG[RULESTWO] == NULL) {
    issuccess = FALSE;
    strcat_s(errors, sizeof errors, "rules2.bmp ");
  }

  hBitmapBG[LOBBY] = (HBITMAP)LoadImage(NULL, "Assets\\lobby.bmp",
    IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
  if (hBitmapBG[LOBBY] == NULL) {
    issuccess = FALSE;
    strcat_s(errors, sizeof errors, "lobby.bmp ");
  }

  hBitmapBG[GAMESTART] = (HBITMAP)LoadImage(NULL, "Assets\\game.bmp",
    IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
  if (hBitmapBG[GAMESTART] == NULL) {
    issuccess = FALSE;
    strcat_s(errors, sizeof errors, "game.bmp ");
  }
  hBitmapBG[DEBUGMODE] = hBitmapBG[GAMESTART];

  // loads the border bitmaps for the baby unicorn selection in the lobby
  for (int i = 0; i < MAX_PLAYERS; i++) {
    char borderfn[32] = "";

    snprintf(borderfn, 19, "Assets\\border%d.bmp", i + 1);
    hBitmapBorder[i] = (HBITMAP)LoadImage(NULL, borderfn,
      IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    if (hBitmapBorder[0] == NULL) {
      issuccess = FALSE;
      strcat_s(errors, sizeof errors, borderfn);
      strcat_s(errors, sizeof errors, " ");
    }
  }

  // loads all possible baby unicorn player icons for the in-game distinction between players
  for (int i = 0; i < sizeof icons / sizeof icons[0]; i++) {
    icons[i].bitmap = (HBITMAP)LoadImage(NULL, icons[i].filename,
      IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    if (icons[i].bitmap == NULL) {
      issuccess = FALSE;
      strcat_s(errors, sizeof errors, icons[i].filename);
      strcat_s(errors, sizeof errors, " ");
    }
  }

  // player_nums are paired with icons at the same coordinates, but the bitmaps should be separate
  for (int i = 0; i < sizeof player_nums / sizeof player_nums[0]; i++) {
    player_nums[i].bitmap = (HBITMAP)LoadImage(NULL, player_nums[i].filename,
      IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    if (player_nums[i].bitmap == NULL) {
      issuccess = FALSE;
      strcat_s(errors, sizeof errors, player_nums[i].filename);
      strcat_s(errors, sizeof errors, " ");
    }
  }

  // bitmaps displaying the stable count; it goes from 0 to 7, so 8 in total
  for (int i = 0; i < 8; i++) {
    snprintf(stable_nums[i].filename, 19, "Assets\\stable%d.bmp", i);
    stable_nums[i].bitmap = (HBITMAP)LoadImage(NULL, stable_nums[i].filename,
      IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    if (stable_nums[i].bitmap == NULL) {
      issuccess = FALSE;
      strcat_s(errors, sizeof errors, stable_nums[i].filename);
      strcat_s(errors, sizeof errors, " ");
    }
  }

  // bitmaps for displaying the tab background
  hBitmapTab[UNICORN_TAB] = (HBITMAP)LoadImage(NULL, "Assets\\unicorntab_bg.bmp",
    IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
  if (hBitmapTab[UNICORN_TAB] == NULL) {
    issuccess = FALSE;
    strcat_s(errors, sizeof errors, "unicorntab_bg.bmp ");
  }
  hBitmapTab[UPGRADE_TAB] = (HBITMAP)LoadImage(NULL, "Assets\\upgradetab_bg.bmp",
    IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
  if (hBitmapTab[UPGRADE_TAB] == NULL) {
    issuccess = FALSE;
    strcat_s(errors, sizeof errors, "unicorntab_bg.bmp ");
  }
  hBitmapTab[HAND_TAB] = (HBITMAP)LoadImage(NULL, "Assets\\handtab_bg.bmp",
    IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
  if (hBitmapTab[HAND_TAB] == NULL) {
    issuccess = FALSE;
    strcat_s(errors, sizeof errors, "unicorntab_bg.bmp ");
  }

  // TODO: (maybe) use file I/O to read directories and copy the file names to load; could possibly parse XML or csv files too
  char cardname[64];
  for (int i = 0; i < 13; i++) {
    snprintf(cardname, sizeof cardname, "Assets\\Cards\\default_%03d.bmp", i + 1);
    hBitmapCard[i] = (HBITMAP)LoadImage(NULL, cardname,
      IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    if (hBitmapCard[i] == NULL) {
      issuccess = FALSE;
      strcat_s(errors, sizeof errors, cardname);
      strcat_s(errors, sizeof errors, " ");
    }
  }
  hBitmapCard[13] = (HBITMAP)LoadImage(NULL, "Assets\\back.bmp",
    IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
  if (hBitmapCard[0] == NULL) {
    issuccess = FALSE;
    strcat_s(errors, sizeof errors, "back.bmp ");
  }
  hBitmapCard[14] = (HBITMAP)LoadImage(NULL, "Assets\\default_superneigh.bmp",
    IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
  if (hBitmapCard[1] == NULL) {
    issuccess = FALSE;
    strcat_s(errors, sizeof errors, "default_superneigh.bmp ");
  }

  if (issuccess == FALSE) {
    MessageBox(hWnd, errors, "Error", MB_OK);
  }
}

void InitFonts(HWND hWnd) {
  RECT rc;
  const int pntpi = 72; // points-per-inch
  int pxpi, points, pxheight;

  HDC hdc = GetDC(hWnd);

  fonts[OLDFONT] = GetCurrentObject(hdc, OBJ_FONT); // save old font

  pxpi = GetDeviceCaps(hdc, LOGPIXELSY); // pixels-per-inch

  points = 30;
  pxheight = -(points * pxpi / pntpi);
  fonts[CHONKYFONT] = CreateFontA(pxheight, 0, // size
    0, 0,               // normal orientation
    FW_BOLD,            // normal weight--e.g., bold would be FW_BOLD
    NULL, NULL, NULL,   // not italic, underlined or strike out
    DEFAULT_CHARSET,
    OUT_OUTLINE_PRECIS, // select only outline (not bitmap) fonts
    CLIP_DEFAULT_PRECIS,
    CLEARTYPE_QUALITY,
    VARIABLE_PITCH | FF_SWISS,
    "Arial");

  points = 16;
  pxheight = -(points * pxpi / pntpi);
  fonts[BOLDFONT] = CreateFontA(pxheight, 0, // size
    0, 0,               // normal orientation
    FW_BOLD,            // normal weight--e.g., bold would be FW_BOLD
    NULL, NULL, NULL,   // not italic, underlined or strike out
    DEFAULT_CHARSET,
    OUT_OUTLINE_PRECIS, // select only outline (not bitmap) fonts
    CLIP_DEFAULT_PRECIS,
    CLEARTYPE_QUALITY,
    VARIABLE_PITCH | FF_SWISS,
    "Arial");

  points = 11;
  pxheight = -(points * pxpi / pntpi);
  fonts[ITALICFONT] = CreateFontA(pxheight, 0, // size
    0, 0,               // normal orientation
    FW_NORMAL,          // normal weight--e.g., bold would be FW_BOLD
    TRUE, NULL, NULL,   // italic, not underlined or strike out
    DEFAULT_CHARSET,
    OUT_OUTLINE_PRECIS, // select only outline (not bitmap) fonts
    CLIP_DEFAULT_PRECIS,
    CLEARTYPE_QUALITY,
    VARIABLE_PITCH | FF_SWISS,
    "Times New Roman");

  LPCWSTR font_resource = L"Assets\\runescape_large.ttf";
  AddFontResource(font_resource);

  points = 24;
  pxheight = -(points * pxpi / pntpi);
  fonts[FANCYFONT] = CreateFontA(pxheight, 0, // size
    0, 0,                 // normal orientation
    FW_NORMAL,            // normal weight--e.g., bold would be FW_BOLD
    NULL, NULL, NULL,     // not italic, underlined or strike out
    DEFAULT_CHARSET,
    OUT_OUTLINE_PRECIS,   // select only outline (not bitmap) fonts
    CLIP_DEFAULT_PRECIS,
    CLEARTYPE_QUALITY,
    VARIABLE_PITCH | FF_SWISS,
    "RuneScape Large");

  fonts[FANCYITALICS] = CreateFontA(pxheight, 0, // size
    0, 0,                 // normal orientation
    FW_NORMAL,            // normal weight--e.g., bold would be FW_BOLD
    TRUE, NULL, NULL,     // italic, not underlined or strike out
    DEFAULT_CHARSET,
    OUT_OUTLINE_PRECIS,   // select only outline (not bitmap) fonts
    CLIP_DEFAULT_PRECIS,
    CLEARTYPE_QUALITY,
    VARIABLE_PITCH | FF_SWISS,
    "RuneScape Large");
}

void DestroyFonts() {
  for (int i = 1; i < sizeof fonts / sizeof fonts[0]; i++) {
    DeleteObject(fonts[i]);
  }
  RemoveFontResource(L"Assets\\runescape_large.ttf");
}

void InitDebugMode() {
  // add some arbitrary number of cards to the player's stable for testing purposes
  // TODO: if I want to quickly edit future tests, then I can read text files for easier input and testing

  // test 1
  // 
  // // this should output 7 unicorns in the first page and 1 in the 2nd page
  // // WILL BUG OUT if i keep the stable_size at 25 instead of 100+, so use
  // // this test specifically for the deck later on
  // for (int i = 0; i < 80; i++) {
  //   player[0].stable.unicorns[i] = (i % 10) ? 128 : 30;
  //   if (checkClass(ANYUNICORN, deck[player[0].stable.unicorns[i]].class))
  //     player[0].stable.num_unicorns++;
  //   player[0].stable.size++;
  // }

  // test 2
  // 
  // // this should output 7 unicorns in the first page and 1 in the 2nd page
  // strcpy_s(player[0].username, sizeof player[0].username, "host");
  // player[0].icon = BABYNARWHAL;
  // for (int i = 0; i < 15; i++) {
  //   player[0].stable.unicorns[i] = (i & 1) ? 128 : 30;
  //   if (checkClass(ANYUNICORN, deck[player[0].stable.unicorns[i]].class))
  //     player[0].stable.num_unicorns++;
  //   player[0].stable.size++;
  // }

  srand(time(NULL));

  // player 1: host
  // this should output 7 unicorns in the first page and 1 in the 2nd page
  strcpy_s(player[0].username, sizeof player[0].username, "host");
  // strcpy_s(player[0].username, sizeof player[0].username, "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW");
  player[0].icon = BABYNARWHAL;
  for (int i = 0; i < 10; i++) {
    BOOL isvalid = FALSE;
    do {
      player[0].stable.unicorns[i] = NURSERY_SIZE + rand() % 116;
      if (checkClass(ANYUNICORN, deck[player[0].stable.unicorns[i]].class) ||
        checkClass(UPGRADE, deck[player[0].stable.unicorns[i]].class) ||
        checkClass(DOWNGRADE, deck[player[0].stable.unicorns[i]].class)) {
        isvalid = TRUE;
      }
    } while (!isvalid);

    if (checkClass(ANYUNICORN, deck[player[0].stable.unicorns[i]].class))
      player[0].stable.num_unicorns++;
    player[0].stable.size++;
  }
  for (int i = 0; i < 5; i++) {
    // player[0].hand.cards[i] = 60; // #60 is shark with a horn
    player[0].hand.cards[i] = NURSERY_SIZE + rand() % 116;
    player[0].hand.num_cards++;
  }

  // player 2: noob
  strcpy_s(player[1].username, sizeof player[1].username, "nooblet");
  player[1].icon = rand() % 12;
  int handsize = 1 + rand() % 7;
  for (int i = 0; i < 5; i++) {
    player[1].stable.unicorns[i] = (i & 1) ? 50 : 40;
    if (checkClass(ANYUNICORN, deck[player[1].stable.unicorns[i]].class))
      player[1].stable.num_unicorns++;
    player[1].stable.size++;
  }
  for (int i = 0; i < handsize; i++) {
    player[1].hand.cards[i] = 42 + i;
    player[1].hand.num_cards++;
  }

  current_players = 2;
  pnumindex = 0;
}

void ResetDebugMode() {
  // reset player's cards/stable/etc. to 0;
  // TODO: this should be implemented as a hard reset when you join the game, but for now this will close off the init function
  for (int i = 0; i < MAX_PLAYERS; i++) {
    memset(player[i].stable.unicorns, 0, sizeof player[i].stable / sizeof player[i].stable.unicorns[0]);
    memset(player[i].hand.cards, 0, sizeof player[i].hand.cards / sizeof player[i].hand.cards[0]);
    memset(player[i].username, 0, sizeof player[i].username / sizeof player[i].username[0]);
    player[i].stable.num_unicorns = 0;
    player[i].stable.size = 0;
    player[i].hand.num_cards = 0;
  }
  current_players = 1;
}

// ********************************************************************************
// ********************* general game logic helper functions **********************
// ********************************************************************************

int SelectBabyUnicorn(int pnum, POINT pnt) {
  for (int i = 0; i < 13; i++) {
    if (pnt.y >= babies[i].top && pnt.y <= babies[i].bottom &&
        pnt.x >= babies[i].left && pnt.x <= babies[i].right &&
        babytoggle[i] == FALSE) {
      babytoggle[i] = TRUE;
      // checks if the player selected a previous baby unicorn just to make sure the 
      // player unicorn index isn't either garbage info or mistakenly zero when someone else may have
      // zero as an actual index in use (i.e. the red baby unicorn)
      if (pselect[pnum].left != 0) {
        // resets babytoggle at old position (since baby toggle goes from 0-12 just like the
        // baby unicorn card numbers, player[num].stable.unicorns[0] can be used as an index)
        babytoggle[player[pnum].stable.unicorns[0]] = FALSE;
      }
      player[pnum].stable.unicorns[0] = i;
      pselect[pnum].left = babies[i].left - 7; // the actual border should be offset by 7 pixels
      pselect[pnum].top = babies[i].top;
      return 1;
    }
  }

  return 0;
}

// ********************************************************************************
// *************************** game state wm functions ****************************
// ********************************************************************************

StateManager stateMach[NUMSTATES];

void InitStateMachine() {
  // stateMach = (StateManager*)malloc(NUMSTATES * sizeof(StateManager));
  stateMach[TITLEBLANK] = (StateManager){ TITLEBLANK, PaintTitle, HoverTitle, ClickTitle };
  stateMach[RULESONE]   = (StateManager){ RULESONE  , NULL      , NULL      , ClickRules };
  stateMach[RULESTWO]   = (StateManager){ RULESTWO  , NULL      , NULL      , ClickRules };
  stateMach[LOBBY]      = (StateManager){ LOBBY     , PaintLobby, NULL      , ClickLobby };
  stateMach[GAMESTART]  = (StateManager){ GAMESTART , PaintDebug, HoverDebug, ClickDebug };
  stateMach[DEBUGMODE]  = (StateManager){ DEBUGMODE , PaintDebug, HoverDebug, ClickDebug };
}

// WM_MOUSEMOVE

void HoverTitle(POINT pnt) {
  hornbutton.source = FALSE;

  for (int i = 0; i < 3; i++) {
    if (pnt.x >= titleButtons[i].x && pnt.x <= titleButtons[i].x + titleButtons[i].width &&
      pnt.y >= titleButtons[i].y && pnt.y <= titleButtons[i].y + titleButtons[i].height) {
        HornPosition(titleButtons[i]);
        return;
    }
  }
}

void HoverDebug(POINT pnt) {
  hoverTip.ishover = FALSE;

  for (int i = 0; i < sizeof cardslots / sizeof cardslots[0]; i++) {
    if (pnt.x >= cardslots[i].x && pnt.x < cardslots[i].x + cardslots[i].width &&
      pnt.y >= cardslots[i].y && pnt.y < cardslots[i].y + cardslots[i].height) {
      if (cardslots[i].source != NULL) {
        hoverTip = ReturnCardHoverTip(cardslots[i]);
        return;
      }
    }
  }

  for (int i = 0; i < current_players; i++) {
    if (pnt.x >= player_nums[i].x && pnt.x < player_nums[i].x + player_nums[i].width &&
      pnt.y >= player_nums[i].y && pnt.y < player_nums[i].y + player_nums[i].height) {
      hoverTip = ReturnPlayerHoverTip(i, player_nums[i].x, player_nums[i].y);
      return;
    }
  }
}

// WM_LBUTTONDOWN

void ClickTitle(POINT pnt) {
  if (pnt.x >= 0 && pnt.x <= 50 && pnt.y >= 0 && pnt.y <= 50) {
    menustate = DEBUGMODE;
    tabnum = UNICORN_TAB;
    InitDebugMode(); // for card testing purposes
    return;
  }

  for (int i = 0; i < sizeof titleButtons / sizeof titleButtons[0]; i++) {
    if (pnt.x >= titleButtons[i].x && pnt.x < titleButtons[i].x + titleButtons[i].width &&
      pnt.y >= titleButtons[i].y && pnt.y < titleButtons[i].y + titleButtons[i].height) {
      // left click action
      if (windowOpen[1] == FALSE) {
        titleButtons[i].onClick(titleButtons[i].source);
        hornbutton.source = FALSE;
        return;
      }
    }
  }
}

void ClickRules(POINT pnt) {
  for (int i = 0; i < sizeof ruleButtons / sizeof ruleButtons[0]; i++) {
    if (pnt.x >= ruleButtons[i].x && pnt.x < ruleButtons[i].x + ruleButtons[i].width &&
      pnt.y >= ruleButtons[i].y && pnt.y < ruleButtons[i].y + ruleButtons[i].height) {
      // left click action
      ruleButtons[i].onClick(ruleButtons[i].source);
      return;
    }
  }
}

volatile DWORD whatever;
void ClickLobby(POINT pnt) {
  int ret;
  DWORD waitResult;

  // TODO: (maybe) change the structure of RECTs [babies] into a general button structure tying into the whole GetAsyncKeyState if statement w/ the start/leave buttons
  // TODO: implement this with R-trees (specifically because the selectbabyunicorn function
  // has a lot of "buttons") so that it's more optimally chunked into bigger pieces

  // TODO: use a mutex to selectively access current_players, networktoggle, and clientPnt
  // waitResult = WaitForSingleObject(
  //   mutex,      // mutex handle
  //   INFINITE);  // no time-out interval
  // 
  // switch (waitResult) {
  // case WAIT_OBJECT_0:
  //   if (pnt.y >= 590 && pnt.y <= 639) {
  //     if (pnt.x >= 360 && pnt.x <= 549) {
  //       // user clicked the leave button
  //       networktoggle ^= 1;
  //       if (!isclient) {
  //         closesocket(udpfd);
  //       }
  //       return;
  //     }
  //     else if (pnt.x >= 120 && pnt.x <= 309) {
  //       // user clicked the start button; only the host can properly start the game
  //       if (!isclient && current_players >= 2) {
  //         networktoggle ^= 4;
  //         closesocket(udpfd);
  //       }
  //       return;
  //     }
  //   }
  // 
  //   if (isclient) {
  //     clientPnt = pnt;
  //     networktoggle ^= 2;
  //   }
  //   else {
  //     ret = SelectBabyUnicorn(0, pnt); // server is always player index 0
  //     if (ret) {
  //       networktoggle ^= 2;
  //     }
  //   }
  // 
  //   if (!ReleaseMutex(mutex)) {
  //     // handle error
  //   }
  //   break;
  // case WAIT_ABANDONED:
  //   // thread holding the mutex was killed rip in peace :'(
  //   return;
  // }

  for (int i = 0; i < sizeof lobbyButtons / sizeof lobbyButtons[0]; i++) {
    if (pnt.x >= lobbyButtons[i].x && pnt.x < lobbyButtons[i].x + lobbyButtons[i].width &&
      pnt.y >= lobbyButtons[i].y && pnt.y < lobbyButtons[i].y + lobbyButtons[i].height) {
      // left click action
      lobbyButtons[i].onClick(lobbyButtons[i].source);
      return;
    }
  }

  if (isclient) {
    clientPnt = pnt;
    networktoggle ^= 2;
  }
  else {
    ret = SelectBabyUnicorn(0, pnt); // server is always player index 0
    if (ret && current_players >= 2) {
      // only toggle it when there are actual players or else it won't untoggle on its own
      networktoggle ^= 2;
    }
  }
}

void ClickDebug(POINT pnt) {
  if (pnt.x >= 0 && pnt.x <= 50 && pnt.y >= 0 && pnt.y <= 50) {
    menustate = TITLEBLANK;
    ResetDebugMode(); // for card testing purposes
    tabnum = UNICORN_TAB;
    pagenum = 1;
    return;
  }

  // general tab buttons
  for (int i = 0; i < sizeof debugButtons / sizeof debugButtons[0]; i++) {
    if (pnt.x >= debugButtons[i].x && pnt.x < debugButtons[i].x + debugButtons[i].width &&
      pnt.y >= debugButtons[i].y && pnt.y < debugButtons[i].y + debugButtons[i].height) {
      // left click action
      debugButtons[i].onClick(debugButtons[i].source);
      return;
    }
  }

  // select the player to view
  // 
  // TODO: should probably only initialize player_nums up to the current_player count instead of the full MAX_PLAYERS count
  // anything with player_nums will bug out unless the loops go up to current_players, so limiting the player_nums array
  // or initializing it as negative numbers would reduce the possibility of more bugs popping up
  // 
  // TODO: will need to add an additional action here (or in a separate state) for whenever the user has to select a player to
  // apply their card effect on; could also potentially just make another window for those cases, but that sounds lame :/
  for (int i = 0; i < current_players; i++) {
    if (pnt.x >= player_nums[i].x && pnt.x < player_nums[i].x + player_nums[i].width &&
      pnt.y >= player_nums[i].y && pnt.y < player_nums[i].y + player_nums[i].height) {
      pnumindex = i;
      pagenum = 1;

      // this will switch to the new player window in case the user previously looked at the nursery, deck, or discard piles
      if (tabnum == NURSERY_TAB || tabnum == DECK_TAB || tabnum == DISCARD_TAB) {
        tabnum = UNICORN_TAB;
      }
      return;
    }
  }
}

// WM_PAINT

void PaintTitle(HDC hdc, HDC *hdcMem) {
  HDC hdcSprite;
  HGDIOBJ oldSprite;

  hdcSprite = CreateCompatibleDC(hdc);

  if ((BOOL)hornbutton.source) {
    oldSprite = SelectObject(hdcSprite, hornbutton.bitmap);
    TransparentBlt(*hdcMem, hornbutton.x, hornbutton.y, hornbutton.width, hornbutton.height, hdcSprite, 0, 0, hornbutton.width, hornbutton.height, RGB(0, 255, 0));
    SelectObject(hdcSprite, oldSprite);
  }
  DeleteDC(hdcSprite);
}

void PaintLobby(HDC hdc, HDC *hdcMem) {
  RECT rc;
  HDC hdcSprite;
  HGDIOBJ oldSprite;

  SetBkMode(*hdcMem, TRANSPARENT); // box surrounding text is transparent instead of white

  // type out lobby code
  SetRect(&rc, 78, 180, 1202, 225);
  SetTextColor(*hdcMem, RGB(255, 255, 255));
  SelectObject(*hdcMem, fonts[CHONKYFONT]);
  DrawText(*hdcMem, hexcode, strlen(hexcode), &rc, DT_CENTER);

  // type out party members
  SetRect(&rc, 115, 260, 800, 625);
  SelectObject(*hdcMem, fonts[OLDFONT]);
  DrawText(*hdcMem, partymems, strlen(partymems), &rc, DT_LEFT);

  // draw border for chosen baby unicorns (ughhhh)
  hdcSprite = CreateCompatibleDC(hdc);
  for (int i = 0; i < current_players; i++) {
    if (pselect[i].left != 0) {
      oldSprite = SelectObject(hdcSprite, hBitmapBorder[i]);
      TransparentBlt(*hdcMem, pselect[i].left, pselect[i].top, BORDERWIDTH, BORDERWIDTH, hdcSprite, 0, 0, BORDERWIDTH, BORDERWIDTH, RGB(0, 255, 0));
      SelectObject(hdcSprite, oldSprite);
    }
  }
  DeleteDC(hdcSprite);
}

void PaintDebug(HDC hdc, HDC *hdcMem) {
  HDC hdcSprite;
  HGDIOBJ oldSprite;

  hdcSprite = CreateCompatibleDC(hdc);

  // display the player boxes on the right side of the in-game screen
  for (int i = 0; i < current_players; i++) {
    // player icon in-game
    oldSprite = SelectObject(hdcSprite, icons[player[i].icon].bitmap);
    TransparentBlt(*hdcMem, player_nums[i].x, player_nums[i].y, player_nums[i].width, player_nums[i].height, hdcSprite, 0, 0, player_nums[i].width, player_nums[i].height, RGB(0, 255, 0));
    SelectObject(hdcSprite, oldSprite);

    // player number
    oldSprite = SelectObject(hdcSprite, player_nums[i].bitmap);
    TransparentBlt(*hdcMem, player_nums[i].x, player_nums[i].y, player_nums[i].width, player_nums[i].height, hdcSprite, 0, 0, player_nums[i].width, player_nums[i].height, RGB(0, 255, 0));
    SelectObject(hdcSprite, oldSprite);

    // number of unicorns in stable
    if (player[i].stable.num_unicorns < 7) {
      oldSprite = SelectObject(hdcSprite, stable_nums[player[i].stable.num_unicorns].bitmap);
    }
    else {
      oldSprite = SelectObject(hdcSprite, stable_nums[7].bitmap);
    }
    TransparentBlt(*hdcMem, player_nums[i].x, player_nums[i].y, player_nums[i].width, player_nums[i].height, hdcSprite, 0, 0, player_nums[i].width, player_nums[i].height, RGB(0, 255, 0));
    SelectObject(hdcSprite, oldSprite);
  }

  // display the nursery and discard pile if they are not empty
  if (nursery_index < NURSERY_SIZE) {
    oldSprite = SelectObject(hdcSprite, hBitmapCard[nursery_ref[nursery_index]]);
    BitBlt(*hdcMem, debugButtons[NURSERY_TAB].x, debugButtons[NURSERY_TAB].y, debugButtons[NURSERY_TAB].width, debugButtons[NURSERY_TAB].height, hdcSprite, 0, 0, SRCCOPY);
    SelectObject(hdcSprite, oldSprite);
  }

  if (discard_index > 0) {
    oldSprite = SelectObject(hdcSprite, hBitmapCard[discard_ref[discard_index]]);
    BitBlt(*hdcMem, debugButtons[DISCARD_TAB].x, debugButtons[DISCARD_TAB].y, debugButtons[DISCARD_TAB].width, debugButtons[DISCARD_TAB].height, hdcSprite, 0, 0, SRCCOPY);
    SelectObject(hdcSprite, oldSprite);

  }

  // display the list of cards under the current tab
  DisplayCardWindow(hdcMem, &hdcSprite);
  DeleteDC(hdcSprite);

  if (hoverTip.ishover) {
    CreateCustomToolTip(hdcMem, hoverTip);
  }
}

// ********************************************************************************
// *************************** button function pointers ***************************
// ********************************************************************************

void SwitchState(int statenum) {
  menustate = statenum;
}

void HostGeneration(HWND hWnd) {
  MSG msg;

  // https://stackoverflow.com/questions/17202377/c-creating-a-window-in-a-new-thread
  // this won't work in a thread unless there's a message loop, but that would create bliting issues
  CreateHostWindow(hWnd);

  while (windowOpen[1]) {
    if (GetMessage(&msg, NULL, 0, 0)) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
  }
}

void JoinGeneration(HWND hWnd) {
  MSG msg;

  // https://stackoverflow.com/questions/17202377/c-creating-a-window-in-a-new-thread
  // this won't work in a thread unless there's a message loop, but that would create bliting issues
  CreateJoinWindow(hWnd);

  while (windowOpen[1]) {
    if (GetMessage(&msg, NULL, 0, 0)) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
  }
}

void LeaveLobby() {
  // user clicked the leave button
  networktoggle ^= 1;
  if (!isclient) {
    closesocket(udpfd);
  }

  // menustate will switch in the server file
}

void StartGame() {
  // user clicked the start button; only the host can properly start the game
  if (!isclient && current_players >= 2) {
    networktoggle ^= 4;
    // closesocket(udpfd); // TODO: (look into this) i might still need the udp socket for instant neigh shenanigans
  }

  // menustate will switch in the server file
}

void SwitchTab(int num) {
  tabnum = num;
  pagenum = 1;
}

void TurnPage(int page) {
  if (page == PAGE_RIGHT && tabsize > pagenum * 7) {
    pagenum++;
  }
  else if (page == PAGE_LEFT && pagenum > 1) {
    pagenum--;
  }
}

// ********************************************************************************
// ****************************** callback functions ******************************
// ********************************************************************************


//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_CREATE       - initialize data when the window is created
//  WM_ACTIVATE     - trigger when the window is being activated or deactivated
//  WM_TIMER        - run when the timer expires
//  WM_ERASEBKGND   - prepare an invalidated portion of a window for painting
//  WM_MOUSEMOVE    - trigger upon mouse movement across the window
//  WM_LBUTTONDOWN  - trigger upon left mouse clicks
//  WM_PAINT        - Paint the main window
//  WM_DESTROY      - post a quit message and return
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  HDC hdc;
  PAINTSTRUCT ps;
  POINT pnt;

  HDC hdcMem;
  HGDIOBJ oldBitmap, oldSprite;
  HBITMAP hBitmapBuffer;

  HDC hdcSprite;
  HBITMAP hbmSprite;

  switch (uMsg)
  {
  case WM_CREATE:
    LoadImages(hWnd);
    InitFonts(hWnd);
    InitButtonManager(hWnd);
    InitStateMachine();
    windowOpen[0] = TRUE;
    webhwnd = hWnd; // for server and client to access in the wsapoll lobby loop
    break;
  case WM_TIMER:
    InvalidateRect(hWnd, NULL, FALSE); // TRUE = bg is erased when BeginPaint is called; FALSE = no
    break;
  case WM_ERASEBKGND:
    return (LRESULT)1; // Say we handled it.
  case WM_MOUSEMOVE: 
  {
    // check mousemove for assets that update through hovering
    GetCursorPos(&pnt);
    ScreenToClient(hWnd, &pnt);

    if (stateMach[menustate].stateHover != NULL)
      stateMach[menustate].stateHover(pnt);

    break;
  }
  case WM_LBUTTONDOWN:
  {
    // for clicking purposes
    GetCursorPos(&pnt);
    ScreenToClient(hWnd, &pnt);

    // if (wParam == WA_ACTIVE)
    stateMach[menustate].stateClick(pnt);

    break;
  }
  case WM_PAINT:
  {
    // TODO: DON'T use UpdateLayeredWindow() instead of WM_PAINT, but it's nice to know about though
    // https://stackoverflow.com/questions/67689087/correct-way-to-handle-and-redraw-a-layered-window-with-a-bitmap

    // validate that HWND
    hdc = BeginPaint(hWnd, &ps);

    // create memory DC and memory bitmap where we shall do our drawing
    hdcMem = CreateCompatibleDC(hdc);
    // create a deep copy so that the original image doesn't get updated when bitblt'ing the extra info to the memory bitmap
    hBitmapBuffer = (HBITMAP)CopyImage(hBitmapBG[menustate], IMAGE_BITMAP, BWIDTH, BHEIGHT, LR_COPYRETURNORG);
    oldBitmap = SelectObject(hdcMem, hBitmapBuffer);

    if (stateMach[menustate].statePaint != NULL) {
      stateMach[menustate].statePaint(hdc, &hdcMem);
    }

    // render memory buffer to on-screen DC
    BitBlt(hdc, 0, 0, BWIDTH, BHEIGHT, hdcMem, 0, 0, SRCCOPY);

    // cleanup, be aware of memory leaks >.<
    SelectObject(hdcMem, oldBitmap);
    DeleteDC(hdcMem);
    DeleteObject(hBitmapBuffer);
    EndPaint(hWnd, &ps);
    break;
  }
  case WM_DESTROY:
  {
    windowOpen[0] = FALSE;
    for (int i = 0; i < sizeof hBitmapBG / sizeof hBitmapBG[0]; i++) {
      DeleteObject(hBitmapBG[i]);
    }
    for (int i = 0; i < sizeof hBitmapBorder / sizeof hBitmapBorder[0]; i++) {
      DeleteObject(hBitmapBorder[i]);
    }
    for (int i = 0; i < sizeof icons / sizeof icons[0]; i++) {
      DeleteObject(icons[i].bitmap);
    }
    for (int i = 0; i < sizeof player_nums / sizeof player_nums[0]; i++) {
      DeleteObject(player_nums[i].bitmap);
    }
    for (int i = 0; i < sizeof stable_nums / sizeof stable_nums[0]; i++) {
      DeleteObject(stable_nums[i].bitmap);
    }
    for (int i = 0; i < sizeof debugButtons / sizeof debugButtons[0]; i++) {
      DeleteObject(debugButtons[i].bitmap); // TODO: this is looping extra because there are only 2 buttons w/ bitmaps
    }
    for (int i = 0; i < sizeof hBitmapTab / sizeof hBitmapTab[0]; i++) {
      DeleteObject(hBitmapTab[i]);
    }
    for (int i = 0; i < sizeof hBitmapCard / sizeof hBitmapCard[0]; i++) {
      DeleteObject(hBitmapCard[i]);
    }
    DestroyFonts();
    PostQuitMessage(0);
    CloseHandle(mutex);
    WSACleanup();
    break;
  }
  }

  return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK WndProcHost(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  // char tmpuser[NAME_SIZE * 2] = { 0 };
  char portstr[NAME_SIZE] = { 0 };
  int count = 0, portno;
  PAINTSTRUCT ps;
  HDC hdc;
  RECT rc;

  switch (uMsg)
  {
  case WM_CREATE:
    windowOpen[1] = TRUE;

    textNameHwnd = CreateWindow(TEXT("EDIT"), NULL, WS_BORDER | WS_CHILD | WS_VISIBLE, 10, 50, 200, 20, hWnd, (HMENU)ID_TEXT, hInstanceGlob, NULL);
    portHwnd = CreateWindow(TEXT("EDIT"), NULL, WS_BORDER | WS_CHILD | WS_VISIBLE, 10, 145, 200, 20, hWnd, (HMENU)ID_TEXT, hInstanceGlob, NULL);
    CreateWindow(TEXT("BUTTON"), TEXT("Submit"), WS_CHILD | WS_VISIBLE, 150, 200, 100, 30, hWnd, (HMENU)ID_BUTTON, hInstanceGlob, NULL);

    break;
  case WM_COMMAND:
  {
    switch (LOWORD(wParam))
    {
    case ID_BUTTON:
      // for some reason, the returned string is split by null '\0' for each character
      // e.g. if user puts "apple" then the return is "a \0 p \0 p \0 l \0 e \0 \0 \0..."
      // This only happens with unicode? probably because the buffer is char* instead of TCHAR
      // GetWindowText(textNameHwnd, tmpuser, NAME_SIZE); // buffer overrun when namesize * 2??? why
      // for (int i = 0; i < NAME_SIZE * 2 - 2; i += 2) {
      //   if (tmpuser[i] != '\0')
      //     player[0].username[count++] = tmpuser[i];
      //   else
      //     break;
      // }
      GetWindowText(textNameHwnd, player[0].username, NAME_SIZE);
      GetWindowText(portHwnd, portstr, NAME_SIZE);

      // TODO: put this under some ifdef so that it doesn't affect future deployment
      if (player[0].username[0] == '\0') strcpy_s(player[0].username, 5, "host");
      if (portstr[0] == '\0') strcpy_s(portstr, 5, "1234");
      portno = atoi(portstr);

      // input check for username and port #
      if (player[0].username[0] != '\0' && portno >= 1024 && portno <= 65535) {
        // close this window and switch to lobby window
        getIPreq(ip);
        char localIPcode[16];
        getLocalIP(localIPcode);
        sprintf_s(hexcode, sizeof(hexcode), "Wi-Fi Code: %08X, Local Code: %08X", IPtoHexCode(ip), IPtoHexCode(localIPcode));
        sprintf_s(partymems, PARTYSTRSIZE, "1. %s (host)", player[0].username);
        
        mutex = CreateMutex(
          NULL,              // default security attributes
          FALSE,             // initially not owned
          NULL);             // unnamed mutex

        if (mutex == NULL)
        {
          MessageBoxA(NULL,
            _T("CreateMutex error before creating network thread :("),
            _T("Server Connection"),
            NULL);
          DestroyWindow(hWnd);
        }

        //serverInit(portno);
        networkthread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)serverInit, portno, 0, &tIdweb);

        menustate = LOBBY;
        DestroyWindow(hWnd);
      }

      break;
    }
  }
  break;
  case WM_PAINT:
    hdc = BeginPaint(hWnd, &ps);
    SetBkMode(hdc, TRANSPARENT); // box surrounding text is transparent instead of white

    TextOut(hdc, 10, 25, TEXT("Enter desired username:"), strlen("Enter desired username:"));
    SetRect(&rc, 10, 100, 300, 140);
    // if nCount is -1, then the lpchText parameter is assumed to be a pointer to a null-terminated
    // string and DrawText computes the character count automatically
    DrawText(hdc, TEXT("Enter valid port number\n(between 1024 and 65535 inclusive):"), -1, &rc, DT_LEFT | DT_WORDBREAK);

    EndPaint(hWnd, &ps);
    ReleaseDC(hWnd, hdc);
    break;
  case WM_DESTROY:
    windowOpen[1] = FALSE;
    break;
  }

  return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK WndProcJoin(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  char portstr[NAME_SIZE] = { 0 }, codestr[9] = { 0 };
  int count = 0, portno;
  PAINTSTRUCT ps;
  HDC hdc;
  RECT rc;

  switch (uMsg)
  {
  case WM_CREATE:
    windowOpen[1] = TRUE;

    textNameHwnd = CreateWindow(TEXT("EDIT"), NULL, WS_BORDER | WS_CHILD | WS_VISIBLE, 10, 40, 200, 20, hWnd, (HMENU)ID_TEXT, hInstanceGlob, NULL);
    portHwnd = CreateWindow(TEXT("EDIT"), NULL, WS_BORDER | WS_CHILD | WS_VISIBLE, 10, 115, 200, 20, hWnd, (HMENU)ID_TEXT, hInstanceGlob, NULL);
    codeHwnd = CreateWindow(TEXT("EDIT"), NULL, WS_BORDER | WS_CHILD | WS_VISIBLE, 10, 170, 200, 20, hWnd, (HMENU)ID_TEXT, hInstanceGlob, NULL);
    CreateWindow(TEXT("BUTTON"), TEXT("Submit"), WS_CHILD | WS_VISIBLE, 150, 210, 100, 30, hWnd, (HMENU)ID_BUTTON, hInstanceGlob, NULL);

    break;
  case WM_COMMAND:
  {
    switch (LOWORD(wParam))
    {
    case ID_BUTTON:
      GetWindowText(textNameHwnd, player[0].username, NAME_SIZE - 1); // -1 to account for the null variable :)
      GetWindowText(portHwnd, portstr, NAME_SIZE);
      GetWindowText(codeHwnd, codestr, 9);
      
      // TODO: put this under some ifdef so that it doesn't affect future deployment
      if (codestr[0] == '\0') strcpy_s(codestr, 9, "142B8F0B");
      if (portstr[0] == '\0') strcpy_s(portstr, 5, "1234");
      portno = atoi(portstr);

      // TODO: add support for inserting ip codestr's directly instead of just some "obfuscated" hex value
      // input check for username and port #
      if (player[0].username[0] != '\0' && portno >= 1024 && portno <= 65535 && strlen(codestr) == 8) {
        // local ip 127.0.0.1: 142B8F0B
        HexCodetoIP(codestr, ip);

        //clientJoin(portno);
        networkthread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)clientJoin, portno, 0, &tIdweb);

        // TODO: (very low priority) add a "loading" state as an intermediate period while connecting to the host
        DestroyWindow(hWnd);
      }

      break;
    }
  }
  break;
  case WM_PAINT:
    hdc = BeginPaint(hWnd, &ps);
    SetBkMode(hdc, TRANSPARENT); // box surrounding text is transparent instead of white

    TextOut(hdc, 10, 15, TEXT("Enter desired username:"), strlen("Enter desired username:"));
    SetRect(&rc, 10, 75, 300, 115);
    // if nCount is -1, then the lpchText parameter is assumed to be a pointer to a null-terminated
    // string and DrawText computes the character count automatically
    DrawText(hdc, TEXT("Enter valid port number\n(between 1024 and 65535 inclusive):"), -1, &rc, DT_LEFT | DT_WORDBREAK);
    TextOut(hdc, 10, 150, TEXT("Enter room code:"), strlen("Enter room code:"));

    EndPaint(hWnd, &ps);
    ReleaseDC(hWnd, hdc);
    break;
  case WM_DESTROY:
    windowOpen[1] = FALSE;
    break;
  }

  return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
