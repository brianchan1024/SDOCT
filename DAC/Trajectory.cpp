#include "Trajectory.h"
#include <cmath>
#include <windows.h>
#include <assert.h>
#include "DAC_logger.h"

#define M_PI       3.14159265358979323846
char buf[2048];

Trajectory* TrajectoryGenerator::getTrajectory(WorkingModel_e model){
	//
    if (model == ACQUIRE)
    {
        initAcquireTraj();
    }
    else if (model == PREVIEWXZ)
    {
		int previewType = 0;
        initPreviewTraj(previewType);
    }
	else if (model == PREVIEWYZ)
    {
		int previewType = 1;
        initPreviewTraj(previewType);
    }
    else if (model == FUNDUS_VIEW)
    {
        initFundusViewTraj();
    }
    return m_Trajectory;
}

Trajectory* TrajectoryGenerator::initAcquireTraj(){
	m_Trajectory = new Trajectory();
	createAcquireTraj();
	DAC_LOGGER_INFO << __FUNCTION__ << "\t\t initAcquireTraj done";
    return m_Trajectory;
}

Trajectory* TrajectoryGenerator::initPreviewTraj(int previewType){
	m_Trajectory = new Trajectory();
	createpPreviewTraj(previewType);
	DAC_LOGGER_INFO << __FUNCTION__ << "\t\t initPreviewTraj done";
    return m_Trajectory;
}

Trajectory* TrajectoryGenerator::initFundusViewTraj(){
	m_Trajectory = new Trajectory();
	createFundusViewTraj();
	DAC_LOGGER_INFO << __FUNCTION__ << "\t\t initFundusViewTraj done";
    return m_Trajectory;
}

Trajectory* TrajectoryGenerator::initBackgoundTraj(){
	m_Trajectory = new Trajectory();
	createpBackgroundTraj();
	DAC_LOGGER_INFO << __FUNCTION__ << "\t\t initBackgoundTraj done";
	return m_Trajectory;
}

bool TrajectoryGenerator::createAcquireTraj()
{
	float fXstart = - m_pScanPatt->m_fXRangeV / 2 + m_pScanPatt->m_fXOffsetV;
	float fXend = m_pScanPatt->m_fXRangeV / 2 + m_pScanPatt->m_fXOffsetV;
	float fYstart = - m_pScanPatt->m_fYRangeV / 2 + m_pScanPatt->m_fYOffsetV;	
	float fYend = m_pScanPatt->m_fYRangeV / 2 + m_pScanPatt->m_fYOffsetV;

	m_uiNumAscan = m_pScanPatt->m_uiNumAscan;
    m_uiNumBscan = m_pScanPatt->m_uiNumBscan;

	//generateTrajectory(fXstart, fXend, fYstart, fYend);
	bool re = generateTrajectory(fXstart, fXend, fYstart, fYend, m_pScanPatt->m_uiNumBscanRepeat);
	return re;
}

bool TrajectoryGenerator::createFundusViewTraj()
{
	float fXstart = - m_pScanPatt->m_fXRangeV / 2 + m_pTrajCali->m_fFundusXOffsetV;
	float fXend = m_pScanPatt->m_fXRangeV / 2 + m_pTrajCali->m_fFundusXOffsetV;
	float fYstart = - m_pScanPatt->m_fYRangeV / 2 + m_pTrajCali->m_fFundusYOffsetV;	
	float fYend = m_pScanPatt->m_fYRangeV / 2 + m_pTrajCali->m_fFundusYOffsetV;

	m_uiNumAscan = m_pTrajCali->m_iFundusViewAScan;
	m_uiNumBscan = m_pTrajCali->m_iFundusViewAScan;
	
	bool re = generateTrajectory(fXstart, fXend, fYstart, fYend, 0);
	return re;
}
bool TrajectoryGenerator::createpPreviewTraj(int type)
{
	float fXstart, fXend, fYstart, fYend;
	if(type == 0) {
		//XZ	
		fXstart = - m_pScanPatt->m_fXRangeV / 2 + m_pScanPatt->m_fXOffsetV;
		fXend = m_pScanPatt->m_fXRangeV / 2 + m_pScanPatt->m_fXOffsetV;
		fYstart = m_pScanPatt->m_fYOffsetV;
		fYend = m_pScanPatt->m_fYOffsetV;
		m_bXPriority = true;
	}else{
		//YZ
		fXstart = - m_pScanPatt->m_fYRangeV / 2 + m_pScanPatt->m_fYOffsetV;
		fXend = m_pScanPatt->m_fYRangeV / 2 + m_pScanPatt->m_fYOffsetV;
		fYstart = m_pScanPatt->m_fXOffsetV;
		fYend = m_pScanPatt->m_fXOffsetV;
		m_bXPriority = false;
	}
	//printf("fXstart: %f, fYstart: %f, fXend: %f, fYend: %f.\n", fXstart, fYstart, fXend, fYend);
	m_uiNumAscan = m_pTrajCali->m_iPreviewAScan;
    m_uiNumBscan = 1;

	bool re = generateTrajectory(fXstart, fXend, fYstart, fYend, 0);
	return re;
}

