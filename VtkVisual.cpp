
#include <vtkArrowSource.h>
#include <vtkNamedColors.h>
#include <vtkQuaternion.h>
#include <vtkTextActor.h>
#include <vtkTextProperty.h>
#include <vtkPointData.h>
#include <vtkTransform.h>
#include "VTKFile/VTKDisplay.h"
#include "VTKFile/ArrowActor.h"

#include "VtkVisual.h"
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkInteractorStyleJoystickCamera.h>

//#include "VTKFile\vtkInteraction.h"

#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkInteractionStyle);
VTK_MODULE_INIT(vtkRenderingFreeType);

#define SCALE_FACTOR 40
void mulitiPoint(vtkMatrix4x4 *m, double p[3],double pp[3] ) {
	pp[0] = m->Element[0][0] * p[0] + m->Element[0][1] * p[1] + m->Element[0][2] * p[2];
	pp[1] = m->Element[1][0] * p[0] + m->Element[1][1] * p[1] + m->Element[1][2] * p[2];
	pp[2] = m->Element[2][0] * p[0] + m->Element[2][1] * p[1] + m->Element[2][2] * p[2];
}
std::vector<vtkColor3d> mColors = {
	vtkColor3d(192 / 256.0, 57 / 256.0, 43 / 256.0),  vtkColor3d(39 / 256.0, 174 / 256.0, 96 / 256.0),
	vtkColor3d(211 / 256.0, 84 / 256.0, 0 / 256.0),   vtkColor3d(41 / 256.0, 128 / 256.0, 185 / 256.0),
	vtkColor3d(22 / 256.0, 160 / 256.0, 133 / 256.0), vtkColor3d(243 / 256.0, 156 / 256.0, 18 / 256.0),
	vtkColor3d(142 / 256.0, 68 / 256.0, 173 / 256.0), vtkColor3d(44 / 256.0, 62 / 256.0, 80 / 256.0) };

class VtkTimerCallback : public vtkCommand {
public:
	VtkTimerCallback() = default;

	static VtkTimerCallback *New() {
		VtkTimerCallback *cb = new VtkTimerCallback;
		return cb;
	}

	virtual void Execute(vtkObject *caller, unsigned long eventId, void *vtkNotUsed(callData));

public:
	VtkVisual *parent;
private:
	Vector3d lastP = {0,0,0};
	std::vector<toolPosition>toolList;
	vtkSmartPointer<vtkTransform> Conetrans1 = vtkSmartPointer<vtkTransform>::New();
	vtkSmartPointer<vtkTransform> Conetrans2 = vtkSmartPointer<vtkTransform>::New();

};

static void CvtQuatToMatrix(double qt[7], vtkMatrix4x4 *matrix) {

	Quaterniond quat(qt[3], qt[4], qt[5], qt[6]);
	Matrix3d    rot = quat.toRotationMatrix();
	Vector3d    trans(qt[0], qt[1], qt[2]);

	Matrix4d homo;
	homo.setIdentity();
	homo.block<3, 3>(0, 0) = rot;
	homo.block<3, 1>(0, 3) = trans;
	// Copy to vtkMatrix4x4
	Map<Matrix4d>(matrix->GetData(), homo.rows(), homo.cols()) = homo.transpose();
}

