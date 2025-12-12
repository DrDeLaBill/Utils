/* Copyright © 2023 Georgy E. All rights reserved. */

#ifndef __TYPELIST_SERVICE__
#define __TYPELIST_SERVICE__


#include <limits>
#include <cstddef>

#include "TypeListBuilder.h"


namespace utl 
{
	namespace prvt {
		template<bool condition, unsigned SizeIfTrue, class SizeIfFalse>
		struct StaticSizeIf
		{
			static constexpr unsigned SIZE = SizeIfTrue;
		};

		template<unsigned SizeIfTrue, class SizeIfFalse>
		struct StaticSizeIf<false, SizeIfTrue, SizeIfFalse>
		{
			static constexpr unsigned SIZE = SizeIfFalse::SIZE;
		};
	}

	template<unsigned VALUE, unsigned MULTIPLIER, unsigned START_VALUE = MULTIPLIER>
	struct SizeMultiplierSelector
	{
		static constexpr bool isTargetMult = (VALUE < START_VALUE) && ((START_VALUE % MULTIPLIER) == 0);
		static constexpr unsigned SIZE =
			prvt::StaticSizeIf<
				isTargetMult,
				START_VALUE,
				SizeMultiplierSelector<VALUE, MULTIPLIER, START_VALUE * MULTIPLIER>
			>::SIZE;
	};

	template<typename T, T NUMBER>
	struct NumbersCount
	{
		static constexpr size_t SIZE = (NUMBER == 0 ? 0 : 1 + NumbersCount<T, NUMBER / 10>::SIZE);
	};

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
	}

	template<unsigned SIZE>
	struct TypeSelector
	{
		static constexpr bool isLE8bit  = SIZE <= std::numeric_limits<uint8_t>::max();
		static constexpr bool isLE16bit = SIZE <= std::numeric_limits<uint16_t>::max();
		static constexpr bool isLE32bit = SIZE <= std::numeric_limits<uint32_t>::max();

		typedef typename prvt::StaticTypeIf<
			isLE8bit,
			uint8_t,
			typename prvt::StaticTypeIf<
				isLE16bit,
				uint16_t,
				typename prvt::StaticTypeIf<
					isLE32bit,
					uint32_t,
					uint64_t
				>::TYPE
			>::TYPE
		>::TYPE TYPE;
	};

    // Get type list length
    template<class... TypeList>
    constexpr std::size_t size(unit_t<TypeList...>) {
        return 1 + size(typename unit_t<TypeList...>::TAIL{});
    }

    template<class Head>
    constexpr std::size_t size(unit_t<Head, null_type_t>) {
        return 1;
    }
    
    template<class... TypeList>
    constexpr bool empty(unit_t<TypeList...> list) {
        return size(list) == 0;
    }
    
    template<>
    constexpr bool empty(unit_t<null_type_t>) {
        return true;
    }
    
    constexpr bool empty(null_type_t) {
        return true;
    }

    // TODO: операторы сравнения
}


#endif