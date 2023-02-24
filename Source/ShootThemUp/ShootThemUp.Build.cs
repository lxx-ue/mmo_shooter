// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class ShootThemUp : ModuleRules
{
	public ShootThemUp(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] 
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"Niagara",
			"PhysicsCore",
			"GameplayTasks",
			"NavigationSystem",
			"SlateCore"
		});

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		//тут прописываются директории к файлам с++
		//лю юичку
		PublicIncludePaths.AddRange(new string[] 
		{
			"ShootThemUp/Public/Player", 
			"ShootThemUp/Public/Components", 
			"ShootThemUp/Public/Dev",
			"ShootThemUp/Public/Weapon",
			"ShootThemUp/Public/UI",
			"ShootThemUp/Public/Animations",
			"ShootThemUp/Public/Pickups",
			"ShootThemUp/Public/Weapon/Components",
			"ShootThemUp/Public/AI",
			"ShootThemUp/Public/AI/Tasks",
			"ShootThemUp/Public/AI/Services",
			"ShootThemUp/Public/AI/EQS",
			"ShootThemUp/Public/AI/Decorators",
			"ShootThemUp/Public/Menu",
			"ShootThemUp/Public/Menu/UI",
			"ShootThemUp/Public/Sound",
			"ShootThemUp/Public/GameModes"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
