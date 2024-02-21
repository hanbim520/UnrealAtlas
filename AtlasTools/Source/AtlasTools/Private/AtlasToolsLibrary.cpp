// Fill out your copyright notice in the Description page of Project Settings.


#include "AtlasToolsLibrary.h"
#include "../Plugins/2D/Paper2D/Source/Paper2D/Classes/PaperSpriteAtlas.h"
#include "ImageUtils.h"
#include "Blutility/Public/EditorUtilitySubsystem.h"
#include "Blutility/Classes/EditorUtilityWidgetBlueprint.h"
#include "Editor/EditorEngine.h"
#include "Editor.h"
#include "Misc/FileHelper.h"

#include <DesktopPlatform/Public/DesktopPlatformModule.h>
#include <DesktopPlatform/Public/IDesktopPlatform.h>

// #include "Windows/AllowWindowsPlatformTypes.h"
// #include "Windows/WindowsPlatformProcess.h"
// #include "Windows/HideWindowsPlatformTypes.h"
//#include <ShellAPI.h>
#include "Kismet/KismetSystemLibrary.h"
#include "../Plugins/Editor/EditorScriptingUtilities/Source/EditorScriptingUtilities/Public/EditorAssetLibrary.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "AssetRegistry/ARFilter.h"
#include "UObject/NoExportTypes.h"
#include "ContentBrowserModule.h"
#include "AssetToolsModule.h"
#include "Modules/ModuleManager.h"
#include "Misc/PackageName.h"
#include "../Plugins/2D/Paper2D/Source/Paper2D/Classes/PaperSprite.h"
#include "../Plugins/2D/Paper2D/Source/Paper2DEditor/Classes/PaperSpriteFactory.h"
#include "IContentBrowserSingleton.h"
#include "CustomPaperSpriteFactory.h"
#include "UObject/UnrealType.h"
#include "UObject/UnrealTypePrivate.h"
#include "Widgets/Notifications/SNotificationList.h"
#include "Framework/Notifications/NotificationManager.h"
#include "ShowAtlasWidget.h"
#include <LevelEditor.h>
#include <IBlutilityModule.h>
#include <Framework/Docking/TabManager.h>
#include "ObjectTools.h"
#include "PackageTools.h"
//#include "Editor/Blutility/Classes/EditorUtilityWidgetBlueprint.h"

#define SpriteAtlas_MaxWidth 2048
#define SpriteAtlas_MaxHeight 2048
TArray<FAtlasResolutionStruct> UAtlasToolsLibrary::Resolutions;
TMap<TWeakObjectPtr< class UPaperSpriteAtlas>, FIntPoint> UAtlasToolsLibrary::SpriteMaxSizes;

void UAtlasToolsLibrary::InitResolutionData()
{

    if (UAtlasToolsLibrary::Resolutions.Num() >= AtlasResolutionSize::EMAX)
    {
        return;
    }
    
    UAtlasToolsLibrary::Resolutions.Empty();

    FAtlasResolutionStruct AtlasResolution128x128;
    AtlasResolution128x128.Width = 128;
    AtlasResolution128x128.Height = 128;
    UAtlasToolsLibrary::Resolutions.Add(AtlasResolution128x128);

    FAtlasResolutionStruct AtlasResolution128x256;
    AtlasResolution128x256.Width = 128;
    AtlasResolution128x256.Height = 256;
    UAtlasToolsLibrary::Resolutions.Add(AtlasResolution128x256);

    FAtlasResolutionStruct AtlasResolution256x128;
    AtlasResolution256x128.Width = 256;
    AtlasResolution256x128.Height = 128;
    UAtlasToolsLibrary::Resolutions.Add(AtlasResolution256x128);

    FAtlasResolutionStruct AtlasResolution256x256;
    AtlasResolution256x256.Width = 256;
    AtlasResolution256x256.Height = 256;
    UAtlasToolsLibrary::Resolutions.Add(AtlasResolution256x256);

    FAtlasResolutionStruct AtlasResolution256x512;
    AtlasResolution256x512.Width = 256;
    AtlasResolution256x512.Height = 512;
    UAtlasToolsLibrary::Resolutions.Add(AtlasResolution256x512);

    FAtlasResolutionStruct AtlasResolution512x256;
    AtlasResolution512x256.Width = 512;
    AtlasResolution512x256.Height = 256;
    UAtlasToolsLibrary::Resolutions.Add(AtlasResolution512x256);

    FAtlasResolutionStruct AtlasResolution512x512;
    AtlasResolution512x512.Width = 512;
    AtlasResolution512x512.Height = 512;
    UAtlasToolsLibrary::Resolutions.Add(AtlasResolution512x512);

    FAtlasResolutionStruct AtlasResolution512x1024;
    AtlasResolution512x1024.Width = 512;
    AtlasResolution512x1024.Height = 1024;
    UAtlasToolsLibrary::Resolutions.Add(AtlasResolution512x1024);

    FAtlasResolutionStruct AtlasResolution1024x512;
    AtlasResolution1024x512.Width = 1024;
    AtlasResolution1024x512.Height = 512;
    UAtlasToolsLibrary::Resolutions.Add(AtlasResolution1024x512);

    FAtlasResolutionStruct AtlasResolution1024x1024;
    AtlasResolution1024x1024.Width = 1024;
    AtlasResolution1024x1024.Height = 1024;
    UAtlasToolsLibrary::Resolutions.Add(AtlasResolution1024x1024);

    FAtlasResolutionStruct AtlasResolution1024x2048;
    AtlasResolution1024x2048.Width = 1024;
    AtlasResolution1024x2048.Height = 2048;
    UAtlasToolsLibrary::Resolutions.Add(AtlasResolution1024x2048);

    FAtlasResolutionStruct AtlasResolution2048x1024;
    AtlasResolution2048x1024.Width = 2048;
    AtlasResolution2048x1024.Height = 1024;
    UAtlasToolsLibrary::Resolutions.Add(AtlasResolution2048x1024);

    FAtlasResolutionStruct AtlasResolution2048x2048;
    AtlasResolution2048x2048.Width = 2048;
    AtlasResolution2048x2048.Height = 2048;
    UAtlasToolsLibrary::Resolutions.Add(AtlasResolution2048x2048);

}


