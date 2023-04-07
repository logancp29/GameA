#pragma once

#define GAME_NAME	"GAME_A"

#define GAME_RES_WIDTH	384

#define GAME_RES_HEIGHT	216

#define GAME_BPP		32

// Amount of memory each frame of the game will occupy
#define GAME_DRAWING_AREA_MEMORY_SIZE	(GAME_RES_WIDTH * GAME_RES_HEIGHT * (GAME_BPP / 8))


typedef struct GAMEBITMAP
{
	BITMAPINFO BitmapInfo;

	void* Memory;

} GAMEBITMAP;

LRESULT CALLBACK MainWindowProc(_In_ HWND WindowHandle, _In_ UINT Message, _In_ WPARAM WParam, _In_ LPARAM LParam);   //Predefining function so compiler knows where to look

DWORD CreateMainGameWindow(void);       // _In_ specifies parameter is an input to the compiler; source code annotation language

BOOL GameIsAlreadyRunning(void);

void ProcessPlayerInput(void);

void RenderFrameGraphics(void);