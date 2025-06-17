#include "MathUtils.h"

namespace SafeMath
{

template <>
float Parse<float>(const FString& String)
{
	return FCString::Atof(*String);
}

template <>
double Parse<double>(const FString& String)
{
	return FCString::Atod(*String);
}

}  // namespace SafeMath
