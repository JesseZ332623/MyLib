#include "../../cinCheck.h"

void testInputA(void);
void testInputB(void);
void testInputC(void);

using namespace MyLib::cinCheck;
using namespace MyLib::MyLoger;

int main(int argc, char const *argv[])
{
    system("cls");

    testInputA();
    testInputB();
    testInputC();

    eatLine();

    DONE

    return EXIT_SUCCESS;
}

void testInputA(void)
{
    int number = 0;
    MessageStrings messages = 
    {
        "Please a number: ", 
        "Invalid input! Please only enter intenger!\n", 
        "Out of range! (0 < number <= 100)\n"
    };

    istreamInputAndCheck(
                            std::cin, number, messages, 
                            [&number]() { return (number > 0 && number <= 100);}
                        );
    
    CORRECT_LOG("Test Input A Pass!\n");
    printSplitLine(messages.outRangeMessage.size(), '-');
}

void testInputB(void)
{
    long double number = 0.0L;
    MessageStrings messages = 
    {
        "Please a number: ", 
        "Invalid input! Please only enter number!\n", 
        "Out of range! (114514.1919810 < number <= 1919810.1141514)\n"
    };

    istreamInputAndCheck(
                            std::cin, number, messages, 
                            [&number]() { return (number > 114514.1919810L && number <= 1919810.114514L);}
                        );

    CORRECT_LOG("Test Input B Pass!\n");
    printSplitLine(messages.outRangeMessage.size(), '-');
}

void testInputC(void)
{
    std::size_t number = 0ULL;
    constexpr std::size_t maxRange = (std::size_t)(ULLONG_MAX / 114154);

    MessageStrings messages = 
    {
        "Please a number: ", 
        "Invalid input! Please only enter number!\n", 
        "Out of range! (0 < number <= " + std::to_string(maxRange) + ")\n"
    };

    istreamInputAndCheck(
                            std::cin, number, messages, 
                            [&number]() { return (number > 0 && number <= maxRange);}
                        );

    CORRECT_LOG("Test Input C Pass!\n");
    printSplitLine(messages.outRangeMessage.size(), '-');
}