bool TrajectoryGenerator::createpBackgroundTraj()
{
	float fXstart = - m_pScanPatt->m_fXRangeV / 2 + m_pScanPatt->m_fXOffsetV;
	float fXend = m_pScanPatt->m_fXRangeV / 2 + m_pScanPatt->m_fXOffsetV;
	float fYstart = - m_pTrajCali->m_fYMaxV;
	float fYend = - m_pTrajCali->m_fYMaxV;

	m_uiNumAscan = m_pTrajCali->m_iBackgroundAScan;
	m_uiNumBscan = 1;
	m_bXPriority = true;
	bool re = generateTrajectory(fXstart, fXend, fYstart, fYend, 0);
	return re;
}

void TrajectoryGenerator::trajectorySafep2Startp(float fXstart, float fYstart)
{
	float deltaX = fXstart - m_fSafepX;
	float deltaY = fYstart - m_fSafepY;
	float maxDelta = max(deltaX, deltaY);

	double t1, x1, y1;

	double tTotal = calSafep2StartpTime(m_fSafepX, m_fSafepY, fXstart, fYstart, t1, x1, y1);

	DAC_LOGGER_DEBUG << __FUNCTION__<<" m_fSafepX: "<<m_fSafepX<<" m_fSafepY: "<<m_fSafepY<<" fXstart: "<<fXstart<<" fYstart: "<<fYstart << " x1:" << x1 <<" y1:" << y1 <<" t1:" << t1 << "tTotal: " << tTotal;

	double tUpdate = 1.0/((double)m_pTrajCali->m_uiUpdateRate);
	double t = 0.0;
	float x, y;
	DAC_LOGGER_DEBUG << __FUNCTION__<<" point: "<<tTotal/tUpdate;
	while (t < tTotal)
	{
		double ax = (deltaX / maxDelta) * m_pTrajCali->m_fTrajTriAMax;
		double ay = (deltaY / maxDelta) * m_pTrajCali->m_fTrajTriAMax;
		if (t < t1)
		{
			x = m_fSafepX + 0.5 * ax * t * t; // d = 0.5att
			y = m_fSafepY + 0.5 * ay * t * t; // d = 0.5att

		} else if (t1 <= t && t <= tTotal)
		{
			x = x1 + ax * t1 * (t - t1) - 0.5 * ax * (t - t1) * (t - t1); // x = x1 + vt - 0.5att
			y = y1 + ay * t1 * (t - t1) - 0.5 * ay * (t - t1) * (t - t1); // x = x1 + vt - 0.5att
		}
		//DAC_LOGGER_DEBUG << __FUNCTION__<<"\t\t x:"<<x<<" y:"<<y;
		if(m_bXPriority){
			m_Trajectory->pushBack(x,y,TP_SLOPE);
		} else {
			m_Trajectory->pushBack(y,x,TP_SLOPE);
		}
		t += tUpdate;
	}

}

double TrajectoryGenerator::calSafep2StartpTime(float savepX, float savepY, float startpX, float startpY, double &t1, double &x1, double &y1)

{
	float maxDelta = max(startpX-savepX, startpY-savepY);
	//assert(maxDelta < m_pTrajCali->m_fTrajTriVMax * m_pTrajCali->m_fTrajTriVMax / m_pTrajCali->m_fTrajTriAMax);
	if (maxDelta >= m_pTrajCali->m_fTrajTriVMax * m_pTrajCali->m_fTrajTriVMax / m_pTrajCali->m_fTrajTriAMax)
	{ // d>v2/a
		DAC_LOGGER_ERROR << __FUNCTION__<<"\t\t " << "distance between safe point and start point is two large for just acceleration";
		throw std::invalid_argument("distance between safe point and start point is two large for just acceleration");
	}
	t1 = sqrt(maxDelta / m_pTrajCali->m_fTrajTriAMax); // 2 * 0.5att == d
	x1 = (savepX + startpX) / 2;
	y1 = (savepY + startpY) / 2;
	return 2 * t1;
}

