// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CMasMas_Proyecto : ModuleRules
{
	public CMasMas_Proyecto(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput" });
	}
}
