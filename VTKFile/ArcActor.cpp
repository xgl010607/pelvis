#include "ArcActor.h"

CArcActor::CArcActor(void) {
    arc = vtkArcSource::New();

    arc->SetResolution(20);
    arc->SetOutputPointsPrecision(vtkAlgorithm::DOUBLE_PRECISION);
    m_tffilter->SetTransform(m_trans);
    m_actor->SetUserTransform(NULL);
    SetInputConnection(arc->GetOutputPort());
    SetLineWidth(3);
}

void CArcActor::SetPoint1(double *pt1) {
    arc->SetPoint1(pt1);
}

void CArcActor::SetPoint2(double *pt2) {
    arc->SetPoint2(pt2);
}

void CArcActor::SetCenter(double *center) {
    arc->SetCenter(center);
}

void CArcActor::SetNormal(double *normal) {
    arc->SetNormal(normal);
}

void CArcActor::SetResolution(int resolution) {
    arc->SetResolution(resolution);
}

void CArcActor::SetPolarVector1(double *vector) {
    for (int i = 0; i < 3; i++) {
        polarVector1[i] = vector[i];
    }
}

void CArcActor::SetPolarVector2(double *vector) {
    for (int i = 0; i < 3; i++) {
        polarVector2[i] = vector[i];
    }
}

void CArcActor::SetUseNormalAndAngle(bool bUse) {
    arc->SetUseNormalAndAngle(bUse);
}

void CArcActor::Modify() {
    arc->Modified();
}
