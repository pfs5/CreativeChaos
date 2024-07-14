#pragma once
#include "modalwindow_selector.h"

class ModalWindow_SetCustomCategory : public ModalWindow_Selector
{
public:
    ModalWindow_SetCustomCategory(const TaskPtr& task):
        _targetTask{ task }
    {
        
    }
    
protected:
    void InitOptions() override;
    void OnConfirm(uint32_t optionIndex) override;

private:
    TaskPtr _targetTask;

private:
    const char* GetModalTitle() const override { return "Set Category"; }
    ImVec2 GetModalSize() const override;
};
