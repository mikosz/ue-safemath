#include "Misc/AutomationTest.h"
#include "SafeMath/QuantityView.h"
#include "SafeMath/Unit.h"
#include "SafeMath/Utils.h"

#include <type_traits>

namespace SafeMath::Test
{

IMPLEMENT_COMPLEX_AUTOMATION_TEST(
	QuantityViewTest,
	"SafeMath.QuantityViewTest",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

void QuantityViewTest::GetTests(TArray<FString>& OutBeautifiedNames, TArray<FString>& OutTestCommands) const
{
	OutTestCommands.Emplace(TEXT("GetValue"));
	OutTestCommands.Emplace(TEXT("Comparison"));
	OutTestCommands.Emplace(TEXT("Multiplication"));
	OutTestCommands.Emplace(TEXT("Division"));
	OutTestCommands.Emplace(TEXT("MultiplicationByScalar"));
	OutTestCommands.Emplace(TEXT("DivisionByScalar"));
	OutTestCommands.Emplace(TEXT("Addition"));
	OutTestCommands.Emplace(TEXT("Subtraction"));
	Algo::Copy(OutTestCommands, OutBeautifiedNames);
}

// #TODO_dontcommit: test template deduction correct, views modify viewed data, implicit conversion from TQuantity works (also when calling free functions)
// #TODO_dontcommit: add and text quantity * view etc
bool QuantityViewTest::RunTest(const FString& TestCommand)
{
	using namespace SafeMath;

	if (TestCommand == TEXT("GetValue"))
	{
		constexpr auto Length = TQuantity{Units::FCentimeters{}, 100};
		static_assert(Length.GetValue<Units::FCentimeters>() == 100);
		static_assert(Length.GetValue<Units::FMeters>() == 1);

		const auto DirectionValue = FVector{1., 2., 3.};
		const auto Direction = TQuantityView{Units::FMeters{}, DirectionValue};

		TestNearlyEqual("Vector GetValue", Direction.GetValue<Units::FCentimeters>(), FVector{100., 200., 300.});

		// GetValue should not compile for different dimensions
		// Length.GetValue<Units::FKilometersPerHour>();
	}
	else if (TestCommand == TEXT("Comparison"))
	{
		static constexpr auto LengthValue = 2'000.;
		constexpr auto Length = TQuantityView{Units::FCentimeters{}, LengthValue};
		static constexpr auto LengthGreaterValue = 21.;
		constexpr auto LengthGreater = TQuantityView{Units::FMeters{}, LengthGreaterValue};
		static constexpr auto LengthEqualValue = .02;
		constexpr auto LengthEqual = TQuantityView{Units::FKilometers{}, LengthEqualValue};

		// ReSharper disable CppRedundantComplexityInComparison
		static constexpr auto LengthIValue = 20;
		constexpr auto LengthI = TQuantityView{Units::FMeters{}, LengthIValue};
		// Equality tests for int only
		static_assert(LengthI == Length);
		static_assert(!(LengthI != Length));

		// Comparison should not compile for different dimensions
		// constexpr auto Time = TValue{Units::FSeconds{}, 3.f};
		// Time == Length;
		// Time != Length;
		// Time < Length;
		// Time <= Length;
		// Time > Length;
		// Time >= Length;

		static_assert(NearlyEqual(Length, LengthEqual));
		static_assert(Length <= LengthEqual);
		static_assert(Length >= LengthEqual);
		static_assert(!(Length < LengthEqual));
		static_assert(!(Length > LengthEqual));

		static_assert(!NearlyEqual(Length, LengthGreater));
		static_assert(Length <= LengthGreater);
		static_assert(!(Length >= LengthGreater));
		static_assert(Length < LengthGreater);
		static_assert(!(Length > LengthGreater));

		static_assert(!NearlyEqual(LengthGreater, Length));
		static_assert(!(LengthGreater <= Length));
		static_assert(LengthGreater >= Length);
		static_assert(!(LengthGreater < Length));
		static_assert(LengthGreater > Length);

		const FVector DirectionValue{1., 2., 3.};
		const auto Direction = TQuantityView{Units::FMeters{}, DirectionValue};
		TestTrue("Vector NearlyEqual", NearlyEqual(Direction.GetValue<Units::FCentimeters>(), DirectionValue * 100.f));
		// ReSharper restore CppRedundantComplexityInComparison
	}
	else if (TestCommand == TEXT("Multiplication"))
	{
		static constexpr auto LengthAValue = 20.f;
		constexpr auto LengthA = TQuantityView{Units::FMeters{}, LengthAValue};
		static constexpr auto LengthBValue = 50.f;

		constexpr auto LengthB = TQuantity{Units::FCentimeters{}, LengthBValue};
		constexpr auto AreaABFromQuantity = LengthA * LengthB;
		constexpr auto AreaBAFromQuantity = LengthB * LengthA;

		static_assert(NearlyEqual(AreaABFromQuantity.GetValue<Units::FMetersSquared>(), 10.));
		static_assert(NearlyEqual(AreaABFromQuantity.GetValue<Units::FCentimetersSquared>(), 100'000.));

		static_assert(NearlyEqual(AreaBAFromQuantity.GetValue<Units::FMetersSquared>(), 10.));
		static_assert(NearlyEqual(AreaBAFromQuantity.GetValue<Units::FCentimetersSquared>(), 100'000.));

		// #TODO_dontcommit: also do the division for quantity / view tests for addition / subtraction / comparison
		constexpr auto LengthBView = TQuantityView{Units::FCentimeters{}, LengthBValue};
		constexpr auto AreaABFromView = LengthA * LengthBView;
		constexpr auto AreaBAFromView = LengthBView * LengthA;

		static_assert(NearlyEqual(AreaABFromView.GetValue<Units::FMetersSquared>(), 10.));
		static_assert(NearlyEqual(AreaABFromView.GetValue<Units::FCentimetersSquared>(), 100'000.));

		static_assert(NearlyEqual(AreaBAFromView.GetValue<Units::FMetersSquared>(), 10.));
		static_assert(NearlyEqual(AreaBAFromView.GetValue<Units::FCentimetersSquared>(), 100'000.));
	}
	else if (TestCommand == TEXT("Division"))
	{
		static constexpr auto DistanceValue = 20.;
		constexpr auto Distance = TQuantityView{Units::FMeters{}, DistanceValue};
		static constexpr auto TimeValue = 5.;

		constexpr auto Time = TQuantity{Units::FSeconds{}, TimeValue};
		constexpr auto VelocityFromQuantity = Distance / Time;
		static_assert(NearlyEqual(VelocityFromQuantity.GetValue<Units::FMetersPerSecond>(), 4.));
		static_assert(NearlyEqual(VelocityFromQuantity.GetValue<Units::FKilometersPerHour>(), 14.4));

		constexpr auto TimeView = TQuantityView{Units::FSeconds{}, TimeValue};
		constexpr auto VelocityFromView = Distance / TimeView;
		static_assert(NearlyEqual(VelocityFromView.GetValue<Units::FMetersPerSecond>(), 4.));
		static_assert(NearlyEqual(VelocityFromView.GetValue<Units::FKilometersPerHour>(), 14.4));
	}
	else if (TestCommand == TEXT("Addition"))
	{
		static constexpr auto LengthAValue = 20.;
		constexpr auto LengthA = TQuantityView{Units::FMeters{}, LengthAValue};
		static constexpr auto LengthBValue = 50.;
		constexpr auto LengthB = TQuantityView{Units::FCentimeters{}, LengthBValue};
		constexpr auto SumAB = LengthA + LengthB;
		constexpr auto SumBA = LengthB + LengthA;

		static_assert(NearlyEqual(SumAB.GetValue<Units::FMeters>(), 20.5));
		static_assert(NearlyEqual(SumAB.GetValue<Units::FCentimeters>(), 2'050.));

		static_assert(NearlyEqual(SumBA.GetValue<Units::FMeters>(), 20.5));
		static_assert(NearlyEqual(SumBA.GetValue<Units::FCentimeters>(), 2'050.));

		auto LengthACopyValue = LengthAValue;
		auto LengthACopy = TQuantityView{Units::FMeters{}, LengthACopyValue};
		LengthACopy += LengthB;
		TestNearlyEqual("TValue +=", LengthACopy.GetValue<Units::FMeters>(), 20.5);

		// + should not compile for different dimensions
		// constexpr auto Velocity = TValue{Units::FKilometersPerHour{}, 20.};
		// LengthA + Velocity;
	}
	else if (TestCommand == TEXT("Subtraction"))
	{
		static constexpr auto LengthAValue = 20.;
		constexpr auto LengthA = TQuantityView{Units::FMeters{}, LengthAValue};
		static constexpr auto LengthBValue = 50.;
		constexpr auto LengthB = TQuantityView{Units::FCentimeters{}, LengthBValue};
		constexpr auto SumAB = LengthA - LengthB;
		constexpr auto SumBA = LengthB - LengthA;

		static_assert(NearlyEqual(SumAB.GetValue<Units::FMeters>(), 19.5));
		static_assert(NearlyEqual(SumAB.GetValue<Units::FCentimeters>(), 1'950.));

		static_assert(NearlyEqual(SumBA.GetValue<Units::FMeters>(), -19.5));
		static_assert(NearlyEqual(SumBA.GetValue<Units::FCentimeters>(), -1'950.));

		auto LengthACopyValue = LengthAValue;
		auto LengthACopy = TQuantityView{Units::FMeters{}, LengthACopyValue};
		LengthACopy -= LengthB;
		TestNearlyEqual("TValue -=", LengthACopy.GetValue<Units::FMeters>(), 19.5);

		// + should not compile for different dimensions
		// constexpr auto Velocity = TValue{Units::FKilometersPerHour{}, 20.};
		// LengthA - Velocity;
	}
	else if (TestCommand == TEXT("MultiplicationByScalar"))
	{
		static constexpr auto LengthValue = 42;
		constexpr auto Length = TQuantityView{Units::FMeters{}, LengthValue};

		static_assert((Length * 2).GetValue<Units::FMeters>() == 84);
		static_assert((2 * Length).GetValue<Units::FMeters>() == 84);

		auto LengthValueCopy = LengthValue;
		auto LengthCopy = TQuantityView{Units::FMeters{}, LengthValueCopy};
		LengthCopy *= 2;
		TestEqual("TValue *=", LengthCopy.GetValue<Units::FMeters>(), 84);
	}
	else if (TestCommand == TEXT("DivisionByScalar"))
	{
		static constexpr auto LengthValue = 42;
		constexpr auto Length = TQuantityView{Units::FMeters{}, LengthValue};

		static_assert((Length / 2).GetValue<Units::FMeters>() == 21);

		auto LengthValueCopy = LengthValue;
		auto LengthCopy = TQuantityView{Units::FMeters{}, LengthValueCopy};
		LengthCopy /= 2;
		TestEqual("TValue *=", LengthCopy.GetValue<Units::FMeters>(), 21);
	}
	else
	{
		AddError("Unrecognized Test: " + TestCommand);
	}

	return true;
}

}  // namespace SafeMath::Test
