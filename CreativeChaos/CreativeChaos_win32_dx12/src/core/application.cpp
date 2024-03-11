#include "pch.h"
#include "core/application.h"

#include "imgui.h"
#include "time/time.h"
#include "managers/WindowManager.h"

void Application::Init()
{
	LoadFonts();
	InitTheme_1();
	ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NavEnableKeyboard;
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavNoCaptureKeyboard;

	// Setup
	DebugManagerProxy::Open(_debugManager);
	TaskManagerProxy::Open(_taskManager);
	InputManagerProxy::Open(_inputManager);
	StateManagerProxy::Open(_stateManager);

	RegisterManager(_debugManager);	// must be first
	RegisterManager(_taskManager);
	RegisterManager(_inputManager);
	RegisterManager(_stateManager);

	RegisterWindow(_windowTasks);
	RegisterWindow(_windowProperties);
	RegisterWindow(_windowDebug);
	RegisterWindow(_windowLog);

	_taskManager.Initialize();

	DebugManagerProxy::Get().PushLog("App initialized.");

	_isOpen = true;
	ApplicationProxy::Open(*this);
}

void Application::Shutdown()
{
	ApplicationProxy::Close(*this);

	StateManagerProxy::Close(_stateManager);
	InputManagerProxy::Close(_inputManager);
	TaskManagerProxy::Close(_taskManager);
	DebugManagerProxy::Close(_debugManager);
}

void Application::Update()
{
	for (Manager* m : _managers)
	{
		m->Update();
	}

	static bool showDemoWindow = false;

	bool newTaskPressed = false;
	ImGui::BeginMainMenuBar();
	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::MenuItem("New Task", "CTRL+N"))
		{
			newTaskPressed = true;
		}

		if (ImGui::MenuItem("Open", "CTRL+O")) {}

		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Edit"))
	{
		if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
		if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
		ImGui::Separator();
		if (ImGui::MenuItem("Cut", "CTRL+X")) {}
		if (ImGui::MenuItem("Copy", "CTRL+C")) {}
		if (ImGui::MenuItem("Paste", "CTRL+V")) {}
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Help"))
	{
		ImGui::MenuItem("Show demo window", nullptr, &showDemoWindow);
		
		ImGui::EndMenu();
	}

	ImGui::EndMainMenuBar();

	if (newTaskPressed)
	{
		StateManagerProxy::Get().SetMode(EApplicationMode::Modal);
	}

	if (showDemoWindow)
	{
		ImGui::ShowDemoWindow(&showDemoWindow);
	}

	ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

	for (Window* const w : _windows)
	{
		w->Draw();
	}
}

void Application::Minimize()
{
	WindowManagerProxy::Get().MinimizeWindow();
}

void Application::Exit()
{
	if (ApplicationProxy::IsValid()) 
	{
		ApplicationProxy::Get()._isOpen = false; 
	}
}

