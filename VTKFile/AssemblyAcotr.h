#pragma once
#include <vtkAssembly.h>
#include <vtkTransform.h>
#include <vtkSmartPointer.h>

class CAssemblyAcotr {
public:
    CAssemblyAcotr(void);
    ~CAssemblyAcotr(void);

    void       AddPart(vtkProp3D *Part);
    void       RemovePart(vtkProp3D *part);
    vtkProp3D *GetActor();
    void       SetTransform(double matrix[16]);
    void       SetTransform(vtkMatrix4x4 *matrix);
    void       Modified();

protected:
    vtkSmartPointer<vtkAssembly>  m_Assembly;
    vtkSmartPointer<vtkTransform> m_trans;
};
