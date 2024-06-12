#include "Misc/AutomationTest.h"
#include "SafeMath/Dimension.h"

#include <type_traits>

namespace SafeMath::Test
{
IMPLEMENT_COMPLEX_AUTOMATION_TEST(
	DimensionTest, "SafeMath.DimensionTest", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

void DimensionTest::GetTests(TArray<FString>& OutBeautifiedNames, TArray<FString>& OutTestCommands) const
{
	OutTestCommands.Emplace(TEXT("MultiplicationMergesDimensions"));
	OutTestCommands.Emplace(TEXT("DivisionMergesDimensionsWithInverse"));
	OutTestCommands.Emplace(TEXT("EqualityComparison"));
	Algo::Copy(OutTestCommands, OutBeautifiedNames);
}

bool DimensionTest::RunTest(const FString& Parameters)
{
	using namespace SafeMath;

	if (Parameters == TEXT("MultiplicationMergesDimensions"))
	{
		constexpr auto Mass = MakeDimension<BaseDimensions::FMass, 1>();
		constexpr auto Time = MakeDimension<BaseDimensions::FTime, 1>();
		constexpr auto MassTime = Mass * Time;
		constexpr auto MassTimeExpected = Private::ConstexprList::MakeConstexprList(
			TDimensionEntry<BaseDimensions::FTime, 1>{}, TDimensionEntry<BaseDimensions::FMass, 1>{});

		static_assert(
			std::is_same_v<typename decltype(MassTime)::BaseDimensionAndExponentListType, std::decay_t<decltype(MassTimeExpected)>>);

		constexpr auto InverseTime = MakeDimension<BaseDimensions::FTime, -1>();
		constexpr auto Distance = MakeDimension<BaseDimensions::FDistance, 1>();
		constexpr auto Force = Mass * InverseTime * Distance * InverseTime;
		constexpr auto ForceExpected = Private::ConstexprList::MakeConstexprList(
			TDimensionEntry<BaseDimensions::FTime, -2>{},
			TDimensionEntry<BaseDimensions::FDistance, 1>{},
			TDimensionEntry<BaseDimensions::FMass, 1>{});

		static_assert(
			std::is_same_v<typename decltype(Force)::BaseDimensionAndExponentListType, std::decay_t<decltype(ForceExpected)>>);

		constexpr auto InverseMass = MakeDimension<BaseDimensions::FMass, -1>();
		constexpr auto Acceleration = InverseMass * Force;
		constexpr auto AccelerationExpected = Private::ConstexprList::MakeConstexprList(
			TDimensionEntry<BaseDimensions::FDistance, 1>{}, TDimensionEntry<BaseDimensions::FTime, -2>{});

		static_assert(std::is_same_v<
					  typename decltype(Acceleration)::BaseDimensionAndExponentListType,
					  std::decay_t<decltype(AccelerationExpected)>>);
	}
	else if (Parameters == TEXT("DivisionMergesDimensionsWithInverse"))
	{
		constexpr auto Mass = MakeDimension<BaseDimensions::FMass, 1>();
		constexpr auto Distance = MakeDimension<BaseDimensions::FDistance, 1>();
		constexpr auto Time = MakeDimension<BaseDimensions::FTime, 1>();

		constexpr auto Force = (Distance / Time) / (Time / Mass);
		constexpr auto ForceExpected = Private::ConstexprList::MakeConstexprList(
			TDimensionEntry<BaseDimensions::FTime, -2>{},
			TDimensionEntry<BaseDimensions::FMass, 1>{},
			TDimensionEntry<BaseDimensions::FDistance, 1>{});

		static_assert(
			std::is_same_v<typename decltype(Force)::BaseDimensionAndExponentListType, std::decay_t<decltype(ForceExpected)>>);
	}
	else if (Parameters == TEXT("EqualityComparison"))
	{
		constexpr auto Mass = MakeDimension<BaseDimensions::FMass, 1>();
		constexpr auto Distance = MakeDimension<BaseDimensions::FDistance, 1>();
		constexpr auto Time = MakeDimension<BaseDimensions::FTime, 1>();

		constexpr auto Force = Mass * Distance / Time / Time;
		constexpr auto Force2 = Distance / Time * (Mass / Time);
		constexpr auto Speed = Distance / Time;

		static_assert(!std::is_same_v<decltype(Force), decltype(Force2)>);
		static_assert(Force == Force2);
		// static_assert(!(Force != Force2));
		// static_assert(!(Force == Speed));
		// static_assert(Force != Speed);
	}

	return true;
}

}  // namespace SafeMath::Test