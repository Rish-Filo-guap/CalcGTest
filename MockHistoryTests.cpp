#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <climits>
#include "SimpleCalculator.h"
#include "InMemoryHistory.h"
#include "IHistory.h"
#include "OperationsTests.h"
#include "MockHistoryTests.h"
using namespace std;
namespace calcTest
{

    class IHistoryMock : public calc::IHistory
    {
    public:
        MOCK_METHOD(void, AddEntry, (const std::string &operation), (override));
        MOCK_METHOD(std::vector<std::string>, GetLastOperations, (size_t count), (const, override));
    };
    class CalcHistoryTest : public ::testing::Test
    {
    public:
        IHistoryMock m_history;
        calc::SimpleCalculator m_c{m_history};
    };
    TEST_F(CalcHistoryTest, BasicAddition)//проверка правильности данных передаваемых калькулятором
    {
        EXPECT_CALL(m_history, AddEntry("2 + 2 = 4"));
        ASSERT_EQ(m_c.Add(2, 2), 4);
        
        EXPECT_CALL(m_history, AddEntry("2 * 2 = 4"));
        ASSERT_EQ(m_c.Multiply(2, 2), 4);
        
        EXPECT_CALL(m_history, AddEntry("2 / 2 = 1"));
        ASSERT_EQ(m_c.Divide(2, 2), 1);
        
        EXPECT_CALL(m_history, AddEntry("2 - 2 = 0"));
        ASSERT_EQ(m_c.Subtract(2, 2), 0);

    }

    TEST_F(CalcHistoryTest, ErrorDivision)//проверка записи в истории при выполнении некорректной операции
    {
        EXPECT_CALL(m_history, AddEntry("2 / 0 : Division by zero"));
        EXPECT_THROW(m_c.Divide(2, 0), invalid_argument);
    }
    TEST_F(CalcHistoryTest, GetOperations){//проверка запроса нескольких операций
        EXPECT_CALL(m_history, GetLastOperations(2)).WillOnce(::testing::Return(vector<string>{"2 - 3 = -1", "1 + 1 = 2"}));
        auto history = m_history.GetLastOperations(2);
        ASSERT_EQ(history.size(),2);
    }

    int RunMockHistoryTests()
    {
        return RUN_ALL_TESTS();
    }
} // calcTest