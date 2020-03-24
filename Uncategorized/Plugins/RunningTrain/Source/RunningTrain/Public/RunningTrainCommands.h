// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "RunningTrainStyle.h"

class FRunningTrainCommands : public TCommands<FRunningTrainCommands>
{
public:

	FRunningTrainCommands()
		: TCommands<FRunningTrainCommands>(TEXT("RunningTrain"), NSLOCTEXT("Contexts", "RunningTrain", "RunningTrain Plugin"), NAME_None, FRunningTrainStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > PluginAction;
};
