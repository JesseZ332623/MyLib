#ifndef _TERMINAL_COLOR_H_
#define _TERMINAL_COLOR_H_

#include <ostream>

enum ColorMenu { RED = 1, GREEN, YELLO, BLUE = 6, WHITE };

class TerminalTextColor
{
    private:
        int colorCode {0};

    public:
        TerminalTextColor(const int __colorSign) : colorCode(__colorSign) {}

        friend std::ostream & operator<<(std::ostream & __os, const TerminalTextColor & __colorSign)
        {
            std::string asciiColorSign;

            asciiColorSign += "\033[38;5;" + std::to_string(__colorSign.colorCode) + "m";

            __os << asciiColorSign; 

            return __os;
        }
};

typedef struct LogLevel
{
    public:
        TerminalTextColor Original{WHITE};
        TerminalTextColor Notify{BLUE};
        TerminalTextColor Warning{YELLO};
        TerminalTextColor Correct{GREEN};
        TerminalTextColor Error{RED};
} MyLog;

/*一个简单的日志库，用于不同类型消息的输出*/
const MyLog myLog;

#define ORIGINAL    myLog.Original
#define NOTIFY      myLog.Notify
#define WARNING     myLog.Warning
#define CORRECT     myLog.Correct
#define ERROR       myLog.Error

/**
* @brief            这个函数可以打印不同风格的分割线
* 
* @param __len      分割线的长度
* @param __style    分割线的风格，如（------- ****** 或 ·········）
* 
* @return           non-return
*/
static inline void printSplitLine(int __len, const char __style)
{
    std::cout << std::string(__len, __style) << '\n';
}

#endif  // _TERMINAL_COLOR_H_