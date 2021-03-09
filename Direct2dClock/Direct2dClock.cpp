#include <Windows.h>
#include <d2d1.h>
#include <cassert>
#include <atlbase.h>
#include <string>

#include "Direct2DClock.h"
#include "AboutDlg.h"
#include "resource.h"

INT WINAPI wWinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine,
	_In_ int nShowCmd
)
{
	if (FAILED(CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE)))
	{
		return 0;
	}

	wchar_t szWindowName[MAX_LOAD_STRING];

	LoadStringW(hInstance, IDS_APP_TITLE, szWindowName, MAX_LOAD_STRING);

	MainWindow win;

	if (!win.Create(szWindowName, WS_OVERLAPPEDWINDOW))
	{
		return 0;
	}

	ShowWindow(win.Handle(), nShowCmd);

	auto* const hAccelerators = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DIRECT2DCLOCK));

	MSG msg = { };

	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelerators, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	CoUninitialize();
	return 0;
}

MainWindow::MainWindow() = default;


LRESULT MainWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	auto* const hWnd = m_hWnd;

	switch (uMsg)
	{
	case WM_CREATE:
		if (FAILED(m_cScene.Initialize()) || !InitializeTimer())
		{
			return -1;
		}
		return 0;

	case WM_DESTROY:
		KillTimer(m_hWnd, m_hTimer);
		m_cScene.CleanUp();
		PostQuitMessage(0);
		return 0;

	case WM_TIMER:
		InvalidateRect(m_hWnd, nullptr, FALSE);
		return 0;

	case WM_PAINT:
	case WM_DISPLAYCHANGE:
	{
		PAINTSTRUCT ps;
		BeginPaint(m_hWnd, &ps);
		m_cScene.Render(m_hWnd);
		EndPaint(m_hWnd, &ps);
	}
	return 0;

	case WM_SIZE:
	{
		const auto x = static_cast<int>(static_cast<short>(LOWORD(lParam)));
		const auto y = static_cast<int>(static_cast<short>(HIWORD(lParam)));
		m_cScene.Resize(x, y);
		InvalidateRect(m_hWnd, nullptr, FALSE);
	}
	return 0;

	case WM_ERASEBKGND:
		return 1;

	case WM_COMMAND:
	{
		const int wmId = LOWORD(wParam);

		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(GetModuleHandle(nullptr), MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(m_hWnd);
			break;
		default:
			;
		}

		break;
	}
	default:
		;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

/// <summary>
/// Add app specific icons and a menu
/// </summary>
/// <param name="wc"></param>
void MainWindow::AmendWindowClass(WNDCLASSEXW * wc)
{
	wc->lpszMenuName = MAKEINTRESOURCEW(IDC_DIRECT2DCLOCK);
	wc->hIcon = LoadIcon(wc->hInstance, MAKEINTRESOURCE(IDI_DIRECT2DCLOCK));
	wc->hIconSm = LoadIcon(wc->hInstance, MAKEINTRESOURCE(IDI_SMALL));
}

BOOL MainWindow::InitializeTimer()
{
	m_hTimer = SetTimer(
		m_hWnd,
		m_uClockTimerId,
		(1000 / 60),
		nullptr
	);

	return TRUE;
}

/// <summary>
/// Retrieve and return a pointer to the window class name stored in a string resource
/// </summary>
/// <returns></returns>
PCWSTR MainWindow::ClassName() const
{
	LoadStringW(
		GetModuleHandle(nullptr),
		IDC_DIRECT2DCLOCK,
		const_cast<LPWSTR>(m_szMainWindowClass),
		MAX_LOAD_STRING
	);

	return m_szMainWindowClass;
}

