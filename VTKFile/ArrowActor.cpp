#include <vtkVector.h>
#include "ArrowActor.h"

CArrowActor::CArrowActor(void) {
    m_arrow = vtkArrowSource::New();
    m_scale = vtkTransform::New();

    m_arrow->SetTipRadius(0.2);
    m_arrow->SetShaftRadius(0.05);
    m_scale->SetInput(m_trans);
    m_tffilter->SetTransform(m_scale);
    m_actor->SetUserTransform(NULL);
    m_tffilter->SetInputConnection(m_arrow->GetOutputPort());
    m_tffilter->Update();
    m_trans->Scale(1, 1, 1);
}

void CArrowActor::Modified() {
    m_arrow->Modified();
}

void CArrowActor::SetTransform(double matrix[16]) {
    m_trans->SetMatrix(matrix);
    m_trans->Modified();
}

void CArrowActor::SetTransform(vtkMatrix4x4 *matrix) {
    m_trans->SetMatrix(matrix);
    m_trans->Modified();
}

void CArrowActor::SetPosition(double point[3]) {
    vtkMatrix4x4 *homo  = m_trans->GetMatrix();
    homo->Element[0][3] = point[0];
    homo->Element[1][3] = point[1];
    homo->Element[2][3] = point[2];
    m_trans->SetMatrix(homo);
    m_trans->Modified();
}

void CArrowActor::SetDirection(double vector[3]) {
    vtkMatrix4x4 *homo = m_trans->GetMatrix();
    vtkVector3d   vec(vector);
    vec.Normalize();
    vtkVector3d y(homo->Element[0][1], homo->Element[1][1], homo->Element[2][1]);
    double      nz[3];
    vtkMath::Cross(y.GetData(), vec.GetData(), nz);
    homo->Element[0][0] = vec[0];
    homo->Element[1][0] = vec[1];
    homo->Element[2][0] = vec[2];
    homo->Element[0][2] = nz[0];
    homo->Element[1][2] = nz[1];
    homo->Element[2][2] = nz[2];
    m_trans->SetMatrix(homo);
    m_trans->Modified();
}

void CArrowActor::SetStartEnd(double start[3], double end[3]) {
    vtkVector3d vec(end[0] - start[0], end[1] - start[1], end[2] - start[2]);
    m_trans->Identity();
    SetPosition(start);
    SetDirection(vec.GetData());
    // ·ÅËõ³¤¶È
    SetScale(vec.Norm(), 5, 5);
}

void CArrowActor::SetScale(double x, double y, double z) {
    m_scale->Identity();
    m_scale->Scale(x, y, z);
}
