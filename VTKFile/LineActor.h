#pragma once
#include <vtkPoints.h>
#include <vtkLineSource.h>
#include <vtkSmartPointer.h>
#include <vtkTransformFilter.h>
#include <vtkUnsignedCharArray.h>
#include <vtkOBBTree.h>
#include <vtkCell.h>
#include <vtkTubeFilter.h>
#include <vtkDataSet.h>
#include <vtkCellArray.h>
#include <vtkCellData.h>
#include "STLReader.h"
#include "Actor.hpp"

class CLineActor : public CActor<vtkTransformFilter> {
public:
    CLineActor(void);
    ~CLineActor(void) = default;
    
    void Reset();
    void InsertPoint(int i, double x, double y, double z);
    void InsertNextPoint(double x, double y, double z);
    void CreateMultiLine(CSTLReader stlReader, double lineP0[3], double lineP1[3]);
    void Modified();
    vtkSmartPointer<vtkPoints> GetInsertPoint();
    void SetPoints(vtkPoints* pts);
    void SetLines(vtkCellArray* lines);
    void SetScalars(vtkUnsignedCharArray* color);

protected:
    vtkSmartPointer<vtkPoints>     m_points;
    vtkSmartPointer<vtkPolyData> linesPolyData =
        vtkSmartPointer<vtkPolyData>::New();
    vtkSmartPointer<vtkCellArray> lines =
        vtkSmartPointer<vtkCellArray>::New();
    vtkSmartPointer<vtkPoints> intersectPoints =
        vtkSmartPointer<vtkPoints>::New();

    vtkSmartPointer<vtkIdList> intersectCells =
        vtkSmartPointer<vtkIdList>::New();
};