float lastRx = 0, lastRy = 0, lastRz = 0, lastx = 0, lasty = 0, lastz = 0;
float curRx = 0, curRy = 0, curRz = 0, curx = 0, cury = 0, curz = 0;
void VtkTimerCallback::Execute(vtkObject *caller, unsigned long eventId, void *vtkNotUsed(callData)) {
	double yaw = 0;
	double pitch = 0;
	double roll = 0;
	char   buff[256];
	//parent->m_StlModel.SetTransform(parent->m_StlModel.GetTransform()->GetMatrix());

	//Vector3d FocalPoint(0,0,parent->m_Second->m_Render->GetActiveCamera()->GetDistance());
	Vector4d FocalPoint(0,0,1, 0);
	if (eventId == vtkCommand::TimerEvent) {
		vtkSmartPointer<vtkMatrix4x4> matrixC = vtkSmartPointer<vtkMatrix4x4>::New();
		vtkSmartPointer<vtkMatrix4x4> Rx90 = vtkSmartPointer<vtkMatrix4x4>::New();
		Rx90->Identity();
		Rx90->Element[0][0] = 0;
		Rx90->Element[2][2] = 1;
		Rx90->Element[1][0] = 1;
		Rx90->Element[0][1] = -1;
		vtkSmartPointer<vtkMatrix4x4> matrixS = vtkSmartPointer<vtkMatrix4x4>::New();
		matrixC->Identity();
		matrixS->Identity();

		srand(unsigned(time(0)));   //获取系统时间 
		int number = rand() % 100;      //生成随机数 
		if (aimooeOperator::GetInstance().isStarted) {
			toolList.clear();
			aimooeOperator::GetInstance().GetToolInfo(toolList);//单机调试
			if (toolList.size() != 2)
				return;
			for (int o = 0; o < 2; o++) {
				std::string toolname = toolList.at(o).name;
				if (toolname == "pin") {
					for(int i=0;i<3;i++){
						matrixC->SetElement(i, 3, toolList.at(o).Tto[i]);
						for (int j = 0; j < 3; j++) 
							matrixC->SetElement(i, j, toolList.at(o).Rto[i][j]);
						}

					matrixC->Multiply4x4(matrixC, Rx90,  matrixC);
					Conetrans1->SetMatrix(matrixC);
					parent->cameraActor->SetUserTransform(Conetrans1);
					parent->cameraActor->Modified();

					parent->m_fp->SetPosition(Conetrans1->GetPosition()[0],
						Conetrans1->GetPosition()[1],
						Conetrans1->GetPosition()[2]);
				
					parent->m_fp->SetFocalPoint(
						matrixC->Element[0][3] + matrixC->Element[0][1],
						matrixC->Element[1][3] + matrixC->Element[1][1],
						matrixC->Element[2][3] + matrixC->Element[2][1]);
					parent->m_fp->SetViewUp(0, -1, 0);
					parent->m_fp = parent->m_Second->m_Render->GetActiveCamera();
					
				}
				else if (toolname == "base") {
					for (int i = 0; i < 3; i++) {
						matrixS->SetElement(i, 3, toolList.at(o).Tto[i]);
						for (int j = 0; j < 3; j++)
							matrixS->SetElement(i, j, toolList.at(o).Rto[i][j]);
					}
					Conetrans2->SetMatrix(matrixS);
					parent->m_StlModel.GetActor()->SetUserTransform(Conetrans2);
				}
				else
					break;
			}
			double orign[3] = { matrixC->Element[0][3] ,
					matrixC->Element[1][3] ,
					matrixC->Element[2][3] };
			double end[3] = { matrixC->Element[0][3] + matrixC->Element[0][1]*1000,
					matrixC->Element[1][3] + matrixC->Element[1][1]*1000,
					matrixC->Element[2][3] + matrixC->Element[2][1]*1000 };
			double orignT[3];
			mulitiPoint(matrixS, orign, orignT);
			double endT[3];
			mulitiPoint(matrixS, end,endT);
			parent->m_Parent->m_LineActor.CreateMultiLine(parent->m_StlModel,orignT,endT);
			vtkSmartPointer<vtkPoints> intersectPoints = vtkSmartPointer<vtkPoints>::New();
			intersectPoints = parent->m_Parent->m_LineActor.GetInsertPoint();
			int numP = intersectPoints->GetNumberOfPoints();
			if (numP == 2) {
				
				double point1[3];
				double point2[2];
				vtkDataArray* points;
				points = intersectPoints->GetData();//获得与模型焦点
				intersectPoints->GetPoint(0, point1);
				intersectPoints->GetPoint(1, point2);
				matrixS->Invert();
				mulitiPoint(matrixS, point1,point1);
				mulitiPoint(matrixS, point2, point2);

				vtkSmartPointer<vtkPoints> pts =
					vtkSmartPointer<vtkPoints>::New();//构造线段
				pts->InsertNextPoint(orign);
				pts->InsertNextPoint(point1);
				pts->InsertNextPoint(point2);
				pts->InsertNextPoint(end);
				parent->m_Parent->m_LineActor.SetPoints(pts);


				vtkSmartPointer<vtkLine> line0 =
					vtkSmartPointer<vtkLine>::New();
				line0->GetPointIds()->SetId(0, 0); // 第二个0表示pts中的origin点
				line0->GetPointIds()->SetId(1, 1); //第二个1表示pts中的p0点

				vtkSmartPointer<vtkLine> line1 =
					vtkSmartPointer<vtkLine>::New();
				line1->GetPointIds()->SetId(0, 1);
				line1->GetPointIds()->SetId(1, 2);

				vtkSmartPointer<vtkLine> line2 =
					vtkSmartPointer<vtkLine>::New();
				line1->GetPointIds()->SetId(0, 2);
				line1->GetPointIds()->SetId(1, 3);

				vtkSmartPointer<vtkCellArray> lines =
					vtkSmartPointer<vtkCellArray>::New();
				lines->InsertNextCell(line0);
				lines->InsertNextCell(line1);
				lines->InsertNextCell(line2);


				parent->m_Parent->m_LineActor.SetLines(lines);
				parent->m_Parent->m_LineActor.SetLineWidth(20);
				const double red[3] = { 255, 0, 0 };
				const double green[3] = { 0, 255, 0 };

				vtkSmartPointer<vtkUnsignedCharArray> colors =
					vtkSmartPointer<vtkUnsignedCharArray>::New();
				colors->SetNumberOfComponents(3);
				colors->InsertNextTuple(red);
				colors->InsertNextTuple(green);
				colors->InsertNextTuple(red);

				parent->m_Parent->m_LineActor.SetScalars(colors);
			}
			else {
				
				
				vtkSmartPointer<vtkPoints> pts =
					vtkSmartPointer<vtkPoints>::New();
			
				pts->InsertNextPoint(orign);
				pts->InsertNextPoint(end);
				parent->m_Parent->m_LineActor.SetPoints(pts);


				vtkSmartPointer<vtkLine> line0 =
					vtkSmartPointer<vtkLine>::New();
				line0->GetPointIds()->SetId(0, 0); // 第二个0表示pts中的origin点
				line0->GetPointIds()->SetId(1, 1); //第二个1表示pts中的p0点

				

				vtkSmartPointer<vtkCellArray> lines =
					vtkSmartPointer<vtkCellArray>::New();
				lines->InsertNextCell(line0);

				parent->m_Parent->m_LineActor.SetLines(lines);
				const double red[3] = { 255, 0, 0 };
				const double green[3] = { 0, 255, 0 };

				vtkSmartPointer<vtkUnsignedCharArray> colors =
					vtkSmartPointer<vtkUnsignedCharArray>::New();
				colors->SetNumberOfComponents(3);
				colors->InsertNextTuple(red);
				parent->m_Parent->m_LineActor.SetScalars(colors);
			}
		}

		
		parent->m_Parent->UpdateView();// 更新显示
		parent->m_Second->UpdateView();

		}
}
void VtkVisual::Load(const std::string FileName) {
	//m_StlModel.SetFileName(".\\Prefab\\cube.stl");
	m_StlModel.SetFileName(FileName);
}

