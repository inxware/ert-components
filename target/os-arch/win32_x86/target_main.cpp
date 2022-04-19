// EHS_windows.cpp : Defines the entry point for the application.
//

#include "stdafx.h"

#define EHS_TARGET_CODE
#include "ehs_windows.h"
#include "ehs_main.h"
#include "console_queue.h"
#include "revision.h"
#include "console_server.h"
#include "hal_console.h"
#include <ctype.h>
#include <atlbase.h>

/* #define SHOW_STATUSBAR */
#define MAX_LOADSTRING 100

#define MONITOR_RES_X (1280)
#define MONITOR_RES_Y (768)

#ifndef EHS_GUI_SUPPORT
#error "EHS_TARGET_GUI_SUPPORT not defined"
#endif
/**
 * This specifies the subdirectory of the bin directory where EHS files
 * are to go.
 */
const TCHAR* EhsWorkingDir = _T("ehs_env");
extern "C" void EhsTV_paint(HWND hWnd, LPPAINTSTRUCT lpPS);
#ifdef EHS_GUI_SUPPORT
extern "C" HWND EhsTgtMainWindowHandle;
EHS_GLOBAL HWND hEhsOsdLayerWnd;
#endif
#ifdef SHOW_STATUSBAR
EHS_LOCAL HWND EhsTgtStatusbarHandle;
#endif

extern "C"  void EhsWindowsKbHit(long);


/**
 * Output the current state of EHS to the toolbar
 */
EHS_GLOBAL void EhsTgtStatus_state(const char*);
EHS_GLOBAL ehs_bool EhsTgtMainSetWorkingDirectory(const ehs_char* szChildDir);

/**
 * Output the an error message to the toolbar
 */
EHS_GLOBAL void EhsTgtStatus_error(const char*);

INT_PTR CALLBACK hMsgLogDialogHandler(HWND hWndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

/**
 * Set the working directory as  <Lucid Install>/bin/ehs_env
 *
 * @param szChildDir if set, this is the child directory that we change to.
 * @return false if unsuccessful
 */
ehs_bool EhsTgtMainSetWorkingDirectory(const ehs_char* szChildDir)
{
	LPTSTR psCmdLine = GetCommandLine();
	TCHAR *psWorkingDir;
	size_t idx;
	ehs_bool bRet = EHS_FALSE;
	ehs_bool bCurDirUsed = EHS_FALSE; /* indicates that the current directory was used and memory was allocated */

	// ignore leading " (if applicable)
	if (psCmdLine[0] == '"')
		psCmdLine++;

	// remove the executable name from the command line, leaving the path
	for (idx = _tcslen(psCmdLine)-1; (idx > 0) && (psCmdLine[idx] != '\\'); idx--)
		;

	if (idx == 0)
	{
		// there is no path, so just get the current working directory
		idx = GetCurrentDirectory(0,NULL);
		psCmdLine = (LPTSTR)malloc(idx*sizeof(TCHAR));
		GetCurrentDirectory((DWORD)idx,psCmdLine);
		idx = _tcslen(psCmdLine);
		bCurDirUsed = EHS_TRUE;
	}

	psWorkingDir = (TCHAR*)malloc(sizeof(TCHAR)*(idx+3+_tcslen(EhsWorkingDir)+(szChildDir?strlen(szChildDir):0)));
	_tcsncpy(psWorkingDir, psCmdLine, idx);
	psWorkingDir[idx] = 0;
	_tcscat(psWorkingDir,_T("\\"));
	_tcscat(psWorkingDir,EhsWorkingDir);

	if (szChildDir)
	{
		_tcscat(psWorkingDir,_T("\\"));
#ifdef UNICODE

		/* find the end of the string */
		TCHAR *pEnd = psWorkingDir;
		while (*pEnd)
			pEnd++;

		mbstowcs(pEnd,szChildDir,strlen(szChildDir));
		pEnd += strlen(szChildDir);
		*pEnd = 0;
#else
		_tcscat(psWorkingDir,szChildDir);
#endif
	}
#ifdef UNICODE
	if (!_wchdir(psWorkingDir)) bRet = EHS_TRUE;
#else
	if (!_chdir(psWorkingDir)) bRet = EHS_TRUE;
#endif

	if (bCurDirUsed)
	{
		free(psCmdLine);
	}
	free(psWorkingDir);
	return bRet;
}



/**
 * Thread function to start off the main EHS code
 */
DWORD WINAPI EhsThreadFunctionMain(LPVOID lpParam)
{
	EhsMain(NULL);

	return 0;
}

//This code is from Q243953 in case you lose the article and wonder
//where this code came from.
class CLimitSingleInstance
{
protected:
  DWORD  m_dwLastError;
  HANDLE m_hMutex;

public:
  CLimitSingleInstance(TCHAR *strMutexName)
  {
    //Make sure that you use a name that is unique for this application otherwise
    //two apps may think they are the same if they are using same name for
    //3rd parm to CreateMutex
    m_hMutex = CreateMutex(NULL, FALSE, strMutexName); //do early
    m_dwLastError = GetLastError(); //save for use later...
  }

  ~CLimitSingleInstance()
  {
    if (m_hMutex)  //Do not forget to close handles.
    {
       CloseHandle(m_hMutex); //Do as late as possible.
       m_hMutex = NULL; //Good habit to be in.
    }
  }

  BOOL IsAnotherInstanceRunning()
  {
    return (ERROR_ALREADY_EXISTS == m_dwLastError);
  }
};

CLimitSingleInstance g_SingleInstanceObj(TEXT("Global\\{B5D8361C-D06B-4162-AC33-A75FF8F8B35B}"));

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
DWORD dwError;									// Last error received
HWND hMsgLogDialog = NULL;


// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);


int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	HANDLE hEhsThread; /* Handle for the EHS thread */
	DWORD dwEhsThreadId; /* Identifier for the EHS thread */
	DWORD dwTcpIpThreadId; /* Identifier for the TCP/IP Server thread */
	MSG msg;
	HACCEL hAccelTable;
	INITCOMMONCONTROLSEX xInitCtrls;

	/* tell everyone that we haven't started yet */
	*EhsHSys_initCompleteRef = EHS_FALSE;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_EHS_WINDOWS, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Check that there are no other instances of EHS running on this machine (see artf1120)
    if (g_SingleInstanceObj.IsAnotherInstanceRunning())
       return FALSE;


	xInitCtrls.dwSize = sizeof(xInitCtrls);
	xInitCtrls.dwICC = ICC_WIN95_CLASSES; /* required for status bar */
	InitCommonControlsEx(&xInitCtrls);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_EHS_WINDOWS));

	EhsTgtMainSetWorkingDirectory(NULL);

	hEhsThread = CreateThread(NULL /* can't inherit handle */,
		0 /* default stack size */,
		EhsThreadFunctionMain /* start routine */,
		NULL /* parameter passed to the thread */,
		0 /* creation flags - start running the thread immediately */,
		&dwEhsThreadId /* identifier for the thread */
		);

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!IsDialogMessage(hMsgLogDialog, &msg) ||
			!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}


	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_EHS_WINDOWS));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+2); /* makes a black background, apparently */
	wcex.lpszMenuName	= NULL; // MAKEINTRESOURCE(IDC_EHS_WINDOWS);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

