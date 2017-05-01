using namespace std;

#include <iostream>
#include "include/kendall.h"

int main()
{
  vector<double> a;
  vector<double> b;
  for(int i = 0; i < 1000; i++)
  {
    a.push_back(i);
    b.push_back(-i);
  }
  cout << kendallCorrelation(a,b) << endl;


  return 0;
}
