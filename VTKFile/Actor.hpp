#pragma once
#include <vtkActor.h>
#include <vtkPolyDataMapper.h>
#include <vtkPolyData.h>
#include <vtkProperty.h>
#include <vtkTransform.h>
#include <vtkSmartPointer.h>

/// You should manually invoke something like m_tffilter->SetTransform(m_trans).
/// Here will invoke m_actor->SetUserTransform(m_trans) by default to make m_trans work.
/// You can recover it by m_actor->SetUserTransform(NULL).
template <typename Filter_t>
class CActor {
public:
    CActor(void);
    ~CActor(void) = default;

public:
    void                SetInputConnection(vtkAlgorithmOutput *in);
    void                SetColor(double rgb_r, double rgb_g, double rgb_b);
    void                SetColor(double color[3]);
    void                GetColor(double &r, double &g, double &b);
    void                SetOpacity(double opa);
    double              GetOpacity();
    void                SetDiffuse(double dif);
    void                SetAmbient(double amb);
    void                SetTransform(const double matrix[16]);
    void                SetTransform(vtkMatrix4x4 *matrix);
    vtkActor *          GetActor();
    void                Visibility(bool bshow);
    void                SetLineWidth(double width);
    vtkTransform *const GetTransform();
    Filter_t *const     GetFilter();

protected:
    vtkSmartPointer<vtkActor>          m_actor;
    vtkSmartPointer<vtkTransform>      m_trans;
    vtkSmartPointer<Filter_t>          m_tffilter;
    vtkSmartPointer<vtkPolyDataMapper> m_pdmapper;
};

template <typename Filter_t>
CActor<Filter_t>::CActor(void) {
    m_actor    = vtkActor::New();
    m_pdmapper = vtkPolyDataMapper::New();
    m_trans    = vtkTransform::New();
    m_tffilter = Filter_t::New();

    m_trans->Identity();

    m_pdmapper->SetInputConnection(m_tffilter->GetOutputPort());
    m_actor->SetMapper(m_pdmapper);
    m_actor->SetUserTransform(m_trans);
}

template <typename Filter_t>
void CActor<Filter_t>::SetInputConnection(vtkAlgorithmOutput *in) {
    m_pdmapper->SetInputConnection(in);
}

template <typename Filter_t>
void CActor<Filter_t>::SetColor(double rgb_r, double rgb_g, double rgb_b) {
    this->m_actor->GetProperty()->SetColor(rgb_r, rgb_g, rgb_b);
}

template <typename Filter_t>
void CActor<Filter_t>::SetColor(double color[3]) {
    this->m_actor->GetProperty()->SetColor(color);
}

template <typename Filter_t>
void CActor<Filter_t>::GetColor(double &r, double &g, double &b) {
    this->m_actor->GetProperty()->GetColor(r, g, b);
}

template <typename Filter_t>
void CActor<Filter_t>::SetOpacity(double opa) {
    m_actor->GetProperty()->SetOpacity(opa);
}

template <typename Filter_t>
double CActor<Filter_t>::GetOpacity() {
    return this->m_actor->GetProperty()->GetOpacity();
}

template <typename Filter_t>
void CActor<Filter_t>::SetDiffuse(double dif) {
    m_actor->GetProperty()->SetDiffuse(dif);
}

template <typename Filter_t>
void CActor<Filter_t>::SetAmbient(double amb) {
    m_actor->GetProperty()->SetAmbient(amb);
}

template <typename Filter_t>
void CActor<Filter_t>::SetTransform(const double matrix[16]) {
    m_trans->SetMatrix(matrix);
    m_trans->Modified();
}

template <typename Filter_t>
void CActor<Filter_t>::SetTransform(vtkMatrix4x4 *matrix) {
    m_trans->SetMatrix(matrix);
    m_trans->Modified();
}

template <typename Filter_t>
vtkActor *CActor<Filter_t>::GetActor() {
    return m_actor;
}

template <typename Filter_t>
void CActor<Filter_t>::Visibility(bool bshow) {
    if (bshow) {
        m_actor->VisibilityOn();
    } else {
        m_actor->VisibilityOff();
    }
}

template <typename Filter_t>
void CActor<Filter_t>::SetLineWidth(double width) {
    m_actor->GetProperty()->SetLineWidth(width);
}

template <typename Filter_t>
vtkTransform *const CActor<Filter_t>::GetTransform() {
    return m_trans;
}

template <typename Filter_t>
Filter_t *const CActor<Filter_t>::GetFilter() {
    return m_tffilter;
}
