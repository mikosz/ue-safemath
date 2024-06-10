// Copyright Mikolaj Radwan 2024
// Distributed under the Apache 2.0 license (See accompanying file LICENSE)

#pragma once

#include <type_traits>

// #TODO_astrosim: this should be exported to a separate plugin. But only maybe, then again I'd have to handle it like
// errant utilities, because if someone wanted to use safemath I wouldn't want them to be required to pull in multiple
// plugins. Then we'd give it a namespace and list storage would be moved to a private name (maybe flattened too)

namespace SafeMath::Private
{

/// ListStorage is organised reflecting the list elements, but using inheritance. This means that if you store
/// three elements, an int, a char and a double, ListStorage will be a ListStorage<int, ...>, inheriting
/// from a ListStorage<char, ...>, inheriting from a ListStorage<double, ...>, each level storing an int, char and
/// a double respectively.
template <class DataType, class ParentType, class = void>
struct TListStorage;

template <class DataType, class ParentType>
struct TListStorage<DataType, ParentType, std::enable_if_t<std::is_empty_v<DataType>>> : ParentType
{
	constexpr TListStorage() = default;

	template <class... ParentArgsType>
	explicit constexpr TListStorage(DataType, ParentArgsType... ParentArgs) : ParentType(std::move(ParentArgs)...)
	{
	}

	constexpr DataType GetData() const
	{
		return DataType{};
	}
};

template <class DataType, class ParentType>
struct TListStorage<DataType, ParentType, std::enable_if_t<!std::is_empty_v<DataType>>> : ParentType
{
	DataType Data;

	constexpr TListStorage() = default;

	template <class... ParentArgsType>
	explicit constexpr TListStorage(DataType Data, ParentArgsType... ParentArgs)
		: ParentType(std::move(ParentArgs)...), Data(std::move(Data))
	{
	}

	constexpr TListStorage(DataType Data, const ParentType& Parent) : ParentType(Parent), Data(std::move(Data))
	{
	}

	constexpr DataType GetData() const
	{
		return Data;
	}
};

template <class... TailTypes>
class TConstexprList;

template <class HeadType, class... TailTypes>
class TConstexprList<HeadType, TailTypes...> : /* private */ TListStorage<HeadType, TConstexprList<TailTypes...>>
{
public:
	constexpr TConstexprList() = default;

	explicit constexpr TConstexprList(HeadType Head, TailTypes... Tail) : Parent(std::move(Head), std::move(Tail)...)
	{
	}

	constexpr TConstexprList(HeadType Head, const TConstexprList<TailTypes...>& Tail) : Parent(std::move(Head), Tail)
	{
	}

	constexpr auto GetHead() const
	{
		return Parent::GetData();
	}

