#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkSmartPointer.h>
#include <vtkDataSet.h>
#include <vtkPointData.h>
#include <vtkDoubleArray.h>

#include "PointsActor.h"

CPointsActor::CPointsActor(void) {
    m_points   = vtkPoints::New();
    m_polydata = vtkPolyData::New();
    m_sphere   = vtkSphereSource::New();
    m_glyph    = vtkGlyph3D::New();

    m_polydata->SetPoints(m_points);
    m_tffilter->SetInputData(m_polydata);
    m_tffilter->SetTransform(m_trans);
    m_actor->SetUserTransform(NULL);
    m_glyph->SetSourceConnection(m_sphere->GetOutputPort());
    m_glyph->SetInputConnection(m_tffilter->GetOutputPort());
    m_pdmapper->SetInputConnection(m_glyph->GetOutputPort());
    m_actor->SetMapper(m_pdmapper);
}

void CPointsActor::DeleteLastPoint() {
    m_points->SetNumberOfPoints(m_points->GetNumberOfPoints() - 1);
    m_points->Modified();
}

void CPointsActor::Reset() {
    m_points->Reset();
    m_points->Modified();
}

void CPointsActor::SetPoints(vtkPoints *points) {
    m_points->DeepCopy(points);
    m_points->Modified();
}

void CPointsActor::Modified() {
    m_actor->Modified();
}

void CPointsActor::SetSphere(float radius) {
    m_sphere->SetRadius(radius);
    m_sphere->Modified();
}

void CPointsActor::GetPoint(int i, double *pt) {
    m_polydata->GetPoint(i, pt);
}

vtkVector3d CPointsActor::GetPoint(int i) {
    return vtkVector3d(m_polydata->GetPoint(i));
}

void CPointsActor::AddPoint(double point[3]) {
    vtkNew<vtkMatrix4x4> invtf;
    m_trans->GetInverse(invtf);
    double in[4]  = {point[0], point[1], point[2], 1};
    double out[4] = {0};
    invtf->MultiplyPoint(in, out);
    m_points->InsertNextPoint(out);
    m_points->Modified();
}

void CPointsActor::AddPoint(double x, double y, double z) {
    vtkNew<vtkMatrix4x4> invtf;
    m_trans->GetInverse(invtf);
    double in[4]  = {x, y, z, 1};
    double out[4] = {0};
    invtf->MultiplyPoint(in, out);
    m_points->InsertNextPoint(out);
    m_points->Modified();
}

size_t CPointsActor::GetNumberOfPoints() {
    return m_points->GetNumberOfPoints();
}

vtkPolyData *const CPointsActor::GetOutptut() {
    return m_tffilter->GetOutput();
}
