// C_NI_PCIe1429.cpp : Defines the exported functions for the DLL application.
//
#include <omp.h>
#include <windows.h>
#include <mmsystem.h>
#include <stdio.h>
#include "niimaq.h" //includes the functions of the IMAQ library
//#include "fftw3.h"
#include <conio.h>
#include <stdlib.h>
#define _USE_MATH_DEFINES
#include "math.h"
#include <string.h>
#include <iostream>
#include <tchar.h>
#include "bspline.h"
#include "MAQEngine.h"
#include <vector>
#include <fstream>

#include "DAQ_logger.h"

//#define LAMBDA_0 859 // this is for the Basler High Speed B2 system with 160mm lens and 1200 grating

//fixation target constants
#define LED_AUTOBAUD 85
#define LED_CLEARSCREEN 69
#define LED_DRAWrect 114

// error checking macro
#define errChk(fCall) if (error = (fCall), error < 0) {goto Error;} else

#define IMG_ATTR_EXTENDED_MEMORY (_IMG_BASE + 0x0539) 

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

int FindLambda (void *acqblk, int lambda0, int bandwidth, float *locArray)
{
	struct AcqBlock *ab = (struct AcqBlock *)acqblk;

	//find lambda k ranges
	//no (int) terms or else (float) will round to integer
	float pixellambda = (float)bandwidth / (float)(*ab).NbPixel;
	float minlambda = (float)lambda0 - (float)bandwidth/(float)2.0 + (float)0.5*pixellambda;
	float maxlambda = (float)lambda0 + (float)bandwidth/(float)2.0 - (float)0.5*pixellambda;
	float minK = (float)2.0 * (float)M_PI/maxlambda;
	float maxK = (float)2.0 * (float)M_PI/minlambda;
	//find k increment value for 2*ib->numPixels points (consistent with float points for interpolated values)
	float Kinc = (maxK - minK)/(float)((*ab).NbPixel);

	#pragma omp parallel for
	for (int i=0; i<(*ab).NbPixel; i++)
	{
		locArray[i] = ((float)2.0*(float)M_PI/(maxK-Kinc*(i+(float)0.5)) - minlambda) / pixellambda;
	}
	return 0;
}

int LambdaK (void *acqblk, float *locArray, float *rawfringe, float *Kfringe)
{
	struct AcqBlock *ab = (struct AcqBlock *)acqblk;

	Bspline bs((*ab).NbPixel, (*ab).NbPixel);
	//Build spline interpolation matrix
	bs.spline_coeff(rawfringe);
	//Perform spline
	bs.b_spline(locArray, Kfringe);
	
	return 0;

}// end OpenMP_LambdaK

int grablastframe(uInt16 *imagePtrGrabber, void *acqblk)
{
	Int32 error;
	struct AcqBlock *ab = (struct AcqBlock *)acqblk;
 	uInt16 *BackgroundAddr;
	int bufferNumber;	

	// grab last buffer
	errChk(imgSessionExamineBuffer2((*ab).Sid, IMG_LAST_BUFFER, (uInt32 *)&bufferNumber, (void**)&BackgroundAddr));

	// copy buffer
	memcpy(imagePtrGrabber, (uInt16*)BackgroundAddr, (*ab).NbPixel*(*ab).NbLine*sizeof(uInt16));

	// release buffer
	errChk(imgSessionReleaseBuffer ((*ab).Sid));

	return bufferNumber;

	Error :
    if(error<0)
    {
		imgSessionAbort((*ab).Sid, NULL);
		imgClose((*ab).Sid, TRUE);
		imgClose((*ab).Iid, TRUE);        
		DisplayIMAQError(error);
    }
	return 0;

}// end GrabLastFrame


int omp_processFundus(uInt16 *BuffPtr, float *linePtrGrabber, float *Background,int currentframe, void *acqblk)
{
	struct AcqBlock *ab = (struct AcqBlock *)acqblk;

	//if (currentframe == 0)
	//{	
		#pragma omp parallel for
		for(int pixelcounter = 0; pixelcounter <  (*ab).NbPixel; pixelcounter++)
		{
			Background[pixelcounter] = 0;
			for(int linecounter = 0; linecounter < (*ab).NbLine; linecounter++)
			{
				Background[pixelcounter] += ((uInt16*)BuffPtr)[linecounter*(*ab).NbPixel+ pixelcounter];
			}
			Background[pixelcounter] = Background[pixelcounter]/(*ab).NbLine;
		}
	//}

	int frameoffset = currentframe*(*ab).NbLine;

	// subtract background and sum
	#pragma omp parallel for
	for(int lcounter = 0; lcounter < (*ab).NbLine; lcounter++)
	{
		int BuffPtroffset = lcounter*(*ab).NbPixel;
		int linePtroffset = frameoffset + lcounter;
		linePtrGrabber[lcounter] = 0;
		for(int pcounter = 0; pcounter <  (*ab).NbPixel; pcounter++)
		{
			linePtrGrabber[linePtroffset]+= abs(((uInt16*)BuffPtr)[BuffPtroffset+ pcounter]-Background[pcounter]);
		}
	}

	return currentframe;
}// end omp_processFundus

int DisplayIMAQError(Int32 error)
{
	FILE *debug_file;

    static Int8 ErrorMessage[256];
    memset(ErrorMessage, 0x00, sizeof(ErrorMessage));

    // converts error code to a message
    imgShowError(error, ErrorMessage);

	//save error meaage in file
	fopen_s( &debug_file, "E:\\test\\error.txt", "w");
	fwrite(ErrorMessage, sizeof(ErrorMessage), 1, debug_file);
	fwrite("\n", sizeof("\n"), 1, debug_file);
	fclose(debug_file);
	DAQ_LOGGER_ERROR << __FUNCTION__ << " ErrorMessage:" << ErrorMessage;
	return 0;

} // end of DisplayIMAQError

