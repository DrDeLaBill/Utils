/* Copyright © 2023 Georgy E. All rights reserved. */

#ifndef __TYPELIST_BUILDER__
#define __TYPELIST_BUILDER__


#if __cplusplus > 201402L
	#include <variant>


namespace utl
{
	struct null_type_t {};

	template<class Head, class Tail = null_type_t>
	struct unit_t
	{
		using HEAD = Head;
		using TAIL = Tail;
	};

	// Simple type list
	template<class... Types>
	struct simple_list_t { };

	// Get concrete type
	template<class T>
	struct getType
	{
		using TYPE = T;
	};

	template<template<class> class T, class U>
	struct getType<T<U>>
	{
		using TYPE = typename getType<U>::TYPE;
	};

	// Type list with unit structure
	template<class... Types>
	struct typelist_t { };

	template<>
	struct typelist_t<>
	{
		using RESULT = null_type_t;
	};

	template<class Head>
	struct typelist_t<Head>
	{
		using RESULT = unit_t<Head, typename typelist_t<>::RESULT>;
	};

	template<class Head, class... Types>
	struct typelist_t<Head, Types...>
	{
		using RESULT = unit_t<Head, typename typelist_t<Types...>::RESULT>;
	};

	// Removed target type list
	template<class TypeList, class Target>
	struct removed_target_t { };

	template<class Head, class Tail, class Target>
	struct removed_target_t<unit_t<Head, Tail>, Target>
	{
		using RESULT = unit_t<Head, typename removed_target_t<Tail, Target>::RESULT>;
	};

	template<class Tail, class Target>
	struct removed_target_t<unit_t<Target, Tail>, Target>
	{
		using RESULT = Tail;
	};

	template<class Target>
	struct removed_target_t<null_type_t, Target>
	{
		using RESULT = null_type_t;
	};

	// Removed duplications type list
	template<class... Types>
	struct removed_duplicates_t
	{
		using RESULT = typename removed_duplicates_t<typename typelist_t<Types...>::RESULT>::RESULT;
	};

	template<class Head>
	struct removed_duplicates_t<Head>
	{
		using RESULT = unit_t<Head, null_type_t>;
	};

	template<class Head, class Tail>
	struct removed_duplicates_t<unit_t<Head, Tail>>
	{
	private:
		using NEXT_PART = typename removed_duplicates_t<Tail>::RESULT;
		using REMOVED_T = typename removed_target_t<NEXT_PART, Head>::RESULT;
	public:
		using RESULT = unit_t<Head, REMOVED_T>;
	};

	template<>
	struct removed_duplicates_t<null_type_t>
	{
		using RESULT = null_type_t;
	};

	// Variant factory
	template<class... Types>
	struct variant_factory { };

	template<class Head, class Tail>
	struct variant_factory<unit_t<Head, Tail>>
	{
		using VARIANT = typename variant_factory<std::variant<Head>, Tail>::VARIANT;
	};

	template<template<class> class List, class Head, class Tail>
	struct variant_factory<List<unit_t<Head, Tail>>>
	{
		using VARIANT = typename variant_factory<std::variant<Head>, Tail>::VARIANT;
	};

	template<class... Types, template<class> class List>
	struct variant_factory<List<Types...>>
	{
		using LIST_RES = typename List<Types...>::RESULT;
		using VARIANT = typename variant_factory<LIST_RES>::VARIANT;
	};

	template<class Head, class Tail, class... TypeList>
	struct variant_factory<std::variant<TypeList...>, unit_t<Head, Tail>>
	{
		using VARIANT = typename variant_factory<std::variant<TypeList..., Head>, Tail>::VARIANT;
	};

	template<class... TypeList, class Head>
	struct variant_factory<std::variant<TypeList...>, unit_t<Head, null_type_t>>
	{
		using VARIANT = std::variant<TypeList..., Head>;
	};

	template<class Type>
	struct variant_factory<std::variant<Type>, unit_t<null_type_t>>
	{
		using VARIANT = std::variant<Type>;
	};

	template<class Type>
	struct variant_factory<std::variant<Type>, null_type_t>
	{
	    using VARIANT = std::variant<Type>;
	};

	template<class Type>
	struct variant_factory<Type, unit_t<null_type_t>>
	{
	    using VARIANT = std::variant<Type>;
	};

	template<class Type>
	struct variant_factory<Type, null_type_t>
	{
	    using VARIANT = std::variant<Type>;
	};

	template<>
	struct variant_factory<std::variant<>, unit_t<null_type_t>>
	{
	    using VARIANT = std::variant<null_type_t>;
	};

	template<>
	struct variant_factory<std::variant<>, null_type_t>
	{
	    using VARIANT = std::variant<null_type_t>;
	};
}


#endif


#endif