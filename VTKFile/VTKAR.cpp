#include "VTKAR.h"

CVTKAR::CVTKAR(QVTKWidget *pView) {
    scene_Renderer = vtkSmartPointer<vtkRenderer>::New();
    bg_renderer       = vtkSmartPointer<vtkRenderer>::New();
    m_vtkWin       = pView;
    m_vtkItr       = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    m_Mark         = vtkSmartPointer<vtkOrientationMarkerWidget>::New();
    m_Actor        = vtkSmartPointer<vtkActor>::New();
    m_scale        = vtkTransform::New();
    im_Actor       = vtkSmartPointer<vtkImageActor>::New();
    im             = vtkSmartPointer<vtkImageData>::New();

    scene_Renderer->TwoSidedLightingOn();
    {
        vtkNew<vtkNamedColors> colors;
        scene_Renderer->SetBackground(colors->GetColor3d("SlateGray").GetData());
    }
    scene_Renderer->ResetCamera();
    //m_camera = scene_Renderer->GetActiveCamera();
    scene_Renderer->GetActiveCamera()->SetPosition(3.39, -4.60, 1.58);
    scene_Renderer->GetActiveCamera()->SetFocalPoint(-0.32, 0.22, -0.21);
    scene_Renderer->GetActiveCamera()->SetViewUp(0, -1, 0);
    //scene_Renderer->GetActiveCamera()->SetPosition(0, 0, 0);
    //scene_Renderer->GetActiveCamera()->SetFocalPoint(0, 0, 1);
    //scene_Renderer->GetActiveCamera()->SetViewUp(0, 1, 0);

    scene_Renderer->GetActiveCamera()->SetViewAngle(60);
    scene_Renderer->GetActiveCamera()->ParallelProjectionOn();
    m_scale->SetInput(m_trans);
    m_tffilter->SetTransform(m_scale);
    m_Actor->SetUserTransform(m_trans);
    /*if (FirstPerspective)
        m_Actor->VisibilityOn();
    else
        m_Actor->VisibilityOff();*/

    //m_vtkWin与qwidget绑定

    m_vtkWin->GetRenderWindow()->SetNumberOfLayers(2);
    m_vtkWin->GetRenderWindow()->GetStereoCapableWindow();
    m_vtkWin->GetRenderWindow()->StereoCapableWindowOn();
    m_vtkWin->GetRenderWindow()->AddRenderer(bg_renderer);
    m_vtkWin->GetRenderWindow()->AddRenderer(scene_Renderer);
    m_vtkWin->GetRenderWindow()->SetStereoRender(1);
    m_vtkWin->GetRenderWindow()->SetStereoRender(2);
    m_vtkWin->GetRenderWindow()->SetStereoTypeToDresden();
    m_vtkWin->GetRenderWindow()->StereoRenderOn();


    if ((m_vtkItr = m_vtkWin->GetInteractor()) == NULL) {
        
        vtkSmartPointer<vtkInteractorStyleTrackballCamera> style =
            vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
        m_vtkItr->SetInteractorStyle(style);
        m_vtkItr->SetRenderWindow(m_vtkWin->GetRenderWindow());
        m_vtkItr->Initialize();
    }

    bg_renderer->SetLayer(0);       // 背景render以显示图像
    scene_Renderer->SetLayer(1);    // 前景render以显示3D模型
    scene_Renderer->SetLightFollowCamera(1);
    im_Actor->SetInputData(im);

    bg_renderer->AddActor(im_Actor);
    
}
void CVTKAR::UpdateView() {
    m_vtkWin->GetRenderWindow()->Render();
}

void CVTKAR::Display(cv::Mat frame) {
    // if (FirstPerspective)
    //    m_Actor->VisibilityOn();
    // else
    //    m_Actor->VisibilityOff();
    if (!frame.empty()) {
        Ipl2VTK(frame, im);
        im_Actor->Modified();
        m_vtkWin->GetRenderWindow()->Render();
    }
}

