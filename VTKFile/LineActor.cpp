#include "LineActor.h"

CLineActor::CLineActor(void) {
    m_points     = vtkPoints::New();
    linesPolyData->SetPoints(m_points);
    m_pdmapper->SetInputData(linesPolyData);
    m_actor->SetMapper(m_pdmapper);
    m_tffilter->SetTransform(m_trans);
    m_actor->SetUserTransform(NULL);
    
}

void CLineActor::Reset() {
    m_points->Reset();
}

void CLineActor::InsertPoint(int i, double x, double y, double z) {
    m_points->InsertPoint(i, x, y, z);
}

void CLineActor::InsertNextPoint(double x, double y, double z) {
    m_points->InsertNextPoint(x, y, z);
}

void CLineActor::Modified() {
    m_points->Modified();
    m_actor->Modified();
}
void CLineActor::CreateMultiLine(CSTLReader stlReader, double lineP0[3], double lineP1[3]) {
    // Create the locator
    vtkSmartPointer<vtkOBBTree> tree =
        vtkSmartPointer<vtkOBBTree>::New();
    tree->SetDataSet(stlReader.GetPoly());
    tree->BuildLocator();

    // Intersect the locator with the line
   
    

    double tol = 1.e-8;
    tree->SetTolerance(tol);
    tree->IntersectWithLine(lineP0, lineP1,
        intersectPoints,
        intersectCells);
}
vtkSmartPointer<vtkPoints> CLineActor::GetInsertPoint() {
    return intersectPoints;
}
void CLineActor::SetPoints(vtkPoints* pts) {
    linesPolyData->SetPoints(pts);
}
void CLineActor::SetLines(vtkCellArray* lines) {
    linesPolyData->SetLines(lines);
}
void CLineActor::SetScalars(vtkUnsignedCharArray* color) {
    linesPolyData->GetCellData()->SetScalars(color);
}