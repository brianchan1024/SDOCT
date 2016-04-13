
#include <iostream>
#include <string>
#include "common.h"
#include "TrajectoryApi.h"
#include "Trajectory.h"
#include "NIDacCard.h"

#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
//#include "Ctr_logger.h"

using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

using namespace std;

// calibration params...
float fXMaxV = 9;
float fYMaxV = 9;
float fXScaleVM = 0.98; // [v/mm]
float fYScaleVM = 0.98; // [v/mm]

uInt_32 uiUpdateRate = 40000; //[points/sec]
float fWaveEffi = 0.85;
std::string sPhyChannel = "Dev1";

float fExposureTime = 0.000022;

float fFundusXOffsetV = 2 * fXScaleVM;
float fFundusYOffsetV = 2 * fYScaleVM;

// scanpattern params...
float fXRangeV = 8;
float fYRangeV = 6;
float fXOffsetV = 0;
float fYOffsetV = 0;

uInt_32 uiNumPixel = 1024;
uInt_32 uiNumAscan = 400;
uInt_32 uiNumBscan = 30;
uInt_32 uiNumBcgrFrame = 0; // background frames
uInt_32 uiNumBscanRepeat = 2;

//dac calinration
std::string sXChanName = "Dev1/ao1";
std::string sYChanName = "Dev1/ao2";
std::string sSwitchChanName = "Dev1/ao0";
bool bExterClked = false;
std::string sExterClkChanName = "PFI5";
unsigned long ulTrajGenRate = uiUpdateRate;

int iPreviewAScan = 400;
int iBackgroundAScan = 400;

int iFundusViewAScan = 200;

Trajectory* getTrajectory()
{
	TrajectoryApi* trajApi = new TrajectoryApi();
	trajApi->setTrajectoryCalibration(fXMaxV, fYMaxV, fXScaleVM, fYScaleVM, uiUpdateRate, fWaveEffi, sPhyChannel, fExposureTime, iPreviewAScan, iBackgroundAScan, iFundusViewAScan, fFundusXOffsetV, fFundusYOffsetV);
	trajApi->setScanPattern(fXRangeV, fYRangeV, fXOffsetV, fYOffsetV, uiNumPixel, uiNumAscan, uiNumBscan, uiNumBcgrFrame, uiNumBscanRepeat);

	Trajectory* prevTrajXZ = trajApi->getPreviewTraj(0);
	Trajectory* prevTrajYZ = trajApi->getPreviewTraj(1);
	Trajectory* acqTraj = trajApi->getAcquireTraj();
	Trajectory* fundusTraj = trajApi->getFundusViewTraj();
	return fundusTraj;
}

#if 1

int main()
{
	
	std::remove("../log/sdoct_0.log");
	Trajectory* trajectory = getTrajectory();

	cout<<"-------------------\n trajectory out:\n"<<endl;
	trajectory->printTrajectory();
	
	
	DacCalibration* dacCali = new DacCalibration(fXMaxV, fYMaxV, fXScaleVM, fYScaleVM,
	                                sXChanName, sYChanName, sSwitchChanName,
                                    bExterClked, sExterClkChanName, ulTrajGenRate);
	NIDacCard* dacCard = new NIDacCard(dacCali);
	dacCard->setSafePosition();
	dacCard->setTrajectory(trajectory);
	

	cout<<"-------------------\n trajectory out:\n"<<endl;
	trajectory->printTrajectory();

	

	system("pause");
	return 0;
}

#else

