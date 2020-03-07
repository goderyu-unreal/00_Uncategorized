#pragma once

#include "CoreMinimal.h"
#include "Misc/ConfigCacheIni.h"

class ConfigFileManager
{
private:
    FConfigFile ConfigFile;
public:
    ConfigFileManager();
    ~ConfigFileManager();

    static ConfigFileManager GetInstance();

    FString GetValue(const FName& Key, const FString& Section = "") const;
};

