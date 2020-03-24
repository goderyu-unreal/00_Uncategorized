// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AbnormalWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNCATEGORIZED_API UAbnormalWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Meta = (BindWidget))
	class UButton* Btn_Test;
	
	UPROPERTY(Meta = (BindWidget))
	class UVerticalBox* VBox_Test;

	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* Text_Test;

	UPROPERTY(Meta = (BindWidget))
	class UCanvasPanel* Panel_Test;

	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnBtn_Test();
	
};
