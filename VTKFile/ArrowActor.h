#pragma once
#include <vtkArrowSource.h>
#include <vtkSmartPointer.h>
#include <vtkTransformFilter.h>
#include <vtkQuaternion.h>
#include "Actor.hpp"

class CArrowActor : public CActor<vtkTransformFilter> {
public:
    CArrowActor(void);
    ~CArrowActor(void) = default;

    void Modified();
    void SetTransform(double matrix[16]);
    void SetTransform(vtkMatrix4x4 *matrix);
    void SetPosition(double point[3]);
    void SetDirection(double vector[3]);
    void SetStartEnd(double start[3], double end[3]);
    void SetScale(double x, double y, double z);

protected:
    vtkSmartPointer<vtkArrowSource> m_arrow;
    vtkSmartPointer<vtkTransform>   m_scale;
};
