// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "RunningTrainCommands.h"

#define LOCTEXT_NAMESPACE "FRunningTrainModule"

void FRunningTrainCommands::RegisterCommands()
{
	UI_COMMAND(PluginAction, "跑车工具", "配置跑车工具SPS匹配当前项目", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
