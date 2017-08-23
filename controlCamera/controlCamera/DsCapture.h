#pragma once
#include <Windows.h>
#include <tchar.h>
#include "DsCaptureImpExp.h"
#include "ICamera.h"
//// �����Ǵ� DsCapture.dll ������
//class DSCAPTURE_API CDsCapture {
//public:
//	CDsCapture(void);
//	// TODO: �ڴ�������ķ�����
//};
//
//extern DSCAPTURE_API int nDsCapture;
//
//DSCAPTURE_API int fnDsCapture(void);

class ARFrameGrabber;

class DSCAPTURE_API CDsCapture: public ICamera
{
public:
  CDsCapture()
  {
  }
  virtual ~CDsCapture()
  {
  }
  virtual bool Initialize(const char* cFileLoad);
  virtual bool GetCurrentImg(Mat& img, vector<cv::Point3f>& vecPoint);
  virtual IplImage* GetImage();
  virtual void Clear();

private:
  ARFrameGrabber* m_pgrabber;
};