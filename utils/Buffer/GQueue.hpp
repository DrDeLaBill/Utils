/* Copyright © 2025 Georgy E. All rights reserved. */

#ifndef _G_QUEUE_H_
#define _G_QUEUE_H_


#include "CircleBuffer.hpp"


namespace utl {

    template<unsigned SIZE, class DATA_T=uint8_t>
    class GQueue : protected CircleBuffer<SIZE, DATA_T> {
    public:
        GQueue() : CircleBuffer<SIZE, DATA_T>() {}
        
        void push(DATA_T data) {
            this->push_back(data);
        }

        DATA_T pop() {
            return this->pop_front();
        }

        DATA_T peek() {
            return this->front();
        }

        typename CircleBuffer<SIZE, DATA_T>::INDEX_T count() {
            return CircleBuffer<SIZE, DATA_T>::count();
        }

        bool empty() {
            return this->empty();

        }

        bool full() {
            return this->full();
        }

        const DATA_T& back()
        {
            return this->back();
        }

        const DATA_T& front()
        {
            return this->front();
        }
    };
}


#endif
