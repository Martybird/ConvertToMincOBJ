#include <vtkSmartPointer.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkOBJReader.h>
#include <vtkUnstructuredGrid.h>
#include <vtkCell.h>
#include <vtkCellArray.h>
#include <vtkIdList.h>
#include <vtkUnsignedCharArray.h>
#include <vtkPointData.h>
#include <vtkMNIObjectWriter.h>
#include <string>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkPolyData.h>

int main(int argc, char *argv[])
{
  // Parse command line arguments
  if(argc != 3)
    {
    std::cout << "Usage: " << argv[0] << " WavefrontInput(.obj) MNIOutput(.obj)" << std::endl;
    return EXIT_FAILURE;
    }

  std::string filename = argv[1];
  std::string output = argv[2];

  vtkSmartPointer<vtkOBJReader> reader =
    vtkSmartPointer<vtkOBJReader>::New();
  reader->SetFileName(filename.c_str());
  reader->Update();


// set up the transformation to scale the obj x 10 uniformally 

 vtkSmartPointer<vtkTransform> scale =
    vtkSmartPointer<vtkTransform>::New();


  scale->Scale(10.0, 10.0, 10.0); // dimension difference between object

  vtkSmartPointer<vtkTransformPolyDataFilter> transformFilter =
    vtkSmartPointer<vtkTransformPolyDataFilter>::New();
  transformFilter->SetInputConnection(reader->GetOutputPort());
  transformFilter->SetTransform(scale);
  transformFilter->Update();


  // connect the port to MNI Object conversion

  vtkSmartPointer<vtkMNIObjectWriter> writer = 
  vtkSmartPointer<vtkMNIObjectWriter>::New(); 
  writer->SetFileName(output.c_str()); 
  writer->SetInputConnection(transformFilter->GetOutputPort()); 
  writer->Write(); 


  // Visualize the object

  vtkSmartPointer<vtkPolyDataMapper> mapper =
    vtkSmartPointer<vtkPolyDataMapper>::New();
  mapper->SetInputConnection(reader->GetOutputPort());

  vtkSmartPointer<vtkActor> actor =
    vtkSmartPointer<vtkActor>::New();
  actor->SetMapper(mapper);

  vtkSmartPointer<vtkRenderer> renderer =
    vtkSmartPointer<vtkRenderer>::New();

  vtkSmartPointer<vtkRenderWindow> renderWindow =
    vtkSmartPointer<vtkRenderWindow>::New();
  renderWindow->AddRenderer(renderer);


  vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
    vtkSmartPointer<vtkRenderWindowInteractor>::New();
  renderWindowInteractor->SetRenderWindow(renderWindow);


  renderer->AddActor(actor);
  renderer->SetBackground(.3, .6, .3); // Background color green

  renderWindow->Render();
  renderWindowInteractor->Start();
  



  return EXIT_SUCCESS;
}
