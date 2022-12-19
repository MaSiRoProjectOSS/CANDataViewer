/**
 * @file common_function.hpp
 * @author Akari (masiro.to.akari@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-12-05
 *
 * @copyright Copyright (c) 2022 / MaSiRo Project.
 *
 */

#ifndef USR_LIB_COMMON_FUNCTION_HPP
#define USR_LIB_COMMON_FUNCTION_HPP

namespace MaSiRoProject
{
namespace ToyBox
{
class CommonFunction {
public:
    static void output_message(bool message_type_is_error, const char *message, bool add_header = true);
};
} // namespace ToyBox
} // namespace MaSiRoProject

#endif