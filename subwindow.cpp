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
	//��loadmode�л�ȡ����ǰģ��
}
void MainWindow::DisplayVTKImage(cv::Mat& mat)
{

}
void MainWindow::on_btnProjectCtrl_clicked() {
	if ((ui->btnProjectCtrl->text() == "���ģʽ")) {
		VtkVisual::GetInstance().SwitchTrackbll("a");
		ui->btnProjectCtrl->setText("ģ��ģʽ");
	}

	else {
		VtkVisual::GetInstance().SwitchTrackbll("c");
		ui->btnProjectCtrl->setText("���ģʽ");
	}


}

void MainWindow::on_btnCameraCtrl_clicked()//����ͷ���ؿ���
{
	if (VideoFlow::GetInstance().isStarted) {
		vtkAR = std::make_shared<CVTKAR>(ui->qvtkWidget);
		ARVisual::GetInstance().Start(*vtkAR.get());
		startTimer(0);
		isCamera = true;

	}
	else if (ui->actionOpen_Camera->isChecked())//δ����״̬
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



QImage MainWindow::cvMat2QImage(const cv::Mat& mat)    // Mat �ĳ� QImage
{
	if (mat.type() == CV_8UC1)                  // ��ͨ��
	{
		QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);
		image.setColorCount(256);               // �Ҷȼ���256
		for (int i = 0; i < 256; i++)
		{
			image.setColor(i, qRgb(i, i, i));
		}
		uchar* pSrc = mat.data;                 // ����mat����
		for (int row = 0; row < mat.rows; row++)
		{
			uchar* pDest = image.scanLine(row);
			memcpy(pDest, pSrc, mat.cols);
			pSrc += mat.step;
		}
		return image;
	}

	else if (mat.type() == CV_8UC3)             // 3ͨ��
	{
		const uchar* pSrc = (const uchar*)mat.data;         // ��������
		QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);    // R, G, B ��Ӧ 0,1,2
		return image.rgbSwapped();              // rgbSwapped��Ϊ����ʾЧ��ɫ�ʺ�һЩ��
	}
	else if (mat.type() == CV_8UC4)
	{
		const uchar* pSrc = (const uchar*)mat.data;         // ��������
		// Create QImage with same dimensions as input Mat
		QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);        // B,G,R,A ��Ӧ 0,1,2,3
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
		ui->Camera->setPixmap(QPixmap::fromImage(image));  // ��ͼƬ��ʾ��label��
		
	}

}

void MainWindow::openNDI()
{
	aimooeThread->start();
}
void MainWindow::mousePressEvent(QMouseEvent* e)
{
	//��ȡ������±�
	qDebug() << e->x() << ":" << e->y();
	if (e->button() == Qt::LeftButton)
	{
		qDebug() << "���";
	}
	else if (e->button() == Qt::RightButton)
	{
		qDebug() << "�Ҽ�";
	}
}
//��� �ƶ�
void MainWindow::mouseMoveEvent(QMouseEvent* e)
{
	qDebug() << "move" << e->x() << ":" << e->y();
}
//��� �ɿ�
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
		VtkVisual::GetInstance().DoTransform();//���ݶ�λ��У׼ģ��λ��
	}
	
}
