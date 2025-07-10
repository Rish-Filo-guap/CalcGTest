#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <climits>
#include "SimpleCalculator.h"
#include "InMemoryHistory.h"
#include "IHistory.h"
#include "OperationsTests.h"
using namespace std;
namespace calcTest
{
    class CalcTests : public ::testing::Test
    {
    public:
        static void SetUpTestSuite()
        {
            cout << "Preparing test environment..." << endl;
        }
        void SetUp() override
        {
            c = make_unique<calc::SimpleCalculator>(m);
        }
        void TearDown() override
        {
            c.reset();
            m=calc::InMemoryHistory();
        }
        calc::InMemoryHistory m = calc::InMemoryHistory();
        unique_ptr<calc::SimpleCalculator> c;
        static void TearDownTestSuite()
        {
            cout << "Cleaning up..." << endl;
        }
    };
    
    TEST_F(CalcTests, AddBoundaryValue)// проверка граничных значений при сложении
    {
        EXPECT_EQ(c->Add(INT32_MAX - 1, 1), INT32_MAX);
        EXPECT_EQ(c->Add(INT32_MAX, 1), INT32_MIN); //с точки зрения математики результат неверный, однако в рамках переполнения переменных все логично
        EXPECT_EQ(c->Add(INT32_MAX, 0), INT32_MAX);

        EXPECT_EQ(c->Add(INT32_MIN, INT32_MAX), -1);

        EXPECT_EQ(c->Add(INT32_MIN, INT32_MIN), 0);
        EXPECT_EQ(c->Add(INT32_MIN, 0), INT32_MIN);
        EXPECT_EQ(c->Add(INT32_MIN, 1), INT32_MIN + 1);
    }

    TEST_F(CalcTests, SubstractBoundaryValue) // проверка граничных значений при вичитании
    {
        EXPECT_EQ(c->Subtract(INT32_MIN + 1, 1), INT32_MIN);
        EXPECT_EQ(c->Subtract(INT32_MIN, 1), INT32_MAX); //с точки зрения математики результат неверный, однако в рамках переполнения переменных все логично
        EXPECT_EQ(c->Subtract(INT32_MIN, 0), INT32_MIN);
        EXPECT_EQ(c->Subtract(INT32_MIN, INT32_MIN), 0);

        EXPECT_EQ(c->Subtract(INT32_MIN, INT32_MAX), 1); //с точки зрения математики результат неверный, однако в рамках переполнения переменных все логично

        EXPECT_EQ(c->Subtract(INT32_MAX, INT32_MAX), 0);
        EXPECT_EQ(c->Subtract(INT32_MIN, 0), INT32_MIN);

        EXPECT_EQ(c->Subtract(INT32_MAX, 1), INT32_MAX - 1);
    }
    TEST_F(CalcTests, MultiplyBoundaryValue) // проверка граничных значений при умножении
    {
        EXPECT_EQ(c->Multiply(INT32_MAX, 1), INT32_MAX);
        EXPECT_EQ(c->Multiply(INT32_MAX, 0), 0);
        EXPECT_EQ(c->Multiply(INT32_MIN, 1), INT32_MIN);
        EXPECT_EQ(c->Multiply(INT32_MIN, 0), 0);
    }
    TEST_F(CalcTests, DivideBoundaryValue) // проверка граничных значений при делении
    {
        EXPECT_EQ(c->Divide(INT32_MAX, 1), INT32_MAX);
        EXPECT_EQ(c->Divide(INT32_MAX, 2), INT32_MAX >> 1);
        EXPECT_EQ(c->Divide(INT32_MAX, INT32_MAX), 1);

        EXPECT_EQ(c->Divide(INT32_MIN, 1), INT32_MIN);
        EXPECT_EQ(c->Divide(INT32_MIN, 2), INT32_MIN >> 1);
        EXPECT_EQ(c->Divide(INT32_MIN, INT32_MIN), 1);

    }
    TEST_F(CalcTests, DivideMultiplyWithZero) // проверка значений делении или умножении с 0
    {
        EXPECT_EQ(c->Multiply(1, 0), 0);
        EXPECT_EQ(c->Multiply(0, 1), 0);
        EXPECT_EQ(c->Multiply(0, 0), 0);

        EXPECT_EQ(c->Divide(0, 1), 0);
        EXPECT_THROW(c->Divide(1, 0), invalid_argument);
        EXPECT_THROW(c->Divide(0, 0), invalid_argument);
    }
    TEST_F(CalcTests, DivideNotInt) // проверка значений при дробном результате деления (происходит целочисленное деление с округлением вниз)
    {
        EXPECT_EQ(c->Divide(3, 2), 1);
        EXPECT_EQ(c->Divide(2, 3), 0);
    }
    int RunOperationsTests()
    {
        return RUN_ALL_TESTS();
    }
    TEST_F(CalcTests, IsHistoryContentCorrect){ // проверка правильности сообщения в истории
        c->Add(2,2);
        auto result=m.GetLastOperations(1)[0];
        ASSERT_EQ(result, "2 + 2 = 4");
    }
    TEST_F(CalcTests, IsHistorySizeCorrect){ // проверка размера истории
        c->Add(2,2);
        c->Add(2,2);
        auto result=m.GetLastOperations(2);
        ASSERT_EQ(result.size(), 2);
    }
    TEST_F(CalcTests, IsHistorySizeCorrectSmaller){ // проверка размера истории при недостатке операций
        c->Add(2,2);
        auto result=m.GetLastOperations(2);
        ASSERT_EQ(result.size(), 1);
    }

} // calcTest