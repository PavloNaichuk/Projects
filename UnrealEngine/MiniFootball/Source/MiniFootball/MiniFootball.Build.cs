// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MiniFootball : ModuleRules
{
	public MiniFootball(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}
