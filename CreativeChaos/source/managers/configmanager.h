#pragma once

#include "core/manager.h"
#include "util/proxy.h"
#include "util/jsonfwd.h"

class ConfigManager : public Manager
{
public:
    struct Data
    {
        std::string LastTaskView;
        std::vector<std::string> TaskCategories;
    };

public:
    void Initialize() override;
    
    const Data& GetData() const { return _data; }

    using UpdateDataPredicate = std::function<void(Data& data, bool& dataUpdated)>;
    void UpdateData(const UpdateDataPredicate& predicate);

private:
    void SaveData();
    void LoadData();
    
private:
    Data _data;
};

struct ConfigManagerProxy : public Proxy<ConfigManager>
{
    ConfigManagerProxy();
};

void to_json(nlohmann::json& j, const ConfigManager::Data& data);
void from_json(const nlohmann::json& j, ConfigManager::Data& data);