void TrajectoryGenerator::trajectoryEndp2Safep(float fXstart, float fYstart)
{
	float deltaX = fXstart - m_fSafepX;
	float deltaY = fYstart - m_fSafepY;
	float maxDelta = max(deltaX, deltaY);

	double t1, x1, y1;
	double tTotal = calSafep2StartpTime(m_fSafepX, m_fSafepY, fXstart, fYstart,
		t1, x1, y1);

	DAC_LOGGER_DEBUG << __FUNCTION__<<" m_fSafepX: "<<m_fSafepX<<" m_fSafepY: "<<m_fSafepY<<" fXstart: "<<fXstart<<" fYstart: "<<fYstart << " x1:" << x1 <<" y1:" << y1 <<" t1:" << t1 << "tTotal: " << tTotal;

	double tUpdate = 1.0/((double)m_pTrajCali->m_uiUpdateRate);
	double t = 0.0;
	float x, y;
	while (t < tTotal)
	{
		double ax = (deltaX / maxDelta) * m_pTrajCali->m_fTrajTriAMax;
		double ay = (deltaY / maxDelta) * m_pTrajCali->m_fTrajTriAMax;
		if (t < t1)
		{
			x = fXstart - 0.5 * ax * t * t; // d = 0.5att
			y = fYstart - 0.5 * ay * t * t; // d = 0.5att
		} else if (t1 <= t && t <= tTotal)
		{
			x = x1 - (ax * t1 * (t - t1) - 0.5 * ax * (t - t1) * (t - t1)); // x = x1 - (vt - 0.5att)
			y = y1 - (ay * t1 * (t - t1) - 0.5 * ay * (t - t1) * (t - t1)); // x = x1 - (vt - 0.5att)
		} else
		{
			break;
		}
		// DAC_LOGGER_DEBUG << __FUNCTION__<<"\t\t x:"<<x<<" y:"<<y;
		if(m_bXPriority){
			m_Trajectory->pushBack(x,y,TP_SLOPE);
		} else {
			m_Trajectory->pushBack(y,x,TP_SLOPE);
		}
		t += tUpdate;
	}
}

std::vector<TrajectoryPoint_t> TrajectoryGenerator::getOneRepeatBscanPoints(std::vector<TrajectoryPoint_t>& OneRasterBscanPoints)
{
	std::vector<TrajectoryPoint_t> OneRepeatBscanPoints = OneRasterBscanPoints;
	for (int i = 0; i < OneRepeatBscanPoints.size(); i++)
	{
		OneRepeatBscanPoints[i].y = 0;
	}
	return OneRepeatBscanPoints;
}

