#ifndef C_NI_PCIe1429_h
#define C_NI_PCIe1429_h
#endif

#include "niimaq.h"
//#include "fftw3.h"

//#define C_NI_PCIe1429 __declspec(dllexport)
#define C_NI_PCIe1429  

#ifndef _NIWIN
#define _NIWIN
#endif

int DisplayIMAQError(Int32 error);
void *InitializeSession(const char* interface_name, int numPixels, int numLines);
bool EndSession(void *initacqblk);
void *AllocateMemory(void *initacqblk, long NbPixel, long NbLine, long NbFrame);
void *AllocateACQMemory(void *initacqblk, long NbPixel, uInt32 *ListNbLine, long NbFrame, long iterations);
bool DeallocateMemory(void *acqblk);
bool DeallocateQueMemory(void *acqblk);
void *StartGrabImage(void *acqblk);
bool StopGrabImage(void *acqblk);
uInt32 QueImage(void *acqblk, uInt32 *ListNbLine, long iteration);
void SaveImage(void *acqblk, char *SaveFile, uInt32 *ListNbLine);
void SaveQueImage(void *acqblk, char *SaveFile, uInt32 *ListNbLine, uInt32 bufSizeTot, int iterations);
int FundusPreview(float* linePtrGrabber, void *acqblk, float *Background, uInt32 bufferNumber);
int CrossSectionPreview(float* imagePtrGrabber, uInt16* linePtrGrabber, void *acqblk, float *background, int lambda0, int deltalambda);
int UpdateBackground(void *acqblk, float *background);
void *GrabLastFrame(uInt16 *imagePtrGrabber, void *acqblk);
void *PreviewAll(float* centerframe, float* imageall, float* enfacepreview, void *acqblk, uInt32 *ListNbLine, float *background, int lambda0, int deltalambda);


void *COM_CONFIG(int *xpos, int *ypos);
int COM_CLOSE(void *fixtargblk);
int MOVE(void *fixtargblk, int new_horz, int new_vert);


//Use this struct to initialize multiple AckBlock structs 
struct InitAcqBlock
{
	INTERFACE_ID Iid;
	SESSION_ID   Sid;
};

struct AcqBlock
{
	INTERFACE_ID Iid;
	SESSION_ID   Sid;
	BUFLIST_ID	 Bid;
	int		 NbPixel;
	int		 NbLine;
	int		 NbFrame;
	uInt16	*MemQue;
};

struct FixTargBlock
{
	//The commands are in the manual(in hex)-need to convert to decimal to change
	void * hCom;
	static const int rectlong = 20;	//size of long side of rectangle
	static const int rectshort = 2;	//size of short side of rectangle
};

/*
class fftwComplexPtr
{
public:
	fftwComplexPtr(void)
	{
		m_ptr=0;
		m_ulLen =0;
	}
	~fftwComplexPtr(void)
	{
		if (m_ptr)
		{
			fftwf_free(m_ptr);
		}
	}
	void AssertAllocated(size_t wantLen)
	{
		if (m_ulLen != wantLen)
		{
			if (m_ptr)
			{
				fftwf_free(m_ptr);
			}
			m_ptr= (fftwf_complex*) fftwf_malloc(sizeof(fftwf_complex)*wantLen);
			memset(m_ptr,0,sizeof(fftwf_complex)*wantLen);
			m_ulLen = wantLen;
		}
	}
	fftwf_complex * GetPointer(void) { return m_ptr;}
private:
	fftwf_complex * m_ptr;
	size_t		m_ulLen;
};
*/