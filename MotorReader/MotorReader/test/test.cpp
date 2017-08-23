// test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <CommonUtil.h>
#include "MotorReader.h"
int m_iPort = 3;
int m_iBaudRate = 115200;
int main(int argc, _TCHAR* argv[])
{
	MotorReader MotorReader;
	if (MotorReader.initial(m_iPort, m_iBaudRate))
	{
		while (1)
		{
			MotorReader.runFrame();
			Sleep(30);
		}
	}
	else
	{
		cout << "MotorReader is failed !" << endl;
		return -1;
	}
	return 0;
}

