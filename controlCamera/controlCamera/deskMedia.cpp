#include "deskMedia.h"


deskMedia::deskMedia()
{
	m_bParity = true;
	m_vecStrMedia.clear();
	m_vecStrCommonMediaName.clear();
	m_bFirstCircleFlag = false;
}
deskMedia::~deskMedia()
{
	m_bParity = true;
	m_vecStrMedia.clear();
	m_vecStrCommonMediaName.clear();
	m_bFirstCircleFlag = false;

}
int  deskMedia::help(string m_strPath, string m_strMainMedia, string m_strCommonMedia)
{
	string m_strFile;
	m_vecStrMedia.clear();
	m_vecStrCommonMediaName.clear();
	m_vecStrMainMediaName.clear();

	ifstream infile;//定义文件变量  
	infile.open(m_strPath, ios::in);//打开txtPath为文件路径  
	if (!infile)
	{
		return -1;
	}
	
	while (getline(infile, m_strFile)) //读取一行，while循环，直到文件最后一行  
	{

		m_vecStrMedia.push_back(m_strFile);
	}
	infile.close();//关闭文件 
	
	infile.open(m_strCommonMedia, ios::in);//打开txtPath为文件路径  
	if (!infile)
	{
		return -1;
	}

	while (getline(infile, m_strFile)) //读取一行，while循环，直到文件最后一行  
	{

		m_vecStrCommonMediaName.push_back(m_strFile);
	}
	infile.close();//关闭文件 

	infile.open(m_strMainMedia, ios::in);//打开txtPath为文件路径  
	if (!infile)
	{
		return -1;
	}

	while (getline(infile, m_strFile)) //读取一行，while循环，直到文件最后一行  
	{
		m_vecStrMainMediaName.push_back(m_strFile);
	}
	infile.close();//关闭文件 

	m_iClientID = atoi(m_vecStrMedia[0].c_str());
	m_iRectWidth = atoi(m_vecStrMedia[1].c_str());
	m_iRectHeight = atoi(m_vecStrMedia[2].c_str());
	m_iFps = atoi(m_vecStrMedia[3].c_str());
	m_iMediaNum = atoi(m_vecStrMedia[4].c_str());

	m_iCurrentMediaId = -1;
	m_iMinMediaId = 0;
	m_iMaxMediaId = m_vecStrCommonMediaName.size() / m_iMediaNum / 2;

	return 0;
}
void deskMedia::Initialize(PM_Hub * g_PM_Hub, string m_strPath, string m_strMainMedia, string m_strCommonMedia)
{
	help( m_strPath,  m_strMainMedia,  m_strCommonMedia);
	m_PM_Hub = g_PM_Hub;
	m_vecQueue.clear();
	n_vecQueue.clear();
	for (int i = 0; i < m_iMediaNum; i++)
	{
		m_PM_Hub->SetUpClient(i + 1, 0, 0, m_iRectWidth, m_iRectHeight, 1);
		m_vecQueue.push_back(new CQueue(m_PM_Hub));
		n_vecQueue.push_back(new CQueue(m_PM_Hub));
	}
	m_vecMainQueue.push_back(new CQueue(m_PM_Hub));
	m_vecMainQueue.push_back(new CQueue(m_PM_Hub));
}

