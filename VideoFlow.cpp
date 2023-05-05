#include "VideoFlow.h"
void VideoFlow::runThread() {
	cap >> frame;
	if (!frame.empty()) {
		cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);
	}
}
void VideoFlow::Start() {
	frame =cv::Mat(cv::Size(1920,1080),0);
	std::string url = "rtsp://admin:123456@192.168.137.175:8554/live";
	cap.open(0, cv::CAP_DSHOW);
	cap.set(cv::CAP_PROP_FRAME_WIDTH, 1920);
	cap.set(cv::CAP_PROP_FRAME_HEIGHT, 1080);
	cap.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'));
	cap.set(cv::CAP_PROP_FPS, 30);//Ö¡Êý
	isStarted = 1;
}
bool VideoFlow::GetImage(cv::Mat& Image) {
	if (frame.data != NULL)
		frame.release();

	cap >> frame;
	if (!frame.empty()) {
		cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);
	}
	if (frame.data == NULL)
		return 0;
	Image = frame;
	return 1;
}
void VideoFlow::separetImage(cv::Mat& upImage, cv::Mat& downImage) {
	int height = this->frame.rows;
	int width = this->frame.cols;
	upImage = this->frame(cv::Rect(0, 0, this->frame.size().width, this->frame.size().height / 2));
	downImage = this->frame(cv::Rect(0, this->frame.size().height / 2, this->frame.size().width, this->frame.size().height / 2));
	upImage.copyTo(this->upImage);
	downImage.copyTo(this->downImage);
}
void VideoFlow::separetImage(cv::Mat &Image, cv::Mat& upImage, cv::Mat& downImage) {
	int height = Image.rows;
	int width = Image.cols;
	upImage = Image(cv::Rect(0, 0, Image.size().width, Image.size().height / 2));
	downImage = Image(cv::Rect(0, Image.size().height / 2, Image.size().width, Image.size().height / 2));
	upImage.copyTo(this->upImage);
	downImage.copyTo(this->downImage);
}
bool VideoFlow::interlaceImage(cv::Mat& overlapImage) {
	overlapImage = cv::Mat(this->frame);
	//upImage = frame(cv::Rect(0, 0, frame.size().width, frame.size().height / 2));
	//downImage = frame(cv::Rect(0, frame.size().height / 2, frame.size().width, frame.size().height / 2));

	int Rows = upImage.cols * upImage.rows;
	int Cols = 1;

	cv::Mat TmpU(Rows, Cols, upImage.type(), upImage.data);
	cv::Mat TmpD(Rows, Cols, upImage.type(), downImage.data);
	for (int i = 0; i < upImage.rows - 2; i = i + 2) {
		overlapImage.rowRange(i, i + 1) = TmpU.rowRange(i * this->frame.cols, (i + 1) * this->frame.cols + 1).t();
		overlapImage.rowRange(i + 1, i + 2) = TmpD.rowRange(i * this->frame.cols, (i + 1) * this->frame.cols + 1).t();
	}
	
	return true;
}
bool VideoFlow::interlaceImage(cv::Mat& Image, cv::Mat& overlapImage) {
	overlapImage = cv::Mat(Image);
	//upImage = frame(cv::Rect(0, 0, frame.size().width, frame.size().height / 2));
	//downImage = frame(cv::Rect(0, frame.size().height / 2, frame.size().width, frame.size().height / 2));

	int Rows = upImage.cols * upImage.rows;
	int Cols = 1;

	cv::Mat TmpU(Rows, Cols, upImage.type(), upImage.data);
	cv::Mat TmpD(Rows, Cols, upImage.type(), downImage.data);
	for (int i = 0; i < upImage.rows - 2; i = i + 2) {
		overlapImage.rowRange(i, i + 1) = TmpU.rowRange(i * Image.cols, (i + 1) * Image.cols + 1).t();
		overlapImage.rowRange(i + 1, i + 2) = TmpD.rowRange(i * Image.cols, (i + 1) * Image.cols + 1).t();
	}

	return true;
}
void VideoFlow::End() {
	upImage.release();
	downImage.release();
	overlapImage.release();
	frame.release();
	cap.release();
}