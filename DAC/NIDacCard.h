#pragma once

#ifdef NIDACCARDDLL_EXPORTS
#define NIDACCARDDLL __declspec(dllexport) 
#else
#define NIDACCARDDLL __declspec(dllimport) 
#endif

#include <string>
#include <NIDAQmx.h>
#include "Trajectory.h"
#include "DacCalibration.h"

#define NUM_INITIAL_SAMPLES 0
#define NUM_FINAL_SAMPLES 0

class NIDACCARDDLL NIDacCard
{
private:
    TaskHandle m_taskHandleAout;
    DacCalibration* m_pDacCali;
    char m_errBuff[2048];
    bool m_bIsOpen;
    
    bool initCard(size_t trajLength);
    bool createTraj(Trajectory* const traj);

public:
	NIDacCard();
    NIDacCard(DacCalibration* pDacCali);
    ~NIDacCard();
    void setNIDacCard(DacCalibration* pDacCali);
    void close();
    bool isOpen();
    bool isActive();
    
    bool setSafePosition();
    bool setTrajectory (Trajectory* const traj);
    bool triggerTrajectory();
    const std::string getErrorMessage();
};