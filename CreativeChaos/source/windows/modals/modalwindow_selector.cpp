#include "pch.h"
#include "windows/modals/modalwindow_selector.h"

#include "imgui.h"

void ModalWindow_Selector::OnDraw()
{
    Super::OnDraw();

    ImGui::Text("Up/Down - Select | ENTER - Accept | ESC - Cancel");

    ImGui::NewLine();
    
    const std::string tableId { "##TaskSelector_Table" };
    if (ImGui::BeginTable(tableId.c_str(), 1, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
    {
        ImGui::TableSetupColumn("Option", ImGuiTableColumnFlags_WidthStretch);

        for (uint32_t idx = 0; idx < _options.size(); ++idx)
        {
            const std::string& option = _options[idx];
            
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);

            if (idx == _currentActionIdx)
            {
                ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg1, ImGui::GetColorU32(ImGui::GetStyle().Colors[ImGuiCol_NavHighlight]));
                ImGui::TextColored(ImVec4{ 1.f, 1.f, 1.f, 1.f }, "%s", option.c_str());
            }
            else
            {
                ImGui::Text("%s", option.c_str());
            }
        }
        
        ImGui::EndTable();
    }
}

void ModalWindow_Selector::OnOpen()
{
    Super::OnOpen();

    _options.clear();
    InitOptions();
}

void ModalWindow_Selector::SetupInputs(InputHandler& inputHandler)
{
    Super::SetupInputs(inputHandler);

    inputHandler.RegisterInputCallbackTemplated<ModalWindow_Selector>(this, EInputAction::Next, &ModalWindow_Selector::OnInput_NextAction);
    inputHandler.RegisterInputCallbackTemplated<ModalWindow_Selector>(this, EInputAction::Previous, &ModalWindow_Selector::OnInput_PreviousAction);
    inputHandler.RegisterInputCallbackTemplated<ModalWindow_Selector>(this, EInputAction::Confirm, &ModalWindow_Selector::OnInput_Confirm);
    inputHandler.RegisterInputCallbackTemplated<ModalWindow_Selector>(this, EInputAction::Cancel, &ModalWindow_Selector::OnInput_Cancel);
}

void ModalWindow_Selector::OnInput_PreviousAction(const InputEvent& /*e*/)
{
    if (_options.size() == 0)
    {
        return;
    }
    
    if (_currentActionIdx == 0)
    {
        _currentActionIdx = static_cast<uint32_t>(_options.size()) - 1u;
    }
    else
    {
        --_currentActionIdx;
    }
}

void ModalWindow_Selector::OnInput_NextAction(const InputEvent& /*e*/)
{
    if (_options.size() == 0)
    {
        return;
    }
    
    _currentActionIdx = (_currentActionIdx + 1) % _options.size();
}

void ModalWindow_Selector::OnInput_Confirm(const InputEvent& /*e*/)
{
    OnConfirm(_currentActionIdx);
    CloseModal();
}

void ModalWindow_Selector::OnInput_Cancel(const InputEvent& /*e*/)
{
    OnCancel(_currentActionIdx);
    CloseModal();
}
