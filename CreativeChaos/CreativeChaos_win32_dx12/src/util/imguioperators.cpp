#include "pch.h"
#include "util/imguioperators.h"

#include "imgui.h"

ImVec2 operator+(const ImVec2 lhs, const ImVec2 rhs)
{
	return ImVec2{ lhs.x + rhs.x, lhs.y + rhs.y };
}

ImVec2 operator-(const ImVec2 lhs, const ImVec2 rhs)
{
	return ImVec2{ lhs.x - rhs.x, lhs.y - rhs.y };
}

ImVec2 operator*(const ImVec2 vec, const float scalar)
{
	return ImVec2{ vec.x * scalar, vec.y * scalar };
}

ImVec2 operator/(const ImVec2 vec, const float scalar)
{
	return ImVec2{ vec.x / scalar, vec.y / scalar };
}
