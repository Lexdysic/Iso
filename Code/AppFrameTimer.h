class CFrameTimer
{
public:
    CFrameTimer ();
    ~CFrameTimer ();

    void Update ();

    Time::Delta GetAverageFrameTime () const { return m_currentAverage; }
    Time::Delta GetCurrentFrameTime () const { return m_values.front(); }

private:
    static const unsigned N = 64;

    std::deque<Time::Delta>  m_values;
    Time::Delta              m_currentAverage;
    CRealTimer               m_timer;
};