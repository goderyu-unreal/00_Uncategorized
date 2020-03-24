// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "RunningTrain.h"
#include "RunningTrainStyle.h"
#include "RunningTrainCommands.h"
#include "Misc/MessageDialog.h"
#include "Misc/Paths.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"

#include "LevelEditor.h"

#include <stdlib.h>

static const FName RunningTrainTabName("跑车工具");

#define LOCTEXT_NAMESPACE "FRunningTrainModule"

void FRunningTrainModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FRunningTrainStyle::Initialize();
	FRunningTrainStyle::ReloadTextures();

	FRunningTrainCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FRunningTrainCommands::Get().PluginAction,
		FExecuteAction::CreateRaw(this, &FRunningTrainModule::PluginButtonClicked),
		FCanExecuteAction());
		
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	
	{
		TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender());
		MenuExtender->AddMenuExtension("WindowLayout", EExtensionHook::After, PluginCommands, FMenuExtensionDelegate::CreateRaw(this, &FRunningTrainModule::AddMenuExtension));

		LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);
	}
	
	{
		TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
		ToolbarExtender->AddToolBarExtension("Game", EExtensionHook::First, PluginCommands, FToolBarExtensionDelegate::CreateRaw(this, &FRunningTrainModule::AddToolbarExtension));
		
		LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);
	}
}

void FRunningTrainModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FRunningTrainStyle::Shutdown();

	FRunningTrainCommands::Unregister();
}

void FRunningTrainModule::PluginButtonClicked()
{
	auto RunningTrainExecutableFilePath = FPaths::ConvertRelativePathToFull(FPaths::ProjectContentDir() + FString(TEXT("Simulator/SPS/sps.exe")));
	system(TCHAR_TO_UTF8(*RunningTrainExecutableFilePath));
	// system("calc");
	// system("D:/WorkSpaceUE/GoderyuUnrealDemo/00_Uncategorized/Uncategorized/Content/Simulator/SPS/sps.exe");
	// Put your "OnButtonClicked" stuff here
	// FText DialogText = FText::Format(
	// 						LOCTEXT("PluginButtonDialogText", "Add code to {0} in {1} to override this button's actions"),
	// 						FText::FromString(TEXT("FRunningTrainModule::PluginButtonClicked()")),
	// 						FText::FromString(TEXT("RunningTrain.cpp"))
	// 				   );
	// FMessageDialog::Open(EAppMsgType::Ok, DialogText);
}

void FRunningTrainModule::AddMenuExtension(FMenuBuilder& Builder)
{
	Builder.AddMenuEntry(FRunningTrainCommands::Get().PluginAction);
}

void FRunningTrainModule::AddToolbarExtension(FToolBarBuilder& Builder)
{
	Builder.AddToolBarButton(FRunningTrainCommands::Get().PluginAction);
	Builder.AddComboButton(FUIAction(), FOnGetContent::CreateRaw(this, &FRunningTrainModule::GetComboContent), FText::FromString(TEXT("")));
}

TSharedRef<SWidget> FRunningTrainModule::GetComboContent()
{
	FMenuBuilder MenuBuilder(true, PluginCommands);

	MenuBuilder.AddMenuEntry(FRunningTrainCommands::Get().PluginAction);
	MenuBuilder.AddMenuEntry(FRunningTrainCommands::Get().PluginAction);
	MenuBuilder.AddMenuEntry(FRunningTrainCommands::Get().PluginAction);

	return MenuBuilder.MakeWidget();

}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FRunningTrainModule, RunningTrain)