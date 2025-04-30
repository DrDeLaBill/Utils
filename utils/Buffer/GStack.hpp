/* Copyright © 2025 Georgy E. All rights reserved. */

#ifndef _G_STACK_H_
#define _G_STACK_H_


#include "CircleBuffer.hpp"


namespace utl {

    template<unsigned SIZE, class DATA_T=uint8_t>
    class GStack : protected CircleBuffer<SIZE, DATA_T> {
    public:
        GStack() : CircleBuffer<SIZE, DATA_T>() {}
            
        void push(DATA_T data) {
            this->push_back(data);
        }

        DATA_T pop() {
            return this->pop_back();
        }

        DATA_T& peek() {
            return this->back();
        }

        typename CircleBuffer<SIZE, DATA_T>::INDEX_T count() {
            return CircleBuffer<SIZE, DATA_T>::count();
        }

        bool empty() {
            return CircleBuffer<SIZE, DATA_T>::empty();

        }

        bool full() {
            return CircleBuffer<SIZE, DATA_T>::full();
        }

        void clear()
        {
            return CircleBuffer<SIZE, DATA_T>::clear();
        }
    };
}


#endif
