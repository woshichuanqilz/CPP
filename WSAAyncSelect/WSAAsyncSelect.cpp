#include <WinSock2.h>
#include <Windows.h>
#include <StrSafe.h>

#pragma comment(lib, "Ws2_32")

#define WM_SOCKET WM_USER+1

LRESULT CALLBACK WinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_SOCKET:
        {
            if (WSAGETSELECTERROR(lParam))
            {
                shutdown(wParam, SD_BOTH);
                closesocket(wParam);
                return 0;
            }

            switch (WSAGETSELECTEVENT(lParam))
            {
            case FD_ACCEPT:
                {
                    SOCKET client = accept(wParam, NULL, NULL);
                    WSAAsyncSelect(client, hWnd, WM_SOCKET, FD_READ | FD_WRITE | FD_CLOSE);
                }
                break;
            case FD_WRITE:
                break;
            case FD_READ:
                {
                    CHAR szText[1024] = { 0 };
                    if (SOCKET_ERROR == recv(wParam, szText, 1024, 0))
                    {
                        shutdown(wParam, SD_BOTH);
                        closesocket(wParam);
                    }
                    else
                    {
                        CHAR szMsg[1100] = "receive data";
                        strcat_s(szMsg, szText);
                        MessageBoxA(hWnd, szMsg, "结果", MB_OK);
                    }
                }
                break;
            case FD_CLOSE:
                shutdown(wParam, SD_BOTH);
                closesocket(wParam);
                break;
            }
        }
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, PSTR pcszCommand, int iShow)
{
    TCHAR szAppName[] = TEXT("WSAAsyncSelect");
    WNDCLASSEX wndClsEx;

    wndClsEx.cbClsExtra = 0;
    wndClsEx.cbSize = sizeof(wndClsEx);
    wndClsEx.cbWndExtra = 0;
    wndClsEx.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndClsEx.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndClsEx.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndClsEx.hIconSm = NULL;
    wndClsEx.hInstance = hInstance;
    wndClsEx.lpfnWndProc = WinProc;
    wndClsEx.lpszClassName = szAppName;
    wndClsEx.lpszMenuName = NULL;
    wndClsEx.style = CS_HREDRAW | CS_VREDRAW;
    
    if (!RegisterClassEx(&wndClsEx))
    {
        MessageBox(NULL, TEXT("register window failed"), TEXT("错误"), MB_OK | MB_ICONERROR);
        return 0;
    }

    HWND hWnd = CreateWindowEx(
        0,
        szAppName, 
        szAppName,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        NULL,
        NULL,
        hInstance,
        NULL);
    if (NULL == hWnd)
    {
        MessageBox(NULL, TEXT("创建窗口失败"), TEXT("错误"), MB_OK | MB_ICONERROR);
        return 0;
    }

    ShowWindow(hWnd, iShow);
    UpdateWindow(hWnd);

    WSADATA wsaData;
    WSAStartup(WINSOCK_VERSION, &wsaData);

    SOCKET sListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (INVALID_SOCKET == sListen)
    {
        MessageBox(hWnd, TEXT("Create socket failed"), TEXT("错误"), MB_OK | MB_ICONERROR);
        WSACleanup();
        return 0;
    }

    SOCKADDR_IN sin;
    USHORT uPort = 4567;
    
    ZeroMemory(&sin, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(uPort);
    sin.sin_addr.S_un.S_addr = INADDR_ANY;

    if (SOCKET_ERROR == bind(sListen, (PSOCKADDR)&sin, sizeof(sin)))
    {
        MessageBox(hWnd, TEXT("bind socket failed"), TEXT("错误"), MB_OK | MB_ICONERROR);
        shutdown(sListen, SD_RECEIVE);
        closesocket(sListen);
        WSACleanup();
        return 0;
    }

    //将套接字设为窗口通知消息类型
    WSAAsyncSelect(sListen, hWnd, WM_SOCKET, FD_ACCEPT | FD_CLOSE);
    listen(sListen, 5);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    WSACleanup();

    return msg.wParam;
}
