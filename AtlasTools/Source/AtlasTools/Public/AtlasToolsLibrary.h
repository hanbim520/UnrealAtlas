// Fill out your copyright notice in the Description page of Project Settings.
//张海军 710605420@qq.com

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AtlasToolsLibrary.generated.h"

USTRUCT()
struct FAtlasResolutionStruct
{
    GENERATED_BODY()
    int32 Width;
    int32 Height;
};


UENUM()
enum AtlasResolutionSize
{
    E128X128 = 0,
    E128X256,
    E256X128,
    E256X256,
    E256X512,
    E512X256,
    E512X512,
    E512X1024,
    E1024X512,
    E1024X1024,
    E1024X2048,
    E2048X1024,
    E2048X2048,
    
    EMAX
};

UCLASS()
class ATLASTOOLS_API UAtlasToolsLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "AtlasToolsLibrary", meta = (WorldContext = "WorldContextObject"))
	static void ShowAtlasImage();

	UFUNCTION(BlueprintCallable, Category = "AtlasToolsLibrary", meta = (WorldContext = "WorldContextObject"))
	static void SetImageToAtlasAllInFolder(bool bRecreateAtlas = true,bool bClearSprite = true);

	UFUNCTION(BlueprintCallable, Category = "AtlasToolsLibrary", meta = (WorldContext = "WorldContextObject"))
	static int32 GenareteAtlasAllInFolder(FString Path,bool bRecreateAtlas = true,bool bClearSprite = true);
	UFUNCTION(BlueprintCallable, Category = "AtlasToolsLibrary", meta = (WorldContext = "WorldContextObject"))
	static int32 GenareteAtlasAllInFolders(TArray<FString> FolderPaths,bool bRecreateAtlas = true,bool bClearSprite = true);

	static int32 CreateSpritesFromTextures(TArray<UTexture2D*>& Textures,class UPaperSpriteAtlas* PaperSpriteAtlas = nullptr);

	static bool AutoAdjustSpriteAtlasTextureSize(class UPaperSpriteAtlas* PaperSpriteAtlas);

    UFUNCTION(BlueprintCallable, Category = "AtlasToolsLibrary", meta = (WorldContext = "WorldContextObject"))
    static TArray<FString> CaculateAtlashUseage(TArray<FString> PaperSpriteAtlasPaths,float MinPercent,bool bUseArea = true);

    //UFUNCTION(BlueprintCallable, Category = "AtlasToolsLibrary", meta = (WorldContext = "WorldContextObject"))
    static float CaculateAtlashUseageInternal(class UTexture2D* Texture2D);
    static float CaculateAtlashUseageUseAreaInternal(class UPaperSpriteAtlas* PaperSpriteAtlas );
private:
	static bool ExportImage(FString FolderName,UTexture2D* Texture2D, const FString& FileName);
	static void SetSpriteAtlasParams(class UPaperSpriteAtlas* PaperSpriteAtlas);
    static void InitResolutionData();

    static TArray<class UPaperSpriteAtlas*> DealMultipleTexturesToSingle(class UPaperSpriteAtlas* PaperSpriteAtlas);

public:
    static TArray<FAtlasResolutionStruct> Resolutions;
    static TMap<TWeakObjectPtr< class UPaperSpriteAtlas>, FIntPoint> SpriteMaxSizes;
};
 