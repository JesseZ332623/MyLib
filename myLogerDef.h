#ifndef _MYLOGDEF_H_
#define _MYLOGDEF_H_

#include <iostream>
#include <ctime>

namespace MyLib
{
    namespace MyLoger
    {
        enum ColorMenu { RED = 1, GREEN, YELLO, BLUE = 6, WHITE };

        class TerminalTextColor
        {
            private:
                int colorCode {0};

            public:
                explicit TerminalTextColor(const int __colorSign) : colorCode(__colorSign) {}

                const int getColorCode(void) const { return colorCode; }

                bool operator==(const TerminalTextColor & __b) const { return (this->getColorCode() == __b.getColorCode()); }

                friend std::ostream & operator<<(std::ostream & __os, const TerminalTextColor & __colorSign)
                {
                    std::string asciiColorSign;

                    asciiColorSign += "\033[38;5;" + std::to_string(__colorSign.colorCode) + "m";

                    __os << asciiColorSign; 

                    return __os;
                }
        };

        const TerminalTextColor defaultLogLevel [] = 
        {
            TerminalTextColor(WHITE), TerminalTextColor(BLUE), TerminalTextColor(YELLO),
            TerminalTextColor(GREEN), TerminalTextColor(RED)
        };

        #define ORIGINAL    defaultLogLevel[0]
        #define NOTIFY      defaultLogLevel[1]
        #define WARNING     defaultLogLevel[2]
        #define CORRECT     defaultLogLevel[3]
        #define ERROR       defaultLogLevel[4]

        /**
         * @brief 获取当前的时间
         * 
         * @return 时间字符串，格式为: Month/Day/Year Hour/Minute/Second
        */
        std::string getCurrentTime(void)
        {
            char timeStringBuffer[30] = {"0"};

            /* 获取当前的时间戳 */
            std::time_t timeStamp = std::time(nullptr);

            /*
                将时间戳转化为字符串，
                存入 timeStringBuffer 之中
            */
            std::strftime(timeStringBuffer, sizeof(timeStringBuffer), "%c", std::localtime(&timeStamp));

            /* 最后交给 std::string 构造并返回 */
            return std::string(timeStringBuffer);
        }

        /**
         * @brief 用户可以通过该函数简便的使用该日志库（函数名使用 loger 避免和 std::log 混淆）
         * 
         * @tparam OutPutType 需要输出的类型
         * 
         * @param __os          标准输出流的引用
         * @param __logLevel    日志等级
         * @param __out         要输出的数据，会被打包成形参包
         * 
         * @return              non-return
        */
        template <typename ...OutPutType>
        static inline void loger(std::ostream & __os, const TerminalTextColor  __logLevel, OutPutType ... __out)
        {
            __os << __logLevel;

            if (__logLevel == WARNING || __logLevel == ERROR)
            {
                __os << '[' << getCurrentTime() << ']' << ' ';
            }

            /**
             * 通过 C++17 标准推出的折叠表达式功能，可以将形参包解包然后依次完美转发，
             * 逐个向标准输出流发送数据。
            */
            (void)(__os << ... << std::forward<OutPutType>(__out));
            
            __os.flush();
            __os << ORIGINAL;
        }

        /**
        * @brief            这个函数可以打印不同风格的分割线
        * 
        * @param __len      分割线的长度
        * @param __style    分割线的风格，如（------- ****** 或 ·········）
        * @param __os       标准输出流的引用
        * 
        * @return           non-return
        */
        static inline void printSplitLine(int __len, const char __style, std::ostream & __os = std::cout)
        {
            __os << std::string(__len, __style) << '\n';
        }
    } // END namespace MyLog
} // END namespace MyLib

#endif  // _MYLOGDEF_H_