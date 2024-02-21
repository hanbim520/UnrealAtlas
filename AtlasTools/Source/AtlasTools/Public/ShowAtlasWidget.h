// Fill out your copyright notice in the Description page of Project Settings.
//ÕÅº£¾ü 710605420@qq.com

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "Components/Image.h"
#include "ShowAtlasWidget.generated.h"

/**
 * 
 */
UCLASS()
class ATLASTOOLS_API UShowAtlasWidget : public UEditorUtilityWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

public:
	UImage* AtlasImage;
};
