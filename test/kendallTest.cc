#include <vector>
#include <algorithm>
#include "../include/gtest/gtest.h"
#include "../include/gtest/gtest-spi.h"
#include "../include/kendall.h"

using namespace std;

/**
 * @brief Expected result of the algorithm given the number of pairs, number
 * of pairs with tied X, number of pairs with tied Y and number of identical
 * pairs, doesn't take discordant pairs in consideration and shouldn't be used
 * in tests that have them.
 *
 * @param n
 * @param sameX
 * @param sameY
 * @param sameXY
 *
 * @return 
 */
static double expectedResult(double n, double sameX, double sameY, double sameXY)
{
    double totalPairs = (n * (n - 1.))/2.;
    double num = totalPairs - sameX - sameY + sameXY;
    double den = sqrt((totalPairs - sameX) * (totalPairs - sameY));
    return num/den;
}

/**
 * @brief Gets the sign of a value.
 *
 * @tparam T
 * @param val
 *
 * @return 
 */
template <typename T> static int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

//O(n^2) method that doesn't include ties
static  double lazyMethod(vector<double> x, vector<double> y)
{
    int n = x.size();
    double totalPairs = (n * (n - 1.))/2.;
    double res = 0;
    for(unsigned long i = 0; i < x.size(); i++)
        for(unsigned long u = i+1; u < x.size(); u++)
            res += sgn(x[i] - x[u]) * sgn(y[i]- y[u]);
    return res/totalPairs;
}


TEST(testWithElements, test1Element)
{
  vector<double> x,y;
  x.push_back(1.);
  y.push_back(1.);
  EXPECT_EQ(kendallCorrelation(x, y), 1.0);
}

TEST(testNoTies, testNoTiesFullyConcordant)
{
  for(int n = 100; n < 100000; n *= 10)
  {
      vector<double> x;
      vector<double> y;

      for(double i = 0; i < n ; i++)
      {
          x.push_back(i);
          y.push_back(i);
      }
      EXPECT_EQ(1.0, kendallCorrelation(x, y));
  }
}

TEST(testNoTies, testNoTiesFullyDiscordant)
{

  for(int n = 10; n < 100000; n *= 10)
  {
      vector<double> x;
      vector<double> y;

      for(double i = 0; i < n ; i++)
      {
          x.push_back(n-i);
          y.push_back(i);
      }
      EXPECT_EQ(-1.0, kendallCorrelation(x, y));
  }
}
    
TEST(testNoTies, testNoTies0Correlation)
{
  vector<double> x;
  vector<double> y;
  x.push_back(0.);
  x.push_back(1.);
  x.push_back(2.);
  x.push_back(3.);
  y.push_back(0.);
  y.push_back(1.);
  y.push_back(0.8);
  y.push_back(0.7);

  EXPECT_EQ(0., kendallCorrelation(x, y));
}
    
TEST(testNoTies, testNoTiesPositiveCorrelation)
{
  vector<double> x;
  vector<double> y;
  x.push_back(0.);
  x.push_back(1.);
  x.push_back(2.);
  x.push_back(3.);
  y.push_back(0.);
  y.push_back(1.);
  y.push_back(0.8);
  y.push_back(0.9);

  EXPECT_EQ(2./6., kendallCorrelation(x, y));
}

TEST(testNoTies, testNoTiesNegativeCorrelation)
{
  vector<double> x;
  vector<double> y;
  x.push_back(0.);
  x.push_back(1.);
  x.push_back(2.);
  x.push_back(3.);
  y.push_back(0.);
  y.push_back(1.);
  y.push_back(0.8);
  y.push_back(-0.5);

  EXPECT_EQ(-2./6., kendallCorrelation(x, y));
}

TEST(testNoTies, testShuffledNoTies)
{
  for(int n = 10; n < 100000; n *= 10)
  {
      vector<double> x;
      vector<double> y;
      
      for(double i = 0; i < n ; i++)
      {
          x.push_back(i);
          y.push_back(i);
      }
      
      std::random_device rd;
      std::mt19937 g(rd());
      shuffle(x.begin(), x.end(), g);
      shuffle(y.begin(), y.end(), g);

      EXPECT_EQ(lazyMethod(x, y), kendallCorrelation(x, y));
  }
}

TEST(testWithTies, testSameX)
{
  for(int n = 10; n < 100000; n *= 10)
  {
      vector<double> x;
      vector<double> y;

      for(double i = 0; i < n ; i++)
      {
          x.push_back(0.);
          y.push_back(i);
      }
      EXPECT_EQ(0., kendallCorrelation(x, y));
  }
}

TEST(testWithTies, testSameY)
{
  for(int n = 10; n < 100000; n *= 10)
  {
    vector<double> x;
    vector<double> y;

    for(double i = 0; i < n ; i++)
    {
        x.push_back(i);
        y.push_back(0.);
    }
    EXPECT_EQ(0., kendallCorrelation(x, y));
  }
}
    
TEST(testWithTies, testSameXY)
{
  for(int n = 10; n < 100000; n *= 10)
  {
      vector<double> x;
      vector<double> y;
      for(double i = 0; i < n; i++)
      {
          x.push_back(0.);
          y.push_back(0.);
      }
      EXPECT_EQ(1., kendallCorrelation(x, y));
  }
}
    
TEST(testWithTies, testSomeSameX)
{
  for(int n = 10; n < 100000; n *= 10)
  {
      vector<double> x;
      vector<double> y;

      for(double i = 0; i < n/2; i++)
      {
          x.push_back(-1.);
          y.push_back(i);
      }
      
      for(double i = n/2; i < n; i++)
      {
          x.push_back(i);
          y.push_back(i);
      }

      EXPECT_EQ(expectedResult(n, (n/2. *(n/2. - 1.)/2), 0, 0), kendallCorrelation(x, y));
  }
}

TEST(testWithTies, testSomeSameY)
{
  for(int n = 10; n < 100000; n *= 10)
  {
      vector<double> x;
      vector<double> y;

      for(double i = 0; i < n/2; i++)
      {
          y.push_back(-1.);
          x.push_back(i);
      }
      for(double i = n/2; i < n; i++)
      {
          x.push_back(i);
          y.push_back(i);
      }

      EXPECT_EQ(expectedResult(n, 0, (n/2. *(n/2. - 1.)/2), 0), kendallCorrelation(x, y));
  }
}

TEST(testWithTies, testSomeSameXY)
{
  for(int n = 10; n < 100000; n *= 10)
  {
      vector<double> x;
      vector<double> y;

      for(int i = 0; i < n/2; i++)
      {
          y.push_back(-1.);
          x.push_back(-1.);
      }
      for(double i = n/2; i < n; i++)
      {
          x.push_back(i);
          y.push_back(i);
      }

      EXPECT_EQ(expectedResult(n, (n/2. *(n/2. - 1.)/2),
              (n/2. *(n/2. - 1.)/2), (n/2. *(n/2. - 1.)/2)),
              kendallCorrelation(x, y));
  }
}
