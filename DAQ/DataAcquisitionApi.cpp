#include "DataAcquisitionApi.h"
#include "DAQ_logger.h"

DataAcquisitionApi::DataAcquisitionApi(void)
{
	m_pDaq = new DataAcquisition();
	m_pDaqCali = new DaqCalibration();
}


DataAcquisitionApi::~DataAcquisitionApi(void)
{
	delete m_pDaq; m_pDaq = NULL;
	delete m_pDaqCali; m_pDaqCali = NULL;
}

void DataAcquisitionApi::generateDaq()
{
	//m_pDaq = new DataAcquisition(m_pDaqCali);
	m_pDaq->setDaqCalibration(m_pDaqCali);
}

void DataAcquisitionApi::setDaqCalibration(uInt_32 uiNumPixel, uInt_32 uiNumAscan, uInt_32 uiNumBscan, uInt_32 uiNumBscanRepeat)
{
	//m_pDaqCali = new DaqCalibration(uiNumPixel, uiNumAscan, uiNumBscan);
	m_pDaqCali->setDaqCalibration(uiNumPixel, uiNumAscan, uiNumBscan, uiNumBscanRepeat);
}
void DataAcquisitionApi::startDataAcquire()
{
	DAQ_LOGGER_TRACE << __FUNCTION__;
	generateDaq();
	DAQ_LOGGER_TRACE << __FUNCTION__;
	m_pDaq->startAcquire();
}
void DataAcquisitionApi::stopDataAcquire()
{
	m_pDaq->stopAcquire();
}
void DataAcquisitionApi::copyData(DataBuffer* pBuf)
{
	m_pDaq->copyData(pBuf);
}