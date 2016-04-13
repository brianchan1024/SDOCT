#ifdef DATABUFFER_EXPORTS
#define DATABUFFERDLL __declspec(dllexport) 
#else
#define DATABUFFERDLL __declspec(dllimport) 
#endif

#include <vector>
#include <iostream>
#include <fstream>
#include "common.h"

class DataBuffer
{
private:
    static uInt_32 m_uiInstanceCounter;
    std::vector<short> m_vBuffer;
    size_t m_ulSampleAcquired;

    uInt_32 m_uiNumPixels;
    uInt_32 m_uiNumAscan;
    uInt_32 m_uiNumBscan;
	uInt_32 m_uiNumBscanRepeat;

public:
    DATABUFFERDLL DataBuffer(size_t size);
	DATABUFFERDLL DataBuffer(uInt_32 numPixels, uInt_32 numAscan, uInt_32 numBscan, uInt_32 uiNumBscanRepeat);
    const size_t getSize() {return m_vBuffer.size(); }
    bool bufferCopyIn(unsigned short* pMemory, size_t numSamples);
    const bool bufferCopyOut(short* pMemory, size_t starIdx, size_t numSamples);
	DATABUFFERDLL void print_buf();
	DATABUFFERDLL void save_buf(std::string file);
	DATABUFFERDLL std::vector<short> getFrameData();
	template <typename T>
	std::string to_string(T value);
    
};