// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TestToolLib.generated.h"


DECLARE_LOG_CATEGORY_EXTERN(LogTestTool, Log, All)

/**
 * 
 */
UCLASS()
class TESTTOOL_API UTestToolLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "TestTool|Lib")
	/**
	* @return FString: ObjectName, int32: ObjectsCount
	*/
	static TMap<FString, int32> GetAllCategoriesObjects();

	UFUNCTION(BlueprintCallable, Category = "TestTool|Lib")
	static void CompareTwoMapDifferent(const TMap<FString, int32>& InMapA, const TMap<FString, int32>& InMapB);

	static void PrintLog(const FString& InText, const FColor& InColor);
};
