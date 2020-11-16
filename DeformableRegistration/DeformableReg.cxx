#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

#include "itkImageRegionIterator.h"

#include "itkDemonsRegistrationFilter.h"
#include "itkHistogramMatchingImageFilter.h"
#include "itkCastImageFilter.h"
#include "itkResampleImageFilter.h"
#include "itkDisplacementFieldTransform.h"
#include "itkLinearInterpolateImageFunction.h"

#include <string>
#include <sstream>


int main ( int argc, char * argv[] )
{
  const unsigned int nDims = 3 ;
  typedef itk::Image< double, nDims > ImageType ;

  typedef itk::ImageFileReader < ImageType > ImageReaderType ;
  typedef itk::ImageFileWriter < ImageType > ImageWriterType ;

  int iterations = strtol(argv[1], NULL, 10);

  std::stringstream stream;
  stream << argv[2]; // image number

  std::stringstream iterStream;
  iterStream << argv[1];

  std::string movingImageFilename = argv[3];
  std::string fixedImageFilename = "../AffineReg/affine-template.nii.gz";
  std::string outputRegisteredFilename = "../DeformReg/DeformReg-" + stream.str()
                                          + "-" + iterStream.str() + ".nii.gz";

  // read in the first image
  ImageReaderType::Pointer movingReader = ImageReaderType::New() ;
  movingReader->SetFileName ( movingImageFilename ) ;
  movingReader->Update() ;
  ImageType::Pointer movingImage = movingReader->GetOutput() ;

  // read in the second image
  ImageReaderType::Pointer fixedReader = ImageReaderType::New() ;
  fixedReader->SetFileName ( fixedImageFilename ) ;
  fixedReader->Update() ;
  ImageType::Pointer fixedImage = fixedReader->GetOutput() ;

  typedef itk::HistogramMatchingImageFilter < ImageType, ImageType > MatchType ;
  MatchType::Pointer matcher = MatchType::New() ;

  matcher->SetInput( movingImage ) ;
  matcher->SetReferenceImage( fixedImage ) ;
  matcher->SetNumberOfHistogramLevels( 1024 ) ;
  matcher->SetNumberOfMatchPoints( 7 ) ;
  matcher->ThresholdAtMeanIntensityOn() ;

  typedef itk::Vector < double, nDims > Vector ;
  typedef itk::Image < Vector, nDims > DisType ;
  typedef itk::DemonsRegistrationFilter < ImageType, ImageType, DisType > DemonsFilter ;
  DemonsFilter::Pointer filter = DemonsFilter::New() ;

  filter->SetFixedImage( fixedImage ) ;
  filter->SetMovingImage( matcher->GetOutput() ) ;
  filter->SetNumberOfIterations( iterations ) ;
  filter->SetStandardDeviations( 1.0 ) ;
  filter->Update() ;


  typedef itk::ResampleImageFilter < ImageType, ImageType > ResampleType ;
  typedef itk::DisplacementFieldTransform < double, nDims > DisplacementType ;
  typedef itk::LinearInterpolateImageFunction < ImageType, double > LinearType ;

  LinearType::Pointer linear = LinearType::New() ;
  DisplacementType::Pointer transform = DisplacementType::New() ;
  ResampleType::Pointer resampleFilter = ResampleType::New() ;

  transform->SetDisplacementField( filter->GetOutput() ) ;

  resampleFilter->SetInput( movingImage ) ;
  resampleFilter->SetTransform( transform ) ;
  resampleFilter->SetInterpolator( linear ) ;
  resampleFilter->SetSize( movingImage->GetLargestPossibleRegion().GetSize() ) ;
  resampleFilter->SetReferenceImage( fixedImage ) ;
  resampleFilter->UseReferenceImageOn() ;
  resampleFilter->Update() ;


  ImageWriterType::Pointer writer = ImageWriterType::New() ;
  writer->SetFileName( outputRegisteredFilename ) ;
  writer->SetInput( resampleFilter->GetOutput() ) ;
  writer->Update() ;

  return 0;

}
