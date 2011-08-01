/*=========================================================================
 *
 *  Copyright David Doria 2011 daviddoria@gmail.com
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/

// This file is provided for the sole purpose of creating an image to demonstrate the pixel
// ordering for the associated article. It creates a black image with the outline of a white square
// and then traces the square to obtain the ordering. The output is a color image with a Hot colormap
// applied to the pixel order image.

// ITK
#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <itkImageRegionConstIterator.h>
#include <itkRescaleIntensityImageFilter.h>
#include <itkScalarToRGBColormapImageFilter.h>

#include "MooreTracing.h"

void CreateImage(UnsignedCharImageType::Pointer image);

int main(int argc, char *argv[])
{
  UnsignedCharImageType::Pointer image = UnsignedCharImageType::New();
  CreateImage(image);
  
  typedef  itk::ImageFileWriter< UnsignedCharImageType  > InputWriterType;
  InputWriterType::Pointer inputWriter = InputWriterType::New();
  inputWriter->SetFileName("input.png");
  inputWriter->SetInput(image);
  inputWriter->Update();
  
  // Prepare the output image
  UnsignedCharImageType::Pointer output = UnsignedCharImageType::New();
  CreateImage(output);
  
  std::vector< itk::Index<2> > path = MooreTrace(image);
  
  for(unsigned int i = 0; i < path.size(); ++i)
    {
    output->SetPixel(path[i], i);
    std::cout << "Set pixel " << path[i] << " to " <<  i << std::endl;
  
    }
    
  typedef itk::RescaleIntensityImageFilter< UnsignedCharImageType, UnsignedCharImageType > RescaleFilterType;
  RescaleFilterType::Pointer rescaleFilter = RescaleFilterType::New();
  rescaleFilter->SetInput(output);
  rescaleFilter->SetOutputMinimum(0);
  rescaleFilter->SetOutputMaximum(255);
  rescaleFilter->Update();
  
  typedef itk::RGBPixel<unsigned char>    RGBPixelType;
  typedef itk::Image<RGBPixelType, 2>  RGBImageType;
  typedef itk::ScalarToRGBColormapImageFilter<UnsignedCharImageType, RGBImageType> RGBFilterType;
  RGBFilterType::Pointer rgbfilter = RGBFilterType::New();
  rgbfilter->SetInput(rescaleFilter->GetOutput());
  rgbfilter->SetColormap( RGBFilterType::Hot );
  rgbfilter->Update();
 
  typedef  itk::ImageFileWriter< RGBImageType > OutputWriterType;
  OutputWriterType::Pointer outputWriter = OutputWriterType::New();
  outputWriter->SetFileName("output.png");
  outputWriter->SetInput(rgbfilter->GetOutput());
  outputWriter->Update();
  
  return EXIT_SUCCESS;
}

void CreateImage(UnsignedCharImageType::Pointer image)
{
  itk::Index<2> start;
  start.Fill(0);
 
  itk::Size<2> size;
  size.Fill(100);
 
  itk::ImageRegion<2> region(start,size);
 
  image->SetRegions(region);
  image->Allocate();
  image->FillBuffer(0);
 
  itk::ImageRegionConstIterator<UnsignedCharImageType> imageIterator(image, image->GetLargestPossibleRegion());
 
  // Draw a square
  for(unsigned int i = 40; i < 60; ++i)
    {
    itk::Index<2> pixel;
    pixel[0] = 40;
    pixel[1] = i;
    image->SetPixel(pixel, 255);
 
    pixel[0] = i;
    pixel[1] = 40;
    image->SetPixel(pixel, 255);
 
    pixel[0] = i;
    pixel[1] = 60;
    image->SetPixel(pixel, 255);
 
    pixel[1] = i;
    pixel[0] = 60;
    image->SetPixel(pixel, 255);
    }
    
}
