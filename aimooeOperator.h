#pragma once
#include "windows.h"
#include "direct.h"
#include "io.h"
#include <iostream>
#include <math.h>
#include<algorithm>
#include<thread>
#include<mutex>
#include<cmath>
#include<Eigen/Core>
#include <Eigen/Dense>
#include<opencv2/core/types_c.h>
#include<opencv2/opencv.hpp>
#include<opencv2/imgproc/types_c.h>

#include <vtkSmartPointer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleImage.h>
#include <vtkRenderer.h>
#include <vtkBMPReader.h>
#include <vtkImageActor.h>
#include <vtkImageData.h>
#include <vtkImageImport.h>
#include <vtkImageFlip.h>
#include <QObject>
#include<QThread>
#include "AimPositionAPI.h"
#include "AimPositionDef.h"
//#include "vld.h"
using   namespace   std;
#ifdef _DEBUG
#ifdef _WIN64
#pragma comment(lib,"AimPosition2.3.3d.lib")
#else
#pragma comment(lib,"AimPosition2.3.3dX86.lib")
#endif
#else
#ifdef _WIN64
#pragma comment(lib,"AimPosition2.3.3.lib")
#else
#pragma comment(lib,"AimPosition2.3.3X86.lib")
#endif

#endif // _DEBUG

#define GET_DATA_FROM_HARDWARE 0
struct toolPosition {
	std::string name;
	float *Tto=new float[3];
	float Rto[3][3];
	float* qR=new float[4];
	float* vR = new float[3];
		
};
enum E_BMPBIT
{
	BMP8BIT,
	BMP16BIT,
	BMP24BIT,
};
class aimooeOperator : public QThread
{
	Q_OBJECT
private:
	explicit aimooeOperator(QObject* parent = nullptr);
	aimooeOperator(const aimooeOperator&) = delete;
	void write_bmpheader(unsigned char* bitmap, int offset, int bytes, int value);
	unsigned char* convertGrayTo24bitBmp(unsigned char* inputImg, int width, int height, int* ouputSize);
	unsigned char* convertColor16To24bitBmp(unsigned char* inputImg, int width, int height, int* ouputSize);
	bool saveToBmp(unsigned char* inputImg, int width, int height, char* outputFileName, E_BMPBIT bmpBit);
	void Ipl2VTK(cv::Mat& src, vtkImageData* dst);


	int useUSBorEthernetorWIFI = 0;

	int cntC = 0;
	mutex mC;
	int cntD = 0;
	mutex mD;
	int cntI = 0;
	mutex mI;
	E_Interface EI;
	E_Interface oldEI;
	int dataNum;
	E_DataType EDT;
	bool inittoolist = false;
	T_AimPosStatusInfo statusSt;
	T_MarkerInfo markerSt;
	T_ManufactureInfo manufactureInfo;
	AimHandle aimHandle = NULL;
	int rlt;
	const char *patht = "M:\\ARSystem\\Aimooe\\Pelvis\\System\\AimTools\\";
	std::vector<std::string>toolidarr;
	
	//double RT[7];
	
public:
	bool isStarted = false;

	static aimooeOperator& GetInstance()
	{
		static aimooeOperator aOperator;
		return aOperator;
	}
	void captureGrayFrame();
	void captureColorFrame();
	void GetToolInfo(std::vector<toolPosition> &toolList);
	void GetMarkerInfo();
	
	
	char* iL = NULL;
	char* iR = NULL;
	char* iC = NULL;
	T_AIMPOS_DATAPARA mPosDataPara;
	

	
signals:
	void ImageLeft(char*)  ;
	void ImageRight(char*) ;
	void ImageColor(char*) ;
	void RTSignal(QString);
	
	void flage(bool);
public slots:
	void loopGetTool();
	void openAimooe();
};