#ifndef __ROVOT_VTK__
#define __ROVOT_VTK__

#pragma once
#include <vtkRenderWindowInteractor.h>
#include <vtkCameraActor.h>
#include<vtkConeSource.h>
#include <vtkUnsignedCharArray.h>

#include <vtkLine.h>
#include <vtkIdList.h>
#include <Eigen/Dense>
#include<vtkInteractorStyleTrackballActor.h>
#include "VTKFile/VTKDisplay.h"
#include "VTKFile/ArrowActor.h"
#include "VTKFile/VTKAR.h"
#include"VideoFlow.h"
#include "aimooeOperator.h"
//#include "thread.h"
#include <string>
#include <vtkObjectFactory.h>
#include <qdebug.h>
using namespace Eigen;



    class VtkVisual {

    public:
        /**
         * @brief 单例模式获取实例
         */
        static VtkVisual &GetInstance() {
            static VtkVisual _vtk;
            return _vtk;
        }

        void Load();
        void Start(CVTKDisplay &,CVTKDisplay &);
        void Reset();

        bool   Record(void);
        void   ResetRecord();
        size_t GetRecordSize();
        bool   GetRecord(size_t id, Vector3d &point);
        bool   GetLastRecord(Vector3d &point);
        void   RemoveLastRecord();

        double * GetCameraPosition();
        bool     Calibrate(bool clear = false, size_t method = 0);
        void     GetMatrix4x4(double mat[16]);
        Matrix4d GetMatrix4x4();
        void     DoTransform();
		void Load(const std::string FileName);
        void Highlight(bool ispancreas);
        bool FirstPerspective;
        bool ThirdPerspective;
		bool isStarted=0;
		void SwitchTrackbll(std::string mth);
        vtkSmartPointer<vtkCamera> m_fp;
        // vtkSmartPointer<vtkTransform>   m_trans;  //虚拟相机位置
        int RecordChannel = 0;

    private:
        VtkVisual() {
        }
        VtkVisual(VtkVisual &) = delete;
        ~VtkVisual() {
        }

    public:
        CSTLReader m_StlModel;

    protected:
        const int    m_MaxPorts = 1;
        CVTKDisplay *m_Parent   = NULL;
		CVTKDisplay *m_Second = NULL;
        CVTKAR *     m_seWidget = NULL;
	
        std::vector<CArrowActor>      m_Arrows;
        std::vector<vtkTextActor *>   m_Textactors;
        vtkSmartPointer<vtkTextActor> m_TextPicker;

        CPointsActor m_RecordPoints;
		vtkSmartPointer<vtkActor> cameraActor;
        CArrowActor pancreas_arrow;
        CArrowActor gallbladder_arrow;
        //CSTLReader m_ViewModel ;

        friend class VtkTimerCallback;
    };

// namespace ercp

#endif  // __ROVOT_VTK__
