// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CombatSystemDemo : ModuleRules
{
	public CombatSystemDemo(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
            "GameplayAbilities",
            "GameplayTags",
            "GameplayTasks",
            "AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"CombatSystemDemo",
			"CombatSystemDemo/Variant_Platforming",
			"CombatSystemDemo/Variant_Platforming/Animation",
			"CombatSystemDemo/Variant_Combat",
			"CombatSystemDemo/Variant_Combat/AI",
			"CombatSystemDemo/Variant_Combat/Animation",
			"CombatSystemDemo/Variant_Combat/Gameplay",
			"CombatSystemDemo/Variant_Combat/Interfaces",
			"CombatSystemDemo/Variant_Combat/UI",
			"CombatSystemDemo/Variant_SideScrolling",
			"CombatSystemDemo/Variant_SideScrolling/AI",
			"CombatSystemDemo/Variant_SideScrolling/Gameplay",
			"CombatSystemDemo/Variant_SideScrolling/Interfaces",
			"CombatSystemDemo/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
