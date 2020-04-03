// Fill out your copyright notice in the Description page of Project Settings.


#include "AbnormalBase.h"
#include "AbnormalPlugin.h"
#include "AbnormalPopMenu.h"
#include "Engine/World.h"
#include "Components/Button.h"

void AAbnormalBase::RegisterMenu_Implementation()
{
	auto PopMenuAssetPath = ConfigFileManager.GetValue(TEXT("PopMenuAssetPath"), TEXT("ProjectConfig"));
	UClass* AbnormalPopMenuBPClass = LoadClass<UAbnormalPopMenu>(this, *PopMenuAssetPath);
	if (AbnormalPopMenuBPClass)
	{
		AbnormalPopMenu = NewObject<UAbnormalPopMenu>(this, AbnormalPopMenuBPClass);
		if (AbnormalPopMenu)
		{
			AbnormalPopMenu->SetVisibility(ESlateVisibility::Collapsed);
			AbnormalPopMenu->AddToViewport();
			auto DefaultRegisterMenu = ConfigFileManager.GetValue(TEXT("DefaultRegisterMenu"), TEXT("ProjectConfig"));
			TArray<FString> MenuItems;
			DefaultRegisterMenu.ParseIntoArray(OUT MenuItems, TEXT(";"));
			for (auto MenuItem : MenuItems)
			{
				TArray<FString> MenuItemDescAndFuncName;
				MenuItem.ParseIntoArray(OUT MenuItemDescAndFuncName, TEXT(","));
				if (MenuItemDescAndFuncName.Num() == 2)
				{
					if (auto Button = AbnormalPopMenu->RegisterMenuItem(MenuItemDescAndFuncName[0]))
					{
						FScriptDelegate Delegate;
						// 根据函数名进行绑定，传入的第一个参数为this是非正常基类，但是假如子类自己实现一个函数，依旧会正确绑定
						// 即便是非正常基类没有声明的函数，只要子类中定义了该函数，就能正确执行
						// 这个功能可谓相当的强大！！
						Delegate.BindUFunction(this, *MenuItemDescAndFuncName[1]);
						Button->OnClicked.AddUnique(Delegate);
					}
				}
				else
				{
					UE_LOG(LogAbnormalPlugin, Warning, TEXT("解析配置文件DefaultRegisterMenu数据时出错，导致没有按照既定格式分割数据，请及时检查，其内容为%s"), *DefaultRegisterMenu);
				}
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
	ConfigFileManager = UConfigFileManager();
	Super::BeginPlay();
}

// Called every frame
void AAbnormalBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

