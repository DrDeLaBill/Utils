/* Copyright © 2023 Georgy E. All rights reserved. */

#pragma once


#include <stdint.h>

#include "bmacro.h"


namespace utl 
{
	
	namespace prvt
	{
		template<bool condition, class TypeIfTrue, class TypeIfFalse>
		struct StaticTypeIf
		{
			typedef TypeIfTrue TYPE;
		};

		template<>
		struct StaticTypeIf<false, class TypeIfTrue, class TypeIfFalse>
		{
			typedef TypeIfFalse TYPE;
		};

		template<unsigned SIZE>
		struct TypeSelector
		{
			static const bool isLE8bit = SIZE <= 0xFF;
			static const bool isLE16bit = SIZE <= 0xFFFF;

			typedef typename StaticTypeIf<isLE8bit, uint8_t, typename StaticTypeIf<isLE16bit, uint16_t, uint32_t>::TYPE>::TYPE TYPE;
		};
	}
	
	template<unsigned SIZE, class DATA_T=uint8_t>
	class circle_buffer
	{
	
	static_assert(SIZE > 0);
	static_assert((SIZE & (SIZE - 1)) == 0);

	private:
	
		typedef typename prvt::TypeSelector<SIZE>::TYPE INDEX_T;
	
		static const INDEX_T m_mask = SIZE - 1;
		
		volatile INDEX_T m_readCount;
		volatile INDEX_T m_writeCount;
		
		DATA_T m_data[SIZE];
		
		DATA_T& operator[] (INDEX_T i)
        {
			if(empty() || i > count()) {
				return DATA_T();
			}
			return m_data[(m_readCount + i) & m_mask];
        }

        const DATA_T operator[] (INDEX_T i) const
        {
			if(empty()) {
				return DATA_T();
			}
			return m_data[(m_readCount + i) & m_mask];
        }
		
	public:
		circle_buffer()
		{
			m_readCount = 0;
			m_writeCount = 0;
		}

		bool push_back(const DATA_T& value) 
		{
			if (full()) {
				return false;
			}
			m_data[m_writeCount++ & m_mask] = value;
			return true;
		}

		bool push_front(const DATA_T& value)
		{
			if (full()) {
				return false;
			}
			m_data[(--m_readCount) & m_mask] = value;
			return true;
		}
		
		bool shift(DATA_T& value)
		{
			if (empty()) {
				return false;
			}
			value = m_data[m_readCount++ & m_mask];
			return true;
		}

		bool pop_front()
		{
			if (empty()) {
				return false;
			}
			m_readCount++;
			return true;
		}

		bool pop_back()
		{
			if (empty()) {
				return false;
			}
			m_writeCount--;
			return true;
		}

		const DATA_T& front()
		{
			BEDUG_ASSERT(!empty(), "get front unit - circle buffer is empty");
			return m_data[m_readCount & m_mask];
		}

		const DATA_T& back()
		{
			BEDUG_ASSERT(!empty(), "get back unit - circle buffer is empty");
			return m_data[(m_writeCount - 1) & m_mask];
		}

		bool empty() const
		{
			return m_writeCount == m_readCount;
		}

		bool full() const
		{
			return ((INDEX_T)(m_writeCount - m_readCount) & (INDEX_T)~(m_mask)) != 0;
		}
		
		void clear()
        {
			m_readCount  = 0;
			m_writeCount = 0;
        }
		
        INDEX_T count()const
        {
			return (m_writeCount - m_readCount) & m_mask;
        }
		
		inline unsigned size() { return SIZE; }
	};
}
