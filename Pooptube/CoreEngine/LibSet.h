
#pragma once

#pragma comment(lib, "winmm.lib") // timeGetTime ����� �ʿ������ �Ƹ� chrono�� �ٲٸ� �����սô�

#pragma comment(lib, "ws2_32.lib") // ���� ���̺귯��

// d3d
//#pragma comment(lib, "C:\\Program Files (x86)\\Microsoft DirectX SDK (June 2010)\\Lib\\x86\\d3d9.lib")
#pragma comment(lib, "d3d9.lib")

#ifdef _DEBUG
//#pragma comment(lib, "C:\\Program Files (x86)\\Microsoft DirectX SDK (June 2010)\\Lib\\x86\\d3dx9d.lib")
#pragma comment(lib, "d3dx9d.lib")
//#pragma comment(lib, "C:\\Program Files\\Autodesk\\FBX\\FBX SDK\\2014.2.1\\lib\\vs2012\\x86\\debug\\libfbxsdk.lib")
#pragma comment(lib, "libfbxsdk.lib")

#else
#pragma comment(lib, "d3dx9.lib")
//#pragma comment(lib, "C:\\Program Files (x86)\\Microsoft DirectX SDK (June 2010)\\Lib\\x86\\d3dx9.lib")
#pragma comment(lib, "libfbxsdk.lib")
//#pragma comment(lib, "C:\\Program Files\\Autodesk\\FBX\\FBX SDK\\2014.2.1\\lib\\vs2012\\x86\\release\\libfbxsdk.lib")

#endif