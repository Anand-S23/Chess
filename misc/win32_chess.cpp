#include <Windows.h>
#include <stdint.h>

#define internal static
#define global_variable static
#define local_persist static

struct win32_offscreen_buffer
{
    BITMAPINFO Info;
    void *Memory;
    int Width;
    int Height;
    int Pitch;
    int BytesPerPixel = 4;
};

struct win32_window_dimension
{
    int Width;
    int Height;
};

// TODO: Need to make this non-global later
global_variable bool Running;
global_variable win32_offscreen_buffer GlobalBackbuffer;

win32_window_dimension Win32GetWindowDimension(HWND Window)
{
    win32_window_dimension Dimension;

    RECT ClientRect;
    GetClientRect(Window, &ClientRect);
    Dimension.Width = ClientRect.right - ClientRect.left;
    Dimension.Height = ClientRect.bottom - ClientRect.top;

    return Dimension;
}
    
internal void RenderBoard(win32_offscreen_buffer Buffer)
{
    int Width = Buffer.Width;
    int Height = Buffer.Height;

    uint8_t *Row = (uint8_t *)Buffer.Memory;
    for (int y = 0; y < Height; ++y)
    {
        uint32_t *Pixel = (uint32_t *)Row;
        for (int x = 0; x < Width; ++x)
        {
            uint8_t CheckerWidth = Width / 8; 
            uint8_t CheckerHeight = Height / 8;

            if ((x / CheckerWidth) % 2 == (y / CheckerHeight) % 2)
            {
                *Pixel++ = 0;
            }
            else
            {
                *Pixel++ = ((255 << 16) | (255 << 8) | 255);
            }
        }

        Row += Buffer.Pitch;
    }
}

internal void Win32ResizeDIBSection(win32_offscreen_buffer *Buffer,
                                    int Width, int Height)
{
    // TODO: Need to bulletproof
    if (Buffer->Memory)
    {
        VirtualFree(Buffer->Memory, 0, MEM_RELEASE);
    }

    Buffer->Width = Width;
    Buffer->Height = Height;
    Buffer->BytesPerPixel = 4;

    Buffer->Info.bmiHeader.biSize = sizeof(Buffer->Info.bmiHeader);
    Buffer->Info.bmiHeader.biWidth = Buffer->Width;
    Buffer->Info.bmiHeader.biHeight = Buffer->Height;
    Buffer->Info.bmiHeader.biPlanes = 1;
    Buffer->Info.bmiHeader.biBitCount = 32;
    Buffer->Info.bmiHeader.biCompression = BI_RGB;

    int MemorySize = (Buffer->Width * Buffer->Height) * Buffer->BytesPerPixel;
    Buffer->Memory = VirtualAlloc(0, MemorySize, MEM_COMMIT, PAGE_READWRITE);

    Buffer->Pitch = Width * Buffer->BytesPerPixel;
}

internal void Win32DisplayBufferInWindow(HDC DeviceContext,
                                         int WindowWidth, int WindowHeight,
                                         win32_offscreen_buffer Buffer,
                                         int x, int y, int Width, int Height)
{
    StretchDIBits(DeviceContext,
                  0, 0, WindowWidth, WindowHeight,
                  0, 0, Buffer.Width, Buffer.Height,
                  Buffer.Memory, &Buffer.Info,
                  DIB_RGB_COLORS, SRCCOPY);
}

LRESULT CALLBACK Win32MainWindowCallback(HWND Window,
                                         UINT Message,
                                         WPARAM WParam,
                                         LPARAM LParam)
{
    LRESULT Result = 0;
    
    switch(Message)
    {
        case WM_SIZE:
        {
        } break;

        case WM_DESTROY:
        {
            Running = false; 
        } break;

        case WM_CLOSE:
        {
            Running = false;
        } break;

        case WM_ACTIVATEAPP:
        {
            OutputDebugStringA("WM_ACTIVATEAPP");
        } break;

        case WM_PAINT:
        {
            win32_window_dimension Dimension =
                Win32GetWindowDimension(Window);

            PAINTSTRUCT Paint;
            HDC DeviceContext = BeginPaint(Window, &Paint);
            int x = Paint.rcPaint.left;
            int y = Paint.rcPaint.top;
            int Width = Paint.rcPaint.right - Paint.rcPaint.left;
            int Height = Paint.rcPaint.bottom - Paint.rcPaint.top;
            Win32DisplayBufferInWindow(DeviceContext,
                                       Dimension.Width, Dimension.Height,
                                       GlobalBackbuffer, x, y,
                                       Dimension.Width, Dimension.Height);
            EndPaint(Window, &Paint);
        } break;

        default:
        {
            Result = DefWindowProcA(Window, Message, WParam, LParam);
        } break;
    }

    return Result;
}
            
int CALLBACK WinMain(HINSTANCE Instance,
                     HINSTANCE PrevInstance,
                     LPSTR CmdLine,
                     int CmdShow)
{
    WNDCLASS WindowClass = {};

    Win32ResizeDIBSection(&GlobalBackbuffer, 720, 720);

    WindowClass.style = CS_OWNDC|CS_HREDRAW|CS_VREDRAW;
    WindowClass.lpfnWndProc = Win32MainWindowCallback;
    WindowClass.hInstance = Instance;
    //WindowClass.hIcon;
    WindowClass.lpszClassName = "Chess";

    if (RegisterClassA(&WindowClass))
    {
       HWND Window = CreateWindowExA( 0, WindowClass.lpszClassName,
                                      "Chess",
                                      WS_OVERLAPPEDWINDOW|WS_VISIBLE,
                                      CW_USEDEFAULT, CW_USEDEFAULT,
                                      CW_USEDEFAULT, CW_USEDEFAULT,
                                      0, 0, Instance, 0);

       if (Window)
       {
           Running = true;
           while (Running)
           {
               MSG Message;
               while (PeekMessage(&Message, 0, 0, 0, PM_REMOVE))
               {
                   if (Message.message == WM_QUIT)
                   {
                       Running = false;
                   }

                   TranslateMessage(&Message);
                   DispatchMessage(&Message);
               }

               RenderBoard(GlobalBackbuffer);

               HDC DeviceContext = GetDC(Window);
               win32_window_dimension Dimension =
                   Win32GetWindowDimension(Window);
               Win32DisplayBufferInWindow(DeviceContext,
                                          Dimension.Width, Dimension.Height,
                                          GlobalBackbuffer, 0, 0,
                                          Dimension.Width, Dimension.Height);
               ReleaseDC(Window, DeviceContext);
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
