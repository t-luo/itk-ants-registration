#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

//#include "itkImageRegistrationMethod.h"
#include "itkMultiResolutionImageRegistrationMethod.h"
#include "itkAffineTransform.h"
#include "itkMeanSquaresImageToImageMetric.h"
#include "itkRegularStepGradientDescentOptimizer.h"
#include "itkLinearInterpolateImageFunction.h"
#include "itkResampleImageFilter.h"
#include <string>
#include <sstream>


int main ( int argc, char * argv[] )
{
  const unsigned int nDims = 3 ;
  typedef itk::Image < double, nDims > ImageType ;

  std::string fixedImageFilename = "../KKI2009-ALL-MPRAGE/KKI2009-21-MPRAGE.nii.gz";
  for (int i = 0; i < 20; ++i) {
    std::stringstream stream;
		stream << (i+1);

		std::string movingImageFilename = "../KKI2009-ALL-MPRAGE/KKI2009-" + stream.str() + "-MPRAGE.nii.gz";
    std::string outputRegisteredFilename = "../AffineReg/AffineReg-" + stream.str() + "-to-21.nii.gz";


    // Setup types
    //const unsigned int nDims = 3 ;
    //typedef itk::Image < int, nDims > ImageType ;
    typedef itk::ImageFileReader < ImageType > ImageReaderType ;
    typedef itk::ImageFileWriter < ImageType > ImageWriterType ;

    // read in the first image
    ImageReaderType::Pointer movingReader = ImageReaderType::New() ;
    movingReader->SetFileName ( movingImageFilename ) ;
    movingReader->Update() ;

    // read in the second image
    ImageReaderType::Pointer fixedReader = ImageReaderType::New() ;
    fixedReader->SetFileName ( fixedImageFilename ) ;
    fixedReader->Update() ;

    // Registration!

    // set up typedefs (don't forget to include the header files)
    //  typedef itk::ImageRegistrationMethod < ImageType, ImageType > RegistrationMethodType ;
    typedef itk::MultiResolutionImageRegistrationMethod < ImageType, ImageType > RegistrationMethodType ;
    typedef itk::AffineTransform < double, 3 > AffineTransformType ; // leaving at default values, could skip it
    typedef itk::MeanSquaresImageToImageMetric < ImageType, ImageType > MetricType ;
    typedef itk::RegularStepGradientDescentOptimizer OptimizerType ; // no template arguments
    typedef itk::LinearInterpolateImageFunction < ImageType > InterpolatorType ;
    typedef itk::ResampleImageFilter < ImageType, ImageType > ResampleFilterType ;

    // declare the variables
    RegistrationMethodType::Pointer registrationMethod = RegistrationMethodType::New() ;
    AffineTransformType::Pointer transform = AffineTransformType::New ();
    MetricType::Pointer metric = MetricType::New () ;
    OptimizerType::Pointer optimizer = OptimizerType::New() ;
    InterpolatorType::Pointer interpolator = InterpolatorType::New() ;

    // connect the pipeline
    registrationMethod->SetMovingImage ( movingReader->GetOutput() ) ;
    registrationMethod->SetFixedImage ( fixedReader->GetOutput() ) ;
    registrationMethod->SetOptimizer ( optimizer ) ;
    registrationMethod->SetMetric ( metric ) ;
    registrationMethod->SetInterpolator ( interpolator ) ;
    registrationMethod->SetTransform ( transform ) ;

    // set up the relevant parameters
    optimizer->MinimizeOn () ;
    optimizer->SetNumberOfIterations ( 20 ) ;

    std::cout << "Min: " << optimizer->GetMinimumStepLength () << std::endl ;
    std::cout << "Max: " << optimizer->GetMaximumStepLength () << std::endl ;
    std::cout << "Current: " << optimizer->GetCurrentStepLength () << std::endl ;

    optimizer->SetMinimumStepLength ( 0 ) ;
    optimizer->SetMaximumStepLength ( 0.125 ) ; // TODO: might want to adjust this some more
    transform->SetIdentity () ;
    registrationMethod->SetInitialTransformParameters ( transform->GetParameters() ) ;

    registrationMethod->SetNumberOfLevels ( 3 ) ;
    registrationMethod->SetFixedImageRegion ( fixedReader->GetOutput()->GetLargestPossibleRegion() ) ;

    // run the registration
    // TODO: put this in a try-catch block
    registrationMethod->Update() ;

    // why did it stop?
    std::cout << optimizer->GetStopConditionDescription () << std::endl ;

    // apply the transform we get from the registration
    ResampleFilterType::Pointer resampleFilter = ResampleFilterType::New() ;
    resampleFilter->SetInput ( movingReader->GetOutput() ) ;
    // transform that we are applying
    resampleFilter->SetTransform ( transform ) ;
    // set the grid to where the fixed image is, just in case it moved too far
    resampleFilter->SetReferenceImage ( fixedReader->GetOutput() ) ;
    resampleFilter->UseReferenceImageOn () ;

    // update
    resampleFilter->Update() ;


    // Write out the result
    ImageWriterType::Pointer writer = ImageWriterType::New() ;
    writer->SetFileName ( outputRegisteredFilename ) ;
    writer->SetInput ( resampleFilter->GetOutput() ) ; // need to figure out the argument here
    writer->Update() ;
  }




  // Done.
  return 0 ;
}
