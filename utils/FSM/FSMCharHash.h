/* Copyright © 2023 Georgy E. All rights reserved. */

#ifndef _FSM_CHAR_HASH_H_
#define _FSM_CHAR_HASH_H_


namespace fsm
{
    constexpr unsigned charHash(const char* data)
    {
        unsigned int  res = 0;

        for (unsigned i = 0; data[i] != '\0'; i++) {
            char symbol = data[i];
            for (unsigned j = sizeof (char) * 8; j > 0; j--) {
                res = ((res ^ symbol) & 1) ? (res >> 1) ^ 0x8C : (res >> 1);
                symbol >>= 1;
            }
            res <<= 1;
        }

        return res;
    }
}


#endif
