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

        DATA_T& peek() {
            return this->front();
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

        DATA_T& back()
		{
			return CircleBuffer<SIZE, DATA_T>::back();
		}

        DATA_T& front()
		{
			return CircleBuffer<SIZE, DATA_T>::front();
		}

        void clear()
        {
            return CircleBuffer<SIZE, DATA_T>::clear();
        }
    };
}


#endif