void *InitializeSession(const char* interface_name, int numPixels, int numLines)
{
	//printf("in InitializeSession; numPixels:%d, numLines:%d.\n", numPixels, numLines);
	struct InitAcqBlock *initacqblk = (struct InitAcqBlock *) malloc(sizeof(struct InitAcqBlock));	

	Int32 error;

	SESSION_ID   Sid = 0;
	INTERFACE_ID Iid = 0;

	errChk(imgInterfaceOpen(interface_name, &Iid));
	errChk(imgSessionOpen(Iid, &Sid));
	//errChk(imgSetAttribute (Sid, IMG_ATTR_EXTENDED_MEMORY, 0));

	// Set attribute
	errChk(imgSetAttribute2 (Sid, IMG_ATTR_ACQWINDOW_WIDTH, numPixels));
	errChk(imgSetAttribute2 (Sid, IMG_ATTR_ACQWINDOW_HEIGHT, numLines));
	errChk(imgSetAttribute2 (Sid, IMG_ATTR_ACQWINDOW_WIDTH, numPixels));
	errChk(imgSetAttribute2 (Sid, IMG_ATTR_ACQWINDOW_HEIGHT, numLines));
	//errChk(imgSetAttribute2 (Sid, IMG_ATTR_ACQWINDOW_HEIGHT, 1));

	// reset works @brian
	//errChk(imgSetAttribute2 (Sid, IMG_ATTR_ROI_WIDTH, numPixels));
	////errChk(imgSetAttribute2 (Sid, IMG_ATTR_ROI_HEIGHT, numLines));
	//errChk(imgSetAttribute2 (Sid, IMG_ATTR_ROI_HEIGHT, 1));
	//errChk(imgSetAttribute2 (Sid, IMG_ATTR_ROWPIXELS, numPixels)); 
	// create acqblk
	(*initacqblk).Sid = Sid;
	(*initacqblk).Iid = Iid;

	//printf("in InitializeSession 2 %d. %d\n", Sid, Iid);

	return initacqblk;

	//if no preview is needed, pass in 0 for all image pointers.
	
Error :
    if(error<0)
    {
        DisplayIMAQError(error);
		//printf("in InitializeSession error.\n");
		DAQ_LOGGER_ERROR << __FUNCTION__ << " InitializeSession error";
    }
	return 0;
} // end of InitializeSessionRTFFT


bool EndSession(void *initacqblk)
{
	Int32 error;

	struct InitAcqBlock *ab = (struct InitAcqBlock *)initacqblk;	

	errChk(imgClose((*ab).Sid, TRUE));
	errChk(imgClose((*ab).Iid, TRUE));
	//printf("in EndSession.\n");
	return true;
Error :
    if(error<0)
    {
		//printf("in EndSession error.\n");
        DisplayIMAQError(error);
		DAQ_LOGGER_ERROR << __FUNCTION__ << "EndSession error";
		throw;
    }
	return false;
} // end of EndSession

void *AllocateMemory(void *initacqblk, long NbPixel, long NbLine, long NbFrame)
{
	BUFLIST_ID	 Bid;

	int i, error;
	struct InitAcqBlock *ab = (struct InitAcqBlock *)initacqblk;
	struct AcqBlock *acqblk = (struct AcqBlock *) malloc(sizeof(struct AcqBlock));
	uInt32    bufCmd, bufSize, bytesPerPixel;
	// manually define Imaqbuffer number
	static Int8 *ImaqBuffers[10000];

	(*acqblk).Iid = (*ab).Iid;
	(*acqblk).Sid = (*ab).Sid;

	// Compute the size of the required buffer
	errChk(imgGetAttribute ((*acqblk).Sid, IMG_ATTR_BYTESPERPIXEL, &bytesPerPixel));
	bufSize = NbLine * NbPixel * bytesPerPixel;

	// Create a buffer list with n elements
	errChk(imgCreateBufList(NbFrame, &Bid));

	/* The following configuration makes theseassignments to the buffer list elements: 
	buffer pointer that will contain image 
	size of the buffer for buffer element 0 
	command to go to next buffer or loop when the last element is reached */
	//printf("in AllocateMemory 0. NbLine:%d, NbPixel:%d, bytesPerPixel:%d\n", NbLine, NbPixel, bytesPerPixel);
	for (i = 0; i < NbFrame; i++)
	{
		//printf("in AllocateMemory 1 Sid:%d. bufSize:%d\n", (*acqblk).Sid, bufSize);
		errChk(imgCreateBuffer((*acqblk).Sid, IMG_HOST_FRAME, bufSize, (void**)&ImaqBuffers[i]));
		//printf("in AllocateMemory 2");
		errChk(imgSetBufferElement2(Bid, i, IMG_BUFF_ADDRESS, (uInt32)ImaqBuffers[i]));
		errChk(imgSetBufferElement2(Bid, i, IMG_BUFF_SIZE, bufSize));
		//printf("in AllocateMemory 3.\n");
		bufCmd = (i == (NbFrame - 1)) ? IMG_CMD_LOOP : IMG_CMD_NEXT;
		errChk(imgSetBufferElement2(Bid, i, IMG_BUFF_COMMAND, bufCmd));
	}

	// Lock down the buffers contained in the buffer list
	errChk(imgMemLock(Bid));

	(*acqblk).Bid = Bid;
	(*acqblk).NbPixel = (int)NbPixel;
	(*acqblk).NbLine = (int)NbLine;
	(*acqblk).NbFrame = (int)NbFrame;

	return acqblk;

	Error :
    if(error<0)
    {
        DisplayIMAQError(error);
		//printf("in AllocateMemory error.\n");
		DAQ_LOGGER_ERROR << __FUNCTION__ << "AllocateMemory error";
    }

	return 0;
} //end AllocateMemory
	
