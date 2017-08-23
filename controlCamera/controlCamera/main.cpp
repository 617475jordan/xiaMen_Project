#include "machineReader.h"
#include <opencv_all.h>
#include "deskMedia.h"
#include "PM_Hub.h"

string m_strPath = "..\\data\\initialize.txt";
string m_strMainMedia = "..\\data\\MainMedia.txt";
string m_strCommonMedia = "..\\data\\CommonMedia.txt";

int main()
{
	machineReader machineReader;
	deskMedia deskMedia;
	PM_Hub *g_PM_Hub = new PM_Hub("testPlayer");
	deskMedia.Initialize(g_PM_Hub, m_strPath, m_strMainMedia, m_strCommonMedia);
	  
	deskMedia.run(1);
	machineReader.Initialize();                               //Ğı×ª±àÂëÆ÷³õÊ¼»¯
	while (1)
	{
		machineReader.run();
		if (machineReader.isOK() == true)
		{
			deskMedia.run(machineReader.currentPositionFlag());
		}
		else
		{
			deskMedia.run(-1);
		}
	}
	return 0;
}


