#pragma once
#include <windowsx.h>
#include <tchar.h>
#include "globals.h"

#define BWIDTH 1280
#define BHEIGHT 720
#define PARTYSTRSIZE 296

enum game_state { TITLEBLANK, TITLEHOST, TITLEJOIN, TITLERULES, RULESONE, RULESTWO, LOBBY, GAMESTART, DEBUGMODE };

extern HINSTANCE hInstanceGlob;
extern char ip[16], hexcode[43];
extern short menustate;
extern char partymems[PARTYSTRSIZE];
extern HWND webhwnd;
extern BOOL is_active;
extern BOOL babytoggle[13];
extern RECT pselect[MAX_PLAYERS];
extern unsigned char networktoggle; // 00000000 nothing
																		// 00000001 closesocket
																		// 00000010 selectbabyunicorn
POINT clientPnt;

// x, y, width, height
struct Button {
	short x;
	short y;
	short width;
	short height;
	char	filename[64];
	HBITMAP bitmap;
};

// potential joint structure for the baby unicorn icon + player number
struct PlayerIcon {
	short x;
	short y;
	short width;
	short height;
	char icon_filename[64];
	char player_filename[64];
	HBITMAP icon_bitmap;
	HBITMAP player_bitmap;
};

struct ToolTip {
	char title[128];
	char msg[512];
	int x;
	int y;
	int width;
	int height;
	BOOL ishover;
	int fonttxt;
	int fonttitle;
};

// callback functions
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK WndProcHost(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK WndProcJoin(HWND, UINT, WPARAM, LPARAM);

// window/UI creation
void CreateHostWindow(HWND);
void CreateJoinWindow(HWND);
void DisplayCardWindow(HDC*, HDC*, int, int*, int);
void CreateCustomToolTip(HDC*);
struct ToolTip ReturnCardHoverTip(char*, char*, int, int);
struct ToolTip ReturnPlayerHoverTip(int, int, int);

void InitFonts(HDC);
void DestroyFonts();

// helper functions
void GameLoop(HWND);
void GUIlobby(POINT);
int SelectBabyUnicorn(int, POINT);
void InitDebugMode();
void ResetDebugMode();
