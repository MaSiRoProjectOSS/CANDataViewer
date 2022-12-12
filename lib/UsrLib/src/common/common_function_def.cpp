/**
 * @file common_function_def.cpp
 * @author Akari (masiro.to.akari@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-12-05
 *
 * @copyright Copyright (c) 2022 / MaSiRo Project.
 *
 */
#include "common/common_function_def.hpp"

namespace MaSiRoProject
{
namespace ToyBox
{
bool set_callback_message(ToyBoxMessageFunction target, ToyBoxMessageFunction src)
{
    bool result = false;
    try {
        if (nullptr != src) {
            if (nullptr != target) {
                target = src;
                result = true;
            }
        }
    } catch (...) {
    }
    return result;
}

} // namespace ToyBox
} // namespace MaSiRoProject
