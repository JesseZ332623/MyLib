#ifndef _TERMINAL_COLOR_H_
#define _TERMINAL_COLOR_H_

#include <iostream>

namespace MyLog
{

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
     * @brief 用户可以通过该函数简便的使用该日志库
     * 
     * @tparam OutPutType 需要输出的类型
     * 
     * @param __logLevel    日志等级
     * @param __out         要输出的数据
     * @param __os          标准输出流
     * 
     * @return              non-return
    */
    template <typename OutPutType>
    static inline void log(const TerminalTextColor & __logLevel, const OutPutType & __out, std::ostream & __os = std::cout)
    {
        __os << __logLevel << __out << ORIGINAL << std::endl;
    }

    /**
    * @brief            这个函数可以打印不同风格的分割线
    * 
    * @param __len      分割线的长度
    * @param __style    分割线的风格，如（------- ****** 或 ·········）
    * @param __os       标准输出流
    * 
    * @return           non-return
    */
    static inline void printSplitLine(int __len, const char __style, std::ostream & __os = std::cout)
    {
        __os << std::string(__len, __style) << '\n';
    }
}

#endif  // _TERMINAL_COLOR_H_