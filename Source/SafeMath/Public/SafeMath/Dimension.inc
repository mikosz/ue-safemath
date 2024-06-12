#pragma once

#include "Dimension.h"

namespace SafeMath
{

template <class LhsBaseDimensionAndExponentListType, class RhsBaseDimensionAndExponentListType>
constexpr auto operator*(
	TDimension<LhsBaseDimensionAndExponentListType> Lhs, TDimension<RhsBaseDimensionAndExponentListType> Rhs)
{
	using namespace Private::ConstexprList;
	constexpr auto Merged = FoldLeft(
		typename decltype(Rhs)::BaseDimensionAndExponentListType{},
		typename decltype(Lhs)::BaseDimensionAndExponentListType{},
		[](auto Result, auto Element)
		{
			constexpr auto Found = Find(Result, Element.TypeMatches);
			if constexpr (IsNullElement(Found))
			{
				return Prepend(Result, Element);
			}
			else
			{
				return Prepend(
					RemoveIf(Result, Element.TypeMatches),
					TDimensionEntry<
						typename decltype(Element)::BaseDimensionType,
						Element.Exponent + Found.Exponent>{});
			}
		});
	constexpr auto MergedNoZeroes = RemoveIf(Merged, [](auto Element) { return Element.Exponent == 0; });

	return TDimension<std::decay_t<decltype(MergedNoZeroes)>>{};
}

template <class BaseDimensionAndExponentListType>
constexpr auto Inverse(TDimension<BaseDimensionAndExponentListType> Dimension)
{
	using namespace Private::ConstexprList;
	constexpr auto InverseBaseDimensionAndExponentListType = Transform(BaseDimensionAndExponentListType{},
		[](auto Element) { return TDimensionEntry<typename decltype(Element)::BaseDimensionType, -Element.Exponent>{}; });
	
	return TDimension<std::decay_t<decltype(InverseBaseDimensionAndExponentListType)>>{};
}

template <class LhsBaseDimensionAndExponentListType, class RhsBaseDimensionAndExponentListType>
constexpr auto operator/(
	TDimension<LhsBaseDimensionAndExponentListType> Lhs, TDimension<RhsBaseDimensionAndExponentListType> Rhs)
{
	return Lhs * Inverse(Rhs);
}

template <class LhsBaseDimensionAndExponentListType, class RhsBaseDimensionAndExponentListType>
constexpr bool operator==(TDimension<LhsBaseDimensionAndExponentListType> Lhs, TDimension<RhsBaseDimensionAndExponentListType> Rhs)
{
	using namespace Private;
	
	constexpr auto Quotient = Lhs / Rhs;
	using QuotientType = std::decay_t<decltype(Quotient)>;
	
	return std::is_same_v<QuotientType, TDimension<TConstexprList<>>>;
}

template <class LhsBaseDimensionAndExponentListType, class RhsBaseDimensionAndExponentListType>
constexpr bool operator!=(TDimension<LhsBaseDimensionAndExponentListType> Lhs, TDimension<RhsBaseDimensionAndExponentListType> Rhs)
{
	return !(Lhs == Rhs);
}

}  // namespace SafeMath
