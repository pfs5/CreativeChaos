#include "pch.h"
#include "windows/modals/modalwindow_setcustomcategory.h"

#include "imgui.h"
#include "managers/configmanager.h"
#include "managers/taskmanager.h"

void ModalWindow_SetCustomCategory::InitOptions()
{
    const std::vector<std::string>& categories = ConfigManagerProxy::Get().GetData().TaskCategories;

    _options = categories;
}

void ModalWindow_SetCustomCategory::OnConfirm(uint32_t optionIndex)
{
    ModalWindow_Selector::OnConfirm(optionIndex);

    const std::vector<std::string>& categories = ConfigManagerProxy::Get().GetData().TaskCategories;
    if (_targetTask.IsValid() && optionIndex < categories.size())
    {
        TaskManagerProxy::Get().SetTaskCustomCategory(_targetTask, categories[optionIndex].c_str());
    }
}

ImVec2 ModalWindow_SetCustomCategory::GetModalSize() const
{
    return ImVec2{ 600.f, 250.f };
}
