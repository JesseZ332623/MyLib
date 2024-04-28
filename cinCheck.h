#ifndef _CIN_CHECK_H_
#define _CIN_CHECK_H_

#include "./myLogerDef.h"
#include "./myDelay.h"

#include <limits>

namespace MyLib
{
    namespace cinCheck
    {
        /**
         * @brief 忽略掉标准输入缓冲区中，换行符前的所有字符。
         * 
         * @return non-return
        */
        static inline void eatLine(void) { while (std::cin.get() != '\n') { continue; } }

        /**
         * @brief 一个消息结构体，供 `istreamInputAndCheck()` 函数使用，由三个字符串组成：
         * 
         * @brief - `promptMessage`     用于提示用户该如何输入的消息
         * @brief - `errorMessage`      当输入流出现不符合数据格式的输入（由 `istreamStateCheck()` 函数检测）时报告的消息
         * @brief - `outRangeMessage`   当输入的数据合规但不在用户指定范围内时输出的消息
         *                             （由 `istreamStateCheck()` 函数内的 `__rangeRule()` 回调函数检测，用户自行定义）
        */
        typedef struct __MessageStrings
        {
            std::string promptMessage;
            std::string errorMessage;
            std::string outRangeMessage;

        } MessageStrings;

        /**
         * @brief 在对数字类型的数据使用 >> 运算符进行输入时，对标准输入流进行检查，
         *        若不符合数据格式的输入（比如用户输入非数字字符）就清理标志位并忽略此次输入。
         * 
         * @param __is              标准输入流的引用
         * @param __errMessage      当出现意外输入时所报的错误消息
         * 
         * @return 输入数据是否合规
        */
        static inline bool istreamStateCheck(std::istream & __is, const std::string & __errMessage)
        {
            using namespace MyLib::MyLoger;

            if (__is.fail())
            {
                ERROR_LOG(__errMessage);

                __is.clear();
                __is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                return false;
            }

            return true;
        }

        /**
         * @brief 封装对数字类型输入格式和范围的检查，
         *        用来过滤输入垃圾数据避免程序意外终止或进入不一致状态。
         * 
         * @tparam DataType     输入数据的类型
         * @tparam InputRule    用户为输入的数字范围所指定的规则，通常是 `Lamba` 表达式
         * 
         * @param __is          标准输入流的引用
         * @param __member      要输入的数据类型
         * @param __messages    针对不同的情况输出的消息结构体，需要在外部实例化
         * @param __rangeRule   为输入的数字范围所指定的规则
         * 
         * @return non-return
        */
        template <typename DataType, typename InputRule>
        static inline void istreamInputAndCheck(
                            std::istream & __is, DataType & __member, 
                            const MessageStrings & __messages, InputRule __rangeRule
                        )
        {
            using namespace MyLib::MyLoger;
            using MyDelay::delay;

            while (true)
            {
                NOTIFY_LOG(__messages.promptMessage);
                __is >> __member;

                if (!istreamStateCheck(__is, __messages.errorMessage)) { delay(800); continue; }
                if (!__rangeRule()) 
                { 
                    ERROR_LOG(__messages.outRangeMessage); 
                    eatLine(); 
                    continue; 
                }

                break;
            }
        }

    }; // END namespace cinCheck
};  // END namespace MyLib

#endif // _CIN_CHECK_H_