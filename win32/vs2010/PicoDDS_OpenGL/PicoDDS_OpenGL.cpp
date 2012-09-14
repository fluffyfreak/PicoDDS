// PicoDDS_OpenGL.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "PicoDDS_OpenGL.h"

#define MAX_LOADSTRING 100

// Global Variables:
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

HINSTANCE hInst;								// current instance
HWND	hWnd;									// current window handle
HGLRC	hglRC=nullptr;
HDC		hglDC=nullptr;

// Forward declarations of functions included in this code module:
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
void RenderOpenGLScene();
HWND CreateOpenGLWindow(int x, int y, int width, int height, BYTE type, DWORD flags, int nCmdShow);
void EnableOpenGL(HWND hWnd, HDC * hDC, HGLRC * hRC);
void DisableOpenGL(HWND hWnd, HDC hDC, HGLRC hRC);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_PICODDS_OPENGL, szWindowClass, MAX_LOADSTRING);

	// Perform application initialization:
	hWnd = CreateOpenGLWindow(0, 0, 512, 512, PFD_TYPE_RGBA, 0, nCmdShow);
	if( nullptr==hWnd )
	{
		return FALSE;
	}
	hglDC = GetDC(hWnd);
    hglRC = wglCreateContext(hglDC);
    wglMakeCurrent(hglDC, hglRC);

	EnableOpenGL( hWnd, &hglDC, &hglRC );

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PICODDS_OPENGL));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	DisableOpenGL( hWnd, hglDC, hglRC );

	return (int) msg.wParam;
}



//
//   FUNCTION: CreateOpenGLWindow
//
//   PURPOSE: 
//
//   COMMENTS:
//
//        blah blah
//
HWND CreateOpenGLWindow(int x, int y, int width, int height, 
	BYTE type, DWORD flags, int nCmdShow)
{
    int         pf;
    HDC         hDC;
    HWND        hWnd;
    WNDCLASSEX  wc;
    PIXELFORMATDESCRIPTOR pfd;

    /* only register the window class once - use hInstance as a flag. */
    if (!hInst) {
        hInst = GetModuleHandle(NULL);
		wc.cbSize		 = sizeof(WNDCLASSEX);
        wc.style         = CS_OWNDC;
        wc.lpfnWndProc   = WndProc;
        wc.cbClsExtra    = 0;
        wc.cbWndExtra    = 0;
        wc.hInstance     = hInst;
        wc.hIcon         = LoadIcon(hInst, MAKEINTRESOURCE(IDI_PICODDS_OPENGL));
        wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
        wc.hbrBackground = NULL;
        wc.lpszMenuName  = MAKEINTRESOURCE(IDC_PICODDS_OPENGL);
        wc.lpszClassName = szWindowClass;
		wc.hIconSm		 = LoadIcon(hInst, MAKEINTRESOURCE(IDI_SMALL));

        if (!RegisterClassEx(&wc)) {
            MessageBox(NULL, L"RegisterClass() failed: Cannot register window class.", L"Error", MB_OK);
            return NULL;
        }
    }

    hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW |
                        WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
                        x, y, width, height, NULL, NULL, hInst, NULL);

    if (hWnd == NULL) {
        MessageBox(NULL, L"CreateWindow() failed: Cannot create a window.", L"Error", MB_OK);
        return NULL;
    }

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

    hDC = GetDC(hWnd);

    /* there is no guarantee that the contents of the stack that become
       the pfd are zeroed, therefore _make sure_ to clear these bits. */
    memset(&pfd, 0, sizeof(pfd));
    pfd.nSize        = sizeof(pfd);
    pfd.nVersion     = 1;
    pfd.dwFlags      = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | flags;
    pfd.iPixelType   = type;
    pfd.cColorBits   = 32;

    pf = ChoosePixelFormat(hDC, &pfd);
    if (pf == 0) {
        MessageBox(NULL, L"ChoosePixelFormat() failed: Cannot find a suitable pixel format.", L"Error", MB_OK); 
        return 0;
    } 
 
    if (SetPixelFormat(hDC, pf, &pfd) == FALSE) {
        MessageBox(NULL, L"SetPixelFormat() failed: Cannot set format specified.", L"Error", MB_OK);
        return 0;
    } 

    DescribePixelFormat(hDC, pf, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

    ReleaseDC(hWnd, hDC);

    return hWnd;
}    

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		RenderOpenGLScene();
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;
	case WM_SIZE:
        glViewport(0, 0, LOWORD(lParam), HIWORD(lParam));
        PostMessage(hWnd, WM_PAINT, 0, 0);
        break;
	case WM_CHAR:
        switch (wParam) {
        case 27:                        /* ESC key */
            PostQuitMessage(0);
            break;
        }
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

void RenderOpenGLScene()
{
    /* rotate a triangle around */
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_TRIANGLES);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex2i(0,  1);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex2i(-1, -1);
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex2i(1, -1);
    glEnd();
    glFlush();
}

void EnableOpenGL(HWND hWnd, HDC * hDC, HGLRC * hRC)
{
    PIXELFORMATDESCRIPTOR pfd;
    int iFormat;

    // get the device context (DC)
    *hDC = GetDC( hWnd );

    // set the pixel format for the DC
    ZeroMemory( &pfd, sizeof( pfd ) );
    pfd.nSize = sizeof( pfd );
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL |
                  PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;
    iFormat = ChoosePixelFormat( *hDC, &pfd );
    SetPixelFormat( *hDC, iFormat, &pfd );

    // create and enable the render context (RC)
    *hRC = wglCreateContext( *hDC );
    wglMakeCurrent( *hDC, *hRC );
}

void DisableOpenGL(HWND hWnd, HDC hDC, HGLRC hRC)
{
    wglMakeCurrent( NULL, NULL );
    wglDeleteContext( hRC );
    ReleaseDC( hWnd, hDC );
}