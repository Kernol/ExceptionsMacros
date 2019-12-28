// ExceptionsMacros. Provides custom exceptions.
// Copyright (C) 2019 Evgeny Zaytsev
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#ifndef EXCEPTIONSMACROS_H
#define EXCEPTIONSMACROS_H

#include <iostream>


/*!
 *    Examples:
 *    ADD_EXCEPTION_LAYER(db, std)
 *    ADD_EXCEPTION(db, custom_exception, db::logic_error)
 *    ADD_SYSTEM_EXCEPTION(db, custom_system_exception, db::system_error)
 */

#define CONCATENATE(var1, var2, var3) var1##var2##var3

#define __SEMICOLON_CONDITION(layer, name)\
    /*GCOV_EXCL_START*/\
    inline void CONCATENATE(__exceptions_macros_function_declaration_for_semicolon_after_macros_, layer, name)()\
    /*GCOV_EXCL_STOP*/


/// ADD_EXCEPTION_LAYER macros

#define __ADD_LAYER_EXCEPTION_CLASS(name)\
    /*GCOV_EXCL_START*/\
    class name : public exception\
    {\
        std::string _msg;\
    public:\
        explicit name(std::string msg) : _msg(#name" { " + std::move(msg) + " }") {}\
        const char *what() const noexcept override { return _msg.c_str();}\
    }\
    /*GCOV_EXCL_STOP*/

#define ADD_EXCEPTION_LAYER_HERE(parent)\
    /*GCOV_EXCL_START*/\
    class exception : public parent::exception {\
    public:\
        exception() noexcept { }\
    };\
    __ADD_LAYER_EXCEPTION_CLASS(logic_error);\
    __ADD_LAYER_EXCEPTION_CLASS(invalid_argument);\
    __ADD_LAYER_EXCEPTION_CLASS(domain_error);\
    __ADD_LAYER_EXCEPTION_CLASS(length_error);\
    __ADD_LAYER_EXCEPTION_CLASS(out_of_range);\
    __ADD_LAYER_EXCEPTION_CLASS(runtime_error);\
    __ADD_LAYER_EXCEPTION_CLASS(range_error);\
    __ADD_LAYER_EXCEPTION_CLASS(overflow_error);\
    __ADD_LAYER_EXCEPTION_CLASS(underflow_error);\
    class system_error : public exception {\
        std::string _msg;\
        int _code;\
    public:\
        system_error(int code, std::string msg = "system error occurred") : _code(code) {\
            char buffer[64];\
            _msg = "system_error { " + msg + " } : " + std::string(strerror_r(code, buffer, sizeof(buffer)));\
        }\
        const char* what() const noexcept override { return _msg.c_str(); }\
        int getErrorCode() const { return _code; }\
    }\
    /*GCOV_EXCL_STOP*/


#define ADD_EXCEPTION_LAYER(layer, parent) \
    /*GCOV_EXCL_START*/\
    namespace layer{\
        ADD_EXCEPTION_LAYER_HERE(parent);\
    }\
    __SEMICOLON_CONDITION(layer, parent)\
    /*GCOV_EXCL_STOP*/


/// ADD_EXCEPTION macros

#define ADD_EXCEPTION_HERE(name, parent)\
    /*GCOV_EXCL_START*/\
    class name : public parent\
    {\
    public:\
        explicit name(std::string msg) : parent(#name" { " + std::move(msg) + " }") {}\
    }\
    /*GCOV_EXCL_STOP*/

#define ADD_EXCEPTION(layer, name, parent)\
    /*GCOV_EXCL_START*/\
    namespace layer{\
        ADD_EXCEPTION_HERE(name, parent);\
    }\
    __SEMICOLON_CONDITION(layer, name)\
    /*GCOV_EXCL_STOP*/


/// ADD_SYSTEM_EXCEPTION macros

#define ADD_SYSTEM_EXCEPTION_HERE(name, parent)\
    /*GCOV_EXCL_START*/\
    class name : public parent\
    {\
    public:\
    explicit name(int code, std::string msg = "system error occurred") : parent(code, #name" { " + std::move(msg) + " }") {}\
    }\
    /*GCOV_EXCL_STOP*/

#define ADD_SYSTEM_EXCEPTION(layer, name, parent)\
    /*GCOV_EXCL_START*/\
    namespace layer{\
        ADD_SYSTEM_EXCEPTION_HERE(name, parent);\
    }\
    __SEMICOLON_CONDITION(layer, name)\
    /*GCOV_EXCL_STOP*/


#endif //EXCEPTIONSMACROS_H
