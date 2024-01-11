#include "windowsapp.h"
#include "server.h"
#include "client.h"
#include "gamemechanics.h"

#pragma comment(lib,"msimg32.lib") // Transparent blt

struct BMPHeader {
  short header; // hex value is 4D42 even though "BM" is 424D??
  int filesize;
  int rservd;
  int pixeloffset;

  // DIB Header
  int headersize;
  int width;
  int height;
  short planes;
  short bpp;
  int compression;
  int datasize;
  int hres;
  int vres;
  int colorused;
  int colorimportant;
  int rbmask;
  int gbmask;
  int bbmask;
  int abmask;
  int wcolorspace;
  int cspaceendpts[9];
  int rgamma;
  int ggamma;
  int bgamma;
  int filler[4];
} h;

// super global variables TM
HINSTANCE hInstanceGlob;
char ip[16], hexcode[43];
short menustate = 0;
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

// global variables for this source file only
HWND gamethread, networkthread, textNameHwnd, portHwnd, codeHwnd, scrollHwnd;
DWORD tId, tIdweb;
WNDCLASSEX wcexHost;
WNDCLASSEX wcexJoin;
HBITMAP hBitmapTitle[9], hBitmapBorder[8], hBitmapIcon[13], hBitmapCard[2];
BOOL windowOpen[2] = { FALSE };
BOOL childWindow[2] = { FALSE };
HFONT oldfont, chonkyfont;

int stablePadding = 20; // arbitrary number of pixels to pad between the displayed list of cards in their respective stables/hands/decks

enum BabySelection {BABYRED, BABYPINK, BABYORANGE, BABYYELLOW, BABYGREEN, BABYBLUE, BABYPURPLE, BABYBLACK, BABYWHITE, BABYBROWN, BABYRAINBOW, BABYDEATH, BABYNARWHAL};

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

#define BORDERWIDTH 97

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

// this is basically a copy of host/join, may delete later and implement the scrolling under a switch statement
void CreateCardWindow(HWND hwnd) {
  ZeroMemory(&wcexJoin, sizeof(WNDCLASSEX)); // TODO: figure out what this does
  wcexJoin.cbSize = sizeof(WNDCLASSEX);
  wcexJoin.cbClsExtra = 0;
  wcexJoin.cbWndExtra = 0;
  wcexJoin.style = CS_HREDRAW | CS_VREDRAW;
  wcexJoin.hInstance = hInstanceGlob;
  wcexJoin.hCursor = LoadCursor(NULL, IDC_ARROW);
  wcexJoin.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
  wcexJoin.lpszMenuName = NULL;
  wcexJoin.lpszClassName = _T("CardSt00f");
  wcexJoin.lpfnWndProc = ScrollingCardWndProc;
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

  // TODO: i tested WS_EX_LAYERED in the host window and it was completely transparent instead of semi-transparent :(
  HWND hWndCard = CreateWindowEx(
    WS_EX_LAYERED,
    wcexJoin.lpszClassName,
    (PTSTR) NULL,
    WS_HSCROLL | WS_VISIBLE,
    0, 497,
    1280, 222,
    hwnd, // parent/owner window; using the WS_CHILD flag w/ WS_OVERLAPPEDWINDOW or SetParent function makes the window bound to the dimensions of the game window
    NULL,
    hInstanceGlob,
    NULL
  );

  if (!hWndCard)
  {
    MessageBox(NULL,
      _T("Join window creation failed!"),
      _T("Windows Desktop Guided Tour"),
      NULL);
    return;
  }

  ShowWindow(hWndCard, SW_NORMAL);
}

