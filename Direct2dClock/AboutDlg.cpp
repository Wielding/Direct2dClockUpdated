#include "AboutDlg.h"
#include "resource.h"

#include <string>

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
	{
		wchar_t moduleFileName[MAX_PATH];
		GetModuleFileNameW(GetModuleHandle(nullptr), moduleFileName, MAX_PATH);
		DWORD ptr;

		const auto versionInfoSize = GetFileVersionInfoSize(moduleFileName, &ptr);
		auto* const versionInfo = new char[versionInfoSize];

		GetFileVersionInfo
		(
			moduleFileName,
			0,
			versionInfoSize,
			versionInfo
		);

		UINT pBlockSize;
		LPVOID pVersionBlock = nullptr;

		VerQueryValue(versionInfo, L"\\StringFileInfo\\040904b0\\FileVersion", &pVersionBlock, &pBlockSize);
		std::wstring versionText = static_cast<const wchar_t*>(pVersionBlock);
			
		delete[] versionInfo;

		HWND staticTextHandle = GetDlgItem(hDlg, IDC_VERSION_TEXT);
		versionText = L"Version " + versionText;
		SetWindowText(staticTextHandle, versionText.c_str());
	}

	return static_cast<INT_PTR>(TRUE);

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return static_cast<INT_PTR>(TRUE);
		}
		break;
	default:
		return DefWindowProc(hDlg, message, wParam, lParam);
	}
	return static_cast<INT_PTR>(FALSE);
}