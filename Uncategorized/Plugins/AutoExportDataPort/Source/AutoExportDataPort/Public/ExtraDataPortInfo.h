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

	FDataPortInfo(const EDataPortType& _Type, const EDataPortMode& _Mode, const FString& _Description)
	: Type(_Type), Mode(_Mode), Description(_Description) {}
};

UCLASS()
class AUTOEXPORTDATAPORT_API AExtraDataPortInfo : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AExtraDataPortInfo();
	UPROPERTY(EditAnywhere, Category = "AutoExportDataPort | ModeReadOnly", DisplayName = "String")
	/// 字符串型读权限通道，左侧填通道名，右侧可选填通道描述
	TMap<FString, FString> StringReadOnlyDataPorts;
	UPROPERTY(EditAnywhere, Category = "AutoExportDataPort | ModeReadOnly", DisplayName = "Integer")
	/// 整数型读权限通道，左侧填通道名，右侧可选填通道描述
	TMap<FString, FString> IntegerReadOnlyDataPorts;
	UPROPERTY(EditAnywhere, Category = "AutoExportDataPort | ModeReadOnly", DisplayName = "Real")
	/// 实数型读权限通道，左侧填通道名，右侧可选填通道描述
	TMap<FString, FString> RealReadOnlyDataPorts;
	UPROPERTY(EditAnywhere, Category = "AutoExportDataPort | ModeReadOnly", DisplayName = "Bool")
	/// 布尔型读权限通道，左侧填通道名，右侧可选填通道描述
	TMap<FString, FString> BoolReadOnlyDataPorts;
	UPROPERTY(EditAnywhere, Category = "AutoExportDataPort | ModeReadWrite", DisplayName = "String")
	/// 字符串型读写权限通道，左侧填通道名，右侧可选填通道描述
	TMap<FString, FString> StringReadWriteDataPorts;
	UPROPERTY(EditAnywhere, Category = "AutoExportDataPort | ModeReadWrite", DisplayName = "Integer")
	/// 整数型读写权限通道，左侧填通道名，右侧可选填通道描述
	TMap<FString, FString> IntegerReadWriteDataPorts;
	UPROPERTY(EditAnywhere, Category = "AutoExportDataPort | ModeReadWrite", DisplayName = "Real")
	/// 实数型读写权限通道，左侧填通道名，右侧可选填通道描述
	TMap<FString, FString> RealReadWriteDataPorts;
	UPROPERTY(EditAnywhere, Category = "AutoExportDataPort | ModeReadWrite", DisplayName = "Bool")
	/// 布尔型读写权限通道，左侧填通道名，右侧可选填通道描述
	TMap<FString, FString> BoolReadWriteDataPorts;

	UFUNCTION(BlueprintCallable, Category = "AutoExportDataPort | ExtraDataPortInfo")
	/**
	 * @brief 整合该Actor填表数据中的所有通道信息到DataPortManager的通道Map表中
	 * 
	 * @param DataPortsInfo DataPortManager类中的成员变量DataPortsInfo
	 */
	void IntegrateDataPorts(TMap<FString, FDataPortInfo>& DataPortsInfo);

	/**
	 * @brief 将填表中每一个分类的通道信息组装成对应格式添加到
	 * DataPortManager类中的成员变量DataPortsInfoz中
	 * 
	 * @param DataPortsInfo DataPortManager类中的成员变量DataPortsInfoz
	 * @param CategorizedDataPorts 每一个具体的分类Map
	 * @param Type 标识分类Map表中的通道是什么类型的
	 * @param Mode 表示分类Map表中的通道是什么权限的
	 */
	inline void AppendDataPort(TMap<FString, FDataPortInfo>& DataPortsInfo,
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
