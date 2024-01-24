#pragma once

#include "imgui.h"
#include "util/util.h"

#define ImGuiScopedIndent() \
	ImGui::ScopedIndent UNIQUE_NAME(ScopedIndent_6e02ccd4);

namespace ImGui
{
	struct ScopedIndent
	{
		ScopedIndent() { ImGui::Indent(); }
		~ScopedIndent() { ImGui::Unindent(); }
	};
}