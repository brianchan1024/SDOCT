#pragma once

#ifdef DATAACQUIRISITIONAPIDLL_EXPORTS
#define TDATAACQUIRISITIONDLL __declspec(dllexport) 
#else
#define TDATAACQUIRISITIONDLL __declspec(dllimport) 
#endif

#include "common.h"
#include "DaqCalibration.h"
#include "DataAcquisition.h"

class TDATAACQUIRISITIONDLL DataAcquisitionApi
{
private:
	DaqCalibration* m_pDaqCali;
	DataAcquisition* m_pDaq;
	void generateDaq();

public:
	DataAcquisitionApi(void);
	~DataAcquisitionApi(void);
	void setDaqCalibration(uInt_32 uiNumPixel, uInt_32 uiNumAscan, uInt_32 uiNumBscan, uInt_32 uiNumBscanRepeat);
	void startDataAcquire();
    void stopDataAcquire();
    void copyData(DataBuffer* pBuf);
};