void *AllocateACQMemory(void *initacqblk, long NbPixel, uInt32 *ListNbLine, long NbFrame, long iterations)
{
	//std::cout<<"Debug: "<<__FUNCTION__<<" ListNbLine: "<<ListNbLine[0]<<std::endl;

	//system("pause");
	BUFLIST_ID	 Bid;

	int i, error;
	struct InitAcqBlock *ab = (struct InitAcqBlock *)initacqblk;
	struct AcqBlock *acqblk = (struct AcqBlock *) malloc(sizeof(struct AcqBlock));
	uInt32    bufCmd, bufSize, bytesPerPixel;
	uInt16   *MemQue;
	// manually define Imaqbuffer number
	static Int8 *ImaqBuffers[10000];

	(*acqblk).Iid = (*ab).Iid;
	(*acqblk).Sid = (*ab).Sid;

	// Compute the size of the required buffer
	errChk(imgGetAttribute ((*acqblk).Sid, IMG_ATTR_BYTESPERPIXEL, &bytesPerPixel));

	// Create a buffer list with n elements
	errChk(imgCreateBufList(NbFrame, &Bid));

	// for finding maximum number of lines
	uInt32 MaxNbLine=ListNbLine[0];
	// for calculating memory queue size
	uInt32 bufSizeTotal=0;
	//printf("in AllocateACQMemory 2. bytesPerPixel:%d, \n", bytesPerPixel);
	for (i = 0; i < NbFrame; i++)
	{
		bufSize = ListNbLine[i] * NbPixel * bytesPerPixel;
		//printf("in AllocateACQMemory 2.5 Sid:%d. bufSize:%d\n", (*acqblk).Sid, bufSize);
		errChk(imgCreateBuffer((*acqblk).Sid, IMG_HOST_FRAME, bufSize, (void**)&ImaqBuffers[i]))
		//errChk(imgCreateBuffer((*acqblk).Sid, IMG_HOST_FRAME, 409600, (void**)&ImaqBuffers[i]))
		//printf("in AllocateACQMemory 3.\n");
		errChk(imgSetBufferElement2(Bid, i, IMG_BUFF_ADDRESS, (uInt32)ImaqBuffers[i]));
		//printf("in AllocateACQMemory 4.\n");
		errChk(imgSetBufferElement2(Bid, i, IMG_BUFF_SIZE, bufSize));
		bufCmd = (i == (NbFrame - 1)) ? IMG_CMD_LOOP : IMG_CMD_NEXT;
		errChk(imgSetBufferElement2(Bid, i, IMG_BUFF_COMMAND, bufCmd));
		//printf("in AllocateACQMemory 5.\n");
		if(ListNbLine[i] > MaxNbLine)
		{
			MaxNbLine = ListNbLine[i];
		}
		bufSizeTotal += ListNbLine[i] * NbPixel;
	}
	//printf("in AllocateACQMemory 6.\n");
	// Lock down the buffers contained in the buffer list
	errChk(imgMemLock(Bid));

	// allocate memory queue
	MemQue = (uInt16 *) malloc(bufSizeTotal*iterations*sizeof(uInt16));

	(*acqblk).Bid = Bid;
	(*acqblk).NbPixel = NbPixel;
	(*acqblk).NbLine = MaxNbLine;
	(*acqblk).NbFrame = NbFrame;
	(*acqblk).MemQue = MemQue;
	//printf("in AllocateACQMemory; NbPixel:%d, NbLine:%d.\n", (*acqblk).NbPixel, (*acqblk).NbLine);
	return acqblk;

	Error :
    if(error<0)
    {
		DAQ_LOGGER_ERROR << __FUNCTION__ << "AllocateACQMemory error";
        DisplayIMAQError(error);
		//printf("in AllocateACQMemory error.\n");
    }

	return 0;
} //end AllocateACQMemory

bool DeallocateMemory(void *acqblk)
{
	Int32 error;
	struct AcqBlock *ab = (struct AcqBlock *)acqblk;	

	// Free the grab buffer
	//printf("in DeallocateMemory 0.\n");
	errChk(imgDisposeBufList((*ab).Bid, TRUE));
	//printf("in DeallocateMemory end.\n");
	return true;

	Error :
    if(error<0)
    {
		//printf("in DeallocateMemory error.\n");
		DAQ_LOGGER_ERROR << __FUNCTION__ << "DeallocateMemory error";
        DisplayIMAQError(error);
    }
	return false;
} //end DeallocateMemory

bool DeallocateQueMemory(void *acqblk)
{
	Int32 error;
	struct AcqBlock *ab = (struct AcqBlock *)acqblk;	

	// Free the grab buffer
	//printf("in DeallocateQueMemory 0.\n");
	errChk(imgDisposeBufList((*ab).Bid, TRUE));
	//printf("in DeallocateQueMemory 1.\n");
	// Free the queue memory
	//free((*ab).MemQue); //@brain, original keep
	//printf("in DeallocateQueMemory end.\n");
	return true;

	Error :
    if(error<0)
    {
		//printf("in DeallocateQueMemory error.\n");
		DAQ_LOGGER_ERROR << __FUNCTION__ << "DeallocateQueMemory error";
        DisplayIMAQError(error);
    }
	return false;
} //end DeallocateQueMemory

void *StartGrabImage(void *acqblk)
{
	Int32 error;
	struct AcqBlock *ab = (struct AcqBlock *)acqblk;
	CALL_BACK_PTR callback = NULL;

	//printf("in StartGrabImage. (*ab).NbPixel:%d, (*ab).NbLine:%d\n", (*ab).NbPixel, (*ab).NbLine);

	//Set session attributes
	errChk(imgSetAttribute2 ((*ab).Sid, IMG_ATTR_ACQWINDOW_WIDTH, (*ab).NbPixel));
	errChk(imgSetAttribute2 ((*ab).Sid, IMG_ATTR_ACQWINDOW_HEIGHT, (*ab).NbLine));

	// Set trigger
	errChk(imgSessionTriggerConfigure2((*ab).Sid, IMG_SIGNAL_EXTERNAL, IMG_EXT_TRIG0, IMG_TRIG_POLAR_ACTIVEH, IMG_FRAMETIME_2SECONDS, IMG_TRIG_ACTION_BUFFER));
	//errChk(imgSessionLineTrigSource2((*ab).Sid, IMG_SIGNAL_EXTERNAL, 0, IMG_TRIG_POLAR_ACTIVEH, 0));

	// Configure the session to use this buffer list
	errChk(imgSessionConfigure((*ab).Sid, (*ab).Bid));

	//IMAQ start
	errChk(imgSessionAcquire((*ab).Sid, TRUE, callback));
	//printf("in StartGrabImage 1.\n");
	return (void *)(*ab).Sid;

	Error :
    if(error<0)
    {
		//printf("in StartGrabImage error.\n");
		DAQ_LOGGER_ERROR << __FUNCTION__ << " StartGrabImage error";
		imgSessionAbort((*ab).Sid, NULL);
		imgClose((*ab).Sid, TRUE);
		imgClose((*ab).Iid, TRUE);
		DisplayIMAQError(error);
    }
	return 0;

} // end StartGrabImage

