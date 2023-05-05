#include<Qt3DRender/QTexture>
#include "subwindow.h"
#include "ui_subwindow.h"

#include <QMessageBox>

bool isInit = false;
float RA = 0;
MainWindow::MainWindow(QWidget* parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	isCamera = false;
	isModel = false;
	isvGuide = false;
	isCollection = false;

	connect(ui->actionRegistration, SIGNAL(triggered()), this, SLOT(Regist()));
	connect(ui->actionLoad_Model, SIGNAL(triggered()), this, SLOT(onLoadModel()));
	aimooeThread = new QThread();
	aimooeOperator::GetInstance().moveToThread(aimooeThread);
	connect(ui->actionOpen_NDI, SIGNAL(triggered()), this, SLOT(openNDI()));
	connect(aimooeThread, SIGNAL(started()), &aimooeOperator::GetInstance(), SLOT(openAimooe()));
	connect(ui->actionOpen_Camera, SIGNAL(triggered()), this, SLOT(on_btnCameraCtrl_clicked()));
	connect(ui->actionLoad_vGuide, SIGNAL(triggered()), this, SLOT(vGuide()));
	connect(ui->btnLocation, SIGNAL(triggered()), this, SLOT(on_btnLocation_clicked()));
	connect(ui->btnProjectCtrl, SIGNAL(triggered()), this, SLOT(on_btnProjectCtrl_clicked()));
	connect(&aimooeOperator::GetInstance(), SIGNAL(flage(bool)), this, SLOT(Sampling(bool)));

	//thread = new QThread;
	//VideoFlow::GetInstance().moveToThread(thread);
	
	
	//t_Msg = &MyThread::GetInstance();
	//thread = new QThread;
	//connect(thread, SIGNAL(started()), t_Msg, SLOT(runThread()));
	//connect(t_Msg, SIGNAL(MySignal(QString)), this, SLOT(HandleSignal(QString)));
	//t_Msg->moveToThread(thread);


	

	/*

		backgroundActor = vtkImageActor::New();
		backgroundActor->SetScale(1000);
		backgroundRenderer = vtkSmartPointer<vtkRenderer>::New();*/
}

MainWindow::~MainWindow()
{
	delete ui;
}
void MainWindow::HandleSignal(QString msg)
{
	

}

void MainWindow::Sampling(bool flage) {
	
}
void MainWindow::Regist() {
	//在loadmode中获取了术前模型
}
void MainWindow::DisplayVTKImage(cv::Mat& mat)
{

}
void MainWindow::on_btnProjectCtrl_clicked() {
	if ((ui->btnProjectCtrl->text() == "相机模式")) {
		VtkVisual::GetInstance().SwitchTrackbll("a");
		ui->btnProjectCtrl->setText("模型模式");
	}

	else {
		VtkVisual::GetInstance().SwitchTrackbll("c");
		ui->btnProjectCtrl->setText("相机模式");
	}


}

void MainWindow::on_btnCameraCtrl_clicked()//摄像头开关控制
{
	if (VideoFlow::GetInstance().isStarted) {
		vtkAR = std::make_shared<CVTKAR>(ui->qvtkWidget);
		ARVisual::GetInstance().Start(*vtkAR.get());
		startTimer(0);
		isCamera = true;

	}
	else if (ui->actionOpen_Camera->isChecked())//未开启状态
	{
		VideoFlow::GetInstance().Start();
		//capture.open(0);
		if (VideoFlow::GetInstance().isStarted) {
			vtkAR = std::make_shared<CVTKAR>(ui->qvtkWidget);
			ARVisual::GetInstance().Start(*vtkAR.get());
			startTimer(0);
			isCamera = true;


		}
		else {
			qDebug() << "error";
			isCamera = false;
		}
	}
	else
	{

		killTimer(0);

		isCamera = false;
	}
}

void MainWindow::paintEvent(QPaintEvent* e)
{

}



void MainWindow::onLoadModel()
{
	//qDebug() << "onBrowseBtnClicked";
	QString directory = QFileDialog::getOpenFileName(this, tr("Open file"), "", nullptr);
	QFileInfo  fileInfo(directory);
	if (directory.isEmpty() || !fileInfo.exists()) {
		return;
	}
	qDebug() << directory;
	if (directory.isEmpty() || !fileInfo.exists()) {
		return;
	}
	vtkDisplay = std::make_shared<CVTKDisplay>(ui->qvtkWidget_2);
	vtkDisplay2 = std::make_shared<CVTKDisplay>(ui->qvtkWidget_3);
	VtkVisual::GetInstance().Load(directory.toStdString());
	VtkVisual::GetInstance().Start(*vtkDisplay.get(), *vtkDisplay2.get());
	
	qDebug() << "load ready";
	isModel = true;

}

void MainWindow::vGuide()
{
	qDebug() << "onBrowseBtnClicked";
	QString directory = QFileDialog::getOpenFileName(this, tr("Open file"), "", nullptr);
	QFileInfo  fileInfo(directory);
	if (directory.isEmpty() || !fileInfo.exists()) {
		return;
	}
	qDebug() << directory;
	if (directory.isEmpty() || !fileInfo.exists()) {
		return;
	}



	isvGuide = true;
}



