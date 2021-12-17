// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ActionRoguelike : ModuleRules
{
	public ActionRoguelike(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore",
			"AIModule",
			"GameplayTasks",
			"GameplayTags",
		});
		
		PublicIncludePaths.AddRange(new string[]
		{
			"ActionRoguelike/Public/Character",
			"ActionRoguelike/Public/Projectile",
			"ActionRoguelike/Public/Interactable",
			"ActionRoguelike/Public/Interactable/Pickups",
			"ActionRoguelike/Public/Interfaces",
			"ActionRoguelike/Public/Enemies",
			"ActionRoguelike/Public/Components",
			"ActionRoguelike/Public/AI",
			"ActionRoguelike/Public/AI/Services",
			"ActionRoguelike/Public/AI/Tasks",
			"ActionRoguelike/Public/Actions",
		});

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
