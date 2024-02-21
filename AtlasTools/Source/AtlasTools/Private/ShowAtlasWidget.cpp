// Fill out your copyright notice in the Description page of Project Settings.


#include "ShowAtlasWidget.h"

void UShowAtlasWidget::NativeConstruct()
{
	Super::NativeConstruct();
	AtlasImage = Cast<UImage>(GetWidgetFromName(TEXT("AtlasImage")));

}