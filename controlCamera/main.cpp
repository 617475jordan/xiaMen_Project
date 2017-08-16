#include "machineReader.h"
#include <opencv_all.h>
#include "deskMedia.h"
#include "PM_Hub.h"

int main()
{
	machineReader machineReader;
	deskMedia deskMedia;
	PM_Hub *g_PM_Hub = new PM_Hub("testPlayer");
	deskMedia.Initialize(g_PM_Hub, "..\\data\\playVideo.txt");//运行视频停止于第0帧
	  
	deskMedia.run(1);
	machineReader.Initialize();                               //旋转编码器初始化
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
		//	cout << "isOk:" << machineReader.isOK() << endl;
		//	cout << "Flag:" << machineReader.currentPositionFlag() << endl;
	}
	return 0;
}


