#include "stdafx.h"

using namespace System;
using namespace System::Reflection;
using namespace System::Runtime::CompilerServices;
using namespace System::Runtime::InteropServices;
using namespace System::Security::Permissions;

//
// ������� �Ϲ� ������ ���� Ư�� ������ ���� ����˴ϴ�.
// ������� ���õ� ������ �����Ϸ���
// �� Ư�� ���� �����Ͻʽÿ�.
//
[assembly:AssemblyTitleAttribute(L"CLICore")];
[assembly:AssemblyDescriptionAttribute(L"")];
[assembly:AssemblyConfigurationAttribute(L"")];
[assembly:AssemblyCompanyAttribute(L"")];
[assembly:AssemblyProductAttribute(L"CLICore")];
[assembly:AssemblyCopyrightAttribute(L"Copyright (c)  2014")];
[assembly:AssemblyTrademarkAttribute(L"")];
[assembly:AssemblyCultureAttribute(L"")];

//
// ������� ���� ������ ���� �� ���� ������ �����˴ϴ�.
//
//      �� ����
//      �� ����
//      ���� ��ȣ
//      ���� ����
//
// ��� ���� �����ϰų� �Ʒ��� ���� '*'�� ����Ͽ� ���� ��ȣ �� ���� ������ �ڵ�����
// �����ǵ��� �� �� �ֽ��ϴ�.

[assembly:AssemblyVersionAttribute("1.0.*")];

[assembly:ComVisible(false)];

[assembly:CLSCompliantAttribute(true)];