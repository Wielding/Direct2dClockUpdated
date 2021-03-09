#pragma once

#include <d2d1.h>
#include <atlbase.h>
#include <cassert>

class Scene
{
protected:

	// D2D Resources
	CComPtr<ID2D1Factory>           m_pFactory;
	CComPtr<ID2D1HwndRenderTarget>  m_pRenderTarget;

	float   m_fScaleX;
	float   m_fScaleY;

	// Derived class must implement these methods.
	virtual HRESULT CreateDeviceIndependentResources() = 0;
	virtual void    DiscardDeviceIndependentResources() = 0;
	virtual HRESULT CreateDeviceDependentResources() = 0;
	virtual void    DiscardDeviceDependentResources() = 0;
	virtual void    CalculateLayout() = 0;
	virtual void    RenderScene() = 0;

	HRESULT CreateGraphicsResources(HWND hWnd)
	{
		HRESULT hr = S_OK;
		if (m_pRenderTarget == nullptr)
		{
			RECT rc;
			GetClientRect(hWnd, &rc);

			D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

			hr = m_pFactory->CreateHwndRenderTarget(
				D2D1::RenderTargetProperties(),
				D2D1::HwndRenderTargetProperties(hWnd, size),
				&m_pRenderTarget
			);

			if (SUCCEEDED(hr))
			{
				hr = CreateDeviceDependentResources();
			}
			if (SUCCEEDED(hr))
			{
				CalculateLayout();
			}
		}
		return hr;
	}

	template <class T>
	T PixelToDipX(T pixels) const
	{
		return static_cast<T>(pixels / m_fScaleX);
	}

	template <class T>
	T PixelToDipY(T pixels) const
	{
		return static_cast<T>(pixels / m_fScaleY);
	}

public:
	Scene() : m_fScaleX(1.0f), m_fScaleY(1.0f) {}
	virtual ~Scene() = default;

	HRESULT Initialize()
	{
		HRESULT hr = D2D1CreateFactory(
			D2D1_FACTORY_TYPE_SINGLE_THREADED,
			&m_pFactory
		);

		if (SUCCEEDED(hr))
		{
			CreateDeviceIndependentResources();
		}
		return hr;
	}

	void Render(HWND hWnd)
	{
		HRESULT hr = CreateGraphicsResources(hWnd);
		if (FAILED(hr))
		{
			return;
		}

		assert(m_pRenderTarget != nullptr);

		m_pRenderTarget->BeginDraw();

		RenderScene();

		hr = m_pRenderTarget->EndDraw();
		if (hr == D2DERR_RECREATE_TARGET)
		{
			DiscardDeviceDependentResources();
			m_pRenderTarget.Release();
		}
	}

	HRESULT Resize(const int x, const int y)
	{
		HRESULT hr = S_OK;
		if (m_pRenderTarget)
		{
			hr = m_pRenderTarget->Resize(D2D1::SizeU(x, y));
			if (SUCCEEDED(hr))
			{
				CalculateLayout();
			}
		}
		return hr;
	}

	void CleanUp()
	{
		DiscardDeviceDependentResources();
		DiscardDeviceIndependentResources();
	}
};
