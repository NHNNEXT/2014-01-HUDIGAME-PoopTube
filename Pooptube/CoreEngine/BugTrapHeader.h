#pragma once

#include <tchar.h>
#include <eh.h>                 // include set_terminate() declaration

// BugTrap includes //////////////////////////////////////////////////////////////

// Include main BugTrap header.
#include "..\Library\BugTrap\BugTrap.h"
#include "stdafx.h"

// Link with one of BugTrap libraries.
#if defined _M_IX86
#ifdef _UNICODE
#pragma comment(lib, "BugTrapU.lib")
#else
#pragma comment(lib, "BugTrap.lib")
#endif
#elif defined _M_X64
#ifdef _UNICODE
#pragma comment(lib, "BugTrapU-x64.lib")
#else
#pragma comment(lib, "BugTrap-x64.lib")
#endif
#else
#error CPU architecture is not supported.
#endif

// Enable Common Controls support

#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif


INT_PTR g_iLogHandle = -1;

static void SetupExceptionHandler()
{
	// Setup exception handler
	BT_SetAppName( _T( "PoopTube Error Report" ) );
	BT_SetSupportEMail( _T( "your@email.com" ) );
	BT_SetFlags( BTF_DETAILEDMODE | BTF_ATTACHREPORT );

	// = BugTrapServer ===========================================
	BT_SetSupportServer( _T( "localhost" ), 9999 );
	// - or -
	//BT_SetSupportServer(_T("127.0.0.1"), 9999);

	// = BugTrapWebServer ========================================
	//BT_SetSupportServer(_T("http://localhost/BugTrapWebServer/RequestHandler.aspx"), BUGTRAP_HTTP_PORT);

	// required for VS 2005 & 2008
	BT_InstallSehFilter();

	// Add custom log file using default name
	g_iLogHandle = BT_OpenLogFile( NULL, BTLF_TEXT );
	BT_SetLogSizeInEntries( g_iLogHandle, 100 );
	BT_SetLogFlags( g_iLogHandle, BTLF_SHOWTIMESTAMP );
	BT_SetLogEchoMode( g_iLogHandle, BTLE_STDERR | BTLE_DBGOUT );

	PCTSTR pszLogFileName = BT_GetLogFileName( g_iLogHandle );
	BT_AddLogFile( pszLogFileName );
}