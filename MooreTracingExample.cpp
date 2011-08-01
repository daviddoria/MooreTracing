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

// This file is provided to perform the algorithm on your own input data.

// ITK
#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkImageRegionConstIterator.h>

#include "MooreTracing.h"

int main(int argc, char *argv[])
{
  // Verify arguments
  if(argc < 2)
    {
    std::cerr << "Required arguments: inputFileName" << std::endl;
    return EXIT_FAILURE;
    }

  std::string fileName = argv[1];
  
  std::cout << "Reading: " << fileName << std::endl;
  
  typedef itk::ImageFileReader<UnsignedCharImageType> ReaderType;
 
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(fileName);
  reader->Update();
    
  std::vector< itk::Index<2> > path = MooreTrace(reader->GetOutput());
  
  for(unsigned int i = 0; i < path.size(); ++i)
    {
    std::cout << path[i] << std::endl;
    }
  
  return EXIT_SUCCESS;
}
