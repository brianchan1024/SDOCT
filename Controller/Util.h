#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
using namespace std;


class Util
{
public:
	template <typename T>
	static std::string to_string(T value)
	{
		std::ostringstream os ;
		os << value ;
		return os.str() ;
	}

	template <typename T>
	static bool saveFrameData2File(std::vector<T> frameData, std::string filename, int length)
	{
		try
		{
			std::ofstream myfile;
			myfile.open (filename);
			std::string data = "00";
			std::stringstream ss;
			for(int i = 0; i < frameData.size(); i++)
			{
				if (i!=0 && i%length == 0)
				{
					ss << '\n';
				}
				ss << frameData.at(i);
				ss << '\t';
			}

			data = ss.str();
			myfile << data;
			myfile.flush();
			myfile.close();
			return true;
		} catch (std::exception &ex) {
			throw std::invalid_argument("Util saveFrameData2File error");
		}	
	}
	
	static bool saveFrameData2BinFile(std::vector<short> frameData, std::string filename)
	{
		std::cout<<"TRACE: "<<__FUNCTION__<< " \n"<< filename<<std::endl;

		try
		{
			int data_len = frameData.size();
			short* data = frameData.data();
			std::ofstream myfile(filename, ios::out | ios::binary);

			if (myfile.is_open())
			{
				myfile.write ((char*)(data), sizeof(short)*data_len);
				myfile.close();
			}
			return true;
		} catch (std::exception &ex) {
			throw std::invalid_argument("Util saveFrameData2BinFile error");
		}
	}

};

