#include "Misc/AutomationTest.h"
#include "SafeMath/Quantity.h"
#include "SafeMath/Utils.h"

#include <type_traits>

namespace SafeMath::Test
{

IMPLEMENT_COMPLEX_AUTOMATION_TEST(
	QuantityTest, "SafeMath.QuantityTest", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

void QuantityTest::GetTests(TArray<FString>& OutBeautifiedNames, TArray<FString>& OutTestCommands) const
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

bool QuantityTest::RunTest(const FString& TestCommand)
{
	using namespace SafeMath;

	if (TestCommand == TEXT("GetValue"))
	{
		constexpr auto Length = TQuantity{Units::FCentimeters{}, 100};
		static_assert(Length.GetValue<Units::FCentimeters>() == 100);
		static_assert(Length.GetValue<Units::FMeters>() == 1);

		const auto Direction = TQuantity{Units::FMeters{}, FVector{1., 2., 3.}};
		TestNearlyEqual("Vector GetValue", Direction.GetValue<Units::FCentimeters>(), FVector{100., 200., 300.});

		// GetValue should not compile for different dimensions
		// Length.GetValue<Units::FKilometersPerHour>();
	}
	else if (TestCommand == TEXT("Comparison"))
	{
		constexpr auto Length = TQuantity{Units::FCentimeters{}, 2'000.};
		constexpr auto LengthGreater = TQuantity{Units::FMeters{}, 21.};
		constexpr auto LengthEqual = TQuantity{Units::FKilometers{}, 0.02};

		// ReSharper disable CppRedundantComplexityInComparison
		constexpr auto LengthI = TQuantity{Units::FMeters{}, 20};
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

		const auto Direction = TQuantity{Units::FMeters{}, FVector{1., 2., 3.}};
		TestTrue(
			"Vector NearlyEqual", NearlyEqual(Direction.GetValue<Units::FCentimeters>(), FVector{100., 200., 300.}));
		// ReSharper restore CppRedundantComplexityInComparison
	}
	else if (TestCommand == TEXT("Multiplication"))
	{
		constexpr auto LengthA = TQuantity{Units::FMeters{}, 20.f};
		constexpr auto LengthB = TQuantity{Units::FCentimeters{}, 50.f};
		constexpr auto AreaAB = LengthA * LengthB;
		constexpr auto AreaBA = LengthB * LengthA;

		static_assert(NearlyEqual(AreaAB.GetValue<Units::FMetersSquared>(), 10.));
		static_assert(NearlyEqual(AreaAB.GetValue<Units::FCentimetersSquared>(), 100'000.));

		static_assert(NearlyEqual(AreaBA.GetValue<Units::FMetersSquared>(), 10.));
		static_assert(NearlyEqual(AreaBA.GetValue<Units::FCentimetersSquared>(), 100'000.));
	}
	else if (TestCommand == TEXT("Division"))
	{
		constexpr auto Distance = TQuantity{Units::FMeters{}, 20.};
		constexpr auto Time = TQuantity{Units::FSeconds{}, 5.};
		constexpr auto Velocity = Distance / Time;
		static_assert(NearlyEqual(Velocity.GetValue<Units::FMetersPerSecond>(), 4.));
		static_assert(NearlyEqual(Velocity.GetValue<Units::FKilometersPerHour>(), 14.4));
	}
	else if (TestCommand == TEXT("Addition"))
	{
		constexpr auto LengthA = TQuantity{Units::FMeters{}, 20.};
		constexpr auto LengthB = TQuantity{Units::FCentimeters{}, 50.};
		constexpr auto SumAB = LengthA + LengthB;
		constexpr auto SumBA = LengthB + LengthA;

		static_assert(NearlyEqual(SumAB.GetValue<Units::FMeters>(), 20.5));
		static_assert(NearlyEqual(SumAB.GetValue<Units::FCentimeters>(), 2'050.));

		static_assert(NearlyEqual(SumBA.GetValue<Units::FMeters>(), 20.5));
		static_assert(NearlyEqual(SumBA.GetValue<Units::FCentimeters>(), 2'050.));

		auto LengthACopy = LengthA;
		LengthACopy += LengthB;
		TestNearlyEqual("TValue +=", LengthACopy.GetValue<Units::FMeters>(), 20.5);

		// + should not compile for different dimensions
		// constexpr auto Velocity = TValue{Units::FKilometersPerHour{}, 20.};
		// LengthA + Velocity;
	}
	else if (TestCommand == TEXT("Subtraction"))
	{
		constexpr auto LengthA = TQuantity{Units::FMeters{}, 20.};
		constexpr auto LengthB = TQuantity{Units::FCentimeters{}, 50.};
		constexpr auto SumAB = LengthA - LengthB;
		constexpr auto SumBA = LengthB - LengthA;

		static_assert(NearlyEqual(SumAB.GetValue<Units::FMeters>(), 19.5));
		static_assert(NearlyEqual(SumAB.GetValue<Units::FCentimeters>(), 1'950.));

		static_assert(NearlyEqual(SumBA.GetValue<Units::FMeters>(), -19.5));
		static_assert(NearlyEqual(SumBA.GetValue<Units::FCentimeters>(), -1'950.));

		auto LengthACopy = LengthA;
		LengthACopy -= LengthB;
		TestNearlyEqual("TValue +=", LengthACopy.GetValue<Units::FMeters>(), 19.5);

		// + should not compile for different dimensions
		// constexpr auto Velocity = TValue{Units::FKilometersPerHour{}, 20.};
		// LengthA - Velocity;
	}
	else if (TestCommand == TEXT("MultiplicationByScalar"))
	{
		constexpr auto Length = TQuantity{Units::FMeters{}, 42};

		static_assert((Length * 2).GetValue<Units::FMeters>() == 84);
		static_assert((2 * Length).GetValue<Units::FMeters>() == 84);

		auto LengthCopy = Length;
		LengthCopy *= 2;
		TestEqual("TValue *=", LengthCopy.GetValue<Units::FMeters>(), 84);
	}
	else if (TestCommand == TEXT("DivisionByScalar"))
	{
		constexpr auto Length = TQuantity{Units::FMeters{}, 42};

		static_assert((Length / 2).GetValue<Units::FMeters>() == 21);

		auto LengthCopy = Length;
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