void Application::InitTheme_1()
{
	auto MakeAlpha = [](const ImVec4& color, float alpha) -> ImVec4 {
		return ImVec4{ color.x, color.y, color.z, alpha};
	};

	static const ImVec4 COLOR_GRAY_1  { 0.56f, 0.56f, 0.56f, 1.00f };
	static const ImVec4 COLOR_GRAY_2  { 0.50f, 0.50f, 0.50f, 1.00f };
	static const ImVec4 COLOR_GRAY_3  { 0.44f, 0.44f, 0.44f, 1.00f };
	static const ImVec4 COLOR_GRAY_4  { 0.40f, 0.40f, 0.40f, 1.00f };
	static const ImVec4 COLOR_GRAY_5  { 0.34f, 0.34f, 0.34f, 1.00f };
	static const ImVec4 COLOR_GRAY_6  { 0.28f, 0.28f, 0.28f, 1.00f };
	static const ImVec4 COLOR_GRAY_7  { 0.20f, 0.20f, 0.20f, 1.00f };
	static const ImVec4 COLOR_GRAY_8  { 0.19f, 0.19f, 0.19f, 1.00f };
	static const ImVec4 COLOR_GRAY_9  { 0.14f, 0.14f, 0.14f, 1.00f };
	static const ImVec4 COLOR_GRAY_10 { 0.10f, 0.10f, 0.10f, 1.00f };
	static const ImVec4 COLOR_GRAY_11 { 0.06f, 0.06f, 0.06f, 1.00f };
	static const ImVec4 COLOR_GRAY_12 { 0.05f, 0.05f, 0.05f, 1.00f };

	static const ImVec4 COLOR_GRAY_13 { 0.20f, 0.22f, 0.23f, 1.00f };
	static const ImVec4 COLOR_GRAY_14 { 0.40f, 0.44f, 0.47f, 1.00f };

	static const ImVec4 COLOR_WHITE			{ 1.00f, 1.00f, 1.00f, 1.00f };
	static const ImVec4 COLOR_BLACK			{ 0.00f, 0.00f, 0.00f, 1.00f };
	static const ImVec4 COLOR_TRANSPARENT   { 0.00f, 0.00f, 0.00f, 0.00f };

	static const ImVec4 COLOR_RED { 1.00f, 0.00f, 0.00f, 1.00f };
	static const ImVec4 COLOR_LIGHT_BLUE { 0.33f, 0.67f, 0.86f, 1.00f };

	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_Text] = MakeAlpha(COLOR_WHITE, 1.00f);
	colors[ImGuiCol_TextDisabled] = MakeAlpha(COLOR_GRAY_2, 1.00f);
	colors[ImGuiCol_WindowBg] = MakeAlpha(COLOR_GRAY_10, 1.00f);
	colors[ImGuiCol_ChildBg] = MakeAlpha(COLOR_TRANSPARENT, 0.00f);
	colors[ImGuiCol_PopupBg] = MakeAlpha(COLOR_GRAY_8, 0.92f);
	colors[ImGuiCol_Border] = MakeAlpha(COLOR_GRAY_8, 0.29f);
	colors[ImGuiCol_BorderShadow] = MakeAlpha(COLOR_GRAY_1, 0.24f);
	colors[ImGuiCol_FrameBg] = MakeAlpha(COLOR_GRAY_12, 0.54f);
	colors[ImGuiCol_FrameBgHovered] = MakeAlpha(COLOR_GRAY_8, 0.54f);
	colors[ImGuiCol_FrameBgActive] = MakeAlpha(COLOR_GRAY_13, 1.00f);
	colors[ImGuiCol_TitleBg] = MakeAlpha(COLOR_BLACK, 1.00f);
	colors[ImGuiCol_TitleBgActive] = MakeAlpha(COLOR_GRAY_11, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = MakeAlpha(COLOR_BLACK, 1.00f);
	colors[ImGuiCol_MenuBarBg] = MakeAlpha(COLOR_GRAY_9, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = MakeAlpha(COLOR_GRAY_12, 0.54f);
	colors[ImGuiCol_ScrollbarGrab] = MakeAlpha(COLOR_GRAY_5, 0.54f);
	colors[ImGuiCol_ScrollbarGrabHovered] = MakeAlpha(COLOR_GRAY_4, 0.54f);
	colors[ImGuiCol_ScrollbarGrabActive] = MakeAlpha(COLOR_GRAY_1, 0.54f);
	colors[ImGuiCol_CheckMark] = MakeAlpha(COLOR_LIGHT_BLUE, 1.00f);
	colors[ImGuiCol_SliderGrab] = MakeAlpha(COLOR_GRAY_5, 0.54f);
	colors[ImGuiCol_SliderGrabActive] = MakeAlpha(COLOR_GRAY_1, 0.54f);
	colors[ImGuiCol_Button] = MakeAlpha(COLOR_GRAY_12, 0.54f);
	colors[ImGuiCol_ButtonHovered] = MakeAlpha(COLOR_GRAY_8, 0.54f);
	colors[ImGuiCol_ButtonActive] = MakeAlpha(COLOR_GRAY_13, 1.00f);
	colors[ImGuiCol_Header] = MakeAlpha(COLOR_BLACK, 0.52f);
	colors[ImGuiCol_HeaderHovered] = MakeAlpha(COLOR_BLACK, 0.36f);
	colors[ImGuiCol_HeaderActive] = MakeAlpha(COLOR_GRAY_13, 0.33f);
	colors[ImGuiCol_Separator] = MakeAlpha(COLOR_GRAY_6, 0.29f);
	colors[ImGuiCol_SeparatorHovered] = MakeAlpha(COLOR_GRAY_3, 0.29f);
	colors[ImGuiCol_SeparatorActive] = MakeAlpha(COLOR_GRAY_14, 1.00f);
	colors[ImGuiCol_ResizeGrip] = MakeAlpha(COLOR_GRAY_6, 0.29f);
	colors[ImGuiCol_ResizeGripHovered] = MakeAlpha(COLOR_GRAY_3, 0.29f);
	colors[ImGuiCol_ResizeGripActive] = MakeAlpha(COLOR_GRAY_14, 1.00f);
	colors[ImGuiCol_Tab] = MakeAlpha(COLOR_BLACK, 0.52f);
	colors[ImGuiCol_TabHovered] = MakeAlpha(COLOR_GRAY_9, 1.00f);
	colors[ImGuiCol_TabActive] = MakeAlpha(COLOR_GRAY_7, 0.36f);
	colors[ImGuiCol_TabUnfocused] = MakeAlpha(COLOR_BLACK, 0.52f);
	colors[ImGuiCol_TabUnfocusedActive] = MakeAlpha(COLOR_GRAY_9, 1.00f);
	colors[ImGuiCol_DockingPreview] = MakeAlpha(COLOR_LIGHT_BLUE, 1.00f);
	colors[ImGuiCol_DockingEmptyBg] = MakeAlpha(COLOR_BLACK, 1.00f);
	colors[ImGuiCol_PlotLines] = MakeAlpha(COLOR_RED, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = MakeAlpha(COLOR_RED, 1.00f);
	colors[ImGuiCol_PlotHistogram] = MakeAlpha(COLOR_RED, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = MakeAlpha(COLOR_RED, 1.00f);
	colors[ImGuiCol_TableHeaderBg] = MakeAlpha(COLOR_BLACK, 0.52f);
	colors[ImGuiCol_TableBorderStrong] = MakeAlpha(COLOR_GRAY_1, 0.52f);
	colors[ImGuiCol_TableBorderLight] = MakeAlpha(COLOR_GRAY_6, 0.29f);
	colors[ImGuiCol_TableRowBg] = MakeAlpha(COLOR_TRANSPARENT, 0.00f);
	colors[ImGuiCol_TableRowBgAlt] = MakeAlpha(COLOR_WHITE, 0.06f);
	colors[ImGuiCol_TextSelectedBg] = MakeAlpha(COLOR_GRAY_13, 1.00f);
	colors[ImGuiCol_DragDropTarget] = MakeAlpha(COLOR_LIGHT_BLUE, 1.00f);
	colors[ImGuiCol_NavHighlight] = MakeAlpha(COLOR_LIGHT_BLUE, 0.50f);
	colors[ImGuiCol_NavWindowingHighlight] = MakeAlpha(COLOR_RED, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg] = MakeAlpha(COLOR_RED, 0.20f);
	colors[ImGuiCol_ModalWindowDimBg] = MakeAlpha(COLOR_LIGHT_BLUE, 0.35f);

	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowPadding = ImVec2(8.00f, 8.00f);
	style.FramePadding = ImVec2(5.00f, 6.00f);
	style.CellPadding = ImVec2(6.00f, 12.00f);
	style.ItemSpacing = ImVec2(6.00f, 6.00f);
	style.ItemInnerSpacing = ImVec2(6.00f, 6.00f);
	style.TouchExtraPadding = ImVec2(0.00f, 0.00f);
	style.IndentSpacing = 25;
	style.ScrollbarSize = 15;
	style.GrabMinSize = 10;
	style.WindowBorderSize = 1;
	style.ChildBorderSize = 1;
	style.PopupBorderSize = 1;
	style.FrameBorderSize = 1;
	style.TabBorderSize = 1;
	style.WindowRounding = 7;
	style.ChildRounding = 4;
	style.FrameRounding = 3;
	style.PopupRounding = 4;
	style.ScrollbarRounding = 9;
	style.GrabRounding = 3;
	style.LogSliderDeadzone = 4;
	style.TabRounding = 4;
}


void Application::InitTheme_2()
{
	// CURRENT THEME

	// ptodo - Find colors
	ImGuiStyle& style = ImGui::GetStyle();

	style.Colors[ImGuiCol_Text] = ImVec4(0.90f, 0.90f, 0.90f, 0.90f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.09f, 0.09f, 0.15f, 1.00f);
	//style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.05f, 0.05f, 0.10f, 0.85f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.70f, 0.70f, 0.70f, 0.65f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.00f, 0.00f, 0.01f, 1.00f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.90f, 0.80f, 0.80f, 0.40f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.90f, 0.65f, 0.65f, 0.45f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.83f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.40f, 0.40f, 0.80f, 0.20f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.00f, 0.00f, 0.00f, 0.87f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.01f, 0.01f, 0.02f, 0.80f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.20f, 0.25f, 0.30f, 0.60f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.55f, 0.53f, 0.55f, 0.51f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.56f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.91f);
	//style.Colors[ImGuiCol_ComboBg] = ImVec4(0.1f, 0.1f, 0.1f, 0.99f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.90f, 0.90f, 0.90f, 0.83f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.70f, 0.70f, 0.70f, 0.62f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.30f, 0.30f, 0.30f, 0.84f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.48f, 0.72f, 0.89f, 0.49f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.50f, 0.69f, 0.99f, 0.68f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.80f, 0.50f, 0.50f, 1.00f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.30f, 0.69f, 1.00f, 0.53f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.44f, 0.61f, 0.86f, 1.00f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.38f, 0.62f, 0.83f, 1.00f);
	//style.Colors[ImGuiCol_Column] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	//style.Colors[ImGuiCol_ColumnHovered] = ImVec4(0.70f, 0.60f, 0.60f, 1.00f);
	//style.Colors[ImGuiCol_ColumnActive] = ImVec4(0.90f, 0.70f, 0.70f, 1.00f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.85f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.60f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.90f);
	//style.Colors[ImGuiCol_CloseButton] = ImVec4(0.50f, 0.50f, 0.90f, 0.50f);
	//style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.70f, 0.70f, 0.90f, 0.60f);
	//style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.70f, 0.70f, 0.70f, 1.00f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.00f, 0.00f, 1.00f, 0.35f);
	//style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);

	style.WindowRounding = 5.3f;
	style.FrameRounding = 2.3f;
	style.ScrollbarRounding = 0;

	style.WindowPadding = ImVec2(8.00f, 8.00f);
	style.FramePadding = ImVec2(5.00f, 6.00f);
	style.CellPadding = ImVec2(6.00f, 12.00f);
	style.ItemSpacing = ImVec2(6.00f, 6.00f);
	style.ItemInnerSpacing = ImVec2(6.00f, 6.00f);
	style.TouchExtraPadding = ImVec2(0.00f, 0.00f);
	style.IndentSpacing = 25;
	style.ScrollbarSize = 15;
	style.GrabMinSize = 10;
	style.WindowBorderSize = 1;
	style.ChildBorderSize = 1;
	style.PopupBorderSize = 1;
	style.FrameBorderSize = 1;
	style.TabBorderSize = 1;
	style.WindowRounding = 7;
	style.ChildRounding = 4;
	//style.FrameRounding = 3;
	style.PopupRounding = 4;
	style.ScrollbarRounding = 9;
	style.GrabRounding = 3;
	style.LogSliderDeadzone = 4;
	style.TabRounding = 4;
}

void Application::LoadFonts()
{
	// ptodo - don't hard-code this here.
	ImGuiIO& io = ImGui::GetIO();

	static const char * const ROOT = ".\\res\\fonts";

	static const std::vector<std::pair<const char*, float>> FONTS = {
		{"Roboto-Medium.ttf", 17.f},

		{"Cousine-Regular.ttf", 12.f},
		{"DroidSans.ttf", 12.f},
		{"Karla-Regular.ttf", 12.f},
		{"ProggyClean.ttf", 12.f},
		{"ProggyTiny.ttf", 12.f},
		{"Tahoma.ttf", 17.f}
	};

	ImFontConfig config;
	config.OversampleH = 4;
	config.OversampleV = 4;
	//config.RasterizerDensity = 4.f;

	for (const std::pair<const char* /*path*/, float /*size*/> pair : FONTS)
	{
		const std::string path = std::format("{}\\{}", ROOT, pair.first); 
		io.Fonts->AddFontFromFileTTF(path.c_str(), pair.second, &config);
	}

	io.Fonts->Build();
}

ApplicationProxy::ApplicationProxy() = default;