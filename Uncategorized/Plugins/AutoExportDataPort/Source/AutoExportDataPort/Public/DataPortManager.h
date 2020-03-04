// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DataPortManager.generated.h"

UENUM(BlueprintType)
enum class EDataPortType : uint8
{
	DPT_BOOL    UMETA(DisplayName = "布尔型"),
	DPT_INTEGER UMETA(DisplayName = "整数型"),
	DPT_REAL    UMETA(DisplayName = "实数型"),
	DPT_STRING  UMETA(DisplayName = "字符串型")
};

UENUM(BlueprintType)
enum class EDataPortMode : uint8
{
	DPM_READONLY  UMETA(DisplayName = "只读权限"),
	DPM_WRITEONLY UMETA(DisplayName = "只写权限"),
	DPM_READWRITE UMETA(DisplayName = "读写权限")
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

	FDataPortInfo(const FString& _Name,EDataPortType _Type, EDataPortMode _Mode, FString _Description)
	: Name(_Name), Type(_Type), Mode(_Mode), Description(_Description) {}
};

UCLASS()
class AUTOEXPORTDATAPORT_API ADataPortManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADataPortManager();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = AutoExportDataPort, meta = (WorldContext = "WorldContextObject", DisplayName = "Get DataPort Manager"))
	static ADataPortManager* GetInstance(const UObject* WorldContextObject);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = AutoExportDataPort)
	bool AddLocomotiveLevelDataPorts(const FString& LocomotiveLevelName);

	UFUNCTION(BlueprintCallable, Category = AutoExportDataPort)
	void AddDataPort(const FString& Name, const EDataPortType& Type = EDataPortType::DPT_STRING, const EDataPortMode& Mode = EDataPortMode::DPM_READWRITE, const FString& Description = FString(TEXT("")));

	UFUNCTION(BlueprintCallable, Category = AutoExportDataPort)
	void ExportDataPortsInfo(const FString& SavedFileName);
	// void AddWriteOnlyDataPort(const FDataPortInfo& WriteOnlyDataPortInfo);

	// UFUNCTION(BlueprintCallable, Category = AutoExportDataPort)
	// void AddReadWriteDataPort(const FDataPortInfo& ReadWriteDataPortInfo);

public:
	UPROPERTY(VisibleAnywhere, Category = AutoExportDataPort)
	uint32 EmptyDataPortNameCount = 0;

	/// 额外添加的通道权限信息，注意：若此处添加的通道已经存在，会覆盖之前其他地方添加的通道。
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = AutoExportDataPort)
	TArray<FDataPortInfo> ExtraDataPortsInfo;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	TArray<FDataPortInfo> DataPortsInfo;
};
