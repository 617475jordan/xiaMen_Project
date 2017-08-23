#pragma once
#include <Windows.h>
#include <time.h>
#include <math.h> 
#include <string>
#include <vector> 
#include <iostream>
#include <sstream>
#include <fstream>
//#include <boost/filesystem.hpp>
using namespace std;
using std::vector;
#define MAX 1000

/*
\breif
This class define the function which read the parameters from the config.ini
\author
Ziguang Zeng
\date
2010-4-15
*/
class MParamReader
{

public:

	MParamReader(void);

	/*
	\breif
	The method for initializing the parameters.
	*/
	bool Initialize(const char * fileName)
	{
		//Read file
		//ifstream LoadFile("../data/config/config.ini");
       /* boost::filesystem::path path(fileName);
        if (!boost::filesystem::exists(path))
		{
			return false;
		}*/
		
		ifstream LoadFile(fileName);
		char data[MAX];
		char* szLinkParam = new char[MAX];
		char* szIDParam = new char[MAX];
		memset(szLinkParam, 0, MAX);
		memset(szIDParam, 0, MAX);
		int iIndex = 0;
		int iID = 0;
		//vector<string> list;
		while(!LoadFile.eof())
		{	
			string str;
			memset(data, 0, MAX);
			memset(szLinkParam, 0, MAX);
			memset(szIDParam, 0, MAX);
			LoadFile.getline(data, sizeof(data));
			size_t fileLen = strlen(data);
			size_t tempLen;
			for (tempLen = 0; tempLen < fileLen; tempLen++)
			{
				//stop at the ":"
				if (data[tempLen] == ':')
				{
					break;
				}
			}
			memcpy(szIDParam, &data[0], tempLen);
			iIndex = atoi(szIDParam);
			//iID++;
			if (iIndex == 0)
			{
				iID = 0;
			}
			if (iIndex != 0)
			{
				iID++;
				m_IDlist.push_back(iID);
			}
			size_t cpyLen = fileLen - tempLen - 1;
			if (cpyLen > MAX)
			{
				continue;
			}
			memcpy(szLinkParam, &data[tempLen + 2], cpyLen);
			str = szLinkParam;
			//push string
			m_list.push_back(str);
			
		}
 		return true;
	}

	void Clear()
	{
		m_list.clear();
		m_IDlist.clear();
	}
	/*
	\brief
	The method to get the parameters from the file config
	@param szParam, the string need to be read
	*/
	//float GetModeParam(const char* szParam)
	//{
	//	float fModeParam = atof(szParam);
	//	return fModeParam;
	//}

public:

	~MParamReader(void);

public:

	vector<string> m_list;//Define vector to store strings
	vector<int> m_IDlist;

	/*
	\brief
	The parameters read by config.ini
	@param m_fParam[0] the total frame number
	@param m_fParam[1] the fps
	@param m_fParam[2] the year value list number
	@param m_fParam[3] the light value list number
	*/
	float m_fParam[95];
	
};
