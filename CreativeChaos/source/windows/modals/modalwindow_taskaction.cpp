#include "pch.h"
#include "windows/modals/modalwindow_taskaction.h"

#include "imgui.h"
#include "managers/TaskManager.h"

void ModalWindow_TaskAction::InitOptions()
{
    _actions.clear();

    if (_targetTask.IsValid())
    {
        const Task& task = _targetTask.GetTask();

        if(!task.Done)
        {
            RegisterAction("Complete task", &ModalWindow_TaskAction::OnAction_CompleteTask);
        }
    }
    
    _options.reserve(_actions.size());
    for (const Action& action : _actions)
    {
        _options.emplace_back(action.Name);
    }
}

void ModalWindow_TaskAction::OnConfirm(uint32_t optionIndex)
{
    ModalWindow_Selector::OnConfirm(optionIndex);

    if(optionIndex < _actions.size())
    {
        (this->*_actions[optionIndex].Process)();
    }
}

ImVec2 ModalWindow_TaskAction::GetModalSize() const
{
    return ImVec2{ 600.f, 250.f };
}

void ModalWindow_TaskAction::OnAction_CompleteTask()
{
    if (_targetTask.IsValid())
    {
        TaskManagerProxy::Get().SetTaskDone(_targetTask, true);
    }
}

