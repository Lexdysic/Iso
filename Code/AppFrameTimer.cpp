
#include "AppPch.h"

//=============================================================================
CFrameTimer::CFrameTimer () :
    m_currentAverage(0.0f)
{

}

//=============================================================================
CFrameTimer::~CFrameTimer ()
{

}

//=============================================================================
void CFrameTimer::Update ()
{
    // Pop
    if (m_values.Count() > N)
    {
        const Time::Delta lastValue = m_values[-1];
        m_currentAverage -= lastValue / m_values.Count();
        m_values.RemoveBack();
    }

    // Push
    {
        const Time::Delta frameTime = m_timer.Reset();
        m_values.AddFront(frameTime);
        m_currentAverage += frameTime / m_values.Count();
    }

    // TODO: the cumulative moving average wasnt working for some reason, so recalculating from the source
    m_currentAverage = std::accumulate(m_values.begin(), m_values.end(), Time::Delta()) / m_values.Count();
}
