

#include <vtkArrowSource.h>
#include <vtkNamedColors.h>
#include <vtkQuaternion.h>
#include <vtkTextActor.h>
#include <vtkTextProperty.h>
#include <vtkPointData.h>

#include "VTKFile/VTKAR.h"
#include "VTKFile/ArrowActor.h"


#include "ARVisual.h"

#include <vtkAutoInit.h>
//VTK_MODULE_INIT(vtkRenderingOpenGL2);
//VTK_MODULE_INIT(vtkInteractionStyle);
//VTK_MODULE_INIT(vtkRenderingFreeType);

#define SCALE_FACTOR 40


class vtkTimerCallback : public vtkCommand {
public:
	vtkTimerCallback() = default;

	static vtkTimerCallback* New() {
		vtkTimerCallback* cb = new vtkTimerCallback;

		cb->TimerCount = 0;
		return cb;
	}

	virtual void Execute(vtkObject* vtkNotUsed(caller), unsigned long eventId, void* vtkNotUsed(callData));

public:
	ARVisual* parent;

private:
	int                   TimerCount;
	cv::Mat frame;
	//BufferedFrame_t frame;
};

void vtkTimerCallback::Execute(vtkObject* vtkNotUsed(caller), unsigned long eventId, void* vtkNotUsed(callData)) {
	char buff[256];
	if (vtkCommand::TimerEvent == eventId) {

		/*   ++this->TimerCount;*/
		if (VideoFlow::GetInstance().GetImage(frame)) {
			if (VideoFlow::GetInstance().interlaceImage(frame, parent->frame)) {
				parent->m_Parent->Display(parent->frame);
				parent->m_Parent->scene_Renderer->SetActiveCamera(
					VtkVisual::GetInstance().m_fp);
				parent->m_StlModel.GetTransform()->SetInput(
					VtkVisual::GetInstance().m_StlModel.GetTransform());
			}
			
		}
		

	}

	parent->m_Parent->UpdateView();
}

void ARVisual::Load() {
	m_Parent->scene_Renderer->AddActor(VtkVisual::GetInstance().m_StlModel.GetActor());

	m_StlModel.GetTransform()->SetInput(VtkVisual::GetInstance().m_StlModel.GetTransform());
	
	//int res = ercp::RoughRegistration::GetInstance().LoadReferenceFromPCD(".\\visual\\points.pcd");
	//assert(res >= 0);
}
//void CallbackFunction(vtkObject* caller, long unsigned int vtkNotUsed(eventId),
//	void* vtkNotUsed(clientData), void* vtkNotUsed(callData))
//{
//	vtkRenderer* renderer = static_cast<vtkRenderer*>(caller);
//
//	double timeInSeconds = renderer->GetLastRenderTimeInSeconds();
//	double fps = 1.0 / timeInSeconds;
//	std::cout << "FPS: " << fps << std::endl;
//
//	std::cout << "Callback" << std::endl;
//}

void ARVisual::Start(CVTKAR& vtkwin) {

	m_Parent = &vtkwin;
	Load();

	m_Parent->FirstPerspective = FirstPerspective;
	//



	//vtkNew<vtkAxesActor> axes;
	//axes->SetPosition(0, 0, 0);
	//axes->SetTotalLength(SCALE_FACTOR, SCALE_FACTOR, SCALE_FACTOR);
	//axes->SetShaftType(0);
	//axes->SetAxisLabels(0);
	//axes->SetCylinderRadius(0.02);
	//m_Parent->scene_Renderer->AddActor(axes);
	if (VtkVisual::GetInstance().isStarted) {
		m_Parent->scene_Renderer->AddActor(VtkVisual::GetInstance().m_StlModel.GetActor());
		m_Parent->scene_Renderer->GetActiveCamera()->ParallelProjectionOn();

		m_StlModel.GetTransform()->SetInput(
			VtkVisual::GetInstance().m_StlModel.GetTransform());
	}
	cv::Mat orFrame;
	VideoFlow::GetInstance().GetImage(orFrame);
	VideoFlow::GetInstance().interlaceImage(orFrame,frame);
	m_Parent->Ipl2VTK(frame, m_Parent->im);

	double origin[3] = { 0 };
	double spacing[3] = { 0 };
	int    extent[6] = { 0 };
	m_Parent->im->GetOrigin(origin);
	m_Parent->im->GetSpacing(spacing);
	m_Parent->im->GetExtent(extent);
	m_Parent->bg_renderer->GetActiveCamera()->ParallelProjectionOn();
	m_Parent->bg_renderer->InteractiveOff();
	m_Parent->Display(frame);

	double xc = origin[0] + 0.5 * (extent[0] + extent[1]) * spacing[0];
	double yc = origin[1] + 0.5 * (extent[2] + extent[3]) * spacing[1];
	double yd = (extent[3] - extent[2] + 1) * spacing[0];
	double d = m_Parent->bg_renderer->GetActiveCamera()->GetDistance();
	m_Parent->bg_renderer->GetActiveCamera()->SetParallelScale(0.5 * yd);
	m_Parent->bg_renderer->GetActiveCamera()->SetFocalPoint(xc, yc, 0.0);
	m_Parent->bg_renderer->GetActiveCamera()->SetPosition(xc, yc, d);
	m_Parent->bg_renderer->GetActiveCamera()->SetViewUp(0, -1, 0);

	vtkSmartPointer<vtkTimerCallback> callback = vtkTimerCallback::New();
	m_Parent->m_vtkItr->CreateRepeatingTimer(30);
	callback->parent = this;
	m_Parent->m_vtkItr->AddObserver(vtkCommand::TimerEvent, callback);


	m_Parent->m_vtkWin->GetRenderWindow()->Render();
}

// namespace ercp