// try graphics here
VOID OnPaint(HDC hdc)
{
   Graphics graphics(hdc);
  
   Pen* pen = new Pen(Color(0, 255, 0, 255));
   // get data
   
   Trajectory* traj = getTrajectory();
   std::vector<TrajectoryPoint_t> points = traj->getTrajactoryPoints();
   int scale = 70;
   int offset = 450;
   char buf[2048];
   sprintf(buf,"points total size: %d \n",points.size());OutputDebugString(buf);
   
   for(std::vector<TrajectoryPoint_t>::iterator iter=points.begin(); iter!=points.end()-1; ++iter) {

	   int x = (*iter).x * scale+offset;
	   int y = (*iter).y * scale+offset;
	   int x2 = (*(iter+1)).x * scale+offset;
	   int y2 = (*(iter+1)).y * scale+offset;

	    //Sleep(10);
		switch((*iter).type) {
			case TP_ACQUIRE:
				pen->SetColor(Color(255, 0, 0, 255));
				sprintf(buf,"TP_ACQUIRE points: x:%f  y:%f \n",(*iter).x, (*iter).y); OutputDebugString(buf);
				break;
			case TP_SLOPE:
				pen->SetColor(Color(255, 255, 0, 0));
				break;
			case TP_FLYBACK:
				pen->SetColor(Color(255, 0, 255, 0));
				break;
			case TP_RAMP:
				pen->SetColor(Color(255, 0, 0, 0));
				break;
			case TP_BAD:
				pen->SetColor(Color(255, 255, 255, 0));
				break;
			}
	   
	   

	   

	   //if (abs(x+y-x2-y2)<10)
	   if (abs(x+y-x2-y2))
	   {
		   //pen->SetWidth(4);
		   pen->SetWidth(1);
		   graphics.DrawLine(pen, x, y, x+2, y+2);
		   //graphics.DrawLine(pen, x, y, x2, y2);
		   //Sleep(20);
		   //pen->SetColor(Color(255, 255, 255, 255));
		   //graphics.DrawLine(pen, x, y, x+2, y+2);
	   } else{
			pen->SetColor(Color(255, 0, 0, 0));
			pen->SetWidth(0);
			graphics.DrawLine(pen, x, y, x+2, y+2);
			//graphics.DrawLine(pen, x, y, x2, y2);
	   }
	   
	   //graphics.DrawLine(pen, x, y, x+1, y+1);
	   
   }
   pen->SetColor(Color(255, 255, 0, 0));
   pen->SetWidth(2);
   graphics.DrawLine(pen, offset, offset, offset+2, offset+2);

   
   int x1=(-fXRangeV/2+fXOffsetV)*scale+offset, x2=(fXRangeV/2+fXOffsetV)*scale+offset, y1=(-fYRangeV/2+fYOffsetV)*scale+offset, y2=(fYRangeV/2+fYOffsetV)*scale+offset;
   graphics.DrawLine(pen, x1, y1, x1, y2);
   graphics.DrawLine(pen, x2, y1, x2, y2);
   graphics.DrawLine(pen, x1, y1, x2, y1);
   graphics.DrawLine(pen, x1, y2, x2, y2);
   

   pen->SetColor(Color(255, 0, 0, 255));
   graphics.DrawLine(pen, 10, 10, 20, 10);

   pen->SetColor(Color(255, 0, 0, 0));
   graphics.DrawLine(pen, 10, 20, 20, 20);

   pen->SetColor(Color(255, 255, 0, 0));
   graphics.DrawLine(pen, 10, 30, 20, 30);

   pen->SetColor(Color(255, 0, 255, 0));
   graphics.DrawLine(pen, 10, 40, 20, 40);
   
   /*
   int scale = 250;
   int scaley = 50;
   int offset = 200;
   Trajectory* traj = getTrajectory();
   std::vector<float> ys = traj->getTrajectoryY();
   float idx = 0;
   pen->SetColor(Color(255, 255, 0, 0));
   pen->SetWidth(2);
   for(std::vector<float>::iterator iter=ys.begin(); iter!=ys.end()-1; ++iter) {
	   idx+=0.01;
	   float x1 = (idx) * scale + offset;
	   float x2 = (idx) * scale + offset + 1;
	   float y1 = (*iter +fYRangeV/2) * scaley + offset;
	   float y2 = (*iter+fYRangeV/2) * scaley + offset;
	   graphics.DrawLine(pen, x1, y1, x2, y2);
   }
   */
   OutputDebugString("hello world\n");
   
}

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, PSTR, INT iCmdShow)
{
   OutputDebugString("hello world\n");
   HWND                hWnd;
   MSG                 msg;
   WNDCLASS            wndClass;
   GdiplusStartupInput gdiplusStartupInput;
   ULONG_PTR           gdiplusToken;
   
   // Initialize GDI+.
   GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
   
   wndClass.style          = CS_HREDRAW | CS_VREDRAW;
   wndClass.lpfnWndProc    = WndProc;
   wndClass.cbClsExtra     = 0;
   wndClass.cbWndExtra     = 0;
   wndClass.hInstance      = hInstance;
   wndClass.hIcon          = LoadIcon(NULL, IDI_APPLICATION);
   wndClass.hCursor        = LoadCursor(NULL, IDC_ARROW);
   wndClass.hbrBackground  = (HBRUSH)GetStockObject(WHITE_BRUSH);
   wndClass.lpszMenuName   = NULL;
   wndClass.lpszClassName  = TEXT("GettingStarted");
   
   RegisterClass(&wndClass);
   
   hWnd = CreateWindow(
      TEXT("GettingStarted"),   // window class name
      TEXT("Getting Started"),  // window caption
      WS_OVERLAPPEDWINDOW,      // window style
      CW_USEDEFAULT,            // initial x position
      CW_USEDEFAULT,            // initial y position
      CW_USEDEFAULT,            // initial x size
      CW_USEDEFAULT,            // initial y size
      NULL,                     // parent window handle
      NULL,                     // window menu handle
      hInstance,                // program instance handle
      NULL);                    // creation parameters
	  
   ShowWindow(hWnd, iCmdShow);
   UpdateWindow(hWnd);
   
   while(GetMessage(&msg, NULL, 0, 0))
   {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
   }
   
   GdiplusShutdown(gdiplusToken);
   return msg.wParam;
}  // WinMain

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, 
   WPARAM wParam, LPARAM lParam)
{
   HDC          hdc;
   PAINTSTRUCT  ps;
   
   switch(message)
   {
   case WM_PAINT:
      hdc = BeginPaint(hWnd, &ps);
      OnPaint(hdc);
      EndPaint(hWnd, &ps);
      return 0;
   case WM_DESTROY:
      PostQuitMessage(0);
      return 0;
   default:
      return DefWindowProc(hWnd, message, wParam, lParam);
   }

} // WndProc

#endif