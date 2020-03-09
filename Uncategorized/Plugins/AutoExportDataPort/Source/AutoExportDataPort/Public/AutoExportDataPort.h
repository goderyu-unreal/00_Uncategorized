// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

#ifndef OUT
#define OUT
#endif

DECLARE_LOG_CATEGORY_EXTERN(LogAutoExportDataPort, Log, All)

class FAutoExportDataPortModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
