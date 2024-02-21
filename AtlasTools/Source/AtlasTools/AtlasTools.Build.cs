// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class AtlasTools : ModuleRules
{
	public AtlasTools(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
        
        PrivateDefinitions.Add("XD_TORCHLIGHT_TEXTUREATLAS_IMPORT_PATH=1"); // Replace texture import path from 'Textures' into 'TexturesAtlas' directory

        PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"UnrealEd",
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"ImageWrapper",
				"Blutility",
				"UMG",
				"UMGEditor",
				"EditorScriptingUtilities",
				"Paper2D",
				"Paper2DEditor",
				"Core",
				"CoreUObject",
				"ApplicationCore",
				"Slate",
				"SlateCore",
				"Engine",
				"InputCore",
				"UnrealEd", // for AssetEditorSubsystem
				"KismetWidgets",
				"Kismet",  // for FWorkflowCentricApplication
				"PropertyEditor",
				"RenderCore",
				"Paper2D",
				"ContentBrowser",
				"WorkspaceMenuStructure",
				"MeshPaint",
				"EditorWidgets",
				"Projects",
				"ToolMenus",
				"Settings",
				"AssetTools",
				"LevelEditor",
				"DesktopPlatform",
				"AssetRegistry",
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"UMG",
				"UMGEditor",
				"UnrealEd",
				"EditorStyle",
				"EditorScriptingUtilities",
				"Core",
				"CoreUObject",
				"ApplicationCore",
				"InputCore",
				"Engine",
				"AssetTools",
				"Json",
				"JsonUtilities",
				"Core",
				"CoreUObject",
				"ApplicationCore",
				"InputCore",
				"Engine",
				"AssetTools",
				"UnrealEd", // for FAssetEditorManager
				"KismetWidgets",
				"KismetCompiler",
				"BlueprintGraph",
				"GraphEditor",
				"Kismet",  // for FWorkflowCentricApplication
				"PropertyPath",
				"PropertyEditor",
				"UMG",
				"EditorStyle",
				"Slate",
				"SlateCore",
				"SlateRHIRenderer",
				"MessageLog",
				"MovieScene",
				"MovieSceneTools",
				"MovieSceneTracks",
				"DetailCustomizations",
				"Settings",
				"RenderCore",
				"TargetPlatform",
				"TimeManagement",
				"GameProjectGeneration",
				"PropertyPath",
				"ToolMenus",
				"SlateReflector",
				"DeveloperSettings",
				"SequencerWidgets",
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
