#pragma once

template <class DerivedType> 
class BaseWindow
{
public:
    static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        DerivedType *pThis;

        if (uMsg == WM_NCCREATE)
        {
	        auto* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
            pThis = static_cast<DerivedType*>(pCreate->lpCreateParams);
            SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));

            pThis->m_hWnd = hWnd;
        }
        else
        {
            pThis = reinterpret_cast<DerivedType*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
        }
        if (pThis)
        {
            return pThis->HandleMessage(uMsg, wParam, lParam);
        }
    	
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }

    BaseWindow() : m_hWnd(nullptr) { }

    BOOL Create(
        PCWSTR lpWindowName,
        DWORD dwStyle,
        DWORD dwExStyle = 0,
        int x = CW_USEDEFAULT,
        int y = CW_USEDEFAULT,
        int nWidth = CW_USEDEFAULT,
        int nHeight = CW_USEDEFAULT,
        HWND hWndParent = nullptr,
        HMENU hMenu = nullptr
        )
    {
        WNDCLASSEXW wc{};

        wc.cbSize = sizeof(WNDCLASSEX);
        wc.lpfnWndProc   = DerivedType::WindowProc;
        wc.hInstance     = GetModuleHandle(nullptr);
        wc.lpszClassName = ClassName();

    	// Override AmendWindowClass in your derived class to add app specific
    	// resources to the window class (e.g. icons and menu).
        AmendWindowClass(&wc);

        RegisterClassEx(&wc);

        m_hWnd = CreateWindowEx(
            dwExStyle, wc.lpszClassName, lpWindowName, dwStyle, x, y,
            nWidth, nHeight, hWndParent, hMenu, GetModuleHandle(nullptr), this
            );

        return m_hWnd ? TRUE : FALSE;
    }

    HWND Handle() const { return m_hWnd; }

protected:
    HWND m_hWnd;
	
    ~BaseWindow() = default;

    virtual PCWSTR  ClassName() const = 0;
    virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;
    virtual void AmendWindowClass(WNDCLASSEXW* wc) {}
};