#pragma once
#include <vtkWin32OpenGLRenderWindow.h>
#include <vtkWin32RenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkCallbackCommand.h>
#include <vtkCellPicker.h>
#include <vtkOrientationMarkerWidget.h>
#include <vtkAxesActor.h>
#include <vtkCameraActor.h>
#include <vtkTextActor.h>
#include <vtkTextProperty.h>
#include <vtkSmartPointer.h>
#include <vtkBoxWidget.h>
#include <vtkSmartPointer.h>
#include <vtkColor.h>

#include <functional>
#include <QVTKWidget.h>
#include <QWidget>
#include "ArcActor.h"
#include "STLReader.h"
#include "AssemblyAcotr.h"
#include "PointsActor.h"
#include "LineActor.h"
#include <vtkLine.h>

class CVTKDisplay {
public:
    CVTKDisplay(QVTKWidget *pView);
    ~CVTKDisplay() = default;

public:
    void                SetBKColor(double r = 0, double g = 0, double b = 0);  //
    int                 AddActor(vtkProp *actor) const;
    void                DeleteActor(vtkProp *actor) const;
    void                Show();
    void                UpdateView();
    vtkActor *          GetActor(int actorIndex = 0) const;
    vtkActorCollection *GetAllActor();
    static void         MyExecute(vtkObject *caller, unsigned long eid, void *clientdata, void *calldata);

public:
    vtkSmartPointer<vtkRenderer>                m_Render;
    QVTKWidget* m_vtkWin;
    vtkSmartPointer<vtkRenderWindowInteractor>  m_vtkItr;
    vtkSmartPointer<vtkOrientationMarkerWidget> m_Mark;
    vtkSmartPointer<vtkCellPicker>              m_picker;


    std::function<void(vtkVector3d, vtkActor *)> OnPick = nullptr;
    bool FirstPerspective;
    CLineActor m_LineActor;
    CPointsActor m_PickedPoints;
};
