#pragma once
#include <vtkTubeFilter.h>
#include <vtkSmartPointer.h>
#include "Actor.hpp"

class CTubeActor : public CActor<vtkTubeFilter> {
public:
    CTubeActor(void);
    ~CTubeActor(void) = default;

    void AddInputConnection(vtkAlgorithmOutput *in);
    void AddInputConnection(int portindex, vtkAlgorithmOutput *in);
    void Modified();

protected:
    vtkSmartPointer<vtkTubeFilter> m_tubeFiter;
};
