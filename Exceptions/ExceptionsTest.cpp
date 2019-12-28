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

#define CATCH_CONFIG_MAIN

#include <catch.hpp>
#include "exceptions_macros.h"

#define REPEAT_NUM 100


//! Common local macros

#define __CHECK_VALID_THROW(parent, name, text)\
    REQUIRE_THROWS_AS(throw parent::name(text), parent::name);

#define __CHECK_STRING_CONSTRUCTOR_MSG_CORRECTNESS(parent, name, text) \
        msg = text;\
        try{throw parent::name(msg);}\
        catch(parent::name& ex) { REQUIRE(std::string(ex.what()) == (std::string(#name) + " { " + std::string(msg) + " }")); }


//! Macros for ADD_EXCEPTION_LAYER

ADD_EXCEPTION_LAYER(db, std);

#define __TEST_MACRO(macroName)\
    macroName(db, logic_error, "logic_error occurred")\
    macroName(db, invalid_argument, "invalid_argument occurred")\
    macroName(db, domain_error, "domain_error occurred")\
    macroName(db, length_error, "length_error occurred")\
    macroName(db, out_of_range, "out_of_range occurred")\
    \
    macroName(db, runtime_error, "runtime_error occurred")\
    macroName(db, range_error, "range_error occurred")\
    macroName(db, overflow_error, "overflow_error occurred")\
    macroName(db, underflow_error, "underflow_error occurred")


//! Macros for ADD_EXCEPTION

ADD_EXCEPTION(db, custom_logic_error, db::logic_error);
ADD_EXCEPTION(db, custom_invalid_argument, db::invalid_argument);
ADD_EXCEPTION(db, custom_domain_error, db::domain_error);
ADD_EXCEPTION(db, custom_length_error, db::length_error);
ADD_EXCEPTION(db, custom_out_of_range, db::out_of_range);

ADD_EXCEPTION(db, custom_runtime_error, db::runtime_error);
ADD_EXCEPTION(db, custom_range_error, db::range_error);
ADD_EXCEPTION(db, custom_overflow_error, db::overflow_error);
ADD_EXCEPTION(db, custom_underflow_error, db::underflow_error);
ADD_SYSTEM_EXCEPTION(db, custom_system_error, db::system_error);

#define __TEST_MACRO_ADD(macroName)\
    macroName(db, custom_logic_error)\
    macroName(db, custom_invalid_argument)\
    macroName(db, custom_domain_error)\
    macroName(db, custom_length_error)\
    macroName(db, custom_out_of_range)\
    \
    macroName(db, custom_runtime_error)\
    macroName(db, custom_range_error)\
    macroName(db, custom_overflow_error)\
    macroName(db, custom_underflow_error)

#define __CHECK_VALID_THROW_ADD(parent, name)\
    REQUIRE_THROWS_AS(throw parent::name(""), parent::name);

#define __CHECK_STRING_CONSTRUCTOR_MSG_CORRECTNESS_ADD(parent, name) \
        msg = #name" occurred";\
        try{throw parent::name(msg);}\
        catch(parent::name& ex) { REQUIRE(std::string(ex.what()).find(#name" { " + std::string(msg) + " }") != std::string::npos); }


TEST_CASE("ExceptionsMacrosTests")
{
    SECTION("Throw-catch_ADD_EXCEPTION_LAYER")
    {
        SECTION("ValidThrow")
        {
            for (size_t i = 0; i < REPEAT_NUM; ++i)
            {
                __TEST_MACRO(__CHECK_VALID_THROW)
                REQUIRE_THROWS_AS(throw db::system_error(random() % 133), db::system_error);
            }
        }

        SECTION("Constructor")
        {
            for (size_t i = 0; i < REPEAT_NUM; ++i)
            {
                std::string msg;
                __TEST_MACRO(__CHECK_STRING_CONSTRUCTOR_MSG_CORRECTNESS)
                auto error_code = random() % 133;
                char buffer[64];
                try { throw db::system_error(error_code); }
                catch (db::system_error& ex)
                {
                    REQUIRE(std::string(ex.what()) ==
                            ("system_error { system error occurred } : " + std::string(strerror_r(error_code, buffer, 64))));
                    REQUIRE(ex.getErrorCode() == error_code);
                }
                msg = "custom system error text";
                try { throw db::system_error(error_code, msg); }
                catch (db::system_error &ex)
                {
                    REQUIRE(std::string(ex.what()) ==
                            ("system_error { " + msg + " } : " + std::string(strerror_r(error_code, buffer, 64))));
                    REQUIRE(ex.getErrorCode() == error_code);
                }
            }
        }
    }

    SECTION("Throw-catch_ADD_EXCEPTION")
    {
        SECTION("ValidThrow")
        {
            for (size_t i = 0; i < REPEAT_NUM; ++i)
            {
                __TEST_MACRO_ADD(__CHECK_VALID_THROW_ADD)
                REQUIRE_THROWS_AS(throw db::custom_system_error(random() % 133),
                                  db::custom_system_error);
            }
        }

        SECTION("Constructor")
        {
            for (size_t i = 0; i < REPEAT_NUM; ++i)
            {
                std::string msg;
                __TEST_MACRO_ADD(__CHECK_STRING_CONSTRUCTOR_MSG_CORRECTNESS_ADD)
                auto error_code = random() % 133;
                char buffer[64];
                try { throw db::custom_system_error(error_code); }
                catch (db::custom_system_error &ex)
                {
                    REQUIRE(std::string(ex.what()) ==
                            ("system_error { custom_system_error { system error occurred } } : " + std::string(strerror_r(error_code, buffer, 64))));
                    REQUIRE(ex.getErrorCode() == error_code);
                }
                msg = "custom system error text";
                try { throw db::custom_system_error(error_code, msg); }
                catch (db::custom_system_error &ex)
                {
                    REQUIRE(std::string(ex.what()) ==
                            ("system_error { custom_system_error { " + msg + " } } : " + std::string(strerror_r(error_code, buffer, 64))));
                    REQUIRE(ex.getErrorCode() == error_code);
                }
            }
        }
    }
}