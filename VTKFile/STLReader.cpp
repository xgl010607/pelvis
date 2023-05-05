#include "STLReader.h"
#include "vtkProperty.h"
#include "vtkMatrix4x4.h"
#include "vtkSTLWriter.h"

CSTLReader::CSTLReader(void) {
    pdreader = vtkSTLReader::New();
    normals  = vtkPolyDataNormals::New();
    m_polydata = vtkSmartPointer<vtkPolyData>::New();
    m_polydata = pdreader->GetOutput();

   
    normals->SetInputConnection(pdreader->GetOutputPort());
    //normals->SetFeatureAngle(60);
    m_tffilter->SetInputConnection(normals->GetOutputPort());
    m_tffilter->SetTransform(m_trans);
    m_actor->SetUserTransform(NULL);
    m_pdmapper->SetInputConnection(m_tffilter->GetOutputPort());
	m_actor->GetProperty()->SetOpacity(1);
}

void CSTLReader::SetFileName(std::string filename) {
    pdreader->SetFileName(filename.c_str());
    pdreader->Update();
}

void CSTLReader::Modified() {
    pdreader->Modified();
}

int CSTLReader::GetNumberOfPoints() {
    return pdreader->GetOutput()->GetPoints()->GetNumberOfPoints();
}
vtkPolyData* CSTLReader::GetPoly() {
    
    return m_polydata;
}
