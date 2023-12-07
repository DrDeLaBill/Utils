/* Copyright © 2023 Georgy E. All rights reserved. */

#pragma once


namespace utl 
{
    struct NullType { };
 
    namespace Private
    {   
        template<class H, class T = NullType>
        struct TypeUnit { };
        
        template<class T>
        struct Head { 
            using TYPE = T;
        };
        
        template<class H, class T>
        struct Head<TypeUnit<H, T>> {
            using TYPE = H;
        };
        
        template<class T>
        struct Tail {
            using TYPE = NullType;
        };
        
        template<class H, class T>
        struct Tail<TypeUnit<H, T>> {
            using TYPE = T;                 
        };
    }
    
    template<class T>
    using head_t = typename Private::Head<T>::TYPE;
    
    template<class T>
    using tail_t = typename Private::Tail<T>::TYPE;
    
 
    namespace Private
    {   
        template<class... TypeList>
        struct List { };
        
        template<>
        struct List<> {
            using RESULT = NullType;
        };
        
        template<class H, class... TypeList>
        struct List<H, TypeList...> {
            using RESULT = TypeUnit<H, typename List<TypeList...>::RESULT>;
        };
    }
     
    template<class... TypeList>
    using type_list_t = typename Private::List<TypeList...>::RESULT;

    namespace Private
    {
        template<class TypeList, class Target>
        struct EraseTarget
        {
        private:
            using HEAD = typename Head<TypeList>::TYPE;
            using TAIL = typename Tail<TypeList>::TYPE;
            
        public:
            using RESULT = TypeUnit<HEAD, typename EraseTarget<TAIL, Target>::RESULT>;
        };
        
        template<class Tail, class Target>
        struct EraseTarget<TypeUnit<Target, Tail>, Target>
        {
            using RESULT = Tail;
        };
        
        template<class Target>
        struct EraseTarget<NullType, Target>
        {
            using RESULT = NullType;
        };
    }

    template<class TypeList, class Target>
    using erase_target_t = typename Private::EraseTarget<TypeList, Target>::RESULT;
    
    namespace Private
    {
        template<class TypeList>
        struct RemoveDuplicates { };
        
        template<class Head, class Tail>
        struct RemoveDuplicates<TypeUnit<Head, Tail>>
        {
        private:
            using NEXT_PART = typename RemoveDuplicates<Tail>::RESULT;
            using ERASE_RES = typename EraseTarget<NEXT_PART, Head>::RESULT;
            
        public:
            using RESULT = TypeUnit<Head, ERASE_RES>;
        };
        
        template<>
        struct RemoveDuplicates<NullType>
        {
            using RESULT = NullType;
        };
    }

    template<class... TypeList>
    using remove_duplicates_t = typename Private::RemoveDuplicates<type_list_t<TypeList...>>::RESULT;
}