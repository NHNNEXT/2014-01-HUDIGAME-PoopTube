
#pragma once

#pragma comment(lib, "winmm.lib") // timeGetTime ����� �ʿ������ �Ƹ� chrono�� �ٲٸ� �����սô�

// d3d
#pragma comment(lib, "d3d9.lib")
#ifdef _DEBUG
#pragma comment(lib, "d3dx9d.lib")
#else
#pragma comment(lib, "d3dx9.lib")
#endif