QImage MainWindow::cvMat2QImage(const cv::Mat& mat)    // Mat 改成 QImage
{
	if (mat.type() == CV_8UC1)                  // 单通道
	{
		QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);
		image.setColorCount(256);               // 灰度级数256
		for (int i = 0; i < 256; i++)
		{
			image.setColor(i, qRgb(i, i, i));
		}
		uchar* pSrc = mat.data;                 // 复制mat数据
		for (int row = 0; row < mat.rows; row++)
		{
			uchar* pDest = image.scanLine(row);
			memcpy(pDest, pSrc, mat.cols);
			pSrc += mat.step;
		}
		return image;
	}

	else if (mat.type() == CV_8UC3)             // 3通道
	{
		const uchar* pSrc = (const uchar*)mat.data;         // 复制像素
		QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);    // R, G, B 对应 0,1,2
		return image.rgbSwapped();              // rgbSwapped是为了显示效果色彩好一些。
	}
	else if (mat.type() == CV_8UC4)
	{
		const uchar* pSrc = (const uchar*)mat.data;         // 复制像素
		// Create QImage with same dimensions as input Mat
		QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);        // B,G,R,A 对应 0,1,2,3
		return image.copy();
	}
	else
	{
		return QImage();
	}
}
cv::Mat MainWindow::QImage2cvMat(QImage image)
{
	cv::Mat mat;
	switch (image.format())
	{
	case QImage::Format_ARGB32:
	case QImage::Format_RGB32:
	case QImage::Format_ARGB32_Premultiplied:
		mat = cv::Mat(image.height(), image.width(), CV_8UC4, (void*)image.bits(), image.bytesPerLine());
		break;
	case QImage::Format_RGB888:
		mat = cv::Mat(image.height(), image.width(), CV_8UC3, (void*)image.bits(), image.bytesPerLine());
		cv::cvtColor(mat, mat, cv::COLOR_BGR2GRAY);
		break;
	case QImage::Format_Indexed8:
		mat = cv::Mat(image.height(), image.width(), CV_8UC1, (void*)image.bits(), image.bytesPerLine());
		break;
	}
	return mat;
}
void MainWindow::timerEvent(QTimerEvent* event)
{
	QApplication::processEvents();

	if (isCamera) {
		VideoFlow::GetInstance().GetImage(orFrame);
		VideoFlow::GetInstance().interlaceImage(orFrame,frame);
		QImage image = cvMat2QImage(frame);
		ui->Camera->setPixmap(QPixmap::fromImage(image));  // 将图片显示到label上
		
	}

}

void MainWindow::openNDI()
{
	aimooeThread->start();
}
void MainWindow::mousePressEvent(QMouseEvent* e)
{
	//获取点击的下标
	qDebug() << e->x() << ":" << e->y();
	if (e->button() == Qt::LeftButton)
	{
		qDebug() << "左键";
	}
	else if (e->button() == Qt::RightButton)
	{
		qDebug() << "右键";
	}
}
//鼠标 移动
void MainWindow::mouseMoveEvent(QMouseEvent* e)
{
	qDebug() << "move" << e->x() << ":" << e->y();
}
//鼠标 松开
void MainWindow::mouseReleaseEvent(QMouseEvent* e)
{
	qDebug() << "Release" << e->x() << ":" << e->y();
}
void MainWindow::keyPressEvent(QKeyEvent* event)
{
	cout << event->key();
	cv::Mat R;
	switch (event->key())
	{
	case Qt::Key_Escape:
		qDebug() << "ESC";
		break;
	case Qt::Key_F1:
		
		break;
	case Qt::Key_F2:
	{

		break;
	}
	case Qt::Key_F3: {

		//cameraForm * w = new cameraForm();
		//w->show();
	}
	case Qt::Key_5:
		qDebug() << "5";
		break;
	case Qt::Key_Back:
		qDebug() << "Back";
		break;
	case Qt::Key_Enter:
		qDebug() << "Enter";
		break;
	case Qt::Key_Left:


		break;
	case Qt::Key_Down:
	

		break;
	case Qt::Key_Up:
	

		break;
	case Qt::Key_Right:
		
		break;
	case Qt::Key_Q:
		
		break;
	case Qt::Key_E:
		
		break;
	case Qt::Key_P:
		
		break;
	case Qt::Key_L:
		
		break;
	case Qt::Key_O:
		
		break;
	case Qt::Key_K:
		
		break;
	case Qt::Key_I:
		
		break;
	case Qt::Key_J:
		
		break;
	default:
		qDebug() << event->key();
		break;
	}
}


void MainWindow::on_btnLocation_clicked()
{
	if (aimooeOperator::GetInstance().isStarted&&VideoFlow::GetInstance().isStarted) {
		VtkVisual::GetInstance().DoTransform();//根据定位器校准模型位置
	}
	
}
