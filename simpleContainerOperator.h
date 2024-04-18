#ifndef _SIMPLE_CONTAINER_OPERATOR_H_
#define _SIMPLE_CONTAINER_OPERATOR_H_

#include "./myDelay.h"
#include "./myLogerDef.h"

#include <iterator>
#include <algorithm>
#include <cstdlib>

namespace MyLib
{
    namespace SimpleContainerOperator
    {
        /**
         * @brief 往指定标准输出流发送 Container 容器的相关信息和数据（只适用于元素为数字类型的 STL 容器）。
         * 
         * @tparam Container STL 容器类型
         * 
         * @param __os          标准输出流的引用
         * @param __container   要发送给输出流的容器的引用
         * 
         * @return non-return
        */
        template <typename Container>
        void showContainerToStream(std::ostream & __os, const Container & __container)
        {
            using namespace MyLoger;
            using MyDelay::delay;

            log(__os, NOTIFY, "This Container size = ", __container.size(), '\n');

            std::size_t containerIndex = 0L;

            std::for_each(__container.cbegin(), __container.cend(), 
                        [& __os, & containerIndex](const decltype((*__container.cbegin())) & n) 
                        { 
                                __os << n << ' '; 
                                delay(45); 
                                if (containerIndex % 5 == 4) { __os << std::endl; }
                                ++containerIndex;
                        }
                        );
            __os << std::endl;
        }

        /**
         * @brief 往一个容器里面插入随机的数据，只适用于元素为数字类型的 STL 容器。
         * 
         * @tparam __Container      STL 容器类型
         * 
         * @param __container       一个 STL 容器的引用，但这个容器的元素最好是数字类型
         * @param __limit           随机数生成的返回不超过这个值
         * @param __randomCount     生成的次数（默认为 10 次）
         * @param __deviationVal    误差值，用于随机生成小数，不填就为 0
         * 
         * @return non-return
        */
        template <typename __Container>
        void pushRandomValue(__Container & __container, int __limit, std::size_t __randomCount = 10, double __deviationVal = 0.0)
        {
            for (std::size_t randomIndex = 0; randomIndex < __randomCount; ++randomIndex)
            {
                __container.push_back(std::rand() % __limit + __deviationVal);
            }
        }
    } // END namespace SimpleContainerOperator
} // END namespace MyLib

#endif // _SIMPLE_CONTAINER_OPERATOR_H_