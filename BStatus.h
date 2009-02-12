#ifndef _BUTTONS_H_
#define _BUTTONS_H_

class BStatus
{
  public:
    int m_startSimulation;
    float m_holeWeight;
    int m_breakSimulation;
    int m_replaySimulation;
    int m_breakIntro;
    int m_exit;

  public:
    void clearStatus();

    static void smallHole(BStatus &);
    static void mediumHole(BStatus &);
    static void largeHole(BStatus &);
    static void startSimulation(BStatus &);
    static void breakSimulation(BStatus &);
    static void replaySimulation(BStatus &);
    static void breakIntro(BStatus &);

    static void exit(BStatus &);
};

#endif
