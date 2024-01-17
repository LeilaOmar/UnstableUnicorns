#pragma once
#include <windowsx.h>
#include <tchar.h>
#include "globals.h"

#define BWIDTH 1280
#define BHEIGHT 720
#define PARTYSTRSIZE 296

enum GameState { TITLEBLANK, TITLEHOST, TITLEJOIN, TITLERULES, RULESONE, RULESTWO, LOBBY, GAMESTART, DEBUGMODE } ;

extern HINSTANCE hInstanceGlob;
extern char ip[16], hexcode[43];
extern enum GameState menustate;
extern char partymems[PARTYSTRSIZE];
extern HWND webhwnd;
extern BOOL is_active;
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
	void (*onHover)(struct Button*, void*);	// function that triggers on mouse hover
	void (*onClick)(void*);	// function that triggers on left mouse click
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

// button management
void HoverTitle(struct Button*, BOOL*);
void InitTitleButtons(struct Button, HWND);
void InitRuleButtons(struct Button);
void InitLobbyButtons(struct Button);
void InitPlayerIcons();
void InitButtonManager(HWND);

// initializing/deinitializing data
void LoadImages(HWND);
void InitFonts(HDC);
void DestroyFonts();
void InitDebugMode();
void ResetDebugMode();

// state functions
void NothingBurger();
int StateButtonMap(int);
void SwitchState(int);
void HostGeneration(HWND);
void JoinGeneration(HWND);
void StartGame();
void LeaveLobby();
void ClickBabyUnicorn(POINT);
void GUIlobby(POINT);

// helper functions
void GameLoop(HWND);
int SelectBabyUnicorn(int, POINT);