// Description:
//   Creates a horizontal scroll bar along the bottom of the parent 
//   window's area.
// Parameters:
//   hwndParent - handle to the parent window.
//   sbHeight - height, in pixels, of the scroll bar.
// Returns:
//   The handle to the scroll bar.
HWND CreateScrollBar(HWND hwndParent, int sbHeight) {
  RECT rect;

  // Get the dimensions of the parent window's client area;
  if (!GetClientRect(hwndParent, &rect))
    return NULL;

  // Create the scroll bar.
  return (CreateWindowEx(
    0,                      // no extended styles 
    L"SCROLLBAR",           // scroll bar control class 
    (PTSTR)NULL,            // no window text 
    WS_CHILD | WS_VISIBLE   // window styles  
    | SBS_HORZ,             // horizontal scroll bar style 
    rect.left,              // horizontal position 
    rect.bottom - sbHeight, // vertical position 
    rect.right,             // width of the scroll bar 
    sbHeight,               // height of the scroll bar
    hwndParent,             // handle to main window 
    (HMENU)NULL,            // no menu 
    hInstanceGlob,          // instance owning this window 
    (PVOID)NULL             // pointer not needed 
  ));
}

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

  hBitmapTitle[TITLEHOST] = (HBITMAP)LoadImage(NULL, "Assets\\titlehost.bmp",
    IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
  if (hBitmapTitle[TITLEHOST] == NULL) {
    issuccess = FALSE;
    strcat_s(errors, sizeof errors, "titlehost.bmp ");
  }

  hBitmapTitle[TITLEJOIN] = (HBITMAP)LoadImage(NULL, "Assets\\titlejoin.bmp",
    IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
  if (hBitmapTitle[TITLEJOIN] == NULL) {
    issuccess = FALSE;
    strcat_s(errors, sizeof errors, "titlejoin.bmp ");
  }

  hBitmapTitle[TITLERULES] = (HBITMAP)LoadImage(NULL, "Assets\\titlerules.bmp",
    IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
  if (hBitmapTitle[TITLERULES] == NULL) {
    issuccess = FALSE;
    strcat_s(errors, sizeof errors, "titlerules.bmp ");
  }

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

  // these three should ideally all fall under the same structure because they
  // have the same default size
  hBitmapIcon[0] = (HBITMAP)LoadImage(NULL, "Assets\\icon_narwhal.bmp",
    IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
  if (hBitmapIcon[0] == NULL) {
    issuccess = FALSE;
    strcat_s(errors, sizeof errors, "icon_narwhal.bmp ");
  }
  hBitmapIcon[1] = (HBITMAP)LoadImage(NULL, "Assets\\player1.bmp",
    IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
  if (hBitmapIcon[1] == NULL) {
    issuccess = FALSE;
    strcat_s(errors, sizeof errors, "player1.bmp ");
  }
  hBitmapIcon[2] = (HBITMAP)LoadImage(NULL, "Assets\\stable1.bmp",
    IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
  if (hBitmapIcon[2] == NULL) {
    issuccess = FALSE;
    strcat_s(errors, sizeof errors, "stable1.bmp ");
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

volatile DWORD whatever;

void GUIstuff(HWND hWnd) {
  POINT pnt;
  int ret;
  DWORD waitResult;

  // gets mouse click for baby unicorn picker
  GetCursorPos(&pnt);
  ScreenToClient(hWnd, &pnt);

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
          closesocket(udpfd);
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
      if (ret) {
        networktoggle ^= 2;
      }
    }
  }
}

void InitDebugMode(HWND hWnd) {
  // add some arbitrary number of cards to the player's stable for testing purposes
  // TODO: if I want to quickly edit future tests, then I can read text files for easier input and testing

  for (int i = 0; i < 4; i++) {
    player[0].stable.unicorns[i] = (i & 1) ? 128 : 129; 
    player[0].stable.num_unicorns++;
    player[0].stable.size++;
  }
}

void ResetDebugMode() {
  // reset player's cards/stable/etc. to 0;
  // TODO: this should be implemented as a hard reset when you join the game, but for now this will close off the init function
  memset(player[0].stable.unicorns, 0, sizeof player[0].stable.unicorns);
  player[0].stable.num_unicorns = 0;
  player[0].stable.size = 0;
}

// this is basically a copy of WS_PAINT; will either delete later or repurpose this function to only focus on the background bitmap rendering
void CardWindow(HWND hWnd) {
  // create the visual bitmap for the card display at the bottom of the game screen

  HDC hdc;
  HDC hdcMem;
  HBITMAP hBitmapBuffer;
  HGDIOBJ oldBitmap;

  HDC hdcSprite;
  HBITMAP hbmSprite;
  HGDIOBJ oldSprite;

  PAINTSTRUCT ps;
  POINT pnt;
  RECT rc;

  const int pntpi = 72; // points-per-inch
  int pxpi, points, pxheight;


  // validate that HWND
  hdc = BeginPaint(hWnd, &ps);

  // for clicking purposes
  GetCursorPos(&pnt);
  ScreenToClient(hWnd, &pnt);

  // get window's client rectangle. We need this for bitmap creation.
  GetClientRect(hWnd, &rc);

  // create memory DC and memory bitmap where we shall do our drawing
  hdcMem = CreateCompatibleDC(hdc);
  // create a deep copy so that the original image doesn't get updated when bitblt'ing the extra info to the memory bitmap
  hBitmapBuffer = (HBITMAP)CopyImage(hBitmapTitle[menustate], IMAGE_BITMAP, BWIDTH, BHEIGHT, LR_COPYRETURNORG);
  oldBitmap = SelectObject(hdcMem, hBitmapBuffer);

  hdcSprite = CreateCompatibleDC(hdc);

  // start is the starting point for the display, of course, with the scroll bar it could go to 0
  POINT start = { 60, 507 };
  BITMAP bm;
  GetObject(hBitmapCard[0], (int)sizeof bm, &bm); // fetches width/height; all cards have the same w/h

  int distance = stablePadding + bm.bmWidth;

  for (int i = 0; i < player[0].stable.size; i++) {
    // for now, this cycles through hBitmapCard which only has the super neigh card and the back design
    // in the future, hBitmapCard could be replaced/assimilated by an updated Unicorn or Deck structure featuring the file name and the loaded hBitmap
    oldSprite = SelectObject(hdcSprite, hBitmapCard[i % 2]);
    TransparentBlt(hdcMem, start.x + (distance * i), start.y, bm.bmWidth, bm.bmHeight, hdcSprite, 0, 0, bm.bmWidth, bm.bmHeight, RGB(0, 255, 0));
    SelectObject(hdcSprite, oldSprite);
  }
  DeleteDC(hdcSprite);


  // render memory buffer to on-screen DC
  BitBlt(hdc, 0, 0, BWIDTH, BHEIGHT, hdcMem, 0, 0, SRCCOPY);

  // cleanup, be aware of memory leaks >.<
  SelectObject(hdcMem, oldBitmap);
  DeleteDC(hdcMem);
  DeleteObject(hBitmapBuffer);
  EndPaint(hWnd, &ps);
}

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

  switch (message)
  {
  case WM_CREATE:
    LoadImages(hWnd);
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
  case WM_LBUTTONDOWN:
    if (menustate == LOBBY) {
      GUIstuff(hWnd);
    }
    break;
  case WM_PAINT:
    // TODO: use UpdateLayeredWindow() instead of WM_PAINT????
    // https://stackoverflow.com/questions/67689087/correct-way-to-handle-and-redraw-a-layered-window-with-a-bitmap

    // validate that HWND
    hdc = BeginPaint(hWnd, &ps);

    // for clicking purposes
    GetCursorPos(&pnt);
    ScreenToClient(hWnd, &pnt);

    // TODO: get rid of these magic numbers and add new constants or pre-defined variables to globals.c
    // this is sickening :/
    switch (menustate) {
    case TITLEBLANK:
    case TITLEHOST:
    case TITLEJOIN:
    case TITLERULES:
    {
      if (pnt.x >= 528 && pnt.x <= 757) {
        if (pnt.y >= 385 && pnt.y < 472) {
          // Host Button
          menustate = TITLEHOST;
          if (GetAsyncKeyState(VK_LBUTTON) < 0 && is_active && windowOpen[1] == FALSE) {
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
        }
        else if (pnt.y >= 472 && pnt.y < 572) {
          // Join Button
          menustate = TITLEJOIN;
          if (GetAsyncKeyState(VK_LBUTTON) < 0 && is_active && windowOpen[1] == FALSE) {
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
        }
        else if (pnt.y >= 572 && pnt.y <= 659) {
          // Rules Button
          menustate = TITLERULES;
          // use Async instead of GetKeyState because this loop doesn't process messages ig
          // https://stackoverflow.com/questions/59923765/getkeystate-function-not-working-when-checking-if-left-mouse-button-is-clicked
          if (GetAsyncKeyState(VK_LBUTTON) < 0 && is_active) {
            menustate = RULESONE;
          }
        }
        else {
          menustate = TITLEBLANK;
        }
      }
      else {
        menustate = TITLEBLANK;
      }

      // TOOD: write a proper ifdef
      if (pnt.x >= 0 && pnt.x <= 50 && pnt.y >= 0 && pnt.y <= 50) {
        // use Async instead of GetKeyState because this loop doesn't process messages ig
        // https://stackoverflow.com/questions/59923765/getkeystate-function-not-working-when-checking-if-left-mouse-button-is-clicked
        if (GetAsyncKeyState(VK_LBUTTON) < 0 && is_active) {
          menustate = DEBUGMODE;
          scrollHwnd = CreateScrollBar(hWnd, 15);
          ShowWindow(scrollHwnd, SW_NORMAL);
          InitDebugMode(scrollHwnd); // for card testing purposes
        }
      }
      break;
    }
    case RULESONE:
    {
      if (pnt.y >= 619 && pnt.y <= 650 && GetAsyncKeyState(VK_LBUTTON) < 0 && is_active) {
        if (pnt.x >= 975 && pnt.x <= 1074) {
          menustate = TITLEBLANK;
        }
        else if (pnt.x >= 1129 && pnt.x <= 1160) {
          menustate = RULESTWO;
        }
      }
      break;
    }
    case RULESTWO:
    {
      if (pnt.y >= 619 && pnt.y <= 650 && GetAsyncKeyState(VK_LBUTTON) < 0 && is_active) {
        if (pnt.x >= 975 && pnt.x <= 1074) {
          menustate = TITLEBLANK;
        }
        else if (pnt.x >= 1087 && pnt.x <= 1118) {
          menustate = RULESONE;
        }
      }
      break;
    }
    case LOBBY:
      //if (pnt.x >= 120 && pnt.x <= 309 && pnt.y >= 590 && pnt.y <= 639 && GetAsyncKeyState(VK_LBUTTON) < 0 && is_active) {
      //  // start game
      //}
      break;
    case DEBUGMODE:
      // TOOD: write a proper ifdef
      if (pnt.x >= 0 && pnt.x <= 50 && pnt.y >= 0 && pnt.y <= 50) {
        // use Async instead of GetKeyState because this loop doesn't process messages ig
        // https://stackoverflow.com/questions/59923765/getkeystate-function-not-working-when-checking-if-left-mouse-button-is-clicked
        if (GetAsyncKeyState(VK_LBUTTON) < 0 && is_active) {
          menustate = TITLEBLANK;
          DestroyWindow(scrollHwnd);
          ResetDebugMode(); // for card testing purposes
        }
      }
    default:
      break;
    }

    // get window's client rectangle. We need this for bitmap creation.
    GetClientRect(hWnd, &rc);

    // create memory DC and memory bitmap where we shall do our drawing
    hdcMem = CreateCompatibleDC(hdc);
    // create a deep copy so that the original image doesn't get updated when bitblt'ing the extra info to the memory bitmap
    hBitmapBuffer = (HBITMAP)CopyImage(hBitmapTitle[menustate], IMAGE_BITMAP, BWIDTH, BHEIGHT, LR_COPYRETURNORG);
    oldBitmap = SelectObject(hdcMem, hBitmapBuffer);

    // TODO cleanup: decide whether or not this should be moved to inside of the switch statement while
    // the lines "GetClientRect(hWnd, &rc)" -> "oldBitmap =" are moved before the switch. there would be
    // a slight delay because the buffer would only update on the next cycle
    if (menustate == LOBBY) {
      // initialize custom font
      // create larger font because default is teeny tiny
      pxpi = GetDeviceCaps(hdcMem, LOGPIXELSY); // pixels-per-inch
      points = 30;
      pxheight = -(points * pxpi / pntpi);
      oldfont = GetCurrentObject(hdcMem, OBJ_FONT); // save old font

      chonkyfont = CreateFontA(pxheight, 0, // size
        0, 0,               // normal orientation
        FW_BOLD,            // normal weight--e.g., bold would be FW_BOLD
        NULL, NULL, NULL,   // not italic, underlined or strike out
        DEFAULT_CHARSET,
        OUT_OUTLINE_PRECIS, // select only outline (not bitmap) fonts
        CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY,
        VARIABLE_PITCH | FF_SWISS,
        "Arial");

      SetBkMode(hdcMem, TRANSPARENT); // box surrounding text is transparent instead of white

      // type out lobby code
      SetRect(&rc, 78, 180, 1202, 225);
      SetTextColor(hdcMem, RGB(255, 255, 255));
      SelectObject(hdcMem, chonkyfont);
      DrawText(hdcMem, hexcode, strlen(hexcode), &rc, DT_CENTER);

      // type out party members
      SetRect(&rc, 115, 260, 800, 625);
      SelectObject(hdcMem, oldfont);
      DeleteObject(chonkyfont);
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

    if (menustate == DEBUGMODE) {
      // draw player Narwhal!!
      hdcSprite = CreateCompatibleDC(hdc);
      // TODO: this one actually has to be based off of current players, so it is capped at 1 for now
      for (int i = 0; i < 1; i++) {
        // TODO: replace magic numbers later :) this for loop will get deleted once variables are better organized in the next 1 or 2 updates!
        // 95 x 81 = width x height; 1064 is the left coordinate on the plane and 28 is the top coordinate
        for (int j = 0; j < 3; j++) {
          oldSprite = SelectObject(hdcSprite, hBitmapIcon[j]);
          TransparentBlt(hdcMem, 1064, 28, 95, 81, hdcSprite, 0, 0, 95, 81, RGB(0, 255, 0));
          SelectObject(hdcSprite, oldSprite);
        }
      }


      // start is the starting point for the display, of course, with the scroll bar it could go to 0
      POINT start = { 60, 507 };
      BITMAP bm;
      GetObject(hBitmapCard[0], (int)sizeof bm, &bm); // fetches width/height; all cards have the same w/h

      int distance = stablePadding + bm.bmWidth;

      for (int i = 0; i < player[0].stable.size; i++) {
        // for now, this cycles through hBitmapCard which only has the super neigh card and the back design
        // in the future, hBitmapCard could be replaced/assimilated by an updated Unicorn or Deck structure featuring the file name and the loaded hBitmap
        oldSprite = SelectObject(hdcSprite, hBitmapCard[i % 2]);
        TransparentBlt(hdcMem, start.x + (distance * i), start.y, bm.bmWidth, bm.bmHeight, hdcSprite, 0, 0, bm.bmWidth, bm.bmHeight, RGB(0, 255, 0));
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
  case WM_DESTROY:
    windowOpen[0] = FALSE;
    for (int i = 0; i < sizeof hBitmapTitle / sizeof hBitmapTitle[0]; i++) {
      DeleteObject(hBitmapTitle[i]);
    }
    for (int i = 0; i < sizeof hBitmapBorder / sizeof hBitmapBorder[0]; i++) {
      DeleteObject(hBitmapBorder[i]);
    }
    for (int i = 0; i < sizeof hBitmapIcon / sizeof hBitmapIcon[0]; i++) {
      DeleteObject(hBitmapIcon[i]);
    }
    for (int i = 0; i < sizeof hBitmapCard / sizeof hBitmapCard[0]; i++) {
      DeleteObject(hBitmapCard[i]);
    }
    PostQuitMessage(0);
    CloseHandle(mutex);
    WSACleanup();
    break;
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
      // TODO: (bug) entering an "empty" name erroneously counts as a valid username
      if (player[0].username == '\0') strcpy_s(player[0].username, 5, "host");
      if (portstr[0] == '\0') strcpy_s(portstr, 5, "1234");
      portno = atoi(portstr);

      // input check for username and port #
      if (player[0].username != NULL && portno >= 1024 && portno <= 65535) {
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
      // TODO: (bug) entering an "empty" name erroneously counts as a valid username
      if (player[0].username != NULL && portno >= 1024 && portno <= 65535 && strlen(codestr) == 8) {
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

// glorious microsoft
// https://learn.microsoft.com/en-us/windows/win32/controls/scroll-a-bitmap-in-scroll-bars
LRESULT CALLBACK ScrollingCardWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  HDC hdc;
  PAINTSTRUCT ps;
  SCROLLINFO si;

  // These variables are required by BitBlt. 
  static HDC hdcWin;           // window DC 
  static HDC hdcScreen;        // DC for entire screen 
  static HDC hdcScreenCompat;  // memory DC for screen 
  static HBITMAP hbmpCompat;   // bitmap handle to old DC 
  static BITMAP bmp;           // bitmap data structure 
  static BOOL fBlt;            // TRUE if BitBlt occurred 
  static BOOL fScroll;         // TRUE if scrolling occurred 

  // These variables are required for horizontal scrolling. 
  static int xMinScroll;       // minimum horizontal scroll value 
  static int xCurrentScroll;   // current horizontal scroll value 
  static int xMaxScroll;       // maximum horizontal scroll value 

  switch (message)
  {
  case WM_CREATE:
  {
    // Create a normal DC and a memory DC for the entire 
    // screen. The normal DC provides a snapshot of the 
    // screen contents. The memory DC keeps a copy of this 
    // snapshot in the associated bitmap. 
    hdcScreen = CreateDC(L"DISPLAY", (PCTSTR)NULL,
      (PCTSTR)NULL, (CONST DEVMODE*) NULL);
    hdcScreenCompat = CreateCompatibleDC(hdcScreen);

    // Retrieve the metrics for the bitmap associated with the 
    // regular device context. 
    bmp.bmBitsPixel = (BYTE)GetDeviceCaps(hdcScreen, BITSPIXEL);
    bmp.bmPlanes = (BYTE)GetDeviceCaps(hdcScreen, PLANES);
    bmp.bmWidth = GetDeviceCaps(hdcScreen, HORZRES);
    bmp.bmHeight = GetDeviceCaps(hdcScreen, VERTRES);

    // The width must be byte-aligned. 
    bmp.bmWidthBytes = ((bmp.bmWidth + 15) & ~15) / 8;

    // Create a bitmap for the compatible DC. 
    hbmpCompat = CreateBitmap(bmp.bmWidth, bmp.bmHeight,
      bmp.bmPlanes, bmp.bmBitsPixel, (CONST VOID*) NULL);

    // Select the bitmap for the compatible DC. 
    SelectObject(hdcScreenCompat, hbmpCompat);

    // Initialize the flags. 
    fBlt = FALSE;
    fScroll = FALSE;

    // Initialize the horizontal scrolling variables. 
    xMinScroll = 0;
    xCurrentScroll = 0;
    xMaxScroll = 0;
    break;
  }
  case WM_PAINT:
  {
    PRECT prect;

    hdc = BeginPaint(hwnd, &ps);

    // If scrolling has occurred, use the following call to 
    // BitBlt to paint the invalid rectangle. 
    // 
    // The coordinates of this rectangle are specified in the 
    // RECT structure to which prect points. 
    // 
    // Note that it is necessary to increment the seventh 
    // argument (prect->left) by xCurrentScroll and the 
    // eighth argument (prect->top) by yCurrentScroll in 
    // order to map the correct pixels from the source bitmap. 
    if (fScroll)
    {
      prect = &ps.rcPaint;

      BitBlt(ps.hdc,
        prect->left, prect->top,
        (prect->right - prect->left),
        (prect->bottom - prect->top),
        hdcScreenCompat,
        prect->left + xCurrentScroll,
        prect->top,
        SRCCOPY);

      fScroll = FALSE;
    }

    EndPaint(hwnd, &ps);
    break;
  }
  case WM_HSCROLL:
  {
    int xDelta;     // xDelta = new_pos - current_pos  
    int xNewPos;    // new position 
    int yDelta = 0;

    switch (LOWORD(wParam)) {
    // User clicked the scroll bar shaft left of the scroll box. 
    case SB_PAGEUP:
      xNewPos = xCurrentScroll - 50;
      break;

    // User clicked the scroll bar shaft right of the scroll box. 
    case SB_PAGEDOWN:
      xNewPos = xCurrentScroll + 50;
      break;

    // User clicked the left arrow. 
    case SB_LINEUP:
      xNewPos = xCurrentScroll - 5;
      break;

    // User clicked the right arrow. 
    case SB_LINEDOWN:
      xNewPos = xCurrentScroll + 5;
      break;

    // User dragged the scroll box. 
    case SB_THUMBPOSITION:
      xNewPos = HIWORD(wParam);
      break;

    default:
      xNewPos = xCurrentScroll;
    }

    // New position must be between 0 and the screen width. 
    xNewPos = max(0, xNewPos);
    xNewPos = min(xMaxScroll, xNewPos);

    // If the current position does not change, do not scroll.
    if (xNewPos == xCurrentScroll)
      break;

    // Set the scroll flag to TRUE. 
    fScroll = TRUE;

    // Determine the amount scrolled (in pixels). 
    xDelta = xNewPos - xCurrentScroll;

    // Reset the current scroll position. 
    xCurrentScroll = xNewPos;

    // Scroll the window. (The system repaints most of the 
    // client area when ScrollWindowEx is called; however, it is 
    // necessary to call UpdateWindow in order to repaint the 
    // rectangle of pixels that were invalidated.) 
    ScrollWindowEx(hwnd, -xDelta, -yDelta, (CONST RECT*) NULL,
      (CONST RECT*) NULL, (HRGN)NULL, (PRECT)NULL,
      SW_INVALIDATE);
    UpdateWindow(hwnd);

    // Reset the scroll bar. 
    si.cbSize = sizeof(si);
    si.fMask = SIF_POS;
    si.nPos = xCurrentScroll;
    SetScrollInfo(hwnd, SB_HORZ, &si, TRUE);
    break;
  }

  case WM_RBUTTONDOWN:
  {
    // Get the compatible DC of the client area. 
    hdcWin = GetDC(hwnd);

    // Fill the client area to remove any existing contents. 
    RECT rect;
    GetClientRect(hwnd, &rect);
    FillRect(hdcWin, &rect, (HBRUSH)(COLOR_WINDOW + 1));

    // Copy the contents of the current screen 
    // into the compatible DC. 
    BitBlt(hdcScreenCompat, 0, 0, bmp.bmWidth,
      bmp.bmHeight, hdcScreen, 0, 0, SRCCOPY);

    // Copy the compatible DC to the client area.
    BitBlt(hdcWin, 0, 0, bmp.bmWidth, bmp.bmHeight,
      hdcScreenCompat, 0, 0, SRCCOPY);

    ReleaseDC(hwnd, hdcWin);
    fBlt = TRUE;
    break;
  }
  case WM_DESTROY:
    PostQuitMessage(0);
    return 0;
  }
  return DefWindowProc(hwnd, message, wParam, lParam);
}