bool TrajectoryGenerator::generateTrajectory(float fXstart, float fXend, float fYstart, float fYend, int BScanRepeatCnt)
{
	double tUpRate = 1.0/((double)m_pTrajCali->m_uiUpdateRate);
	double vxPositive = (fXend-fXstart) / (m_uiNumAscan*m_pTrajCali->m_fExposureTime);
	float ratio = 2; // do not change this !!!85/15 is big , wave efficiency  85%
	double vxNegetive = vxPositive * ratio;
	double aMax = m_pTrajCali->m_fTrajTriAMax;
	double xt0, xt1, xt2, xt3, xt4, xt5, xt6;
	double x0, x1, x2, x3, x4, x5, x6;

	if (vxPositive > m_pTrajCali->m_fTrajTriVMax || vxNegetive > m_pTrajCali->m_fTrajTriVMax)
	{
		DAC_LOGGER_ERROR << __FUNCTION__<<"\t\t " << "generateTrajectory velocity is larger than max error";
		throw std::invalid_argument("generateTrajectory velocity is larger than max error");
	}

	DAC_LOGGER_DEBUG << __FUNCTION__<<"\t\t fXstart:"<<fXstart<<" fXend:"<<fXend<<" fYstart:"<<fYstart<<" fYend:"<<fYend;

	infoOneRaterAscanX(fXstart, fXend, vxPositive, vxNegetive, aMax,
		xt0, xt1, xt2, xt3, xt4, xt5, xt6,
		x0, x1, x2, x3, x4, x5, x6);

	DAC_LOGGER_DEBUG << __FUNCTION__<<"\t\t x0:"<<x0<<" x1:"<<x1<<" x2:"<<x2<<" x3:"<<x3<<" x4:"<<x4<<" x5:"<<x5<<" x6:"<<x6;

	double yBase = 0;
	double yGap = (fYend - fYstart) / m_uiNumBscan;
	double yt0, yt1, yt2, yt3;
	double y0, y1, y2, y3;
	double ya = 0;

	infoOneRaterAscanY(yBase, yGap, aMax, xt0, xt3, xt6,
		yt0, yt1, yt2, yt3,
		y0, y1, y2, y3, ya);


	std::vector<TrajectoryPoint_t> OneRasterBscanPoints;
	OneRasterBscanPoints = getOneRasterBscanPoints(fXstart, fXend, yBase, yGap, vxPositive, vxNegetive, aMax, tUpRate,
		xt0, xt1, xt2, xt3, xt4, xt5, xt6,
		x0, x1, x2, x3, x4, x5, x6,
		yt0, yt1, yt2, yt3,
		y0, y1, y2, y3, ya);

	std::vector<TrajectoryPoint_t> OneRepeatBscanPoints;
	if (BScanRepeatCnt > 0)
	{
		OneRepeatBscanPoints = getOneRepeatBscanPoints(OneRasterBscanPoints);
	}

	trajectorySafep2Startp(x0, fYstart);


	int numScanCnt = 0;
	double yAscanSatrt;
	float x, y;
	TrajectoryPointType_e type;

	DAC_LOGGER_DEBUG << __FUNCTION__<<"\t\t m_uiNumAscan: "<<m_uiNumAscan<<" m_uiNumBscan: "<<m_uiNumBscan << "yGap:" << yGap;

	while(numScanCnt < m_uiNumBscan)
	{
		yAscanSatrt = fYstart + numScanCnt * yGap;

		// add repeat scan here
		for(int i = 0; i < BScanRepeatCnt; i++)
		{
			for (int i =0; i < OneRepeatBscanPoints.size(); i++)
			{
				x = OneRepeatBscanPoints[i].x;
				y = OneRepeatBscanPoints[i].y + yAscanSatrt;
				type = OneRepeatBscanPoints[i].type;

				if(m_bXPriority){
					m_Trajectory->pushBack(x,y,type);
				} else {
					m_Trajectory->pushBack(y,x,type);
				}
			}
		}

		// add raster scan here
		for (int i =0; i < OneRasterBscanPoints.size(); i++)
		{
			x = OneRasterBscanPoints[i].x;
			y = OneRasterBscanPoints[i].y + yAscanSatrt;
			type = OneRasterBscanPoints[i].type;

			if(m_bXPriority){
				m_Trajectory->pushBack(x,y,type);
			} else {
				m_Trajectory->pushBack(y,x,type);
			}
		}
		numScanCnt += 1;
	}

	float yScanEnd = fYstart + m_uiNumBscan * yGap;
	trajectoryEndp2Safep(x0, yScanEnd);
	return true;
}

std::vector<TrajectoryPoint_t> TrajectoryGenerator::getOneRasterBscanPoints(double xStart, double xEnd, double yStart, double yEnd, double vxPositive, double vxNegetive, double aMax, double tUpRate,
	double xt0, double xt1, double xt2, double xt3, double xt4, double xt5, double xt6,
	double x0, double x1, double x2, double x3, double x4, double x5, double x6,
	double yt0, double yt1, double yt2, double yt3,
	double y0, double y1, double y2, double y3, double ya)
{
	std::vector<TrajectoryPoint_t> OneRasterBscanPoints;
	int numPoint = int((xt6 - xt0) / tUpRate) + 1;

	DAC_LOGGER_DEBUG << __FUNCTION__<<"\t\t numPoint: "<<numPoint;

	double t = 0.0;
	
	float x, y;
	TrajectoryPointType_e type = TP_BAD;
	TrajectoryPoint_t point;
	while(t <= xt6)
	{
		x = getRasterBscanX(xStart, xEnd, vxPositive, vxNegetive, aMax,
			xt0, xt1, xt2, xt3, xt4, xt5, xt6,
			x0, x1, x2, x3, x4, x5, x6,
			t, type);

		y = getRasterBscanY(yStart, yEnd, ya,
			yt0, yt1, yt2, yt3,
			y0, y1, y2, y3,
			t);

		point.x = x;
		point.y = y;
		point.type = type;
		OneRasterBscanPoints.push_back(point);

		t += tUpRate;
	}
	DAC_LOGGER_DEBUG << __FUNCTION__<<"\t\t OneRasterBscanPoints len: "<<OneRasterBscanPoints.size();
	return OneRasterBscanPoints;
}


