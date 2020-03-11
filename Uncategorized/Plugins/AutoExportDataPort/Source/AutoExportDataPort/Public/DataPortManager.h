// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ExtraDataPortInfo.h"
#include "ConfigFileManager.h"
#include "DataPortManager.generated.h"

// TODO 1.是否添加一个功能，在额外通道信息Actor构造函数中设置标签，一旦管理器生成了权限文件后即可根据标签销毁所有额外通道信息Actor节约资源。

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDataPortInfoFileGenerated, const FString, GeneratedDataPortInfoFileFullPath);

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
	/**
	 * @brief 将DataPortsInfo的所有信息导出成csv文件
	 * 
	 * @param SavedFileName 要保存的文件名，注意传值时不要带后缀名，默认已加.csv
	 */
	void ExportDataPortsInfo(const FString& SavedFileName);
	// void AddWriteOnlyDataPort(const FDataPortInfo& WriteOnlyDataPortInfo);

	// UFUNCTION(BlueprintCallable, Category = AutoExportDataPort)
	// void AddReadWriteDataPort(const FDataPortInfo& ReadWriteDataPortInfo);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = AutoExportDataPort)
	void IntegrateDataPorts();

	void IntegrateDataPorts_Implementation();

public:
	/// 委托，用于通知通道权限文件已生成，传递一个文件完整路径名的参数
	UPROPERTY(BlueprintAssignable, Category = AutoExportDataPort)
	FOnDataPortInfoFileGenerated OnDataPortInfoFileGenerated;

	/// 固定的通道权限信息，注意：若此处添加的通道已经存在，会覆盖之前其他地方添加的通道。
	/// Key: 通道名
	/// Value: 通道类型、权限、详细描述的结构体
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = AutoExportDataPort)
	TMap<FString, FDataPortInfo> FixedDataPortsInfo;
	/// 存放程序运行期间动态获取到的所有通道数据，配合AddLocomotiveLevelDataPorts
	UPROPERTY(BlueprintReadWrite, Category = AutoExportDataPort)
	TMap<FString, FDataPortInfo> DynamicGotDataPortsInfo;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	/// 最终输出到表文件的所有通道信息的Map
	TMap<FString, FDataPortInfo> DataPortsInfo;

	/// 导出时检测DataPortsInfo中有多少个空通道，因为是Map表，肯定值在0或1...不如去掉
	uint32 EmptyDataPortNameCount = 0;

	inline const FString GetDataPortTypeString(const EDataPortType& Type) const;
	inline const FString GetDataPortModeString(const EDataPortMode& Mode) const;
	inline const EDataPortType GetDataPortTypeEnum(const FString& Type) const;
	inline const EDataPortMode GetDataPortModeEnum(const FString& Mode) const;

	UConfigFileManager ConfigFileManager;
};
