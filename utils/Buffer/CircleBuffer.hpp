/* Copyright © 2025 Georgy E. All rights reserved. */

#ifndef _CIRCLE_BUFFER_H_
#define _CIRCLE_BUFFER_H_


#include <limits>
#include <cstdint>
#include <cstring>

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

		template<class TypeIfTrue, class TypeIfFalse>
		struct StaticTypeIf<false, TypeIfTrue, TypeIfFalse>
		{
			typedef TypeIfFalse TYPE;
		};

		template<unsigned SIZE>
		struct TypeSelector
		{
			static constexpr bool isLE8bit  = SIZE <= std::numeric_limits<uint8_t>::max();
			static constexpr bool isLE16bit = SIZE <= std::numeric_limits<uint16_t>::max();

			typedef typename StaticTypeIf<
				isLE8bit,
				uint8_t,
				typename StaticTypeIf<
					isLE16bit,
					uint16_t,
					uint32_t
				>::TYPE
			>::TYPE TYPE;
		};
	}
	
	template<unsigned SIZE, class DATA_T=uint8_t>
	class CircleBuffer
	{
	
	static_assert(SIZE > 0);
	static_assert((SIZE & (SIZE - 1)) == 0);

	private:
	
		using INDEX_T = typename prvt::TypeSelector<SIZE>::TYPE;
	
		static const INDEX_T m_mask = SIZE - 1;
		
		INDEX_T m_readCount;
		INDEX_T m_writeCount;
		
		DATA_T m_data[SIZE];
		
	public:
		CircleBuffer()
		{
			m_readCount = 0;
			m_writeCount = 0;
		}

		DATA_T& operator[] (INDEX_T i)
        {
			return m_data[(m_readCount + i) & m_mask];
        }

        const DATA_T operator[] (INDEX_T i) const
        {
			return m_data[(m_readCount + i) & m_mask];
        }

		void push_back(const DATA_T& value)
		{
			if (full()) {
				pop_front();
			}
			m_data[(m_writeCount++) & m_mask] = value;
		}

		void push_front(const DATA_T& value)
		{
			if (full()) {
				pop_back();
			}
			m_data[(--m_readCount) & m_mask] = value;
		}
		
		bool shift(DATA_T& value)
		{
			if (empty()) {
				return false;
			}
			value = m_data[m_readCount++ & m_mask];
			return true;
		}

		const DATA_T& pop_front()
		{
			BEDUG_ASSERT(!empty(), "error pop front unit - circle buffer is empty");
			if (empty()) {
				return m_data[m_readCount & m_mask];
			}
			return m_data[m_readCount++ & m_mask];
		}

		const DATA_T& pop_back()
		{
			BEDUG_ASSERT(!empty(), "error pop back unit - circle buffer is empty");
			if (empty()) {
				return m_data[m_readCount & m_mask];
			}
			return m_data[(m_writeCount-- - 1) & m_mask];
		}

		const DATA_T& front()
		{
			BEDUG_ASSERT(!empty(), "error get front unit - circle buffer is empty");
			if (empty()) {
				return m_data[m_readCount & m_mask];
			}
			return m_data[m_readCount & m_mask];
		}

		const DATA_T& back()
		{
			BEDUG_ASSERT(!empty(), "error get back unit - circle buffer is empty");
			if (empty()) {
				return m_data[m_readCount & m_mask];
			}
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
		
        INDEX_T count() const
        {
			return full() ? SIZE : ((m_writeCount - m_readCount) & m_mask);
        }

		inline unsigned size() { return SIZE; }
	};
}

#endif
