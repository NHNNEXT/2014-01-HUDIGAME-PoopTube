
#pragma once

#pragma comment(lib, "winmm.lib") // timeGetTime 지우면 필요없을걸 아마 chrono로 바꾸면 삭제합시다

// d3d
#pragma comment(lib, "d3d9.lib")
#ifdef _DEBUG
#pragma comment(lib, "d3dx9d.lib")
#else
#pragma comment(lib, "d3dx9.lib")
#endif