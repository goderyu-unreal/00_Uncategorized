// Fill out your copyright notice in the Description page of Project Settings.


#include "DataPortManager.h"
#include "EngineUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogAutoExportDataPort, Log, All)

// Sets default values
ADataPortManager::ADataPortManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// for (const auto& DataPortInfo : DataPortsInfo)
	// {
	// 	if (DataPortInfo.Name.IsEmpty())
	// 	{
	// 		++EmptyDataPortNameCount;
	// 	}
	// }
}

ADataPortManager* ADataPortManager::GetInstance(const UObject* WorldContextObject)
{
	static ADataPortManager* Instance = nullptr;
	if (!Instance->IsValidLowLevel())
	{
		if (auto It = TActorIterator<ADataPortManager>(WorldContextObject->GetWorld()))
		{
			Instance = *It;
		}
		else
		{
			Instance = WorldContextObject->GetWorld()->SpawnActor<ADataPortManager>();
		}
	}
	return Instance;
}

void ADataPortManager::AddDataPort(const FString& Name, const EDataPortType& Type, const EDataPortMode& Mode, const FString& Description)
{
	auto It = DataPortsInfo.FindByPredicate(
		[=](const FDataPortInfo& IteratorDataPortInfo) {
			return IteratorDataPortInfo.Name == Name;
		});
	if (!It)
	{
		DataPortsInfo.Emplace(FDataPortInfo(Name, Type, Mode, Description));
	}
}

void ADataPortManager::ExportDataPortsInfo(const FString& SavedFileName)
{
	FString FileContent = ANSI_TO_TCHAR("ChannelName,Type,Mode\r\n");
	for (const auto& DataPortInfo : DataPortsInfo)
	{
		if (DataPortInfo.Name.IsEmpty())
		{
			++EmptyDataPortNameCount;
			continue;
		}


		FString Type;
		FString Mode;
		switch (DataPortInfo.Type)
		{
		case EDataPortType::DPT_BOOL:
			Type = FString(TEXT("Bool"));
			break;
		case EDataPortType::DPT_INTEGER:
			Type = FString(TEXT("Integer"));
			break;
		case EDataPortType::DPT_REAL:
			Type = FString(TEXT("Real"));
			break;
		case EDataPortType::DPT_STRING:
			Type = FString(TEXT("String"));
			break;
		default:
			break;
		}
		switch (DataPortInfo.Mode)
		{
		case EDataPortMode::DPM_READONLY:
			Mode = FString(TEXT("Input"));
			break;
		case EDataPortMode::DPM_WRITEONLY:
			Mode = FString(TEXT("Output"));
			break;
		case EDataPortMode::DPM_READWRITE:
			// TODO 现在读写和只读没有区分，只要是Output即为读写权限
			Mode = FString(TEXT("Output"));
			break;
		default:
			break;
		}

	}
}

// Called when the game starts or when spawned
void ADataPortManager::BeginPlay()
{
	Super::BeginPlay();
	
	for (const auto& DataPortInfo : DataPortsInfo)
	{
		if (DataPortInfo.Name.IsEmpty())
		{
			++EmptyDataPortNameCount;
		}
	}
	UE_LOG(LogAutoExportDataPort, Warning, TEXT("There are a total of %d dataports named null"), EmptyDataPortNameCount);
	UE_LOG(LogAutoExportDataPort, Warning, TEXT("共计有 %d 个通道名为空"), EmptyDataPortNameCount);
}

// Called every frame
void ADataPortManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

