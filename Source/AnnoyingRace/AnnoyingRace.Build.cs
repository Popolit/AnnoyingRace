// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class AnnoyingRace : ModuleRules
{
	public AnnoyingRace(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "LevelSequence", "MovieScene", "UMG", "Slate", "SlateCore" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

        PublicIncludePaths.AddRange(new string[] { "AnnoyingRace" });
        PrivateIncludePaths.AddRange(new string[] { "AnnoyingRace" });
	}
}
