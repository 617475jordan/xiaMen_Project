#pragma once
#include "PM_Hub.h"
#include <iostream>
#include<sstream>     
#include<iostream>    
#include<fstream>     
#include<string>
#ifdef _DEBUG
#pragma comment(lib, "CommuDll30D.lib")
#pragma comment(lib, "CommonUtil31D.lib")
#pragma comment(lib, "PlayerManager31D.lib")
#pragma comment(lib, "GlobalSyncTimeManager31D.lib")
#else

#pragma comment(lib, "CommuDll30.lib")
#pragma comment(lib, "CommonUtil31.lib")
#pragma comment(lib, "PlayerManager31.lib")
#pragma comment(lib, "GlobalSyncTimeManager31.lib")
#endif

class deskMedia
{
public:
	deskMedia();
	~deskMedia();

	int  run(int flag);
	int  help(string m_strPath,string m_strMainMedia,string m_strCommonMedia);

	bool isOver(int m_iFlag);

	void clear();
	void queueMQ();
	void queueML();
	void queueNQ();
	void queueNL();
	void Initialize(PM_Hub * g_PM_Hub, string m_strPath, string m_strMainMedia, string m_strCommonMedia);
	bool getQueueId(int m_queueId, string m_strCurrentMediaName);
	bool getBool(bool m_bFlag);
	void transToMedia(int m_iFlag);
private:
	enum MyEnum
	{
		_NULL=0,
		_Play,
		_IsOver,
		_WaitForTrans,
		_WaitForTransEnd
	};
	int				m_nStatus=0;
	int             m_iIsOverNum = 0;

	int  m_iClientID, m_iRectWidth, m_iRectHeight, m_iFps;
	//int m_iSoloClient
	int  m_iCurrentMediaId = -1;
	int  m_iMaxMediaId;
	int  m_iMinMediaId = 0;
	int  m_iFileId = 4;
	int  m_iMediaNum;
	bool m_bParity;                  //��żλ��־����ʼ��false
	bool m_bFirstCircleFlag;              //��ʾ�Ƿ�Ϊ��һȦ,false
	bool m_bMainMediaOverFlag = false;

	PM_Hub * m_PM_Hub;
	vector<string> m_vecStrMedia;
	vector<string> m_vecStrCommonMediaName, m_vecStrMainMediaName;
	vector<CQueue*> m_vecQueue,n_vecQueue,m_vecMainQueue;
};

