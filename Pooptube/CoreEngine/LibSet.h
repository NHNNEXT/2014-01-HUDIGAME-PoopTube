
#pragma once

#pragma comment(lib, "winmm.lib") // timeGetTime ����� �ʿ������ �Ƹ� chrono�� �ٲٸ� �����սô�

// d3d
#pragma comment(lib, "C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Lib\x86\d3d9.lib")

#ifdef _DEBUG
#pragma comment(lib, "C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Lib\x86\d3dx9d.lib")
#pragma comment(lib, "C:\Program Files\Autodesk\FBX\FBX SDK\2014.2.1\lib\vs2012\x86\debug\libfbxsdk.lib")
#else
#pragma comment(lib, "C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Lib\x86\d3dx9.lib")
#pragma comment(lib, "C:\Program Files\Autodesk\FBX\FBX SDK\2014.2.1\lib\vs2012\x86\release\libfbxsdk.lib")
#endif