bool UAtlasToolsLibrary::ExportImage(FString FolderName, UTexture2D* Texture2D, const FString& FileName)
{
	// setup required parameters
	TextureCompressionSettings OldCompressionSettings = Texture2D->CompressionSettings;
	TextureMipGenSettings OldMipGenSettings = Texture2D->MipGenSettings;
	bool OldSRGB = Texture2D->SRGB;

	Texture2D->CompressionSettings = TextureCompressionSettings::TC_VectorDisplacementmap;
	Texture2D->MipGenSettings = TextureMipGenSettings::TMGS_NoMipmaps;
	Texture2D->SRGB = false;
	Texture2D->UpdateResource();

	FTexture2DMipMap& mipmap = Texture2D->PlatformData->Mips[0];
	uint8* Data = (uint8*)mipmap.BulkData.Lock(LOCK_READ_WRITE);
	//FColor* FormatedImageData = static_cast<FColor*>(mipmap.BulkData.Lock(LOCK_READ_WRITE));
	if (Data == nullptr)
	{
		mipmap.BulkData.Unlock(); // 不释放，会卡住
		Texture2D->UpdateResource();
		return false;
	}

	int width = Texture2D->PlatformData->SizeX;
	int height = Texture2D->PlatformData->SizeY;
	TArray<FColor> nColors;

	for (int32 y = 0; y < height; y++)
	{
		for (int32 x = 0; x < width; x++)
		{
			FColor bColor;
			bColor.B = Data[(y * width + x) * 4 + 0];//B 0 - 255
			bColor.G = Data[(y * width + x) * 4 + 1];//G
			bColor.R = Data[(y * width + x) * 4 + 2];//R
			bColor.A = Data[(y * width + x) * 4 + 3];//A 
			nColors.Add(bColor);
		}
	}
	mipmap.BulkData.Unlock();

	// return old parameters
	Texture2D->CompressionSettings = OldCompressionSettings;
	Texture2D->MipGenSettings = OldMipGenSettings;
	Texture2D->SRGB = OldSRGB;

	Texture2D->UpdateResource();


	FString AssetFolder = FPaths::ProjectContentDir();
	
	FString Path = AssetFolder + "../PreviewAtlasTexture/" + FolderName +"/";
	
//	IPlatformFile& FileManager = FPlatformFileManager::Get().GetPlatformFile();
	
// 	if (FileManager.DirectoryExists(*Path))
// 	{
// 		FileManager.DeleteDirectoryRecursively(*Path);
// 		
// 	}

//	FileManager.CreateDirectory(*Path);
	
	FString FilePath = Path + FileName;
	TArray<uint8> ImgData;
	FImageUtils::CompressImageArray(width, height, nColors, ImgData);
	bool ret = FFileHelper::SaveArrayToFile(ImgData, *FilePath);
	FString AbsolutePath = UKismetSystemLibrary::ConvertToAbsolutePath(FilePath);
    FPlatformProcess::ExploreFolder(*AbsolutePath);

// 	std::string str_path = TCHAR_TO_UTF8(*AppPath);
// 	std::wstring wstr_path;
// 	wstr_path.assign(str_path.begin(), str_path.end());
// 	ShellExecute(NULL, L"open", wstr_path.c_str(), NULL, NULL, SW_SHOWDEFAULT);
// 	std::string str_path =std::string("shimgvw.dll,ImageView_Fullscreen ")+ std::string(TCHAR_TO_UTF8(*AbsolutePath));
// 	std::wstring wstr_path;
// 	wstr_path.assign(str_path.begin(), str_path.end());
// 	::ShellExecute(NULL, L"open", _T("rundll32.exe"), wstr_path.c_str(),  NULL, SW_SHOWNORMAL);

	return ret;
}

void UAtlasToolsLibrary::SetSpriteAtlasParams(class UPaperSpriteAtlas* PaperSpriteAtlas)
{
	if (!PaperSpriteAtlas)
	{
		return;
	}

	//default value
	PaperSpriteAtlas->MaxWidth = SpriteAtlas_MaxWidth;
	PaperSpriteAtlas->MaxHeight = SpriteAtlas_MaxHeight;

	PaperSpriteAtlas->bRebuildAtlas = true;
	PaperSpriteAtlas->CompressionSettings = TextureCompressionSettings::TC_Default;
	PaperSpriteAtlas->PaddingType = EPaperSpriteAtlasPadding::DilateBorder;
	PaperSpriteAtlas->Padding = 1;
	PaperSpriteAtlas->Filter = TextureFilter::TF_Bilinear;
	PaperSpriteAtlas->MipCount = 1;
}

