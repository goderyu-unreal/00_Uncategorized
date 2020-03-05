// Fill out your copyright notice in the Description page of Project Settings.


#include "ExtraDataPortInfo.h"


// Sets default values
AExtraDataPortInfo::AExtraDataPortInfo()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

const TArray<FDataPortInfo> AExtraDataPortInfo::IntegrateDataPorts()
{
	TArray<FDataPortInfo> ExtraDataPortInfo;

	AppendDataPort(ExtraDataPortInfo, StringReadOnlyDataPorts, EDataPortType::DPT_STRING, EDataPortMode::DPM_READONLY);
	AppendDataPort(ExtraDataPortInfo, IntegerReadOnlyDataPorts, EDataPortType::DPT_INTEGER, EDataPortMode::DPM_READONLY);
	AppendDataPort(ExtraDataPortInfo, RealReadOnlyDataPorts, EDataPortType::DPT_REAL, EDataPortMode::DPM_READONLY);
	AppendDataPort(ExtraDataPortInfo, BoolReadOnlyDataPorts, EDataPortType::DPT_BOOL, EDataPortMode::DPM_READONLY);

	AppendDataPort(ExtraDataPortInfo, StringReadWriteDataPorts, EDataPortType::DPT_STRING, EDataPortMode::DPM_READWRITE);
	AppendDataPort(ExtraDataPortInfo, IntegerReadWriteDataPorts, EDataPortType::DPT_INTEGER, EDataPortMode::DPM_READWRITE);
	AppendDataPort(ExtraDataPortInfo, RealReadWriteDataPorts, EDataPortType::DPT_REAL, EDataPortMode::DPM_READWRITE);
	AppendDataPort(ExtraDataPortInfo, BoolReadWriteDataPorts, EDataPortType::DPT_BOOL, EDataPortMode::DPM_READWRITE);

	return ExtraDataPortInfo;
}

void AExtraDataPortInfo::AppendDataPort(
	OUT TArray<FDataPortInfo>& ExtraDataPortInfo,
	const TMap<FString, FString>& CategorizedDataPorts,
	const EDataPortType& Type,
	const EDataPortMode& Mode)
{
	for (auto& NameAndDescription : CategorizedDataPorts)
	{
		if (NameAndDescription.Key.IsEmpty())
		{
			continue;
		}
		auto PtrFindedItem = ExtraDataPortInfo.FindByPredicate([&](FDataPortInfo& Info){
			return Info.Name == NameAndDescription.Key;
		});
		if (PtrFindedItem)
		{
			UE_LOG(LogAutoExportDataPort, Warning, TEXT("关卡%s的额外通道权限配置填表存在重名通道%s"), *(this->GetLevel()->GetOuter()->GetName()),*(NameAndDescription.Key));
		}
		else
		{
			ExtraDataPortInfo.Emplace(FDataPortInfo(NameAndDescription.Key, Type, Mode, NameAndDescription.Value));
		}
	}
}

// Called when the game starts or when spawned
void AExtraDataPortInfo::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AExtraDataPortInfo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

