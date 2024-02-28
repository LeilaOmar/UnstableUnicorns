#pragma once

#include <tchar.h>
#include "globals.h"

#define BWIDTH 1280
#define BHEIGHT 720
#define PARTYSTRSIZE 296

enum GameState {
  TITLEBLANK, //!< Title screen
  RULESONE,   //!< Rules Page 1
  RULESTWO,   //!< Rules Page 2
  LOBBY,      //!< Lobby selection
  GAMESTART,  //!< Game loop
  DEBUGMODE,  //!< Debug testing ground
  NUMSTATES   //!< Total size
};

typedef struct StateManager {
  enum GameState state;
  void (*StatePaint)(HDC *hdcMem);
  void (*StateHover)(POINT pnt);
  void (*StateClick)(POINT pnt);
} StateManager;

extern HINSTANCE g_hInstance;
extern enum GameState menuState;

extern char ip[16];
extern char hexcode[43];
extern char partyMems[PARTYSTRSIZE];

extern RECT pselect[MAX_PLAYERS];
extern BOOL babyToggle[NURSERY_SIZE];
extern int babyMap[MAX_PLAYERS];

extern unsigned char networkToggle;
POINT clientPnt;

struct Button {
  int x;                  //!< x-coordinate at the top-left corner
  int y;                  //!< y-coordinate at the top-left corner
  int width;              //!< width in pixels
  int height;             //!< height in pixels
  char filename[64];      //!< Filename of bitmap source
  HBITMAP bitmap;         //!< Bitmap for button asset
  void *source;           //!< Void variable to use as a reference for functions
  void (*OnClick)(void*); //!< Function that triggers on left mouse click
};

struct ToolTip {
  char title[64];     //!< Unicorn card name or Player username
  char subtitle[64];  //!< Unicorn card type; no equivalent subtitle for Player
  char msg[512];      //!< Unicorn card description or # of cards in Player's stable/hand
  int x;              //!< x-coordinate at the top-left corner
  int y;              //!< y-coordinate at the top-left corner
  int width;          //!< width in pixels
  int height;         //!< height in pixels
  BOOL isHover;       //!< TRUE/FALSE depending on whether or not the cursor is on top of a Hoverable button
  int fonttxt;        //!< Index of the font array to select a font for msg text
  int fonttitle;      //!< Index of the font array to select a font for the title text
  COLORREF bgcolor;   //!< Background color of the ToolTip box; color coded based off of the Card type (player bg's are Magenta)
};

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

/**
 * @brief Lobby helper function for checking pnt against Baby Unicorn RECTs and selecting the corresponding Baby Unicorn
 * @return 0 Nothing was successfully selected
 * @return 1 A Baby Unicorn was picked by player[pnum]
 */
int SelectBabyUnicorn(int pnum, POINT pnt);

/**
 * @brief Sets the starting tab display window to show your own hand
 */
void SetTabs(int pnum);

/**
 * @brief Sets the client-instance player number
 */
void SetClientPnum(int pnum);

/**
 * @brief Sets the current player number upon the start of a new turn
 */
void SetCurrPnum(int pnum);
