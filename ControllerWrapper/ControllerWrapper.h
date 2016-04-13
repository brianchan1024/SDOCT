// ControllerWrapper.h

#pragma once

#ifdef __cplusplus_cli
#define generic __identifier(generic)
#endif
#include <boost/filesystem.hpp>
#ifdef __cplusplus_cli
#undef generic
#endif

#include <iostream>
#include <string>
#include <cliext/adapter>
#include <cliext/vector>

#include "common.h"
#include "Controller.h"

#include <msclr/marshal_cppstd.h>

using namespace std;
using namespace System;
using namespace System::Collections::Generic;

namespace ControllerWrapper {


	public ref class ControllerAccess
	{
		// TODO: Add your methods for this class here.
	
		Controller* controller;

	public:
		ControllerAccess() {}
		~ControllerAccess() {}
		ControllerAccess(const ControllerAccess ^rhs) {}

		string sysStrToStdStr(System::String^ str)
		{
			string cppString = msclr::interop::marshal_as<std::string>(str);
			return cppString;
		}

		void initiate()
		{
			controller = new Controller();
		}

		void clear()
		{
			if (controller) {delete controller; controller = NULL;}
		}

		bool isActive()
		{
			if (controller)
			{
				return true;
			}
			else
			{
				return false;
			}
		}

		void setControllerCalibration(uInt_32 uiNumPixel, uInt_32 uiNumAscan, uInt_32 uiNumBscan, uInt_32 uiNumBcgrFrame, uInt_32 uiNumBscanRepeat, int iPreviewAScan, int iBackgroundAScan, int iFundusViewAScan, System::String^ fbackground, System::String^ fpreviewxz, System::String^ fpreviewyz, System::String^ fAcquireDataSet, System::String^ fFundusDataSet)
		{
			string _fbackground = sysStrToStdStr(fbackground);
			string _fpreviewxz = sysStrToStdStr(fpreviewxz);
			string _fpreviewyz = sysStrToStdStr(fpreviewyz);
			string _fAcquireDataSet = sysStrToStdStr(fAcquireDataSet);
			string _fFundusDataSet = sysStrToStdStr(fFundusDataSet);

			controller->setControllerCalibration(uiNumPixel, uiNumAscan, uiNumBscan, uiNumBcgrFrame, uiNumBscanRepeat, iPreviewAScan, iBackgroundAScan, iFundusViewAScan, _fbackground, _fpreviewxz, _fpreviewyz, _fAcquireDataSet, _fFundusDataSet);
		}

		void resetAcquireDataSet(System::String^ fAcquireDataSet)
		{
			string _fAcquireDataSet = sysStrToStdStr(fAcquireDataSet);
			controller->resetAcquireDataSet(_fAcquireDataSet);
		}
		// Need to init trajectory again after call resetScanPattern, but not needed for setScanPattern.
		void setScanPattern(float fXRangeV, float fYRangeV, float fXOffsetV, float fYOffsetV, uInt_32 uiNumPixel, uInt_32 uiNumAscan, uInt_32 uiNumBscan, uInt_32 uiNumBcgrFrame, uInt_32 uiNumBscanRepeat)
		{
			controller->setScanPattern(fXRangeV, fYRangeV, fXOffsetV, fYOffsetV, uiNumPixel, uiNumAscan, uiNumBscan, uiNumBcgrFrame, uiNumBscanRepeat);
		}

		void resetScanPattern(float fXRangeV, float fYRangeV, float fXOffsetV, float fYOffsetV, uInt_32 uiNumPixel, uInt_32 uiNumAscan, uInt_32 uiNumBscan, uInt_32 uiNumBcgrFrame, uInt_32 uiNumBscanRepeat)
		{
			controller->resetScanPattern(fXRangeV, fYRangeV, fXOffsetV, fYOffsetV, uiNumPixel, uiNumAscan, uiNumBscan, uiNumBcgrFrame, uiNumBscanRepeat);
		}

		// Need to init trajectory again after call resetTrajectoryCalibration, but not needed for setTrajectoryCalibration.
		void setTrajectoryCalibration(float fXMaxV, float fYMaxV, float fXScaleVM, float fYScaleVM, uInt_32 uiUpdateRate, float fWaveEffi, System::String^ sPhyChannel, float fExposureTime, int iPreviewAScan, int iBackgroundAScan, int iFundusViewAScan, float fFundusXOffsetV, float fFundusYOffsetV)
		{
			string _sPhyChannel = sysStrToStdStr(sPhyChannel);
			controller->setTrajectoryCalibration(fXMaxV, fYMaxV, fXScaleVM, fYScaleVM, uiUpdateRate, fWaveEffi, _sPhyChannel, fExposureTime, iPreviewAScan, iBackgroundAScan, iFundusViewAScan, fFundusXOffsetV, fFundusYOffsetV);
		}

		void resetTrajectoryCalibration(float fXMaxV, float fYMaxV, float fXScaleVM, float fYScaleVM, uInt_32 uiUpdateRate, float fWaveEffi, System::String^ sPhyChannel, float fExposureTime, int iPreviewAScan, int iBackgroundAScan, int iFundusViewAScan, float fFundusXOffsetV, float fFundusYOffsetV)
		{
			string _sPhyChannel = sysStrToStdStr(sPhyChannel);
			controller->resetTrajectoryCalibration(fXMaxV, fYMaxV, fXScaleVM, fYScaleVM, uiUpdateRate, fWaveEffi, _sPhyChannel, fExposureTime, iPreviewAScan, iBackgroundAScan, iFundusViewAScan, fFundusXOffsetV, fFundusYOffsetV);
		}

		int initTrajectory()
		{
			return controller->initTrajectory();
		}

		void setDacCalibration(float fXMaxV, float fYMaxV, float fXScaleVM, float fYScaleVM, System::String^ sXChanName, System::String^ sYChanName, System::String^ sSwitchChanName, bool bExterClked, System::String^ sExterClkChanName, uInt_32 ulTrajGenRate)
		{
			string _sXChanName = sysStrToStdStr(sXChanName);
			string _sYChanName = sysStrToStdStr(sYChanName);
			string _sSwitchChanName = sysStrToStdStr(sSwitchChanName);
			string _sExterClkChanName = sysStrToStdStr(sExterClkChanName);

			controller->setDacCalibration(fXMaxV, fYMaxV, fXScaleVM, fYScaleVM, _sXChanName, _sYChanName, _sSwitchChanName, bExterClked, _sExterClkChanName, ulTrajGenRate);
		}
		void initNIDacCard()
		{
			controller->initNIDacCard();
		}

		// set PrcssrCalibration for preview
		void setPreviewPrcssrCalibration(int batchFrames, int batchAscans, int prevFreq, bool reslice, int numAscansPerBscan, int numBScans, int numCameraPixels, int numBgFrame,
			int startPixel, int endPixel, float alpha, int grayLevel, bool interpLinear, bool dispModeGS, int dispFrame, int dispTests, float dispA2, float dispA3,
			System::String^ fnamePhase, System::String^ fnameBg, System::String^ fnameData)
		{
			string _fnamePhase = sysStrToStdStr(fnamePhase);
			string _fnameBg = sysStrToStdStr(fnameBg);
			string _fnameData = sysStrToStdStr(fnameData);

			controller->setPreviewPrcssrCalibration(batchFrames, batchAscans, prevFreq, reslice, numAscansPerBscan, numBScans, numCameraPixels, numBgFrame,
				startPixel, endPixel, alpha, grayLevel, interpLinear, dispModeGS, dispFrame, dispTests, dispA2, dispA3,
				_fnamePhase, _fnameBg, _fnameData);
		}

		// set PrcssrCalibration for acquire mode
		void setFundusPrcssrCalibration(int batchFrames, int batchAscans, int prevFreq, bool reslice, int numAscansPerBscan, int numBScans, int numCameraPixels, int numBgFrame,
			int startPixel, int endPixel, float alpha, int grayLevel, bool interpLinear, bool dispModeGS, int dispFrame, int dispTests, float dispA2, float dispA3,
			System::String^ fnamePhase, System::String^ fnameBg, System::String^ fnameData)
		{
			string _fnamePhase = sysStrToStdStr(fnamePhase);
			string _fnameBg = sysStrToStdStr(fnameBg);
			string _fnameData = sysStrToStdStr(fnameData);

			controller->setFundusPrcssrCalibration(batchFrames, batchAscans, prevFreq, reslice, numAscansPerBscan, numBScans, numCameraPixels, numBgFrame,
				startPixel, endPixel, alpha, grayLevel, interpLinear, dispModeGS, dispFrame, dispTests, dispA2, dispA3,
				_fnamePhase, _fnameBg, _fnameData);
		}

		// set PrcssrCalibration for acquire mode
		void setAcquirePrcssrCalibration(int batchFrames, int batchAscans, int prevFreq, bool reslice, int numAscansPerBscan, int numBScans, int numCameraPixels, int numBgFrame, int numBScansRepeat,
			int startPixel, int endPixel, float alpha, int grayLevel, bool interpLinear, bool dispModeGS, int dispFrame, int dispTests, float dispA2, float dispA3,
			System::String^ fnamePhase, System::String^ fnameBg, System::String^ fnameData)
		{
			string _fnamePhase = sysStrToStdStr(fnamePhase);
			string _fnameBg = sysStrToStdStr(fnameBg);
			string _fnameData = sysStrToStdStr(fnameData);

			controller->setAcquirePrcssrCalibration(batchFrames, batchAscans, prevFreq, reslice, numAscansPerBscan, numBScans, numCameraPixels, numBgFrame, numBScansRepeat,
				startPixel, endPixel, alpha, grayLevel, interpLinear, dispModeGS, dispFrame, dispTests, dispA2, dispA3,
				_fnamePhase, _fnameBg, _fnameData);
		}

		void resetDispersion(bool dispModeGS, int dispFrame, int dispTests, float dispA2, float dispA3)
		{
			controller->resetDispersion(dispModeGS, dispFrame, dispTests, dispA2, dispA3);
		}

		int getAcquireDataSet()
		{
			return controller->getAcquireDataSet(); // get the scan data of the whole sample
		}

		int getPreviewXZ()
		{
			return controller->getPreviewXZ();
		}

		int getPreviewYZ()
		{
			return controller->getPreviewYZ();
		}

		int getFundusDataSet()
		{
			return controller->getFundusDataSet();
		}
		int getBackground()
		{
			return controller->getBackground();
		}

		int getData(WorkingModel_e mode)
		{
			return controller->getData(mode);
		}

		int processPreviewBcgr()
		{
			return controller->processPreviewBcgr();
		}

		int processFundusBcgr()
		{
			return controller->processFundusBcgr();
		}
		int processPreviewBcgr(bool dispModeGS, float &a2, float &a3)
		{
			return controller->processPreviewBcgr(dispModeGS, a2, a3);
		}

		int processPreviewFrameData(System::String^ datafile)
		{
			string _datafile = sysStrToStdStr(datafile);
			return controller->processPreviewFrameData(_datafile);
		}

		int processFundusFrameData(System::String^ datafile)
		{
			string _datafile = sysStrToStdStr(datafile);
			return controller->processFundusFrameData(_datafile);
		}

		int processAcquireBcgr()
		{
			return controller->processAcquireBcgr();
		}

		int processAcquireFrameData(System::String^ datafile)
		{
			string _datafile = sysStrToStdStr(datafile);
			return controller->processAcquireFrameData(_datafile);
		}
	
public:
		//std::vector<short> getSpectrum()
		cliext::vector<int>::generic_container ^getSpectrum()
		{
			std::vector<short> data;
			if (controller)
			{
				data = controller->getSpectrum();
			}
			//return data;
			cliext::vector<int>::generic_container ^re = gcnew cliext::vector<int>;
			if (data.size()>1024)
			{
				for (int i=0; i<1024; i++)
				{
					re->push_back(data.at(i));
				}
			}
			
			return re;
		}
	};
}
