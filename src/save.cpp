#include "save.h"

#include <iostream>

#ifdef __EMSCRIPTEN__

#include <emscripten.h>
// Test if the loadSave function is defined (if the program is being run from website)
EM_JS(bool, loadSaveDefined, (), {
	return (typeof loadSave != "undefined");
});

// Calling JS load function to get the save data from Django
EM_ASYNC_JS(char*, getSave, (), {
	let result = "failed";

	try {
		result = await loadSave();
	} catch (error) {
		console.error(error);
	}

	return allocateUTF8(result);
});

#else
#include <fstream>
#endif

Save::Save(const nlohmann::json& constants)
	: empty(false)
{
	load(constants);
}

void Save::load(const nlohmann::json& constants)
{
#ifdef __EMSCRIPTEN__
	std::string save = "failed";
	
	if (loadSaveDefined())
	{
		save = std::string(getSave());
		emscripten_sleep(0); // Handle promise "unwind" exception
	}

	if (save == "failed")
		empty = true;
	else
		this->data = nlohmann::json::parse(save);
#else
	const std::string savePath = constants["save"]["path"].get<std::string>();
	std::ifstream file(savePath);

	if (!file.is_open())
		empty = true;
	else
		this->data = nlohmann::json::parse(file);
#endif

	if (empty)
		this->data = nlohmann::json::array(); // Creates an empty JSON
}

void Save::save(const nlohmann::json& constants)
{
	
}