	constexpr decltype(auto) GetTail() const
	{
		return static_cast<const TConstexprList<TailTypes...>&>(*this);
	}

private:
	using Parent = TListStorage<HeadType, TConstexprList<TailTypes...>>;
};

template <>
class TConstexprList<>
{
public:
	constexpr TConstexprList() = default;
};

namespace ConstexprList
{

template <class... EntryTypes>
constexpr auto MakeConstexprList(EntryTypes... Entries)
{
	return TConstexprList<EntryTypes...>{std::move(Entries)...};
}

template <class... EntryTypes>
constexpr bool IsEmpty(TConstexprList<EntryTypes...>)
{
	return sizeof...(EntryTypes) == 0;
}

template <int Index, class... EntryTypes>
constexpr auto GetElement(TConstexprList<EntryTypes...> List)
{
	static_assert(Index < sizeof...(EntryTypes));
	static_assert(Index >= 0);

	if constexpr (Index == 0)
	{
		return List.GetHead();
	}
	else
	{
		return GetElement<Index - 1>(List.GetTail());
	}
}

template <class NewHeadType, class... EntryTypes>
constexpr auto Prepend(const TConstexprList<EntryTypes...>& List, NewHeadType NewHead)
{
	return TConstexprList<NewHeadType, EntryTypes...>{std::move(NewHead), List};
}

template <class... RhsEntriesType>
constexpr auto Concatenate(TConstexprList<>, TConstexprList<RhsEntriesType...> Rhs)
{
	return Rhs;
}

template <class LhsHeadType, class... LhsTailTypes, class... RhsEntriesType>
constexpr auto Concatenate(TConstexprList<LhsHeadType, LhsTailTypes...> Lhs, TConstexprList<RhsEntriesType...> Rhs)
{
	return Prepend(Concatenate(Lhs.GetTail(), Rhs), Lhs.GetHead());
}

template <class PredicateType>
constexpr auto AnyOf(TConstexprList<>, PredicateType)
{
	return false;
}

template <class PredicateType, class HeadType, class... TailTypes>
constexpr auto AnyOf(TConstexprList<HeadType, TailTypes...> List, PredicateType Predicate)
{
	return Predicate(HeadType{}) || AnyOf(List.GetTail(), Predicate);
}

template <class PredicateType, class... EntryTypes>
constexpr auto AllOf(TConstexprList<EntryTypes...> List, PredicateType Predicate)
{
	return !AnyOf(List, [Predicate](auto Element) { return !Predicate(Element); });
}

template <class PredicateType>
constexpr auto Find(TConstexprList<>, PredicateType Predicate)
{
	static_assert(AnyOf(TConstexprList{}, Predicate), "Element not found");
}

template <class HeadType, class... TailTypes, class PredicateType>
constexpr auto Find([[maybe_unused]] TConstexprList<HeadType, TailTypes...> List, PredicateType Predicate)
{
	if constexpr (Predicate(HeadType{}))
	{
		return List.GetHead();
	}
	else
	{
		return Find(List.GetTail(), Predicate);
	}
}

template <class PredicateType>
constexpr auto Filter(TConstexprList<>, PredicateType)
{
	return TConstexprList{};
}

template <class HeadType, class... TailTypes, class PredicateType>
constexpr auto Filter(TConstexprList<HeadType, TailTypes...> List, PredicateType Predicate)
{
	if constexpr (Predicate(HeadType{}))
	{
		return Prepend(Filter(List.GetTail(), Predicate), List.GetHead());
	}
	else
	{
		return Filter(List.GetTail(), Predicate);
	}
}

template <class FunctionType, class HeadType, class... TailTypes>
constexpr void ForEach(TConstexprList<HeadType, TailTypes...> List, FunctionType Function)
{
	Function(List.GetHead());
	ForEach(List.GetTail(), Function);
}

template <class Function>
constexpr void ForEach(TConstexprList<>, Function)
{
}

template <class TransformerType>
constexpr auto Transform(TConstexprList<>, TransformerType)
{
	return TConstexprList{};
}

template <class TransformerType, class HeadType, class... TailTypes>
constexpr auto Transform(TConstexprList<HeadType, TailTypes...> List, TransformerType Transformer)
{
	return Prepend(Transform(List.GetTail(), Transformer), Transformer(List.GetHead()));
}

template <class NeedleType>
constexpr bool Contains(TConstexprList<>, NeedleType)
{
	return false;
}

template <class NeedleType, class HeadType, class... TailTypes>
constexpr bool Contains(TConstexprList<HeadType, TailTypes...> List, NeedleType Needle)
{
	return List.GetHead() == Needle || Contains(List.GetTail(), Needle);
}

constexpr bool HasDuplicates(TConstexprList<>)
{
	return false;
}

template <class HeadType, class... TailTypes>
constexpr bool HasDuplicates(TConstexprList<HeadType, TailTypes...> List)
{
	return Contains(List.GetTail(), List.GetHead()) || HasDuplicates(List.GetTail());
}

template <class ListType>
struct TFlattenedConstexprList;

template <class HeadType, class... TailType>
struct TFlattenedConstexprList<TConstexprList<HeadType, TailType...>>
{
	using Type = decltype(Prepend(typename TFlattenedConstexprList<TConstexprList<TailType...>>::Type{}, HeadType{}));
};

template <class... HeadEntryTypes, class... TailEntryTypes>
struct TFlattenedConstexprList<TConstexprList<TConstexprList<HeadEntryTypes...>, TailEntryTypes...>>
{
	using Type = decltype(Concatenate(
		typename TFlattenedConstexprList<TConstexprList<HeadEntryTypes...>>::Type{},
		typename TFlattenedConstexprList<TConstexprList<TailEntryTypes...>>::Type{}));
};

template <>
struct TFlattenedConstexprList<TConstexprList<>>
{
	using Type = TConstexprList<>;
};

template <class... EntryTypes>
constexpr auto Flatten(TConstexprList<EntryTypes...> List)
{
	return typename TFlattenedConstexprList<decltype(List)>::Type{};
}

template <class ListType, class InitType, class FunctionType>
constexpr auto FoldLeft([[maybe_unused]] ListType List, InitType Init, [[maybe_unused]] FunctionType Function)
{
	if constexpr (IsEmpty(ListType{}))
	{
		return Init;
	}
	else
	{
		return FoldLeft(List.GetTail(), Function(Init, List.GetHead()), Function);
	}
}

template <class FunctionType, class... EntryTypes>
constexpr auto Unpack(TConstexprList<EntryTypes...>, FunctionType Function)
{
	return Function(EntryTypes{}...);
}

template <class... LhsEntryTypes, class... RhsEntryTypes>
constexpr auto IsSubset(TConstexprList<LhsEntryTypes...> NeedleList, TConstexprList<RhsEntryTypes...> HaystackList)
{
	return AllOf(NeedleList, [HaystackList](auto Element) { return Contains(HaystackList, Element); });
}

template <class... LhsEntryTypes, class... RhsEntryTypes>
constexpr auto Difference(TConstexprList<LhsEntryTypes...> Lhs, [[maybe_unused]] TConstexprList<RhsEntryTypes...> Rhs)
{
	return FoldLeft(
		Lhs,
		TConstexprList{},
		[](auto Result, auto Element)
		{
			if constexpr (Contains(TConstexprList<RhsEntryTypes...>{}, decltype(Element){}))
			{
				return Result;
			}
			else
			{
				return Prepend(Result, Element);
			}
		});
}

}  // namespace ConstexprList

}  // namespace SafeMath::Private
