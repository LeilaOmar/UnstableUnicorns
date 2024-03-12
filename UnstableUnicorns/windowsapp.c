#include "windowsapp.h"
#include "server.h"
#include "client.h"
#include "gamemechanics.h"
#include "gamephase.h"

#pragma comment(lib,"msimg32.lib") // Transparent blt
#pragma comment(lib,"Winmm.lib") // play sound

// ********************************************************************************
// ***************************** Forward Declarations *****************************
// ********************************************************************************

// callback functions

static LRESULT CALLBACK WndProcHost(HWND, UINT, WPARAM, LPARAM);
static LRESULT CALLBACK WndProcJoin(HWND, UINT, WPARAM, LPARAM);

// game state WM functions

static void HoverTitle(POINT);
static void HoverDebug(POINT);

static void ClickTitle(POINT);
static void ClickRules(POINT);
static void ClickLobby(POINT);
static void ClickDebug(POINT);
static void ClickGame(POINT);

static void PaintTitle(HDC*);
static void PaintLobby(HDC*);
static void PaintDebug(HDC*);

// button function pointers (OnClick)

static void SwitchState(int);
static void HostGeneration(HWND);
static void JoinGeneration(HWND);
static void StartGame(void);
static void LeaveLobby(void);
static void ViewDeck(void);
static void SwitchTab(int);
static void TurnPage(int);

// ********************************************************************************
// ************************** Super Global Variables TM ***************************
// ********************************************************************************

HINSTANCE g_hInstance;
enum GameState menuState = 0;

char ip[16]; //!< ipv4 string
char hexcode[43]; //!< hexcode hash of ip
char partyMems[PARTYSTRSIZE] = { 0 }; //!< full char* list of party members

RECT pselect[MAX_PLAYERS]; //!< the coordinate position of the player's chosen Baby Unicorn
BOOL babyToggle[NURSERY_SIZE] = { FALSE }; //!< toggle to see whether or not a baby unicorn is taken during lobby selection; 0 = free, 1 = taken
int babyMap[MAX_PLAYERS] = { 0 }; //!< maps babyToggle to the player indices

// switch to activate certain networking features in the network thread to avoid improperly closing a socket
// TODO: combine w/ network events from all of the console stuff
unsigned char networkToggle = 0;

// ********************************************************************************
// ************************ Private/Static Global Variables ***********************
// ********************************************************************************

static enum Fonts { OLDFONT, CHONKYFONT, BOLDFONT, ITALICFONT, FANCYFONT, FANCYITALICS, NUMCUSTOMFONTS };

static HWND networkThread, textNameHwnd, portHwnd, codeHwnd;
static DWORD tIdweb;
static HBITMAP hBitmapBG[NUMSTATES], hBitmapBorder[MAX_PLAYERS], hBitmapCard[85], hBitmapTab[3];
static BOOL isChildOpen = FALSE; //!< TRUE if the Host or Join dialogue box is open, FALSE if inactive
static BOOL isChildInit[2] = { FALSE }; //!< Boolean check for initializing the child windows (RegisterClass is only supposed to run once); [0] is host/server, [1] is client
static HFONT fonts[NUMCUSTOMFONTS] = { NULL };

static enum Tab tabnum;    //!< The tab number representation of the window to display on the bottom of the in-game screen
static int tabsize;        //!< Number of total cards within the current tab view array (e.g. if you're looking at the deck, then tabsize could be anywhere from 1 to 116)
static int pagenum = 1;    //!< The page number within the current tab view array
static int pnumindex = 0;  //!< Used to switch between player indices to show their respective stables inside the DisplayCardWindow function

static int currPnum = 0;
static int clientPnum = 0;

static struct ToolTip hoverTip; //!< A color coded tooltip that displays the card info when hovering over cards
static const int STABLE_PADDING = 15; //!< arbitrary number of pixels to pad between the displayed list of cards in their respective stables/hands/decks

// These are in the same order as UnicornDatatypes, which are used to represent the card classes
static const COLORREF CARD_COLORS[] = {
  RGB(185, 109, 171),   //!< baby unicorn color     - purple
  RGB(107, 116, 181),   //!< basic unicorn color    - dark periwinkle (blue)
  RGB( 32, 189, 242),   //!< magical unicorn color  - aqua blue
  RGB(242, 128,  38),   //!< upgrade color          - orange
  RGB(255, 180,  23),   //!< downgrade color        - gold
  RGB(134, 200,  80),   //!< magic color            - lime green
  RGB(244,  29,  37),   //!< instant color          - red
};

// TODO: (see LoadCards) this is an example of what such a script for generating card->bitmap indices could look like
static const unsigned int CARD_MAP[] = {
   0,   // 1: Baby Unicorn (Red) [ID: 0]
   1,   // 2: Baby Unicorn (Pink) [ID: 1]
   2,   // 3: Baby Unicorn (Orange) [ID: 2]
   3,   // 4: Baby Unicorn (Yellow) [ID: 3]
   4,   // 5: Baby Unicorn (Green) [ID: 4]
   5,   // 6: Baby Unicorn (Blue) [ID: 5]
   6,   // 7: Baby Unicorn (Purple) [ID: 6]
   7,   // 8: Baby Unicorn (Black) [ID: 7]
   8,   // 9: Baby Unicorn (White) [ID: 8]
   9,   // 10: Baby Unicorn (Brown) [ID: 9]
  10,   // 11: Baby Unicorn (Rainbow) [ID: 10]
  11,   // 12: Baby Unicorn (Death) [ID: 11]
  12,   // 13: Baby Narwhal [ID: 12]
  13,   // 1: Basic Unicorn (Red) [ID: 13]
  13,   // 2: Basic Unicorn (Red) [ID: 14]
  13,   // 3: Basic Unicorn (Red) [ID: 15]
  14,   // 4: Basic Unicorn (Orange) [ID: 16]
  14,   // 5: Basic Unicorn (Orange) [ID: 17]
  14,   // 6: Basic Unicorn (Orange) [ID: 18]
  15,   // 7: Basic Unicorn (Yellow) [ID: 19]
  15,   // 8: Basic Unicorn (Yellow) [ID: 20]
  15,   // 9: Basic Unicorn (Yellow) [ID: 21]
  16,   // 10: Basic Unicorn (Green) [ID: 22]
  16,   // 11: Basic Unicorn (Green) [ID: 23]
  16,   // 12: Basic Unicorn (Green) [ID: 24]
  17,   // 13: Basic Unicorn (Blue) [ID: 25]
  17,   // 14: Basic Unicorn (Blue) [ID: 26]
  17,   // 15: Basic Unicorn (Blue) [ID: 27]
  18,   // 16: Basic Unicorn (Indigo) [ID: 28]
  18,   // 17: Basic Unicorn (Indigo) [ID: 29]
  18,   // 18: Basic Unicorn (Indigo) [ID: 30]
  19,   // 19: Basic Unicorn (Purple) [ID: 31]
  19,   // 20: Basic Unicorn (Purple) [ID: 32]
  19,   // 21: Basic Unicorn (Purple) [ID: 33]
  20,   // 22: Narwhal [ID: 34]
  20,   // 23: Narwhal [ID: 35]
  20,   // 24: Narwhal [ID: 36]
  21,   // 25: Rhinocorn [ID: 37]
  22,   // 26: Extremely Fertile Unicorn [ID: 38]
  23,   // 27: Magical Kittencorn [ID: 39]
  24,   // 28: Stabby the Unicorn [ID: 40]
  25,   // 29: Puppicorn [ID: 41]
  26,   // 30: Rainbow Unicorn [ID: 42]
  27,   // 31: Zombie Unicorn [ID: 43]
  28,   // 32: Extremely Destructive Unicorn [ID: 44]
  29,   // 33: Chainsaw Unicorn [ID: 45]
  30,   // 34: Llamacorn [ID: 46]
  31,   // 35: Americorn [ID: 47]
  32,   // 36: Ginormous Unicorn [ID: 48]
  33,   // 37: Seductive Unicorn [ID: 49]
  34,   // 38: Angel Unicorn [ID: 50]
  35,   // 39: Queen Bee Unicorn [ID: 51]
  36,   // 40: Greedy Flying Unicorn [ID: 52]
  37,   // 41: Annoying Flying Unicorn [ID: 53]
  38,   // 42: Magical Flying Unicorn [ID: 54]
  39,   // 43: Swift Flying Unicorn [ID: 55]
  40,   // 44: Majestic Flying Unicorn [ID: 56]
  41,   // 45: Unicorn Phoenix [ID: 57]
  42,   // 46: Unicorn on the Cob [ID: 58]
  43,   // 47: Black Knight Unicorn [ID: 59]
  44,   // 48: Shark With a Horn [ID: 60]
  45,   // 49: Shabby the Narwhal [ID: 61]
  46,   // 50: Narwhal Torpedo [ID: 62]
  47,   // 51: Alluring Narwhal [ID: 63]
  48,   // 52: Mermaid Unicorn [ID: 64]
  49,   // 53: Classy Narwhal [ID: 65]
  50,   // 54: The Great Narwhal [ID: 66]
  51,   // 55: Unicorn Poison [ID: 67]
  51,   // 56: Unicorn Poison [ID: 68]
  51,   // 57: Unicorn Poison [ID: 69]
  52,   // 58: Back Kick [ID: 70]
  52,   // 59: Back Kick [ID: 71]
  52,   // 60: Back Kick [ID: 72]
  53,   // 61: Change of Luck [ID: 73]
  53,   // 62: Change of Luck [ID: 74]
  54,   // 63: Glitter Tornado [ID: 75]
  54,   // 64: Glitter Tornado [ID: 76]
  55,   // 65: Unicorn Swap [ID: 77]
  55,   // 66: Unicorn Swap [ID: 78]
  56,   // 67: Re-Target [ID: 79]
  56,   // 68: Re-Target [ID: 80]
  57,   // 69: Unfair Bargain [ID: 81]
  57,   // 70: Unfair Bargain [ID: 82]
  58,   // 71: Two-For-One [ID: 83]
  58,   // 72: Two-For-One [ID: 84]
  59,   // 73: Unicorn Shrinkray [ID: 85]
  60,   // 74: Targeted Destruction [ID: 86]
  61,   // 75: Mystical Vortex [ID: 87]
  62,   // 76: Good Deal [ID: 88]
  63,   // 77: Shake Up [ID: 89]
  64,   // 78: Blatant Thievery [ID: 90]
  65,   // 79: Reset Button [ID: 91]
  66,   // 80: Rainbow Mane [ID: 92]
  66,   // 81: Rainbow Mane [ID: 93]
  66,   // 82: Rainbow Mane [ID: 94]
  67,   // 83: Extra Tail [ID: 95]
  67,   // 84: Extra Tail [ID: 96]
  67,   // 85: Extra Tail [ID: 97]
  68,   // 86: Glitter Bomb [ID: 98]
  68,   // 87: Glitter Bomb [ID: 99]
  69,   // 88: Yay [ID: 100]
  69,   // 89: Yay [ID: 101]
  70,   // 90: Unicorn Lasso [ID: 102]
  71,   // 91: Rainbow Aura [ID: 103]
  72,   // 92: Double Dutch [ID: 104]
  73,   // 93: Summoning Ritual [ID: 105]
  74,   // 94: Barbed Wire [ID: 106]
  75,   // 95: Pandamonium [ID: 107]
  76,   // 96: Sadistic Ritual [ID: 108]
  77,   // 97: Slowdown [ID: 109]
  78,   // 98: Nanny Cam [ID: 110]
  79,   // 99: Broken Stable [ID: 111]
  80,   // 100: Blinding Light [ID: 112]
  81,   // 101: Tiny Stable [ID: 113]
  82,   // 102: Neigh [ID: 114]
  82,   // 103: Neigh [ID: 115]
  82,   // 104: Neigh [ID: 116]
  82,   // 105: Neigh [ID: 117]
  82,   // 106: Neigh [ID: 118]
  82,   // 107: Neigh [ID: 119]
  82,   // 108: Neigh [ID: 120]
  82,   // 109: Neigh [ID: 121]
  82,   // 110: Neigh [ID: 122]
  82,   // 111: Neigh [ID: 123]
  82,   // 112: Neigh [ID: 124]
  82,   // 113: Neigh [ID: 125]
  82,   // 114: Neigh [ID: 126]
  82,   // 115: Neigh [ID: 127]
  83,   // 116: Super Neigh [ID: 128]
  84
};
static HBITMAP hBitmapBack;