void UAtlasToolsLibrary::ShowAtlasImage()
{

	TArray<FString> SelectPaths; //选中文件路径
	FString fileType = TEXT("UrealFile (*.uasset)|*.uasset"); //过滤文件类型
	FString defaultPath = FPaths::ConvertRelativePathToFull(FPaths::ProjectContentDir()); //文件选择窗口默认开启路径
	IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
	bool bSuccess = DesktopPlatform->OpenFileDialog(nullptr, TEXT("UmgAnimationImportDialog"), defaultPath, TEXT(""), *fileType, EFileDialogFlags::None, SelectPaths);
	if (bSuccess)
	{
		if (SelectPaths.Num() <=0)
		{
			return;
		}
		FString SelectFile = SelectPaths[0];
		FString SelectFilePath = SelectFile.Left(SelectFile.Len() - FString(TEXT(".uasset")).Len());

		TArray<FString> StringArray;
		SelectFilePath.ParseIntoArray(StringArray, TEXT("/"));
		FString SelectFileName = StringArray[StringArray.Num() - 1];
		FString RelativePath = SelectFilePath.Right(SelectFilePath.Len() - FString("../../../UE_game/Content/").Len());
		RelativePath = "/Game/" + RelativePath;
		FString AssetPath = RelativePath + "." + SelectFileName;

		UObject* AssetObject =  UEditorAssetLibrary::LoadAsset(AssetPath);
		if (!AssetObject->IsA(UPaperSpriteAtlas::StaticClass()))
		{
			auto Message = NSLOCTEXT("AtlasTools", "Select Asset Error", "You need to select PaperSpriteAtlas asset!");
			FNotificationInfo Info(Message);
			Info.bFireAndForget = true;
			Info.ExpireDuration = 5.0f;
			Info.bUseSuccessFailIcons = false;
			Info.bUseLargeFont = false;
			FSlateNotificationManager::Get().AddNotification(Info);
			return;
		}
		
		UPaperSpriteAtlas* PaperSpriteAtlas = (UPaperSpriteAtlas*)AssetObject;
		if (PaperSpriteAtlas == nullptr)
		{
			auto Message = NSLOCTEXT("AtlasTools", "Select Asset Error", "You need to select PaperSpriteAtlas asset!");
			FNotificationInfo Info(Message);
			Info.bFireAndForget = true;
			Info.ExpireDuration = 5.0f;
			Info.bUseSuccessFailIcons = false;
			Info.bUseLargeFont = false;

			FSlateNotificationManager::Get().AddNotification(Info);

			return;
		}

       

	//	UPaperSpriteAtlas* PaperSpriteAtlas = LoadObject<UPaperSpriteAtlas>(nullptr, TEXT("/Script/Paper2D.PaperSpriteAtlas'/Game/Art/UI/Sprites/NewPaperSpriteAtlas.NewPaperSpriteAtlas'"));
		int32 MaxWidth = PaperSpriteAtlas->BuiltWidth;
		int32 MaxHeight = PaperSpriteAtlas->BuiltHeight;


		FString AssetFolder = FPaths::ProjectContentDir();

		FString Path = AssetFolder + "../PreviewAtlasTexture/" + PaperSpriteAtlas->GetName() + "/";

		IPlatformFile& FileManager = FPlatformFileManager::Get().GetPlatformFile();

		if (FileManager.DirectoryExists(*Path))
		{
			FileManager.DeleteDirectoryRecursively(*Path);

		}

		FileManager.CreateDirectory(*Path);

		TArray<UTexture*> Textures = PaperSpriteAtlas->GeneratedTextures;
		for (UTexture* Texture : Textures)
		{
			UTexture2D* Texture2D = Cast<UTexture2D>(Texture);
			
            //float Percent = CaculateAtlashUseageInternal(Texture2D);
            float Percent = CaculateAtlashUseageUseAreaInternal(PaperSpriteAtlas);
            UE_LOG(LogTemp, Log, TEXT("SpriteAtlas Useage Percent : %f"), Percent);


			FString fileName = PaperSpriteAtlas->GetName()+"_"+ Texture->GetName() + ".png";
			
			ExportImage(PaperSpriteAtlas->GetName(),Texture2D, fileName);


// 			UEditorUtilityWidgetBlueprint* EditorWidget = LoadObject<UEditorUtilityWidgetBlueprint>(nullptr, TEXT("/Script/Blutility.EditorUtilityWidgetBlueprint'/AtlasTools/ShowAtlas.ShowAtlas'"));
// 			UShowAtlasWidget* ShowCombineTextureIns = nullptr;
// 			UEditorUtilitySubsystem* EditorUtilitySubsystem = GEditor->GetEditorSubsystem<UEditorUtilitySubsystem>();
// 			
// 			ShowCombineTextureIns = Cast<UShowAtlasWidget>(EditorUtilitySubsystem->SpawnAndRegisterTab(EditorWidget));
//             ShowCombineTextureIns->Rename(*fileName);
// 			FName NewTabID = FName(fileName);
// 			FLevelEditorModule& LevelEditorModule = FModuleManager::GetModuleChecked<FLevelEditorModule>(TEXT("LevelEditor"));
// 			TSharedPtr<FTabManager> LevelEditorTabManager = LevelEditorModule.GetLevelEditorTabManager();
// 			if (!LevelEditorTabManager->HasTabSpawner(NewTabID))
// 			{
// 				FText DisplayName = FText::FromString(EditorWidget->GetName());
// 				TSharedPtr<SDockTab> NewDockTab = LevelEditorTabManager->TryInvokeTab(NewTabID);
// 				IBlutilityModule* BlutilityModule = FModuleManager::GetModulePtr<IBlutilityModule>("Blutility");
// 				LevelEditorTabManager->RegisterTabSpawner(NewTabID, FOnSpawnTab::CreateUObject(EditorWidget, &UEditorUtilityWidgetBlueprint::SpawnEditorUITab))
// 					.SetDisplayName(DisplayName)
// 					.SetGroup(BlutilityModule->GetMenuGroup().ToSharedRef());
// 				EditorWidget->SetRegistrationName(FName(fileName));
// 				BlutilityModule->AddLoadedScriptUI(EditorWidget);
// 
// 				ShowCombineTextureIns = Cast<UShowAtlasWidget>(EditorWidget->GetCreatedWidget());
// 			}
// 			
			
			
// 			ShowCombineTextureIns->AtlasImage->SetBrushFromTexture(Texture2D, true);
// 			//ShowCombineTextureIns->AtlasImage->SetDesiredSizeOverride(FVector2D(MaxWidth, MaxHeight));
// 			ShowCombineTextureIns->SetDesiredSizeInViewport(FVector2D(MaxWidth, MaxHeight));
		}

	}

	


	

//     FString AssetFolder = FPaths::ProjectContentDir();
//     FString ObjectName = FString("Preview_") + PaperSpriteAtlas->GetName() + "_Alpha";
//     //FString NewTranslateAndRotationAssetPath = AssetFolder + "/" + *(TranslateAndRotationObjectName + "." + TranslateAndRotationObjectName);
//     //UEditorAssetLibrary::DuplicateAsset(AssetData.ObjectPath.ToString(), NewAssetPath);
//     FString PackageName = AssetFolder + ObjectName;
//     UPackage* Package = CreatePackage(*PackageName);
//     Package->FullyLoad();
//     Package->SetDirtyFlag(true);
    // 创建实例
//     FImageUtils::CreateTexture2D
//     UTexture2D* PreviewTexture = NewObject<UTexture2D>(Package, *ObjectName, RF_Public | RF_Standalone);
// 
//     PreviewTexture->setwi
}

int32 UAtlasToolsLibrary::GenareteAtlasAllInFolders(TArray<FString> FolderPaths, bool bRecreateAtlas /* = true */, bool bClearSprite /* = true */)
{
	if (FolderPaths.Num() == 0)
	{
		return -1;
	}

	for (auto Folder : FolderPaths)
	{
		int32 res = GenareteAtlasAllInFolder(Folder,bRecreateAtlas,bClearSprite);
		if (res != 0)
		{
			return -1;
		}
	}

	return 0;
}

