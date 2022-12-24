// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TheLastAugust_UE5_1 : ModuleRules
{
	public TheLastAugust_UE5_1(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput" });
	}
}