void deskMedia::transToMedia(int m_iFlag)
{
	
	switch (m_iFlag)
	{
	case 1:
		++m_iCurrentMediaId;
		m_bParity = getBool(m_bParity);
		if (m_iCurrentMediaId >= m_iMaxMediaId)
		{
			m_iCurrentMediaId = m_iMinMediaId;
			m_bParity = false;
			m_bFirstCircleFlag = true;
		}
		m_nStatus = _Play;
		break;
	case 0:
		--m_iCurrentMediaId;
		m_bParity = getBool(m_bParity);
		if (m_iCurrentMediaId < m_iMinMediaId)
		{
			m_iCurrentMediaId = m_iMinMediaId;
			m_bParity = false;
		}
		m_nStatus = _Play;
		break;
	default:
		break;
	}
}
int deskMedia::run(int m_iFlag)
{
	cout << "m_iCurrentMediaId:" << m_iCurrentMediaId << endl;
	if (m_bParity == false)
	{
		switch (m_nStatus)
		{
		case _NULL:
			//m_iIsOverNum = 0;
			transToMedia(m_iFlag);
			break;
		case _Play:
			queueMQ();

			Sleep(500);//延时100ms
			m_nStatus = _IsOver;
			break;
		case _IsOver:
			isOver(2);
			break;
			//case _WaitForTrans:
			//	queueML();

			//	Sleep(1000);//延时100ms
			//	m_nStatus = _NULL;
			//	break;
		case _WaitForTransEnd:
			queueML();
			//Sleep(100);//延时100ms
			m_nStatus = _NULL;
			break;
		default:
			break;

		}
	}
	else if (m_bParity == true)
	{
		switch (m_nStatus)
		{
		case _NULL:
			//m_iIsOverNum = 0;
			transToMedia(m_iFlag);
			break;
		case _Play:
			queueNQ();
			Sleep(500);//延时100ms
			m_nStatus = _IsOver;
			break;
		case _IsOver:
			isOver(1);
			break;
		case _WaitForTransEnd:
			queueNL();
			//Sleep(100);//延时100ms
			m_nStatus = _NULL;
			break;
		default:
			break;

		}
	}
	return 0;
}
bool deskMedia::getBool(bool m_bFlag)
{
	if (m_bFlag == true)
	{
		return false;
	}
	else
	{
		return true;
	}
}
///-------------------------------------------------------------------------------------------------
/// <summary>	/***************判断影片是否结束***************************. </summary>
///
/// <remarks>	Admin, 2017/8/11. </remarks>
///
/// <returns>	True if over, false if not. </returns>
///-------------------------------------------------------------------------------------------------

bool deskMedia::isOver(int m_iFlag)
{
	if (m_iFlag == 2)
	{
		if (getQueueId(m_vecMainQueue[0]->GetQueueID(), m_vecStrMainMediaName[m_iCurrentMediaId * 2]))
			m_bMainMediaOverFlag = true;
		for (unsigned int i = 0; i < n_vecQueue.size(); i++)
		{
			if (getQueueId(n_vecQueue[i]->GetQueueID(), m_vecStrCommonMediaName[m_iCurrentMediaId * m_iMediaNum * 2 + i]))
				m_iIsOverNum++;
		}
	}
	
	else if (m_iFlag == 1)
	{
		if (getQueueId(m_vecMainQueue[1]->GetQueueID(), m_vecStrMainMediaName[m_iCurrentMediaId * 2]))
			m_bMainMediaOverFlag = true;
		for (unsigned int i = 0; i < m_vecQueue.size(); i++)
		{
			if (getQueueId(m_vecQueue[i]->GetQueueID(), m_vecStrCommonMediaName[m_iCurrentMediaId * m_iMediaNum * 2 + i]))
				m_iIsOverNum++;
		}
		
	}
	if (m_iIsOverNum >= 1 && m_bMainMediaOverFlag==true)
	{
		m_iIsOverNum = 0;
		m_bMainMediaOverFlag = false;
		m_nStatus = _WaitForTransEnd;
		return true;
	}
	else
	{
		return false;
	}
}

///-------------------------------------------------------------------------------------------------
/// <summary>	/**************获取片子结束标志位****************. </summary>
///
/// <remarks>	Admin, 2017/8/11. </remarks>
///
/// <param name="queue">	[in,out] If non-null, the queue. </param>
///
/// <returns>	True if it succeeds, false if it fails. </returns>
///-------------------------------------------------------------------------------------------------

bool deskMedia::getQueueId(int m_queueId, string m_strCurrentMediaName)
{
	bool m_getQueueIdFlag = false;
	m_getQueueIdFlag = m_PM_Hub->GetItemEndFeedback(m_queueId, m_strCurrentMediaName);

	return m_getQueueIdFlag;
}

///-------------------------------------------------------------------------------------------------
/// <summary>	/********过场***************. </summary>
///
/// <remarks>	Admin, 2017/8/11. </remarks>
///-------------------------------------------------------------------------------------------------

