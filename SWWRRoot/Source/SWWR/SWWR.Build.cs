// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class SWWR : ModuleRules
{
    public SWWR(TargetInfo Target)
    {
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG", "RHI", "RenderCore" });

        PrivateDependencyModuleNames.AddRange(new string[] { });

        // Uncomment if you are using Slate UI
        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
        //MinFilesUsingPrecompiledHeaderOverride = 1;
        //bFasterWithoutUnity = true;
        if (UEBuildConfiguration.bBuildEditor)
        {
            //for detail customization
            PublicDependencyModuleNames.Add("PropertyEditor");
            PrivateDependencyModuleNames.AddRange(new string[] { "UnrealEd", "EditorStyle" });
        }
    }
}
