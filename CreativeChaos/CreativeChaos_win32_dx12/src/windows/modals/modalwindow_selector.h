#pragma once

#include "model/taskscommon.h"
#include "windows/modals/modalwindow.h"

class ModalWindow_Selector : public ModalWindow
{
    using Super = ModalWindow;
    
protected:
    void OnDraw() final;
    void OnOpen();
    void SetupInputs(InputHandler& inputHandler) final;

    virtual void InitOptions() = 0;
    virtual void OnConfirm(uint32_t optionIndex) {}
    virtual void OnCancel(uint32_t optionIndex) {}
    
protected:
    std::vector<std::string> _options;
    uint32_t _currentActionIdx = 0;
    
private:
    // Inputs
    void OnInput_PreviousAction(const InputEvent& e);
    void OnInput_NextAction(const InputEvent& e);
    void OnInput_Confirm(const InputEvent& e);
    void OnInput_Cancel(const InputEvent& e);

};