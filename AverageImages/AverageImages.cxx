#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkDivideImageFilter.h"
#include "itkNaryAddImageFilter.h"
#include <string>
#include <sstream>

int main (int argc, char * argv[]) {
	const unsigned int nDims = 3;
	typedef itk::Image < double, nDims > ImageType;

	typedef itk::ImageFileReader< ImageType > readerType;
	readerType::Pointer readers[20];
	typedef itk::NaryAddImageFilter< ImageType, ImageType > naddImageFilterType;

	naddImageFilterType::Pointer naddImageFilter = naddImageFilterType::New();

	for (int i = 0; i < 20; ++i) {
		std::stringstream stream;
		stream << (i+1);

		std::string filename = "../AffineReg/AffineReg-" + stream.str() + "-to-21.nii.gz";


		std::cout << filename << "\n";

		readers[i] = readerType::New();
		readers[i]->SetFileName(filename);
		readers[i]->Update();
		naddImageFilter->SetInput(i, readers[i]->GetOutput());
	}

	naddImageFilter->Update();

	typedef itk::DivideImageFilter< ImageType, ImageType, ImageType> DivideImageFilterType;
	DivideImageFilterType::Pointer divideImageFilter = DivideImageFilterType::New();
	divideImageFilter->SetInput(naddImageFilter->GetOutput());
	divideImageFilter->SetConstant(20.0);
	divideImageFilter->Update();

	typedef itk::ImageFileWriter< ImageType > writerType;
	writerType::Pointer writer = writerType::New();

	writer->SetFileName ("Average-affine.nii.gz");
	writer->SetInput(divideImageFilter->GetOutput());
	writer->Update();

	return 0;
}
