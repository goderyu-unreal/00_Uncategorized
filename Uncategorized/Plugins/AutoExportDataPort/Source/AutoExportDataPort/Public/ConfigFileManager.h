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


    FString GetValue(const FName& Key, const FString& Section = "") const;
private:
    // 先别用单例了，编辑器运行状态下每一次点播放不能获得动态修改的ini文件
    static UConfigFileManager GetInstance();
};

