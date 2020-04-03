// Fill out your copyright notice in the Description page of Project Settings.

#include "AbnormalPopMenu.h"
#include "Components/ContentWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/PanelSlot.h"

void UAbnormalPopMenu::NativeConstruct()
{
}

UButton *UAbnormalPopMenu::RegisterMenuItem_Implementation(const FString &EventDescription)
{
    // FName ButtonName = FName(*(FString::Printf(TEXT("Button_%d"), Count)));
    auto ButtonName = FName(*(EventDescription + "_Btn"));
    auto TextName = FName(*(EventDescription + "_Text"));
    auto TextContent = FText::FromString(EventDescription);
    if (auto Button = NewObject<UButton>(this, UButton::StaticClass(), ButtonName))
    {
        if (auto Text = NewObject<UTextBlock>(this, UTextBlock::StaticClass(), TextName))
        {
            Text->SetText(TextContent);
            Text->Font.Size = 42;
            Button->AddChild(Text);
        }

        VBox_AbnormalPopMenu->AddChild(Button);
        return Button;
    }
    return nullptr;
}