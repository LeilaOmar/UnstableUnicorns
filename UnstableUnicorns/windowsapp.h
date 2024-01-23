#pragma once
#include <windowsx.h>
#include <tchar.h>
#include "globals.h"

#define BWIDTH 1280
#define BHEIGHT 720
#define PARTYSTRSIZE 296

enum GameState { TITLEBLANK, RULESONE, RULESTWO, LOBBY, GAMESTART, DEBUGMODE, NUMSTATES } ;

typedef struct {
	enum GameState state;
	void (*statePaint)(HDC, HDC*);
	void (*stateHover)(POINT);
	void (*stateClick)(POINT);
} StateManager;

extern HINSTANCE hInstanceGlob;
extern char ip[16], hexcode[43];
extern enum GameState menustate;
extern char partymems[PARTYSTRSIZE];
extern HWND webhwnd;
extern BOOL babytoggle[13];
extern RECT pselect[MAX_PLAYERS];
extern unsigned char networktoggle; // 00000000 nothing
																		// 00000001 closesocket
																		// 00000010 selectbabyunicorn
POINT clientPnt;

struct Button {
	int x;									// left coordinate
	int y;									// top coordinate
	int width;							// length
	int height;							// height
	char	filename[64];			// filename of bitmap source
	HBITMAP bitmap;					// bitmap for button asset
	void* source;						// void variable to use as a reference for functions
	void (*onClick)(void*);	// function that triggers on left mouse click
};

struct ToolTip {
	char title[64];
	char subtitle[64];
	char msg[512];
	int x;
	int y;
	int width;
	int height;
	BOOL ishover;
	int fonttxt;
	int fonttitle;
	COLORREF bgcolor;
};

// callback functions

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK WndProcHost(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK WndProcJoin(HWND, UINT, WPARAM, LPARAM);

// window/UI creation

void CreateHostWindow(HWND);
void CreateJoinWindow(HWND);
void DisplayCardWindow(HDC*, HDC*);
void CreateCustomToolTip(HDC*);
struct ToolTip ReturnCardHoverTip(struct Button);
struct ToolTip ReturnPlayerHoverTip(int, int, int);

// button management

void HornPosition(struct Button);
void InitTitleButtons(struct Button*, HWND, int);
void InitRuleButtons(struct Button*, int);
void InitLobbyButtons(struct Button*);
void InitCardWindowButtons(struct Button*);
void InitCardButtons(struct Button*, int);
void InitDeckButtons(struct Button*);
void InitButtonManager(HWND);

// initializing/deinitializing data
void LoadImages(HWND);
void InitFonts(HDC);
void DestroyFonts();
void InitDebugMode();
void ResetDebugMode();

// state window message functions

void HoverTitle(POINT);
void HoverDebug(POINT);

void ClickTitle(POINT);
void ClickRules(POINT);
void ClickLobby(POINT);
void ClickDebug(POINT);

void PaintTitle(HDC, HDC*);
void PaintLobby(HDC, HDC*);
void PaintDebug(HDC, HDC*);

// button function pointers

void SwitchState(int);
void HostGeneration(HWND);
void JoinGeneration(HWND);
void StartGame();
void LeaveLobby();
void SwitchTab(int);
void TurnPage(int);

// game logic helper functions
int SelectBabyUnicorn(int, POINT);