void deskMedia::queueMQ()
{
	if (m_bFirstCircleFlag == false && m_iCurrentMediaId==0)
	{
		for (unsigned int i = 0; i < m_vecQueue.size(); i++)
		{
			m_vecQueue[i]->EraseQueue(1);
		}
		m_vecMainQueue[0]->EraseQueue(1);
		m_PM_Hub->SendLoaded();
	}

	for (unsigned int i = 0; i < m_vecQueue.size(); i++)
	{
		m_vecQueue[i]->NewQueue(-1, 1, i + 1, 0, 1);
		m_vecQueue[i]->AppendQueue(m_vecStrCommonMediaName[m_iCurrentMediaId * m_iMediaNum * 2 + i], "cmp", m_iFps, 0, 0, 1, 1);
		m_vecQueue[i]->SetCoords(m_iRectWidth / 2, m_iRectHeight / 2, 1, m_iRectWidth / 2, m_iRectHeight / 2, 1);
		m_vecQueue[i]->SetStartTime(-1, 1);
		m_vecQueue[i]->MediaSeek(0, 0, 1);
	}

	m_vecMainQueue[0]->NewQueue(-1, 1, m_vecQueue.size() + 1, 0, 1);
	m_vecMainQueue[0]->AppendQueue(m_vecStrMainMediaName[m_iCurrentMediaId  * 2], "cmp", m_iFps, 0, 0, 1, 1);
	m_vecMainQueue[0]->SetCoords(m_iRectWidth / 2, m_iRectHeight / 2, 1, m_iRectWidth / 2, m_iRectHeight / 2, 1);
	m_vecMainQueue[0]->SetStartTime(-1, 1);
	m_vecMainQueue[0]->MediaSeek(0, 0, 1);

	m_PM_Hub->SendLoaded();

}

///-------------------------------------------------------------------------------------------------
/// <summary>	/*********视频效果************. </summary>
///
/// <remarks>	Admin, 2017/8/11. </remarks>
///-------------------------------------------------------------------------------------------------

void deskMedia::queueML()
{
	for (unsigned int i = 0; i < m_vecQueue.size(); i++)
	{
		m_vecQueue[i]->EraseQueue(1);
	}
	m_PM_Hub->SendLoaded();//发送使能
	for (unsigned int i = 0; i < m_vecQueue.size(); i++)
	{
		n_vecQueue[i]->EraseQueue(1);
	}
	m_vecMainQueue[1]->EraseQueue(1);
	m_PM_Hub->SendLoaded();//发送使能
	for (unsigned int i = 0; i < m_vecQueue.size(); i++)
	{
		m_vecQueue[i]->NewQueue(-1, 1, i + 1, 0, 1);
		m_vecQueue[i]->AppendQueue(m_vecStrCommonMediaName[m_iCurrentMediaId * m_iMediaNum * 2 + i + m_iMediaNum], "cmp", m_iFps, 0, 0, 1, 1);
		m_vecQueue[i]->SetCoords(m_iRectWidth / 2, m_iRectHeight / 2, 1, m_iRectWidth / 2, m_iRectHeight / 2, 1);
		m_vecQueue[i]->SetStartTime(-1, 1);
		m_vecQueue[i]->MediaSeek(0, 0, 1);
	}

	m_vecMainQueue[0]->NewQueue(-1, 1, m_vecQueue.size() + 1, 0, 1);
	m_vecMainQueue[0]->AppendQueue(m_vecStrMainMediaName[m_iCurrentMediaId * m_iMediaNum * 2+1], "cmp", m_iFps, 0, 0, 1, 1);
	m_vecMainQueue[0]->SetCoords(m_iRectWidth / 2, m_iRectHeight / 2, 1, m_iRectWidth / 2, m_iRectHeight / 2, 1);
	m_vecMainQueue[0]->SetStartTime(-1, 1);
	m_vecMainQueue[0]->MediaSeek(0, 0, 1);

	m_PM_Hub->SendLoaded();
}


