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
char ip[16], hexcode[21] = "Lobby Code: ";
short menustate = 0;
char partymems[PARTYSTRSIZE] = { 0 };
HWND webhwnd;
// toggle for if window is currently active; 0 = no, 1 = yes :)
unsigned char is_active = 1;
RECT pselect[MAX_PLAYERS];
// toggle to see whether or not a baby unicorn is taken during lobby selection; 0 = free, 1 = taken
unsigned char babytoggle[13] = { 0 };

// global variables for this source file only
HWND gamethread, networkthread, textNameHwnd, portHwnd, codeHwnd;
DWORD tId, tIdweb;
WNDCLASSEX wcexHost;
WNDCLASSEX wcexJoin;
HBITMAP hBitmapTitle[7], hBitmapBorder[8];
int windowOpen[2] = { 0 };
unsigned char isButton = 0;
unsigned char childWindow[2] = { 0 };
HFONT oldfont, chonkyfont;

// left, top, right, bottom
const RECT BABYRED      = {  659,  243,  748,  332 };
const RECT BABYPINK     = {  779,  243,  868,  332 };
const RECT BABYORANGE   = {  899,  243,  988,  332 };
const RECT BABYYELLOW   = { 1019,  243, 1108,  332 };

const RECT BABYGREEN    = {  599,  358,  688,  447 };
const RECT BABYBLUE     = {  719,  358,  808,  447 };
const RECT BABYPURPLE   = {  839,  358,  928,  447 };
const RECT BABYBLACK    = {  959,  358, 1048,  447 };
const RECT BABYWHITE    = { 1079,  358, 1168,  447 };

const RECT BABYBROWN    = {  659,  473,  748,  652 };
const RECT BABYRAINBOW  = {  779,  473,  868,  652 };
const RECT BABYDEATH    = {  899,  473,  988,  652 };
const RECT BABYNARWHAL  = { 1019,  473, 1108,  652 };

#define BORDERWIDTH 97

enum { ID_BUTTON, ID_TEXT };

