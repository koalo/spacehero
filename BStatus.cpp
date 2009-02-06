#include "BStatus.h"
#include "local.h"

void BStatus::clearStatus()
{
  m_startSimulation = 0;
  m_breakSimulation = 0;
  m_breakIntro = 0;
  m_exit = 0;
}

void BStatus::smallHole(BStatus &state)
{
  state.m_holeWeight = HOLESMALLMASS;
}

void BStatus::mediumHole(BStatus &state)
{
  state.m_holeWeight = HOLEMEDIUMMASS;
}

void BStatus::largeHole(BStatus &state)
{
  state.m_holeWeight = HOLELARGEMASS;
}

void BStatus::startSimulation(BStatus &state)
{
  state.m_startSimulation = 1;
}

void BStatus::breakSimulation(BStatus &state)
{
  state.m_breakSimulation = 1;
}

void BStatus::replaySimulation(BStatus &state)
{
  state.m_replaySimulation = 1;
}

void BStatus::breakIntro(BStatus &state)
{
  state.m_breakIntro = 1;
}

void BStatus::exit(BStatus &state)
{
  state.m_exit = 1;
}

