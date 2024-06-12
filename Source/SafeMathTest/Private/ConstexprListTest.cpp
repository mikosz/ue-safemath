// Copyright Mikolaj Radwan 2024
// Distributed under the Apache 2.0 license (See accompanying file LICENSE)

#include "Misc/AutomationTest.h"
#include "SafeMath/Private/ConstexprList.h"

#include <type_traits>

namespace SafeMath::Private::ConstexprList::Test
{

template <int I>
struct FIntWrapper
{
	static constexpr auto Value = I;
};

template <int I>
struct FOtherIntWrapper
{
	static constexpr auto Value = I;
};

template <int I, int J>
constexpr bool operator==(FIntWrapper<I>, FIntWrapper<J>)
{
	return I == J;
}

template <int I>
struct FIntPredicate
{
	constexpr FIntPredicate() = default;

	template <class T>
	constexpr bool operator()(const T& Element) const
	{
		return Element == FIntWrapper<I>{};
	}
};

template <class T>
struct FTypePredicate
{
	constexpr FTypePredicate() = default;

	template <class O>
	constexpr bool operator()(const O&) const
	{
		return std::is_same_v<T, O>;
	}
};

void IsConstexprListIsValid()
{
	static_assert(!IsConstexprList(42));
	static_assert(IsConstexprList(TConstexprList{}));
	static_assert(IsConstexprList(MakeConstexprList(42, 2.0, 'a')));
}

void ListOfEmptyElementsHasSize1()
{
	constexpr auto OneTwoThree = MakeConstexprList(FIntWrapper<1>{}, FIntWrapper<2>{}, FIntWrapper<3>{});
	static_assert(sizeof(OneTwoThree) == 1);
}

void ListOfUnemptyElementsHasSizeOfSumOfElements()
{
	// This may not be true in cases where elements are of different sizes, due
	// to padding, but this is fine for our needs
	constexpr auto OneTwoThree = MakeConstexprList(1.0f, 2.0f, 3.0f);
	static_assert(sizeof(OneTwoThree) == 3 * sizeof(float));
}

void PrependPutsElementsInFront()
{
	constexpr auto OneTwoThreeEmpty =
		Prepend(Prepend(Prepend(TConstexprList{}, FIntWrapper<3>{}), FIntWrapper<2>{}), FIntWrapper<1>{});
	static_assert(std::is_same_v<
				  std::decay_t<decltype(OneTwoThreeEmpty)>,
				  TConstexprList<FIntWrapper<1>, FIntWrapper<2>, FIntWrapper<3>>>);

	constexpr auto OneTwoThreeNonempty = Prepend(Prepend(Prepend(TConstexprList{}, 3), 2.0), '1');
	static_assert(std::is_same_v<std::decay_t<decltype(OneTwoThreeNonempty)>, TConstexprList<char, double, int>>);
	static_assert(GetElement<0>(OneTwoThreeNonempty) == '1');
	static_assert(GetElement<1>(OneTwoThreeNonempty) == 2.0);
	static_assert(GetElement<2>(OneTwoThreeNonempty) == 3);
}

void MakeConstexprListInsertsAllElements()
{
	constexpr auto OneTwoThreeOneEmpty =
		MakeConstexprList(FIntWrapper<1>{}, FIntWrapper<2>{}, FIntWrapper<3>{}, FIntWrapper<1>{});
	static_assert(std::is_same_v<
				  std::decay_t<decltype(OneTwoThreeOneEmpty)>,
				  TConstexprList<FIntWrapper<1>, FIntWrapper<2>, FIntWrapper<3>, FIntWrapper<1>>>);

	constexpr auto OneTwoThreeNonempty = MakeConstexprList(1, 2.0, '3');
	static_assert(std::is_same_v<std::decay_t<decltype(OneTwoThreeNonempty)>, TConstexprList<int, double, char>>);
	static_assert(GetElement<0>(OneTwoThreeNonempty) == 1);
	static_assert(GetElement<1>(OneTwoThreeNonempty) == 2.0);
	static_assert(GetElement<2>(OneTwoThreeNonempty) == '3');
}

void ConcatenateMergesTwoLists()
{
	constexpr auto OneTwoThreeFourEmpty =
		Concatenate(TConstexprList<FIntWrapper<1>, FIntWrapper<2>>{}, TConstexprList<FIntWrapper<3>, FIntWrapper<4>>{});
	static_assert(std::is_same_v<
				  std::decay_t<decltype(OneTwoThreeFourEmpty)>,
				  TConstexprList<FIntWrapper<1>, FIntWrapper<2>, FIntWrapper<3>, FIntWrapper<4>>>);

	constexpr auto OneTwoThreeFourNonempty = Concatenate(MakeConstexprList(1, 2), MakeConstexprList(3.0, 4.0));
	static_assert(GetElement<0>(OneTwoThreeFourNonempty) == 1);
	static_assert(GetElement<1>(OneTwoThreeFourNonempty) == 2);
	static_assert(GetElement<2>(OneTwoThreeFourNonempty) == 3.0);
	static_assert(GetElement<3>(OneTwoThreeFourNonempty) == 4.0);
}

void FindReturnsMatchingElement()
{
	constexpr auto OneTwoThreeEmpty = TConstexprList<FIntWrapper<1>, FIntWrapper<2>, FIntWrapper<3>>{};
	static_assert(Find(OneTwoThreeEmpty, FIntPredicate<1>{}) == FIntWrapper<1>{});
	static_assert(Find(OneTwoThreeEmpty, FIntPredicate<2>{}) == FIntWrapper<2>{});
	static_assert(Find(OneTwoThreeEmpty, FIntPredicate<3>{}) == FIntWrapper<3>{});
	static_assert(IsNullElement(Find(OneTwoThreeEmpty, FIntPredicate<4>{})));

	constexpr auto OneTwoThreeNonempty = MakeConstexprList(1, 2.0, 3.0f);

	static_assert(Find(OneTwoThreeNonempty, FTypePredicate<int>{}) == 1);
	static_assert(Find(OneTwoThreeNonempty, FTypePredicate<double>{}) == 2.0);
	static_assert(Find(OneTwoThreeNonempty, FTypePredicate<float>{}) == 3.0f);
	static_assert(IsNullElement(Find(OneTwoThreeNonempty, FTypePredicate<char>{})));
}

void FilterGetsElementsSatisfyingPredicate()
{
	constexpr auto OneTwoThreeFourEmpty =
		MakeConstexprList(FIntWrapper<1>{}, FIntWrapper<2>{}, FIntWrapper<3>{}, FIntWrapper<4>{});
	constexpr auto TwoFour = RemoveIf(OneTwoThreeFourEmpty, [](auto Element) { return Element.Value % 2 != 0; });
	static_assert(std::is_same_v<std::decay_t<decltype(TwoFour)>, TConstexprList<FIntWrapper<2>, FIntWrapper<4>>>);

	constexpr auto OneTwoThreeFourNonempty = MakeConstexprList(1, 2.0, 3.0f);
	constexpr auto TwoThree =
		RemoveIf(OneTwoThreeFourNonempty, [](auto Element) { return !std::is_floating_point_v<decltype(Element)>; });
	static_assert(GetElement<0>(TwoThree) == 2.0);
	static_assert(GetElement<1>(TwoThree) == 3.0f);
}

void TransformReturnsListWithTransformedElements()
{
	constexpr auto OneTwoThree = MakeConstexprList(FIntWrapper<1>{}, FIntWrapper<2>{}, FIntWrapper<3>{});
	constexpr auto OtherOneTwoThree =
		Transform(OneTwoThree, [](auto Element) { return FOtherIntWrapper<decltype(Element)::Value>{}; });
	static_assert(std::is_same_v<
				  std::decay_t<decltype(OtherOneTwoThree)>,
				  TConstexprList<FOtherIntWrapper<1>, FOtherIntWrapper<2>, FOtherIntWrapper<3>>>);
}

void ContainsReturnsTrueIfListContainsElement()
{
	constexpr auto OneTwoThree = MakeConstexprList(FIntWrapper<1>{}, FIntWrapper<2>{}, FIntWrapper<3>{});
	static_assert(Contains(OneTwoThree, FIntWrapper<1>{}));
	static_assert(Contains(OneTwoThree, FIntWrapper<2>{}));
	static_assert(Contains(OneTwoThree, FIntWrapper<3>{}));
	static_assert(!Contains(OneTwoThree, FIntWrapper<4>{}));
}

void EmptyReturnsTrueIfListIsEmpty()
{
	constexpr auto Nothing = MakeConstexprList();
	static_assert(IsEmpty(Nothing));
	constexpr auto OneTwoThree = MakeConstexprList(FIntWrapper<1>{}, FIntWrapper<2>{}, FIntWrapper<3>{});
	static_assert(!IsEmpty(OneTwoThree));
}

void HasDuplicatesReturnsTrueIfListContainsDuplicates()
{
	constexpr auto WithoutDuplicates = TConstexprList<FIntWrapper<1>, FIntWrapper<2>, FIntWrapper<3>>{};
	static_assert(!HasDuplicates(WithoutDuplicates));
	static_assert(HasDuplicates(Concatenate(WithoutDuplicates, TConstexprList<FIntWrapper<1>>{})));
	static_assert(HasDuplicates(Concatenate(WithoutDuplicates, TConstexprList<FIntWrapper<2>>{})));
	static_assert(HasDuplicates(Concatenate(WithoutDuplicates, TConstexprList<FIntWrapper<3>>{})));
}

void AnyOfReturnsTrueIfElementsSatisfyingPredicateExists()
{
	constexpr auto OneTwoThree = MakeConstexprList(FIntWrapper<1>{}, FIntWrapper<2>{}, FIntWrapper<3>{});

	static_assert(AnyOf(OneTwoThree, [](auto Element) { return Element.Value == 1; }));
	static_assert(AnyOf(OneTwoThree, [](auto Element) { return Element.Value == 2; }));
	static_assert(AnyOf(OneTwoThree, [](auto Element) { return Element.Value == 3; }));
	static_assert(!AnyOf(OneTwoThree, [](auto Element) { return Element.Value == 4; }));
}

void AllOfReturnsTrueIfNoElementNotSatisfyingPredicateExists()
{
	constexpr auto OneTwoThree = MakeConstexprList(FIntWrapper<1>{}, FIntWrapper<2>{}, FIntWrapper<3>{});

	static_assert(AllOf(OneTwoThree, [](auto Element) { return Element.Value < 4; }));
	static_assert(!AllOf(OneTwoThree, [](auto Element) { return Element.Value < 3; }));
}

void FlattenCreatesAFlatList()
{
	constexpr auto Nested = TConstexprList<
		FIntWrapper<1>,
		TConstexprList<TConstexprList<FIntWrapper<2>, FIntWrapper<3>>, FIntWrapper<4>>,
		FIntWrapper<5>>{};
	static_assert(std::is_same_v<
				  std::decay_t<decltype(Flatten(Nested))>,
				  TConstexprList<FIntWrapper<1>, FIntWrapper<2>, FIntWrapper<3>, FIntWrapper<4>, FIntWrapper<5>>>);
}

void FoldLeftCallsFunctorFoldingLeft()
{
	constexpr auto List = MakeConstexprList(FIntWrapper<1>{}, FIntWrapper<2>{}, FIntWrapper<3>{});
	constexpr auto Folded = FoldLeft(
		List,
		FIntWrapper<0>{},
		[](auto Lhs, auto Rhs) { return FIntWrapper<decltype(Lhs)::Value * 10 + decltype(Rhs)::Value>{}; });
	static_assert(std::is_same_v<std::decay_t<decltype(Folded)>, FIntWrapper<123>>);
}

void FoldLeftReturnsInitIfListEmpty()
{
	constexpr auto List = MakeConstexprList();
	constexpr auto Folded = FoldLeft(List, FIntWrapper<0>{}, [] {});
	static_assert(std::is_same_v<std::decay_t<decltype(Folded)>, FIntWrapper<0>>);
}

void UnpackCallsFunctionPassingAllElements()
{
	constexpr auto l = MakeConstexprList(FIntWrapper<1>{}, FIntWrapper<2>{}, FIntWrapper<3>{});
	constexpr auto r = Unpack(l, [](FIntWrapper<1>, FIntWrapper<2>, FIntWrapper<3>) { return 42; });
	static_assert(r == 42);
}

void IsSubsetReturnsTrueIfRhsContainsAllElementsOfLhs()
{
	constexpr auto Sub = MakeConstexprList(FIntWrapper<1>{}, FIntWrapper<2>{}, FIntWrapper<3>{});
	constexpr auto Super = MakeConstexprList(FIntWrapper<3>{}, FIntWrapper<1>{}, FIntWrapper<2>{}, FIntWrapper<4>{});

	static_assert(IsSubset(Sub, Sub));
	static_assert(IsSubset(Sub, Super));
	static_assert(!IsSubset(Super, Sub));
}

void DifferenceReturnsLhsMinusRhs()
{
	constexpr auto Lhs =
		MakeConstexprList(FIntWrapper<1>{}, FIntWrapper<2>{}, FIntWrapper<6>{}, FIntWrapper<3>{}, FIntWrapper<5>{});
	constexpr auto Rhs = MakeConstexprList(FIntWrapper<3>{}, FIntWrapper<1>{}, FIntWrapper<2>{}, FIntWrapper<4>{});
	constexpr auto Diff = Difference(Lhs, Rhs);

	static_assert(std::is_same_v<std::decay_t<decltype(Diff)>, TConstexprList<FIntWrapper<5>, FIntWrapper<6>>>);
}

IMPLEMENT_COMPLEX_AUTOMATION_TEST(
	ConstexprListTest,
	"SafeMath.Private.ConstexprList.Test",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

void ConstexprListTest::GetTests(TArray<FString>& OutBeautifiedNames, TArray<FString>& OutTestCommands) const
{
	OutTestCommands.Emplace(TEXT("IsConstexprList"));
	OutTestCommands.Emplace(TEXT("ListOfEmptyElementsHasSize1"));
	OutTestCommands.Emplace(TEXT("ListOfUnemptyElementsHasSizeOfSumOfElements"));
	OutTestCommands.Emplace(TEXT("PrependPutsElementsInFront"));
	OutTestCommands.Emplace(TEXT("MakeConstexprListInsertsAllElements"));
	OutTestCommands.Emplace(TEXT("ConcatenateMergesTwoLists"));
	OutTestCommands.Emplace(TEXT("FindReturnsMatchingElement"));
	OutTestCommands.Emplace(TEXT("FilterGetsElementsSatisfyingPredicate"));
	OutTestCommands.Emplace(TEXT("TransformReturnsListWithTransformedElements"));
	OutTestCommands.Emplace(TEXT("ContainsReturnsTrueIfListContainsElement"));
	OutTestCommands.Emplace(TEXT("EmptyReturnsTrueIfListIsEmpty"));
	OutTestCommands.Emplace(TEXT("HasDuplicatesReturnsTrueIfListContainsDuplicates"));
	OutTestCommands.Emplace(TEXT("AnyOfReturnsTrueIfElementsSatisfyingPredicateExists"));
	OutTestCommands.Emplace(TEXT("AllOfReturnsTrueIfNoElementNotSatisfyingPredicateExists"));
	OutTestCommands.Emplace(TEXT("FlattenCreatesAFlatList"));
	OutTestCommands.Emplace(TEXT("FoldLeftCallsFunctorFoldingLeft"));
	OutTestCommands.Emplace(TEXT("FoldLeftReturnsInitIfListEmpty"));
	OutTestCommands.Emplace(TEXT("UnpackCallsFunctionPassingAllElements"));
	OutTestCommands.Emplace(TEXT("IsSubsetReturnsTrueIfRhsContainsAllElementsOfLhs"));
	OutTestCommands.Emplace(TEXT("DifferenceReturnsLhsMinusRhs"));

	Algo::Copy(OutTestCommands, OutBeautifiedNames);
}

bool ConstexprListTest::RunTest(const FString& Parameters)
{
	using namespace SafeMath;

	if (Parameters == TEXT("IsConstexprList"))
	{
		IsConstexprListIsValid();
	}
	if (Parameters == TEXT("ListOfEmptyElementsHasSize1"))
	{
		ListOfEmptyElementsHasSize1();
	}
	else if (Parameters == TEXT("ListOfUnemptyElementsHasSizeOfSumOfElements"))
	{
		ListOfUnemptyElementsHasSizeOfSumOfElements();
	}
	else if (Parameters == TEXT("PrependPutsElementsInFront"))
	{
		PrependPutsElementsInFront();
	}
	else if (Parameters == TEXT("MakeConstexprListInsertsAllElements"))
	{
		MakeConstexprListInsertsAllElements();
	}
	else if (Parameters == TEXT("ConcatenateMergesTwoLists"))
	{
		ConcatenateMergesTwoLists();
	}
	else if (Parameters == TEXT("FindReturnsMatchingElement"))
	{
		FindReturnsMatchingElement();
	}
	else if (Parameters == TEXT("FilterGetsElementsSatisfyingPredicate"))
	{
		FilterGetsElementsSatisfyingPredicate();
	}
	else if (Parameters == TEXT("TransformReturnsListWithTransformedElements"))
	{
		TransformReturnsListWithTransformedElements();
	}
	else if (Parameters == TEXT("ContainsReturnsTrueIfListContainsElement"))
	{
		ContainsReturnsTrueIfListContainsElement();
	}
	else if (Parameters == TEXT("EmptyReturnsTrueIfListIsEmpty"))
	{
		EmptyReturnsTrueIfListIsEmpty();
	}
	else if (Parameters == TEXT("HasDuplicatesReturnsTrueIfListContainsDuplicates"))
	{
		HasDuplicatesReturnsTrueIfListContainsDuplicates();
	}
	else if (Parameters == TEXT("AnyOfReturnsTrueIfElementsSatisfyingPredicateExists"))
	{
		AnyOfReturnsTrueIfElementsSatisfyingPredicateExists();
	}
	else if (Parameters == TEXT("AllOfReturnsTrueIfNoElementNotSatisfyingPredicateExists"))
	{
		AllOfReturnsTrueIfNoElementNotSatisfyingPredicateExists();
	}
	else if (Parameters == TEXT("FlattenCreatesAFlatList"))
	{
		FlattenCreatesAFlatList();
	}
	else if (Parameters == TEXT("FoldLeftCallsFunctorFoldingLeft"))
	{
		FoldLeftCallsFunctorFoldingLeft();
	}
	else if (Parameters == TEXT("FoldLeftReturnsInitIfListEmpty"))
	{
		FoldLeftReturnsInitIfListEmpty();
	}
	else if (Parameters == TEXT("UnpackCallsFunctionPassingAllElements"))
	{
		UnpackCallsFunctionPassingAllElements();
	}
	else if (Parameters == TEXT("IsSubsetReturnsTrueIfRhsContainsAllElementsOfLhs"))
	{
		IsSubsetReturnsTrueIfRhsContainsAllElementsOfLhs();
	}
	else if (Parameters == TEXT("DifferenceReturnsLhsMinusRhs"))
	{
		DifferenceReturnsLhsMinusRhs();
	}

	return true;
}

}  // namespace SafeMath::Private::ConstexprList::Test