bool StopGrabImage(void *acqblk)
{
	Int32 error;
	struct AcqBlock *ab = (struct AcqBlock *)acqblk;
	// stop the acquisition
    imgSessionAbort((*ab).Sid, NULL);
	//printf("in StopGrabImage 0\n");
	errChk(imgSessionStopAcquisition((*ab).Sid));
	//printf("in StopGrabImage end\n");
	return true;

	Error :
    if(error<0)
    {
		//printf("in StopGrabImage error\n");
		DAQ_LOGGER_ERROR << __FUNCTION__ << " StopGrabImage error";
		imgClose((*ab).Sid, TRUE);
		imgClose((*ab).Iid, TRUE);        
		DisplayIMAQError(error);
    }

} // end StopGrabImage


void SaveImage(void *acqblk, char *SaveFile, uInt32 *ListNbLine)
{
	//printf("debug in SaveImage");
	Int32 error;
	struct AcqBlock *ab = (struct AcqBlock *)acqblk;
	FILE *f;

	uInt32 buf_num;
	int bufferNumber;	
	uInt16 *BufAddr;

	fopen_s( &f, SaveFile,"wb");

	// find last buffer number
	errChk(imgGetAttribute ((*ab).Sid, IMG_ATTR_LAST_VALID_FRAME, &bufferNumber));	

	
	//for (int frameCounter = bufferNumber - (*ab).NbFrame + 1; frameCounter <= bufferNumber; frameCounter++)
	for (int frameCounter = 0; frameCounter <=  bufferNumber; frameCounter++)
	{	
		errChk(imgSessionExamineBuffer2((*ab).Sid, frameCounter, &buf_num, (void**)&BufAddr));
		//errChk(imgSessionCopyBuffer((*ab).Sid, frameCounter, (uInt8 *)BufAddr, NULL));
		fwrite(BufAddr,sizeof(uInt16), (*ab).NbPixel*ListNbLine[frameCounter], f);
		errChk(imgSessionReleaseBuffer ((*ab).Sid));
	}
	
	fclose(f);

	Error :
    if(error<0)
    {
		DAQ_LOGGER_ERROR << __FUNCTION__ << " SaveImage error";
		imgSessionAbort((*ab).Sid, NULL);
		imgClose((*ab).Sid, TRUE);
		imgClose((*ab).Iid, TRUE);
		DisplayIMAQError(error);
    }
} //end SaveImage


uInt32 QueImage(void *acqblk, uInt32 *ListNbLine, long iteration)
{
	Int32 error;
	struct AcqBlock *ab = (struct AcqBlock *)acqblk;

	uInt32 buf_num;
	int bufferNumber;	
	uInt16 *BufAddr;
	//printf("in QueImage -1 %d\n", (*ab).Sid);
	errChk(imgGetAttribute ((*ab).Sid, IMG_ATTR_LAST_VALID_FRAME, &bufferNumber));	
	//printf("in QueImage 0 bufferNumber:%d\n", bufferNumber);
	// calcuate size of each buffer
	uInt32 bufSizeTot = 0;
	for (int frameNumber = 0; frameNumber <=  bufferNumber; frameNumber++)
	{	
		bufSizeTot += ListNbLine[frameNumber]*(*ab).NbPixel;
	}

	// copy buffers into memory queue
	uInt32 totLine = 0;
	for (int frameCounter = 0; frameCounter <=  bufferNumber; frameCounter++)
	{	
		errChk(imgSessionExamineBuffer2((*ab).Sid, frameCounter, &buf_num, (void**)&BufAddr));
		//printf("in QueImage 1 bufSizeTot: %d \t buf_num:%d \t BufAddr:%d\n", bufSizeTot, buf_num, BufAddr);
		//printf("in QueImage 1 iteration*bufSizeTot + totLine*(*ab).NbPixel:%d\n", iteration*bufSizeTot + totLine*(*ab).NbPixel);
		memcpy((*ab).MemQue + iteration*bufSizeTot + totLine*(*ab).NbPixel, (uInt16*)BufAddr, (*ab).NbPixel*ListNbLine[frameCounter]*sizeof(uInt16));
		//printf("in QueImage 2\n");
		errChk(imgSessionReleaseBuffer ((*ab).Sid));
		//printf("in QueImage 3\n");
		totLine += ListNbLine[frameCounter];
	}
	//printf("in QueImage end. bufSizeTot:%d\n", bufSizeTot);
	return bufSizeTot;

	Error :
    if(error<0)
    {
		//printf("in QueImage error");
		DAQ_LOGGER_ERROR << __FUNCTION__ << " QueImage error";
		imgSessionAbort((*ab).Sid, NULL);
		imgClose((*ab).Sid, TRUE);
		imgClose((*ab).Iid, TRUE);
		DisplayIMAQError(error);
    }
	return -1;

} //end QueImage

void SaveQueImage(void *acqblk, char *SaveFile, uInt32 *ListNbLine, uInt32 bufSizeTot, int iterations)
{
	//printf("in SaveQueImage\n");
	struct AcqBlock *ab = (struct AcqBlock *)acqblk;

	char SaveFileFull[1000];
	
	for (int currentit=0; currentit<iterations; currentit++)
	{
		strcpy (SaveFileFull, SaveFile);

		if (iterations>1 & iterations<10)
		{
			strcat(SaveFileFull, "_I0");
			char buffer[10];
			itoa(currentit,buffer,10);
			strcat(SaveFileFull, buffer);
		}
		else if (iterations>=10)
		{
			strcat(SaveFileFull, "_I");
			char buffer[10];
			itoa(currentit,buffer,10);
			strcat(SaveFileFull, buffer);
		}
		
		strcat(SaveFileFull, ".bin");
		//strcat(SaveFileFull, ".txt");
		//printf("in SaveQueImage %s\n", SaveFileFull);
		//std::ofstream myFile(SaveFileFull, std::ofstream::binary);
		std::ofstream myFile(SaveFileFull);
		myFile.write ((char*)((*ab).MemQue+bufSizeTot*currentit), sizeof(uInt16)*bufSizeTot);
		myFile.close();
		//printf("SaveQueImage done\n");
	}

} //end SaveQueImage

