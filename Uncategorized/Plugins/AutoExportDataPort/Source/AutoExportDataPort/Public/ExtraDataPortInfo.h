// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ExtraDataPortInfo.generated.h"

UENUM(BlueprintType)
enum class EDataPortType : uint8
{
	DPT_STRING  UMETA(DisplayName = "字符串型"),
	DPT_INTEGER UMETA(DisplayName = "整数型"),
	DPT_REAL    UMETA(DisplayName = "实数型"),
	DPT_BOOL    UMETA(DisplayName = "布尔型")
};

UENUM(BlueprintType)
enum class EDataPortMode : uint8
{
	DPM_READWRITE UMETA(DisplayName = "读写权限"),
	DPM_READONLY  UMETA(DisplayName = "只读权限"),
	DPM_WRITEONLY UMETA(DisplayName = "只写权限")
};

USTRUCT(BlueprintType)
struct FDataPortInfo
{
	GENERATED_USTRUCT_BODY()
	/// 通道名
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString       Name;
	/// 通道类型
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EDataPortType Type;
	/// UE4对该通道的权限
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EDataPortMode Mode;
	/// 通道详细描述
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString       Description;

	FDataPortInfo() {}

	FDataPortInfo(const FString& _Name, const EDataPortType& _Type, const EDataPortMode& _Mode, const FString& _Description)
	: Name(_Name), Type(_Type), Mode(_Mode), Description(_Description) {}
};

UCLASS()
class AUTOEXPORTDATAPORT_API AExtraDataPortInfo : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AExtraDataPortInfo();
	/// 字符串型读权限通道，左侧填通道名，右侧可选填通道描述
	UPROPERTY(EditAnywhere, Category = "AutoExportDataPort | ModeReadOnly", DisplayName = "String")
	TMap<FString, FString> StringReadOnlyDataPorts;
	/// 整数型读权限通道，左侧填通道名，右侧可选填通道描述
	UPROPERTY(EditAnywhere, Category = "AutoExportDataPort | ModeReadOnly", DisplayName = "Integer")
	TMap<FString, FString> IntegerReadOnlyDataPorts;
	/// 实数型读权限通道，左侧填通道名，右侧可选填通道描述
	UPROPERTY(EditAnywhere, Category = "AutoExportDataPort | ModeReadOnly", DisplayName = "Real")
	TMap<FString, FString> RealReadOnlyDataPorts;
	/// 布尔型读权限通道，左侧填通道名，右侧可选填通道描述
	UPROPERTY(EditAnywhere, Category = "AutoExportDataPort | ModeReadOnly", DisplayName = "Bool")
	TMap<FString, FString> BoolReadOnlyDataPorts;
	/// 字符串型读写权限通道，左侧填通道名，右侧可选填通道描述
	UPROPERTY(EditAnywhere, Category = "AutoExportDataPort | ModeReadWrite", DisplayName = "String")
	TMap<FString, FString> StringReadWriteDataPorts;
	/// 整数型读写权限通道，左侧填通道名，右侧可选填通道描述
	UPROPERTY(EditAnywhere, Category = "AutoExportDataPort | ModeReadWrite", DisplayName = "Integer")
	TMap<FString, FString> IntegerReadWriteDataPorts;
	/// 实数型读写权限通道，左侧填通道名，右侧可选填通道描述
	UPROPERTY(EditAnywhere, Category = "AutoExportDataPort | ModeReadWrite", DisplayName = "Real")
	TMap<FString, FString> RealReadWriteDataPorts;
	/// 布尔型读写权限通道，左侧填通道名，右侧可选填通道描述
	UPROPERTY(EditAnywhere, Category = "AutoExportDataPort | ModeReadWrite", DisplayName = "Bool")
	TMap<FString, FString> BoolReadWriteDataPorts;

	/// 整合所有通道权限填表的通道数据
	UFUNCTION(BlueprintCallable, Category = "AutoExportDataPort | ExtraDataPortInfo")
	const TArray<FDataPortInfo> IntegrateDataPorts();

	inline void AppendDataPort(OUT TArray<FDataPortInfo>& ExtraDataPortInfo,
									const TMap<FString, FString>& CategorizedDataPorts,
									const EDataPortType& Type,
									const EDataPortMode& Mode);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