void deskMedia::queueNQ()
{
	/*for (int i = 0; i < m_vecQueue.size(); i++)
	{
		m_vecQueue[i]->EraseQueue(1);
		n_vecQueue[i]->EraseQueue(1);
	}
	m_PM_Hub->SendLoaded();*/
	//m_queue2->EraseQueue(1);
	//m_PM_Hub->SendLoaded();//发送使能
	for (unsigned int i = 0; i < n_vecQueue.size(); i++)
	{
		n_vecQueue[i]->NewQueue(-1, 1, i + 1, 0, 1);
		n_vecQueue[i]->AppendQueue(m_vecStrCommonMediaName[m_iCurrentMediaId * m_iMediaNum * 2 + i], "cmp", m_iFps, 0, 0, 1, 1);
		n_vecQueue[i]->SetCoords(m_iRectWidth / 2, m_iRectHeight / 2, 1, m_iRectWidth / 2, m_iRectHeight / 2, 1);
		n_vecQueue[i]->SetStartTime(-1, 1);
		n_vecQueue[i]->MediaSeek(0, 0, 1);
	}


	m_vecMainQueue[1]->NewQueue(-1, 1, m_vecQueue.size() + 1, 0, 1);
	m_vecMainQueue[1]->AppendQueue(m_vecStrMainMediaName[m_iCurrentMediaId  * 2 ], "cmp", m_iFps, 0, 0, 1, 1);
	m_vecMainQueue[1]->SetCoords(m_iRectWidth / 2, m_iRectHeight / 2, 1, m_iRectWidth / 2, m_iRectHeight / 2, 1);
	m_vecMainQueue[1]->SetStartTime(-1, 1);
	m_vecMainQueue[1]->MediaSeek(0, 0, 1);

	m_PM_Hub->SendLoaded();

}

///-------------------------------------------------------------------------------------------------
/// <summary>	/*********视频效果************. </summary>
///
/// <remarks>	Admin, 2017/8/11. </remarks>
///-------------------------------------------------------------------------------------------------

void deskMedia::queueNL()
{
	for (unsigned int i = 0; i < m_vecQueue.size(); i++)
	{
		m_vecQueue[i]->EraseQueue(1);
	}

	m_vecMainQueue[0]->EraseQueue(1);
    m_PM_Hub->SendLoaded();//发送使能

	for (unsigned int i = 0; i < m_vecQueue.size(); i++)
	{
		n_vecQueue[i]->EraseQueue(1);
	}
	m_PM_Hub->SendLoaded();//发送使能

	for (unsigned int i = 0; i < n_vecQueue.size(); i++)
	{
		n_vecQueue[i]->NewQueue(-1, 1, i + 1, 0, 1);
		n_vecQueue[i]->AppendQueue(m_vecStrCommonMediaName[m_iCurrentMediaId * m_iMediaNum * 2 + i + m_iMediaNum], "cmp", m_iFps, 0, 0, 1, 1);
		n_vecQueue[i]->SetCoords(m_iRectWidth / 2, m_iRectHeight / 2, 1, m_iRectWidth / 2, m_iRectHeight / 2, 1);
		n_vecQueue[i]->SetStartTime(-1, 1);
		n_vecQueue[i]->MediaSeek(0, 0, 1);
	}

	m_vecMainQueue[1]->NewQueue(-1, 1, m_vecQueue.size() + 1, 0, 1);
	m_vecMainQueue[1]->AppendQueue(m_vecStrMainMediaName[m_iCurrentMediaId  * 2 + 1], "cmp", m_iFps, 0, 0, 1, 1);
	m_vecMainQueue[1]->SetCoords(m_iRectWidth / 2, m_iRectHeight / 2, 1, m_iRectWidth / 2, m_iRectHeight / 2, 1);
	m_vecMainQueue[1]->SetStartTime(-1, 1);
	m_vecMainQueue[1]->MediaSeek(0, 0, 1);

	m_PM_Hub->SendLoaded();

}

void deskMedia::clear()
{
	m_bParity = false;
	m_vecQueue.clear();
	n_vecQueue.clear();
	m_vecMainQueue.clear();
	if (m_PM_Hub != NULL)
	{
		//	delete m_PM_Hub;
		m_PM_Hub = NULL;
	}
}