
#include <iostream>
#include "common.h"
#include "DataAcquisitionApi.h"

using namespace std;

uInt_32 uiNumPixel = 1024;
uInt_32 uiNumAscan = 200;
uInt_32 uiNumBscan = 200;
uInt_32 uiNumBscanRepeat = 2;
int main()
{
	printf("start\n");
	size_t size = 100;
	DataBuffer* pBuf = new DataBuffer(size);
	DataAcquisitionApi* daqApi = new DataAcquisitionApi();
	daqApi->setDaqCalibration(uiNumPixel, uiNumAscan, uiNumBscan, uiNumBscanRepeat);
	daqApi->startDataAcquire();
	//daqApi->stopDataAcquire();
	daqApi->copyData(pBuf);
	daqApi->stopDataAcquire();
	pBuf->print_buf();
	cout<<"hello daq"<<endl;
	system("pause");
	return 0;
}