// ********************************************************************************
// ****************************** Button Management *******************************
// ********************************************************************************

// for the player number border surrounding the baby unicorn selection in the lobby
#define BORDERWIDTH 97

static struct Button hornButton;
static struct Button titleButtons[3];
static struct Button ruleButtons[3];
static struct Button lobbyButtons[2];
static struct Button debugButtons[8];
static struct Button cardSlots[7];

// TODO: potentially rename these to icon[1-13]?
static struct Button icons[] = {
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
static struct Button playerNums[] = {
  { 1064, 28,  95, 81, "Assets\\player1.bmp", NULL, NULL, NULL },
  { 1175, 28,  95, 81, "Assets\\player2.bmp", NULL, NULL, NULL },
  { 1064, 118, 95, 81, "Assets\\player3.bmp", NULL, NULL, NULL },
  { 1175, 118, 95, 81, "Assets\\player4.bmp", NULL, NULL, NULL },
  { 1064, 208, 95, 81, "Assets\\player5.bmp", NULL, NULL, NULL },
  { 1175, 208, 95, 81, "Assets\\player6.bmp", NULL, NULL, NULL },
  { 1064, 298, 95, 81, "Assets\\player7.bmp", NULL, NULL, NULL },
  { 1175, 298, 95, 81, "Assets\\player8.bmp", NULL, NULL, NULL }
};
static struct Button stableNums[8];

static struct Offset {
  int width;
  int height;
  int offsetX;
  int offsetY;
};

// static const struct Offset clientOverlayIcon = { 96, 36, -12, 57 };
static const struct Offset clientOverlayIcon = { 99, 51, -6, 40 };
static HBITMAP hBitmapClient; //!< the cloud ornament surrounding the client's player icon
static HBITMAP hBitmapCurrPlayer[8]; //!< the red highlight on top of the current player num icon (TODO: create a font and just type the numbers w/ a border and shadow...)

// Baby Unicorn point coordinates in the lobby selection
// ordered: left, top, right, bottom
static const RECT BABY_COORDS[] = {
  {  659,  243,  748,  332 },   //!< BABYRED
  {  779,  243,  868,  332 },   //!< BABYPINK
  {  899,  243,  988,  332 },   //!< BABYORANGE
  { 1019,  243, 1108,  332 },   //!< BABYYELLOW

  {  599,  358,  688,  447 },   //!< BABYGREEN
  {  719,  358,  808,  447 },   //!< BABYBLUE
  {  839,  358,  928,  447 },   //!< BABYPURPLE
  {  959,  358, 1048,  447 },   //!< BABYBLACK
  { 1079,  358, 1168,  447 },   //!< BABYWHITE

  {  659,  473,  748,  562 },   //!< BABYBROWN
  {  779,  473,  868,  562 },   //!< BABYRAINBOW
  {  899,  473,  988,  562 },   //!< BABYDEATH
  { 1019,  473, 1108,  562 },   //!< BABYNARWHAL
};

/**
 * @brief Calculates the position for the horn select bitmap that centers around the hovered Title Screen button
 */
static void HornPosition(struct Button b) {
  int xdiff = -93;
  int ydiff = 17;

  hornButton.x = b.x + xdiff;
  hornButton.y = b.y + ydiff;
  hornButton.source = TRUE;
}

static void InitTitleButtons(struct Button *b, HWND hWnd, int size) {
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
  b[0].OnClick = HostGeneration;
  b[1].OnClick = JoinGeneration;
  b[2].OnClick = SwitchState;

  // TODO: incorporate error msg in case of failure
  // horn select
  hornButton.width = 416;
  hornButton.height = 40;
  hornButton.bitmap = (HBITMAP)LoadImage(NULL, "Assets\\horn_select.bmp",
    IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
}

static void InitRuleButtons(struct Button *b, int size) {
  for (int i = 0; i < size; i++) {
    b[i].y = 619;
    b[i].width = 32;
    b[i].height = 32;
    b[i].OnClick = SwitchState;
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

static void InitLobbyButtons(struct Button *b) {
  // start game
  b[0].x = 120;
  b[0].y = 590;
  b[0].width = 190;
  b[0].height = 50;
  b[0].source = GAMESTART;
  b[0].OnClick = StartGame;

  // leave lobby
  b[1].x = 360;
  b[1].y = 590;
  b[1].width = 190;
  b[1].height = 50;
  b[0].source = TITLEBLANK;
  b[1].OnClick = LeaveLobby;
}

static void InitCardWindowButtons(struct Button *b) {
  // unicorn stable tab
  b[UNICORN_TAB].x = 0;
  b[UNICORN_TAB].y = 463;
  b[UNICORN_TAB].width = 233;
  b[UNICORN_TAB].height = 29;
  b[UNICORN_TAB].source = UNICORN_TAB;
  b[UNICORN_TAB].OnClick = SwitchTab;

  // upgrade/downgrade tab
  b[UPGRADE_TAB].x = 233;
  b[UPGRADE_TAB].y = 463;
  b[UPGRADE_TAB].width = 273;
  b[UPGRADE_TAB].height = 29;
  b[UPGRADE_TAB].source = UPGRADE_TAB;
  b[UPGRADE_TAB].OnClick = SwitchTab;

  // hand tab
  b[HAND_TAB].x = 506;
  b[HAND_TAB].y = 463;
  b[HAND_TAB].width = 272;
  b[HAND_TAB].height = 29;
  b[HAND_TAB].source = HAND_TAB;
  b[HAND_TAB].OnClick = SwitchTab;

  // TODO: incorporate error msg in case of failure
  // page left
  b[PAGE_LEFT].x = 3;
  b[PAGE_LEFT].y = 636;
  b[PAGE_LEFT].width = 80;
  b[PAGE_LEFT].height = 59;
  b[PAGE_LEFT].source = PAGE_LEFT;
  b[PAGE_LEFT].OnClick = TurnPage;
  strcpy_s(b[PAGE_LEFT].filename, sizeof b[PAGE_LEFT].filename, "Assets\\pageleft.bmp");
  b[PAGE_LEFT].bitmap = (HBITMAP)LoadImage(NULL, b[PAGE_LEFT].filename,
    IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

  // page right
  b[PAGE_RIGHT].x = 1196;
  b[PAGE_RIGHT].y = 636;
  b[PAGE_RIGHT].width = 80;
  b[PAGE_RIGHT].height = 59;
  b[PAGE_RIGHT].source = PAGE_RIGHT;
  b[PAGE_RIGHT].OnClick = TurnPage;
  strcpy_s(b[PAGE_RIGHT].filename, sizeof b[PAGE_RIGHT].filename, "Assets\\pageright.bmp");
  b[PAGE_RIGHT].bitmap = (HBITMAP)LoadImage(NULL, b[PAGE_RIGHT].filename,
    IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
}

static void InitCardButtons(struct Button *b, int size) {
  // start is the starting point for the display
  POINT start = { 87, 507 };
  BITMAP bm;
  GetObject(hBitmapBack, (int)sizeof bm, &bm); // fetches width/height; all cards have the same w/h

  int distance = STABLE_PADDING + bm.bmWidth;

  for (int i = 0; i < size; i++) {
    b[i].x = start.x + (distance * i);
    b[i].y = start.y;
    b[i].width = bm.bmWidth;
    b[i].height = bm.bmHeight;
    b[i].filename[0] = '\0';
    b[i].bitmap = NULL;
    b[i].source = NULL;
    b[i].OnClick = NULL;
  }
}

static void InitDeckButtons(struct Button *b) {
  // TODO: the OnClick function will likely be multi-purpose, so this will need to be rewritten

  // nursery pile
  b[NURSERY_TAB].x = 346;
  b[NURSERY_TAB].y = 132;
  b[NURSERY_TAB].width = 145;
  b[NURSERY_TAB].height = 200;
  b[NURSERY_TAB].source = NURSERY_TAB;
  b[NURSERY_TAB].OnClick = SwitchTab;

  // deck pile
  b[DECK_TAB].x = 521;
  b[DECK_TAB].y = 132;
  b[DECK_TAB].width = 145;
  b[DECK_TAB].height = 200;
  b[DECK_TAB].source = DECK_TAB;
  b[DECK_TAB].OnClick = ViewDeck;

  // discard pile
  b[DISCARD_TAB].x = 696;
  b[DISCARD_TAB].y = 132;
  b[DISCARD_TAB].width = 145;
  b[DISCARD_TAB].height = 200;
  b[DISCARD_TAB].source = DISCARD_TAB;
  b[DISCARD_TAB].OnClick = SwitchTab;
}

static void InitButtonManager(HWND hWnd) {
  InitTitleButtons(titleButtons, hWnd, sizeof titleButtons / sizeof titleButtons[0]);
  InitRuleButtons(ruleButtons, sizeof ruleButtons / sizeof ruleButtons[0]);
  InitLobbyButtons(lobbyButtons);
  InitCardWindowButtons(debugButtons);
  InitCardButtons(cardSlots, sizeof cardSlots / sizeof cardSlots[0]);
  InitDeckButtons(debugButtons);
}

// ********************************************************************************
// ************************ Window and General UI Creation ************************
// ********************************************************************************

enum { ID_BUTTON, ID_TEXT };

/**
 * @brief Creates a child window to set up a game as the host
 */
static void CreateHostWindow(HWND hwnd) {
  static WNDCLASSEX wcexHost;

  if (!isChildInit[0]) {
    ZeroMemory(&wcexHost, sizeof(WNDCLASSEX)); // TODO: figure out what this does
    wcexHost.cbSize = sizeof(WNDCLASSEX);
    wcexHost.cbClsExtra = 0;
    wcexHost.cbWndExtra = 0;
    wcexHost.style = CS_HREDRAW | CS_VREDRAW;
    wcexHost.hInstance = g_hInstance;
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
        MB_ICONERROR | MB_OK);
      return;
    }
    else {
      isChildInit[0] = TRUE;
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
    g_hInstance,
    NULL
  );

  if (!hWndHost)
  {
    MessageBox(NULL,
      _T("Host window creation failed!"),
      _T("Windows Desktop Guided Tour"),
      MB_ICONERROR | MB_OK);
    return;
  }

  ShowWindow(hWndHost, SW_NORMAL); // nCmdShow [flag SW...] controls how the window is shown
}

/**
 * @brief Creates a child window to join the game as a client
 */
static void CreateJoinWindow(HWND hwnd) {
  static WNDCLASSEX wcexJoin;

  if (!isChildInit[1]) {
    ZeroMemory(&wcexJoin, sizeof(WNDCLASSEX)); // TODO: figure out what this does
    wcexJoin.cbSize = sizeof(WNDCLASSEX);
    wcexJoin.cbClsExtra = 0;
    wcexJoin.cbWndExtra = 0;
    wcexJoin.style = CS_HREDRAW | CS_VREDRAW;
    wcexJoin.hInstance = g_hInstance;
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
        MB_ICONERROR | MB_OK);
      return;
    }
    else {
      isChildInit[1] = TRUE;
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
    g_hInstance,
    NULL
  );

  if (!hWndJoin)
  {
    MessageBox(NULL,
      _T("Join window creation failed!"),
      _T("Windows Desktop Guided Tour"),
      MB_ICONERROR | MB_OK);
    return;
  }

  ShowWindow(hWndJoin, SW_NORMAL);
}

/**
 * @brief Creates a struct ToolTip for cards from the button info
 * @param self The button object of the hovered card in question
 * @return hoverTip A struct containing the card-specific info to display
 */
static struct ToolTip ReturnCardHoverTip(struct Button self) {
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
  tippy.isHover = TRUE;
  tippy.bgcolor = CARD_COLORS[corn.cType];

  // update edge
  if (tippy.x + tippy.width > BWIDTH) {
    tippy.x = BWIDTH - tippy.width - (padding * 2);
  }

  switch (corn.cType) {
  case BABYUNICORN:
    strcpy_s(tippy.subtitle, sizeof tippy.subtitle, "Baby Unicorn card");
    break;
  case BASICUNICORN:
    strcpy_s(tippy.subtitle, sizeof tippy.subtitle, "Basic Unicorn card");
    break;
  case MAGICALUNICORN:
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

/**
 * @brief Creates a struct ToolTip for players from the button info
 * @param pnum The player number
 * @param buttonX The x-coordinate at the top-left corner of the player icon
 * @param buttonY The y-coordinate at the top-left corner of the player icon
 * @return hoverTip A struct containing the player-specific info to display
 */
static struct ToolTip ReturnPlayerHoverTip(int pnum, int buttonX, int buttonY) {
  struct ToolTip tippy;

  int width = 200;
  int height = 60;

  // TODO: update size for longer names
  strcpy_s(tippy.title, sizeof tippy.title, player[pnum].username);
  if (pnum == clientPnum) strcat_s(tippy.title, sizeof tippy.title, " (You)");
  tippy.subtitle[0] = '\0';
  snprintf(tippy.msg, sizeof tippy.msg, "# of cards in hand: %d\n# of unicorns in stable: %d",
    player[pnum].hand.numCards, player[pnum].stable.numUnicorns);
  tippy.fonttitle = BOLDFONT;
  tippy.fonttxt   = OLDFONT;

  tippy.x = buttonX - width - 20;
  tippy.y = buttonY - 10;
  tippy.width   = width;
  tippy.height  = height;
  tippy.isHover = TRUE;
  tippy.bgcolor = RGB(255, 0, 150);

  return tippy;
}

/**
 * @brief Displays the ToolTip UI on the screen
 * @param[out] hdcMem The memory buffer of the graphics device context for the entire display
 * @param[in] hoverTip A struct containing the card/player-specific info to display
 */
static void DisplayCustomToolTip(HDC *hdcMem, struct ToolTip hoverTip) {
  RECT rc;
  int padding = 8;

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

/**
 * @brief Displays the card window specified by enum Tab tabnum at the bottom of the game screen
 * @param[out] hdcMem The memory buffer of the graphics device context for the entire display
 * @param[in] hdcSprite The secondary handle for loading the extra bitmaps into hdcMem
 */
static void DisplayCardWindow(HDC *hdcMem, HDC hdcSprite) {
  // use pages to view the card window in lieu of scroll bars :)

  char windowtxt[64]; // tab window info to show the name of the currently viewed stable/deck
  HGDIOBJ oldSprite;
  POINT start = { 87, 507 }; // starting point for the display
  BITMAP bm;

  GetObject(hBitmapBack, (int)sizeof bm, &bm); // fetches width/height; all cards have the same w/h
  int distance = STABLE_PADDING + bm.bmWidth;

  // cards are just rectangles, so transparentblt isn't necessary
  // this relies on an accurate unicorn/stable/card count; the pages will bork otherwise
  int index_start = (pagenum - 1) * 7;

  // the actual index for the cards, accounting for filtered class and species types
  int skip = 0;

  switch (tabnum) {
  case UNICORN_TAB:
  {
    // draw the background for ease of view
    oldSprite = SelectObject(hdcSprite, hBitmapTab[tabnum]);
    BitBlt(*hdcMem, 0, 497, BWIDTH, 223, hdcSprite, 0, 0, SRCCOPY);
    SelectObject(hdcSprite, oldSprite);

    tabsize = player[pnumindex].stable.numUnicorns;
    snprintf(windowtxt, sizeof windowtxt, "%s's Stable", player[pnumindex].username);

    // TODO: using the original index_start wouldn't account for skipped upgrade/downgrade cards,
    // but adding an extra array to check for offset cards within extra pages seems extremely overkill...
    // 
    // in any case, filtering is still necessary for specific deck functions, and fixing this one by
    // grouping the unicorns w/ upgrade/downgrade cards in a stable wouldn't fully solve the problem
    int count = 0;
    for (int k = 0; count < player[pnumindex].stable.size && k < (pagenum - 1) * 7; count++) {
      if (CheckType(ANYUNICORN, player[pnumindex].stable.unicorns[count].cType))
        k++;
    }
    index_start = count;

    for (int i = index_start; i < player[pnumindex].stable.size && skip < 7; i++) {
      if (CheckType(ANYUNICORN, player[pnumindex].stable.unicorns[i].cType)) {
        // save the source into cardSlots
        cardSlots[skip].source = &player[pnumindex].stable.unicorns[i];

        // draw the sprite
        oldSprite = SelectObject(hdcSprite, hBitmapCard[player[pnumindex].stable.unicorns[i].bitmap]);
        BitBlt(*hdcMem, start.x + (distance * skip), start.y, bm.bmWidth, bm.bmHeight, hdcSprite, 0, 0, SRCCOPY);
        SelectObject(hdcSprite, oldSprite);
        skip++;
      }
    }
    break;
  }
  case UPGRADE_TAB:
  {
    // draw the background for ease of view
    oldSprite = SelectObject(hdcSprite, hBitmapTab[tabnum]);
    BitBlt(*hdcMem, 0, 497, BWIDTH, 223, hdcSprite, 0, 0, SRCCOPY);
    SelectObject(hdcSprite, oldSprite);

    // TODO: see unicorn_tab for the indexing issues. this is unlikely to happen in a normal game though due to most players having less than 8 up/downgrades
    tabsize = player[pnumindex].stable.size - player[pnumindex].stable.numUnicorns;
    snprintf(windowtxt, sizeof windowtxt, "%s's Stable", player[pnumindex].username);

    int j = 0;
    for (int count = 0; j < player[pnumindex].stable.size && count < (pagenum - 1) * 7; j++) {
      if (!CheckType(ANYUNICORN, player[pnumindex].stable.unicorns[j].cType))
        count++;
    }
    index_start = j;

    for (int i = index_start; i < player[pnumindex].stable.size && skip < 7; i++) {
      if (player[pnumindex].stable.unicorns[i].cType == UPGRADE || player[pnumindex].stable.unicorns[i].cType == DOWNGRADE) {
        // save the source into cardSlots
        cardSlots[skip].source = &player[pnumindex].stable.unicorns[i];

        // draw the sprite
        oldSprite = SelectObject(hdcSprite, hBitmapCard[player[pnumindex].stable.unicorns[i].bitmap]);
        BitBlt(*hdcMem, start.x + (distance * skip), start.y, bm.bmWidth, bm.bmHeight, hdcSprite, 0, 0, SRCCOPY);
        SelectObject(hdcSprite, oldSprite);
        skip++;
      }
    }
    break;
  }
  case HAND_TAB:
  {
    // draw the background for ease of view
    oldSprite = SelectObject(hdcSprite, hBitmapTab[tabnum]);
    BitBlt(*hdcMem, 0, 497, BWIDTH, 223, hdcSprite, 0, 0, SRCCOPY);
    SelectObject(hdcSprite, oldSprite);

    tabsize = player[pnumindex].hand.numCards;
    snprintf(windowtxt, sizeof windowtxt, "%s's Hand", player[pnumindex].username);

    for (int i = index_start; i < tabsize && i < index_start + 7; i++) {
      // save the source into cardSlots
      cardSlots[skip++].source = &player[pnumindex].hand.cards[i];

      oldSprite = SelectObject(hdcSprite, hBitmapCard[player[pnumindex].hand.cards[i].bitmap]);
      BitBlt(*hdcMem, start.x + (distance * (i % 7)), start.y, bm.bmWidth, bm.bmHeight, hdcSprite, 0, 0, SRCCOPY);
      SelectObject(hdcSprite, oldSprite);
    }
    break;
  }
  case NURSERY_TAB:
  {
    tabsize = nursery.size;
    strcpy_s(windowtxt, sizeof windowtxt, "Nursery");

    for (int i = index_start; i < tabsize && i < index_start + 7; i++) {
      // save the source into cardSlots
      cardSlots[skip++].source = &nursery.cards[i];

      oldSprite = SelectObject(hdcSprite, hBitmapCard[nursery.cards[i].bitmap]);
      BitBlt(*hdcMem, start.x + (distance * (i % 7)), start.y, bm.bmWidth, bm.bmHeight, hdcSprite, 0, 0, SRCCOPY);
      SelectObject(hdcSprite, oldSprite);
    }
    break;
  }
  case DECK_TAB:
  {
    // TODO: include a check for card effects that allow deck viewing, and then include a check for specific cards
    tabsize = deck.size;
    strcpy_s(windowtxt, sizeof windowtxt, "Deck");

    if (menuState != DEBUGMODE) {
      break;
    }

    for (int i = index_start; i < tabsize && i < index_start + 7; i++) {
      // save the source into cardSlots
      cardSlots[skip++].source = &deck.cards[i];

      oldSprite = SelectObject(hdcSprite, hBitmapCard[deck.cards[i].bitmap]);
      BitBlt(*hdcMem, start.x + (distance * (i % 7)), start.y, bm.bmWidth, bm.bmHeight, hdcSprite, 0, 0, SRCCOPY);
      SelectObject(hdcSprite, oldSprite);
    }
    break;
  }
  case DISCARD_TAB:
  {
    // TODO: (maybe) include a check for specific cards
    tabsize = discardpile.size;
    strcpy_s(windowtxt, sizeof windowtxt, "Discard Pile");

    for (int i = index_start; i < tabsize && i < index_start + 7; i++) {
      // save the source into cardSlots
      cardSlots[skip++].source = &discardpile.cards[i];

      oldSprite = SelectObject(hdcSprite, hBitmapCard[discardpile.cards[i].bitmap]);
      BitBlt(*hdcMem, start.x + (distance * (i % 7)), start.y, bm.bmWidth, bm.bmHeight, hdcSprite, 0, 0, SRCCOPY);
      SelectObject(hdcSprite, oldSprite);
    }
    break;
  }
  default:
    // what happened???
    break;
  }

  // reset the card button sources so that they don't erroneously trigger the hover/click actions on empty spaces for different tab views
  while (skip < 7) {
    cardSlots[skip++].source = NULL;
  }

  // show the page arrow icons if applicable
  if (tabsize > pagenum * 7) {
    oldSprite = SelectObject(hdcSprite, debugButtons[PAGE_RIGHT].bitmap);
    TransparentBlt(*hdcMem, debugButtons[PAGE_RIGHT].x, debugButtons[PAGE_RIGHT].y, debugButtons[PAGE_RIGHT].width, debugButtons[PAGE_RIGHT].height, hdcSprite, 0, 0, debugButtons[PAGE_RIGHT].width, debugButtons[PAGE_RIGHT].height, RGB(0, 255, 0));
    SelectObject(hdcSprite, oldSprite);
  }
  if (pagenum > 1) {
    oldSprite = SelectObject(hdcSprite, debugButtons[PAGE_LEFT].bitmap);
    TransparentBlt(*hdcMem, debugButtons[PAGE_LEFT].x, debugButtons[PAGE_LEFT].y, debugButtons[PAGE_LEFT].width, debugButtons[PAGE_LEFT].height, hdcSprite, 0, 0, debugButtons[PAGE_LEFT].width, debugButtons[PAGE_LEFT].height, RGB(0, 255, 0));
    SelectObject(hdcSprite, oldSprite);
  }

  // display the window tab as text
  SelectObject(*hdcMem, fonts[FANCYITALICS]);
  SetBkMode(*hdcMem, TRANSPARENT); // box surrounding text is transparent instead of white
  SetTextColor(*hdcMem, RGB(255, 255, 255));
  RECT rc = { 5, 425, 935, 457 }; // left side directly above the tabs
  DrawText(*hdcMem, windowtxt, strlen(windowtxt), &rc, DT_LEFT | DT_NOCLIP | DT_WORDBREAK);
}

// ********************************************************************************
// ******************** Initializing/Deinitializing Misc. Data ********************
// ********************************************************************************

static void LoadImages(HWND hWnd) {
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
    if (hBitmapBorder[i] == NULL) {
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

  // playerNums are paired with icons at the same coordinates, but the bitmaps should be separate
  for (int i = 0; i < sizeof playerNums / sizeof playerNums[0]; i++) {
    playerNums[i].bitmap = (HBITMAP)LoadImage(NULL, playerNums[i].filename,
      IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    if (playerNums[i].bitmap == NULL) {
      issuccess = FALSE;
      strcat_s(errors, sizeof errors, playerNums[i].filename);
      strcat_s(errors, sizeof errors, " ");
    }
  }

  // the red player numbers to signify whose turn it is
  for (int i = 0; i < MAX_PLAYERS; i++) {
    char playerfn[32] = "";

    snprintf(playerfn, 22, "Assets\\player%dred.bmp", i + 1);
    hBitmapCurrPlayer[i] = (HBITMAP)LoadImage(NULL, playerfn,
      IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    if (hBitmapCurrPlayer[i] == NULL) {
      issuccess = FALSE;
      strcat_s(errors, sizeof errors, playerfn);
      strcat_s(errors, sizeof errors, " ");
    }
  }

  // bitmap for the decorative image around the client-specific player icon
  hBitmapClient = (HBITMAP)LoadImage(NULL, "Assets\\player_self.bmp",
    IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
  if (hBitmapClient == NULL) {
    issuccess = FALSE;
    strcat_s(errors, sizeof errors, "player_self.bmp ");
  }

  // bitmaps displaying the stable count; it goes from 0 to 7, so 8 in total
  for (int i = 0; i < 8; i++) {
    snprintf(stableNums[i].filename, 19, "Assets\\stable%d.bmp", i);
    stableNums[i].bitmap = (HBITMAP)LoadImage(NULL, stableNums[i].filename,
      IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    if (stableNums[i].bitmap == NULL) {
      issuccess = FALSE;
      strcat_s(errors, sizeof errors, stableNums[i].filename);
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

  if (issuccess == FALSE) {
    MessageBox(hWnd, errors, "Error", MB_ICONWARNING | MB_OK);
  }
}

// TODO: this isn't totally fool-proof because it doesn't map the deck index w/ the file names
// it's just assuming that the images/deck are both sorted
// writing a script to generate a map might be easier and/or more maintainable *shrugs*
static void LoadCards(HWND hWnd) {

  // TODO: (maybe) use a default error texture for files that couldn't load in the case of missing files
  // ideally the error texture(s) should just be a resource file tbh, same for the back of the card

  char errors[9999] = "Failed to load cards(s) ";
  BOOL issuccess = TRUE;

  // the back of the card; used for unknown cards that the player shouldn't know (i.e. someone else's hand)
  hBitmapBack = (HBITMAP)LoadImage(NULL, "Assets\\back.bmp",
    IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
  if (hBitmapBack == NULL) {
    issuccess = FALSE;
    strcat_s(errors, sizeof errors, "back.bmp ");
  }

  hBitmapCard[sizeof(hBitmapCard) - 1] = hBitmapBack;

  // read through the file directory to find the bitmaps
  HANDLE fileHandle;
  WIN32_FIND_DATA ffd;
  fileHandle = FindFirstFile("Assets\\Cards\\*.bmp", &ffd);
  
  char tmp[128];
  int decksize = sizeof Base_DECK / sizeof Base_DECK[0];
  int index = 0;
  int mapindex = 0;

  snprintf(tmp, sizeof tmp,  "Assets\\Cards\\%s", ffd.cFileName);
  hBitmapCard[mapindex] = (HBITMAP)LoadImage(NULL, tmp,
    IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
  if (hBitmapCard[mapindex] == NULL) {
    issuccess = FALSE;
    strcat_s(errors, sizeof errors, ffd.cFileName);
    strcat_s(errors, sizeof errors, ", ");
  }
  Base_DECK[index].bitmap = mapindex;
  index++;
  mapindex++;
  FindNextFile(fileHandle, &ffd);

  do {
    // assuming the deck is already sorted
    while (index < decksize && strcmp(Base_DECK[index].name, Base_DECK[index - 1].name) == 0) {
      Base_DECK[index].bitmap = Base_DECK[index - 1].bitmap;
      index++;
    }

    snprintf(tmp, sizeof tmp, "Assets\\Cards\\%s", ffd.cFileName);
    hBitmapCard[mapindex] = (HBITMAP)LoadImage(NULL, tmp,
      IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    if (hBitmapCard[mapindex] == NULL) {
      issuccess = FALSE;
      strcat_s(errors, sizeof errors, ffd.cFileName);
      strcat_s(errors, sizeof errors, ", ");
    }
    Base_DECK[index].bitmap = mapindex;
    index++;
    mapindex++;
  } while (FindNextFile(fileHandle, &ffd) != 0 && index < decksize);

  // in case there are missing files, in which i should have just named the cards after their names in the deck
  while (index < decksize) {
    Base_DECK[index++].bitmap = CARD_MAP[sizeof CARD_MAP - 1];
  }

  if (issuccess == FALSE) {
    MessageBox(hWnd, errors, "Error", MB_ICONWARNING | MB_OK);
  }

  FindClose(fileHandle);
}

static void InitFonts(HWND hWnd) {
  const int pntpi = 72; // points-per-inch
  int pxpi, points, pxheight;

  HDC hdc = GetDC(hWnd);

  fonts[OLDFONT] = GetCurrentObject(hdc, OBJ_FONT); // save old font

  pxpi = GetDeviceCaps(hdc, LOGPIXELSY); // pixels-per-inch

  points = 30;
  pxheight = -(points * pxpi / pntpi);
  fonts[CHONKYFONT] = CreateFontA(
    pxheight, 0,        //!< size
    0, 0,               //!< normal orientation
    FW_BOLD,            //!< bold weight
    0, 0, 0,            //!< not italicized, underlined, or struck
    DEFAULT_CHARSET,
    OUT_OUTLINE_PRECIS, //!< select only outline (not bitmap) fonts
    CLIP_DEFAULT_PRECIS,
    CLEARTYPE_QUALITY,
    VARIABLE_PITCH | FF_SWISS,
    "Arial");

  points = 16;
  pxheight = -(points * pxpi / pntpi);
  fonts[BOLDFONT] = CreateFontA(
    pxheight, 0,        //!< size
    0, 0,               //!< normal orientation
    FW_BOLD,            //!< bold weight
    0, 0, 0,            //!< not italicized, underlined, or struck
    DEFAULT_CHARSET,
    OUT_OUTLINE_PRECIS, //!< select only outline (not bitmap) fonts
    CLIP_DEFAULT_PRECIS,
    CLEARTYPE_QUALITY,
    VARIABLE_PITCH | FF_SWISS,
    "Arial");

  points = 11;
  pxheight = -(points * pxpi / pntpi);
  fonts[ITALICFONT] = CreateFontA(
    pxheight, 0,        //!< size
    0, 0,               //!< normal orientation
    FW_NORMAL,          //!< normal weight--e.g., bold would be FW_BOLD
    TRUE, 0, 0,         //!< italicized, but not underlined or struck
    DEFAULT_CHARSET,
    OUT_OUTLINE_PRECIS, //!< select only outline (not bitmap) fonts
    CLIP_DEFAULT_PRECIS,
    CLEARTYPE_QUALITY,
    VARIABLE_PITCH | FF_SWISS,
    "Times New Roman");

  LPCSTR font_resource = (LPCSTR)"Assets\\runescape_large.ttf";
  AddFontResourceA(font_resource);

  points = 24;
  pxheight = -(points * pxpi / pntpi);
  fonts[FANCYFONT] = CreateFontA(
    pxheight, 0,          //!< size
    0, 0,                 //!< normal orientation
    FW_NORMAL,            //!< normal weight--e.g., bold would be FW_BOLD
    0, 0, 0,              //!< not italicized, underlined, or struck
    DEFAULT_CHARSET,
    OUT_OUTLINE_PRECIS,   //!< select only outline (not bitmap) fonts
    CLIP_DEFAULT_PRECIS,
    CLEARTYPE_QUALITY,
    VARIABLE_PITCH | FF_SWISS,
    "RuneScape Large");

  fonts[FANCYITALICS] = CreateFontA(
    pxheight, 0,          //!< size
    0, 0,                 //!< normal orientation
    FW_NORMAL,            //!< normal weight--e.g., bold would be FW_BOLD
    TRUE, 0, 0,           //!< italicized, but not underlined or struck
    DEFAULT_CHARSET,
    OUT_OUTLINE_PRECIS,   //!< select only outline (not bitmap) fonts
    CLIP_DEFAULT_PRECIS,
    CLEARTYPE_QUALITY,
    VARIABLE_PITCH | FF_SWISS,
    "RuneScape Large");
}

static void DestroyFonts(void) {
  for (int i = 1; i < sizeof fonts / sizeof fonts[0]; i++) {
    DeleteObject(fonts[i]);
  }
  RemoveFontResourceA((LPCSTR)"Assets\\runescape_large.ttf");
}

static void InitDebugMode(void) {
  // add some arbitrary number of cards to the player's stable for testing purposes
  // TODO: if I want to quickly edit future tests, then I can read text files for easier input and testing

  // test 1
  // 
  // // this should output 7 unicorns in the first page and 1 in the 2nd page
  // // WILL BUG OUT if i keep the stable_size at 25 instead of 100+, so use
  // // this test specifically for the deck later on
  // for (int i = 0; i < 80; i++) {
  //   player[0].stable.unicorns[i] = (i % 10) ? 128 : 30;
  //   if (CheckType(ANYUNICORN, deck[player[0].stable.unicorns[i]].cType))
  //     player[0].stable.numUnicorns++;
  //   player[0].stable.size++;
  // }

  // test 2
  // 
  // // this should output 7 unicorns in the first page and 1 in the 2nd page
  // strcpy_s(player[0].username, sizeof player[0].username, "host");
  // player[0].icon = BABYNARWHAL;
  // for (int i = 0; i < 15; i++) {
  //   player[0].stable.unicorns[i] = (i & 1) ? 128 : 30;
  //   if (CheckType(ANYUNICORN, deck[player[0].stable.unicorns[i]].cType))
  //     player[0].stable.numUnicorns++;
  //   player[0].stable.size++;
  // }

  srand((unsigned int)time(NULL));

  // player 1: host
  // this should output 7 unicorns in the first page and 1 in the 2nd page
  strcpy_s(player[0].username, sizeof player[0].username, "host");
  // strcpy_s(player[0].username, sizeof player[0].username, "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW");
  player[0].icon = BABYNARWHAL;
  for (int i = 0; i < 10; i++) {
    BOOL isvalid = FALSE;
    do {
      player[0].stable.unicorns[i] = Base_DECK[NURSERY_SIZE + rand() % 116];
      if (CheckType(ANYUNICORN, player[0].stable.unicorns[i].cType) ||
          CheckType(UPGRADE, player[0].stable.unicorns[i].cType) ||
          CheckType(DOWNGRADE, player[0].stable.unicorns[i].cType)) {
        isvalid = TRUE;
      }
    } while (!isvalid);

    if (CheckType(ANYUNICORN, player[0].stable.unicorns[i].cType))
      player[0].stable.numUnicorns++;
    player[0].stable.size++;
  }
  for (int i = 0; i < 5; i++) {
    // player[0].hand.cards[i] = 60; // #60 is shark with a horn
    player[0].hand.cards[i] = Base_DECK[NURSERY_SIZE + rand() % 116];
    player[0].hand.numCards++;
  }

  // player 2: noob
  strcpy_s(player[1].username, sizeof player[1].username, "nooblet");
  player[1].icon = rand() % 12;
  int handsize = 1 + rand() % 7;
  for (int i = 0; i < 5; i++) {
    player[1].stable.unicorns[i] = Base_DECK[(i & 1) ? 50 : 40];
    if (CheckType(ANYUNICORN, player[1].stable.unicorns[i].cType))
      player[1].stable.numUnicorns++;
    player[1].stable.size++;
  }
  for (int i = 0; i < handsize; i++) {
    player[1].hand.cards[i] = Base_DECK[42 + i];
    player[1].hand.numCards++;
  }

  currentPlayers = 2;
  pnumindex = 0;
}

static void ResetDebugMode(void) {
  // reset player's cards/stable/etc. to 0;
  // TODO: this should be implemented as a hard reset when you join the game, but for now this will close off the init function
  for (int i = 0; i < MAX_PLAYERS; i++) {
    memset(player[i].stable.unicorns, 0, sizeof player[i].stable / sizeof player[i].stable.unicorns[0]);
    memset(player[i].hand.cards, 0, sizeof player[i].hand.cards / sizeof player[i].hand.cards[0]);
    memset(player[i].username, 0, sizeof player[i].username / sizeof player[i].username[0]);
    player[i].stable.numUnicorns = 0;
    player[i].stable.size = 0;
    player[i].hand.numCards = 0;
  }
  currentPlayers = 1;
}

// ********************************************************************************
// ********************* General Game Logic Helper Functions **********************
// ********************************************************************************

int SelectBabyUnicorn(int pnum, POINT pnt) {
  for (int i = 0; i < 13; i++) {
    if (pnt.y >= BABY_COORDS[i].top && pnt.y <= BABY_COORDS[i].bottom &&
        pnt.x >= BABY_COORDS[i].left && pnt.x <= BABY_COORDS[i].right &&
        babyToggle[i] == FALSE) {
      babyToggle[i] = TRUE;
      // checks if the player selected a previous baby unicorn just to make sure the 
      // player unicorn index isn't either garbage info or mistakenly zero when someone else may have
      // zero as an actual index in use (i.e. the red baby unicorn)
      if (pselect[pnum].left != 0) {
        // resets babyToggle at old position (since baby toggle goes from 0-12 just like the
        // baby unicorn card numbers, player[num].stable.unicorns[0] can be used as an index)
        babyToggle[babyMap[pnum]] = FALSE;
      }
      babyMap[pnum] = i;
      player[pnum].icon = i;

      pselect[pnum].left = BABY_COORDS[i].left - 7; // the actual border should be offset by 7 pixels
      pselect[pnum].top = BABY_COORDS[i].top;
      PlaySound(TEXT("Assets\\Audio\\lobby-select.wav"), NULL, SND_FILENAME | SND_ASYNC);
      return 1;
    }
  }

  return 0;
}

int SelectCard(int pnum, enum Tab *windownum, POINT pnt) {
  if (tabnum == *windownum ||
      (*windownum == ACTION_TAB && pnt.y > debugButtons[DECK_TAB].y + debugButtons[DECK_TAB].height)) {
    for (int i = 0; i < sizeof cardSlots / sizeof cardSlots[0]; i++) {
      if (pnum == pnumindex &&
          pnt.x >= cardSlots[i].x && pnt.x < cardSlots[i].x + cardSlots[i].width &&
          pnt.y >= cardSlots[i].y && pnt.y < cardSlots[i].y + cardSlots[i].height) {
        if (cardSlots[i].source != NULL) {
          return (pagenum * 7) + i;
        }
      }
    }
  }
  else {
    if (pnt.x >= debugButtons[DECK_TAB].x && pnt.x < debugButtons[DECK_TAB].x + debugButtons[DECK_TAB].width &&
        pnt.y >= debugButtons[DECK_TAB].y && pnt.y < debugButtons[DECK_TAB].y + debugButtons[DECK_TAB].height) {
      return -1;
    }
  }

  return -2;
}

void SetTabs(int pnum) {
  pnumindex = pnum;
  tabnum = HAND_TAB;
}

void SetClientPnum(int pnum) {
  clientPnum = pnum;
}

void SetCurrPnum(int pnum) {
  currPnum = pnum;
}

// ********************************************************************************
// *************************** Game State WM Functions ****************************
// ********************************************************************************

static StateManager stateMach[NUMSTATES];

static void InitStateMachine(void) {
  // stateMach = (StateManager*)malloc(NUMSTATES * sizeof(StateManager));
  stateMach[TITLEBLANK] = (StateManager){ TITLEBLANK, PaintTitle, HoverTitle, ClickTitle };
  stateMach[RULESONE]   = (StateManager){ RULESONE  , NULL      , NULL      , ClickRules };
  stateMach[RULESTWO]   = (StateManager){ RULESTWO  , NULL      , NULL      , ClickRules };
  stateMach[LOBBY]      = (StateManager){ LOBBY     , PaintLobby, NULL      , ClickLobby };
  stateMach[GAMESTART]  = (StateManager){ GAMESTART , PaintDebug, HoverDebug, ClickGame  };
  stateMach[DEBUGMODE]  = (StateManager){ DEBUGMODE , PaintDebug, HoverDebug, ClickDebug };
}

// WM_MOUSEMOVE

static void HoverTitle(POINT pnt) {
  hornButton.source = FALSE;

  for (int i = 0; i < 3; i++) {
    if (pnt.x >= titleButtons[i].x && pnt.x <= titleButtons[i].x + titleButtons[i].width &&
        pnt.y >= titleButtons[i].y && pnt.y <= titleButtons[i].y + titleButtons[i].height) {
      HornPosition(titleButtons[i]);
      return;
    }
  }
}

static void HoverDebug(POINT pnt) {
  hoverTip.isHover = FALSE;

  for (int i = 0; i < sizeof cardSlots / sizeof cardSlots[0]; i++) {
    if (pnt.x >= cardSlots[i].x && pnt.x < cardSlots[i].x + cardSlots[i].width &&
        pnt.y >= cardSlots[i].y && pnt.y < cardSlots[i].y + cardSlots[i].height) {
      if (cardSlots[i].source != NULL) {
        hoverTip = ReturnCardHoverTip(cardSlots[i]);
        return;
      }
    }
  }

  for (int i = 0; i < currentPlayers; i++) {
    if (pnt.x >= playerNums[i].x && pnt.x < playerNums[i].x + playerNums[i].width &&
        pnt.y >= playerNums[i].y && pnt.y < playerNums[i].y + playerNums[i].height) {
      hoverTip = ReturnPlayerHoverTip(i, playerNums[i].x, playerNums[i].y);
      return;
    }
  }
}

// WM_LBUTTONDOWN

static void ClickTitle(POINT pnt) {
#ifdef _DEBUG
  if (pnt.x >= 0 && pnt.x <= 50 && pnt.y >= 0 && pnt.y <= 50) {
    menuState = DEBUGMODE;
    tabnum = UNICORN_TAB;
    InitDebugMode(); // for card testing purposes
    return;
  }
#endif

  for (int i = 0; i < sizeof titleButtons / sizeof titleButtons[0]; i++) {
    if (pnt.x >= titleButtons[i].x && pnt.x < titleButtons[i].x + titleButtons[i].width &&
        pnt.y >= titleButtons[i].y && pnt.y < titleButtons[i].y + titleButtons[i].height) {
      // left click action
      if (isChildOpen == FALSE) {
        PlaySound(TEXT("Assets\\Audio\\button-click.wav"), NULL, SND_FILENAME | SND_ASYNC);
        titleButtons[i].OnClick(titleButtons[i].source);
        hornButton.source = FALSE;
        return;
      }
    }
  }
}

static void ClickRules(POINT pnt) {
  for (int i = 0; i < sizeof ruleButtons / sizeof ruleButtons[0]; i++) {
    if (pnt.x >= ruleButtons[i].x && pnt.x < ruleButtons[i].x + ruleButtons[i].width &&
        pnt.y >= ruleButtons[i].y && pnt.y < ruleButtons[i].y + ruleButtons[i].height) {
      // left click action
      PlaySound(TEXT("Assets\\Audio\\button-click.wav"), NULL, SND_FILENAME | SND_ASYNC);
      ruleButtons[i].OnClick(ruleButtons[i].source);
      return;
    }
  }
}

volatile DWORD whatever;
static void ClickLobby(POINT pnt) {
  int ret;

  // TODO: (maybe) change the structure of RECTs [BABY_COORDS] into a general button structure tying into the whole GetAsyncKeyState if statement w/ the start/leave buttons
  // TODO: implement this with R-trees (specifically because the selectbabyunicorn function
  // has a lot of "buttons") so that it's more optimally chunked into bigger pieces

  // TODO: use a mutex to selectively access currentPlayers, networkToggle, and clientPnt
  // DWORD waitResult;
  // waitResult = WaitForSingleObject(
  //   mutex,      // mutex handle
  //   INFINITE);  // no time-out interval
  // 
  // switch (waitResult) {
  // case WAIT_OBJECT_0:
  //   if (pnt.y >= 590 && pnt.y <= 639) {
  //     if (pnt.x >= 360 && pnt.x <= 549) {
  //       // user clicked the leave button
  //       networkToggle ^= 1;
  //       if (!isClient) {
  //         closesocket(udpfd);
  //       }
  //       return;
  //     }
  //     else if (pnt.x >= 120 && pnt.x <= 309) {
  //       // user clicked the start button; only the host can properly start the game
  //       if (!isClient && currentPlayers >= 2) {
  //         networkToggle ^= 4;
  //         closesocket(udpfd);
  //       }
  //       return;
  //     }
  //   }
  // 
  //   if (isClient) {
  //     clientPnt = pnt;
  //     networkToggle ^= 2;
  //   }
  //   else {
  //     ret = SelectBabyUnicorn(0, pnt); // server is always player index 0
  //     if (ret) {
  //       networkToggle ^= 2;
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
      PlaySound(TEXT("Assets\\Audio\\button-click.wav"), NULL, SND_FILENAME | SND_ASYNC);
      lobbyButtons[i].OnClick(lobbyButtons[i].source);
      return;
    }
  }

  if (isClient) {
    clientPnt = pnt;
    networkToggle ^= 2;
  }
  else {
    ret = SelectBabyUnicorn(0, pnt); // server is always player index 0
    if (ret && currentPlayers >= 2) {
      // only toggle it when there are actual players or else it won't untoggle on its own
      networkToggle ^= 2;
    }
  }
}

static void ClickDebug(POINT pnt) {
  if (pnt.x >= 0 && pnt.x <= 50 && pnt.y >= 0 && pnt.y <= 50) {
    menuState = TITLEBLANK;
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
      debugButtons[i].OnClick(debugButtons[i].source);

      turnCount = 1;
      networkToggle ^= 2;
      clientPnt = pnt;
      ActionPhase(0);
      return;
    }
  }

  // select the player to view
  // 
  // TODO: should probably only initialize playerNums up to the current_player count instead of the full MAX_PLAYERS count
  // anything with playerNums will bug out unless the loops go up to currentPlayers, so limiting the playerNums array
  // or initializing it as negative numbers would reduce the possibility of more bugs popping up
  // 
  // TODO: will need to add an additional action here (or in a separate state) for whenever the user has to select a player to
  // apply their card effect on; could also potentially just make another window for those cases, but that sounds lame :/
  for (int i = 0; i < currentPlayers; i++) {
    if (pnt.x >= playerNums[i].x && pnt.x < playerNums[i].x + playerNums[i].width &&
        pnt.y >= playerNums[i].y && pnt.y < playerNums[i].y + playerNums[i].height) {
      pnumindex = i;
      pagenum = 1;

      // this will switch to the new player window in case the user previously looked at the nursery, deck, or discard piles
      if (tabnum == NURSERY_TAB || tabnum == DECK_TAB || tabnum == DISCARD_TAB) {
        tabnum = UNICORN_TAB;
      }

      PlaySound(TEXT("Assets\\Audio\\button-click.wav"), NULL, SND_FILENAME | SND_ASYNC);
      return;
    }
  }
}

static void ClickGame(POINT pnt) {
  // signal the game thread to select a card w/ the point
  // TODO: actually use a mutex here >.<
  networkToggle ^= 2;
  clientPnt = pnt;

  // general tab buttons
  for (int i = 0; i < sizeof debugButtons / sizeof debugButtons[0]; i++) {
    if (pnt.x >= debugButtons[i].x && pnt.x < debugButtons[i].x + debugButtons[i].width &&
        pnt.y >= debugButtons[i].y && pnt.y < debugButtons[i].y + debugButtons[i].height) {
      // left click action
      debugButtons[i].OnClick(debugButtons[i].source);
      return;
    }
  }

  // select the player to view
  // 
  // TODO: should probably only initialize playerNums up to the current_player count instead of the full MAX_PLAYERS count
  // anything with playerNums will bug out unless the loops go up to currentPlayers, so limiting the playerNums array
  // or initializing it as negative numbers would reduce the possibility of more bugs popping up
  // 
  // TODO: will need to add an additional action here (or in a separate state) for whenever the user has to select a player to
  // apply their card effect on; could also potentially just make another window for those cases, but that sounds lame :/
  for (int i = 0; i < currentPlayers; i++) {
    if (pnt.x >= playerNums[i].x && pnt.x < playerNums[i].x + playerNums[i].width &&
        pnt.y >= playerNums[i].y && pnt.y < playerNums[i].y + playerNums[i].height) {
      pnumindex = i;
      pagenum = 1;

      // this will switch to the new player window in case the user previously looked at the nursery, deck, or discard piles
      if (tabnum == NURSERY_TAB || tabnum == DECK_TAB || tabnum == DISCARD_TAB) {
        tabnum = UNICORN_TAB;
      }

      PlaySound(TEXT("Assets\\Audio\\button-click.wav"), NULL, SND_FILENAME | SND_ASYNC);
      return;
    }
  }
}

// WM_PAINT

static void PaintTitle(HDC *hdcMem) {
  HDC hdcSprite;
  HGDIOBJ oldSprite;

  hdcSprite = CreateCompatibleDC(*hdcMem);

  if ((BOOL)hornButton.source) {
    oldSprite = SelectObject(hdcSprite, hornButton.bitmap);
    TransparentBlt(*hdcMem, hornButton.x, hornButton.y, hornButton.width, hornButton.height, hdcSprite, 0, 0, hornButton.width, hornButton.height, RGB(0, 255, 0));
    SelectObject(hdcSprite, oldSprite);
  }
  DeleteDC(hdcSprite);
}

static void PaintLobby(HDC *hdcMem) {
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
  DrawText(*hdcMem, partyMems, strlen(partyMems), &rc, DT_LEFT);

  // draw borders for the chosen baby unicorns
  hdcSprite = CreateCompatibleDC(*hdcMem);
  for (int i = 0; i < currentPlayers; i++) {
    if (pselect[i].left != 0) {
      oldSprite = SelectObject(hdcSprite, hBitmapBorder[i]);
      TransparentBlt(*hdcMem, pselect[i].left, pselect[i].top, BORDERWIDTH, BORDERWIDTH, hdcSprite, 0, 0, BORDERWIDTH, BORDERWIDTH, RGB(0, 255, 0));
      SelectObject(hdcSprite, oldSprite);
    }
  }
  DeleteDC(hdcSprite);
}

static void PaintDebug(HDC *hdcMem) {
  HDC hdcSprite;
  HGDIOBJ oldSprite;

  hdcSprite = CreateCompatibleDC(*hdcMem);

  // display the player boxes on the right side of the in-game screen
  for (int i = 0; i < currentPlayers; i++) {
    // player icon in-game
    oldSprite = SelectObject(hdcSprite, icons[player[i].icon].bitmap);
    TransparentBlt(*hdcMem, playerNums[i].x, playerNums[i].y, playerNums[i].width, playerNums[i].height, hdcSprite, 0, 0, playerNums[i].width, playerNums[i].height, RGB(0, 255, 0));
    SelectObject(hdcSprite, oldSprite);

    // overlay for the client's player icon
    if (i == clientPnum) {
      oldSprite = SelectObject(hdcSprite, hBitmapClient);
      TransparentBlt(*hdcMem, playerNums[i].x + clientOverlayIcon.offsetX, playerNums[i].y + clientOverlayIcon.offsetY, clientOverlayIcon.width, clientOverlayIcon.height, hdcSprite, 0, 0, clientOverlayIcon.width, clientOverlayIcon.height, RGB(0, 255, 0));
      SelectObject(hdcSprite, oldSprite);
    }

    // player number
    if (i == currPnum) {
      // TODO: just switch playerNums[i].bitmap at the start of every turn
      oldSprite = SelectObject(hdcSprite, hBitmapCurrPlayer[i]);
    }
    else {
      oldSprite = SelectObject(hdcSprite, playerNums[i].bitmap);
    }
    TransparentBlt(*hdcMem, playerNums[i].x, playerNums[i].y, playerNums[i].width, playerNums[i].height, hdcSprite, 0, 0, playerNums[i].width, playerNums[i].height, RGB(0, 255, 0));
    SelectObject(hdcSprite, oldSprite);

    // number of unicorns in stable
    if (player[i].stable.numUnicorns < 7) {
      oldSprite = SelectObject(hdcSprite, stableNums[player[i].stable.numUnicorns].bitmap);
    }
    else {
      oldSprite = SelectObject(hdcSprite, stableNums[7].bitmap);
    }
    TransparentBlt(*hdcMem, playerNums[i].x, playerNums[i].y, playerNums[i].width, playerNums[i].height, hdcSprite, 0, 0, playerNums[i].width, playerNums[i].height, RGB(0, 255, 0));
    SelectObject(hdcSprite, oldSprite);
  }

  // display the nursery and discard pile if they are not empty
  if (nursery.size > 0) {
    oldSprite = SelectObject(hdcSprite, hBitmapCard[nursery.cards[0].bitmap]);
    BitBlt(*hdcMem, debugButtons[NURSERY_TAB].x, debugButtons[NURSERY_TAB].y, debugButtons[NURSERY_TAB].width, debugButtons[NURSERY_TAB].height, hdcSprite, 0, 0, SRCCOPY);
    SelectObject(hdcSprite, oldSprite);
  }

  if (discardpile.size > 0) {
    oldSprite = SelectObject(hdcSprite, hBitmapCard[discardpile.cards[discardpile.size - 1].bitmap]);
    BitBlt(*hdcMem, debugButtons[DISCARD_TAB].x, debugButtons[DISCARD_TAB].y, debugButtons[DISCARD_TAB].width, debugButtons[DISCARD_TAB].height, hdcSprite, 0, 0, SRCCOPY);
    SelectObject(hdcSprite, oldSprite);

  }

  // display the list of cards under the current tab
  DisplayCardWindow(hdcMem, hdcSprite);
  DeleteDC(hdcSprite);

  // display the current player as text
  char playertxt[64];
  snprintf(playertxt, sizeof playertxt, "%s is currently playing", player[currPnum].username);

  SelectObject(*hdcMem, fonts[FANCYITALICS]);
  SetBkMode(*hdcMem, TRANSPARENT); // box surrounding text is transparent instead of white
  SetTextColor(*hdcMem, RGB(255, 255, 255));
  RECT rc = { 600, 425, 1270, 457 }; // right side directly above the tabs
  DrawText(*hdcMem, playertxt, strlen(playertxt), &rc, DT_RIGHT | DT_NOCLIP | DT_WORDBREAK);

  if (hoverTip.isHover) {
    DisplayCustomToolTip(hdcMem, hoverTip);
  }
}

// ********************************************************************************
// *************************** Button Function Pointers ***************************
// ********************************************************************************

static void SwitchState(int statenum) {
  menuState = statenum;
}

static void HostGeneration(HWND hWnd) {
  MSG msg;

  // https://stackoverflow.com/questions/17202377/c-creating-a-window-in-a-new-thread
  // this won't work in a thread unless there's a message loop, but that would create bliting issues
  CreateHostWindow(hWnd);

  while (isChildOpen) {
    if (GetMessage(&msg, NULL, 0, 0)) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
  }
}

static void JoinGeneration(HWND hWnd) {
  MSG msg;

  // https://stackoverflow.com/questions/17202377/c-creating-a-window-in-a-new-thread
  // this won't work in a thread unless there's a message loop, but that would create bliting issues
  CreateJoinWindow(hWnd);

  while (isChildOpen) {
    if (GetMessage(&msg, NULL, 0, 0)) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
  }
}

static void LeaveLobby(void) {
  // user clicked the leave button
  networkToggle ^= 1;
  if (!isClient) {
    closesocket(udpfd);
  }

  // menuState will switch in the server file
}

static void StartGame(void) {
  // user clicked the start button; only the host can properly start the game
  if (!isClient && currentPlayers >= 2) {
    networkToggle ^= 4;
    closesocket(udpfd);
  }

  // menuState will switch in the server file
}

static void ViewDeck(void) {
  // the deck should only be viewable during special card events (e.g. classy narwhal allowing the player to pick an upgrade card from the deck)
  // this checks against a random magic number for now, but it will be changed once the implementation is sorted out
  if (((networkToggle & 255) == 255) || menuState == DEBUGMODE) {
    tabnum = DECK_TAB;
    pagenum = 1;
  }

  PlaySound(TEXT("Assets\\Audio\\button-click.wav"), NULL, SND_FILENAME | SND_ASYNC);
}

static void SwitchTab(int num) {
  tabnum = num;
  pagenum = 1;
  PlaySound(TEXT("Assets\\Audio\\button-click.wav"), NULL, SND_FILENAME | SND_ASYNC);
}

static void TurnPage(int page) {
  if (page == PAGE_RIGHT && tabsize > pagenum * 7) {
    pagenum++;
    PlaySound(TEXT("Assets\\Audio\\page-turn.wav"), NULL, SND_FILENAME | SND_ASYNC);
  }
  else if (page == PAGE_LEFT && pagenum > 1) {
    pagenum--;
    PlaySound(TEXT("Assets\\Audio\\page-turn.wav"), NULL, SND_FILENAME | SND_ASYNC);
  }
}

// ********************************************************************************
// ****************************** Callback Functions ******************************
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
  HGDIOBJ oldBitmap;
  HBITMAP hBitmapBuffer;

  switch (uMsg)
  {
  case WM_CREATE:
    LoadImages(hWnd);
    LoadCards(hWnd);
    InitFonts(hWnd);
    InitButtonManager(hWnd);
    InitStateMachine();

    // initialize the deck here for now
    InitDeck(&nursery, &deck, &discardpile);

    // initialize the network states too
    InitNetworkStates();

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

    if (stateMach[menuState].StateHover != NULL)
      stateMach[menuState].StateHover(pnt);

    break;
  }
  case WM_LBUTTONDOWN:
  {
    // for clicking purposes
    GetCursorPos(&pnt);
    ScreenToClient(hWnd, &pnt);

    // if (wParam == WA_ACTIVE)
    stateMach[menuState].StateClick(pnt);

    break;
  }
  case WM_KEYDOWN:
    // TODO: don't forget to add the gamestate here too
    if (menuState != DEBUGMODE && menuState != GAMESTART) {
      break;
    }

    // TODO: (bug) hoverTip doesn't update when the page turns, so it's a bit scuffed...
    if (wParam == VK_LEFT || wParam == 0x41 || wParam == 0x61) {
      // user pressed the left arrow key, 'a', or 'A'
      TurnPage(PAGE_LEFT);
    }
    else if (wParam == VK_RIGHT || wParam == 0x44 || wParam == 0x64) {
      // user pressed the right arrow key, 'd', or 'D'
      TurnPage(PAGE_RIGHT);
    }
    else if ((wParam >= 0x31 && wParam <= 0x38) || (wParam >= 0x61 && wParam <= 0x68)) {
      int tmp = (wParam > 0x38) ? wParam - 0x61 : wParam - 0x31;

      if (tmp < currentPlayers) {
        pnumindex = tmp;
        pagenum = 1;
        // this will switch to the new player window in case the user previously looked at the nursery, deck, or discard piles
        if (tabnum == NURSERY_TAB || tabnum == DECK_TAB || tabnum == DISCARD_TAB) {
          tabnum = UNICORN_TAB;
        }
        PlaySound(TEXT("Assets\\Audio\\button-click.wav"), NULL, SND_FILENAME | SND_ASYNC);
      }
    }

    break;
  case WM_PAINT:
  {
    // TODO: DON'T use UpdateLayeredWindow() instead of WM_PAINT, but it's nice to know about though
    // https://stackoverflow.com/questions/67689087/correct-way-to-handle-and-redraw-a-layered-window-with-a-bitmap

    // validate that HWND
    hdc = BeginPaint(hWnd, &ps);

    // create memory DC and memory bitmap where we shall do our drawing
    hdcMem = CreateCompatibleDC(hdc);
    // create a deep copy so that the original image doesn't get updated when bitblt'ing the extra info to the memory bitmap
    hBitmapBuffer = (HBITMAP)CopyImage(hBitmapBG[menuState], IMAGE_BITMAP, BWIDTH, BHEIGHT, LR_COPYRETURNORG);
    oldBitmap = SelectObject(hdcMem, hBitmapBuffer);

    if (stateMach[menuState].StatePaint != NULL) {
      stateMach[menuState].StatePaint(&hdcMem);
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
    for (int i = 0; i < sizeof hBitmapBG / sizeof hBitmapBG[0]; i++) {
      DeleteObject(hBitmapBG[i]);
    }
    for (int i = 0; i < sizeof hBitmapBorder / sizeof hBitmapBorder[0]; i++) {
      DeleteObject(hBitmapBorder[i]);
    }
    for (int i = 0; i < sizeof icons / sizeof icons[0]; i++) {
      DeleteObject(icons[i].bitmap);
    }
    for (int i = 0; i < sizeof playerNums / sizeof playerNums[0]; i++) {
      DeleteObject(playerNums[i].bitmap);
    }
    for (int i = 0; i < sizeof stableNums / sizeof stableNums[0]; i++) {
      DeleteObject(stableNums[i].bitmap);
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
    DeleteObject(hBitmapBack);
    DestroyFonts();
    PostQuitMessage(0);
    if (mutex != NULL)
      CloseHandle(mutex);
    WSACleanup();
    break;
  }
  }

  return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

static LRESULT CALLBACK WndProcHost(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  // char tmpuser[LINE_MAX * 2] = { 0 };
  char portstr[LINE_MAX] = { 0 };
  int count = 0, portno;
  PAINTSTRUCT ps;
  HDC hdc;
  RECT rc;

  switch (uMsg)
  {
  case WM_CREATE:
    isChildOpen = TRUE;

    textNameHwnd = CreateWindow(TEXT("EDIT"), NULL, WS_BORDER | WS_CHILD | WS_VISIBLE, 10, 50, 200, 20, hWnd, (HMENU)ID_TEXT, g_hInstance, NULL);
    portHwnd = CreateWindow(TEXT("EDIT"), NULL, WS_BORDER | WS_CHILD | WS_VISIBLE, 10, 145, 200, 20, hWnd, (HMENU)ID_TEXT, g_hInstance, NULL);
    CreateWindow(TEXT("BUTTON"), TEXT("Submit"), WS_CHILD | WS_VISIBLE, 150, 200, 100, 30, hWnd, (HMENU)ID_BUTTON, g_hInstance, NULL);

    break;
  case WM_COMMAND:
  {
    switch (LOWORD(wParam))
    {
    case ID_BUTTON:
      GetWindowText(textNameHwnd, player[0].username, LINE_MAX - 1); //!< -1 to account for the null variable :)
      GetWindowText(portHwnd, portstr, LINE_MAX);

#ifdef _DEBUG
      if (player[0].username[0] == '\0') strcpy_s(player[0].username, 5, "host");
      if (portstr[0] == '\0') strcpy_s(portstr, 5, "1234");
      portno = atoi(portstr);
#endif

      // input check for username and port #
      if (player[0].username[0] != '\0' && portno >= 1024 && portno <= 65535) {
        // close this window and switch to lobby window
        GetIPreq(ip);
        char localIPcode[16];
        GetLocalIP(localIPcode);
        sprintf_s(hexcode, sizeof(hexcode), "Wi-Fi Code: %08X, Local Code: %08X", IPtoHexCode(ip), IPtoHexCode(localIPcode));
        sprintf_s(partyMems, PARTYSTRSIZE, "1. %s (host)", player[0].username);
        
        mutex = CreateMutex(
          NULL,   //!< default security attributes
          FALSE,  //!< initially not owned
          NULL);  //!< unnamed mutex

        if (mutex == NULL)
        {
          MessageBoxA(NULL,
            _T("CreateMutex error before creating network thread :("),
            _T("Server Connection"),
            MB_ICONERROR | MB_OK);
          DestroyWindow(hWnd);
        }

        networkThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ServerHost, &portno, 0, &tIdweb);

        menuState = LOBBY;
        DestroyWindow(hWnd);
      }

      break;
    }
  }
  break;
  case WM_PAINT:
    hdc = BeginPaint(hWnd, &ps);
    SetBkMode(hdc, TRANSPARENT); //!< box surrounding text is transparent instead of white

    TextOut(hdc, 10, 25, TEXT("Enter desired username:"), strlen("Enter desired username:"));
    SetRect(&rc, 10, 100, 300, 140);
    // if nCount is -1, then the lpchText parameter is assumed to be a pointer to a null-terminated
    // string and DrawText computes the character count automatically
    DrawText(hdc, TEXT("Enter valid port number\n(between 1024 and 65535 inclusive):"), -1, &rc, DT_LEFT | DT_WORDBREAK);

    EndPaint(hWnd, &ps);
    ReleaseDC(hWnd, hdc);
    break;
  case WM_DESTROY:
    isChildOpen = FALSE;
    break;
  }

  return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

static LRESULT CALLBACK WndProcJoin(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  char portstr[LINE_MAX] = { 0 }, codestr[9] = { 0 };
  int count = 0, portno;
  PAINTSTRUCT ps;
  HDC hdc;
  RECT rc;

  switch (uMsg)
  {
  case WM_CREATE:
    isChildOpen = TRUE;

    textNameHwnd = CreateWindow(TEXT("EDIT"), NULL, WS_BORDER | WS_CHILD | WS_VISIBLE, 10, 40, 200, 20, hWnd, (HMENU)ID_TEXT, g_hInstance, NULL);
    portHwnd = CreateWindow(TEXT("EDIT"), NULL, WS_BORDER | WS_CHILD | WS_VISIBLE, 10, 115, 200, 20, hWnd, (HMENU)ID_TEXT, g_hInstance, NULL);
    codeHwnd = CreateWindow(TEXT("EDIT"), NULL, WS_BORDER | WS_CHILD | WS_VISIBLE, 10, 170, 200, 20, hWnd, (HMENU)ID_TEXT, g_hInstance, NULL);
    CreateWindow(TEXT("BUTTON"), TEXT("Submit"), WS_CHILD | WS_VISIBLE, 150, 210, 100, 30, hWnd, (HMENU)ID_BUTTON, g_hInstance, NULL);

    break;
  case WM_COMMAND:
  {
    switch (LOWORD(wParam))
    {
    case ID_BUTTON:
      GetWindowText(textNameHwnd, player[0].username, LINE_MAX - 1); //!< -1 to account for the null variable :)
      GetWindowText(portHwnd, portstr, LINE_MAX);
      GetWindowText(codeHwnd, codestr, 9);
      
#ifdef _DEBUG
      if (codestr[0] == '\0') strcpy_s(codestr, 9, "142B8F0B");
      if (portstr[0] == '\0') strcpy_s(portstr, 5, "1234");
      portno = atoi(portstr);
#endif

      // TODO: add support for inserting ip codestr's directly instead of just some "obfuscated" hex value
      // input check for username and port #
      if (player[0].username[0] != '\0' && portno >= 1024 && portno <= 65535 && strlen(codestr) == 8) {
        // local ip 127.0.0.1: 142B8F0B
        HexCodeToIP(codestr, ip);

        networkThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ClientJoin, &portno, 0, &tIdweb);

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
    isChildOpen = FALSE;
    break;
  }

  return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
