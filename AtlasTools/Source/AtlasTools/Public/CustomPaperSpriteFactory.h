// Copyright Epic Games, Inc. All Rights Reserved.
//ÕÅº£¾ü 710605420@qq.com

#pragma once

/**
 * Factory for sprites
 */

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Factories/Factory.h"
#include "CustomPaperSpriteFactory.generated.h"

UCLASS()
class UCustomPaperSpriteFactory : public UFactory
{
	GENERATED_UCLASS_BODY()

	// Initial texture to create the sprite from (Can be nullptr)
	class UTexture2D* InitialTexture;

	// Set bUseSourceRegion to get it to use/set initial sourceUV and dimensions
	bool bUseSourceRegion;
	FIntPoint InitialSourceUV;
	FIntPoint InitialSourceDimension;

	// UFactory interface
	virtual bool ConfigureProperties() override;
	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
	// End of UFactory interface
};
