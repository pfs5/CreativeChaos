#pragma once

#include "modalwindow_selector.h"
#include "model/taskscommon.h"

class ModalWindow_TaskAction : public ModalWindow_Selector
{
    using Super = ModalWindow;
    using ActionProcessPtr = void(ModalWindow_TaskAction::*)();

public:
    ModalWindow_TaskAction(const TaskPtr& task):
        _targetTask{ task }
    {
        
    }
    
protected:
    void InitOptions() override;
    void OnConfirm(uint32_t optionIndex) override;

private:
    struct Action
    {
        std::string Name;
        ActionProcessPtr Process;
    };
    
private:
    TaskPtr _targetTask;
    
    std::vector<Action> _actions;
    uint32_t _currentActionIdx = 0;
    
private:
    const char* GetModalTitle() const final { return "Task action"; }
    ImVec2 GetModalSize() const final;
    
    void RegisterAction(const char* name, ActionProcessPtr process)
    {
        _actions.emplace_back(name, process);
    }

    // Actions
    void OnAction_CompleteTask();
};