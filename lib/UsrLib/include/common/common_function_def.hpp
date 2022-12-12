/**
 * @file common_function_def.hpp
 * @author Akari (masiro.to.akari@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-12-05
 *
 * @copyright Copyright (c) 2022 / MaSiRo Project.
 *
 */

#ifndef USR_LIB_COMMON_FUNCTION_DEF_HPP
#define USR_LIB_COMMON_FUNCTION_DEF_HPP

#include <Arduino.h>

namespace MaSiRoProject
{
namespace ToyBox
{
typedef std::function<void(bool, const char *, bool)> ToyBoxMessageFunction;
static bool set_callback_message(ToyBoxMessageFunction target, ToyBoxMessageFunction src);

} // namespace ToyBox
} // namespace MaSiRoProject

#endif