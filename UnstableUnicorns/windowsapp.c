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
// toggle for if window is currently active; 0 = no, 1 = yes :)
BOOL is_active = TRUE;
RECT pselect[MAX_PLAYERS];
// toggle to see whether or not a baby unicorn is taken during lobby selection; 0 = free, 1 = taken
BOOL babytoggle[13] = { FALSE };
// switch to activate certain networking features in the network thread to avoid improperly closing a socket
// this is super ugly :/
unsigned char networktoggle = 0;

// ********************************************************************************
// ****************** global variables for this source file only ******************
// ********************************************************************************
HWND gamethread, networkthread, textNameHwnd, portHwnd, codeHwnd;
DWORD tId, tIdweb;
WNDCLASSEX wcexHost;
WNDCLASSEX wcexJoin;
HBITMAP hBitmapTitle[9], hBitmapBorder[8], hBitmapCard[2];
BOOL windowOpen[2] = { FALSE };
BOOL childWindow[2] = { FALSE };
HFONT fonts[4] = { NULL };

int stablePadding = 15; // arbitrary number of pixels to pad between the displayed list of cards in their respective stables/hands/decks

enum BabySelection {BABYRED, BABYPINK, BABYORANGE, BABYYELLOW, BABYGREEN, BABYBLUE, BABYPURPLE, BABYBLACK, BABYWHITE, BABYBROWN, BABYRAINBOW, BABYDEATH, BABYNARWHAL};
enum tab {UNICORN_TAB, UPGRADE_TAB, HAND_TAB, NURSERY_TAB, DECK_TAB, DISCARD_TAB};
enum butindex { PAGE_LEFT, PAGE_RIGHT };
enum fonts { OLDFONT, CHONKYFONT, BOLDFONT, FANCYFONT };


// left, top, right, bottom
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

// struct Button pageleft = { 3, 636, 80, 59 };
// struct Button pageright = { 1196, 636, 80, 59 };

// TODO: could split UI elements by game state, so the title screen, rules, and etc would have their own
struct Button buttons[] = {
  { 3, 636, 80, 59 , "Assets\\pageleft.bmp", NULL }, 
  { 1196, 636, 80, 59 , "Assets\\pageright.bmp", NULL }
};

// TODO: potentially rename these to icon[1-13]?
struct Button icons[] = {
  { 1064, 28, 95, 81, "Assets\\icon_red.bmp", NULL },
  { 1064, 28, 95, 81, "Assets\\icon_pink.bmp", NULL },
  { 1064, 28, 95, 81, "Assets\\icon_orange.bmp", NULL },
  { 1064, 28, 95, 81, "Assets\\icon_yellow.bmp", NULL },
  { 1064, 28, 95, 81, "Assets\\icon_green.bmp", NULL },
  { 1064, 28, 95, 81, "Assets\\icon_blue.bmp", NULL },
  { 1064, 28, 95, 81, "Assets\\icon_purple.bmp", NULL },
  { 1064, 28, 95, 81, "Assets\\icon_black.bmp", NULL },
  { 1064, 28, 95, 81, "Assets\\icon_white.bmp", NULL },
  { 1064, 28, 95, 81, "Assets\\icon_brown.bmp", NULL },
  { 1064, 28, 95, 81, "Assets\\icon_rainbow.bmp", NULL },
  { 1064, 28, 95, 81, "Assets\\icon_death.bmp", NULL },
  { 1064, 28, 95, 81, "Assets\\icon_narwhal.bmp", NULL }
};

struct Button player_nums[] = {
  { 1064, 28,  95, 81, "Assets\\player1.bmp", NULL },
  { 1175, 28,  95, 81, "Assets\\player2.bmp", NULL },
  { 1064, 118, 95, 81, "Assets\\player3.bmp", NULL },
  { 1175, 118, 95, 81, "Assets\\player4.bmp", NULL },
  { 1064, 208, 95, 81, "Assets\\player5.bmp", NULL },
  { 1175, 208, 95, 81, "Assets\\player6.bmp", NULL },
  { 1064, 298, 95, 81, "Assets\\player7.bmp", NULL },
  { 1175, 298, 95, 81, "Assets\\player8.bmp", NULL }
};
struct Button stable_nums[8];

#define BORDERWIDTH 97

// ********************************************************************************
// ****************************** button management *******************************
// ********************************************************************************

enum buttontypes { titlebuttons, rulebuttons, lobbybuttons, debugbuttons, numsubsets };

struct Button** buttonManager;
struct Button buttonManager2D[numsubsets - 1][5];
struct Button hornbutton;

// calculates the position for the horn select bitmap that centers around the hovered button
void HoverTitle(struct Button *self, BOOL *hover) {
  HGDIOBJ oldSprite;

  int xdiff = -93;
  int ydiff = 17;

  hornbutton.x = self->x + xdiff;
  hornbutton.y = self->y + ydiff;
  *hover = TRUE;
}

