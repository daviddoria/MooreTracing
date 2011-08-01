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


// ITK
#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkImageRegionConstIterator.h>

#include "MooreTracing.h"

void CreateImage(UnsignedCharImageType::Pointer image);
void TestOffsetOrder();
void TestFirstPixel();

int main(int argc, char *argv[])
{
//   // Verify arguments
//   if(argc < 4)
//     {
//     std::cerr << "Required arguments: inputFileName outputFileName straightnessErrorTolerance" << std::endl;
//     return EXIT_FAILURE;
//     }

  //TestOffsetOrder();
  
  //TestFirstPixel();
  
  UnsignedCharImageType::Pointer image = UnsignedCharImageType::New();
  CreateImage(image);
  
  std::vector< itk::Index<2> > path = MooreTrace(image);
  
  for(unsigned int i = 0; i < path.size(); ++i)
    {
    std::cout << path[i] << std::endl;
    }
  
  return EXIT_SUCCESS;
}


void TestOffsetOrder()
{
  {
  itk::Offset<2> firstOffset;
  firstOffset[0] = -1;
  firstOffset[1] = -1;
  std::vector< itk::Offset<2> > orderedOffsets = GetOrderedOffsets(firstOffset);
  
  for(unsigned int i = 0; i < orderedOffsets.size(); ++i)
    {
    std::cout << orderedOffsets[i] << std::endl;
    }
  }
  
  std::cout << std::endl;
  
  {
  itk::Offset<2> firstOffset;
  firstOffset[0] = 0;
  firstOffset[1] = 1;
  std::vector< itk::Offset<2> > orderedOffsets = GetOrderedOffsets(firstOffset);
  
  for(unsigned int i = 0; i < orderedOffsets.size(); ++i)
    {
    std::cout << orderedOffsets[i] << std::endl;
    }
  } 
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

void TestFirstPixel()
{
  UnsignedCharImageType::Pointer image = UnsignedCharImageType::New();
  CreateImage(image);
  
  itk::Offset<2> backtrack;
  itk::Index<2> firstPixel = FindFirstPixel(image, backtrack);
  
  std::cout << "first pixel: " << firstPixel << std::endl;
  std::cout << "backtrack: " << backtrack << std::endl;
}