int32 UAtlasToolsLibrary::GenareteAtlasAllInFolder(FString Path, bool bRecreateAtlas /* = true */, bool bClearSprite /* = true */)
{
	if (Path == "")
	{
		return -1;
	}
	FString AssetFolder = FPaths::ProjectContentDir();
	FString SelectPath = Path;
	FString defaultPath = FPaths::ConvertRelativePathToFull(FPaths::ProjectContentDir());

	FString RelativePath = SelectPath.Right(SelectPath.Len() - defaultPath.Len());
	RelativePath = "/Game/" + RelativePath;
	AssetFolder = RelativePath;
	TArray<FString> SplitedStr;
	RelativePath.ParseIntoArray(SplitedStr, TEXT("/"));
	FString FolderName = SplitedStr[SplitedStr.Num() - 1];



	FAssetRegistryModule& assetRegistryModule = FModuleManager::GetModuleChecked<FAssetRegistryModule>("AssetRegistry");
	//----------------------------------------------------------------------
		//删除存在的atlas文件
	TArray<FAssetData> ArrayAssetData;

	FARFilter filterDeletaAtlas;
	filterDeletaAtlas.PackagePaths.Add(FName(RelativePath));
#if ENGINE_MAJOR_VERSION >= 5 && ENGINE_MINOR_VERSION >= 1
	filterDeletaAtlas.ClassPaths.Add(FTopLevelAssetPath(UPaperSpriteAtlas::StaticClass()));
#else
	filterDeletaAtlas.ClassNames.Add(UPaperSpriteAtlas::StaticClass()->GetFName());
#endif
	filterDeletaAtlas.bRecursiveClasses = true;
	assetRegistryModule.Get().GetAssets(filterDeletaAtlas, ArrayAssetData);

	UPaperSpriteAtlas* PaperSpriteAtlasNew = nullptr;
	if (bRecreateAtlas)
	{

		UE_LOG(LogTemp, Log, TEXT("-----------------------------RecreateAtlas-----------------------------"));
		for (FAssetData& AssetData : ArrayAssetData)
		{
			UEditorAssetLibrary::DeleteAsset(AssetData.ObjectPath.ToString());
		}


		FString ObjectName = FolderName + "_Atlas";
		FString PackageName = AssetFolder + "/" + ObjectName;

		UPackage* Package = CreatePackage(*PackageName);
		Package->FullyLoad();
		Package->SetDirtyFlag(true);


		PaperSpriteAtlasNew = NewObject<UPaperSpriteAtlas>(Package, *ObjectName, RF_Public | RF_Standalone);

		SetSpriteAtlasParams(PaperSpriteAtlasNew);

		FAssetRegistryModule::AssetCreated(PaperSpriteAtlasNew);
		UEditorAssetLibrary::SaveAsset(FAssetData(PaperSpriteAtlasNew).ObjectPath.ToString());

	}
	else
	{
		if (ArrayAssetData.Num() != 0)
		{
			PaperSpriteAtlasNew = LoadObject<UPaperSpriteAtlas>(nullptr, *ArrayAssetData[0].ToSoftObjectPath().ToString());
		    if (ArrayAssetData.Num() > 1)
		    {
		        UE_LOG(LogTemp, Error, TEXT("More than one atlas file!"));
		        auto Message = NSLOCTEXT("AtlasTools", "Package Atlas", "More than one atlas file!");
		        FNotificationInfo Info(Message);
		        Info.bFireAndForget = true;
		        Info.ExpireDuration = 5.0f;
		        Info.bUseSuccessFailIcons = false;
		        Info.bUseLargeFont = false;
		        FSlateNotificationManager::Get().AddNotification(Info);
		    }
		}
		else
		{
			FString ObjectName = FolderName + "_Atlas";
			FString PackageName = AssetFolder + "/" + ObjectName;

			UPackage* Package = CreatePackage(*PackageName);
			Package->FullyLoad();
			Package->SetDirtyFlag(true);


			PaperSpriteAtlasNew = NewObject<UPaperSpriteAtlas>(Package, *ObjectName, RF_Public | RF_Standalone);

			SetSpriteAtlasParams(PaperSpriteAtlasNew);

			FAssetRegistryModule::AssetCreated(PaperSpriteAtlasNew);
			UEditorAssetLibrary::SaveAsset(FAssetData(PaperSpriteAtlasNew).ObjectPath.ToString());
		}
	}



	//----------------------------------------------------------------------

	//----------------------------------------------------------------------
	//删除存在的PaperSprite文件
	if (bClearSprite)
	{
		UE_LOG(LogTemp,Log,TEXT("-----------------------------ClearSprite-----------------------------"));

		ArrayAssetData.Empty();
		FARFilter filterDeletaSprite;
		filterDeletaSprite.PackagePaths.Add(FName(RelativePath));

#if ENGINE_MAJOR_VERSION >= 5 && ENGINE_MINOR_VERSION >= 1
		filterDeletaSprite.ClassPaths.Add(FTopLevelAssetPath(UPaperSprite::StaticClass()));
#else
		filterDeletaSprite.ClassNames.Add(UPaperSprite::StaticClass()->GetFName());
#endif
		filterDeletaSprite.bRecursiveClasses = true;
		FString PackageAtlasPath;
		FPackageName::TryConvertLongPackageNameToFilename(AssetFolder, PackageAtlasPath);

		TArray<FString> Folders;
		IFileManager::Get().IterateDirectory(*PackageAtlasPath, [&Folders](const TCHAR* FilenameOrDirectory, bool bIsDirectory) -> bool
			{
				bool bIsValidItem = true;

				if (bIsDirectory)
				{
					Folders.Add(FilenameOrDirectory);
				}

				return bIsValidItem;
			}
		);

		for (FString SubFolder : Folders)
		{
			FString SubRelativePath = SubFolder.Right(SubFolder.Len() - defaultPath.Len());
			SubRelativePath = "/Game/" + SubRelativePath;
			filterDeletaSprite.PackagePaths.Add(FName(SubRelativePath));
		}

		assetRegistryModule.Get().GetAssets(filterDeletaSprite, ArrayAssetData);

		for (FAssetData& AssetData : ArrayAssetData)
		{
			UEditorAssetLibrary::DeleteAsset(AssetData.ObjectPath.ToString());
		}
	}

	//----------------------------------------------------------------------
	ArrayAssetData.Empty();

	FARFilter filterAtlas;

	FString PackageAtlasPath;
	FPackageName::TryConvertLongPackageNameToFilename(AssetFolder, PackageAtlasPath);
	// 		TArray<FString> FileNames;
	// 		IFileManager& MyFileManager = IFileManager::Get();
	// 		MyFileManager.FindFilesRecursive(FileNames, *PackageAtlasPath, TEXT("*.uasset"), true, false, true);


#if ENGINE_MAJOR_VERSION >= 5 && ENGINE_MINOR_VERSION >= 1
	filterAtlas.ClassPaths.Add(FTopLevelAssetPath(UTexture::StaticClass()));
#else
	filterAtlas.ClassNames.Add(UTexture::StaticClass()->GetFName());
#endif
	filterAtlas.bRecursiveClasses = true;
	filterAtlas.PackagePaths.Add(FName(RelativePath));

	assetRegistryModule.Get().GetAssets(filterAtlas, ArrayAssetData);

	TArray<FString> Folders;


	IFileManager::Get().IterateDirectory(*PackageAtlasPath, [&Folders](const TCHAR* FilenameOrDirectory, bool bIsDirectory) -> bool
		{
			bool bIsValidItem = true;

			if (bIsDirectory)
			{
				Folders.Add(FilenameOrDirectory);
			}

			return bIsValidItem;
		}
	);

	for (FString SubFolder : Folders)
	{
		FString SubRelativePath = SubFolder.Right(SubFolder.Len() - defaultPath.Len());
		SubRelativePath = "/Game/" + SubRelativePath;
		filterAtlas.PackagePaths.Add(FName(SubRelativePath));
	}

	if (ArrayAssetData.Num() > 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("The textures and directory siblings!!! Please check it! "));
	}

	ArrayAssetData.Empty();
	assetRegistryModule.Get().GetAssets(filterAtlas, ArrayAssetData);


	FString rootPath = FPaths::GetRelativePathToRoot();

	TArray<UTexture2D*> Textures;
	for (FAssetData& AssetData : ArrayAssetData)
	{
		UTexture2D* Texture2D = LoadObject<UTexture2D>(nullptr, *AssetData.ToSoftObjectPath().ToString());
		Textures.Add(Texture2D);
	}

	UE_LOG(LogTemp, Log, TEXT("-----------------------------CreateSpritesFromTextures-----------------------------"));

	return CreateSpritesFromTextures(Textures, PaperSpriteAtlasNew);
}

