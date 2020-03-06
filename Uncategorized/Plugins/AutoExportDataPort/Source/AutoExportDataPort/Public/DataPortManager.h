// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ExtraDataPortInfo.h"
#include "DataPortManager.generated.h"

// TODO 1.整合所有ExtraDataPortInfo的通道信息，整合本管理存放的默认通道信息，整合关卡内动态生成的通道信息
// TODO 2.将整合后的一整个TArray导出到文件中
// TODO 3.是否添加一个功能，在额外通道信息Actor构造函数中设置标签，一旦管理器生成了权限文件后即可根据标签销毁所有额外通道信息Actor节约资源

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
	void AddDataPort(const FString& Name, const EDataPortType Type, const EDataPortMode Mode, const FString& Description);

	UFUNCTION(BlueprintCallable, Category = AutoExportDataPort)
	void ExportDataPortsInfo(const FString& SavedFileName);
	// void AddWriteOnlyDataPort(const FDataPortInfo& WriteOnlyDataPortInfo);

	// UFUNCTION(BlueprintCallable, Category = AutoExportDataPort)
	// void AddReadWriteDataPort(const FDataPortInfo& ReadWriteDataPortInfo);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = AutoExportDataPort)
	void IntegrateDataPorts();

	void IntegrateDataPorts_Implementation();
public:
	UPROPERTY(VisibleAnywhere, Category = AutoExportDataPort)
	uint32 EmptyDataPortNameCount = 0;

	/// 固定的通道权限信息，注意：若此处添加的通道已经存在，会覆盖之前其他地方添加的通道。
	/// 专供最后的修改手段
	/// Key: 通道名
	/// Value: 通道类型、权限、详细描述的结构体
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = AutoExportDataPort)
	TMap<FString, FDataPortInfo> FixedDataPortsInfo;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	/// 最终输出到表文件的所有通道信息的Map
	TMap<FString, FDataPortInfo> DataPortsInfo;
};
