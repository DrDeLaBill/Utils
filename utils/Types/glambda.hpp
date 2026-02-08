/* Copyright © 2025 Georgy E. All rights reserved. */

#ifndef _G_LAMBDA_HPP_
#define _G_LAMBDA_HPP_


#include <functional>


#define G_LAMBDA_ITERATOR_CREATE(...) static std::initializer_list<std::function<void()>> _g_lambda_functors = { __VA_ARGS__ }; static size_t _g_lambda_iterator = 0;

#define G_LAMBDA_ITERATOR_TICK() { \
            if (_g_lambda_iterator >= _g_lambda_functors.size()) { \
                _g_lambda_iterator = 0; \
            } \
            (*(_g_lambda_functors.begin() + _g_lambda_iterator++))(); \
        }

#define G_LAMBDA_ITERATOR_RESET() { \
            _g_lambda_iterator = 0; \
        }


#endif // #ifndef _G_LAMBDA_HPP_