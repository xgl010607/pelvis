#pragma once
#include <vtkPolyDataReader.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkPolyDataNormals.h>
#include <vtkPolyData.h>
#include <vtkSTLReader.h>
#include <vtkSmartPointer.h>
#include "Actor.hpp"

class CSTLReader : public CActor<vtkTransformPolyDataFilter> {
public:
    CSTLReader(void);
    ~CSTLReader(void) = default;

    void Modified();
    void SetFileName(std::string filename);
    int GetNumberOfPoints();
    vtkPolyData* GetPoly();
    vtkSmartPointer<vtkSTLReader>       pdreader;
protected:
    vtkSmartPointer<vtkPolyDataNormals> normals;
    vtkSmartPointer<vtkPolyData> m_polydata ;

};