static int s_monitorPrefs;
static RECT s_monitorRect;

/**
 * Called with information about each monitor in the system
 */
BOOL CALLBACK callbackMonitorEnum(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprMonitor, LPARAM dwData) {
	MONITORINFO mi;
	mi.cbSize = sizeof(MONITORINFO);
	GetMonitorInfo(hMonitor, &mi);
	int prefs = 0x01; /* any monitor is better than no monitor */

	/* determine how well this monitor meets our needs.
	 * First priority, monitor should be non primary monitor
	 */
	if (!(mi.dwFlags & MONITORINFOF_PRIMARY)) {
		prefs |= 0x80;
	}

	/* second priority monitor should be MONITOR_RES_X pixels wide (or greater - 4th priority) */
	if ((mi.rcMonitor.right - mi.rcMonitor.left) == MONITOR_RES_X) {
		prefs |= 0x40;
	} else if (mi.rcMonitor.right - mi.rcMonitor.left > MONITOR_RES_X) {
		prefs |= 0x10;
	}

	/* third priority monitor should be MONITOR_RES_Y pixels high (or greater - 5th priority */
	if ((mi.rcMonitor.bottom - mi.rcMonitor.top) == MONITOR_RES_Y) {
		prefs |= 0x20;
	} else if ((mi.rcMonitor.bottom - mi.rcMonitor.top) > MONITOR_RES_Y) {
		prefs |= 0x08;
	}

	/* is this monitor better than the one we've found so far? */
	if (prefs > s_monitorPrefs) {
		if (!(prefs & 0x40)) {
			/* monitor isn't exactly the same width as our desired width.
			 * centralize position */
			long nHorzOffset = 0;// don't bother! - more stupid crap we don't need ! ((mi.rcMonitor.right - mi.rcMonitor.left) - MONITOR_RES_X)/2;
			s_monitorRect.left = 	mi.rcMonitor.left + nHorzOffset;
			s_monitorRect.right = 	mi.rcMonitor.right - nHorzOffset;
		} else {
			s_monitorRect.left = 	mi.rcMonitor.left;
			s_monitorRect.right = 	mi.rcMonitor.right;
		}

		if (!(prefs & 0x20)) {
			/* monitor isn't exactly the same height as our desired height.
			 * centralize it. */
			long nVertOffset = 0;// don't bother! - more stupid crap we don't need !((mi.rcMonitor.bottom - mi.rcMonitor.top) - MONITOR_RES_Y)/2;
			s_monitorRect.top = 	mi.rcMonitor.top + nVertOffset;
			s_monitorRect.bottom = 	mi.rcMonitor.bottom - nVertOffset;
		} else {
			s_monitorRect.top = 	mi.rcMonitor.top;
			s_monitorRect.bottom = 	mi.rcMonitor.bottom;
		}
		s_monitorPrefs = prefs;
	}

	return TRUE;
}
//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;
   HWND hWnd_Blank;
   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_POPUP /*WS_OVERLAPPEDWINDOW*/,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);
   if (!hWnd)
   {
	   dwError = GetLastError();
      return FALSE;
   }
	 hWnd_Blank = CreateWindow(szWindowClass, szTitle, WS_POPUP /*WS_OVERLAPPEDWINDOW*/,
         CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);
      if (!hWnd_Blank)
      {
   	   dwError = GetLastError();
         return FALSE;
      }
