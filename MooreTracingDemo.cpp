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

// This file is provided to demonstrate the interface to the code, and to show that it works
// on known input data.

// ITK
#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkImageRegionConstIterator.h>

#include "MooreTracing.h"

void CreateImage(UnsignedCharImageType::Pointer image);

int main(int argc, char *argv[])
{
  UnsignedCharImageType::Pointer image = UnsignedCharImageType::New();
  CreateImage(image);
  
  std::vector< itk::Index<2> > path = MooreTrace(image);
  
  for(unsigned int i = 0; i < path.size(); ++i)
    {
    std::cout << path[i] << std::endl;
    }
  
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
