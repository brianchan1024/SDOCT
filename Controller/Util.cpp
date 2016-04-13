#include "Util.h"
#include <vector>
#include <fstream>
#include <sstream>


template <typename T>
std::string Util::to_string(T value)
{
	std::ostringstream os ;
	os << value ;
	return os.str() ;
}

template <typename T>
bool Util::saveFrameData2File(std::vector< std::vector<T> > frameData, std::string filename)
{
	std::ofstream myfile;
	myfile.open (filename);
	std::string data = "";
	for(int i = 0; i < frameData.size(); i++)
	{
		if(i != 0) data += '\n';
		std::vector<T> tmp = frameData.at(i);
		for(int j = 0; j < tmp.size(); j++)
		{
			data += to_string(tmp.at(j)) + '\t';
		}
		
	}
	cout<<data<< endl;
	myfile << data;
	myfile.close();
	return true;
}