using UnrealBuildTool;

public class LoadingScreenModule : ModuleRules
{
    public LoadingScreenModule(ReadOnlyTargetRules Target) : base(Target)
    {
        PrivatePCHHeaderFile = "Public/LoadingScreenModule.h";
        
        PCHUsage = PCHUsageMode.UseSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "Engine",
                "CoreUObject",
                "CommonUI",
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "MoviePlayer",
                "CommonUI",
            }
        );
    }
}