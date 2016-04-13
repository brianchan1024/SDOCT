#include "DataAcquisition.h"
#include "MAQEngine.h"
#include "DAQ_logger.h"

DataAcquisition::DataAcquisition()
{
}

DataAcquisition::DataAcquisition(DaqCalibration* daqCalibration)
{
    setDaqCalibration(daqCalibration);
}

DataAcquisition::~DataAcquisition()
{   
}

void DataAcquisition::setDaqCalibration(DaqCalibration* daqCalibration)
{
	m_pDaqCali = daqCalibration;
    //m_pSession = InitializeSession("img0", m_pDaqCali->m_uiNumPixels, m_pDaqCali->m_uiNumAscan * m_pDaqCali->m_uiNumBscan);
    m_pSession = InitializeSession("img0", m_pDaqCali->m_uiNumPixels, m_pDaqCali->m_uiNumAscan);
    if (&m_pSession == 0) {
		DAQ_LOGGER_ERROR << __FUNCTION__ << " InitializeSession error";
        throw std::invalid_argument("DataAcquisition InitializeSession error");
    }
    std::fill_n(m_ulAscanList, m_pDaqCali->m_uiNumBscan * (1+ m_pDaqCali->m_uiNumBscanRepeat), m_pDaqCali->m_uiNumAscan);
	DAQ_LOGGER_ERROR << __FUNCTION__ << " InitializeSession done";
}


void DataAcquisition::startAcquire()
{
	DAQ_LOGGER_TRACE << __FUNCTION__;
    if (&m_pSession == 0) {
		m_pSession = InitializeSession("img0", m_pDaqCali->m_uiNumPixels, m_pDaqCali->m_uiNumAscan);
        if (&m_pSession == 0) {
			DAQ_LOGGER_ERROR << __FUNCTION__ << " InitializeSession error";
            throw std::invalid_argument("DataAcquisition InitializeSession error");
        }
    }

	DAQ_LOGGER_DEBUG << __FUNCTION__ << " 0";

    struct InitAcqBlock *ab = (struct InitAcqBlock *)m_pSession;
	try
	{
		m_pMemory = AllocateACQMemory(ab, m_pDaqCali->m_uiNumPixels, m_ulAscanList, m_pDaqCali->m_uiNumBscan * (1+ m_pDaqCali->m_uiNumBscanRepeat), INTERATION);
	} catch (std::exception &ex)
	{
		DAQ_LOGGER_ERROR << __FUNCTION__ << " AllocateACQMemory error";
		throw std::invalid_argument("DataAcquisition AllocateACQMemory error");
	}

	DAQ_LOGGER_DEBUG << __FUNCTION__ << " 1";

	void* image = NULL;
	try
	{
		image = StartGrabImage(m_pMemory);

	} catch (std::exception &ex)
	{
		DAQ_LOGGER_ERROR << __FUNCTION__ << " StartGrabImage error";
		throw std::invalid_argument("DataAcquisition StartGrabImage error");
	}

	DAQ_LOGGER_DEBUG << __FUNCTION__ << " 2";

    if (&image == 0)
    {
        if (!StopGrabImage(m_pMemory) || !DeallocateQueMemory(m_pMemory))
		{
			DAQ_LOGGER_ERROR << __FUNCTION__ << " StopGrabImage or DeallocateQueMemory error";
            throw std::invalid_argument("DataAcquisition StopGrabImage error");
		}
    }

	DAQ_LOGGER_DEBUG << __FUNCTION__ << " 3";
}

void DataAcquisition::stopAcquire()
{
    if (!EndSession(m_pSession)) {
        throw std::invalid_argument("DataAcquisition stopAcquire error");
    }
}


void DataAcquisition::copyData(DataBuffer *pBuf)
{
    struct AcqBlock* ab = (struct AcqBlock *)m_pMemory;
    for (int i = 0; i < INTERATION; i++)
	{
		unsigned long buffersize;
		try{
			buffersize = QueImage(m_pMemory, m_ulAscanList, i);
		} catch (std::exception &ex) {
			throw std::invalid_argument("DataAcquisition QueImage error");
		}

		/*
		try{
			SaveQueImage(ab, "E:\\test\\raw_data", m_ulAscanList, buffersize, i+1);
		} catch (std::exception &ex) {
			throw std::invalid_argument("DataAcquisition SaveQueImage error");
		}
        */

		try{
			pBuf->bufferCopyIn((*ab).MemQue + i * buffersize, buffersize);
		} catch (std::exception &ex) {
			throw std::invalid_argument("DataAcquisition bufferCopyIn error");
		}
    }

    if(!StopGrabImage(m_pMemory) || !DeallocateQueMemory(m_pMemory))
		throw std::invalid_argument("DataAcquisition StopGrabImage error");
}
