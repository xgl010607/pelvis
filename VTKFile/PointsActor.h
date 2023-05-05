#pragma once
#include <vtkPoints.h>
#include <vtkSphereSource.h>
#include <vtkGlyph3D.h>
#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include <vtkVector.h>
#include <vtkTransformPolyDataFilter.h>
#include "Actor.hpp"

class CPointsActor : public CActor<vtkTransformPolyDataFilter> {
public:
    CPointsActor(void);
    ~CPointsActor(void) = default;

    void        DeleteLastPoint();
    void        Reset();
    void        Modified();
    void        SetSphere(float radius);
    void        GetPoint(int i, double *pt);
    vtkVector3d GetPoint(int i);
    void        AddPoint(double point[3]);
    void        AddPoint(double x, double y, double z);
    void        SetPoints(vtkPoints *points);
    size_t      GetNumberOfPoints();

    vtkPolyData *const  GetOutptut();

protected:
    vtkSmartPointer<vtkPoints>       m_points;
    vtkSmartPointer<vtkSphereSource> m_sphere;
    vtkSmartPointer<vtkPolyData>     m_polydata;
    vtkSmartPointer<vtkGlyph3D>      m_glyph;

    friend class CVTKDisplay;
};
