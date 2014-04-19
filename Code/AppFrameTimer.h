class CFrameTimer
{
public:
    CFrameTimer ();
    ~CFrameTimer ();

    void Update ();

    Time::Delta GetAverageFrameTime () const { return m_currentAverage; }
    Time::Delta GetCurrentFrameTime () const { return m_values[0]; }

private:
    static const unsigned N = 64;

    TQueue<Time::Delta> m_values;
    Time::Delta         m_currentAverage;
    CRealTimer          m_timer;
};