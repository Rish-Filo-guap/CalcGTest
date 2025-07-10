#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <climits>
#include "SimpleCalculator.h"
#include "InMemoryHistory.h"
#include "IHistory.h"
#include "OperationsTests.h"
#include "MockHistoryTests.h"
using namespace std;

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  ::testing::InitGoogleMock(&argc, argv);

  cout<<endl<< "operations tests:"<<endl<<calcTest::RunOperationsTests();
  cout<<endl<<"mock history tests:"<<endl<<calcTest::RunMockHistoryTests();
  //cout<< RUN_ALL_TESTS();
  return 0;

}