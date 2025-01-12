#include "Misc/AutomationTest.h"
#include "SafeMath/Quantity/Unit.h"

#include <type_traits>

namespace SafeMath::Test
{

IMPLEMENT_COMPLEX_AUTOMATION_TEST(
	UnitTest, "SafeMath.UnitTest", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

void UnitTest::GetTests(TArray<FString>& OutBeautifiedNames, TArray<FString>& OutTestCommands) const
{
	OutTestCommands.Emplace(TEXT("Multiplication"));
	OutTestCommands.Emplace(TEXT("Division"));
	OutTestCommands.Emplace(TEXT("IsSameDimension"));
	OutTestCommands.Emplace(TEXT("GetConversionRateBetween"));
	Algo::Copy(OutTestCommands, OutBeautifiedNames);
}

bool UnitTest::RunTest(const FString& TestCommand)
{
	using namespace SafeMath;

	if (TestCommand == TEXT("Multiplication"))
	{
		constexpr auto CentimeterHours = Units::FCentimeters{} * Units::FHours{};
		static_assert(std::is_same_v<
					  std::decay_t<decltype(CentimeterHours)::DimensionType>,
					  decltype(Dimensions::FDistance{} * Dimensions::FTime{})>);
		TestNearlyEqual("CentimeterHoursConversionRate", CentimeterHours.ConversionRate, .01 * 3600.);
	}
	else if (TestCommand == TEXT("Division"))
	{
		constexpr auto CentimetersPerHour = Units::FCentimeters{} / Units::FHours{};
		static_assert(std::is_same_v<std::decay_t<decltype(CentimetersPerHour)::DimensionType>, Dimensions::FVelocity>);
		TestNearlyEqual("CentimetersPerHourConversionRate", CentimetersPerHour.ConversionRate, .01 / 3600.);
	}
	else if (TestCommand == TEXT("IsSameDimension"))
	{
		static_assert(IsSameDimension(Units::FKilometersPerHour{}, Units::FMetersPerSecond{}));
		static_assert(!IsSameDimension(Units::FMetersPerSecondPerSecond{}, Units::FMetersPerSecond{}));
	}
	else if (TestCommand == TEXT("GetConversionRateBetween"))
	{
		TestNearlyEqual(
			"MPSToKPH", GetConversionRateBetween(Units::FMetersPerSecond{}, Units::FKilometersPerHour{}), 3.6);
		TestNearlyEqual(
			"KPHToMPS", GetConversionRateBetween(Units::FKilometersPerHour{}, Units::FMetersPerSecond{}), 1. / 3.6);
	}
	else
	{
		AddError("Unrecognized Test: " + TestCommand);
	}

	return true;
}

}  // namespace SafeMath::Test