/*
int FundusPreview(float* linePtrGrabber, void *acqblk)
{
	Int32 error;
	uInt32 buf_num;
	uInt16 *BufAddr;
	int retval = 0;

	struct AcqBlock *ab = (struct AcqBlock *)acqblk;

	static float* BackgroundBuffer = (float *) malloc((*ab).NbPixel*sizeof(float));

	// grab frame by frame and send to processFundus
	for(int framecounter = 0 ; framecounter <  (*ab).NbFrame; framecounter++)
	{
		// grab last buffer
		errChk(imgSessionExamineBuffer2((*ab).Sid, framecounter, &buf_num, (void**)&BufAddr));

		//Cilk processFundus
		retval = omp_processFundus(BufAddr, linePtrGrabber, BackgroundBuffer, framecounter, acqblk);

		// release buffer
		errChk(imgSessionReleaseBuffer ((*ab).Sid));
	}

	return retval;

	Error :
    if(error<0)
    {
		imgSessionAbort((*ab).Sid, NULL);
		imgClose((*ab).Sid, TRUE);
		imgClose((*ab).Iid, TRUE);
		DisplayIMAQError(error);
    }
	return 0;
} // end of FundusPreview

int CrossSectionPreview(float* imagePtrGrabber, uInt16* linePtrGrabber, void *acqblk, float *background, int lambda0, int deltalambda)
{
	int retval;
	struct AcqBlock *ab = (struct AcqBlock *)acqblk;
	static uInt16 *framebuffer= (uInt16 *)malloc(sizeof(uInt16)*(*ab).NbPixel*(*ab).NbLine);
	retval = grablastframe(framebuffer, ab);

	for (int pp=0; pp<(*ab).NbPixel; pp++)
	{
		linePtrGrabber[pp] = framebuffer[pp]; // this is used to display real-time spectrum, linePtrGrabber is spectrum
	}

	
	using namespace std;
	//create buffers for processing
	//omp requires vector instead of array pointers 
	static vector<float> linebufferfloat((*ab).NbPixel);
	static vector<float> Kline((*ab).NbPixel);

	static fftwf_complex *fftin = (fftwf_complex*) fftwf_malloc(sizeof(fftwf_complex) * (*ab).NbPixel);
	static fftwf_complex *fftout = (fftwf_complex*) fftwf_malloc(sizeof(fftwf_complex) * (*ab).NbPixel);
	static fftwf_plan plan = fftwf_plan_dft_1d((*ab).NbPixel, fftin, fftout, FFTW_FORWARD, FFTW_MEASURE);

	//using fftwComplexPtr fftwRealPtr class for omp private requirements
	fftwComplexPtr ftIn;
	fftwComplexPtr ftOut;

	static float *locArray = (float *) malloc((*ab).NbPixel*sizeof(float));
	int rv;
	rv = FindLambda (ab, lambda0, deltalambda, locArray);

	#pragma omp parallel for firstprivate (linebufferfloat, Kline) private(ftIn,ftOut)
	for (int ll=0; ll<(*ab).NbLine; ll++)
	{
		ftIn.AssertAllocated((*ab).NbPixel);
		ftOut.AssertAllocated((*ab).NbPixel);

		fftwf_complex *in=ftIn.GetPointer();

		int readoffset = ll*(*ab).NbPixel;
		for (int pp=0; pp<(*ab).NbPixel; pp++)
		{
			linebufferfloat[pp] = (float)framebuffer[readoffset + pp]-background[pp];
			// to zero out parts of the spectrum, add something here
		}

		rv = LambdaK(ab, locArray, &linebufferfloat[0], &Kline[0]);

		for (int j=0;j<(*ab).NbPixel;j++)
		{
			in[j][0] = Kline[j];
		}

		fftwf_execute_dft(plan, ftIn.GetPointer(), ftOut.GetPointer());

		fftwf_complex *out=ftOut.GetPointer();
		int writeoffset = ll*(*ab).NbPixel/2;
		for(int pp=5; pp<(*ab).NbPixel/2; pp++)
		{
			imagePtrGrabber[writeoffset + pp] = log10(out[pp][0]*out[pp][0]+out[pp][1]*out[pp][1]);
			//imagePtrGrabber[writeoffset + pp] = (out[pp][0]*out[pp][0]+out[pp][1]*out[pp][1]);
		}

	}	

	return retval;

} // end of CrossSectionPreview


int UpdateBackground(void *acqblk, float *background)
{
	int retval;
	struct AcqBlock *ab = (struct AcqBlock *)acqblk;
	static uInt16 *framebuffer= (uInt16 *)malloc(sizeof(uInt16)*(*ab).NbPixel*(*ab).NbLine);
	retval = grablastframe(framebuffer, ab);

	#pragma omp parallel for
	for (int p=0; p<(*ab).NbPixel; p++)
	{
		background[p] = 0;
		for(int l = 0; l < (*ab).NbLine; l++)
		{
			background[p] += framebuffer[l*(*ab).NbPixel+p];
		}
		background[p] = background[p]/(*ab).NbLine;
	}
	

	return retval;

} // end of UpdateBackground

void *GrabLastFrame(uInt16 *imagePtrGrabber, void *acqblk)
{
	
	Int32 error;
	struct AcqBlock *ab = (struct AcqBlock *)acqblk;
 	uInt16 *BackgroundAddr;
	int bufferNumber;	
	//int linecounter, pixelcounter;

	// grab last buffer
	errChk(imgSessionExamineBuffer2((*ab).Sid, IMG_LAST_BUFFER, (uInt32 *)&bufferNumber, (void**)&BackgroundAddr));

	// copy buffer
	memcpy(imagePtrGrabber, (uInt16*)BackgroundAddr, (*ab).NbPixel*(*ab).NbLine*sizeof(uInt16));

	// release buffer
	errChk(imgSessionReleaseBuffer ((*ab).Sid));

	return (void *)bufferNumber;

	Error :
    if(error<0)
    {
		imgSessionAbort((*ab).Sid, NULL);
		imgClose((*ab).Sid, TRUE);
		imgClose((*ab).Iid, TRUE);        
		DisplayIMAQError(error);
    }
	return 0;

}// end GrabLastFrame

void *PreviewAll(float* centerframe, float* imageall, float* enfacepreview, void *acqblk, uInt32 *ListNbLine, float *background, int lambda0, int deltalambda)
{
	struct AcqBlock *ab = (struct AcqBlock *)acqblk;

	using namespace std;
	//omp requires vector instead of array pointers 
	static vector<float> linebufferfloat((*ab).NbPixel);
	static vector<float> Kline((*ab).NbPixel);

	Int32 error;
	int buf_num;
	uInt16 *BufAddr;
	int retval = 0;
	//set imageall to be 1024 by 20000 pixels
	int imageallpixelsize = 1024; //1024 before FFT
	int imagealllinesize = 2000;

	//create buffers for FFT
	fftwf_complex *fftin = (fftwf_complex*) fftwf_malloc(sizeof(fftwf_complex) * imageallpixelsize);
	//memset(fftin,0,sizeof(fftwf_complex)*imageallpixelsize);
	fftwf_complex *fftout = (fftwf_complex*) fftwf_malloc(sizeof(fftwf_complex) * imageallpixelsize);
	fftwf_plan plan = fftwf_plan_dft_1d(imageallpixelsize, fftin, fftout, FFTW_FORWARD, FFTW_MEASURE);

	//using fftwComplexPtr fftwRealPtr class for omp private requirements
	fftwComplexPtr ftIn;
	fftwComplexPtr ftOut;

	int checkequallines = TRUE;
	long totallines = 0;

	int bufferNumber;
	// find last buffer number
	errChk(imgGetAttribute ((*ab).Sid, IMG_ATTR_LAST_VALID_FRAME, &bufferNumber));	

	// check if all frames are same size
	for (int framenumber = 0; framenumber <=  bufferNumber; framenumber++)
	{
		totallines += ListNbLine[framenumber];
		if(ListNbLine[0]!=ListNbLine[framenumber])
		{
			checkequallines = FALSE;
		}
	}

	int scaledown = 0;
	if(totallines > imagealllinesize){
		scaledown = totallines / imagealllinesize;
	}else{
		scaledown =1;
	}

	//BackgroundBuffer for omp_processFundus
	static float* BackgroundBuffer = (float *) malloc((*ab).NbPixel*sizeof(float));

	int currentframe = 0;
	int currentline = 0;
	// grab frame by frame and send to processFundus
	//for(uInt32 framecounter = totalframes-1; (long)framecounter >= 0; framecounter--)
	for(uInt32 framecounter = 0; (long)framecounter <= bufferNumber; framecounter++)
	{
		// grab last buffer
		errChk(imgSessionExamineBuffer2((*ab).Sid, framecounter, (uInt32 *)&buf_num, (void**)&BufAddr));

		//processFundus
		if ((*ab).NbFrame > 99 && (*ab).NbLine < 4096 && checkequallines == TRUE)
		{
			retval = omp_processFundus(BufAddr, enfacepreview, BackgroundBuffer, currentframe, acqblk);
		}

		//view all
		#pragma omp parallel for private(ftIn,ftOut)
		for (int ll=0; ll<(int)ListNbLine[framecounter]; ll+=scaledown)
		{
			ftIn.AssertAllocated(imageallpixelsize);
			ftOut.AssertAllocated(imageallpixelsize);

			fftwf_complex *in=ftIn.GetPointer();

			int readoffset = ll*(*ab).NbPixel;
			for (int pp=0; pp<imageallpixelsize; pp++)
			{
				in[pp][0] = (float)BufAddr[readoffset + pp]-background[pp];
			}

			fftwf_execute_dft(plan, ftIn.GetPointer(), ftOut.GetPointer());

			fftwf_complex *out=ftOut.GetPointer();
			int writeoffset = (currentline+ll)/scaledown*imageallpixelsize/2;
			for(int pp=5; pp<imageallpixelsize/2; pp++)
			{
				if (writeoffset + pp<imageallpixelsize/2*imagealllinesize){
				imageall[writeoffset + pp] = log10(out[pp][0]*out[pp][0]+out[pp][1]*out[pp][1]);
				//imageall[writeoffset + pp] = (out[pp][0]*out[pp][0]+out[pp][1]*out[pp][1]);
				}
			}
		}
		
		// release buffer
		errChk(imgSessionReleaseBuffer ((*ab).Sid));
		currentframe++;
		currentline += ListNbLine[framecounter];
	}

	fftwf_free(fftin);
	fftwf_free(fftout);
	fftwf_destroy_plan(plan);
	
	//center frame
	errChk(imgSessionExamineBuffer2((*ab).Sid, bufferNumber/2, (uInt32 *)&buf_num, (void**)&BufAddr));
	//create buffers for processing
	fftwf_complex *fftincen = (fftwf_complex*) fftwf_malloc(sizeof(fftwf_complex) * (*ab).NbPixel);
	fftwf_complex *fftoutcen = (fftwf_complex*) fftwf_malloc(sizeof(fftwf_complex) * (*ab).NbPixel);
	fftwf_plan plan1 = fftwf_plan_dft_1d((*ab).NbPixel, fftincen, fftoutcen, FFTW_FORWARD, FFTW_MEASURE);

	static float *locArray = (float *) malloc((*ab).NbPixel*sizeof(float));
	int rv;
	rv = FindLambda (ab, lambda0, deltalambda, locArray);

	#pragma omp parallel for firstprivate (linebufferfloat,Kline) private(ftIn,ftOut)
	for (int ll=0; ll<(int)ListNbLine[bufferNumber/2]; ll++)
	{
		ftIn.AssertAllocated((*ab).NbPixel);
		ftOut.AssertAllocated((*ab).NbPixel);
		fftwf_complex *in=ftIn.GetPointer();
		int readoffset = ll*(*ab).NbPixel;
		for (int pp=0; pp<(*ab).NbPixel; pp++)
		{
			linebufferfloat[pp] = (float)BufAddr[readoffset + pp]-background[pp];
		}
		rv = LambdaK(ab, locArray, &linebufferfloat[0], &Kline[0]);
		for (int j=0;j<(*ab).NbPixel;j++)
		{

			in[j][0] = Kline[j];
		}
		fftwf_execute_dft(plan1, ftIn.GetPointer(), ftOut.GetPointer());
		fftwf_complex *out=ftOut.GetPointer();
		int writeoffset = ll*(*ab).NbPixel/2;
		for(int pp=5; pp<(*ab).NbPixel/2; pp++)
		{
			centerframe[writeoffset + pp] = log10(out[pp][0]*out[pp][0]+out[pp][1]*out[pp][1]);
			//centerframe[writeoffset + pp] = (out[pp][0]*out[pp][0]+out[pp][1]*out[pp][1]);
		}
	}
	
	errChk(imgSessionReleaseBuffer ((*ab).Sid));
	
	fftwf_free(fftincen);
	fftwf_free(fftoutcen);
	fftwf_destroy_plan(plan1);

	return (void *)buf_num;

	Error :
    if(error<0)
    {
		imgSessionAbort((*ab).Sid, NULL);
		imgClose((*ab).Sid, TRUE);
		imgClose((*ab).Iid, TRUE);
		DisplayIMAQError(error);
    }
	return 0;
} // end of PreviewAll

void *COM_CONFIG(int *xpos, int *ypos) {
	struct FixTargBlock *fixtargblk = (struct FixTargBlock *) malloc(sizeof(struct FixTargBlock));	
	unsigned char * control = new unsigned char;
	unsigned char * rdstr = new unsigned char;
	unsigned char * horzrect = new unsigned char[7];
	unsigned char * vertrect = new unsigned char[7];
	int rectlong = (*fixtargblk).rectlong;	//size of long side of rectangle
	int rectshort = (*fixtargblk).rectshort;	//size of short side of rectangle

	DWORD BytesWritten;
	DWORD BytesRead;

	DCB dcb;
	BOOL fSuccess;

	// open port for I/O
	(*fixtargblk).hCom = CreateFile(_T("COM4"),GENERIC_READ|GENERIC_WRITE,0,NULL,OPEN_EXISTING,0,NULL);
	if((*fixtargblk).hCom == INVALID_HANDLE_VALUE) {
		printf("E012_Failed to open port\n");
		return 0;
	}
	// set timeouts
	COMMTIMEOUTS cto = { 1, 100, 1000, 0, 0 };
	if(!SetCommTimeouts((*fixtargblk).hCom,&cto)) {
		printf("E013_SetCommTimeouts failed\n");
		return 0;
	}
	// Build on the current configuration, and skip setting the size
	// of the input and output buffers with SetupComm.
	SecureZeroMemory(&dcb, sizeof(DCB));
	dcb.DCBlength = sizeof(DCB);
	fSuccess = GetCommState((*fixtargblk).hCom, &dcb);
	if (!fSuccess) {
       printf ("GetCommState failed with error %d.\n", GetLastError());
       return 0;
    }

    // Fill in DCB: 9600 bps, 8 data bits, no parity, and 1 stop bit.
    dcb.BaudRate = CBR_9600;     // set the baud rate
    dcb.ByteSize = 8;             // data size, xmit, and rcv
	dcb.Parity = NOPARITY;        // no parity bit
	dcb.StopBits = ONESTOPBIT;    // one stop bit
	fSuccess = SetCommState((*fixtargblk).hCom, &dcb);
	if (!fSuccess) {
       printf ("SetCommState failed with error %d.\n", GetLastError());
       return 0;
	}
	*control=(unsigned char)LED_AUTOBAUD;
	WriteFile((*fixtargblk).hCom, control, 1, &BytesWritten, NULL);
	ReadFile((*fixtargblk).hCom,rdstr,1,&BytesRead,NULL);
	if (int(*rdstr)==6) {	//6 is the acknowledge signal
		*control=(unsigned char)LED_CLEARSCREEN;
		WriteFile((*fixtargblk).hCom, control, 1, &BytesWritten, NULL);

		// Background Color
		horzrect[0]=(unsigned char)LED_DRAWrect;
		horzrect[5]=(unsigned char)6<<3;	//These two lines set
		horzrect[6]=(unsigned char)0;	//the color to red
		horzrect[1]=(unsigned char)0;	//x1
		horzrect[2]=(unsigned char)0;	//y1
		horzrect[3]=(unsigned char)127;	//x2
		horzrect[4]=(unsigned char)127;	//y2
		WriteFile((*fixtargblk).hCom, horzrect, 7, &BytesWritten, NULL);

		//Note that the cross pattern will be centered on (64,64)
		horzrect[0]=(unsigned char)LED_DRAWrect;
		horzrect[5]=(unsigned char)0;	//These two lines set
		horzrect[6]=(unsigned char)0;	//the color to black
		horzrect[1]=(unsigned char)64-rectlong/2;	//x1
		horzrect[2]=(unsigned char)64-rectshort/2;	//y1
		horzrect[3]=(unsigned char)64+rectlong/2;	//x2
		horzrect[4]=(unsigned char)64+rectshort/2;	//y2
		xpos[0] = (horzrect[2]+horzrect[4])/2;
		WriteFile((*fixtargblk).hCom, horzrect, 7, &BytesWritten, NULL);
		
		vertrect[0]=(unsigned char)LED_DRAWrect;
		vertrect[5]=(unsigned char)0;	//These two lines set
		vertrect[6]=(unsigned char)0;	//the color to black
		vertrect[1]=(unsigned char)64-rectshort/2;	
		vertrect[2]=(unsigned char)64-rectlong/2;
		vertrect[3]=(unsigned char)64+rectshort/2;
		vertrect[4]=(unsigned char)64+rectlong/2;
		ypos[0] = (vertrect[1]+vertrect[3])/2;
		WriteFile((*fixtargblk).hCom, vertrect, 7, &BytesWritten, NULL);

		//Note that the cross pattern will be centered on (64,64)
		horzrect[0]=(unsigned char)LED_DRAWrect;
		horzrect[5]=(unsigned char)6;	//These two lines set
		horzrect[6]=(unsigned char)0;	//the color to green
		horzrect[1]=(unsigned char)64-rectlong/2/2;	//x1
		horzrect[2]=(unsigned char)64-rectshort/2/2;	//y1
		horzrect[3]=(unsigned char)64+rectlong/2/2;	//x2
		horzrect[4]=(unsigned char)64+rectshort/2/2;	//y2
		xpos[0] = (horzrect[2]+horzrect[4])/2;
		WriteFile((*fixtargblk).hCom, horzrect, 7, &BytesWritten, NULL);

		vertrect[0]=(unsigned char)LED_DRAWrect;
		vertrect[5]=(unsigned char)6;	//These two lines set
		vertrect[6]=(unsigned char)0;	//the color to green
		vertrect[1]=(unsigned char)64-rectshort/2/2;	
		vertrect[2]=(unsigned char)64-rectlong/2/2;
		vertrect[3]=(unsigned char)64+rectshort/2/2;
		vertrect[4]=(unsigned char)64+rectlong/2/2;
		ypos[0] = (vertrect[1]+vertrect[3])/2;
		WriteFile((*fixtargblk).hCom, vertrect, 7, &BytesWritten, NULL);

		return fixtargblk;
	}
	else
		return 0;
}

int COM_CLOSE(void *fixtargblk) {
	struct FixTargBlock *ftb = (struct FixTargBlock *)fixtargblk;
	CloseHandle((*ftb).hCom);
	return 0;
}

int MOVE(void *fixtargblk, int new_horz, int new_vert) {
	struct FixTargBlock *ftb = (struct FixTargBlock *)fixtargblk;
	unsigned char * control = new unsigned char;
	DWORD BytesWritten;

	*control=(unsigned char)LED_CLEARSCREEN;
	WriteFile((*ftb).hCom, control, 1, &BytesWritten, NULL);
	
	int rectlong = (*ftb).rectlong;
	int rectshort = (*ftb).rectshort;

	unsigned char * horzrect = new unsigned char[7];
	unsigned char * vertrect = new unsigned char[7];

	int vr[4],hr[4];
	vr[0]=new_horz-rectshort/2;
	vr[2]=new_horz+rectshort/2;
	hr[0]=new_horz-rectlong/2;
	hr[2]=new_horz+rectlong/2;

	if ((hr[0]<=0&&hr[2]<=0)||(hr[0]>=127&&hr[2]>=127))	//Cross Moved off-screen
		return (1);
	if (hr[0]<0)
		hr[0]=0;
	if (hr[2]>127)
		hr[2]=127;
	if (vr[0]<0)
		vr[0]=0;
	if (vr[2]>127)
		vr[2]=127;


	int vrc[4],hrc[4];
	vrc[0]=new_horz-rectshort/2/2;
	vrc[2]=new_horz+rectshort/2/2;
	hrc[0]=new_horz-rectlong/2/2;
	hrc[2]=new_horz+rectlong/2/2;

	if ((hrc[0]<=0&&hrc[2]<=0)||(hrc[0]>=127&&hrc[2]>=127))	//Cross Moved off-screen
		return (1);
	if (hrc[0]<0)
		hrc[0]=0;
	if (hrc[2]>127)
		hrc[2]=127;
	if (vrc[0]<0)
		vrc[0]=0;
	if (vrc[2]>127)
		vrc[2]=127;

	new_vert=127-new_vert;	//need this since on LED +y is down

	vr[1]=new_vert-rectlong/2;
	vr[3]=new_vert+rectlong/2;
	hr[1]=new_vert-rectshort/2;
	hr[3]=new_vert+rectshort/2;

	if ((vr[1]<=0&&vr[3]<=0)||(vr[1]>=127&&vr[3]>=127))	//Cross Moved off-screen
		return (1);
	if (vr[1]<0)
		vr[1]=0;
	if (vr[3]>127)
		vr[3]=127;
	if (hr[1]<0)
		hr[1]=0;
	if (hr[3]>127)
		hr[3]=127;

	vrc[1]=new_vert-rectlong/2/2;
	vrc[3]=new_vert+rectlong/2/2;
	hrc[1]=new_vert-rectshort/2/2;
	hrc[3]=new_vert+rectshort/2/2;

	if ((vrc[1]<=0&&vrc[3]<=0)||(vrc[1]>=127&&vrc[3]>=127))	//Cross Moved off-screen
		return (1);
	if (vrc[1]<0)
		vrc[1]=0;
	if (vrc[3]>127)
		vrc[3]=127;
	if (hrc[1]<0)
		hrc[1]=0;
	if (hrc[3]>127)
		hrc[3]=127;




	// Background Color
	horzrect[0]=(unsigned char)LED_DRAWrect;
	horzrect[5]=(unsigned char)6<<3;	//These two lines set
	horzrect[6]=(unsigned char)0;	//the color to red
	horzrect[1]=(unsigned char)0;	//x1
	horzrect[2]=(unsigned char)0;	//y1
	horzrect[3]=(unsigned char)127;	//x2
	horzrect[4]=(unsigned char)127;	//y2
	WriteFile((*ftb).hCom, horzrect, 7, &BytesWritten, NULL);

	// Cross Target
	horzrect[5]=(unsigned char)0;	//These two lines set
	horzrect[6]=(unsigned char)0;	//the color to black
	horzrect[1]=(unsigned char)hr[0];
	horzrect[2]=(unsigned char)hr[1];
	horzrect[3]=(unsigned char)hr[2];
	horzrect[4]=(unsigned char)hr[3];
	WriteFile((*ftb).hCom, horzrect, 7, &BytesWritten, NULL);

	vertrect[0]=(unsigned char)LED_DRAWrect;
	vertrect[5]=(unsigned char)0;	//These two lines set
	vertrect[6]=(unsigned char)0;	//the color to black
	vertrect[1]=(unsigned char)vr[0];
	vertrect[2]=(unsigned char)vr[1];
	vertrect[3]=(unsigned char)vr[2];
	vertrect[4]=(unsigned char)vr[3];
	WriteFile((*ftb).hCom, vertrect, 7, &BytesWritten, NULL);

	// Cross Target
	horzrect[5]=(unsigned char)6;	//These two lines set
	horzrect[6]=(unsigned char)0;	//the color to green
	horzrect[1]=(unsigned char)hrc[0];
	horzrect[2]=(unsigned char)hrc[1];
	horzrect[3]=(unsigned char)hrc[2];
	horzrect[4]=(unsigned char)hrc[3];
	WriteFile((*ftb).hCom, horzrect, 7, &BytesWritten, NULL);

	vertrect[0]=(unsigned char)LED_DRAWrect;
	vertrect[5]=(unsigned char)6;	//These two lines set
	vertrect[6]=(unsigned char)0;	//the color to green
	vertrect[1]=(unsigned char)vrc[0];
	vertrect[2]=(unsigned char)vrc[1];
	vertrect[3]=(unsigned char)vrc[2];
	vertrect[4]=(unsigned char)vrc[3];
	WriteFile((*ftb).hCom, vertrect, 7, &BytesWritten, NULL);


	return 0;
}

*/