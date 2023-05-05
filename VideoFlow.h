#pragma once
#include <opencv2\opencv.hpp>
#include <opencv2/imgproc/types_c.h>
#include <QThread>
#include <QTextStream>
#include <QFile>


//#include "dependentFile\Delegate/DelegateLib.h"
//#include "dependentFile\Delegate/ThreadWin.h"
//判断初始化硬件是否连接
class VideoFlow:public QThread
{
public:
	static VideoFlow &GetInstance() {
		static VideoFlow _video;
		return _video;
	}
	void Start();
	void End();
	bool isStarted=false;
	bool GetImage(cv::Mat &frame);	
	void separetImage(cv::Mat &upImage,cv::Mat &downImage);
	void separetImage(cv::Mat &Image, cv::Mat& upImage, cv::Mat& downImage);

	bool interlaceImage(cv::Mat &overlapImage);
	bool interlaceImage(cv::Mat& Image, cv::Mat& overlapImage);
	cv::Mat frame;

public slots:
	void runThread();
signals:
	
private:
	cv::VideoCapture cap;
	cv::Mat upImage;
	cv::Mat downImage;
	cv::Mat overlapImage;
	VideoFlow() : QThread() {
	}
	VideoFlow(const VideoFlow &) = delete;
	~VideoFlow() {
		frame.release();
		cap.release();
	}
};


