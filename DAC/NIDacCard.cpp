#include "NIDacCard.h"
#include <NIDAQmx.h>
#include "DAC_logger.h"

#define DAQmxErrChk(functionCall) if( DAQmxFailed(error=(functionCall)) ) goto Error; else

NIDacCard::NIDacCard()
{
}

NIDacCard::NIDacCard(DacCalibration* pDacCali)
{
    setNIDacCard(pDacCali);
}

void NIDacCard::setNIDacCard(DacCalibration* pDacCali)
{
	m_pDacCali = pDacCali;
}

NIDacCard::~NIDacCard()
{
	if (m_bIsOpen == true)
		close();
}

void NIDacCard::close()
{
    DAQmxStopTask(m_taskHandleAout);
    DAQmxClearTask(m_taskHandleAout);
    m_bIsOpen = false;
}

bool NIDacCard::isOpen()
{ 
	return m_bIsOpen; 
}

bool NIDacCard::isActive()
{
    if(m_taskHandleAout)
        return (bool)DAQmxWaitUntilTaskDone(m_taskHandleAout,0);
    else
        return false;
}

bool NIDacCard::setSafePosition()
{
    int32 error;
    if(m_bIsOpen)
        close();
    
    DAQmxErrChk (DAQmxCreateTask("NIDAC0",&m_taskHandleAout));
    DAQmxErrChk (DAQmxCreateAOVoltageChan(m_taskHandleAout,m_pDacCali->m_sXChanName.c_str(),"Ben1",-10.0,10.0,DAQmx_Val_Volts,NULL));
    DAQmxErrChk (DAQmxWriteAnalogScalarF64(m_taskHandleAout,1,10.0, -m_pDacCali->m_fXMaxV,NULL));
    DAQmxClearTask(m_taskHandleAout);
    
    DAQmxErrChk (DAQmxCreateTask("NIDAC0",&m_taskHandleAout));
    DAQmxErrChk (DAQmxCreateAOVoltageChan(m_taskHandleAout,m_pDacCali->m_sYChanName.c_str(),"Ben2",-10.0,10.0,DAQmx_Val_Volts,NULL));
    DAQmxErrChk (DAQmxWriteAnalogScalarF64(m_taskHandleAout,1,10.0,-m_pDacCali->m_fXMaxV,NULL));
    DAQmxClearTask(m_taskHandleAout);
    
    DAQmxErrChk (DAQmxCreateTask("NIDAC0",&m_taskHandleAout));
    DAQmxErrChk (DAQmxCreateAOVoltageChan(m_taskHandleAout,m_pDacCali->m_sSwitchChanName.c_str(),"Ben3",-10.0,10.0,DAQmx_Val_Volts,NULL));
    DAQmxErrChk (DAQmxWriteAnalogScalarF64(m_taskHandleAout,1,10.0,0,NULL));
    DAQmxClearTask(m_taskHandleAout);
    
	DAC_LOGGER_INFO << __FUNCTION__ << "\t\t NIDacCard setSafePosition done.";
    return true;
Error:
    if( DAQmxFailed(error) )
    {
        DAQmxGetExtendedErrorInfo(m_errBuff,2048);
    }
	DAC_LOGGER_ERROR << __FUNCTION__<<"\t\t NIDacCard setSafePosition error";
	DAC_LOGGER_ERROR << __FUNCTION__<<"\t\t " << m_errBuff;
	//throw std::invalid_argument("NIDacCard setSafePosition error");
	throw std::invalid_argument(m_errBuff);
	return false;
}

bool NIDacCard::setTrajectory(Trajectory* const traj)
{
	bool re = initCard(traj->getTrajactoryLength());
	re = re && createTraj(traj);
	return re;
}

bool NIDacCard::triggerTrajectory()
{
    int32 error;
    DAQmxErrChk (DAQmxStartTask(m_taskHandleAout));
    DAQmxErrChk(DAQmxWaitUntilTaskDone (m_taskHandleAout, -1));//-1 = indefinite

	return true;
Error:
    if( DAQmxFailed(error) )
	{
        DAQmxGetExtendedErrorInfo(m_errBuff,2048);
	}
	DAC_LOGGER_ERROR << __FUNCTION__<<"\t\t NIDacCard triggerTrajectory error";
	DAC_LOGGER_ERROR << __FUNCTION__<<"\t\t " << m_errBuff;
	//throw std::invalid_argument("NIDacCard triggerTrajectory error");
	throw std::invalid_argument(m_errBuff);
	return false;
}

