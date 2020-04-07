// Fill out your copyright notice in the Description page of Project Settings.


#include "AbnormalBase.h"
#include "AbnormalPlugin.h"
#include "AbnormalPopMenu.h"
#include "Engine/World.h"
#include "InputCore/Classes/InputCoreTypes.h"
#include "Components/Button.h"

void AAbnormalBase::PreSet_Implementation()
{
}

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
			// 读取配置文件中的注册菜单模式，根据不同策略来设置MenuItems
			auto RegisterMenuMode = ConfigFileManager.GetValue(TEXT("RegisterMenuMode"), TEXT("ProjectConfig"));
			if (!RegisterMenuMode.Equals(TEXT("OnlyUnreal"), ESearchCase::IgnoreCase))
			{
				if (RegisterMenuMode.Equals(TEXT("OnlyConfig"), ESearchCase::IgnoreCase))
				{
					MenuItems.Empty();
				}
				auto DefaultRegisterMenu = ConfigFileManager.GetValue(TEXT("DefaultRegisterMenu"), TEXT("ProjectConfig"));
				TArray<FString> MenuItemsArr;
				DefaultRegisterMenu.ParseIntoArray(OUT MenuItemsArr, TEXT(";"));
				for (auto MenuItem : MenuItemsArr)
				{
					TArray<FString> MenuItemDescAndFuncName;
					MenuItem.ParseIntoArray(OUT MenuItemDescAndFuncName, TEXT(","));
					if (MenuItemDescAndFuncName.Num() == 2)
					{
						if (RegisterMenuMode.Equals(TEXT("AppendNoReplace"), ESearchCase::IgnoreCase))
						{
							if (MenuItems.Contains(MenuItemDescAndFuncName[0])) continue;
						}
						MenuItems.Emplace(MenuItemDescAndFuncName[0], MenuItemDescAndFuncName[1]);
					}
					else
					{
						UE_LOG(LogAbnormalPlugin, Warning, TEXT("解析配置文件DefaultRegisterMenu数据时出错，导致没有按照既定格式分割数据，请及时检查，其内容为%s"), *DefaultRegisterMenu);
					}
				}
			}
			// 生成按钮，绑定事件
			for (const auto& MenuItem : MenuItems)
			{
				if (auto Button = AbnormalPopMenu->RegisterMenuItem(MenuItem.Value))
				{
					FScriptDelegate Delegate;
					// 根据函数名进行绑定，传入的第一个参数为this是非正常基类，但是假如子类自己实现一个函数，依旧会正确绑定
					// 即便是非正常基类没有声明的函数，只要子类中定义了该函数，就能正确执行
					// 这个功能可谓相当的强大！！
					Delegate.BindUFunction(this, *MenuItem.Key);
					Button->OnClicked.AddUnique(Delegate);	
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

void AAbnormalBase::Cancel_Implementation()
{
	if (AbnormalPopMenu)
	{
		AbnormalPopMenu->SetVisibility(ESlateVisibility::Collapsed);
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


void AAbnormalBase::NotifyActorOnClicked(FKey ButtonPressed)
{
	// 目前使用的是默认的GameMode，PlayerController，除鼠标左键的点击根本无效
	// 还没有找到合适方法开启鼠标右键，因此先实现左键点击弹出菜单
	if (ButtonPressed == EKeys::LeftMouseButton)
	{
		if (AbnormalPopMenu)
		{
			if (auto PlayerController = GWorld->GetFirstPlayerController())
			{
				float MouseLocationX;
				float MouseLocationY;
				PlayerController->GetMousePosition(OUT MouseLocationX, OUT MouseLocationY);
				AbnormalPopMenu->SetPositionInViewport(FVector2D(MouseLocationX, MouseLocationY));
			}
			AbnormalPopMenu->SetVisibility(ESlateVisibility::Visible);
		}
	}

	// 父类的默认实现中，调用了蓝图里实现的ActorOnClicked事件
	// 因此如果蓝图中重新实现了ActorOnClicked事件，会追加这些操作
	Super::NotifyActorOnClicked(ButtonPressed);

}
