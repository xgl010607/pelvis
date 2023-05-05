#pragma once
#ifndef SUBWINDOW_H
#define SUBWINDOW_H

#include <QWidget>

#include <QFileDialog>
#include <QOpenGLFunctions>
#include <QOpenGLWidget>


#include <QTime>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QKeyEvent>
#include "ui_subwindow.h"

#include <QtOpenGL/QtOpenGL>
#include <opencv2/core/types_c.h>
#include "ARVisual.h"
#include "VtkVisual.h"
#include "VTKFile/VTKDisplay.h"
#include "VTKFile/VTKAR.h"
#include "VideoFlow.h"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
//#include "Registration.h"
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
#include <QThread>
namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = 0);
    ~MainWindow();
    QVBoxLayout* verticalLayout;
    QTimer clk;
    float m_x, m_y, m_z;
    GLuint textur;
    cv::Mat frame;
    cv::Mat orFrame;


protected:
    void initializeGL();
    void initWidget();
    void paintGL();
    void resizeGL(int width, int height);
    void loadGLTextures();
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void keyPressEvent(QKeyEvent* event);
    cv::Mat QImage2cvMat(QImage image);
    QThread *aimooeThread;

private slots:
    void on_btnCameraCtrl_clicked();
    void paintEvent(QPaintEvent* e);
    void onLoadModel();
    void timerEvent(QTimerEvent* event);
    void openNDI();
    void vGuide();
    //void updateWindow();
    void HandleSignal(QString);
    void on_btnLocation_clicked();
    void Sampling(bool flage);
    void Regist();
    void DisplayVTKImage(cv::Mat& mat);
    void on_btnProjectCtrl_clicked();
    
private:
    Ui::MainWindow* ui;
    // QCamera* m_camera;//…„œÒÕ∑…Ë±∏
    bool isCamera;
    bool isModel;
    bool isvGuide;
    bool isCollection;
    QWidget* mContainer;
    QWidget* vContainer;
    //Registration *R;
   
    QImage cvMat2QImage(const cv::Mat& mat);
   /* ModelOperator* vModel;
    ModelOperator* mModel;*/
    cv::Mat Model2vGuide;
    cv::Mat Virtrul2Reality;
    std::vector<cv::Point3i> labelPoint;
    std::vector<cv::Point3i> ModelPoint;
    QThread* thread;
    std::ofstream ofile;
    cv::VideoWriter outputVideo;
    float* Msg;
    char timePicDir[40];
    std::vector<cv::Point3f> pts_3d;
    std::vector<cv::Point3f> Showpts_3d;
    float x = -0.370740907093656; float y = -0.2397403632299851; float z = -0.0407927826288379;
    float alpha = 0.1;
    float beta = 0.1;
    float theta = 0.1;
    QStringList Positionlist;
    QVector3D InitPosition;
    QVector3D InitAngle;
    cv::Point3f Modelorg;
    vtkSmartPointer<vtkActor> objActor;
    double* origin;
    vtkImageActor* backgroundActor;
    vtkSmartPointer<vtkRenderer> sceneRenderer;
    vtkSmartPointer<vtkActor> pointActor;
    vtkSmartPointer<vtkRenderer> backgroundRenderer;
    int countFrame = 0;
    std::shared_ptr<CVTKAR> vtkAR = nullptr;
    std::shared_ptr<CVTKDisplay> vtkDisplay = nullptr;
    std::shared_ptr<CVTKDisplay> vtkDisplay2 = nullptr;
    //    Qt3DCore::QEntity *lightEntity;

    //    QString directory;


};

#endif // subWindow_H
