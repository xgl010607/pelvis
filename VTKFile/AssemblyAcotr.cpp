#include "AssemblyAcotr.h"

CAssemblyAcotr::CAssemblyAcotr(void) {
    m_Assembly = vtkAssembly::New();
    m_trans    = vtkTransform::New();
    m_Assembly->SetUserTransform(m_trans);
}

void CAssemblyAcotr::AddPart(vtkProp3D *Part) {
    m_Assembly->AddPart(Part);
}

void CAssemblyAcotr::RemovePart(vtkProp3D *part) {
    m_Assembly->RemovePart(part);
}

vtkProp3D *CAssemblyAcotr::GetActor() {
    return m_Assembly;
}

void CAssemblyAcotr::SetTransform(double matrix[16]) {
    m_trans->SetMatrix(matrix);
    m_trans->Modified();
}

void CAssemblyAcotr::SetTransform(vtkMatrix4x4 *matrix) {
    m_trans->SetMatrix(matrix);
    m_trans->Modified();
}
void CAssemblyAcotr::Modified() {
    m_Assembly->Modified();
}
