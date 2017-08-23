#include <opencv_all.h>
#include "drawing.h"
Mat m_srcImg;
Mat m_grayImg;
Mat foreground;
Mat background;
int thresh = 30;
int max_thresh = 255;
float PI = acos(-1.0);
int main()
{
	drawing drawing;
	m_srcImg = imread("..\\data\\1.jpg");    //×¢ÒâÂ·¾¶µÃ»»³É×Ô¼ºµÄ 
	resize(m_srcImg, m_srcImg, Size(320, 240));
	cvtColor(m_srcImg, foreground, CV_RGB2GRAY);
	while (1)
	{
		threshold(foreground, foreground, 150, 255, THRESH_BINARY);
		foreground.setTo(0);
		m_grayImg=drawing.drawingContourMomentAndPointCentroid(foreground, m_srcImg);
		imshow("m_grayImg", m_grayImg);
		waitKey(1);
	}
	
	//VideoCapture capture;
	//BackgroundSubtractorMOG2 mog;
	//capture.open("..\\data\\panda.mpg");
	//
	//if (!capture.isOpened())
	//{
	//	return -1;
	//}
	//int m_icount = 0;
	//while (1)
	//{
	//	int t = clock();
	//	m_icount++;
	//	//m_srcImg = imread("..\\data\\2.jpg");
	//	capture >> m_srcImg;
	//	if (m_srcImg.empty())
	//	{
	//		return -1;
	//	}
	//	//imshow("m_srcImg", m_srcImg);
	//	resize(m_srcImg, m_srcImg, Size(320, 240));
	//	//cvtColor(m_srcImg, m_grayImg, CV_BGR2GRAY);//»Ò¶È»¯  
	//	//cvtColor(m_grayImg, m_srcImg, CV_GRAY2RGB);
	//	//GaussianBlur(m_srcImg, m_srcImg, Size(3, 3), 0.1, 0, BORDER_DEFAULT);
	//	//blur(m_grayImg, m_grayImg, Size(3, 3)); //ÂË²¨       
	//	////namedWindow("image", CV_WINDOW_AUTOSIZE);
	//	////imshow("image", m_srcImg);
	//	////moveWindow("image", 20, 20);
	//	////¶¨ÒåCanny±ßÔµ¼ì²âÍ¼Ïñ       
	//	//Mat m_cannyImg;
	//	////ÀûÓÃcannyËã·¨¼ì²â±ßÔµ       
	//	//Canny(m_grayImg, m_cannyImg, thresh, max_thresh, 3);
	//   
	//	
	//	mog(m_srcImg, foreground, 0.001);
	//	// ¸¯Ê´  
	//	erode(foreground, foreground, cv::Mat());
	//	// ÅòÕÍ  
	//	dilate(foreground, foreground, cv::Mat());
	//	mog.getBackgroundImage(background);   // ·µ»Øµ±Ç°±³¾°Í¼Ïñ  
	//	//imshow("video", foreground);
	//	//imshow("background", background);
	//	if (m_icount >= 1)
	//	{
	//		drawing drawing;
	//		Mat m_MatImg, m_MatThresholdImg;
	//		static const int nY20_thresh = 50;

	//		
	//		threshold(foreground, foreground, nY20_thresh, 255, THRESH_BINARY);
	//		m_MatImg = drawing.drawingContourMomentAndPointCentroid(foreground, m_srcImg);
	//		namedWindow("Contours", CV_WINDOW_AUTOSIZE);
	//		imshow("Contours", m_MatImg);
	//		//moveWindow("Contours", 1100, 20);
	//		waitKey(1);
	//	}
	//	else
	//	{
	//		namedWindow("Contours", CV_WINDOW_AUTOSIZE);
	//		imshow("Contours", m_srcImg);
	//		waitKey(1);
	//	}
	//	if (waitKey(10)=='c')
	//	{
	//		waitKey();
	//	}
	//	cout << "time:" << clock() - t << endl;
	//}
	waitKey(0);
	m_srcImg.release();
	m_grayImg.release();
	return 0;
}