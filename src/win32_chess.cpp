#include <Windows.h>

#define internal static
#define global_variable static
#define local_persist static

// TODO: Need to make this non-global later
global_variable bool Running;

LRESULT CALLBACK
MainWindowCallback(HWND Window,
                   UINT Message,
                   WPARAM WParam,
                   LPARAM LParam)
{
    LRESULT Result = 0;
    
    switch(Message)
    {
        case WM_SIZE:
        {
            OutputDebugStringA("WM_SIZE");
        } break;
        case WM_DESTROY:
        {
            OutputDebugStringA("WM_DESTROY");
        } break;
        case WM_CLOSE:
        {
            Running = false;
        } break;
        case WM_ACTIVATEAPP:
        {
            Running = false; 
        } break;
        case WM_PAINT:
        {
            PAINTSTRUCT Paint;
            HDC DeviceContext = BeginPaint(Window, &Paint);
            int x = Paint.rcPaint.left;
            int y = Paint.rcPaint.top;
            int Width = Paint.rcPaint.right - Paint.rcPaint.left;
            int Height = Paint.rcPaint.bottom - Paint.rcPaint.top;
            PatBlt(DeviceContext, x, y, Width, Height, WHITENESS);
            EndPaint(Window, &Paint);
        } break;
        default:
        {
            Result = DefWindowProcA(Window, Message, WParam, LParam);
        } break;
    }

    return Result;
}
            
int CALLBACK
WinMain(HINSTANCE Instance,
        HINSTANCE PrevInstance,
        LPSTR CmdLine,
        int CmdShow)
{
    WNDCLASS WindowClass = {};

    WindowClass.style = CS_OWNDC|CS_HREDRAW|CS_VREDRAW;
    WindowClass.lpfnWndProc = MainWindowCallback;
    WindowClass.hInstance = Instance;
    //WindowClass.hIcon;
    WindowClass.lpszClassName = "Chess";

    if (RegisterClassA(&WindowClass))
    {
       HWND WindowHandle =
           CreateWindowExA(
               0,
               WindowClass.lpszClassName,
               "Chess",
               WS_OVERLAPPEDWINDOW|WS_VISIBLE,
               CW_USEDEFAULT,
               CW_USEDEFAULT,
               CW_USEDEFAULT,
               CW_USEDEFAULT,
               0,
               0,
               Instance,
               0);

       if (WindowHandle)
       {
           Running = true;
           while (Running)
           {
               MSG Message;
               BOOL MessageResult = GetMessageA(&Message, 0, 0, 0);
               if (MessageResult > 0)
               {
                   TranslateMessage(&Message);
                   DispatchMessage(&Message);
               }
               else
               {
                   break;
               }
           }
       }
       else
       {
           // TODO: Logging
       }
    }
    else
    {
        // TODO: Logging
    }

    return 0;
}
