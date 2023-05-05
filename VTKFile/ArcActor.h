#pragma once
#include <vtkArcSource.h>
#include <vtkSmartPointer.h>
#include <vtkTransformFilter.h>
#include "Actor.hpp"

class CArcActor : public CActor<vtkTransformFilter> {
public:
    CArcActor(void);
    ~CArcActor(void) = default;

    void SetPoint1(double *pt1);
    void SetPoint2(double *pt2);
    void SetCenter(double *center);
    void SetNormal(double *normal);
    void SetResolution(int resolution);
    void SetPolarVector1(double *vector);
    void SetPolarVector2(double *vector);
    void SetUseNormalAndAngle(bool bUse);
    void Modify();

protected:
    vtkSmartPointer<vtkArcSource> arc;

    double polarVector1[3];
    double polarVector2[3];
};
