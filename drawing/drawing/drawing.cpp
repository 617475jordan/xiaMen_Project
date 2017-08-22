#include "drawing.h"

drawing::drawing()
{

}
drawing::~drawing()
{

}

Mat drawing::drawingContourMomentAndPointCentroid(Mat m_matCommonImg, Mat m_matMainImg)
{
	m_vecFilterContours.clear();
	m_vecContours.clear();
	//查找轮廓 
	findContours(m_matCommonImg, m_vecContours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	for (int i = 0; i < m_vecContours.size(); i++)
	{
		if (contourArea(m_vecContours[i])>m_dAreaThreshold&&contourArea(m_vecContours[i])<m_dAreaThreshold * 8)
		{
			m_vecFilterContours.push_back(m_vecContours[i]);
		}
	}

	//计算轮廓矩
	for (int i = 0; i < m_vecFilterContours.size(); i++)
	{
		m_vecContourMoment.push_back(moments(m_vecFilterContours[i], false));
	}

	//计算轮廓的质心
	for (int i = 0; i < m_vecFilterContours.size(); i++)
	{
		m_vecPointCentroid.push_back(Point2d(m_vecContourMoment[i].m10 / m_vecContourMoment[i].m00,
			m_vecContourMoment[i].m01 / m_vecContourMoment[i].m00));
	}
	return drawingImage(m_matMainImg);
}

Mat drawing::drawingImage( Mat m_matMainImg)
{
	Mat drawing;
	m_matMainImg.copyTo(drawing);
	m_vecRec.clear();

	for (int i = 0; i < m_vecFilterContours.size(); i++)
	{
		m_vecRec.push_back(boundingRect(m_vecFilterContours.at(i)));
	}
	m_vecRectangle.clear();
	m_vecContours.clear();
	m_vecPointCentroidOut.clear();
	/***********************矩形框嵌套消除*****************/
	for (int i = 0; i < m_vecRec.size(); i++)
	{
		Rect r = m_vecRec[i];

		int j = 0;
		for (; j < m_vecRec.size(); j++)
		{
			//如果时嵌套的就推出循环
			if (j != i && (r & m_vecRec[j]) == r)
				break;
		}
		if (j == m_vecRec.size())
		{
			m_vecRectangle.push_back(r);
			m_vecContours.push_back(m_vecFilterContours[i]);
			m_vecPointCentroidOut.push_back(m_vecPointCentroid[i]);
		}
	}
	Mat m_matTmpImg;

	for (int i = 0; i < m_vecRectangle.size(); i++)
	{
		//Rect m_rect;
		Mat m_rect(drawing, Rect(m_vecRectangle[i].x, m_vecRectangle[i].y, m_vecRectangle[i].width, m_vecRectangle[i].height));
		//imshow("m_rect", m_rect);
		int m_iMaxSimilarityFlag = -1;
		double m_dMaxSimilarity = 0;
		matchShape.findMainRectContours(m_rect);
		for (unsigned int j = 1; j <= m_iPictureNum; j++)
		{
			sprintf(m_charPictureName, m_strPictureName, j);
			m_matTmpImg = imread(m_charPictureName);
			matchShape.findCommonRectContours(m_matTmpImg);
			matchShape.isOk();
			if (matchShape.similarityValue() > m_dMaxSimilarity)
			{
				m_dMaxSimilarity = matchShape.similarityValue();
				m_iMaxSimilarityFlag = j;
			}
		}
		double m_dminThreshold = matchShape.similarityValue();
		if (m_dminThreshold > 0.85)
		{

			//drawContours(drawing, m_vecContours, i, m_scalarColor, 2, 8, -1, 0, Point());
			circle(drawing, m_vecPointCentroidOut[i], m_iRaduis, m_scalarCircleColor, -1, m_iType, 0);
			rectangle(drawing, m_vecRectangle[i], m_scalarRectangleColor);

			/**********最小外接矩形*****************/
			RotatedRect rect = minAreaRect(m_vecContours[i]);
			Point2f P[4];
			rect.points(P);
			float m_iThreshold = 0;
			float m_fWidth, m_fHeight;
			float m_fSum;
			int   m_iCoordinateFlag = 0;
			for (unsigned int j = 0; j <= 3; j++)
			{
				int m_iTempThreshold;
				line(drawing, P[j], P[(j + 1) % 4], m_scalarLineColor, m_iThickness);
				m_fWidth = abs(P[j].x - P[(j + 1) % 4].x);
				m_fHeight = abs(P[j].y - P[(j + 1) % 4].y);
				m_fSum = sqrt(m_fWidth*m_fWidth + m_fHeight*m_fHeight);
				if (m_fSum > m_iThreshold)
				{
					m_iThreshold = m_fSum;
					m_iCoordinateFlag = j;
				}

			}
			m_iThreshold = (P[m_iCoordinateFlag].y - P[(m_iCoordinateFlag + 1) % 4].y) / (P[m_iCoordinateFlag].x - P[(m_iCoordinateFlag + 1) % 4].x);
			char m_charFile[100];
			m_iThreshold = atan(m_iThreshold) * 180 / m_fPI;
			if (m_iThreshold < 0)
			{
				m_iThreshold = abs(m_iThreshold);
			}
			else
			{
				m_iThreshold = 180 - m_iThreshold;
			}
			sprintf(m_charPictureName, m_strPictureName, m_iMaxSimilarityFlag);
			sprintf(m_charFile, "(%0.0f,%0.0f,%0.0f)", m_vecPointCentroid[i].x, m_vecPointCentroid[i].y, m_iThreshold);
			putText(drawing, m_charFile, Point(m_vecPointCentroid[i].x, m_vecPointCentroid[i].y), FONT_HERSHEY_SIMPLEX, 0.4, m_scalarTextColor, 1);
			putText(drawing, m_charPictureName, Point(m_vecPointCentroid[i].x, m_vecPointCentroid[i].y + 10), FONT_HERSHEY_SIMPLEX, 0.4, m_scalarTextColor, 1);
		}



	}
	return drawing;
}