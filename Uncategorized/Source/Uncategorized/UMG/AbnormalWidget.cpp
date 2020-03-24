// Fill out your copyright notice in the Description page of Project Settings.


#include "AbnormalWidget.h"
#include "Components/ContentWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/PanelSlot.h"

void UAbnormalWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // UButton* Button = NewObject<UButton>(this, UButton::StaticClass(), FName(TEXT("Button_1")));
    // UTextBlock* Text = NewObject<UTextBlock>(this, UTextBlock::StaticClass(), FName(TEXT("Text_1")));

    // FText Name = FText::FromString(FString(TEXT("Button_1")));
    // Text->SetText(Name);
    // Text->Font.Size = 42;
    // Button->AddChild(Text);
    // VBox_Test->AddChild(Button);
    // auto ButtonSlot = Cast<UCanvasPanelSlot>(Button->Slot);
    // if (ButtonSlot)
    // {
    //     ButtonSlot->SetAlignment(FVector2D(0.5f, 0.5f));
    //     // Cast<UCanvasPanelSlot>(Button->Slot)->SetPosition(FVector2D(0, 0));
    //     // ButtonSlot->SetSize(FVector2D(200, 100));
    //     ButtonSlot->SetAutoSize(true);
    // }
    if (Btn_Test != nullptr)
    {
        //binding event to onclicked
        Btn_Test->OnClicked.AddDynamic(this, &UAbnormalWidget::OnBtn_Test);
    }
    UE_LOG(LogTemp, Warning, TEXT("AbnormalWidget.cpp Line 17 Log NativeConstruct"));
}

void UAbnormalWidget::OnBtn_Test()
{
    UE_LOG(LogTemp, Warning, TEXT("AbnormalWidget.cpp Line 22 Log Btn_Test clicked"));
    static int32 Count = 1;
    FName ButtonName = FName(*(FString::Printf(TEXT("Button_%d"), Count)));
    FName TextName = FName(*(FString::Printf(TEXT("Text_%d"), Count)));
    FText TextContent = FText::FromString(FString::Printf(TEXT("Text_%d"), Count));
    UButton* Button = NewObject<UButton>(this, UButton::StaticClass(), ButtonName);
    UTextBlock* Text = NewObject<UTextBlock>(this, UTextBlock::StaticClass(), TextName);
    Text->SetText(TextContent);
    Text->Font.Size = 42;
    Button->AddChild(Text);
    auto TextSlot = Cast<UCanvasPanelSlot>(Text->Slot); 
    if (TextSlot)
    {
        TextSlot->SetAutoSize(true);
    }
    else
    {
        TextSlot = Cast<UCanvasPanelSlot>(NewObject<UPanelSlot>(this, UPanelSlot::StaticClass()));
        TextSlot->SetAutoSize(true);
    }
    VBox_Test->AddChild(Button);
    ++Count;
}

