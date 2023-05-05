
#include <vtkSmartPointer.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkInteractorStyleJoystickCamera.h>
#include <vtkNamedColors.h>
#include <vtkInformation.h>
#include <vtkPointData.h>
#include <vtkActor.h>
#include <vtkCellArray.h>
#include <vtkCubeSource.h>
#include <vtkGlyph3D.h>
#include <vtkNew.h>
#include <vtkPointData.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkDoubleArray.h>
#include <vtkProperty.h>
#include <vtkPointWidget.h>
#include <vtkAffineWidget.h>
#include <vtkProperty.h>
#include <vtkPolyDataMapper.h>

#include "VTKDisplay.h"

CVTKDisplay::CVTKDisplay(QVTKWidget *pView) {

    m_Render = vtkRenderer::New();
    m_vtkWin = pView;


    m_vtkWin->GetRenderWindow()->AddRenderer(m_Render);
    //m_vtkWin->Initialize();

    if ((m_vtkItr = m_vtkWin->GetInteractor()) == NULL) {
        m_vtkItr = vtkWin32RenderWindowInteractor::New();
        vtkSmartPointer<vtkInteractorStyleJoystickCamera> style =
            vtkSmartPointer<vtkInteractorStyleJoystickCamera>::New();
        m_vtkItr->SetInteractorStyle(style);
        m_vtkItr->SetRenderWindow(m_vtkWin->GetRenderWindow());
        m_vtkItr->Initialize();
    }

    // 设置渲染器和相机
    m_Render->TwoSidedLightingOn();
    {
        vtkNew<vtkNamedColors> colors;
        m_Render->SetBackground(colors->GetColor3d("SlateGray").GetData());
    }
    m_Render->ResetCamera();
    m_Render->GetActiveCamera()->SetPosition(10, 10, 10);
    m_Render->GetActiveCamera()->SetFocalPoint(0,0,0);
    //m_Render->GetActiveCamera()->SetViewUp(0, 0, 0.1);
    m_Render->GetActiveCamera()->SetViewUp(0, 1, 0);
    m_Render->GetActiveCamera()->SetViewAngle(60);
    m_Render->GetActiveCamera()->ParallelProjectionOff();
	

    // 小窗口坐标
    m_Mark = vtkSmartPointer<vtkOrientationMarkerWidget>::New();
    vtkNew<vtkAxesActor> base;
    m_Mark->SetOutlineColor(0.9300, 0.5700, 0.1300);
    m_Mark->SetOrientationMarker(base);
    m_Mark->SetInteractor(m_vtkItr);
    m_Mark->SetEnabled(1);
    m_Mark->InteractiveOff();

    vtkNew<vtkCallbackCommand> m_callback;
    m_callback->SetCallback(CVTKDisplay::MyExecute);
    m_callback->SetClientData(this);

    // 动态拾取点
    m_PickedPoints.SetSphere(0.1);
    m_PickedPoints.SetColor(0.0, 1.0, 0.0);
    m_Render->AddActor(m_PickedPoints.GetActor());

    m_picker = vtkCellPicker::New();
    m_picker->SetTolerance(0.0001);
    m_picker->AddObserver(vtkCommand::EndPickEvent, m_callback);
    m_picker->InitializePickList();
    m_picker->AddPickList(m_PickedPoints.GetActor());
    m_picker->PickFromListOn();
    m_vtkItr->SetPicker(m_picker);

    
    m_Render->AddActor(m_LineActor.GetActor());


    

 }

void CVTKDisplay::SetBKColor(double r /* =0.7 */, double g /* =0.7 */, double b /* =0.9 */) {
    m_Render->SetBackground(r, g, b);
}

int CVTKDisplay::AddActor(vtkProp *actor) const {
    m_Render->AddActor(actor);
    //m_Render->ResetCamera();
    return m_Render->GetActors()->GetNumberOfItems() - 1;
}

void CVTKDisplay::DeleteActor(vtkProp *actor) const {
    m_Render->RemoveActor(actor);
    m_Render->ResetCamera();
}

void CVTKDisplay::Show() {
    UpdateView();
}

void CVTKDisplay::UpdateView() {
    m_vtkWin->GetRenderWindow()->Render();
}

vtkActor *CVTKDisplay::GetActor(int actorIndex) const {
    vtkActorCollection *collection = m_Render->GetActors();
    if (collection) {
        int i = 0;
        while (i < actorIndex) {
            collection->GetNextActor();
        }
        return collection->GetNextActor();
    } else {
        return NULL;
    }
}

vtkActorCollection *CVTKDisplay::GetAllActor() {
    return m_Render->GetActors();
}

void CVTKDisplay::MyExecute(vtkObject *caller, unsigned long eid, void *clientdata, void *calldata) {

    CVTKDisplay *m_viewer = reinterpret_cast<CVTKDisplay *>(clientdata);
    if (eid == vtkCommand::EndPickEvent) {
        double m_pickPoint[3];
        m_viewer->m_picker->GetPickPosition(m_pickPoint);
        vtkActor *actor = m_viewer->m_picker->GetActor();
        vtkVector3d pickpoint(m_pickPoint);
        if (m_viewer->OnPick != nullptr) { m_viewer->OnPick(pickpoint, actor); }
    }
}
