/* Copyright © 2023 Georgy E. All rights reserved. */

#pragma once

namespace fsm
{// TODO: rebase to utl namespace
    constexpr unsigned charHash(const char* data)
    {
        unsigned int  res = 0;

        for (unsigned i = 0; data[i] != '\0'; i++) {
            char symbol = data[i];
            for (unsigned j = sizeof (char) * 8; j > 0; j--) {
                res = ((res ^ symbol) & 1) ? (res >> 1) ^ 0x8C : (res >> 1);
                symbol >>= 1;
            }
        }

        return res;
    }
}