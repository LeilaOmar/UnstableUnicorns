#pragma once
#include <windowsx.h>
#include <tchar.h>
#include "globals.h"

#define TITLEBLANK 0
#define TITLEHOST 1
#define TITLEJOIN 2
#define TITLERULES 3
#define RULESONE 4
#define RULESTWO 5
#define LOBBY 6
#define BWIDTH 1280
#define BHEIGHT 720
#define PARTYSTRSIZE 296

extern HINSTANCE hInstanceGlob;
extern char ip[16], hexcode[43];
extern short menustate;
extern char partymems[PARTYSTRSIZE];
extern HWND webhwnd;
extern unsigned char is_active;
extern unsigned char babytoggle[13];
extern RECT pselect[MAX_PLAYERS];

// Forward declarations of functions included in this code module:
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK WndProcHost(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK WndProcJoin(HWND, UINT, WPARAM, LPARAM);

void CreateHostWindow(HWND);
void CreateJoinWindow(HWND);
void GameLoop(HWND);
int SelectBabyUnicorn(int, POINT);