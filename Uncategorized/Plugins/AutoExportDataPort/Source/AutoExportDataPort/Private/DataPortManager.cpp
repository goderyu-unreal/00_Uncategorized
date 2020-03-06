// Fill out your copyright notice in the Description page of Project Settings.


#include "DataPortManager.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
/// 以下两个头文件是用来获取项目目录以及使用文件接口写文件用的
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"

DEFINE_LOG_CATEGORY_STATIC(LogAutoExportDataPort, Log, All)

// Sets default values
ADataPortManager::ADataPortManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
		PrimaryActorTick.bCanEverTick = true;
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

void ADataPortManager::AddDataPort(const FString& Name, const EDataPortType Type, const EDataPortMode Mode, const FString& Description)
{
	auto It = DataPortsInfo.Find(Name);
	if (!It)
	{
		DataPortsInfo.Emplace(Name, FDataPortInfo(Type, Mode, Description));
	}
}

void ADataPortManager::ExportDataPortsInfo(const FString& SavedFileName)
{
	FString FilePath = FPaths::ConvertRelativePathToFull(FPaths::ProjectConfigDir()) + SavedFileName;
	FString FileContent = ANSI_TO_TCHAR("ChannelName,Type,Mode\r\n");
	/// 按通道名字母正序排序
	DataPortsInfo.KeyStableSort([](const FString& A, const FString& B) {
		return A < B;
	});

	for (const auto& DataPortInfo : DataPortsInfo)
	{
		if (DataPortInfo.Key.IsEmpty())
		{
			++EmptyDataPortNameCount;
			continue;
		}
		auto Type = GetDataPortTypeString(DataPortInfo.Value.Type);
		auto Mode = GetDataPortModeString(DataPortInfo.Value.Mode);
		FileContent += (DataPortInfo.Key + "," + Type + "," + Mode);
		FileContent += "\r\n";
	}
	FFileHelper::SaveStringToFile(FileContent, *FilePath, FFileHelper::EEncodingOptions::ForceUTF8, &IFileManager::Get(), EFileWrite::FILEWRITE_NoReplaceExisting);
	UE_LOG(LogAutoExportDataPort, Warning, TEXT("输出通道权限表时检测到%d个空通道，已过滤"), EmptyDataPortNameCount);
	// 通知对通道权限信息表已生成完毕这一消息感兴趣的模块，并提供给它们一个生成文件路径
	OnDataPortInfoFileGenerated.Broadcast(FilePath);
}

void ADataPortManager::IntegrateDataPorts_Implementation()
{
	// 将所有额外填表的通道信息整合
	TArray<AActor*> FindedActors; 
	UGameplayStatics::GetAllActorsOfClass(this, AExtraDataPortInfo::StaticClass(), OUT FindedActors);
	for (const auto& Actor : FindedActors)
	{
		if (auto ExtraDataPortInfoActor = Cast<AExtraDataPortInfo>(Actor))
		{
			ExtraDataPortInfoActor->IntegrateDataPorts(OUT DataPortsInfo);
		}
		else
		{
			UE_LOG(LogAutoExportDataPort, Warning, TEXT("获取所有ExtraDataPortInfo的Actor时从AActor转型到AExtraDataPortInfo失败一个，该Actor所在关卡为%s"), *Actor->GetLevel()->GetName());
		}
	}

	// TODO 将本管理类添加的默认通道整合，根据类型区分（视景只读/可操作读写）
	for (const auto& FixedDataPortInfo : FixedDataPortsInfo)
	{
		if (DataPortsInfo.Contains(FixedDataPortInfo.Key))
		{
			UE_LOG(LogAutoExportDataPort, Warning, TEXT("添加默认固定通道时发现已有重名通道%s，但已将其覆盖"), *FixedDataPortInfo.Key);
		}
		DataPortsInfo.Emplace(FixedDataPortInfo.Key, FixedDataPortInfo.Value);
	}
}

// Called when the game starts or when spawned
void ADataPortManager::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ADataPortManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

const FString ADataPortManager::GetDataPortTypeString(const EDataPortType& Type) const
{
	switch (Type)
	{
	case EDataPortType::DPT_BOOL:
		return FString(TEXT("Bool"));
	case EDataPortType::DPT_INTEGER:
		return FString(TEXT("Integer"));
	case EDataPortType::DPT_REAL:
		return FString(TEXT("Real"));
	case EDataPortType::DPT_STRING:
		return FString(TEXT("String"));
	default:
		return FString(TEXT("Unknown"));
	}
}

const FString ADataPortManager::GetDataPortModeString(const EDataPortMode& Mode) const
{
	switch (Mode)
	{
	case EDataPortMode::DPM_READONLY:
		return FString(TEXT("Input"));
	case EDataPortMode::DPM_WRITEONLY:
		return FString(TEXT("Output"));
	case EDataPortMode::DPM_READWRITE:
		// TODO 现在读写和只读没有区分，只要是Output即为读写权限
		return FString(TEXT("Output"));
	default:
		return FString(TEXT("Unknown"));
	}
}