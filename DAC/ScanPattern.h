#pragma once

#include "common.h"

class ScanPattern
{
	friend class TrajectoryGenerator;
private:
    float m_fXRangeV;
    float m_fYRangeV;
    float m_fXOffsetV;
    float m_fYOffsetV;
    
    uInt_32 m_uiNumPixel;
    uInt_32 m_uiNumAscan;
    uInt_32 m_uiNumBscan;
    uInt_32 m_uiNumBcgrFrame; // background frames
	uInt_32 m_uiNumBscanRepeat;

public:
	ScanPattern() {};

    ScanPattern(float fXRangeV, float fYRangeV, float fXOffsetV, float fYOffsetV,
                uInt_32 uiNumPixel, uInt_32 uiNumAscan, uInt_32 uiNumBscan, uInt_32 uiNumBcgrFrame, uInt_32 uiNumBscanRepeat)
    {
        setScanPattern(fXRangeV, fYRangeV, fXOffsetV, fYOffsetV,
                uiNumPixel, uiNumAscan, uiNumBscan, uiNumBcgrFrame, uiNumBscanRepeat);
    }

	~ScanPattern() {};
	void setScanPattern(float fXRangeV, float fYRangeV, float fXOffsetV, float fYOffsetV,
                uInt_32 uiNumPixel, uInt_32 uiNumAscan, uInt_32 uiNumBscan, uInt_32 uiNumBcgrFrame, uInt_32 uiNumBscanRepeat)
    {
        m_fXRangeV = fXRangeV;
        m_fYRangeV = fYRangeV;
        m_fXOffsetV = fXOffsetV;
        m_fYOffsetV = fYOffsetV;
        
        m_uiNumPixel = uiNumPixel;
        m_uiNumAscan = uiNumAscan;
        m_uiNumBscan = uiNumBscan;
        m_uiNumBcgrFrame = uiNumBcgrFrame;
		m_uiNumBscanRepeat = uiNumBscanRepeat;
    }
};
