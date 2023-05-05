#include "TubeActor.h"

CTubeActor::CTubeActor(void) {
    m_tubeFiter = vtkTubeFilter::New();
    m_tubeFiter->SetRadius(3.5);
    m_tubeFiter->SetNumberOfSides(10);
    SetInputConnection(m_tubeFiter->GetOutputPort());
    SetColor(0.1, 0.3, 0.6);
    m_actor->SetUserTransform(m_trans);
}

void CTubeActor::AddInputConnection(vtkAlgorithmOutput *in) {
    m_tubeFiter->AddInputConnection(in);
}
void CTubeActor::AddInputConnection(int portindex, vtkAlgorithmOutput *in) {
    m_tubeFiter->AddInputConnection(portindex, in);
}
void CTubeActor::Modified() {
    m_tubeFiter->Modified();
}
