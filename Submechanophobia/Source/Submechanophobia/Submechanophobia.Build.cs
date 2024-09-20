// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Submechanophobia : ModuleRules
{
	public Submechanophobia(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
