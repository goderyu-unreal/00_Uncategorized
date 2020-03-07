#pragma once

#include "CoreMinimal.h"
#include "Misc/ConfigCacheIni.h"

class UConfigFileManager
{
private:
    FConfigFile ConfigFile;
public:
    UConfigFileManager();
    ~UConfigFileManager();

    static UConfigFileManager GetInstance();

    FString GetValue(const FName& Key, const FString& Section = "") const;
};

