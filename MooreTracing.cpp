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

#include "MooreTracing.h"

#include "itkOffset.h"
#include "itkImageRegionConstIterator.h"

std::vector< itk::Offset<2> > GetAllOffsets()
{
  // In ITK, Offset<2> is indexed with (row, column), so the following
  // sets up the 8-neighbor indices in counter-clockwise order.
  std::vector< itk::Offset<2> > offsets;
  
  itk::Offset<2> offset;
  offset[0] = -1; offset[1] = -1;
  offsets.push_back(offset);
  offset[0] = -1; offset[1] = 0;
  offsets.push_back(offset);
  offset[0] = -1; offset[1] = 1;
  offsets.push_back(offset);
  offset[0] = 0; offset[1] = 1;
  offsets.push_back(offset);
  offset[0] = 1; offset[1] = 1;
  offsets.push_back(offset);
  offset[0] = 1; offset[1] = 0;
  offsets.push_back(offset);
  offset[0] = 1; offset[1] = -1;
  offsets.push_back(offset);
  offset[0] = 0; offset[1] = -1;
  offsets.push_back(offset);
  
  return offsets;
}

std::vector< itk::Offset<2> > GetOrderedOffsets(itk::Offset<2> firstOffset)
{
  std::vector< itk::Offset<2> > orderedOffsets;
  
  std::vector< itk::Offset<2> > allOffsets = GetAllOffsets();
  
  // Find the starting point
  unsigned int startingOffset = 0;
  for(unsigned int i = 0; i < allOffsets.size(); ++i)
    {
    if(allOffsets[i] == firstOffset)
      {
      startingOffset = i;
      break;
      }
    }
    
  // Add the reamining offsets to the end of the list
  for(unsigned int i = startingOffset + 1; i < allOffsets.size(); ++i)
    {
    orderedOffsets.push_back(allOffsets[i]);
    }
    
  // Add the reamining offsets from the beginning of the list
  for(unsigned int i = 0; i < startingOffset; ++i)
    {
    orderedOffsets.push_back(allOffsets[i]);
    }
  
  return orderedOffsets;
}

std::vector< itk::Index<2> > MooreTrace(UnsignedCharImageType::Pointer image)
{
  std::vector< itk::Index<2> > path;
  
  itk::Offset<2> backtrack;
  itk::Index<2> firstPixel = FindFirstPixel(image, backtrack);

  itk::Index<2> currentPixel = firstPixel;
    
  do
  {
    path.push_back(currentPixel);
    currentPixel = FindNextPixel(image, currentPixel, backtrack);
    //std::cout << "Current pixel: " << currentPixel << " with backtrack: " << backtrack << std::endl;
  } while(currentPixel != firstPixel);
  
  // Close the loop
  path.push_back(firstPixel);
  
  return path;
}

itk::Index<2> FindNextPixel(UnsignedCharImageType::Pointer image, itk::Index<2> currentPixel, itk::Offset<2>& backtrack)
{
  // The 'backtrack' input has two uses. First, it is used to know where to start the traversal. Second, it returns the next backtrack position by reference.
  
  itk::Offset<2> startingOffset = backtrack;
  
  std::vector< itk::Offset<2> > orderedOffsets = GetOrderedOffsets(startingOffset);
  for(unsigned int i = 0; i < orderedOffsets.size(); ++i)
    {
    if(image->GetPixel(currentPixel + orderedOffsets[i]))
      {
      if(i != 0)
	{
	backtrack = (currentPixel + orderedOffsets[i-1]) - (currentPixel + orderedOffsets[i]);
	}
      else
	{
	backtrack = (currentPixel + startingOffset) - (currentPixel + orderedOffsets[i]);
	}

      return currentPixel + orderedOffsets[i];
      }
    }
  std::cerr << "No next pixel - this means there was a pixel that is not connected to anything!" << std::endl;
  exit(-1);
  
  // This is just so there are no warnings about no return value, it should never be run
  itk::Index<2> zeroPixel;
  zeroPixel.Fill(0);
  return zeroPixel;
}

itk::Index<2> FindFirstPixel(UnsignedCharImageType::Pointer image, itk::Offset<2>& backtrack)
{
  // Perform a raster scan until a non-zero pixel is reached
  itk::ImageRegionConstIterator<UnsignedCharImageType> imageIterator(image, image->GetLargestPossibleRegion());
  itk::Index<2> previousIndex;
  itk::Index<2> firstPixelIndex;
  
  while(!imageIterator.IsAtEnd())
    {
    // Get the value of the current pixel
    if(imageIterator.Get())
      {
      firstPixelIndex = imageIterator.GetIndex();
      break;
      }
    
    previousIndex = imageIterator.GetIndex();
    ++imageIterator;
    }
    
  // Return the backtrack pixel by reference
  backtrack = previousIndex - firstPixelIndex;
  
  return firstPixelIndex;
}
