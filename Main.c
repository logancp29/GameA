#include <stdio.h>		//Pasting the contents of stdio.h into this file; contains function declarations

#pragma warning ( push, 3 )
#pragma warning (disable : 4668)
#include <windows.h>
#include <winbase.h>
#pragma warning ( pop )
#include "Main.h"

BOOL gGameIsRunning;
HWND gGameWindow;
GAMEBITMAP gDrawingSurface;

int WinMain(HINSTANCE Instance, HINSTANCE PreviousInstance, PSTR CommandLine, INT CmdShow)
{
    UNREFERENCED_PARAMETER(Instance);
	UNREFERENCED_PARAMETER(PreviousInstance);
    UNREFERENCED_PARAMETER(CommandLine);
    UNREFERENCED_PARAMETER(CmdShow);

    if (GameIsAlreadyRunning() == TRUE)
    {
        MessageBoxA(NULL, "Another instance of this program is already running!", "Warning", MB_ICONEXCLAMATION | MB_OK);

        goto Exit;
    }

    if (CreateMainGameWindow() != ERROR_SUCCESS)
    {
        goto Exit;
    }

    gDrawingSurface.BitmapInfo.bmiHeader.biSize = sizeof(gDrawingSurface.BitmapInfo.bmiHeader);

    gDrawingSurface.BitmapInfo.bmiHeader.biWidth = GAME_RES_WIDTH;     // Resolution is 384 by 216 pixels

    gDrawingSurface.BitmapInfo.bmiHeader.biHeight = GAME_RES_HEIGHT;

    gDrawingSurface.BitmapInfo.bmiHeader.biBitCount = GAME_BPP;

    gDrawingSurface.BitmapInfo.bmiHeader.biCompression = BI_RGB;

    gDrawingSurface.BitmapInfo.bmiHeader.biPlanes = 1;

    if ((gDrawingSurface.Memory = VirtualAlloc(NULL, GAME_DRAWING_AREA_MEMORY_SIZE, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE)) == NULL)
    {
        MessageBoxA(NULL, "Failed to allocate memory for drawing surface!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        
        goto Exit;
    }

    MSG Message = { 0 };        // { 0 } sets all data structure values to zero

    gGameIsRunning = TRUE;

    while (gGameIsRunning)      // Resembles one frame of a game
    {
        while (PeekMessageA(&Message, gGameWindow, 0, 0, PM_REMOVE))
        {
            DispatchMessageA(&Message);
        }

        ProcessPlayerInput();

        RenderFrameGraphics();

        Sleep(1);   // This needs to be optimised
    }

Exit:

    return(0);
}

LRESULT CALLBACK MainWindowProc(_In_ HWND WindowHandle, _In_ UINT Message, _In_ WPARAM WParam, _In_ LPARAM LParam)
{
    LRESULT Result = 0;

    switch (Message)
    {
        case WM_CLOSE:
        {
            gGameIsRunning = FALSE;

            PostQuitMessage(0);
            break;
        }
        default:
        {
            Result = DefWindowProcA(WindowHandle, Message, WParam, LParam);
        }
    }
    return Result;
}

DWORD CreateMainGameWindow(void)
{
    DWORD Result = ERROR_SUCCESS;

    WNDCLASSEXA WindowClass = { 0 };

    WindowClass.cbSize = sizeof(WNDCLASSEXA);               //Size of the Window

    WindowClass.style = 0;                                  //Style of the Window

    WindowClass.lpfnWndProc = MainWindowProc;               //Points to Window Proceedure  

    WindowClass.cbClsExtra = 0;

    WindowClass.cbWndExtra = 0;

    WindowClass.hInstance = GetModuleHandle(NULL);                       //Handle to an instance

    WindowClass.hIcon = LoadIconA(NULL, IDI_APPLICATION);   //Handle to icon, idi_application is a built-in one

    WindowClass.hIconSm = LoadIconA(NULL, IDI_APPLICATION);

    WindowClass.hCursor = LoadCursorA(NULL, IDC_ARROW);

    WindowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); //Handle to background brush

    WindowClass.lpszMenuName = NULL;

    WindowClass.lpszClassName = GAME_NAME "_WINDOWCLASS";


    if (RegisterClassExA(&WindowClass) == 0)                    //If it fails to register window class; returns 0
    {
        Result = GetLastError();

        MessageBoxA(NULL, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);

        goto Exit;
    }

    gGameWindow = CreateWindowExA(WS_EX_CLIENTEDGE, WindowClass.lpszClassName, "Window Title", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 640, 480, NULL, NULL, GetModuleHandle(NULL), NULL);

    if (gGameWindow == NULL)
    {
        Result = GetLastError();

        MessageBoxA(NULL, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        
        goto Exit;
    }
    

Exit:
    return(Result);
}

BOOL GameIsAlreadyRunning(void)
{
    HANDLE Mutex = NULL;                                    //Mutex is a piece of data that gates access to particular information
    Mutex = CreateMutexA(NULL, FALSE, GAME_NAME "GameMutex");

    if (GetLastError() == ERROR_ALREADY_EXISTS)
    {
        return(TRUE);
    }
    else
    {
        return(FALSE);
    }
}

void ProcessPlayerInput(void)
{
    short EscapeKeyIsDown = GetAsyncKeyState(VK_ESCAPE);

    if (EscapeKeyIsDown)
    {
        SendMessageA(gGameWindow, WM_CLOSE, 0, 0);
    }
}

void RenderFrameGraphics(void)
{

}