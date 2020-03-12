// Copyright (c) Microsoft Corporation.
// Licensed under the MIT license.

#pragma once

#include "til/at.h"
#include "til/color.h"
#include "til/some.h"
#include "til/point.h"
#include "til/size.h"
#include "til/rectangle.h"
#include "til/u8u16convert.h"

//#include "til/operators.h"

namespace til // Terminal Implementation Library. Also: "Today I Learned"
{
}

// These sit outside the namespace because they sit outside for WIL too.

// Inspired from RETURN_IF_WIN32_BOOL_FALSE
// WIL doesn't include a RETURN_BOOL_IF_FALSE, and RETURN_IF_WIN32_BOOL_FALSE
//  will actually return the value of GLE.
#define RETURN_BOOL_IF_FALSE(b)                     \
    do                                              \
    {                                               \
        const bool __boolRet = wil::verify_bool(b); \
        if (!__boolRet)                             \
        {                                           \
            return __boolRet;                       \
        }                                           \
    } while (0, 0)

// Due to a bug (DevDiv 441931), Warning 4297 (function marked noexcept throws exception) is detected even when the throwing code is unreachable, such as the end of scope after a return, in function-level catch.
#define CATCH_LOG_RETURN_FALSE()            \
    catch (...)                             \
    {                                       \
        __pragma(warning(suppress : 4297)); \
        LOG_CAUGHT_EXCEPTION();             \
        return false;                       \
    }

// MultiByteToWideChar has a bug in it where it can return 0 and then not set last error.
// WIL has a fit if the last error is 0 when a bool false is returned.
// This macro doesn't have a fit. It just reports E_UNEXPECTED instead.
#define THROW_LAST_ERROR_IF_AND_IGNORE_BAD_GLE(condition) \
    do                                                    \
    {                                                     \
        if (condition)                                    \
        {                                                 \
            const auto gle = ::GetLastError();            \
            if (gle)                                      \
            {                                             \
                THROW_WIN32(gle);                         \
            }                                             \
            else                                          \
            {                                             \
                THROW_HR(E_UNEXPECTED);                   \
            }                                             \
        }                                                 \
    } while (0, 0)