void VtkVisual::Load() {
	//m_StlModel.SetFileName(".\\Prefab\\cube.stl");
	// 加载模型坐标点
	//assert(res >= 0);
}

bool VtkVisual::Record(void) {
	return false;
}
void VtkVisual::ResetRecord() {
	m_RecordPoints.Reset();
}

void VtkVisual::RemoveLastRecord() {
	m_RecordPoints.DeleteLastPoint();
}

size_t VtkVisual::GetRecordSize() {
	return m_RecordPoints.GetNumberOfPoints();
}

bool VtkVisual::GetRecord(size_t id, Vector3d &point) {
	size_t size = m_RecordPoints.GetNumberOfPoints();
	if (id >= size) { return false; }
	m_RecordPoints.GetPoint(id, point.data());
	return true;
}

bool VtkVisual::GetLastRecord(Vector3d &point) {
	size_t size = m_RecordPoints.GetNumberOfPoints();
	if (size <= 0) { return false; }
	m_RecordPoints.GetPoint(size - 1, point.data());
	return true;
}

bool VtkVisual::Calibrate(bool clear, size_t method) {
	return false;
}

void VtkVisual::DoTransform() {
	std::vector<toolPosition>toolList;

	vtkSmartPointer<vtkMatrix4x4> matrix = vtkSmartPointer<vtkMatrix4x4>::New();
	aimooeOperator::GetInstance().GetToolInfo(toolList);//单机调试
	for (int o = 0; o < 2; o++) {
		if (toolList.at(o).name == "base") {
			for (int i = 0; i < 3; i++) {
				matrix->SetElement(i, 3, toolList.at(o).Tto[i]);
				for (int j = 0; j < 3; j++)
					matrix->SetElement(i, j, toolList.at(o).Rto[i][j]);
			}
			m_StlModel.SetTransform(matrix);
		}
		else
			break;
	}

	
}
double *VtkVisual::GetCameraPosition() {
	double ret[3];
	ret[0] = this->m_Arrows[0].GetTransform()->GetPosition()[0];
	ret[1] = this->m_Arrows[0].GetTransform()->GetPosition()[1];
	ret[2] = this->m_Arrows[0].GetTransform()->GetPosition()[2] + 10;
	return ret;
}

