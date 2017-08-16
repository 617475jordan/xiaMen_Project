#include "deskMedia.h"


deskMedia::deskMedia()
{
	m_bParity = true;
	m_vecStrMedia.clear();
	m_vecStrMediaName.clear();
	m_bFirstCircleFlag = false;
}
deskMedia::~deskMedia()
{
	m_bParity = true;
	m_vecStrMedia.clear();
	m_vecStrMediaName.clear();
	m_bFirstCircleFlag = false;

}
int deskMedia::help(string m_strPath)
{
	m_vecStrMedia.clear();
	m_vecStrMediaName.clear();
	ifstream infile;//定义文件变量  
	infile.open(m_strPath, ios::in);//打开txtPath为文件路径  
	if (!infile)
	{
		return -1;
	}
	string m_strFile;
	int n_icount = 0;
	while (getline(infile, m_strFile)) //读取一行，while循环，直到文件最后一行  
	{
		if (n_icount <= 3)
		{
			m_vecStrMedia.push_back(m_strFile);
		}
	}

	infile.close();//关闭文件 
	infile.open("..\\data\\name.txt", ios::in);
	while (getline(infile, m_strFile))
	{
		m_vecStrMediaName.push_back(m_strFile);
	}
	m_iClientID = atoi(m_vecStrMedia[0].c_str());
	m_iRectWidth = atoi(m_vecStrMedia[1].c_str());
	m_iRectHeight = atoi(m_vecStrMedia[2].c_str());
	m_iFps = atoi(m_vecStrMedia[3].c_str());

	m_iCurrentMediaId = -1;
	m_iMinMediaId = 0;
	m_iMaxMediaId = m_vecStrMediaName.size() / m_iMediaNum / 2;
	return 0;
}
void deskMedia::Initialize(PM_Hub * g_PM_Hub, string path)
{
	help(path);
	m_PM_Hub = g_PM_Hub;
	m_vecQueue.clear();
	n_vecQueue.clear();
	for (int i = 0; i < m_iMediaNum; i++)
	{
		m_PM_Hub->SetUpClient(i + 1, 0, 0, m_iRectWidth, m_iRectHeight, 1);
		m_vecQueue.push_back(new CQueue(m_PM_Hub));
		n_vecQueue.push_back(new CQueue(m_PM_Hub));
	}
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
	if (m_bParity == false)
	{
		switch (m_nStatus)
		{
		case _NULL:
			transToMedia(m_iFlag);
			break;
		case _Play:
			queueMQ();

			//Sleep(100);//延时100ms
			m_nStatus = _IsOver;
			break;
		case _IsOver:
			isOver();
			break;
			//case _WaitForTrans:
			//	queueML();

			//	Sleep(1000);//延时100ms
			//	m_nStatus = _NULL;
			//	break;
		case _WaitForTransEnd:
			queueML();
			Sleep(100);//延时100ms
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
			transToMedia(m_iFlag);
			break;
		case _Play:
			queueNQ();
			//Sleep(100);//延时100ms
			m_nStatus = _IsOver;
			break;
		case _IsOver:
			isOver();
			break;
		case _WaitForTransEnd:
			queueNL();
			Sleep(100);//延时100ms
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

bool deskMedia::isOver()
{
	if (m_nStatus == _IsOver)
	{
		if (m_bParity == true)
		{
			for (int i = 0; i < n_vecQueue.size(); i++)
			{
				if (getQueueId(n_vecQueue[i]->GetQueueID(), m_vecStrMediaName[m_iCurrentMediaId * m_iMediaNum * 2 + i]))
					m_iIsOverNum++;
			}
		}
		else if (m_bParity == false)
		{
			for (int i = 0; i < m_vecQueue.size(); i++)
			{
				if (getQueueId(m_vecQueue[i]->GetQueueID(), m_vecStrMediaName[m_iCurrentMediaId * m_iMediaNum * 2 + i]))
					m_iIsOverNum++;
			}
		}
	}
	if (m_iIsOverNum >= m_iMediaNum)
	{
		m_iIsOverNum = 0;
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
	for (int i = 0; i < m_vecQueue.size(); i++)
	{
		m_vecQueue[i]->EraseQueue(1);
		n_vecQueue[i]->EraseQueue(1);
	}
	m_PM_Hub->SendLoaded();
	//m_queue2->EraseQueue(1);
	//m_PM_Hub->SendLoaded();//发送使能
	for (int i = 0; i < m_vecQueue.size(); i++)
	{
		m_vecQueue[i]->NewQueue(-1, 1, i + 1, 0, 1);
		m_vecQueue[i]->AppendQueue(m_vecStrMediaName[i], "cmp", m_iFps, 0, 0, 1, 1);
		m_vecQueue[i]->SetCoords(m_iRectWidth / 2, m_iRectHeight / 2, 1, m_iRectWidth / 2, m_iRectHeight / 2, 1);
		m_vecQueue[i]->SetStartTime(-1, 1);
		m_vecQueue[i]->MediaSeek(0, 0, 1);
	}

	m_PM_Hub->SendLoaded();

}

///-------------------------------------------------------------------------------------------------
/// <summary>	/*********视频效果************. </summary>
///
/// <remarks>	Admin, 2017/8/11. </remarks>
///-------------------------------------------------------------------------------------------------

void deskMedia::queueML()
{
	for (int i = 0; i < m_vecQueue.size(); i++)
	{
		m_vecQueue[i]->EraseQueue(1);
		n_vecQueue[i]->EraseQueue(1);
	}

	m_PM_Hub->SendLoaded();//发送使能

	for (int i = 0; i < m_vecQueue.size(); i++)
	{
		m_vecQueue[i]->NewQueue(-1, 1, i + 1, 0, 1);
		m_vecQueue[i]->AppendQueue(m_vecStrMediaName[m_iCurrentMediaId * m_iMediaNum * 2 + i + m_iMediaNum], "cmp", m_iFps, 0, 0, 1, 1);
		m_vecQueue[i]->SetCoords(m_iRectWidth / 2, m_iRectHeight / 2, 1, m_iRectWidth / 2, m_iRectHeight / 2, 1);
		m_vecQueue[i]->SetStartTime(-1, 1);
		m_vecQueue[i]->MediaSeek(0, 0, 1);
	}
	m_PM_Hub->SendLoaded();
}


void deskMedia::queueNQ()
{
	for (int i = 0; i < m_vecQueue.size(); i++)
	{
		m_vecQueue[i]->EraseQueue(1);
		n_vecQueue[i]->EraseQueue(1);
	}
	m_PM_Hub->SendLoaded();
	//m_queue2->EraseQueue(1);
	//m_PM_Hub->SendLoaded();//发送使能
	for (int i = 0; i < n_vecQueue.size(); i++)
	{
		n_vecQueue[i]->NewQueue(-1, 1, i + 1, 0, 1);
		n_vecQueue[i]->AppendQueue(m_vecStrMediaName[m_iCurrentMediaId * m_iMediaNum * 2 + i], "cmp", m_iFps, 0, 0, 1, 1);
		n_vecQueue[i]->SetCoords(m_iRectWidth / 2, m_iRectHeight / 2, 1, m_iRectWidth / 2, m_iRectHeight / 2, 1);
		n_vecQueue[i]->SetStartTime(-1, 1);
		n_vecQueue[i]->MediaSeek(0, 0, 1);
	}

	m_PM_Hub->SendLoaded();

}

///-------------------------------------------------------------------------------------------------
/// <summary>	/*********视频效果************. </summary>
///
/// <remarks>	Admin, 2017/8/11. </remarks>
///-------------------------------------------------------------------------------------------------

void deskMedia::queueNL()
{
	for (int i = 0; i < m_vecQueue.size(); i++)
	{
		m_vecQueue[i]->EraseQueue(1);
		n_vecQueue[i]->EraseQueue(1);
	}

	m_PM_Hub->SendLoaded();//发送使能

	for (int i = 0; i < n_vecQueue.size(); i++)
	{
		n_vecQueue[i]->NewQueue(-1, 1, i + 1, 0, 1);
		n_vecQueue[i]->AppendQueue(m_vecStrMediaName[m_iCurrentMediaId * m_iMediaNum * 2 + i + m_iMediaNum], "cmp", m_iFps, 0, 0, 1, 1);
		n_vecQueue[i]->SetCoords(m_iRectWidth / 2, m_iRectHeight / 2, 1, m_iRectWidth / 2, m_iRectHeight / 2, 1);
		n_vecQueue[i]->SetStartTime(-1, 1);
		n_vecQueue[i]->MediaSeek(0, 0, 1);
	}

	m_PM_Hub->SendLoaded();

}

void deskMedia::clear()
{
	m_bParity = false;
	m_vecQueue.clear();
	n_vecQueue.clear();
	if (m_PM_Hub != NULL)
	{
	//	delete m_PM_Hub;
		m_PM_Hub = NULL;
	}
}