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

	const bool emptySave() const { return empty; } // If there is no save data
	const nlohmann::json& getData() const { return data; }
	
private:
	nlohmann::json data;
	bool empty;
};