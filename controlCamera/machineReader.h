#pragma once
#include <opencv_all.h>
class MFastUDP;

class machineReader
{
public:
	machineReader();
	~machineReader();
	void                   currentPosition(float position);
	void                   Initialize();
	void                   run();
	void                   clear();
	int                    currentPositionFlag();
	bool                   isOK();
private:
	float                  m_fCurrentPosition;
	float                  m_fOriginalPosition;
	float                  m_fSpeend;
	float                  m_fThreshold = 50;//Ðý×ª½Ç¶ÈãÐÖµ

	int                    m_fCurrentPositionFlag;
	int                    m_iLen;
	int                    m_iOriginalTime;
	int                    m_iCurrentTime;
	bool                   m_bIsOk;
	
private:
	MFastUDP*			   m_pUDP;

};




