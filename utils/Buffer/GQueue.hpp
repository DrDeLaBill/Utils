/* Copyright © 2025 Georgy E. All rights reserved. */

#ifndef _G_QUEUE_H_
#define _G_QUEUE_H_


#include "CircleBuffer.h"


namespace utl {

    template<unsigned SIZE, class DATA_T=uint8_t>
    class GQueue : private CircleBuffer<SIZE, DATA_T> {
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

        INDEX_T count() {
            return this->count();
        }

        bool empty() {
            return this->empty();

        }

        bool full() {
            return this->full();
        }
    };
}


#endif