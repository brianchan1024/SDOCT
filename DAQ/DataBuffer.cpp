#include "DataBuffer.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <assert.h>

DataBuffer::DataBuffer(size_t size)
{
    m_vBuffer.resize(size);
}

DataBuffer::DataBuffer(uInt_32 numPixels, uInt_32 numAscan, uInt_32 numBscan, uInt_32 uiNumBscanRepeat)
{
	m_uiNumPixels = numPixels;
    m_uiNumAscan = numAscan;
    m_uiNumBscan = numBscan;
	m_uiNumBscanRepeat = uiNumBscanRepeat;
	m_vBuffer.resize(m_uiNumPixels * m_uiNumAscan * m_uiNumBscan * (1+m_uiNumBscanRepeat));
}

bool DataBuffer::bufferCopyIn(unsigned short* pMemory, size_t numSamples)
{
    if(numSamples > m_vBuffer.size()) { return false; }
    size_t numAvailable = m_vBuffer.size() - m_ulSampleAcquired;
    if(numSamples > numAvailable) {return false; }
    
    // confused here, which is correct @brian
    //memcpy(&m_vBuffer[m_ulTotalSamplesAcquired],pMem,sizeof(short)*numSamples);
    //m_ulTotalSamplesAcquired+=numSamples;
    memcpy(&m_vBuffer[0],pMemory,sizeof(short)*numSamples);
    m_ulSampleAcquired = numSamples;
    return true;
}

const bool DataBuffer::bufferCopyOut(short* pMemory, size_t starIdx, size_t numSamples)
{
    if (starIdx + numSamples > m_ulSampleAcquired) { return false; }
    memcpy(pMemory, &m_vBuffer[starIdx], sizeof(short)*numSamples);
    return true;
}

void DataBuffer::print_buf()
{
	printf("DataBuffer::print_buf m_vBuffer");
	/*
	for(int i = 0; i< m_vBuffer.size(); i++)
	{
		if(i%10==0)
		{
			printf("\n");
		}
		printf("%d  ", m_vBuffer.at(i));
	}
	*/
	printf("buffer size: %d\n", m_vBuffer.size());
	save_buf("E:\\test\\raw_data.txt");
}

template <typename T>
std::string DataBuffer::to_string(T value)
{
	std::ostringstream os ;
	os << value ;
	return os.str() ;
}

void DataBuffer::save_buf(std::string file)
{
	std::ofstream myfile;
	myfile.open (file);
	std::string data = "";
	for(int i = 0; i< m_vBuffer.size(); i++){
		if(i%1024==0 && i != 0)
		{
			data += '\n';
		}
		data += to_string(m_vBuffer.at(i)) + '\t';
	}
	myfile << data;
	myfile.close();
}


std::vector<short> DataBuffer::getFrameData()
{
	return m_vBuffer;
}