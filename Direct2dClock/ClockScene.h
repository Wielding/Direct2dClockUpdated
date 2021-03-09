#pragma once

#include <atlbase.h>
#include <d2d1.h>
#include "Scene.h"

class ClockScene : public Scene
{
    CComPtr<ID2D1SolidColorBrush> m_pFill;
    CComPtr<ID2D1SolidColorBrush> m_pStroke;

    D2D1_ELLIPSE          m_stEllipse{};
    D2D_POINT_2F          m_stTics[24]{{0, 0}};

    HRESULT CreateDeviceIndependentResources() override { return S_OK; }
    void    DiscardDeviceIndependentResources() override { }
    HRESULT CreateDeviceDependentResources() override;
    void    DiscardDeviceDependentResources() override;
    void    CalculateLayout() override;
    void    RenderScene() override;

    void    DrawClockHand(float fHandLength, float fAngle, float fStrokeWidth);
};