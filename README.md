# Kendall

Header only implementation of the algorithm from "A Computer Method for Calculating Kendall's Tau with Ungrouped Data " by William R. Knight.  
Just take kendall.h from the include directory and it's good to go, usage:  
kendallCorrelation(vector<double> x, vector<double> y) and that's it.
If you want to run the tests or the extremely basic example in main:  
 - cmake CMakeLists.txt
 - make (this will build the example as kendall and the tests as kendallTest)  

You might need to enable c++11 to compile (and the package build-essential should probably be installed if you are using cmake).
  




