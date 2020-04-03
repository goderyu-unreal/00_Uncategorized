// Fill out your copyright notice in the Description page of Project Settings.


#include "AbnormalBase.h"
#include "AbnormalPlugin.h"
#include "AbnormalPopMenu.h"
#include "Engine/World.h"
#include "Components/Button.h"

void AAbnormalBase::RegisterMenu_Implementation()
{
	UClass* AbnormalPopMenuBPClass = LoadClass<UAbnormalPopMenu>(this, TEXT("Blueprint'/AbnormalPlugin/BPActors/UI_AbnormalPopMenu_Test.UI_AbnormalPopMenu_Test_C'"));    //TSubclassOf<AActor>同理
	if (AbnormalPopMenuBPClass)
	{
		AbnormalPopMenu = NewObject<UAbnormalPopMenu>(this, AbnormalPopMenuBPClass);
		if (AbnormalPopMenu)
		{
			AbnormalPopMenu->SetVisibility(ESlateVisibility::Collapsed);
			AbnormalPopMenu->AddToViewport();
			if (auto Button = AbnormalPopMenu->RegisterMenuItem(FString(TEXT("处理"))))
			{
				FScriptDelegate Delegate;
				Delegate.BindUFunction(this, "Process");
				Button->OnClicked.AddUnique(Delegate);
			}
		}
		else
		{
			UE_LOG(LogAbnormalPlugin, Warning, TEXT("创建弹出式菜单失败"));
		}
	}
}

void AAbnormalBase::Process_Implementation()
{
	UE_LOG(LogAbnormalPlugin, Log, TEXT("执行处理流程"));
	AbnormalPopMenu->RemoveFromParent();
	AbnormalPopMenu = nullptr;
	if (this->Destroy())
	{
		UE_LOG(LogAbnormalPlugin, Log, TEXT("已销毁该非正常Actor"));
	}
}

void AAbnormalBase::SetAbnormalId(const FString& TaskId)
{
	AbnormalId = TaskId;
}

const FString& AAbnormalBase::GetAbnormalId()
{
	return AbnormalId;
}

// Sets default values
AAbnormalBase::AAbnormalBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAbnormalBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAbnormalBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