void CVTKAR::SetVirtualModel() {

    ////int numPts = reader->GetNumberOfPoints();             // 获取模型的顶点数量
    //auto numPts = reader->GetFilter()->GetOutput()->GetPoints()->GetNumberOfPoints();
    //vtkSmartPointer<vtkFloatArray> scalars = vtkSmartPointer<vtkFloatArray>::New();  // 创建存储顶点属性的float数组
    //scalars->SetNumberOfValues(numPts);
    //for (int i = 0; i < numPts; ++i)  // 为属性数组中的每个元素设置标量值（这个标量值可以当作颜色值）
    //    scalars->SetValue(
    //        i,
    //        i);  //这个地方点和标量产生联系(第一个参数是第几个点；第二个参数就是设定的标量值)现在就假设第i个点颜色就是i

    //vtkSmartPointer<vtkPolyData> poly = vtkSmartPointer<vtkPolyData>::New();

    //poly = reader->GetFilter()->GetOutput();
    //poly->GetPointData()->SetScalars(scalars);

    //// 创建颜色查找表
    //vtkSmartPointer<vtkLookupTable> clt = vtkSmartPointer<vtkLookupTable>::New();
    //clt->SetNumberOfColors(1);  // 指定颜色查找表中有多少种颜色
    //clt->SetTableValue(0, 1.0, 0.0, 0.0, 0.2);
    //clt->Build();

    //vtkSmartPointer<vtkPolyDataMapper> VirMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    //VirMapper->SetInputData(poly);
    //VirMapper->SetScalarRange(0, numPts);
    //VirMapper->ScalarVisibilityOn();
    //// objMapper->SetColorModeToMapScalars();
    //VirMapper->SetColorModeToDefault();
    //VirMapper->SetLookupTable(clt);

    //m_Actor->SetMapper(VirMapper);
    //m_Actor->SetOrigin(scene_Renderer->GetActiveCamera()->GetPosition());

    //scene_Renderer->InteractiveOff();
    ////scene_Renderer->AddActor(m_Actor);

    //bg_renderer->InteractiveOff();  ///< 关闭交互操作即不再响应鼠标键盘操作
    //bg_renderer->AddActor(im_Actor);

    /*m_vtkWin->SetNumberOfLayers(2);
    m_vtkWin->AddRenderer(bg_Renderer);
    m_vtkWin->AddRenderer(scene_Renderer);*/
}

void CVTKAR::SetvirtualModelPostion(double x, double y, double z, double a, double b, double t) {
    m_trans->RotateX(a);
    m_trans->RotateX(b);
    m_trans->RotateX(t);
    m_trans->Translate(x, y, z);
}
void CVTKAR::SetvirtualModelPostion(double x, double y, double z) {
    // m_trans->Translate(x, y, z);
    m_Actor->SetPosition(x, y, z);
}
void CVTKAR::SetvirtualModelPostion(double matrix[16]) {
    m_trans->SetMatrix(matrix);
    m_trans->Modified();
}
void CVTKAR::SetvirtualModelPostion(vtkMatrix4x4 *matrix) {
    m_trans->SetMatrix(matrix);
    m_trans->Modified();
}
void CVTKAR::Ipl2VTK(cv::Mat &src, vtkImageData *dst) {
    if (!src.empty()) {
        cv::flip(src, src, 1);
        //cv::cvtColor(src, src, cv::COLOR_BGR2RGB);
        vtkImageImport *importer = vtkImageImport::New();

        importer->SetOutput(dst);
        importer->SetDataSpacing(1, 1, 1);
        importer->SetDataOrigin(0, 0, 0);
        importer->SetWholeExtent(0, src.size().width - 1, 0, src.size().height - 1, 0, 0);
        importer->SetDataExtentToWholeExtent();
        importer->SetDataScalarTypeToUnsignedChar();
        importer->SetNumberOfScalarComponents(src.channels());
        importer->SetImportVoidPointer(src.data);
        importer->Update();
    }
}