using UnrealBuildTool;

public class SafeMathEditor : ModuleRules
{
	public SafeMathEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"InputCore",
				"SafeMath",
				"Slate",
				"SlateCore",
				"UnrealEd",
			}
		);
	}
}