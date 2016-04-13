#pragma once

#include "DaqCalibration.h"
#include "DataBuffer.h"

#define INTERATION 1
class DataAcquisition
{
private:
    void* m_pSession;
    void* m_pMemory;
    unsigned long m_ulAscanList[2000];
    DaqCalibration* m_pDaqCali;
    DataBuffer* m_pDataBuf;
    
    void acquireLoop();
    
public:
	DataAcquisition();
    DataAcquisition(DaqCalibration* daqCalibration);
    ~DataAcquisition();
	void setDaqCalibration(DaqCalibration* daqCalibration);
    void startAcquire();
    void stopAcquire();
    void copyData(DataBuffer* pBuf);

};
