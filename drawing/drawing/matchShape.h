#pragma once
#include <opencv_all.h>
///形状匹配---比较两个形状或轮廓间的相似度  
class matchShape
{
public:
	matchShape();
	~matchShape();
	void                                 computeSimilarity();
	void                                 findMainRectContours(Mat m_matMainImg);
	void                                 findCommonRectContours(Mat m_matCommonImg);
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
	char                                 m_charPictureName[100];
	string                               m_strPictureName = "..\\data\\test%d.jpg";
};