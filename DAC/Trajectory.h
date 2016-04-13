#pragma once

#include <string>
#include <vector>
#include <cmath>
#include <iostream>
#include <stdexcept>      // std::invalid_argument
#include <algorithm>    // std::min

#include "common.h"
#include "TrajectoryCalibration.h"
#include "ScanPattern.h"
#include "DAC_logger.h"

enum WorkingModel_e
{
    IDEL = 0,
	BACKGROUND,
    ACQUIRE,
    PREVIEWXZ,
	PREVIEWYZ,
    FUNDUS_VIEW
};

enum TrajectoryPointType_e
{
    TP_ACQUIRE = 0,
    TP_BACKGROUND,
    TP_FLYBACK,
	TP_SLOPE,
	TP_RAMP,
    TP_BAD
};

struct TrajectoryPoint_t
{
    float x;
    float y;
    TrajectoryPointType_e type;
};

class Trajectory
{
    friend class TrajectoryGenerator;
private:
    std::vector<TrajectoryPoint_t> m_vTrajPoint;

public:
    size_t getTrajactoryLength() { return m_vTrajPoint.size(); }
    std::vector<TrajectoryPoint_t> getTrajactoryPoints() { return m_vTrajPoint; }
	void pushBack(float x, float y, TrajectoryPointType_e type)
	{	
		TrajectoryPoint_t e;
		e.x = x;
		e.y = y;
		e.type = type;
		m_vTrajPoint.push_back(e);
	}

    void printTrajectory()
	{
		int cquCnt = 0;
		int bgrCnt = 0;
		int flybkCnt = 0;
		int badCnt = 0;
		int rampCnt = 0;
		int slopeCnt = 0;
		for(std::vector<TrajectoryPoint_t>::iterator iter=m_vTrajPoint.begin(); iter!=m_vTrajPoint.end(); ++iter) {
			switch((*iter).type) {
			case TP_ACQUIRE:
				cquCnt++;
				break;
			case TP_BACKGROUND:
				bgrCnt++;
				break;
			case TP_FLYBACK:
				flybkCnt++;
				break;
			case TP_RAMP:
				rampCnt++;
				break;
			case TP_SLOPE:
				slopeCnt++;
				break;
			case TP_BAD:
				badCnt++;
				break;
			}
		}
		std::cout<<"DEBUG: "<<__FUNCTION__<<" Trajectory total size: "<<m_vTrajPoint.size()<<"\n";
		std::cout<<"DEBUG: "<<__FUNCTION__<<" TP_ACQUIRE size: "<<cquCnt<<"\n";
		std::cout<<"DEBUG: "<<__FUNCTION__<<" TP_BACKGROUND size: "<<bgrCnt<<"\n";
		std::cout<<"DEBUG: "<<__FUNCTION__<<" TP_FLYBACK size: "<<flybkCnt<<"\n";
		std::cout<<"DEBUG: "<<__FUNCTION__<<" TP_RAMP size: "<<rampCnt<<"\n";
		std::cout<<"DEBUG: "<<__FUNCTION__<<" TP_SLOPE size: "<<slopeCnt<<"\n";
		std::cout<<"DEBUG: "<<__FUNCTION__<<" TP_BAD size: "<<badCnt<<"\n";
	}
};

class TrajectoryGenerator
{
public:
	TrajectoryGenerator() {};
    TrajectoryGenerator(TrajectoryCalibration* pTrajCali, ScanPattern* pScanPatt)
    {
        setTrajectoryGenerator(pTrajCali, pScanPatt);
    }

	void setTrajectoryGenerator(TrajectoryCalibration* pTrajCali, ScanPattern* pScanPatt)
    {
        m_pTrajCali = pTrajCali;
        m_pScanPatt = pScanPatt;
		m_bXPriority = true;

		m_fSafepX = -pScanPatt->m_fXRangeV / 2 + pScanPatt->m_fXOffsetV - 2;
		m_fSafepY = -pScanPatt->m_fYRangeV / 2 + pScanPatt->m_fYOffsetV - 2;

		m_fSafepX = std::min(m_fSafepX, m_fSafepY);
		m_fSafepY = m_fSafepX;

    }

	~TrajectoryGenerator() {}
    Trajectory *getTrajectory(WorkingModel_e model);
    Trajectory *initAcquireTraj();
    Trajectory *initPreviewTraj(int previewType);
    Trajectory *initFundusViewTraj();
	Trajectory *initBackgoundTraj();
    
    
private:
    TrajectoryCalibration* m_pTrajCali;
    ScanPattern* m_pScanPatt;
    Trajectory* m_Trajectory;

	uInt_32 m_uiNumAscan;
    uInt_32 m_uiNumBscan;
	int m_ulNumRepeats;
	bool m_bXPriority; // used in preview if XZ or YZ

	float m_fSafepX;
	float m_fSafepY;

	bool createAcquireTraj();
	bool createFundusViewTraj();
	bool createpPreviewTraj(int type);
	bool createpBackgroundTraj();
	bool generateTrajectory(float fXstart, float fXend, float fYstart, float fYend, int BScanRepeatCnt);

	void trajectorySafep2Startp(float fXstart, float fYstart);
	double calSafep2StartpTime(float savepX, float savepY, float startpX, float startpY, double &t1, double &x1, double &y1);
	void trajectoryEndp2Safep(float fXstart, float fYstart);

	void infoOneRaterAscanX(double xStart, double xEnd, double veloPositive, double veloNegetive, double aMax,
		double &t0, double &t1, double &t2, double &t3, double &t4, double &t5, double &t6,
		double &x0, double &x1, double &x2, double &x3, double &x4, double &x5, double &x6);
	
	void infoOneRaterAscanY(double yStart, double yEnd, double aMax, double xt0, double xt3, double xt6,
		double &t0, double &t1, double &t2, double &t3,
		double &y0, double &y1, double &y2, double &y3, double &a);

	std::vector<TrajectoryPoint_t> getOneRasterBscanPoints(double xStart, double xEnd, double yStart, double yEnd, double vxPositive, double vxNegetive, double aMax, double tUpRate,
		double xt0, double xt1, double xt2, double xt3, double xt4, double xt5, double xt6,
		double x0, double x1, double x2, double x3, double x4, double x5, double x6,
		double yt0, double yt1, double yt2, double yt3,
		double y0, double y1, double y2, double y3, double ya);

	std::vector<TrajectoryPoint_t> getOneRepeatBscanPoints(std::vector<TrajectoryPoint_t>& OneRasterBscanPoints);

	double getRasterBscanX(double xStart, double xEnd, double vxPositive, double vxNegetive, double aMax,
		double xt0, double xt1, double xt2, double xt3, double xt4, double xt5, double xt6,
		double x0, double x1, double x2, double x3, double x4, double x5, double x6,
		double t, TrajectoryPointType_e &type);

	double getRasterBscanY(double yStart, double yEnd, double ya,
		double yt0, double yt1, double yt2, double yt3,
		double y0, double y1, double y2, double y3,
		double t);

};