// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AbnormalPopMenu.generated.h"

/**
 * 
 */
UCLASS()
class ABNORMALPLUGIN_API UAbnormalPopMenu : public UUserWidget
{
	GENERATED_BODY()

public:	

	UPROPERTY(Meta = (BindWidget))
	class UVerticalBox* VBox_AbnormalPopMenu;

	UPROPERTY(Meta = (BindWidget))
	class UCanvasPanel* Panel_AbnormalPopMenu;

	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Abnormal | Menu")
	class UButton* RegisterMenuItem(const FString& EventDescription);
	UButton* RegisterMenuItem_Implementation(const FString& EventDescription);
};