bool NIDacCard::initCard(size_t trajLength)
{
    int32 error;
    if (m_bIsOpen)
        close();
    std::string dacClock = "OnboardClock"; // default internally clocked
	unsigned long sampleRate = m_pDacCali->m_ulTrajGenRate;

    DAQmxErrChk (DAQmxCreateTask("NIDAC0",&m_taskHandleAout));
	//DAQmxErrChk (DAQmxCreateAOVoltageChan(m_taskHandleAout,m_pDacCali->m_sSwitchChanName.c_str(),"Ben0",-10.0,10.0,DAQmx_Val_Volts,NULL));//Dev1/ao0
    //DAQmxErrChk (DAQmxCreateAOVoltageChan(m_taskHandleAout,m_pDacCali->m_sXChanName.c_str(),"Ben1",-10.0,10.0,DAQmx_Val_Volts,NULL));//Dev1/ao1
    //DAQmxErrChk (DAQmxCreateAOVoltageChan(m_taskHandleAout,m_pDacCali->m_sYChanName.c_str(),"Ben2",-10.0,10.0,DAQmx_Val_Volts,NULL));//Dev1/ao2
    
	DAQmxErrChk (DAQmxCreateAOVoltageChan(m_taskHandleAout,m_pDacCali->m_sSwitchChanName.c_str(),"Ben0",-m_pDacCali->m_fXMaxV,m_pDacCali->m_fXMaxV,DAQmx_Val_Volts,NULL));//Dev1/ao0
	DAQmxErrChk (DAQmxCreateAOVoltageChan(m_taskHandleAout,m_pDacCali->m_sXChanName.c_str(),"Ben1",-m_pDacCali->m_fYMaxV,m_pDacCali->m_fYMaxV,DAQmx_Val_Volts,NULL));//Dev1/ao1
	DAQmxErrChk (DAQmxCreateAOVoltageChan(m_taskHandleAout,m_pDacCali->m_sYChanName.c_str(),"Ben2",-m_pDacCali->m_fXMaxV,m_pDacCali->m_fXMaxV,DAQmx_Val_Volts,NULL));//Dev1/ao2
	

    if(m_pDacCali->m_bExterClked) // if externally clocked
    {
        dacClock = m_pDacCali->m_sExterClkChanName;
        sampleRate *= 8;
    }
    
    //DAQmxErrChk (DAQmxCfgSampClkTiming(m_taskHandleAout,dacClock.c_str(),sampleRate,DAQmx_Val_Rising,DAQmx_Val_FiniteSamps,trajLength+NUM_INITIAL_SAMPLES+NUM_FINAL_SAMPLES));
    DAQmxErrChk (DAQmxCfgSampClkTiming(m_taskHandleAout,dacClock.c_str(),sampleRate,DAQmx_Val_Rising,DAQmx_Val_FiniteSamps,trajLength));
    
    //"Configures the task to start acquiring or generating samples immediately upon starting the task" NI website
    DAQmxErrChk (DAQmxDisableStartTrig(m_taskHandleAout));
    m_bIsOpen = true;
	DAC_LOGGER_INFO << __FUNCTION__ << "\t\t NIDacCard initCard done.";
    return true;
Error:
    if( DAQmxFailed(error) )
    {
        DAQmxGetExtendedErrorInfo(m_errBuff,2048);
    }
	DAC_LOGGER_ERROR << __FUNCTION__<<"\t\t NIDacCard initCard error";
	DAC_LOGGER_ERROR << __FUNCTION__<<"\t\t " << m_errBuff;
	//throw std::invalid_argument("NIDacCard initCard error");
	throw std::invalid_argument(m_errBuff);
	return false;
}

bool NIDacCard::createTraj(Trajectory* const traj)
{
    int32 error;
    std::vector<double> vVals;
    vVals.clear();
	size_t traj_len = traj->getTrajactoryLength();
    vVals.resize(traj_len*3);
    
    static int32 ulSamplesWritten =0;
    ulSamplesWritten = 0;
    
    // output of channel 0, SwitchChanName "Ben0" @Brian
    float ch0Val =0.0;

	float xmax = m_pDacCali->m_fXMaxV;
	float ymax = m_pDacCali->m_fYMaxV;

	std::vector<TrajectoryPoint_t> trajPoints = traj->getTrajactoryPoints();
	TrajectoryPoint_t traj_point;
	
    for(int i = 0; i < traj_len;i++)
    {
		traj_point = trajPoints[i];
        ch0Val = 0.0;

		if ( traj_point.type == TP_ACQUIRE )
            ch0Val = 5.0;
        else
            ch0Val = 0.0;
        
		vVals[3*i] = ch0Val;
		vVals[3*i + 1] = traj_point.x;
		vVals[3*i + 2] = traj_point.y;
        
		if (traj_point.x >= xmax || traj_point.y >= ymax)
		{
			throw std::invalid_argument("X Y output signal should smaller than Max");
		}
		
		//assert(traj_point.x < xmax);
		//assert(traj_point.y < ymax);
    }

    //****************** set SwitchChanName done @brian************************//
    
    DAQmxErrChk (DAQmxWriteAnalogF64(m_taskHandleAout,vVals.size()/3,0,100.0, DAQmx_Val_GroupByScanNumber,&vVals[0],&ulSamplesWritten,NULL));
    
	DAC_LOGGER_INFO << __FUNCTION__<<"\t\t NIDacCard create Trajectory done.";
    return true;
    
Error:
    if( DAQmxFailed(error) )
    {
        DAQmxGetExtendedErrorInfo(m_errBuff,2048);
    }
	DAC_LOGGER_ERROR << __FUNCTION__<<"\t\t NIDacCard createTraj error";
	DAC_LOGGER_ERROR << __FUNCTION__<<"\t\t " << m_errBuff;
	//throw std::invalid_argument("NIDacCard createTraj error");
	throw std::invalid_argument(m_errBuff);
	return false;
}


const std::string NIDacCard::getErrorMessage() 
{ 
	return m_errBuff; 
}
