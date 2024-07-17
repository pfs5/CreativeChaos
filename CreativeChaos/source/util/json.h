#pragma once
#include "external/nlohmann_json.hpp"

namespace json
{
	template <typename T>
	bool TryGet(const nlohmann::json& jsonObj, const char* key, T& outValue)
	{
		try 
		{
			jsonObj.at(key).get_to(outValue);
			return true;
		}
		catch (...)
		{
			
		}

		return false;
	}
}