#pragma once
#include <opencv_all.h>
///��״ƥ��---�Ƚ�������״������������ƶ�  
class matchShape
{
public:
	matchShape();
	~matchShape();
	void                                 computeSimilarity();
	bool                                 findMainRectContours(Mat m_matMainImg);
	bool                                 findCommonRectContours(Mat m_matCommonImg);
	double                               similarityValue ();
	double                               isOk();
private:
	vector<vector<Point>>                m_vecMainPointContours, m_vecCommonPointContours;
	Mat                                  m_matMainGrayImg, m_matCommonGrayImg;
	Mat                                  m_matMainTmpImg, m_matCommonTmpImg;
	int                                  m_iMinThreshold = 100;
	int                                  m_iMaxThreshold = 255;
	Scalar                               m_scalarColor = Scalar(255, 0, 0);
	int                                  m_iThickness = 2;
	int                                  m_iLineType = 8;
	double                               m_dMainArea;
	double                               m_dCommonArea;
	double                               m_dReslutSimilarity;
	double                               m_dMinSimliartyAreaTheshold=1;
	double                               m_dMaxSimliartyAreaTheshold = 60;
};