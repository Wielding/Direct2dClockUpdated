#pragma once
#include <Windows.h>

#include "BaseWindow.h"
#include "ClockScene.h"

constexpr int MAX_LOAD_STRING = 100;

class MainWindow : public BaseWindow<MainWindow>
{
protected:

	const UINT_PTR m_uClockTimerId = 0x100;
	UINT_PTR  m_hTimer{};
	ClockScene m_cScene;
	wchar_t m_szMainWindowClass[MAX_LOAD_STRING]{ 0 };

	void AmendWindowClass(WNDCLASSEXW* wc) override;
	BOOL InitializeTimer();

public:
	MainWindow();
	virtual ~MainWindow() = default;

	PCWSTR ClassName() const override;
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;
};
