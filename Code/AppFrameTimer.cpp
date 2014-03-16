
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
    if (m_values.size() > N)
    {
        const Time::Delta lastValue = m_values.back();
        m_currentAverage -= lastValue / m_values.size();
        m_values.pop_back();
    }

    // Push
    {
        const Time::Delta frameTime = m_timer.Reset();
        m_values.push_front(frameTime);
        m_currentAverage += frameTime / m_values.size();
    }

    // TODO: the cumulative moving average wasnt working for some reason, so recalculating from the source
    m_currentAverage = std::accumulate(m_values.begin(), m_values.end(), Time::Delta()) / m_values.size();
}