void UAtlasToolsLibrary::SetImageToAtlasAllInFolder(bool bRecreateAtlas /*= true*/, bool bClearSprite /*= true*/)
{
	FString AssetFolder = FPaths::ProjectContentDir();
	FString SelectPath; // 
	FString defaultPath = FPaths::ConvertRelativePathToFull(FPaths::ProjectContentDir()); 
	IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
	bool bSuccess = DesktopPlatform->OpenDirectoryDialog(nullptr, TEXT("AtlasDialog"), defaultPath, SelectPath);
	if (bSuccess)
	{
		GenareteAtlasAllInFolder(SelectPath, bRecreateAtlas, bClearSprite);
		return;

		FString RelativePath = SelectPath.Right(SelectPath.Len() - defaultPath.Len());
		RelativePath = "/Game/" + RelativePath;
		AssetFolder = RelativePath;
		TArray<FString> SplitedStr;
		RelativePath.ParseIntoArray(SplitedStr,TEXT("/"));
		FString FolderName = SplitedStr[SplitedStr.Num() - 1];

		

		FAssetRegistryModule& assetRegistryModule = FModuleManager::GetModuleChecked<FAssetRegistryModule>("AssetRegistry");
		//----------------------------------------------------------------------
		//删除存在的atlas文件
		TArray<FAssetData> ArrayAssetData;
		
		FARFilter filterDeletaAtlas;
		filterDeletaAtlas.PackagePaths.Add(FName(RelativePath));
#if ENGINE_MAJOR_VERSION >= 5 && ENGINE_MINOR_VERSION >= 1
		filterDeletaAtlas.ClassPaths.Add(FTopLevelAssetPath(UPaperSpriteAtlas::StaticClass()));
#else
		filterDeletaAtlas.ClassNames.Add(UPaperSpriteAtlas::StaticClass()->GetFName());
#endif
		filterDeletaAtlas.bRecursiveClasses = true;
		assetRegistryModule.Get().GetAssets(filterDeletaAtlas, ArrayAssetData);

		UPaperSpriteAtlas* PaperSpriteAtlasNew = nullptr;
		if (bRecreateAtlas)
		{
			for (FAssetData& AssetData : ArrayAssetData)
			{
				UEditorAssetLibrary::DeleteAsset(AssetData.ObjectPath.ToString());
			}


			FString ObjectName = FolderName + "_Atlas";
			FString PackageName = AssetFolder + "/" + ObjectName;
			UPackage* Package = CreatePackage(*PackageName);
			Package->FullyLoad();
			Package->SetDirtyFlag(true);


			PaperSpriteAtlasNew = NewObject<UPaperSpriteAtlas>(Package, *ObjectName, RF_Public | RF_Standalone);

			SetSpriteAtlasParams(PaperSpriteAtlasNew);

			FAssetRegistryModule::AssetCreated(PaperSpriteAtlasNew);
			UEditorAssetLibrary::SaveAsset(FAssetData(PaperSpriteAtlasNew).ObjectPath.ToString());

		}
		else
		{
			if (ArrayAssetData.Num() > 0)
			{
				PaperSpriteAtlasNew = LoadObject<UPaperSpriteAtlas>(nullptr, *ArrayAssetData[0].ToSoftObjectPath().ToString());
				UE_LOG(LogTemp, Error, TEXT("More than one atlas file!"));
				auto Message = NSLOCTEXT("AtlasTools", "Package Atlas", "More than one atlas file!");
				FNotificationInfo Info(Message);
				Info.bFireAndForget = true;
				Info.ExpireDuration = 5.0f;
				Info.bUseSuccessFailIcons = false;
				Info.bUseLargeFont = false;
				FSlateNotificationManager::Get().AddNotification(Info);
			}
			else
			{
				FString ObjectName = FolderName + "_Atlas";
				FString PackageName = AssetFolder + "/" + ObjectName;
				UPackage* Package = CreatePackage(*PackageName);
				Package->FullyLoad();
				Package->SetDirtyFlag(true);


				PaperSpriteAtlasNew = NewObject<UPaperSpriteAtlas>(Package, *ObjectName, RF_Public | RF_Standalone);

				SetSpriteAtlasParams(PaperSpriteAtlasNew);

				FAssetRegistryModule::AssetCreated(PaperSpriteAtlasNew);
				UEditorAssetLibrary::SaveAsset(FAssetData(PaperSpriteAtlasNew).ObjectPath.ToString());
			}
		}



		//----------------------------------------------------------------------

		//----------------------------------------------------------------------
		//删除存在的PaperSprite文件
		if (bClearSprite)
		{
			ArrayAssetData.Empty();
			FARFilter filterDeletaSprite;
			filterDeletaSprite.PackagePaths.Add(FName(RelativePath));
#if ENGINE_MAJOR_VERSION >= 5 && ENGINE_MINOR_VERSION >= 1
			filterDeletaSprite.ClassPaths.Add(FTopLevelAssetPath(UPaperSprite::StaticClass()));
#else
			filterDeletaSprite.ClassNames.Add(UPaperSprite::StaticClass()->GetFName());
#endif
			filterDeletaSprite.bRecursiveClasses = true;
			FString PackageAtlasPath;
			FPackageName::TryConvertLongPackageNameToFilename(AssetFolder, PackageAtlasPath);

			TArray<FString> Folders;

			filterDeletaSprite.PackagePaths.Add(FName(RelativePath));
			IFileManager::Get().IterateDirectory(*PackageAtlasPath, [&Folders](const TCHAR* FilenameOrDirectory, bool bIsDirectory) -> bool
				{
					bool bIsValidItem = true;

					if (bIsDirectory)
					{
						Folders.Add(FilenameOrDirectory);
					}

					return bIsValidItem;
				}
			);

			for (FString SubFolder : Folders)
			{
				FString SubRelativePath = SubFolder.Right(SubFolder.Len() - defaultPath.Len());
				SubRelativePath = "/Game/" + SubRelativePath;
				filterDeletaSprite.PackagePaths.Add(FName(SubRelativePath));
			}

			assetRegistryModule.Get().GetAssets(filterDeletaSprite, ArrayAssetData);

			for (FAssetData& AssetData : ArrayAssetData)
			{
				UEditorAssetLibrary::DeleteAsset(AssetData.ObjectPath.ToString());
			}
		}
		
		//----------------------------------------------------------------------
		ArrayAssetData.Empty();

		FARFilter filterAtlas;

		FString PackageAtlasPath;
		FPackageName::TryConvertLongPackageNameToFilename(AssetFolder, PackageAtlasPath);
// 		TArray<FString> FileNames;
// 		IFileManager& MyFileManager = IFileManager::Get();
// 		MyFileManager.FindFilesRecursive(FileNames, *PackageAtlasPath, TEXT("*.uasset"), true, false, true);


#if ENGINE_MAJOR_VERSION >= 5 && ENGINE_MINOR_VERSION >= 1
        filterAtlas.ClassPaths.Add(FTopLevelAssetPath(UTexture::StaticClass()));
#else
        filterAtlas.ClassNames.Add(UTexture::StaticClass()->GetFName());
#endif
        filterAtlas.bRecursiveClasses = true;
        filterAtlas.PackagePaths.Add(FName(RelativePath));

        assetRegistryModule.Get().GetAssets(filterAtlas, ArrayAssetData);

		TArray<FString> Folders;

		
		IFileManager::Get().IterateDirectory(*PackageAtlasPath, [&Folders](const TCHAR* FilenameOrDirectory, bool bIsDirectory) -> bool
			{
				bool bIsValidItem = true;

				if (bIsDirectory)
				{
					Folders.Add(FilenameOrDirectory);
				}

				return bIsValidItem;
			}
		);

		for (FString SubFolder:Folders)
		{
			FString SubRelativePath = SubFolder.Right(SubFolder.Len() - defaultPath.Len());
			SubRelativePath = "/Game/" + SubRelativePath;
			filterAtlas.PackagePaths.Add(FName(SubRelativePath));
		}
		 
         if (ArrayAssetData.Num() > 0)
         {
            UE_LOG(LogTemp,Warning,TEXT("The textures and directory siblings!!! Please check it! "));
         }

        ArrayAssetData.Empty();
		assetRegistryModule.Get().GetAssets(filterAtlas, ArrayAssetData);


		FString rootPath = FPaths::GetRelativePathToRoot();

		TArray<UTexture2D*> Textures;
		for (FAssetData& AssetData : ArrayAssetData)
		{
		  UTexture2D* Texture2D =  LoadObject<UTexture2D>(nullptr, *AssetData.ToSoftObjectPath().ToString());
		  Textures.Add(Texture2D);
		}

	

		CreateSpritesFromTextures(Textures, PaperSpriteAtlasNew);
	}
}

