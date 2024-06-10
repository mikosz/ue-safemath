// #pragma once
//
// #include "Dimension.h"
// #include "Dimension.impl.h"
// #include "Private/TypeList.h"
//
// namespace SafeMath
// {
//
// namespace Private
// {
//
// template <class T>
// struct TNegatedDimension;
//
// template <class BaseDimensionType, int Exponent>
// struct TNegatedDimension<TBaseDimensionWithExponent<BaseDimensionType, Exponent>>
// {
// 	using Type = TBaseDimensionWithExponent<BaseDimensionType, -Exponent>;
// };
//
// template <class T>
// using TNegatedDimension_t = typename TNegatedDimension<T>::Type;
//
// }  // namespace Private
//
// template <class... BaseDimensionTypes>
// constexpr auto TDimension<BaseDimensionTypes...>::Inverse() const
// {
// 	return TDimension<Private::TNegatedDimension_t<BaseDimensionTypes>...>{};
// }
//
// template <class... BaseDimensionTypes>
// template <class... OtherDimensionTypes>
// constexpr auto TDimension<BaseDimensionTypes...>::operator*(TDimension<OtherDimensionTypes...> OtherDimension) const
// {
// 	constexpr auto ThisTypeList = Private::TTypeList<BaseDimensionTypes...>{};
// 	Private::Combine<OtherDimensionTypes>(ThisTypeList);
// }
//
// template <class... BaseDimensionTypes>
// template <class... OtherDimensionTypes>
// constexpr auto TDimension<BaseDimensionTypes...>::operator/(TDimension<OtherDimensionTypes...> OtherDimension) const
// {
// 	return *this * OtherDimension.Inverse();
// }
//
// template <class BaseDimensionType, int Exponent>
// constexpr auto MakeDimension()
// {
// 	return TDimension<Private::TBaseDimensionWithExponent<BaseDimensionType, Exponent>>{};
// }
//
// }  // namespace SafeMath
