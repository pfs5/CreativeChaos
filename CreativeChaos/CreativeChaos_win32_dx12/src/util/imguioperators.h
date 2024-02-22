#pragma once

struct ImVec2;

ImVec2 operator+(const ImVec2 lhs, const ImVec2 rhs);
ImVec2 operator-(const ImVec2 lhs, const ImVec2 rhs);

ImVec2 operator*(const ImVec2 vec, const float scalar);
ImVec2 operator/(const ImVec2 vec, const float scalar);