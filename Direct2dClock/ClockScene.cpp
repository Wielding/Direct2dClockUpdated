#include "ClockScene.h"

HRESULT ClockScene::CreateDeviceDependentResources()
{
    HRESULT hr = m_pRenderTarget->CreateSolidColorBrush(
        D2D1::ColorF(1.0f, 1.0f, 0),
        D2D1::BrushProperties(),
        &m_pFill
    );

    if (SUCCEEDED(hr))
    {
        hr = m_pRenderTarget->CreateSolidColorBrush(
            D2D1::ColorF(0, 0, 0),
            D2D1::BrushProperties(),
            &m_pStroke
        );
    }
    return hr;
}


void ClockScene::DrawClockHand(float fHandLength, float fAngle, float fStrokeWidth)
{
    m_pRenderTarget->SetTransform(
        D2D1::Matrix3x2F::Rotation(fAngle, m_stEllipse.point)
    );

    // endPoint defines one end of the hand.
    D2D_POINT_2F endPoint = D2D1::Point2F(
        m_stEllipse.point.x,
        m_stEllipse.point.y - (m_stEllipse.radiusY * fHandLength)
    );

    // Draw a line from the center of the ellipse to endPoint.
    m_pRenderTarget->DrawLine(
        m_stEllipse.point, endPoint, m_pStroke, fStrokeWidth);
}


void ClockScene::RenderScene()
{
    m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::SkyBlue));

    m_pRenderTarget->FillEllipse(m_stEllipse, m_pFill);
    m_pRenderTarget->DrawEllipse(m_stEllipse, m_pStroke);

    // Draw tick marks
    for (DWORD i = 0; i < 12; i++)
    {
        m_pRenderTarget->DrawLine(m_stTics[i * 2], m_stTics[i * 2 + 1], m_pStroke, 2.0f);
    }

    // Draw hands
    SYSTEMTIME time;
    GetLocalTime(&time);

    // 60 minutes = 30 degrees, 1 minute = 0.5 degree
    const float fHourAngle = (360.0f / 12) * (time.wHour) + (time.wMinute * 0.5f);
    const float fMinuteAngle = (360.0f / 60) * (time.wMinute);

    const float fSecondAngle =
        (360.0f / 60) * time.wSecond + 360.0f / 60000 * (time.wMilliseconds);

    DrawClockHand(0.6f, fHourAngle, 6);
    DrawClockHand(0.85f, fMinuteAngle, 4);
    DrawClockHand(0.85f, fSecondAngle, 1);

    // Restore the identity transformation.
    m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
}

void ClockScene::CalculateLayout()
{
    D2D1_SIZE_F fSize = m_pRenderTarget->GetSize();

    const float x = fSize.width / 2.0f;
    const float y = fSize.height / 2.0f;
    const float radius = min(x, y);

    m_stEllipse = D2D1::Ellipse(D2D1::Point2F(x, y), radius, radius);

    // Calculate tick marks.

    D2D_POINT_2F pt1 = D2D1::Point2F(
        m_stEllipse.point.x,
        m_stEllipse.point.y - (m_stEllipse.radiusY * 0.9f)
    );

    D2D_POINT_2F pt2 = D2D1::Point2F(
        m_stEllipse.point.x,
        m_stEllipse.point.y - m_stEllipse.radiusY
    );

    for (DWORD i = 0; i < 12; i++)
    {
        D2D1::Matrix3x2F mat = D2D1::Matrix3x2F::Rotation(
            (360.0f / 12) * i, m_stEllipse.point);

        m_stTics[i * 2] = mat.TransformPoint(pt1);
        m_stTics[i * 2 + 1] = mat.TransformPoint(pt2);
    }
}


void ClockScene::DiscardDeviceDependentResources()
{
    m_pFill.Release();
    m_pStroke.Release();
}