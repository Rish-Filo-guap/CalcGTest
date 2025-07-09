#include <gtest/gtest.h>
#include <gmock/gmock.h>

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  ::testing::InitGoogleMock(&argc, argv);
  std::cout<<"sdf"<<std::endl;
  
  return RUN_ALL_TESTS();
}