bool UAtlasToolsLibrary::AutoAdjustSpriteAtlasTextureSize(class UPaperSpriteAtlas* PaperSpriteAtlas)
{
	if (PaperSpriteAtlas == nullptr)
	{
		UE_LOG(LogTemp,Warning,TEXT("PaperSpriteAtlas is nullptr!"));
		return false;
	}

    InitResolutionData();

   
	FIntPoint SpriteMaxSize = SpriteMaxSizes[PaperSpriteAtlas];
    const FIntPoint PaddedSpriteSize(SpriteMaxSize.X + PaperSpriteAtlas->Padding * 2, SpriteMaxSize.Y + PaperSpriteAtlas->Padding * 2);
   
    if ((PaddedSpriteSize.X > SpriteAtlas_MaxWidth) || (PaddedSpriteSize.Y > SpriteAtlas_MaxHeight))
    {
        UE_LOG(LogTemp, Warning, TEXT("Max Size too big"));
        return false;
    }

    
    for (int32 i = 0; i < AtlasResolutionSize::EMAX; i++)
    {
        FAtlasResolutionStruct& AtlasResolution = UAtlasToolsLibrary::Resolutions[i];


        if ((PaddedSpriteSize.X > AtlasResolution.Width) || (PaddedSpriteSize.Y > AtlasResolution.Height))
        {
           // if (AtlasResolution.Width != 2048 && AtlasResolution.Height != 2048)
            {
                continue;
            }
        }
       

        PaperSpriteAtlas->MaxHeight = AtlasResolution.Height;
        PaperSpriteAtlas->MaxWidth = AtlasResolution.Width;

        PaperSpriteAtlas->PostEditChange();
        if (PaperSpriteAtlas->GeneratedTextures.Num() == 1)
        {
            break;
        }

    }



    return true;



}

int32 UAtlasToolsLibrary::CreateSpritesFromTextures(TArray<UTexture2D*>& Textures, class UPaperSpriteAtlas* PaperSpriteAtlas)
{
    //SpriteMaxSize = FIntPoint(0,0);
	SpriteMaxSizes.Empty();

    bool bNoError = true;
	FAssetToolsModule& AssetToolsModule = FModuleManager::Get().LoadModuleChecked<FAssetToolsModule>("AssetTools");
	FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");

	TArray<UObject*> ObjectsToSync;

	for (auto TextureIt = Textures.CreateConstIterator(); TextureIt; ++TextureIt)
	{
		UTexture2D* Texture = *TextureIt;

		
		// Create the sprite
		FString Name;
		FString PackageName;

	    const FString DefaultSuffix = TEXT("_Sprite");

	    // Get a unique name for the sprite
	    AssetToolsModule.Get().CreateUniqueAssetName(Texture->GetOutermost()->GetName(), DefaultSuffix, /*out*/ PackageName, /*out*/ Name);

	    FString PackagePath = FPackageName::GetLongPackagePath(PackageName);

//         UPackage* Pkg = CreatePackage(*FPaths::Combine(PackagePath, Name));
//         // Handle fully loading packages before creating new objects.
//         TArray<UPackage*> TopLevelPackages;
//         TopLevelPackages.Add(Pkg);
//         ObjectTools::CleanupAfterSuccessfulDelete(TopLevelPackages, /*bPerformReferenceCheck*/false);

//         FText OPText = FText::FromString("Delect a asset");
//         if (UPackageTools::HandleFullyLoadingPackages(TopLevelPackages, OPText))
//         {
//            // UObject* ExistingObject = LoadObject<UObject>(nullptr, *FPaths::Combine(PackagePath, Name));
//             UObject* ExistingObject = StaticFindObject(UObject::StaticClass(), Pkg, *Name);
//             // UObject* ExistingObject = StaticFindObject(UObject::StaticClass(), Pkg, *Name);
//             if (ExistingObject)
//             {
//                 bool bDeleteSucceeded = ObjectTools::DeleteSingleObject(ExistingObject);
// 
//                 if (bDeleteSucceeded)
//                 {
//                     // Force GC so we can cleanly create a new asset (and not do an 'in place' replacement)
//                     //CollectGarbage(GARBAGE_COLLECTION_KEEPFLAGS);
//                     GEditor->ForceGarbageCollection(true);
//                     // Old package will be GC'ed... create a new one here
//                     Pkg = CreatePackage(*PackageName);
//                     Pkg->MarkAsFullyLoaded();
//                 }
// 
//             }
//         }

        
        
//         UObject* UnUseAsset = LoadObject<UObject>(nullptr,*FPaths::Combine(PackagePath, Name));
//         if (UnUseAsset != nullptr)
//         {
//             FString NewDuplicatedAssetName = FAssetData(UnUseAsset).AssetName.ToString() + TEXT("_Del") ;
//             UEditorAssetLibrary::RenameAsset(FAssetData(UnUseAsset).ObjectPath.ToString(), NewDuplicatedAssetName);
//             UEditorAssetLibrary::DeleteAsset(FAssetData(UnUseAsset).ObjectPath.ToString());
// 
//             CollectGarbage(GARBAGE_COLLECTION_KEEPFLAGS);
//         }
      // Create the factory used to generate the sprite
        UCustomPaperSpriteFactory* SpriteFactory = NewObject<UCustomPaperSpriteFactory>();
        SpriteFactory->InitialTexture = Texture;

    
		if (UObject* NewAsset = AssetToolsModule.Get().CreateAsset(Name, PackagePath, UPaperSprite::StaticClass(), SpriteFactory))
		{
			ObjectsToSync.Add(NewAsset);

			UPaperSprite* PaperSprite = Cast<UPaperSprite>(NewAsset);
            //PaperSprite->AtlasGroup = PaperSpriteAtlas;
			
            for (TFieldIterator<FObjectPropertyBase> It(PaperSprite->GetClass()); It; ++It)
			{
				FObjectPropertyBase* Property = *It;
				if (Property->GetNameCPP() == FString("AtlasGroup"))
				{
					UPaperSpriteAtlas* AtlasGroup = Property->ContainerPtrToValuePtr<UPaperSpriteAtlas>(PaperSprite);
					Property->SetObjectPropertyValue(AtlasGroup, PaperSpriteAtlas);
					UE_LOG(LogTemp, Log, TEXT("Property:%s value:%s"), *Property->GetName(), *AtlasGroup->GetName());
					//AutoAdjustSpriteAtlasTextureSize(PaperSpriteAtlas);
					//PaperSpriteAtlas->PostEditChange();
					break;
				}
			}
			if (SpriteMaxSizes.Contains(PaperSpriteAtlas))
			{
				FIntPoint& SpriteMaxSize = SpriteMaxSizes[PaperSpriteAtlas];
				const FVector2D SpriteSizeFloat = PaperSprite->GetSourceSize();
				FIntPoint TempSize = FIntPoint(FMath::TruncToInt(SpriteSizeFloat.X), FMath::TruncToInt(SpriteSizeFloat.Y));
				if (TempSize.X > SpriteMaxSize.X)
				{
					SpriteMaxSize.X = TempSize.X;
				}
				if (TempSize.Y > SpriteMaxSize.Y)
				{
					SpriteMaxSize.Y = TempSize.Y;
				}
			}
			else
			{
				FIntPoint SpriteMaxSize = FIntPoint(0, 0);
				const FVector2D SpriteSizeFloat = PaperSprite->GetSourceSize();
				FIntPoint TempSize = FIntPoint(FMath::TruncToInt(SpriteSizeFloat.X), FMath::TruncToInt(SpriteSizeFloat.Y));
				if (TempSize.X > SpriteMaxSize.X)
				{
					SpriteMaxSize.X = TempSize.X;
				}
				if (TempSize.Y > SpriteMaxSize.Y)
				{
					SpriteMaxSize.Y = TempSize.Y;
				}

				SpriteMaxSizes.Add(PaperSpriteAtlas, SpriteMaxSize);
			}
//             const FVector2D SpriteSizeFloat = PaperSprite->GetSourceSize();
//             FIntPoint TempSize = FIntPoint(FMath::TruncToInt(SpriteSizeFloat.X), FMath::TruncToInt(SpriteSizeFloat.Y));
//             if (TempSize.X > SpriteMaxSize.X)
//             {
//                 SpriteMaxSize.X = TempSize.X;
//             }
//             if (TempSize.Y > SpriteMaxSize.Y)
//             {
//                 SpriteMaxSize.Y = TempSize.Y;
//             }
            
			//UEditorAssetLibrary::SaveAsset(FAssetData(NewAsset).ObjectPath.ToString());
		}
	}
    AutoAdjustSpriteAtlasTextureSize(PaperSpriteAtlas);
	SpriteMaxSizes.Empty();
	//FCustomPaperSpriteFactory::HandleAssetChangedEvent(PaperSpriteAtlas);

    TArray<class UPaperSpriteAtlas*>  SpriteAtlases = DealMultipleTexturesToSingle(PaperSpriteAtlas);

    for (int32 sptIdx = 0 ;sptIdx < SpriteAtlases.Num(); ++ sptIdx)
    {

        PaperSpriteAtlas = SpriteAtlases[sptIdx];

        UEditorAssetLibrary::SaveAsset(FAssetData(PaperSpriteAtlas).ObjectPath.ToString());
        TArray<FPaperSpriteAtlasSlot> Slots = PaperSpriteAtlas->AtlasSlots;
        for (FPaperSpriteAtlasSlot& Slot : Slots)
        {
            UEditorAssetLibrary::SaveAsset(FAssetData(Slot.SpriteRef.Get()).ObjectPath.ToString());
        }

    }


    if (FApp::CanEverRender())
    {
	    if (ObjectsToSync.Num() > 0)
	    {
		    ContentBrowserModule.Get().SyncBrowserToAssets(ObjectsToSync);
	    }

	    auto Message = NSLOCTEXT("AtlasTools", "Package Atlas", "Package Atlas Succ");
	    FNotificationInfo Info(Message);
	    Info.bFireAndForget = true;
	    Info.ExpireDuration = 5.0f;
	    Info.bUseSuccessFailIcons = false;
	    Info.bUseLargeFont = false;
	    FSlateNotificationManager::Get().AddNotification(Info);
    }

    if (!bNoError)
    {
        return -1;
    }
	return 0;
}

