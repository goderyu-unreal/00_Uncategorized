// Fill out your copyright notice in the Description page of Project Settings.


#include "TestToolLib.h"
#include "UObject/UObjectIterator.h"
#include "Kismet/KismetSystemLibrary.h"

DEFINE_LOG_CATEGORY(LogTestTool)

void UTestToolLib::PrintLog(const FString& InText, const FColor& InColor)
{
	GEngine->AddOnScreenDebugMessage(-1, 100, InColor, *InText);
	UE_LOG(LogTestTool, Log, TEXT("%s"), *InText);
}


TMap<FString, int32> UTestToolLib::GetAllCategoriesObjects()
{
	TMap<FString, int32> TempMap;
	for (TObjectIterator<UObject> Iterator; Iterator; ++Iterator)
	{
		auto TempObject = *Iterator;
		if (TempObject->IsPendingKill())
			continue;
		auto TempClass = TempObject->GetClass();
		FString ClassName;
		TempClass->GetName(ClassName);
		auto Log = FString::Printf(TEXT("%s"), *ClassName);
		//PrintLog(Log, FColor::White);
		if (TempMap.Contains(ClassName))
		{
			++TempMap[ClassName];
		}
		else
		{
			TempMap.Add(ClassName, 1);
		}
	}
	return MoveTempIfPossible(TempMap);
}

void UTestToolLib::CompareTwoMapDifferent(const TMap<FString, int32>& InOldMap, const TMap<FString, int32>& InNewMap)
{
	for (const auto& Item : InOldMap)
	{
		if (auto ValuePtr = InNewMap.Find(Item.Key))
		{
			FString Log = Item.Key + ":" + FString::Printf(TEXT("%d-->%d"), Item.Value, *ValuePtr);
			if (Item.Value < *ValuePtr)
			{
				PrintLog(Log, FColor::Red);
			}
			else if (Item.Value > *ValuePtr)
			{
				PrintLog(Log, FColor::Green);
			}
		}
		else
		{
			FString Log = Item.Key + ":" + FString::Printf(TEXT("%d-->0"), Item.Value);
			PrintLog(Log, FColor::Silver);
		}
		
	}
	for (const auto& Item : InNewMap)
	{
		if (auto ValuePtr = InOldMap.Find(Item.Key))
		{
			continue;
		}
		else
		{
			FString Log = Item.Key + ":" + FString::Printf(TEXT("0-->%d"), Item.Value);
			PrintLog(Log, FColor::Cyan);
		}
	}
}

void UTestToolLib::GetAllObjectsOfClass(TSubclassOf<UObject> ObjectClass, TArray<UObject*>& OutObjects)
{
	OutObjects.Reset();
	if (ObjectClass)
	{
		for (TObjectIterator<UObject> Iterator; Iterator; ++Iterator)
		{

		}
	}
}