double TrajectoryGenerator::getRasterBscanY(double yStart, double yEnd, double ya,
	double yt0, double yt1, double yt2, double yt3,
	double y0, double y1, double y2, double y3,
	double t)
{
	if(abs(yStart - yEnd) < 1e-7)
		return yStart;

	double y;
	if (yt2 < t && t <= yt3)
	{
		// 3 speed down
		y = y2 + ya * (yt2 - yt1) * (t-yt2) - 0.5 * ya * (t-yt2) * (t-yt2); // y = y0 + vt - o.5att
	}
	else if (yt1 < t && t <= yt2)
	{
		// 2 speed up
		y = 0.5 * ya * (t-yt1) * (t-yt1); // y = o.5att
	}
	else if (yt0 <= t && t <= yt1)
	{
		// 1 y motionless
		y = yStart;
	}
	
	if (yt0 > t || t > yt3)
	{
		DAC_LOGGER_ERROR << __FUNCTION__<<"\t\t " << "getRasterBscanY time error";
		throw std::invalid_argument("getRasterBscanY time error");
	}
	if (y0 > y || y > y3)
	{
		DAC_LOGGER_ERROR << __FUNCTION__<<"\t\t " << "getRasterBscanY y value error";
		throw std::invalid_argument("getRasterBscanY y value error");
	}
	return y;
}

double TrajectoryGenerator::getRasterBscanX(double xStart, double xEnd, double vxPositive, double vxNegetive, double aMax,
	double xt0, double xt1, double xt2, double xt3, double xt4, double xt5, double xt6,
	double x0, double x1, double x2, double x3, double x4, double x5, double x6,
	double t, TrajectoryPointType_e &type)
{
	if(abs(xEnd - xStart) < 1e-7)
		return xStart;
	
	double x;
	if (xt5 < t && t <= xt6)
	{
		// 6 negative direction speed down
		type = TP_FLYBACK;
		x = x5 -  ( vxNegetive * (t-xt5) - 0.5 * aMax * (t-xt5) * (t-xt5) );
	}
	else if (xt4 < t && t <= xt5)
	{
		// 5 negative direction constant speed
		type = TP_FLYBACK;
		x = x4 - vxNegetive * (t-xt4);
	}
	else if (xt3 < t && t <= xt4)
	{
		// 4 negative direction speed up
		type = TP_FLYBACK;
		x = x3 - 0.5 * aMax * (t-xt3) * (t-xt3);
	}
	else if (xt2 < t && t <= xt3)
	{
		// 3 postive direction speed down
		type = TP_RAMP;
		x = x2 + vxPositive * (t-xt2) - 0.5 * aMax * (t-xt2) * (t-xt2);
	}
	else if (xt1 < t && t <= xt2)
	{
		// 2 postive direction constant speed
		type = TP_ACQUIRE;
		x = x1 + vxPositive * (t-xt1);
	}
	else if (xt0 <= t && t <= xt1)
	{
		// 1 postive direction speed up
		type = TP_RAMP;
		x = x0 + 0.5 * aMax * (t-xt0) * (t-xt0);
	}
	else
	{
		type = TP_BAD;
		throw std::invalid_argument("getRasterBscanX time error!");
	}

	if (xt0 > t || t > xt6)
	{
		DAC_LOGGER_ERROR << __FUNCTION__<<"\t\t " << "getRasterBscanX time error";
		throw std::invalid_argument("getRasterBscanX time error");
	}
	if (x0 > x || x > x3)
	{
		DAC_LOGGER_ERROR << __FUNCTION__<<"\t\t " << "getRasterBscanX x value error";
		throw std::invalid_argument("getRasterBscanX x value error");
	}
	return x;
}