TArray<class UPaperSpriteAtlas*> UAtlasToolsLibrary::DealMultipleTexturesToSingle(class UPaperSpriteAtlas* PaperSpriteAtlas)
{
    TArray <UPaperSpriteAtlas*> Atlases;
    if (PaperSpriteAtlas->GeneratedTextures.Num() > 1)
    {
        
        for (int32 i = 0; i < PaperSpriteAtlas->GeneratedTextures.Num();++i)
        {
            FAssetData PaperSpriteAtlasAsset = FAssetData(PaperSpriteAtlas);
            FString SourceAssetPath = PaperSpriteAtlasAsset.ObjectPath.ToString();//获取资产路径【路径名字+资产名字】
            FString NewDuplicatedAssetName = PaperSpriteAtlasAsset.AssetName.ToString() + TEXT("_") + FString::FromInt(i + 1);//新复制的资产名字
            FString NewPathName = FPaths::Combine(PaperSpriteAtlasAsset.PackagePath.ToString(), NewDuplicatedAssetName);//将包路径【没有资产名字的路径】和新的名字结合

            UPackage* Package = CreatePackage(*FPaths::Combine(PaperSpriteAtlasAsset.PackagePath.ToString(), NewDuplicatedAssetName));
            UPaperSpriteAtlas* PaperSpriteAtlasNew = NewObject<UPaperSpriteAtlas>(Package, *NewDuplicatedAssetName, RF_Public | RF_Standalone);

            SetSpriteAtlasParams(PaperSpriteAtlasNew);
            PaperSpriteAtlasNew->MaxWidth = PaperSpriteAtlas->MaxWidth;
            PaperSpriteAtlasNew->MaxHeight = PaperSpriteAtlas->MaxHeight;

            FAssetRegistryModule::AssetCreated(PaperSpriteAtlasNew);
            UEditorAssetLibrary::SaveAsset(FAssetData(PaperSpriteAtlasNew).ObjectPath.ToString());


           // UObject* PaperSpriteAtlasNew = UEditorAssetLibrary::DuplicateAsset(SourceAssetPath, NewPathName);
           // Atlases.Add(Cast<UPaperSpriteAtlas>(PaperSpriteAtlasNew));
            Atlases.Add(PaperSpriteAtlasNew);
        }
       
        for (int32 SlotIdx = 0; SlotIdx < PaperSpriteAtlas->AtlasSlots.Num(); ++SlotIdx)
        {
            FPaperSpriteAtlasSlot Slot = PaperSpriteAtlas->AtlasSlots[SlotIdx];
            if (Slot.SpriteRef.IsValid())
            {
                UPaperSprite* PaperSprite = Cast<UPaperSprite>(Slot.SpriteRef.Get());

                for (TFieldIterator<FObjectPropertyBase> It(PaperSprite->GetClass()); It; ++It)
                {
                    FObjectPropertyBase* Property = *It;
                    if (Property->GetNameCPP() == FString("AtlasGroup"))
                    {

                        FAssetData PaperSpriteAtlasAsset = FAssetData(PaperSpriteAtlas);
                        FString SourceAssetPath = PaperSpriteAtlasAsset.ObjectPath.ToString();//获取资产路径【路径名字+资产名字】
                        FString NewDuplicatedAssetName = PaperSpriteAtlasAsset.AssetName.ToString() + TEXT("_") + FString::FromInt(Slot.AtlasIndex + 1);//新复制的资产名字
                        FString NewPathName = FPaths::Combine(PaperSpriteAtlasAsset.PackagePath.ToString(), NewDuplicatedAssetName);//将包路径【没有资产名字的路径】和新的名字结合
                        UPaperSpriteAtlas* ToSpriteAtlas = LoadObject<UPaperSpriteAtlas>(nullptr, *NewPathName);

                        UPaperSpriteAtlas* AtlasGroup = Property->ContainerPtrToValuePtr<UPaperSpriteAtlas>(PaperSprite);
                        Property->SetObjectPropertyValue(AtlasGroup, ToSpriteAtlas);
                        //UE_LOG(LogTemp, Log, TEXT("Property:%s value:%s"), *Property->GetName(), *ToSpriteAtlas->GetName());

						if (SpriteMaxSizes.Contains(Atlases[Slot.AtlasIndex]))
						{
							FIntPoint& SpriteMaxSize = SpriteMaxSizes[Atlases[Slot.AtlasIndex]];
							const FVector2D SpriteSizeFloat = PaperSprite->GetSourceSize();
							FIntPoint TempSize = FIntPoint(FMath::TruncToInt(SpriteSizeFloat.X), FMath::TruncToInt(SpriteSizeFloat.Y));
							if (TempSize.X > SpriteMaxSize.X)
							{
								SpriteMaxSize.X = TempSize.X;
							}
							if (TempSize.Y > SpriteMaxSize.Y)
							{
								SpriteMaxSize.Y = TempSize.Y;
							}
						}
						else
						{
							FIntPoint SpriteMaxSize = FIntPoint(0, 0);
							const FVector2D SpriteSizeFloat = PaperSprite->GetSourceSize();
							FIntPoint TempSize = FIntPoint(FMath::TruncToInt(SpriteSizeFloat.X), FMath::TruncToInt(SpriteSizeFloat.Y));
							if (TempSize.X > SpriteMaxSize.X)
							{
								SpriteMaxSize.X = TempSize.X;
							}
							if (TempSize.Y > SpriteMaxSize.Y)
							{
								SpriteMaxSize.Y = TempSize.Y;
							}
							SpriteMaxSizes.Add(Atlases[Slot.AtlasIndex], SpriteMaxSize);
						}
                        break;
                    }
                }
            }

        }
       // PaperSpriteAtlas->PostEditChange();
       // UEditorAssetLibrary::SaveAsset(FAssetData(PaperSpriteAtlas).ObjectPath.ToString());
       

        for (int32 i = 0; i < Atlases.Num(); ++i)
        {
            Atlases[i]->PostEditChange();

            UEditorAssetLibrary::SaveAsset(FAssetData(Atlases[i]).ObjectPath.ToString());
        }

        AutoAdjustSpriteAtlasTextureSize(Atlases[Atlases.Num() - 1]);
        UEditorAssetLibrary::DeleteAsset(FAssetData(PaperSpriteAtlas).ObjectPath.ToString());
		SpriteMaxSizes.Empty();

    }
    else
    {
        Atlases.Add(PaperSpriteAtlas);
    }

    return Atlases;
}

