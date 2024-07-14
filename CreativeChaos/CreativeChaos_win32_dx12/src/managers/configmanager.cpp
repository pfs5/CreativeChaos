#include "pch.h"
#include "managers/configmanager.h"

#include "core/staticconfig.h"
#include "util/json.h"

void to_json(nlohmann::json& j, const ConfigManager::Data& data)
{
    j = nlohmann::json
    {
        {"lastTaskView", data.LastTaskView},
        {"taskCategories", data.TaskCategories}
    };
}

void from_json(const nlohmann::json& j, ConfigManager::Data& data)
{
    json::TryGet(j, "lastTaskView", data.LastTaskView);
    json::TryGet(j, "taskCategories", data.TaskCategories);
}

void ConfigManager::Initialize()
{
    Manager::Initialize();

    LoadData();

    // Force save to update config file with new data.
    SaveData();
}

void ConfigManager::UpdateData(const UpdateDataPredicate& predicate)
{
    bool shouldSaveData = false;
    predicate(_data, shouldSaveData);
    
    if (shouldSaveData)
    {
        SaveData();
    }
}

void ConfigManager::SaveData()
{
    nlohmann::json baseObject;
    baseObject["data"] = _data;

    std::ofstream outFile(StaticConfig::DB_PATH_CONFIG);
    outFile << std::setw(4) << baseObject << std::endl;
}

void ConfigManager::LoadData()
{
    std::ifstream f(StaticConfig::DB_PATH_CONFIG);
    if (!f.is_open())
    {
        // Database doesn't exist - force save to create an empty database.
        SaveData();

        return;
    }

    nlohmann::json baseObject = nlohmann::json::parse(f);
    from_json(baseObject["data"], _data);
}