void VtkVisual::Highlight(bool ispancreas) {
	if (m_Parent) {
		vtkNew<vtkNamedColors> namedColors;
		if (ispancreas) {
			pancreas_arrow.SetColor(namedColors->GetColor3d("Red").GetData());
			gallbladder_arrow.SetColor(namedColors->GetColor3d("DarkBlue").GetData());
		}
		else {
			pancreas_arrow.SetColor(namedColors->GetColor3d("DarkBlue").GetData());
			gallbladder_arrow.SetColor(namedColors->GetColor3d("Red").GetData());
		}
	}
}

void VtkVisual::GetMatrix4x4(double mat[16]) {

}

Matrix4d VtkVisual::GetMatrix4x4() {
	Matrix4d ret;
	return ret;

}
void VtkVisual::SwitchTrackbll(std::string mth) {
	if (mth == "c") {
		vtkSmartPointer<vtkInteractorStyleJoystickCamera> style =
			vtkSmartPointer<vtkInteractorStyleJoystickCamera>::New();
		m_Parent->m_vtkWin->GetInteractor()->SetInteractorStyle(style);
	}
	if (mth == "a") {
		vtkSmartPointer<vtkInteractorStyleTrackballActor> style =
			vtkSmartPointer<vtkInteractorStyleTrackballActor>::New();
		m_Parent->m_vtkWin->GetInteractor()->SetInteractorStyle(style);
	}
}
void VtkVisual::Reset() {
	m_Parent = NULL;
}

void VtkVisual::Start(CVTKDisplay &vtkwin, CVTKDisplay &vtkwin2) {
	m_Second = &vtkwin2;
	m_Parent = &vtkwin;
	vtkNew<vtkNamedColors> namedColors;
	/*Load();*/
	/*m_StlModel.SetColor(103/255.0, 254 / 255.0, 227 / 255.0);
	m_StlModel.SetOpacity(0.4);*/
	
	m_StlModel.SetColor(1, 1, 1);
	//m_StlModel.SetOpacity(1);
	
	m_StlModel.GetActor()->SetOrigin(119.51, -92.79, -119.935);//从stl文件中计算对于模型tool的平移和旋转
	m_StlModel.GetActor()->SetOrientation(47.3,60.12,58.7);
	// 加载模型
	m_Parent->AddActor(m_StlModel.GetActor());
	m_Second->AddActor(m_StlModel.GetActor());
	
	
	//// 显示模型点


	//// 关联模型点和模型的坐标变换
	//m_Parent->m_PickedPoints.GetTransform()->SetInput(m_StlModel.GetTransform());
	//m_Second->m_PickedPoints.GetTransform()->SetInput(m_StlModel.GetTransform());


	////// 坐标轴Actor
	vtkNew<vtkAxesActor> axes;
	axes->SetPosition(0, 0, 0);
	axes->SetTotalLength(SCALE_FACTOR, SCALE_FACTOR, SCALE_FACTOR);
	axes->SetShaftType(0);
	axes->SetAxisLabels(0);
	axes->SetTotalLength(1000,1000,1000);
	m_Parent->m_Render->AddActor(axes);
	
	vtkSmartPointer<vtkConeSource> sphereSource =
		vtkSmartPointer<vtkConeSource>::New();
	sphereSource->SetHeight(25.0);
	sphereSource->SetRadius(10.0);
	sphereSource->SetResolution(10);
	sphereSource->SetDirection(0, 1, 0);
	sphereSource->Update();
	
		
	// Create a mapper and actor
	vtkSmartPointer<vtkPolyDataMapper> mapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputConnection(sphereSource->GetOutputPort());
	cameraActor = vtkSmartPointer<vtkActor>::New();
	cameraActor->SetMapper(mapper);
	cameraActor->GetProperty()->SetColor(1, 0, 0);
	
	



	m_Parent->AddActor(cameraActor);
	


	
	m_fp = vtkCamera::New();
	m_fp->SetViewAngle(60);
	m_fp->ParallelProjectionOff();
	m_Second->m_Render->GetActiveCamera()->SetClippingRange(0, 100000);
	m_Second->m_Render->SetActiveCamera(m_fp);
	
	
	

	// 箭头动态更新
	vtkNew<VtkTimerCallback> callback;
	callback->parent = this;
	m_Parent->m_Render->GetActiveCamera()->SetClippingRange(5, 100000);
	
	m_Parent->m_vtkItr->AddObserver(vtkCommand::TimerEvent, callback);
	m_Parent->m_vtkItr->CreateRepeatingTimer(100);
	
	// m_trans = m_Arrows[0].GetTransform();
	m_Parent->Show();
	m_Second->Show();
	isStarted = 1;
}