#ifdef EHS_GUI_SUPPORT
   hEhsOsdLayerWnd = CreateWindow(szWindowClass, L"OSD Layer", WS_POPUP,CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   /***** Make window transparent ****/
   SetWindowLong (hEhsOsdLayerWnd , GWL_EXSTYLE ,
       GetWindowLong (hEhsOsdLayerWnd , GWL_EXSTYLE ) | WS_EX_LAYERED ) ;

   typedef DWORD (WINAPI *PSLWA)(HWND, DWORD, BYTE, DWORD);
   PSLWA pSetLayeredWindowAttributes;
   HMODULE hDLL = LoadLibrary (L"user32");
   pSetLayeredWindowAttributes =
       (PSLWA) GetProcAddress(hDLL,"SetLayeredWindowAttributes");
   if (pSetLayeredWindowAttributes != NULL) {
       /*
       * Second parameter RGB(255,255,255) sets the colorkey
       * to white LWA_COLORKEY flag indicates that color key
       * is valid LWA_ALPHA indicates that ALphablend parameter
       * (factor) is valid
       */
       BOOL bRet = pSetLayeredWindowAttributes (hEhsOsdLayerWnd,
           RGB(0,0,0), 255 /* alphablend factor */, LWA_COLORKEY|LWA_ALPHA);
    /*
        *
        *bRet = pSetLayeredWindowAttributes (hWnd_Blank,
                  RGB(0,0,0), 255 /  * alphablend factor  * /, LWA_COLORKEY);
        */
   }
#endif
   /**********************************/
   /* place window position at secondary monitor (if present) */
#ifdef EHS_GUI_SUPPORT // @todo this following handle is the Video Layer -rename all these stupid references so that they say what they are
   s_monitorPrefs = 0;
   //EnumDisplayMonitors(NULL,NULL,callbackMonitorEnum,NULL);
   SetWindowPos(hWnd_Blank,HWND_TOP,s_monitorRect.left,s_monitorRect.top,MONITOR_RES_X,MONITOR_RES_Y,SWP_SHOWWINDOW);
   SetWindowPos(hWnd,HWND_TOP,s_monitorRect.left,s_monitorRect.top,MONITOR_RES_X,MONITOR_RES_Y,SWP_SHOWWINDOW);

   EhsTgtMainWindowHandle = hWnd;
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);
/* Do the OSD grahics layer */
   SetWindowPos(hEhsOsdLayerWnd,HWND_TOPMOST,s_monitorRect.left,s_monitorRect.top,MONITOR_RES_X,MONITOR_RES_Y,SWP_SHOWWINDOW);
   ShowWindow(hEhsOsdLayerWnd, nCmdShow);
   UpdateWindow(hEhsOsdLayerWnd);
