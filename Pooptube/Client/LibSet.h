﻿#pragma once

// d3d
#pragma comment(lib, "d3d9.lib")
//#pragma comment(lib, "C:\\Program Files (x86)\\Microsoft DirectX SDK (June 2010)\\Lib\\x86\\d3d9.lib")

#ifdef _DEBUG
#pragma comment(lib, "d3dx9d.lib")
//#pragma comment(lib, "C:\\Program Files (x86)\\Microsoft DirectX SDK (June 2010)\\Lib\\x86\\d3dx9d.lib")

#else
#pragma comment(lib, "d3dx9.lib")
//#pragma comment(lib, "C:\\Program Files (x86)\\Microsoft DirectX SDK (June 2010)\\Lib\\x86\\d3dx9.lib")

#endif