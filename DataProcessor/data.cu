#include "data.cuh"
#include <string>
#include <ctime>
//#include "DP_logger.h"

using namespace std;

Data::Data(Parameters &p) : Helper(p)
{
	d_gauss_win = NULL;
	genGaussWin();
}

Data::~Data()
{
	if (d_gauss_win) gpuErrchk( cudaFree(d_gauss_win) );
}

void Data::genGaussWin()
{
	float temp;
	float *gauss_win = new float[width_2x];
    for (int i = 0; i < width_2x; i++)
	{
        temp = p.alpha * (((i+1.f) / width) - 1.f);
		temp *= temp;
        gauss_win[i] = expf(temp*(-0.5f));
    }

	gpuErrchk( cudaMalloc((void **)&d_gauss_win, (p.batchAscans * width_2x * sizeof(float))) );
	gpuErrchk( cudaMemcpy(d_gauss_win, gauss_win, (width_2x * sizeof(float)), cudaMemcpyHostToDevice) );
	repmat<<<dimLine_w2,dimLine_B>>>(p.batchAscans, width_2x, d_gauss_win); gpuErrchk( cudaPeekAtLastError() );

	delete[] gauss_win;

	cout <<"	- Gauss win matrix created" << endl;
}

void Data::loadFile(string fname, int length, float *result_array)
{
	std::time_t rawtime;
    std::tm* timeinfo;
    char buffer [80];
    std::time(&rawtime);
    timeinfo = std::localtime(&rawtime);
    std::strftime(buffer,80,"%Y-%m-%d-%H-%M-%S",timeinfo);
	cout << __FUNCTION__ << "before load file 1" << fname << "\t" << buffer << endl;

    ifstream is(fname);
	if (is)
	{
		for (int i = 0; i < length; i++)
			is >> result_array[i];
		is.close();
	}
	else
	{
		cerr << "Calibration File could not be opened!\n"; // Report error
		cerr << "Error code: " << strerror(errno); // Get some info as to why
		//system("pause");
		//DP_LOGGER_ERROR << __FUNCTION__ << " Calibration File could not be opened error.";
		throw invalid_argument("Calibration File Error");
	}

	std::time(&rawtime);
    timeinfo = std::localtime(&rawtime);
    std::strftime(buffer,80,"%Y-%m-%d-%H-%M-%S",timeinfo);
	cout << __FUNCTION__ << "after load file 1" << fname << "\t" << buffer << endl;
}

// for background and disperion
void Data::loadFile(string fname, int startLocation, int length, float2 *result_array)
{
	std::time_t rawtime;
    std::tm* timeinfo;
    char buffer [80];
    std::time(&rawtime);
    timeinfo = std::localtime(&rawtime);
    std::strftime(buffer,80,"%Y-%m-%d-%H-%M-%S",timeinfo);
	cout << "**********"<<endl;
	cout << __FUNCTION__ << " before load file 2" << fname << "\t" << buffer << endl;

	cout << fname << " startLocation: " << startLocation << endl;
	ifstream is(fname, ios::in|ios::binary);
	if (is)
	{
		is.seekg(startLocation,ios::beg);
	
		int x = 0;
		for (int i = 0; i < length; ++i)
		{
			is.read(reinterpret_cast<char*>(&x), sizeof(uint16_t));
			result_array[i].x = (float)x;
			//result_array[i].y = 0;

			if(i < 2048)
			{
				cout<< x << " ";
			}
		}
		cout << endl;
		is.close();
	}
	else
	{
		
		cerr << " File could not be opened!\n"; // Report error
		cerr << "Error code: " << strerror(errno) << "\n"; // Get some info as to why
		//system("pause");
		//DP_LOGGER_ERROR << __FUNCTION__ << " File could not be opened error.";
		throw invalid_argument("File Open Error");
	}

	cout << "in load file 2 \n";
	for(int i = 0; i < width_2x; i++)
	{
		cout << result_array[i].x << " ";
	}
	cout << endl;

	std::time(&rawtime);
    timeinfo = std::localtime(&rawtime);
    std::strftime(buffer,80,"%Y-%m-%d-%H-%M-%S",timeinfo);
	cout << __FUNCTION__ << " after load file 2" << fname << "\t" << buffer << endl;
	cout << "**********"<<endl<<endl<<endl;
}

void Data::loadFile(string fname, int startLocation, int length, uint16_t *result_array)
{
	std::time_t rawtime;
    std::tm* timeinfo;
    char buffer [80];
    std::time(&rawtime);
    timeinfo = std::localtime(&rawtime);
    std::strftime(buffer,80,"%Y-%m-%d-%H-%M-%S",timeinfo);
	cout << __FUNCTION__ << " before load file 3" << fname << "\t" << buffer;

	cout << fname << endl;
	length *=2;
	ifstream is(fname, ios::in|ios::binary);
	if (is.is_open())
	{
		is.seekg (startLocation, ios::beg);
		is.read(reinterpret_cast<char*>(result_array), length);
		is.close();
	}
	else
	{
		cerr << "File could not be opened!\n";
		cerr << "Error code: " << strerror(errno);
		//system("pause");
		//DP_LOGGER_ERROR << __FUNCTION__ << " Data File Error.";
		throw invalid_argument("Data File Error");
	}

	std::time(&rawtime);
    timeinfo = std::localtime(&rawtime);
    std::strftime(buffer,80,"%Y-%m-%d-%H-%M-%S",timeinfo);
	cout << __FUNCTION__ << " after load file 3" << fname << "\t" << buffer;
}