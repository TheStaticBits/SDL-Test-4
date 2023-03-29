#pragma once

#include <iostream>
#include <nlohmann/json.hpp>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

class Save
{
public:
	Save(const nlohmann::json& constants);

	void load(const nlohmann::json& constants);
	void save(const nlohmann::json& constants);

	inline const bool emptySave() const { return empty; } // If there is no save data
	inline nlohmann::json& getData() { return data; }
	
private:
	nlohmann::json data;
	bool empty;
};