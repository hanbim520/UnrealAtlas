// Copyright Epic Games, Inc. All Rights Reserved.
//张海军 710605420@qq.com
#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FAtlasToolsModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
