#pragma once
#include <opencv_all.h>
#include "matchShape.h"
#include "RegionGrowing.h"

class drawing
{
public:
	drawing();
	~drawing();
	Mat                       drawingContourMomentAndPointCentroid(Mat m_matCommonImg,Mat m_matMainImg);
	Mat                       drawingImage(Mat m_matMainImg);
private:
	vector<Moments>           m_vecContourMoment;//����������  
	vector<Point2f>           m_vecPointCentroid;//��������������  
	vector<Point2f>           m_vecPointCentroidOut;//��������������  
	vector<vector<Point>>     m_vecContours;
	vector<vector<Point>>     m_vecFilterContours;
	vector<Rect>              m_vecRectangle;
	vector<Rect>              m_vecRec;
	vector<Vec4i>             m_vecHierarchy;
	vector<Vec4i>             m_vecOutHierarchy;
	double                     m_fPI = acos(-1.0);
	char                      m_charPictureName[100] ;
	matchShape                matchShape;
	Scalar                    m_scalarLineColor = Scalar(255, 0, 0);
	Scalar                    m_scalarCircleColor = Scalar(0, 0, 255);
	Scalar                    m_scalarRectangleColor = Scalar(0,255,0);
	Scalar                    m_scalarTextColor = Scalar(255, 0, 255);
	int                       m_iRaduis = 4;
	int                       m_iType = 8;
	int                       m_iThickness = 1;
	int                       m_iPictureNum = 1;

#define                       m_strPictureName        "..\\data\\coco2.png"

private:
	regionGrowing             m_regionGrowing;
	int                       m_iGrowPoint = 120;
	int                       m_iGrowJudge = 4;
};