#include "matchShape.h"
///形状匹配---比较两个形状或轮廓间的相似度  
matchShape::matchShape()
{
	m_vecMainPointContours.clear();
	m_vecCommonPointContours.clear();
}
matchShape::~matchShape()
{
	m_vecMainPointContours.clear();
	m_vecCommonPointContours.clear();
}

///-------------------------------------------------------------------------------------------------
/// <summary>	Match shape find contours. </summary>
///
/// <remarks>	Admin, 2017/8/18. </remarks>
///  m_matSrc_1,  m_matSrc_2  must be rgb
/// <param name="m_matSrc_1">	The first m matrix source. </param>
/// <param name="m_matSrc_2">	The second m matrix source. </param>
///-------------------------------------------------------------------------------------------------

void matchShape::computeSimilarity()
{

	double m_dMinSimilarity = 1;
	
	for (int i = 0; i < m_vecCommonPointContours.size(); i++)//遍历待测试图像的轮廓  
	{
		//返回此轮廓与模版轮廓之间的相似度,a0越大越相似  
		m_dReslutSimilarity = 1 - matchShapes(m_vecMainPointContours[0], m_vecCommonPointContours[i], CV_CONTOURS_MATCH_I1, 0);
		if (m_dReslutSimilarity < m_dMinSimilarity)
			m_dMinSimilarity = m_dReslutSimilarity;
		//if (m_dReslutSimilarity>0.8)//如果此轮廓与模版轮廓的相似度大于0.8  
		//{
		//	drawContours(m_matMainTmpImg, m_vecCommonPointContours, i, Scalar(0, 255, 0), 2, 8);//则在待测试图像上画出此轮廓  
		//}
	}
	if (m_dReslutSimilarity < 0)
		m_dReslutSimilarity = 0;
	else if (m_dReslutSimilarity >1)
		m_dReslutSimilarity = 1;
	cout << "模版轮廓与待测试图像轮廓" << "的相似度:" << m_dReslutSimilarity << endl;//输出两个轮廓间的相似度 
}

bool matchShape::findMainRectContours(Mat m_matMainImg)
{
	m_vecMainPointContours.clear();
	m_dMainArea = 0;
	m_matMainImg.copyTo(m_matMainTmpImg);
	cvtColor(m_matMainImg, m_matMainGrayImg, CV_RGB2GRAY);
	threshold(m_matMainGrayImg, m_matMainGrayImg, m_iMinThreshold, m_iMaxThreshold, CV_THRESH_BINARY);//确保黑中找白
	findContours(m_matMainGrayImg, m_vecMainPointContours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);//最外层轮廓  
	//drawContours(m_matMainGrayImg, m_vecMainPointContours, -1, m_scalarColor, m_iThickness, m_iLineType);
	if (m_vecMainPointContours.size() == 0)
	{
		return false;
	}
	else
	{
		for (unsigned int i = 0; i < m_vecMainPointContours.size(); i++)
		{
			if (contourArea(m_vecMainPointContours[i])>0)
				m_dMainArea = m_dMainArea + contourArea(m_vecMainPointContours[i]);
		}
		return true;
	}
	
}

bool matchShape::findCommonRectContours(Mat m_matCommonImg)
{
	resize(m_matCommonImg, m_matCommonImg, Size(320, 240));
	m_vecCommonPointContours.clear();
	m_dCommonArea = 0;
	m_matCommonImg.copyTo(m_matCommonTmpImg);
	cvtColor(m_matCommonImg, m_matCommonGrayImg, CV_RGB2GRAY);
	threshold(m_matCommonGrayImg, m_matCommonGrayImg, m_iMinThreshold, m_iMaxThreshold, CV_THRESH_BINARY);//确保黑中找白
	findContours(m_matCommonGrayImg, m_vecCommonPointContours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);//最外层轮廓  
	//drawContours(m_matCommonGrayImg, m_vecCommonPointContours, -1, m_scalarColor, m_iThickness, m_iLineType);
	if (m_vecCommonPointContours.size() == 0)
	{
		return false;
	}
	else
	{
		for (unsigned int i = 0; i < m_vecCommonPointContours.size(); i++)
		{
			if (contourArea(m_vecCommonPointContours[i])>0)
				m_dCommonArea = m_dCommonArea + contourArea(m_vecCommonPointContours[i]);
		}
		return true;
	}

}
double matchShape::similarityValue()
{
	return m_dReslutSimilarity;
}
double matchShape::isOk()
{
	if (m_dMainArea == 0)
	{
		return -1;
	}
	double m_dReslut = m_dCommonArea / m_dMainArea;
	if (m_dReslut >= m_dMinSimliartyAreaTheshold&&m_dReslut <= m_dMaxSimliartyAreaTheshold)
		computeSimilarity();
	//cout << "m_dReslut" << m_dReslut << endl;
	return m_dReslut;
}