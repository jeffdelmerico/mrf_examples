// denoise.cpp
// Image Denoising

#include <stdio.h>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

// Gridcut
#include <GridCut/GridGraph_2D_4C.h>
#include <GridCut/AlphaExpansion/AlphaExpansion_2D_4C.h>

int main(int argc, char **argv)
{
  if(argc != 4)
  {
    std::cerr << "Usage: " << argv[0] << " imfile" << std::endl;
    return -1;
  }
  
  // Load Image
  cv::Mat noisy, mask;
  try
  {
    noisy = cv::imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
    mask = cv::imread(argv[2], CV_LOAD_IMAGE_GRAYSCALE);
  }
  catch(cv::Exception& e)
  {
    std::cerr << "Problem loading image: " << e.what() << std::endl;
    return -1;
  }
  int width = noisy.cols;
  int height = noisy.rows;
  int numLabels = 256; // [0,255]
  int dmax = atoi(argv[3]);

  // Show the input
  cv::namedWindow( "noisy", cv::WINDOW_NORMAL );
  cv::imshow( "noisy", noisy );
  cv::waitKey(10);
  
  // Set up data costs
  int* dataCosts = new int[width*height*numLabels];
  for(int y=0;y<height;y++)
  for(int x=0;x<width;x++)
  {    
    const int xy = x+y*width;
    for(int label=0;label<numLabels;label++)
    {     
      if (mask.at<unsigned char>(y,x) == 0)
        dataCosts[xy*numLabels+label] = 0;
      else
      {
        int diff = noisy.at<unsigned char>(y,x) - label;
        dataCosts[xy*numLabels+label] = diff*diff;
      }
    }
  }

  // Set up smoothness costs
  int* smoothnessCosts = new int[numLabels*numLabels];
  for(int label=0;label<numLabels;label++)
  for(int otherLabel=0;otherLabel<numLabels;otherLabel++)
  {
    const int lab = otherLabel + label*numLabels;
    int diff = label - otherLabel;
    smoothnessCosts[lab] = std::min(diff*diff, dmax*dmax);
    smoothnessCosts[lab] = std::min(diff*diff, dmax*dmax);
  }
  
  // Solve maxflow/mincut
  typedef AlphaExpansion_2D_4C<unsigned char,int,int> Expansion; 
  Expansion* expansion = new Expansion(width,height,numLabels,dataCosts,smoothnessCosts);
  expansion->perform();
  unsigned char* labeling = expansion->get_labeling();
  
  cv::Mat denoised(height, width, CV_8UC1);
  for(int y=0;y<height;y++)
  for(int x=0;x<width;x++)
  {    
    const int xy = x+y*width;
    denoised.at<unsigned char>(y,x) = labeling[xy];
  }

  // Show the output
  cv::namedWindow( "denoised", cv::WINDOW_NORMAL );
  cv::imshow( "denoised", denoised );
  cv::waitKey(0);

  delete expansion; 

  return 0;
}
