#include"platform.h"
#include"plungs_lib.h"


#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

//Windows Globals
static HWND window;

//Platform Implementations

LRESULT CALLBACK windows_window_callback(HWND window, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LRESULT result = 0;

    switch (uMsg)
    {
        case WM_CLOSE:
        {
            running = false;
            break;
        } 

        default:
        {
            result = DefWindowProcA(window, uMsg, wParam, lParam);
            break;
        } 
    }

    return result;
}

bool platform_create_window(int width, int height, char* title)
{
    HINSTANCE instance = GetModuleHandleA(0);

    WNDCLASSA wc = {};
    wc.hInstance = instance;
    wc.hIcon = LoadIcon(instance, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = title;
    wc.lpfnWndProc = windows_window_callback; //Callback for input into the window

    if (!RegisterClassA(&wc))
    {
        return false;
    }

    //WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX
    int dwStyle = WS_OVERLAPPEDWINDOW;

    window = CreateWindowExA(
        0,title,                        //This references lpzClassName
        title,                          //This references the actual title
        dwStyle,
        100,
        100,
        width,
        height,
        NULL,                           //Parent
        NULL,                           //Menu
        instance,
        NULL                           //lpPar
    );

    if (window == false)
    {
        return false;
    }

    ShowWindow(window, SW_SHOW);

    return true;
}

void platform_update_window()
{
    MSG msg;

    while (PeekMessageA(&msg,window,0,0,PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);                      //Calls the callback specificed when creating the window
    }
}

void* platform_load_gl_function(char* funName)
{
    PROC proc = wglGetProcAddress("glCreateProgram");
    if(!proc)
    {
        static HMODULE openglDLL = LoadLibraryA("opengl32.dll");
        SM_ASSERT(false, "Failed to load gl function %s","glCreateProgram");
    }

    return (void*)proc
}
