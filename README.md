# Digital Image Processing
This is the projects of course Digital Image Processing. Implementing traditional image processing methods based on CDib.h
## Env
- Cdib.h / Opencv3.4
- MFC GUI based on C++
- Visual studio 2017
##CDib
CDib is a device-irrelevant class, providing basic but necessary functions for digital image processing like image reading, writing, bit operations and etc.
## Theory
Here i implemented
1. Spatial domain operations 
   - In an image, space is a 2D plane (xy-plane). So, the spatial domain refers to the image plane itself and methods in spatial domain are based on directly modifying the value of the pixels, i.e. pixel2pixel translation.
   - Related work
     - W1-4 basic image operations  
       Grayscaling, binarization, arithmetic operation,amplification ,rotation...
     - W6-8 pixel operations  
       Linear/exponential/power opt, histogram, colorization  
       Avg, median, weighted filtering  
       Gradient sharpening method, edge enhancement operator and Laplace of Goslapuras(LoG)  
     - W12 advanced operations  
       Roberts/Sobel/Prewitt/Laplace/Canny operator
       Image segmentation: global-iterated/OSTU/
      - W15 others
2. Frequency domain
   - The frequency domain in image processing represents, at each point a particular frequency contained in the spatial domain of the image.Simplified, it is to study the change in pixel values in the image. These change in frequency is a characteristic of change in geometry of the image(spatial distribution). 
     - Edges reflects high frequency components,
     - Smooth regions have low frequency components.
   -  Related work
      - W5  
        Fourier Transform
      - W9  
        Ideal/Butterworth/Gaussian/exponential/gradient low-pass filter
      - W10  
        Ideal/Butterworth/Gaussian/exponential/gradient high-pass filter
3. Morphology method
   - Mathematical morphology (MM) is a theory and technique for the analysis and processing of geometrical structures, based on set theory, lattice theory, topology, and random functions.
     - W13  
       Region grow and Snakes：Active Contour Models
     - W14  
       Eroding and dilating/open and close
## Build
Project excutable file(.sln) is provided in Wxx. With VS2017 and MFC ENV. Simply double click the .sln file and start to build the project locally according to indications by visual studio.
## Result
PDFs: the explanation of the theory and the result of the experiments.