// Forward declarations of functions included in this code module:
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK WndProcHost(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK WndProcJoin(HWND, UINT, WPARAM, LPARAM);

void CreateHostWindow(HWND hwnd) {
  if (childWindow[0] == 0) {
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
    wcexHost.hIcon = NULL;      // LoadIcon(wcexHost.hInstance, IDI_APPLICATION);
    wcexHost.hIconSm = NULL;    // LoadIcon(wcexHost.hInstance, IDI_APPLICATION);

    if (!RegisterClassEx(&wcexHost))
    {
      MessageBox(NULL,
        _T("Call to RegisterClassEx failed!"),
        _T("Windows Desktop Guided Tour"),
        NULL);
      return;
    }
    else {
      childWindow[0] = 1;
    }
  }

  HWND hWndHost = CreateWindowEx(
    WS_EX_OVERLAPPEDWINDOW,
    wcexHost.lpszClassName,
    "Hostie Toastie",
    WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT, CW_USEDEFAULT,
    400, 300,
    NULL, // if this is the parent window then i can get rid of SetParent maybe
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
  SetParent(hWndHost, hwnd);
}

void CreateJoinWindow(HWND hwnd) {
  if (childWindow[1] == 0) {
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
    wcexJoin.hIcon = NULL;      // LoadIcon(wcexJoin.hInstance, IDI_APPLICATION);
    wcexJoin.hIconSm = NULL;    // LoadIcon(wcexJoin.hInstance, IDI_APPLICATION);

    if (!RegisterClassEx(&wcexJoin))
    {
      MessageBox(NULL,
        _T("Call to RegisterClassEx failed!"),
        _T("Windows Desktop Guided Tour"),
        NULL);
      return;
    }
    else {
      childWindow[1] = 1;
    }
  }

  HWND hWndJoin = CreateWindowEx(
    WS_EX_OVERLAPPEDWINDOW,
    wcexJoin.lpszClassName,
    "Join me, Jimmy",
    WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT, CW_USEDEFAULT,
    400, 300,
    NULL, // if this is the parent window then i can get rid of SetParent maybe
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
  SetParent(hWndJoin, hwnd);
}

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
          if (GetAsyncKeyState(VK_LBUTTON) < 0 && is_active && windowOpen[1] == 0) {
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
          if (GetAsyncKeyState(VK_LBUTTON) < 0 && is_active && windowOpen[1] == 0) {
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
  if (pnt.y >= BABYRED.top && pnt.y <= BABYRED.bottom) {
    if (pnt.x >= BABYRED.left && pnt.x <= BABYRED.right && babytoggle[0] == 0) {
      babytoggle[0] = 1;
      // checks if the player even selected a unicorn just to make sure the index isn't either
      // garbage info or mistakenly zero when someone else may have zero
      if (pselect[pnum].left != 0) {
        // resets babytoggle at old position (since baby toggle goes from 0-12 just like the
        // baby unicorn card numbers, player[num].stable.unicorns[0] can be used as an index)
        babytoggle[player[pnum].stable.unicorns[0]] = 0;
      }
      player[pnum].stable.unicorns[0] = 0;
      pselect[pnum].left = BABYRED.left - 7;
      pselect[pnum].top = BABYRED.top;
      return 1;
    }
    else if (pnt.x >= BABYPINK.left && pnt.x <= BABYPINK.right && babytoggle[1] == 0) {
      babytoggle[1] = 1;
      if (pselect[pnum].left != 0) {
        babytoggle[player[pnum].stable.unicorns[0]] = 0;
      }
      player[pnum].stable.unicorns[0] = 1;
      pselect[pnum].left = BABYPINK.left - 7;
      pselect[pnum].top = BABYPINK.top;
      return 1;
    }
    else if (pnt.x >= BABYORANGE.left && pnt.x <= BABYORANGE.right && babytoggle[2] == 0) {
      babytoggle[2] = 1;
      if (pselect[pnum].left != 0) {
        babytoggle[player[pnum].stable.unicorns[0]] = 0;
      }
      player[pnum].stable.unicorns[0] = 2;
      pselect[pnum].left = BABYORANGE.left - 7;
      pselect[pnum].top = BABYORANGE.top;
      return 1;
    }
    else if (pnt.x >= BABYYELLOW.left && pnt.x <= BABYYELLOW.right && babytoggle[3] == 0) {
      babytoggle[3] = 1;
      if (pselect[pnum].left != 0) {
        babytoggle[player[pnum].stable.unicorns[0]] = 0;
      }
      player[pnum].stable.unicorns[0] = 3;
      pselect[pnum].left = BABYYELLOW.left - 7;
      pselect[pnum].top = BABYYELLOW.top;
      return 1;
    }
  }
  else if (pnt.y >= BABYGREEN.top && pnt.y <= BABYGREEN.bottom) {
    if (pnt.x >= BABYGREEN.left && pnt.x <= BABYGREEN.right && babytoggle[4] == 0) {
      babytoggle[4] = 1;
      if (pselect[pnum].left != 0) {
        babytoggle[player[pnum].stable.unicorns[0]] = 0;
      }
      player[pnum].stable.unicorns[0] = 4;
      pselect[pnum].left = BABYGREEN.left - 7;
      pselect[pnum].top = BABYGREEN.top;
      return 1;
    }
    else if (pnt.x >= BABYBLUE.left && pnt.x <= BABYBLUE.right && babytoggle[5] == 0) {
      babytoggle[5] = 1;
      if (pselect[pnum].left != 0) {
        babytoggle[player[pnum].stable.unicorns[0]] = 0;
      }
      player[pnum].stable.unicorns[0] = 5;
      pselect[pnum].left = BABYBLUE.left - 7;
      pselect[pnum].top = BABYBLUE.top;
      return 1;
    }
    else if (pnt.x >= BABYPURPLE.left && pnt.x <= BABYPURPLE.right && babytoggle[6] == 0) {
      babytoggle[6] = 1;
      if (pselect[pnum].left != 0) {
        babytoggle[player[pnum].stable.unicorns[0]] = 0;
      }
      player[pnum].stable.unicorns[0] = 6;
      pselect[pnum].left = BABYPURPLE.left - 7;
      pselect[pnum].top = BABYPURPLE.top;
      return 1;
    }
    else if (pnt.x >= BABYBLACK.left && pnt.x <= BABYBLACK.right && babytoggle[7] == 0) {
      babytoggle[7] = 1;
      if (pselect[pnum].left != 0) {
        babytoggle[player[pnum].stable.unicorns[0]] = 0;
      }
      player[pnum].stable.unicorns[0] = 7;
      pselect[pnum].left = BABYBLACK.left - 7;
      pselect[pnum].top = BABYBLACK.top;
      return 1;
    }
    else if (pnt.x >= BABYWHITE.left && pnt.x <= BABYWHITE.right && babytoggle[8] == 0) {
      babytoggle[8] = 1;
      if (pselect[pnum].left != 0) {
        babytoggle[player[pnum].stable.unicorns[0]] = 0;
      }
      player[pnum].stable.unicorns[0] = 8;
      pselect[pnum].left = BABYWHITE.left - 7;
      pselect[pnum].top = BABYWHITE.top;
      return 1;
    }
  }
  else if (pnt.y >= BABYBROWN.top && pnt.y <= BABYBROWN.bottom) {
    if (pnt.x >= BABYBROWN.left && pnt.x <= BABYBROWN.right && babytoggle[9] == 0) {
      babytoggle[9] = 1;
      if (pselect[pnum].left != 0) {
        babytoggle[player[pnum].stable.unicorns[0]] = 0;
      }
      player[pnum].stable.unicorns[0] = 9;
      pselect[pnum].left = BABYBROWN.left - 7;
      pselect[pnum].top = BABYBROWN.top;
      return 1;
    }
    else if (pnt.x >= BABYRAINBOW.left && pnt.x <= BABYRAINBOW.right && babytoggle[10] == 0) {
      babytoggle[10] = 1;
      if (pselect[pnum].left != 0) {
        babytoggle[player[pnum].stable.unicorns[0]] = 0;
      }
      player[pnum].stable.unicorns[0] = 10;
      pselect[pnum].left = BABYRAINBOW.left - 7;
      pselect[pnum].top = BABYRAINBOW.top;
      return 1;
    }
    else if (pnt.x >= BABYDEATH.left && pnt.x <= BABYDEATH.right && babytoggle[11] == 0) {
      babytoggle[11] = 1;
      if (pselect[pnum].left != 0) {
        babytoggle[player[pnum].stable.unicorns[0]] = 0;
      }
      player[pnum].stable.unicorns[0] = 11;
      pselect[pnum].left = BABYDEATH.left - 7;
      pselect[pnum].top = BABYDEATH.top;
      return 1;
    }
    else if (pnt.x >= BABYNARWHAL.left && pnt.x <= BABYNARWHAL.right && babytoggle[12] == 0) {
      babytoggle[12] = 1;
      if (pselect[pnum].left != 0) {
        babytoggle[player[pnum].stable.unicorns[0]] = 0;
      }
      player[pnum].stable.unicorns[0] = 12;
      pselect[pnum].left = BABYNARWHAL.left - 7;
      pselect[pnum].top = BABYNARWHAL.top;
      return 1;
    }
  }

  return 0;
}

void LoadImages(HWND hWnd) {

  hBitmapTitle[TITLEBLANK] = (HBITMAP)LoadImage(NULL, "Assets\\titleblank.bmp",
    IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
  if (hBitmapTitle[TITLEBLANK] == NULL) {
    MessageBox(hWnd, "Failed to load image", "Error", MB_OK);
  }

  hBitmapTitle[TITLEHOST] = (HBITMAP)LoadImage(NULL, "Assets\\titlehost.bmp",
    IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
  if (hBitmapTitle[TITLEHOST] == NULL) {
    MessageBox(hWnd, "Failed to load image", "Error", MB_OK);
  }

  hBitmapTitle[TITLEJOIN] = (HBITMAP)LoadImage(NULL, "Assets\\titlejoin.bmp",
    IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
  if (hBitmapTitle[TITLEJOIN] == NULL) {
    MessageBox(hWnd, "Failed to load image", "Error", MB_OK);
  }

  hBitmapTitle[TITLERULES] = (HBITMAP)LoadImage(NULL, "Assets\\titlerules.bmp",
    IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
  if (hBitmapTitle[TITLERULES] == NULL) {
    MessageBox(hWnd, "Failed to load image", "Error", MB_OK);
  }

  hBitmapTitle[RULESONE] = (HBITMAP)LoadImage(NULL, "Assets\\rules1.bmp",
    IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
  if (hBitmapTitle[RULESONE] == NULL) {
    MessageBox(hWnd, "Failed to load image", "Error", MB_OK);
  }

  hBitmapTitle[RULESTWO] = (HBITMAP)LoadImage(NULL, "Assets\\rules2.bmp",
    IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
  if (hBitmapTitle[RULESTWO] == NULL) {
    MessageBox(hWnd, "Failed to load image", "Error", MB_OK);
  }

  hBitmapTitle[LOBBY] = (HBITMAP)LoadImage(NULL, "Assets\\lobby.bmp",
    IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
  if (hBitmapTitle[LOBBY] == NULL) {
    MessageBox(hWnd, "Failed to load image", "Error", MB_OK);
  }

  hBitmapBorder[0] = (HBITMAP)LoadImage(NULL, "Assets\\border1.bmp",
    IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
  if (hBitmapBorder[0] == NULL) {
    MessageBox(hWnd, "Failed to load image", "Error", MB_OK);
  }

  hBitmapBorder[1] = (HBITMAP)LoadImage(NULL, "Assets\\border2.bmp",
    IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
  if (hBitmapBorder[1] == NULL) {
    MessageBox(hWnd, "Failed to load image", "Error", MB_OK);
  }

  hBitmapBorder[2] = (HBITMAP)LoadImage(NULL, "Assets\\border3.bmp",
    IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
  if (hBitmapBorder[2] == NULL) {
    MessageBox(hWnd, "Failed to load image", "Error", MB_OK);
  }

  hBitmapBorder[3] = (HBITMAP)LoadImage(NULL, "Assets\\border4.bmp",
    IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
  if (hBitmapBorder[3] == NULL) {
    MessageBox(hWnd, "Failed to load image", "Error", MB_OK);
  }

  hBitmapBorder[4] = (HBITMAP)LoadImage(NULL, "Assets\\border5.bmp",
    IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
  if (hBitmapBorder[4] == NULL) {
    MessageBox(hWnd, "Failed to load image", "Error", MB_OK);
  }

  hBitmapBorder[5] = (HBITMAP)LoadImage(NULL, "Assets\\border6.bmp",
    IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
  if (hBitmapBorder[5] == NULL) {
    MessageBox(hWnd, "Failed to load image", "Error", MB_OK);
  }

  hBitmapBorder[6] = (HBITMAP)LoadImage(NULL, "Assets\\border7.bmp",
    IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
  if (hBitmapBorder[6] == NULL) {
    MessageBox(hWnd, "Failed to load image", "Error", MB_OK);
  }

  hBitmapBorder[7] = (HBITMAP)LoadImage(NULL, "Assets\\border8.bmp",
    IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
  if (hBitmapBorder[7] == NULL) {
    MessageBox(hWnd, "Failed to load image", "Error", MB_OK);
  }
}

volatile DWORD whatever;

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
  BITMAP bitmap;
  HDC hdcMem;
  HGDIOBJ oldBitmap, oldSprite;
  POINT pnt;
  RECT rc;
  const int pntpi = 72; // points-per-inch
  int pxpi, points, pxheight;
  HBITMAP hBitmapBuffer;
  BOOL err;
  DWORD err2;

  HDC hdcBackBuffer, hdcSprite;
  HBITMAP hbmBackBuffer, hbmSprite;
  MSG msg;

  switch (message)
  {
  case WM_CREATE:
    LoadImages(hWnd);
    windowOpen[0] = 1;
    webhwnd = hWnd; // for server and client to access in the wsapoll lobby loop


    //HDC hdcWindow = GetDC(hWnd);
    //
    //// make back buffer
    //GetClientRect(hWnd, &rc);
    //hdcBackBuffer = CreateCompatibleDC(hdcWindow);
    //hbmBackBuffer = CreateCompatibleBitmap(hdcBackBuffer, rc.right - rc.left, rc.bottom - rc.top);
    //SelectObject(hdcBackBuffer, hbmBackBuffer);  // SHOULD SAVE PREVIOUS...
    //
    //// make sprite
    //hdcSprite = CreateCompatibleDC(hdcWindow);
    //hbmSprite = CreateCompatibleBitmap(hdcSprite, 50, 50);
    //SelectObject(hdcSprite, hbmSprite);  // SHOULD SAVE PREVIOUS...
    //RECT rcSprite;
    //SetRect(&rcSprite, 0, 0, 50, 50);
    //FillRect(hdcSprite, &rcSprite, (HBRUSH)GetStockObject(WHITE_BRUSH));
    //
    //ReleaseDC(hWnd, hdcWindow);



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
  case WM_PAINT:
    // TODO: use UpdateLayeredWindow() instead of WM_PAINT????
    // https://stackoverflow.com/questions/67689087/correct-way-to-handle-and-redraw-a-layered-window-with-a-bitmap

    // validate that HWND
    hdc = BeginPaint(hWnd, &ps);
    // int savedDC = SaveDC(hdc);

    // for clicking purposes
    GetCursorPos(&pnt);
    ScreenToClient(hWnd, &pnt);

    switch (menustate) {
    case TITLEBLANK:
    case TITLEHOST:
    case TITLEJOIN:
    case TITLERULES:
      if (pnt.x >= 528 && pnt.x <= 757) {
        if (pnt.y >= 385 && pnt.y < 472) {
          // Host Button
          menustate = TITLEHOST;
          if (GetAsyncKeyState(VK_LBUTTON) < 0 && is_active && windowOpen[1] == 0) {
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
          if (GetAsyncKeyState(VK_LBUTTON) < 0 && is_active && windowOpen[1] == 0) {
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
      break;
    case RULESONE:
      if (pnt.y >= 619 && pnt.y <= 650 && GetAsyncKeyState(VK_LBUTTON) < 0 && is_active) {
        if (pnt.x >= 975 && pnt.x <= 1074) {
          hBitmapBuffer = hBitmapTitle[TITLEBLANK];
          menustate = TITLEBLANK;
        }
        else if (pnt.x >= 1129 && pnt.x <= 1160) {
          hBitmapBuffer = hBitmapTitle[RULESTWO];
          menustate = RULESTWO;
        }
      }
      break;
    case RULESTWO:
      if (pnt.y >= 619 && pnt.y <= 650 && GetAsyncKeyState(VK_LBUTTON) < 0 && is_active) {
        if (pnt.x >= 975 && pnt.x <= 1074) {
          hBitmapBuffer = hBitmapTitle[TITLEBLANK];
          menustate = TITLEBLANK;
        }
        else if (pnt.x >= 1087 && pnt.x <= 1118) {
          hBitmapBuffer = hBitmapTitle[RULESONE];
          menustate = RULESONE;
        }
      }
      break;
    case LOBBY:
      //if (pnt.x >= 120 && pnt.x <= 309 && pnt.y >= 590 && pnt.y <= 639 && GetAsyncKeyState(VK_LBUTTON) < 0 && is_active) {
      //  // start game
      //}
      break;
    default:
      break;
    }

    // get window's client rectangle. We need this for bitmap creation.
    GetClientRect(hWnd, &rc);

    // create memory DC and memory bitmap where we shall do our drawing
    hdcMem = CreateCompatibleDC(hdc);
    // // if i use CreateCompatibleBitmap, then it only gets Selected once and will not update the DC (whyyy??)
    // hBitmapBuffer = CreateCompatibleBitmap(hdc, rc.right - rc.left, rc.bottom - rc.top);
    // hBitmapBuffer = hBitmapTitle[menustate];
    // oldBitmap = SelectObject(hdcMem, hBitmapBuffer);

    hBitmapBuffer = (HBITMAP)CopyImage(hBitmapTitle[menustate], IMAGE_BITMAP, BWIDTH, BHEIGHT, LR_COPYRETURNORG);
    oldBitmap = SelectObject(hdcMem, hBitmapBuffer);

    // oldBitmap = SelectObject(hdcMem, hBitmapTitle[menustate]);

    // loading a new image each time does work! but that's terrible >.<
    // oldBitmap = SelectObject(hdcMem, (HBITMAP)LoadImage(NULL, "Assets\\lobby.bmp",
    //   IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));

    if (whatever == 5) {
      // it looks like hBitMapTitle[xxx] gets overwritten upon blitting, because the lobby menu still has
      // outdated text and selected characters and doesn't properly flush it out; switching to a different
      // bitmap will indeed show a change, but that would also get overwritten eventually
      
      FillRect(hdc, &rc, (HBRUSH)GetStockObject(BLACK_BRUSH));
      oldBitmap = SelectObject(hdcMem, hBitmapTitle[TITLEBLANK]);
      BitBlt(hdc, 0, 0, BWIDTH, BHEIGHT, hdcMem, 0, 0, SRCCOPY);
    }

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

      // test to see what's in hdcMem and draw it at this stage before hdcSprite
      if (whatever == 5) {
        GetClientRect(hWnd, &rc);
        FillRect(hdc, &rc, (HBRUSH)GetStockObject(BLACK_BRUSH));
        BitBlt(hdc, 0, 0, BWIDTH, BHEIGHT, hdcMem, 0, 0, SRCCOPY);
      }

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

    // render memory buffer to on-screen DC
    BitBlt(hdc, 0, 0, BWIDTH, BHEIGHT, hdcMem, 0, 0, SRCCOPY);

    // RestoreDC(hdc, savedDC);

    // cleanup
    SelectObject(hdcMem, oldBitmap);
    DeleteDC(hdcMem);

    //ReleaseDC(hWnd, hdc); // paired w/ GetDC()
    //hdc = NULL;
    EndPaint(hWnd, &ps);
    break;
  case WM_DESTROY:
    windowOpen[0] = 0;
    for (int i = 0; i < 6; i++) {
      DeleteObject(hBitmapTitle[i]);
    }
    for (int i = 0; i < 8; i++) {
      DeleteObject(hBitmapBorder[i]);
    }
    PostQuitMessage(0);
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
    windowOpen[1] = 1;

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
      sprintf_s(partymems, PARTYSTRSIZE, "1. %s (host)", player[0].username);
      GetWindowText(portHwnd, portstr, NAME_SIZE);
      portno = atoi(portstr);

      // input check for username and port #
      if (player[0].username != NULL && portno >= 1024 && portno <= 65535) {
        // close this window and switch to lobby window
        getIPreq(ip);
        // using strlen instead of int length return to avoid using another global variable
        sprintf_s(hexcode + strlen(hexcode), 9, "%08X", IPtoHexCode(ip));

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
    windowOpen[1] = 0;
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
    windowOpen[1] = 1;

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
      portno = atoi(portstr);

      // input check for username and port #
      if (player[0].username != NULL && portno >= 1024 && portno <= 65535 && strlen(codestr) == 8) {
        // using strlen instead of int length return to avoid using another global variable
        sprintf_s(hexcode + strlen(hexcode), 9, "%s", codestr);
        // local ip 127.0.0.1: 142B8F0B
        HexCodetoIP(codestr, ip);

        //clientJoin(portno);
        networkthread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)clientJoin, portno, 0, &tIdweb);

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
    windowOpen[1] = 0;
    break;
  }

  return DefWindowProc(hWnd, message, wParam, lParam);
}
