
#include "ConfigFileManager.h"
#include "Misc/Paths.h"
ConfigFileManager::ConfigFileManager(/* args */)
{
    FString ConfigFilePath = FPaths::ProjectConfigDir() + FString(TEXT("AutoExportDataPort/Config.ini"));
    ConfigFile.Read(ConfigFilePath);
}

ConfigFileManager::~ConfigFileManager()
{
}

ConfigFileManager ConfigFileManager::GetInstance()
{
    static ConfigFileManager Instance;
    return Instance;
}

FString ConfigFileManager::GetValue(const FName& Key, const FString& Section) const
{
    if (!Section.IsEmpty())
    {
        auto TargetSection = ConfigFile.Find(Section);
        if (TargetSection)
        {
            if (TargetSection->Contains(Key))
            {
                return TargetSection->Find(Key)->GetValue().Replace(TEXT("\\n"), TEXT("\n"));
            }
        }
        return "";
    }
    else
    {
        for (const auto& Pair : ConfigFile)
        {
            if (Pair.Value.Contains(Key))
            {
                return Pair.Value.Find(Key)->GetValue().Replace(TEXT("\\n"), TEXT("\n"));
            }
        }
        return "";
    }
    
}