void InitTitleButtons(struct Button *b, HWND hWnd) {
  int size = 3;
  // *b = (struct Button*)calloc(size, sizeof(struct Button));

  // horn is at point 435, 402 for the first button
  for (int i = 0; i < size; i++) {
    b[i].x = 528;
    b[i].y = 385 + (i * 100);
    b[i].width = 230;
    b[i].height = 75;
    b[i].onHover = HoverTitle;
  }

  b[0].source = hWnd;
  b[1].source = hWnd;
  b[2].source = RULESONE;
  b[0].onClick = HostGeneration;
  b[1].onClick = JoinGeneration;
  b[2].onClick = SwitchState;

  // horn select
  hornbutton.width = 416;
  hornbutton.height = 40;
  hornbutton.bitmap = (HBITMAP)LoadImage(NULL, "Assets\\horn_select.bmp",
    IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
}

void InitRuleButtons(struct Button *b) {
  int size = 3;
  // b = (struct Button*)malloc(size * sizeof(struct Button));

  for (int i = 0; i < size; i++) {
    b[i].y = 619;
    b[i].width = 32;
    b[i].height = 32;
    b[i].onHover = NothingBurger;
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
  int size = 2;
  // b = (struct Button*)malloc(size * sizeof(struct Button));

  // start game
  b[0].x = 120;
  b[0].y = 590;
  b[0].width = 190;
  b[0].height = 50;
  b[0].source = GAMESTART;
  b[0].onHover = NothingBurger;
  b[0].onClick = StartGame;

  // leave lobby
  b[1].x = 360;
  b[1].y = 590;
  b[1].width = 190;
  b[1].height = 50;
  b[0].source = TITLEBLANK;
  b[1].onHover = NothingBurger;
  b[1].onClick = LeaveLobby;
}

void InitPlayerIcons() {
  char errors[1024] = "";
  BOOL issuccess = TRUE;

  for (int i = 0; i < sizeof icons / sizeof icons[0]; i++) {
    icons[i].bitmap = (HBITMAP)LoadImage(NULL, icons[i].filename,
      IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    if (icons[i].bitmap == NULL) {
      issuccess = FALSE;
      strcat_s(errors, sizeof errors, icons[i].filename);
      strcat_s(errors, sizeof errors, " ");
    }
  }

  for (int i = 0; i < sizeof player_nums / sizeof player_nums[0]; i++) {
    player_nums[i].bitmap = (HBITMAP)LoadImage(NULL, player_nums[i].filename,
      IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    if (player_nums[i].bitmap == NULL) {
      issuccess = FALSE;
      strcat_s(errors, sizeof errors, player_nums[i].filename);
      strcat_s(errors, sizeof errors, " ");
    }
    player_nums[i].source = &player[i];
    player_nums[i].onHover = ReturnPlayerHoverTip;
  }

  // stable count goes up to 7
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
}

void malloctest(struct Button** b) {
  int size = 3;
  *b = (struct Button*)calloc(size, sizeof(struct Button));
}

void InitButtonManager(HWND hWnd) {
  // buttonManager = (struct Button**)calloc((numsubsets - 1), sizeof(struct Button*));

  // InitTitleButtons(&buttonManager[titlebuttons]);

  // int size = 3;
  // malloctest(&buttonManager[titlebuttons]);
  // 
  // // horn is at point 435, 402 for the first button
  // for (int i = 0; i < size; i++) {
  //   buttonManager[titlebuttons][i].x = 528;
  //   buttonManager[titlebuttons][i].y = 385 + (i * 100);
  //   buttonManager[titlebuttons][i].width = 230;
  //   buttonManager[titlebuttons][i].height = 75;
  //   // b[i]->onHover = HoverTitle;
  //   // b[i]->onClick = SwitchState;
  // }
  // 
  // 
  // // source is just storing the triggered game state that should occur when clicking the button
  // buttonManager[titlebuttons][0].source = TITLEHOST;
  // buttonManager[titlebuttons][1].source = TITLEJOIN;
  // buttonManager[titlebuttons][2].source = RULESONE;
  // 
  // // horn select
  // hornbutton.width = 416;
  // hornbutton.height = 40;
  // hornbutton.bitmap = (HBITMAP)LoadImage(NULL, "Assets\\horn_select.bmp",
  //   IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

  InitTitleButtons(buttonManager2D[titlebuttons], hWnd);
  InitRuleButtons(buttonManager2D[rulebuttons]);
  InitLobbyButtons(buttonManager2D[lobbybuttons]);

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

void DisplayCardWindow(HDC* hdcMem, HDC* hdcSprite, int pagenum, int* tabsize, int tabnum) {
  // use pages to view the card window in lieu of scroll bars :)

  HGDIOBJ oldSprite;

  // start is the starting point for the display, of course, with the scroll bar it could go to 0
  POINT start = { 87, 507 };
  RECT windowTab = { 0, 463, 1279, 491 };
  BITMAP bm;
  GetObject(hBitmapCard[0], (int)sizeof bm, &bm); // fetches width/height; all cards have the same w/h

  int distance = stablePadding + bm.bmWidth;

  // cards are just rectangles, so transparentblt isn't necessary
  // this relies on an accurate unicorn/stable/card count; the pages will bork otherwise
  int index_start = (pagenum - 1) * 7;

  switch (tabnum) {
  case UNICORN_TAB:
  {
    *tabsize = player[0].stable.num_unicorns;
    // TODO: using the original index_start wouldn't account for skipped upgrade/downgrade cards,
    // but adding an extra array to check for offset cards within extra pages seems extremely overkill...
    // 
    // in any case, filtering is still necessary for specific deck functions, and fixing this one by
    // grouping the unicorns w/ upgrade/downgrade cards in a stable wouldn't fully solve the problem
    int j = 0;
    for (int count = 0; j < player[0].stable.size && count < (pagenum - 1) * 7; j++) {
      if (checkClass(ANYUNICORN, deck[player[0].stable.unicorns[j]].class))
        count++;
    }
    index_start = j;

    for (int i = index_start, skip = 0; i < player[0].stable.size && skip < 7; i++) {
      // for now, this cycles through hBitmapCard which only has the super neigh card and the back design
      // in the future, hBitmapCard could be replaced/assimilated by an updated Unicorn or Deck structure featuring the file name and the loaded hBitmap
      if (checkClass(ANYUNICORN, deck[player[0].stable.unicorns[i]].class)) {
        oldSprite = SelectObject(*hdcSprite, hBitmapCard[i & 1]);
        BitBlt(*hdcMem, start.x + (distance * skip), start.y, bm.bmWidth, bm.bmHeight, *hdcSprite, 0, 0, SRCCOPY);
        SelectObject(*hdcSprite, oldSprite);
        skip++;
      }
    }
    break;
  }
  case UPGRADE_TAB:
  {
    // TODO: see unicorn_tab for the indexing issues. this is unlikely to happen in a normal game though due to most players having less than 8 up/downgrades
    *tabsize = player[0].stable.size - player[0].stable.num_unicorns;
    for (int i = index_start, skip = 0; i < player[0].stable.size && skip < 7; i++) {
      // for now, this cycles through hBitmapCard which only has the super neigh card and the back design
      // in the future, hBitmapCard could be replaced/assimilated by an updated Unicorn or Deck structure featuring the file name and the loaded hBitmap
      if (deck[player[0].stable.unicorns[i]].class == UPGRADE || deck[player[0].stable.unicorns[i]].class == DOWNGRADE) {
        oldSprite = SelectObject(*hdcSprite, hBitmapCard[i % 2]);
        BitBlt(*hdcMem, start.x + (distance * skip), start.y, bm.bmWidth, bm.bmHeight, *hdcSprite, 0, 0, SRCCOPY);
        SelectObject(*hdcSprite, oldSprite);
        skip++;
      }
    }
    break;
  }
  case HAND_TAB:
  {
    *tabsize = player[0].hand.num_cards;
    for (int i = index_start; i < player[0].hand.num_cards && i < index_start + 7; i++) {
      // for now, this cycles through hBitmapCard which only has the super neigh card and the back design
      // in the future, hBitmapCard could be replaced/assimilated by an updated Unicorn or Deck structure featuring the file name and the loaded hBitmap
      oldSprite = SelectObject(*hdcSprite, hBitmapCard[i % 2]);
      BitBlt(*hdcMem, start.x + (distance * (i % 7)), start.y, bm.bmWidth, bm.bmHeight, *hdcSprite, 0, 0, SRCCOPY);
      SelectObject(*hdcSprite, oldSprite);
    }
    break;
  }
  case NURSERY_TAB:
  {
    *tabsize = NURSERY_SIZE - nursery_index;
    for (int i = nursery_index + index_start; i < NURSERY_SIZE && i < index_start + 7; i++) {
      // for now, this cycles through hBitmapCard which only has the super neigh card and the back design
      // in the future, hBitmapCard could be replaced/assimilated by an updated Unicorn or Deck structure featuring the file name and the loaded hBitmap
      oldSprite = SelectObject(*hdcSprite, hBitmapCard[i % 2]);
      BitBlt(*hdcMem, start.x + (distance * (i % 7)), start.y, bm.bmWidth, bm.bmHeight, *hdcSprite, 0, 0, SRCCOPY);
      SelectObject(*hdcSprite, oldSprite);
    }
    break;
  }
  case DECK_TAB:
  {
    // TODO: include a check for card effects that allow deck viewing, and then include a check for specific cards
    *tabsize = DECK_SIZE - deck_index;
    for (int i = deck_index + index_start; i < DECK_SIZE && i < index_start + 7; i++) {
      // for now, this cycles through hBitmapCard which only has the super neigh card and the back design
      // in the future, hBitmapCard could be replaced/assimilated by an updated Unicorn or Deck structure featuring the file name and the loaded hBitmap
      oldSprite = SelectObject(*hdcSprite, hBitmapCard[i % 2]);
      BitBlt(*hdcMem, start.x + (distance * (i % 7)), start.y, bm.bmWidth, bm.bmHeight, *hdcSprite, 0, 0, SRCCOPY);
      SelectObject(*hdcSprite, oldSprite);
    }
    break;
  }
  case DISCARD_TAB:
  {
    // TODO: (maybe) include a check for specific cards
    *tabsize = discard_index;
    for (int i = index_start; i < discard_index && i < index_start + 7; i++) {
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

  // show the page arrow icons if applicable
  if (*tabsize > pagenum * 7) {
    oldSprite = SelectObject(*hdcSprite, buttons[PAGE_RIGHT].bitmap);
    TransparentBlt(*hdcMem, buttons[PAGE_RIGHT].x, buttons[PAGE_RIGHT].y, buttons[PAGE_RIGHT].width, buttons[PAGE_RIGHT].height, *hdcSprite, 0, 0, buttons[PAGE_RIGHT].width, buttons[PAGE_RIGHT].height, RGB(0, 255, 0));
    SelectObject(*hdcSprite, oldSprite);
  }
  if (pagenum > 1) {
    oldSprite = SelectObject(*hdcSprite, buttons[PAGE_LEFT].bitmap);
    TransparentBlt(*hdcMem, buttons[PAGE_LEFT].x, buttons[PAGE_LEFT].y, buttons[PAGE_LEFT].width, buttons[PAGE_LEFT].height, *hdcSprite, 0, 0, buttons[PAGE_LEFT].width, buttons[PAGE_LEFT].height, RGB(0, 255, 0));
    SelectObject(*hdcSprite, oldSprite);
  }
}

struct ToolTip ReturnCardHoverTip(char *title, char *msg, int x, int y) {
  struct ToolTip tippy;

  int width = 250;
  int height = 150;

  strcpy_s(tippy.title, sizeof tippy.title, title);
  strcpy_s(tippy.msg, sizeof tippy.msg, msg);
  tippy.fonttitle = FANCYFONT;
  tippy.fonttxt   = OLDFONT;

  tippy.x = x + 45;
  tippy.y = y - height - 25;
  tippy.width   = width;
  tippy.height  = height;
  tippy.ishover = TRUE;

  return tippy;
}

struct ToolTip ReturnPlayerHoverTip(int pnum, int x, int y) {
  struct ToolTip tippy;

  int width = 200;
  int height = 60;

  strcpy_s(tippy.title, sizeof tippy.title, player[pnum].username);
  snprintf(tippy.msg, sizeof tippy.msg, "# of cards in hand: %d\n# of unicorns in stable: %d",
    player[pnum].hand.num_cards, player[pnum].stable.num_unicorns);
  tippy.fonttitle = BOLDFONT;
  tippy.fonttxt   = OLDFONT;

  tippy.x = x - width - 20;
  tippy.y = y - 10;
  tippy.width   = width;
  tippy.height  = height;
  tippy.ishover = TRUE;

  return tippy;
}

void CreateCustomToolTip(HDC *hdcMem, struct ToolTip hoverTip) {
  RECT rc;
  int padding = 8;

  // TODO: mess around with colors later to make it look prettier :3

  // draw the outer box
  HBRUSH brush = CreateSolidBrush(RGB(255, 0, 150));
  HBRUSH old_brush = (HBRUSH) SelectObject(*hdcMem, brush);

  // add some padding to the rectangle area instead of applying it to the text
  RoundRect(*hdcMem, hoverTip.x - padding, hoverTip.y - padding, hoverTip.x + hoverTip.width + (padding * 2), hoverTip.y + hoverTip.height + (padding * 2), 15, 15);

  SelectObject(*hdcMem, old_brush);
  DeleteObject(brush);

  // prepare the message text :)

  SetBkMode(*hdcMem, TRANSPARENT); // box surrounding text is transparent instead of white

  int title_offset = 0;
  // type out title if applicable
  if (hoverTip.title[0] != '\0') {
    SetRect(&rc, hoverTip.x, hoverTip.y, hoverTip.x + hoverTip.width, hoverTip.y + hoverTip.width);
    SetTextColor(*hdcMem, RGB(255, 255, 255));
    SelectObject(*hdcMem, fonts[hoverTip.fonttitle]);
    DrawText(*hdcMem, hoverTip.title, strlen(hoverTip.title), &rc, DT_LEFT | DT_WORDBREAK);

    // calculate the vertical space taken by the title
    SIZE size;
    GetTextExtentPoint(*hdcMem, hoverTip.title, strlen(hoverTip.title), &size);
    title_offset = size.cy + (padding / 2);
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
  int msg[8] = { 0 };
  int bordermsg[8] = { 0 };
  char errors[1024] = "";
  BOOL issuccess = TRUE;

  // TODO: make this a loop by using a c-string array for the image file names. this is too unsightly...

  hBitmapTitle[TITLEBLANK] = (HBITMAP)LoadImage(NULL, "Assets\\titleblank.bmp",
    IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
  if (hBitmapTitle[TITLEBLANK] == NULL) {
    issuccess = FALSE;
    strcat_s(errors, sizeof errors, "titleblank.bmp ");
  }

  hBitmapTitle[TITLEHOST] = hBitmapTitle[TITLEBLANK];
  hBitmapTitle[TITLEJOIN] = hBitmapTitle[TITLEBLANK];
  hBitmapTitle[TITLERULES] = hBitmapTitle[TITLEBLANK];

  hBitmapTitle[RULESONE] = (HBITMAP)LoadImage(NULL, "Assets\\rules1.bmp",
    IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
  if (hBitmapTitle[RULESONE] == NULL) {
    issuccess = FALSE;
    strcat_s(errors, sizeof errors, "rules1.bmp ");
  }

  hBitmapTitle[RULESTWO] = (HBITMAP)LoadImage(NULL, "Assets\\rules2.bmp",
    IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
  if (hBitmapTitle[RULESTWO] == NULL) {
    issuccess = FALSE;
    strcat_s(errors, sizeof errors, "rules2.bmp ");
  }

  hBitmapTitle[LOBBY] = (HBITMAP)LoadImage(NULL, "Assets\\lobby.bmp",
    IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
  if (hBitmapTitle[LOBBY] == NULL) {
    issuccess = FALSE;
    strcat_s(errors, sizeof errors, "lobby.bmp ");
  }

  hBitmapTitle[GAMESTART] = (HBITMAP)LoadImage(NULL, "Assets\\game.bmp",
    IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
  if (hBitmapTitle[GAMESTART] == NULL) {
    issuccess = FALSE;
    strcat_s(errors, sizeof errors, "game.bmp ");
  }
  hBitmapTitle[DEBUGMODE] = hBitmapTitle[GAMESTART];

  hBitmapBorder[0] = (HBITMAP)LoadImage(NULL, "Assets\\border1.bmp",
    IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
  if (hBitmapBorder[0] == NULL) {
    issuccess = FALSE;
    strcat_s(errors, sizeof errors, "border1.bmp ");
  }

  hBitmapBorder[1] = (HBITMAP)LoadImage(NULL, "Assets\\border2.bmp",
    IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
  if (hBitmapBorder[1] == NULL) {
    issuccess = FALSE;
    strcat_s(errors, sizeof errors, "border2.bmp ");
  }

  hBitmapBorder[2] = (HBITMAP)LoadImage(NULL, "Assets\\border3.bmp",
    IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
  if (hBitmapBorder[2] == NULL) {
    issuccess = FALSE;
    strcat_s(errors, sizeof errors, "border3.bmp ");
  }

  hBitmapBorder[3] = (HBITMAP)LoadImage(NULL, "Assets\\border4.bmp",
    IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
  if (hBitmapBorder[3] == NULL) {
    issuccess = FALSE;
    strcat_s(errors, sizeof errors, "border4.bmp ");
  }

  hBitmapBorder[4] = (HBITMAP)LoadImage(NULL, "Assets\\border5.bmp",
    IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
  if (hBitmapBorder[4] == NULL) {
    issuccess = FALSE;
    strcat_s(errors, sizeof errors, "border5.bmp ");
  }

  hBitmapBorder[5] = (HBITMAP)LoadImage(NULL, "Assets\\border6.bmp",
    IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
  if (hBitmapBorder[5] == NULL) {
    issuccess = FALSE;
    strcat_s(errors, sizeof errors, "border6.bmp ");
  }

  hBitmapBorder[6] = (HBITMAP)LoadImage(NULL, "Assets\\border7.bmp",
    IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
  if (hBitmapBorder[6] == NULL) {
    issuccess = FALSE;
    strcat_s(errors, sizeof errors, "border7.bmp ");
  }

  hBitmapBorder[7] = (HBITMAP)LoadImage(NULL, "Assets\\border8.bmp",
    IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
  if (hBitmapBorder[7] == NULL) {
    issuccess = FALSE;
    strcat_s(errors, sizeof errors, "border8.bmp ");
  }

  for (int i = 0; i < sizeof icons / sizeof icons[0]; i++) {
    icons[i].bitmap = (HBITMAP)LoadImage(NULL, icons[i].filename,
      IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    if (icons[i].bitmap == NULL) {
      issuccess = FALSE;
      strcat_s(errors, sizeof errors, icons[i].filename);
      strcat_s(errors, sizeof errors, " ");
    }
  }

  for (int i = 0; i < sizeof player_nums / sizeof player_nums[0]; i++) {
    player_nums[i].bitmap = (HBITMAP)LoadImage(NULL, player_nums[i].filename,
      IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    if (player_nums[i].bitmap == NULL) {
      issuccess = FALSE;
      strcat_s(errors, sizeof errors, player_nums[i].filename);
      strcat_s(errors, sizeof errors, " ");
    }
  }

  // stable count goes up to 7
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

  for (int i = 0; i < sizeof buttons / sizeof buttons[0]; i++) {
    buttons[i].bitmap = (HBITMAP)LoadImage(NULL, buttons[i].filename,
      IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    if (buttons[i].bitmap == NULL) {
      issuccess = FALSE;
      strcat_s(errors, sizeof errors, buttons[i].filename);
      strcat_s(errors, sizeof errors, " ");
    }
  }

  // same for card bitmaps being placed inside their own structures
  hBitmapCard[0] = (HBITMAP)LoadImage(NULL, "Assets\\back.bmp",
    IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
  if (hBitmapCard[0] == NULL) {
    issuccess = FALSE;
    strcat_s(errors, sizeof errors, "back.bmp ");
  }
  hBitmapCard[1] = (HBITMAP)LoadImage(NULL, "Assets\\default_superneigh.bmp",
    IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
  if (hBitmapCard[1] == NULL) {
    issuccess = FALSE;
    strcat_s(errors, sizeof errors, "default_superneigh.bmp ");
  }

  if (issuccess == 0) {
    char errormsg[1024] = "Failed to load image(s) ";
    strcat_s(errormsg, sizeof errormsg, errors);

    MessageBox(hWnd, errormsg, "Error", MB_OK);
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

  // // this should output 7 unicorns in the first page and 1 in the 2nd page
  // // WILL BUG OUT if i keep the stable_size at 25 instead of 100+, so use
  // // this test specifically for the deck later on
  // for (int i = 0; i < 80; i++) {
  //   player[0].stable.unicorns[i] = (i % 10) ? 128 : 30;
  //   if (checkClass(ANYUNICORN, deck[player[0].stable.unicorns[i]].class))
  //     player[0].stable.num_unicorns++;
  //   player[0].stable.size++;
  // }

  // player 1: host
  // this should output 7 unicorns in the first page and 1 in the 2nd page
  strcpy_s(player[0].username, sizeof player[0].username, "host");
  for (int i = 0; i < 15; i++) {
    player[0].stable.unicorns[i] = (i & 1) ? 128 : 30;
    if (checkClass(ANYUNICORN, deck[player[0].stable.unicorns[i]].class))
      player[0].stable.num_unicorns++;
    player[0].stable.size++;
  }
  for (int i = 0; i < 5; i++) {
    player[0].hand.cards[i] = 60;
    player[0].hand.num_cards++;
  }

  // player 2: noob
  strcpy_s(player[1].username, sizeof player[1].username, "nooblet");
  for (int i = 0; i < 5; i++) {
    player[1].stable.unicorns[i] = (i & 1) ? 50 : 40;
    if (checkClass(ANYUNICORN, deck[player[1].stable.unicorns[i]].class))
      player[1].stable.num_unicorns++;
    player[1].stable.size++;
  }
  for (int i = 0; i < 5; i++) {
    player[1].hand.cards[i] = 42 + i;
    player[1].hand.num_cards++;
  }

  current_players = 2;
}

void ResetDebugMode() {
  // reset player's cards/stable/etc. to 0;
  // TODO: this should be implemented as a hard reset when you join the game, but for now this will close off the init function
  for (int i = 0; i < MAX_PLAYERS; i++) {
    memset(player[i].stable.unicorns, 0, sizeof player[i].stable.unicorns[0]);
    memset(player[i].hand.cards, 0, sizeof player[i].hand.cards[0]);
    memset(player[i].username, 0, sizeof player[i].username[0]);
    player[i].stable.num_unicorns = 0;
    player[i].stable.size = 0;
    player[i].hand.num_cards = 0;
  }
  current_players = 1;
}


// ********************************************************************************
// ********************* general game logic helper functions **********************
// ********************************************************************************

// TODO: reorganize WM_PAINT or whichever future function since this is currently not in use
void GameLoop(HWND hwnd) {
  HDC hdc, hdcMem;
  BITMAP bitmap;
  HGDIOBJ oldBitmap;
  POINT pnt;
  MSG msg;
  RECT rec;

  hdc = GetDC(hwnd);
  hdcMem = CreateCompatibleDC(hdc);

  for (;;) {
    GetCursorPos(&pnt);
    ScreenToClient(hwnd, &pnt);

    switch (menustate) {
    case TITLEBLANK:
      if (pnt.x >= 528 && pnt.x <= 757) {
        if (pnt.y >= 385 && pnt.y < 472) {
          // Host Button
          oldBitmap = SelectObject(hdcMem, hBitmapTitle[TITLEHOST]);
          if (GetAsyncKeyState(VK_LBUTTON) < 0 && is_active && windowOpen[1] == FALSE) {
            // https://stackoverflow.com/questions/17202377/c-creating-a-window-in-a-new-thread
            // this won't work in a thread unless there's a message loop, but that would create bliting issues
            CreateHostWindow(hwnd);

            while (windowOpen[1]) {
              if (GetMessage(&msg, NULL, 0, 0)) {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
              }
            }
          }
        }
        else if (pnt.y >= 472 && pnt.y < 572) {
          // Join Button
          oldBitmap = SelectObject(hdcMem, hBitmapTitle[TITLEJOIN]);
          if (GetAsyncKeyState(VK_LBUTTON) < 0 && is_active && windowOpen[1] == FALSE) {
            // https://stackoverflow.com/questions/17202377/c-creating-a-window-in-a-new-thread
            // this won't work in a thread unless there's a message loop, but that would create bliting issues
            CreateJoinWindow(hwnd);

            while (windowOpen[1]) {
              if (GetMessage(&msg, NULL, 0, 0)) {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
              }
            }
          }
        }
        else if (pnt.y >= 572 && pnt.y <= 659) {
          // Rules Button
          oldBitmap = SelectObject(hdcMem, hBitmapTitle[TITLERULES]);
          // use Async instead of GetKeyState because this loop doesn't process messages ig
          // https://stackoverflow.com/questions/59923765/getkeystate-function-not-working-when-checking-if-left-mouse-button-is-clicked
          if (GetAsyncKeyState(VK_LBUTTON) < 0 && is_active) {
            oldBitmap = SelectObject(hdcMem, hBitmapTitle[RULESONE]);
            menustate = RULESONE;
          }
        }
        else {
          oldBitmap = SelectObject(hdcMem, hBitmapTitle[TITLEBLANK]);
        }
      }
      else {
        oldBitmap = SelectObject(hdcMem, hBitmapTitle[TITLEBLANK]);
      }
      break;
    case RULESONE:
      if (pnt.y >= 619 && pnt.y <= 650 && GetAsyncKeyState(VK_LBUTTON) < 0 && is_active) {
        if (pnt.x >= 975 && pnt.x <= 1074) {
          oldBitmap = SelectObject(hdcMem, hBitmapTitle[TITLEBLANK]);
          menustate = TITLEBLANK;
        }
        else if (pnt.x >= 1129 && pnt.x <= 1160) {
          oldBitmap = SelectObject(hdcMem, hBitmapTitle[RULESTWO]);
          menustate = RULESTWO;
        }
        else {
          oldBitmap = SelectObject(hdcMem, hBitmapTitle[RULESONE]);
        }
      }
      else {
        oldBitmap = SelectObject(hdcMem, hBitmapTitle[RULESONE]);
      }
      break;
    case RULESTWO:
      if (pnt.y >= 619 && pnt.y <= 650 && GetAsyncKeyState(VK_LBUTTON) < 0 && is_active) {
        if (pnt.x >= 975 && pnt.x <= 1074) {
          oldBitmap = SelectObject(hdcMem, hBitmapTitle[TITLEBLANK]);
          menustate = TITLEBLANK;
        }
        else if (pnt.x >= 1087 && pnt.x <= 1118) {
          oldBitmap = SelectObject(hdcMem, hBitmapTitle[RULESONE]);
          menustate = RULESONE;
        }
        else {
          oldBitmap = SelectObject(hdcMem, hBitmapTitle[RULESTWO]);
        }
      }
      else {
        oldBitmap = SelectObject(hdcMem, hBitmapTitle[RULESTWO]);
      }
      break;
    case LOBBY:
      //if (pnt.x >= 120 && pnt.x <= 309 && pnt.y >= 590 && pnt.y <= 639 && GetAsyncKeyState(VK_LBUTTON) < 0 && is_active) {
      //  // start game
      //}
      oldBitmap = SelectObject(hdcMem, hBitmapTitle[LOBBY]);
      RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE);
      break;
    default:
      oldBitmap = SelectObject(hdcMem, hBitmapTitle[TITLEBLANK]);
      break;
    }

    BitBlt(hdc, 0, 0, BWIDTH, BHEIGHT,
      hdcMem, 0, 0, SRCCOPY);
    SelectObject(hdcMem, oldBitmap);

    // this just flickers because of sleep(15) i think?
    // RECT rec = {5, 5, 50, 30};
    // DrawText(hdc, greeting, _tcslen(greeting), &rec, DT_LEFT);

    Sleep(15);
  }

  DeleteDC(hdcMem);
  ReleaseDC(hwnd, hdc);
}

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
// ************************* game state helper functions **************************
// ********************************************************************************

void NothingBurger() {
  return NULL;
}

// hacky fix for mapping the game state with the button manager array; a giant array may not be the answer if it requires stuff like this and NothingBurger...
int StateButtonMap(int state) {
  switch (state) {
  case TITLEBLANK:
    return titlebuttons;
  case TITLEHOST:
    return titlebuttons;
  case TITLEJOIN:
    return titlebuttons;
  case TITLERULES:
    return titlebuttons;
  case RULESONE:
    return rulebuttons;
  case RULESTWO:
    return rulebuttons;
  case LOBBY:
    return lobbybuttons;
  case DEBUGMODE:
    return debugbuttons;
  default:
    // ???
    return NULL;
  }
}

void SwitchState(int statenum) {
  menustate = statenum;
}

void HostGeneration(HWND hWnd) {
  MSG msg;

  menustate = TITLEBLANK;

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

  menustate = TITLEBLANK;

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

  menustate = TITLEBLANK;
}

void StartGame() {
  // user clicked the start button; only the host can properly start the game
  if (!isclient && current_players >= 2) {
    networktoggle ^= 4;
    // closesocket(udpfd); // ??? why did i close the socket here o.O ?? testing purposes?
  }

  // // TBC: don't do anything just yet
  // menustate = GAMESTART;
}

void ClickBabyUnicorn(POINT pnt) {
  int ret;

  if (isclient) {
    clientPnt = pnt;
    networktoggle ^= 2;
  }
  else {
    ret = SelectBabyUnicorn(0, pnt); // server is always player index 0
    if (ret) {
      networktoggle ^= 2;
    }
  }
}

volatile DWORD whatever;

void GUIlobby(POINT pnt) {
  int ret;
  DWORD waitResult;

  // TODO: (maybe) change the structure of RECTs [babies] into a general button structure tying into the whole GetAsyncKeyState if statement w/ the start/leave buttons
  // TODO: implement this with R-trees (specifically because the selectbabyunicorn function
  // has a lot of "buttons") so that it's more optimally chunked into bigger pieces
  if (GetAsyncKeyState(VK_LBUTTON) < 0 && is_active) {
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

    if (pnt.y >= 590 && pnt.y <= 639) {
      if (pnt.x >= 360 && pnt.x <= 549) {
        // user clicked the leave button
        networktoggle ^= 1;
        if (!isclient) {
          closesocket(udpfd);
        }
        return;
      }
      else if (pnt.x >= 120 && pnt.x <= 309) {
        // user clicked the start button; only the host can properly start the game
        if (!isclient && current_players >= 2) {
          networktoggle ^= 4;
          // closesocket(udpfd); // ??? why did i close the socket here o.O ?? testing purposes?
        }
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
}

// ********************************************************************************
// ****************************** callback functions ******************************
// ********************************************************************************


//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  HDC hdc;
  PAINTSTRUCT ps;
  HDC hdcMem;
  HGDIOBJ oldBitmap, oldSprite;
  POINT pnt;
  RECT rc;
  const int pntpi = 72; // points-per-inch
  int pxpi, points, pxheight;
  HBITMAP hBitmapBuffer;
  BOOL err;
  DWORD err2;

  HDC hdcSprite;
  HBITMAP hbmSprite;
  MSG msg;

  static int pagenum = 1;
  static int tabsize;
  static int tabnum;
  static BOOL istitleHover; // TODO: maybe combine istitleHover w/ hoverTip.ishover into a single hover boolean check; or combine istitleHover w/ the struct Button "hornbutton"
  static struct ToolTip hoverTip;

  switch (message)
  {
  case WM_CREATE:
    LoadImages(hWnd);
    InitFonts(hWnd);
    InitButtonManager(hWnd);
    windowOpen[0] = TRUE;
    webhwnd = hWnd; // for server and client to access in the wsapoll lobby loop

    //gamethread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)GameLoop, hWnd, 0, &tId);
    break;
  case WM_ACTIVATE:
    // toggles activate variable so that clicks aren't triggered when window is in background
    if (wParam == WA_INACTIVE) {
      is_active = 0;
    }
    else {
      is_active = 1;
    }
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

    if (menustate == DEBUGMODE) {
      hoverTip.ishover = 0;
      if (pnt.x >= 87 && pnt.x <= 237 && pnt.y >= 507 && pnt.y <= 707) {
        if (tabnum == UNICORN_TAB)
          hoverTip = ReturnCardHoverTip(deck[player[0].stable.unicorns[0]].name, deck[player[0].stable.unicorns[0]].description, 87, 507);
        if (tabnum == HAND_TAB)
          hoverTip = ReturnCardHoverTip(deck[player[0].hand.cards[0]].name, deck[player[0].hand.cards[0]].description, 87, 507);
      }

      for (int i = 0; i < current_players; i++) {
        if (pnt.x >= player_nums[i].x && pnt.x <= player_nums[i].x + player_nums[i].width &&
          pnt.y >= player_nums[i].y && pnt.y <= player_nums[i].y + player_nums[i].width) {
          hoverTip = ReturnPlayerHoverTip(i, player_nums[i].x, player_nums[i].y);
        }
      }
      break;
    }

    int state = StateButtonMap(menustate);
    istitleHover = FALSE;

    // TODO: potentially just skip the button manager stuff and create a separate one specifically for hovered items
    // this will avoid having to create a dummy function that does nothing
    for (int i = 0; i < numsubsets - 1; i++) {
      if (pnt.x >= buttonManager2D[state][i].x && pnt.x <= buttonManager2D[state][i].x + buttonManager2D[state][i].width &&
        pnt.y >= buttonManager2D[state][i].y && pnt.y <= buttonManager2D[state][i].y + buttonManager2D[state][i].height) {
        // hover action
        buttonManager2D[state][i].onHover(&buttonManager2D[state][i], &istitleHover);
      }
    }

    break;
  }
  case WM_LBUTTONDOWN:
  {
    // for clicking purposes
    GetCursorPos(&pnt);
    ScreenToClient(hWnd, &pnt);

    int state = StateButtonMap(menustate);

    if (menustate == LOBBY) {
      GUIlobby(pnt);
      break;
    }

    for (int i = 0; i < numsubsets - 1; i++) {
      if (pnt.x >= buttonManager2D[state][i].x && pnt.x <= buttonManager2D[state][i].x + buttonManager2D[state][i].width &&
        pnt.y >= buttonManager2D[state][i].y && pnt.y <= buttonManager2D[state][i].y + buttonManager2D[state][i].height) {
        // left click action
        if (is_active && windowOpen[1] == FALSE) {
          buttonManager2D[state][i].onClick(buttonManager2D[state][i].source);
        }
      }
    }

    // TOOD: write a proper ifdef
    if (menustate == TITLEBLANK) {
      if (pnt.x >= 0 && pnt.x <= 50 && pnt.y >= 0 && pnt.y <= 50 && is_active) {
        menustate = DEBUGMODE;
        tabnum = UNICORN_TAB;
        InitDebugMode(); // for card testing purposes
      }
    }
    else if (menustate == DEBUGMODE) {
      if (pnt.x >= 0 && pnt.x <= 50 && pnt.y >= 0 && pnt.y <= 50 && is_active) {
        menustate = TITLEBLANK;
        ResetDebugMode(); // for card testing purposes
        tabnum = UNICORN_TAB;
        pagenum = 1;
      }
      else if (GetAsyncKeyState(VK_LBUTTON) < 0 && is_active && pnt.y >= 463 && pnt.y <= 491) {
        if (pnt.x <= 232) {
          tabnum = UNICORN_TAB;
          pagenum = 1;
        }
        else if (pnt.x <= 505) {
          tabnum = UPGRADE_TAB;
          pagenum = 1;
        }
        else if (pnt.x <= 777) {
          tabnum = HAND_TAB;
          pagenum = 1;
        }
      }
      else if (pnt.x >= buttons[PAGE_RIGHT].x && pnt.x <= buttons[PAGE_RIGHT].x + buttons[PAGE_RIGHT].width &&
        pnt.y >= buttons[PAGE_RIGHT].y && pnt.y <= buttons[PAGE_RIGHT].y + buttons[PAGE_RIGHT].height &&
        GetAsyncKeyState(VK_LBUTTON) < 0 && is_active) {
        if (tabsize > pagenum * 7) {
          pagenum++;
        }
      }
      else if (pnt.x >= buttons[PAGE_LEFT].x && pnt.x <= buttons[PAGE_LEFT].x + buttons[PAGE_LEFT].width &&
        pnt.y >= buttons[PAGE_LEFT].y && pnt.y <= buttons[PAGE_LEFT].y + buttons[PAGE_LEFT].height &&
        GetAsyncKeyState(VK_LBUTTON) < 0 && is_active) {
        if (pagenum > 1) {
          pagenum--;
        }
      }

    }

    break;
  }
  case WM_PAINT:
  {
    // TODO: use UpdateLayeredWindow() instead of WM_PAINT????
    // https://stackoverflow.com/questions/67689087/correct-way-to-handle-and-redraw-a-layered-window-with-a-bitmap

    // validate that HWND
    hdc = BeginPaint(hWnd, &ps);

    // create memory DC and memory bitmap where we shall do our drawing
    hdcMem = CreateCompatibleDC(hdc);
    // create a deep copy so that the original image doesn't get updated when bitblt'ing the extra info to the memory bitmap
    hBitmapBuffer = (HBITMAP)CopyImage(hBitmapTitle[menustate], IMAGE_BITMAP, BWIDTH, BHEIGHT, LR_COPYRETURNORG);
    oldBitmap = SelectObject(hdcMem, hBitmapBuffer);

    if (menustate == LOBBY) {

      SetBkMode(hdcMem, TRANSPARENT); // box surrounding text is transparent instead of white

      // type out lobby code
      SetRect(&rc, 78, 180, 1202, 225);
      SetTextColor(hdcMem, RGB(255, 255, 255));
      SelectObject(hdcMem, fonts[CHONKYFONT]);
      DrawText(hdcMem, hexcode, strlen(hexcode), &rc, DT_CENTER);

      // type out party members
      SetRect(&rc, 115, 260, 800, 625);
      SelectObject(hdcMem, fonts[OLDFONT]);
      DrawText(hdcMem, partymems, strlen(partymems), &rc, DT_LEFT);

      // draw border for chosen baby unicorns (ughhhh)
      // TODO: MAX_PLAYERS is an inefficient hacky fix; should be current_players (see: server.c)
      hdcSprite = CreateCompatibleDC(hdc);
      for (int i = 0; i < MAX_PLAYERS; i++) {
        if (pselect[i].left != 0) {
          // // for whatever reason, the original hdcMem is incompatible and will draw the whole
          // // border (non-alpha included) at the top-left corner
          // SelectObject(hdcMem, hBitmapBorder[i]);
          // TransparentBlt(hdcMem, pselect[i].left, pselect[i].top, BORDERWIDTH, BORDERWIDTH, hBitmapBorder[i], 0, 0, BORDERWIDTH, BORDERWIDTH, RGB(0, 255, 0));

          oldSprite = SelectObject(hdcSprite, hBitmapBorder[i]);
          TransparentBlt(hdcMem, pselect[i].left, pselect[i].top, BORDERWIDTH, BORDERWIDTH, hdcSprite, 0, 0, BORDERWIDTH, BORDERWIDTH, RGB(0, 255, 0));
          SelectObject(hdcSprite, oldSprite);
        }
      }
      DeleteDC(hdcSprite);
    }
    else if (menustate == DEBUGMODE) {
      // draw player Narwhal!!
      hdcSprite = CreateCompatibleDC(hdc);
      // TODO: this one actually has to be based off of current players, so it is capped at 1 for now
      for (int i = 0; i < current_players; i++) {
        oldSprite = SelectObject(hdcSprite, icons[BABYNARWHAL - i].bitmap);
        TransparentBlt(hdcMem, player_nums[i].x, player_nums[i].y, player_nums[i].width, player_nums[i].height, hdcSprite, 0, 0, player_nums[i].width, player_nums[i].height, RGB(0, 255, 0));
        SelectObject(hdcSprite, oldSprite);

        // player number
        oldSprite = SelectObject(hdcSprite, player_nums[i].bitmap);
        TransparentBlt(hdcMem, player_nums[i].x, player_nums[i].y, player_nums[i].width, player_nums[i].height, hdcSprite, 0, 0, player_nums[i].width, player_nums[i].height, RGB(0, 255, 0));
        SelectObject(hdcSprite, oldSprite);

        // number of unicorns in stable
        if (player[i].stable.num_unicorns <= 7) {
          oldSprite = SelectObject(hdcSprite, stable_nums[player[i].stable.num_unicorns].bitmap);
        }
        else {
          oldSprite = SelectObject(hdcSprite, stable_nums[7].bitmap);
        }
        TransparentBlt(hdcMem, player_nums[i].x, player_nums[i].y, player_nums[i].width, player_nums[i].height, hdcSprite, 0, 0, player_nums[i].width, player_nums[i].height, RGB(0, 255, 0));
        SelectObject(hdcSprite, oldSprite);
      }

      DisplayCardWindow(&hdcMem, &hdcSprite, pagenum, &tabsize, tabnum);
      DeleteDC(hdcSprite);

      if (hoverTip.ishover) {
        CreateCustomToolTip(&hdcMem, hoverTip);
      }
    }
    else if (menustate == TITLEBLANK || menustate == TITLEHOST || menustate == TITLEJOIN || menustate == TITLERULES) {
      hdcSprite = CreateCompatibleDC(hdc);

      if (istitleHover) {
        oldSprite = SelectObject(hdcSprite, hornbutton.bitmap);
        TransparentBlt(hdcMem, hornbutton.x, hornbutton.y, hornbutton.width, hornbutton.height, hdcSprite, 0, 0, hornbutton.width, hornbutton.height, RGB(0, 255, 0));
        SelectObject(hdcSprite, oldSprite);
      }
      DeleteDC(hdcSprite);
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
    for (int i = 0; i < sizeof hBitmapTitle / sizeof hBitmapTitle[0]; i++) {
      DeleteObject(hBitmapTitle[i]);
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
    for (int i = 0; i < sizeof buttons / sizeof buttons[0]; i++) {
      DeleteObject(buttons[i].bitmap);
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

  return DefWindowProc(hWnd, message, wParam, lParam);
}

LRESULT CALLBACK WndProcHost(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
  // char tmpuser[NAME_SIZE * 2] = { 0 };
  char portstr[NAME_SIZE] = { 0 };
  int count = 0, portno;
  PAINTSTRUCT ps;
  HDC hdc;
  RECT rc;

  switch (message)
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

  return DefWindowProc(hWnd, message, wParam, lParam);
}

LRESULT CALLBACK WndProcJoin(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
  char portstr[NAME_SIZE] = { 0 }, codestr[9] = { 0 };
  int count = 0, portno;
  PAINTSTRUCT ps;
  HDC hdc;
  RECT rc;

  switch (message)
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
      GetWindowText(textNameHwnd, player[0].username, NAME_SIZE);
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
        // menustate = LOBBY;
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

  return DefWindowProc(hWnd, message, wParam, lParam);
}
