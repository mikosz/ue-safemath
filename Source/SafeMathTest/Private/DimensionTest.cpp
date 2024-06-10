// #include "Misc/AutomationTest.h"
// #include "SafeMath/Dimension.h"
//
// #include <type_traits>
//
// IMPLEMENT_COMPLEX_AUTOMATION_TEST(
// 	DimensionTest, "SafeMath.DimensionTest", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
//
// void DimensionTest::GetTests(TArray<FString>& OutBeautifiedNames, TArray<FString>& OutTestCommands) const
// {
// 	OutBeautifiedNames.Emplace(TEXT("Inverse"));
//
// 	OutTestCommands.Emplace(TEXT("InverseTC"));
// }
//
// bool DimensionTest::RunTest(const FString& Parameters)
// {
// 	using namespace SafeMath;
//
// 	constexpr auto Mass = MakeDimension<FMass, 1>();
// 	constexpr auto MassInv = Mass.Inverse();
// 	constexpr auto Expected = TDimension<Private::TBaseDimensionWithExponent<FMass, -1>>{};
// 	static_assert(std::is_same_v<decltype(MassInv), decltype(Expected)>);
//
// 	return true;
// }
