#pragma once
#include <QObject>
#include <Eigen/Dense>
#include <vtkCallbackCommand.h>
#include <vtkTimerLog.h>
#include "VTKFile/VTKDisplay.h"
#include "VTKFile/ArrowActor.h"
#include "VTKFile/VTKAR.h"
#include "VideoFlow.h"

#include "VtkVisual.h"

#include <math.h>

using namespace Eigen;
// TD:第一视角相机两个窗口初始化

    class ARVisual {

    public:
        /**
         * @brief 单例模式获取实例
         */
        static ARVisual &GetInstance() {
            static ARVisual _vtk;
            return _vtk;
        }

        void Load();
        void Load(const std::string FileName);
        void Start(CVTKAR &);
		
        bool FirstPerspective = false;
        bool Projection       = true;
        //int  RecordChannel    = 0;

    private:
        ARVisual() {}
        ARVisual(ARVisual &) = delete;
        ~ARVisual() {}
		cv::Mat frame;

    public:
        
		
    protected:
        CVTKAR *m_Parent = NULL;
        friend class vtkTimerCallback;
        CSTLReader m_StlModel;
    };
void CallbackFunction(vtkObject* caller, long unsigned int vtkNotUsed(eventId),
			void* vtkNotUsed(clientData), void* vtkNotUsed(callData));
  // namespace ercp
