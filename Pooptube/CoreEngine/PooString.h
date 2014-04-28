#include "stdafx.h"

namespace pooptube {

	LPCWSTR MultiCharToUniChar(char* mbString) {
		int len = 0;
		len = (int)strlen(mbString) + 1;
		wchar_t *ucString = new wchar_t[len];
		MultiByteToWideChar(CP_ACP, 0, mbString, -1, ucString, len);
		return (LPCWSTR)ucString;
	}

}