#endif

   return TRUE;
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
	PAINTSTRUCT ps; /*Windows GDI The PAINTSTRUCT structure contains information for an application. This information can be used to paint the client area of a window owned by that application.*/
	HDC hdc; /* display device context for painting, also found in ps.hdc */
	RECT r; /* rectangle storage struct, contains r.top r.bottom r.left r.right*/
	static int statusParts[] = {300,400,-1};
	TCHAR DlgText[10000];
	static unsigned int width = 0u;

	switch (message)
	{
    case WM_CREATE:
#ifdef SHOW_STATUSBAR
		EhsTgtStatusbarHandle = CreateWindowEx(
				0L,                              // no extended styles
				STATUSCLASSNAME,                 // status bar
				_T(""),                              // no text
				WS_CHILD | WS_VISIBLE,  // styles
				-100, -100, 10, 10,              // x, y, cx, cy
				hWnd,                            // parent window
				(HMENU)100,                      // window ID
				hInst,                           // instance
				NULL);							// window data
		SendMessage(EhsTgtStatusbarHandle,SB_SETPARTS, 3, (LPARAM)statusParts);
		EhsConsoleQueue_push(&EhsTgtConsoleInputQueue, (ehs_uint8*)("?S\n"), 3);
#endif
		break;
	case ID_ERRORMESSAGE:
#ifdef EHS_GUI_SUPPORT
		if (!hMsgLogDialog)
		{
			hMsgLogDialog = CreateDialog(hInst, MAKEINTRESOURCE(IDD_MSGLOG), EhsTgtMainWindowHandle, hMsgLogDialogHandler);
			SetWindowPos(hMsgLogDialog,HWND_TOP,100,100,540,500,SWP_SHOWWINDOW);
			ShowWindow(hMsgLogDialog,SW_SHOWNORMAL);
		}
#endif
		// Code doesn't yet work - something to look at later.
//		/* if we need to make the box wider, */
//		if (width < _tcslen((LPCWSTR)wParam)*400)
//		{
//			width = _tcslen((LPCWSTR)wParam)*400;
//			SendMessage(GetDlgItem(hMsgLogDialog,1001), LB_SETHORIZONTALEXTENT, (WPARAM)width, 0);
//		}
		GetDlgItemText(hMsgLogDialog,1001,DlgText,10000);
		StrCat(DlgText,(LPCWSTR)wParam);
		SetDlgItemText(hMsgLogDialog,1001,DlgText);

		InvalidateRect(hWnd,NULL,TRUE);
		break;
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
		case ID_SCREENSIZE_PAL720X576:
			GetWindowRect(hWnd,&r);
			SetWindowPos(hWnd,HWND_TOP,r.left,r.top,720,576,SWP_SHOWWINDOW);
			break;
		case ID_SCREENSIZE_PAL16:
			GetWindowRect(hWnd,&r);
			SetWindowPos(hWnd,HWND_TOP,r.left,r.top,1024,576,SWP_SHOWWINDOW);
			break;
		case ID_SCREENSIZE_NTSC720X486:
			GetWindowRect(hWnd,&r);
			SetWindowPos(hWnd,HWND_TOP,r.left,r.top,720,486,SWP_SHOWWINDOW);
			break;
		case ID_SCREENSIZE_HD720P:
			GetWindowRect(hWnd,&r);
			SetWindowPos(hWnd,HWND_TOP,r.left,r.top,1280,720,SWP_SHOWWINDOW);
			break;
		case ID_SCREENSIZE_HD1080P:
			GetWindowRect(hWnd,&r);
			SetWindowPos(hWnd,HWND_TOP,r.left,r.top,1920,1080,SWP_SHOWWINDOW);
			break;
		case ID_SCREENSIZE_800X600:
			GetWindowRect(hWnd,&r);
			SetWindowPos(hWnd,HWND_TOP,r.left,r.top,800,600,SWP_SHOWWINDOW);
			break;
		case ID_SCREENSIZE_1024X768:
			GetWindowRect(hWnd,&r);
			SetWindowPos(hWnd,HWND_TOP,r.left,r.top,1024,800,SWP_SHOWWINDOW);
			break;
		case ID_COMMANDS_RUN:
			/* Note: contention with EhsTcpipThread is a possible (but remote) source of problems here */
			EhsConsoleQueue_push(EhsTgtConsoleInputQueueRef, (ehs_uint8*)"r\n", 2);
			break;
		case ID_COMMANDS_STEP:
			/* Note: contention with EhsTcpipThread is a possible (but remote) source of problems here */
			EhsConsoleQueue_push(EhsTgtConsoleInputQueueRef, (ehs_uint8*)"s\n", 2);
			break;
		case ID_COMMANDS_PAUSE:
			/* Note: contention with EhsTcpipThread is a possible (but remote) source of problems here */
			EhsConsoleQueue_push(EhsTgtConsoleInputQueueRef, (ehs_uint8*)"p\n", 2);
			break;
		case ID_DEBUG_ON:
			/* Note: contention with EhsTcpipThread is a possible (but remote) source of problems here */
			EhsConsoleQueue_push(EhsTgtConsoleInputQueueRef, (ehs_uint8*)"=+\n", 3);
			break;
		case ID_DEBUG_OFF:
			/* Note: contention with EhsTcpipThread is a possible (but remote) source of problems here */
			EhsConsoleQueue_push(EhsTgtConsoleInputQueueRef, (ehs_uint8*)"=-\n", 3);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_ERASEBKGND:
		/* Don't do anything with this message - this helps to avoid
		   flicker (which is caused by frequent erase followed by paint */
		return (LRESULT)1;
	case WM_PAINT:
#ifdef EHS_GUI_SUPPORT
		if (hWnd == hEhsOsdLayerWnd) {
			hdc = BeginPaint(hWnd, &ps);
			EhsTV_paint(hWnd,&ps);
			EndPaint(hWnd,&ps);
		} else
#endif
		{ /* this code appears to be creating an initial black background window */
			hdc = BeginPaint(hWnd, &ps); /* [in] hWnd : the callback's window handle. [out] &ps : locally defined. Pointer to the PAINTSTRUCT structure that will receive painting information. hdc : this is equal to &ps->hdc */
			GetClientRect(hWnd,&r); /* sets r.bottom = window height r.right = window width, top and left to 0*/
			FillRect(hdc,&r,(HBRUSH)GetStockObject(BLACK_BRUSH)); /* draw onto our rectangle r on the context hdc. hdc: device context, &r: long pointer to rect of logical coords for fill rectangle,  third arg is a GDI predefined HBRUSH to use to paint*/
			EndPaint(hWnd,&ps);		/* tell GDI that we are finished painting window hWnd with ps */
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
	case WM_SYSCOMMAND:
		if (wParam == 35) {
			/* shutdown on "end" */
			PostQuitMessage(0);
		} else {
			EhsWindowsKbHit((ehs_sint32)wParam);
		}
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
		SetDlgItemText(hDlg,IDC_VERSION_ID,_T(EHS_REVISION_ID));
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

void EhsTgtStatus_send(const char*szText, ehs_uint16 nStatusSection)
{
#ifdef UNICODE
#define BUFFSIZE 100
	TCHAR *buff = (TCHAR*)malloc(sizeof(TCHAR)*(strlen(szText)+3));
	mbstowcs(buff,szText,strlen(szText)+1);
#ifdef SHOW_STATUSBAR
	SendMessageA(     // returns LRESULT in lResult
        EhsTgtStatusbarHandle,        // handle to destination control
        SB_SETTEXT,         // message ID
		nStatusSection,
		(LPARAM)buff);
		//(LPARAM)szText);
#endif
	if (nStatusSection == 0)
	{
		StrCat(buff,_T("\r\n"));
		FILE* ferr = fopen("ehs_err.txt","a");
		if (ferr) {
			fprintf(ferr,"%s",szText);
			fclose(ferr);
		} else {
			/* Use PostMessage, rather than SendMessage, otherwise error messages sent
			 * when we have EhsTPMutex_viewport locked might result in a deadlock --
			 * if the windows event handler is currently doing a paint operation */
#ifdef EHS_GUI_SUPPORT
			PostMessage(EhsTgtMainWindowHandle, ID_ERRORMESSAGE,(WPARAM)buff,0);
#endif
		}
	}
#else
#ifdef SHOW_STATUSBAR

	SendMessage(     // returns LRESULT in lResult
        EhsTgtStatusbarHandle,        // handle to destination control
        SB_SETTEXT,         // message ID
		nStatusSection,
		//(LPARAM)buff);
		(LPARAM)szText);
#endif
	free(buff);
#endif /* UNICODE */
}

/**
 * Output the current state of EHS to the toolbar
 */
void EhsTgtStatus_state(const char*szState)
{
	EhsTgtStatus_send(szState,1);
}

/**
 * Dialog message handler
 */
INT_PTR CALLBACK hMsgLogDialogHandler(HWND hWndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	INT_PTR ret  = FALSE;
	switch(uMsg)
	{
	case WM_INITDIALOG:
		ret = TRUE;
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
		case IDCANCEL:
			DestroyWindow(hWndDlg);
			hMsgLogDialog = NULL;
			ret = TRUE;
			break;
		}
	}


	return ret;
}

/**
 * Output the an error message to the toolbar
 */
void EhsTgtStatus_error(const char*szError)
{
	EhsTgtStatus_send(szError,0);
}

