#pragma once

#include <vtkOpenGLRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkCallbackCommand.h>
#include <vtkCellPicker.h>
#include <vtkOrientationMarkerWidget.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkAxesActor.h>
#include <vtkCameraActor.h>
#include <vtkBoxWidget.h>
#include <vtkSmartPointer.h>
#include <vtkColor.h>
#include <vtkLookupTable.h>
#include <vtkProperty.h>
#include <vtkPointData.h>
#include <vtkFloatArray.h>
#include <vtkImageActor.h>
#include <vtkImageImport.h>
#include <vtkImageData.h>
#include <vtkNamedColors.h>

#include <opencv2/opencv.hpp>

#include "Actor.hpp"
#include "STLReader.h"
#include <vtkTransformFilter.h>
#include <vtkQuaternion.h>
#include <QVTKWidget.h>
#include "AssemblyAcotr.h"
#include "PointsActor.h"

class CVTKAR : public CActor<vtkTransformFilter> {
public:
    CVTKAR(QVTKWidget *pView);
    ~CVTKAR() = default;

public:
    void        Display(cv::Mat frame);
    void        SetImage(vtkImageData *dst);
    void        SetVirtualModel();
    void        SetvirtualModelPostion(double x, double y, double z);
    void        SetvirtualModelPostion(double x, double y, double z, double a, double b, double t);
    void        SetvirtualModelPostion(double matrix[16]);
    void        SetvirtualModelPostion(vtkMatrix4x4 *matrix);
    static void Ipl2VTK(cv::Mat &src, vtkImageData *dst);
    void        UpdateView();

public:
    vtkSmartPointer<vtkRenderer>                scene_Renderer;
    vtkSmartPointer<vtkRenderer>                bg_renderer;
	QVTKWidget *m_vtkWin;
    vtkSmartPointer<vtkRenderWindowInteractor>  m_vtkItr;
    vtkSmartPointer<vtkOrientationMarkerWidget> m_Mark;
    vtkSmartPointer<vtkActor>                   m_Actor;
    vtkSmartPointer<vtkImageActor>              im_Actor;
    vtkSmartPointer<vtkImageData>               im;
    vtkSmartPointer<vtkTransform>               m_scale;
    CSTLReader *                                reader;
    //cv::Mat                                     frame;

    //vtkSmartPointer<vtkCamera> m_camera;
    bool                       FirstPerspective;

    // vtkSmartPointer<vtkSTLReader> source;
private:
};