TArray<FString> UAtlasToolsLibrary::CaculateAtlashUseage(TArray<FString> PaperSpriteAtlasPaths,float MinPercent,bool bUseArea /* = true */)
{
    TArray<FString> Results;
    for (FString PaperSpriteAtlasPath : PaperSpriteAtlasPaths)
    {
        UPaperSpriteAtlas* PaperSpriteAtlas = LoadObject<UPaperSpriteAtlas>(nullptr, *PaperSpriteAtlasPath);
        if (PaperSpriteAtlas == nullptr)
        {
            Results.Add(PaperSpriteAtlasPath);
            UE_LOG(LogTemp,Warning,TEXT("File is not exist, %s"),*PaperSpriteAtlasPath);
        }
        else
        {
            TArray<UTexture*> Textures = PaperSpriteAtlas->GeneratedTextures;
            for (UTexture* Texture : Textures)
            {
                
                float Percent = 0;
                UTexture2D* Texture2D = Cast<UTexture2D>(Texture);
                if (bUseArea)
                {
                    Percent = CaculateAtlashUseageUseAreaInternal(PaperSpriteAtlas);
                }
                else
                {
                    //1个图集有多张图的，每个都单算
                    Percent = CaculateAtlashUseageInternal(Texture2D);
                }
                
                if (Percent < MinPercent)
                {
                    FString MyFString = FString::Printf(TEXT("Percent = %.2f   Size = (%d, %d)"), Percent, PaperSpriteAtlas->MaxWidth, PaperSpriteAtlas->MaxHeight );
                    for (int i = MyFString.Len(); i < 40; i++)
                    {
                        MyFString.Append(TEXT(" "));
                    }
                    MyFString = FString::Printf(TEXT("%s   %s "), *MyFString, *PaperSpriteAtlasPath);
                    MyFString.ReplaceInline(TEXT("/Game/Art/UI/TexturesAtlas/"), TEXT(""));
                    Results.Add(MyFString);
                }
            }
            
        }
    }

    return Results;
}

//1个图集有2张图的，每个都单算
float UAtlasToolsLibrary::CaculateAtlashUseageInternal(class UTexture2D* Texture2D)
{
    float Percent = 0.0000f;

    Texture2D->CompressionSettings = TextureCompressionSettings::TC_VectorDisplacementmap;
    Texture2D->MipGenSettings = TextureMipGenSettings::TMGS_NoMipmaps;
    Texture2D->SRGB = false;
    Texture2D->UpdateResource();


    FTexture2DMipMap& mipmap = Texture2D->PlatformData->Mips[0];
    uint8* Data = (uint8*)mipmap.BulkData.Lock(LOCK_READ_WRITE);
    if (Data == nullptr)
    {
        mipmap.BulkData.Unlock();
        return 0 ;
    }

//     FTexture2DResource* Resource = (FTexture2DResource*)Texture2D->Resource;
//     uint32 Stride = 0; // Assigned by RHILockTexture2D.
//     uint8* MipData = (uint8*)RHILockTexture2D(
//         Resource->GetTexture2DRHI(),
//         0,
//         RLM_ReadOnly,
//         Stride,
//         false);

    int width = Texture2D->PlatformData->SizeX;
    int height = Texture2D->PlatformData->SizeY;

    int32 AlphaIs0Count = 0;
    for (int32 y = 0; y < height; y++)
    {
        for (int32 x = 0; x < width; x++)
        {
                uint8 Alpha = Data[(y * width + x) * 4 + 3];//A 
                if (Alpha == 0)
                {
                    AlphaIs0Count++;
                }
        }
    }
    mipmap.BulkData.Unlock();

    Percent = (float)AlphaIs0Count / (float)(height * width);

    return Percent;
}


float UAtlasToolsLibrary::CaculateAtlashUseageUseAreaInternal(class UPaperSpriteAtlas* PaperSpriteAtlas )
{
    float Percent = 0.0000f;
   

    int width = PaperSpriteAtlas->MaxWidth;
    int height = PaperSpriteAtlas->MaxHeight;

    int Count = PaperSpriteAtlas->GeneratedTextures.Num();
   
    TArray<FPaperSpriteAtlasSlot>& Sprites =  PaperSpriteAtlas->AtlasSlots;

   float SpriteAreas = 0;
   for (int32 i = 0;i < Sprites.Num();++i)
   {
       FPaperSpriteAtlasSlot& slot = Sprites[i];
       if (slot.IsValid() && !slot.SpriteRef.GetAssetName().IsEmpty())
       {
           SpriteAreas += slot.Width * slot.Height;
       }
       
   }
    
    Percent = (float)SpriteAreas / (float)(Count* height * width);

    return Percent;
}