void TrajectoryGenerator::infoOneRaterAscanX(double xStart, double xEnd, double vPositive, double vNegetive, double aMax,
	double &t0, double &t1, double &t2, double &t3, double &t4, double &t5, double &t6,
	double &x0, double &x1, double &x2, double &x3, double &x4, double &x5, double &x6)
{
	// 1 postive direction speed up
	double deltaT0 = vPositive / aMax;
	double deltaX0 = (vPositive * vPositive) / (2 * aMax);
	t0 = 0;
	x0 = xStart - deltaX0;
	t1 = t0 + deltaT0;
	x1 = xStart;
	

	// 2 postive direction constant speed
	double deltaT1 = (xEnd - xStart) / vPositive;
	double deltaX1 = xEnd - xStart;
	t2 = t1 + deltaT1;
	x2 = xEnd;

	// 3 postive direction speed down
	double deltaT2 = deltaT0;
	double deltaX2 = deltaX0;
	t3 = t2 + deltaT2;
	x3 = x2 + deltaX2;

	double deltaXSum = deltaX0 + deltaX1 + deltaX2;

	// 4 negative direction speed up
	double deltaT3 = vNegetive / aMax;
	double deltaX3 = (vNegetive * vNegetive) / (2 * aMax);
	t4 = t3 + deltaT3;
	x4 = x3 - deltaX3;

	// 5 negative direction constant speed
	double deltaT4 = (deltaXSum - 2*deltaX3) / vNegetive;
	double deltaX4 = deltaXSum - 2*deltaX3;
	t5 = t4 + deltaT4;
	x5 = x4 - deltaX4;

	// 6 negative direction speed down
	double deltaT5 = deltaT3;
	double deltaX5 = deltaX3;
	t6 = t5 + deltaT5;
	x6 = x5 - deltaX5;

	//assert(abs(x6 - x0) < 1e-6);
	if (abs(x6 - x0) > 1e-6)
	{
		DAC_LOGGER_ERROR << __FUNCTION__<<"\t\t " << "infoOneRaterAscanX x value error";
		throw std::invalid_argument("infoOneRaterAscanX x value error");
	}

	DAC_LOGGER_DEBUG << __FUNCTION__<<"\t\t xStart:"<<xStart<<" xEnd:"<<xEnd <<" vPositive:"<<vPositive<<" vNegetive:"<<vNegetive<<" aMax: "<<aMax;
	DAC_LOGGER_DEBUG << __FUNCTION__<<"\t\t deltaX0:"<<deltaX0<<" deltaX1:"<<deltaX1 <<" deltaX2:"<<deltaX2<<" deltaX3:"<<deltaX3<<" deltaX4: "<<deltaX4<<" deltaX5: "<<deltaX5;
	DAC_LOGGER_DEBUG << __FUNCTION__<<"\t\t t0:"<<t0<<" t1:"<<t1<<" t2:"<<t2<<" t3:"<<t3<<" t4: "<<t4 <<" t5: "<<t5<<" t6: "<<t6;
	DAC_LOGGER_DEBUG << __FUNCTION__<<"\t\t x0:"<<x0<<" x1:"<<x1<<" x2:"<<x2<<" x3:"<<x3<<" x4: "<<x4 <<" x5: "<<x5<<" x6: "<<x6;

}

void TrajectoryGenerator::infoOneRaterAscanY(double yStart, double yEnd, double aMax, double xt0, double xt3, double xt6,
	double &t0, double &t1, double &t2, double &t3,
	double &y0, double &y1, double &y2, double &y3,
	double &a)
{
	// at x accelerate ramp start point
	t0 = xt0;
	y0 = yStart;

	// at x decelerate ramp end point
	t1 = xt3;
	y1 = yStart;

	// at end of one ascan point 
	t3 = xt6;
	y3 = yEnd;

	// at center of flyback point, largest y speed
	a = (yEnd - yStart) / ((0.5 * (t3 - t1)) * (0.5 * (t3 - t1)));  // a = d / (t * t)
	t2 = (t1 + t3) / 2;
	y2 = (y1 + y3) / 2;

	if (a > aMax)
	{
		DAC_LOGGER_ERROR << __FUNCTION__<<"\t\t " << "infoOneRaterAscanY a value error";
		throw std::invalid_argument("infoOneRaterAscanY a value error");
	}

	DAC_LOGGER_DEBUG << __FUNCTION__<<"\t\t xStart:"<<yStart<<" xEnd:"<<yEnd <<" aMax: "<<aMax << " a: " <<a;
	DAC_LOGGER_DEBUG << __FUNCTION__<<"\t\t t0:"<<t0<<" t1:"<<t1<<" t2:"<<t2<<" t3:"<<t3;
	DAC_LOGGER_DEBUG << __FUNCTION__<<"\t\t y0:"<<y0<<" y1:"<<y1<<" y2:"<<y2<<" y3:"<<y3;
}
