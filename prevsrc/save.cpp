#include "save.h"

#include <iostream>

#ifdef __EMSCRIPTEN__

#include <emscripten.h>
// Test if the loadSave function is defined (if the program is being run from website)
EM_JS(bool, loadSaveDefined, (), {
	return (typeof loadSave != "undefined");
});

EM_JS(bool, saveDefined, (), {
	return (typeof save != "undefined");
});

// Test if the user is logged in
EM_JS(bool, userAuthenticated, (), {
	if (typeof isAuthenticated != "undefined") {
		return isAuthenticated();
	} else {
		return false;
	}
});


// Calling JS load function to get the save data from Django
EM_ASYNC_JS(char*, getSave, (), {
	let result = "failed";

	try {
		result = await loadSave();
	} catch (error) {
		console.log("Error retrieving save: ");
		console.error(error);
	}

	return allocateUTF8(result);
});

EM_ASYNC_JS(bool, saveToServer, (const char* saveData), {
	try {
		result = await save(UTF8ToString(saveData));
	} catch (error) {
		console.log("Error saving to server: ");
		console.error(error);
		result = "failed";
	}

	return result == "success";
});


// Cookies
EM_JS(bool, hasCookie, (const char* cookieName), {
	return window.localStorage.getItem(UTF8ToString(cookieName)) != null;
});

EM_JS(void, setCookie, (const char* cookieName, const char* saveData), {
	window.localStorage.setItem(UTF8ToString(cookieName), UTF8ToString(saveData));
	console.log("Saved: " + UTF8ToString(cookieName) + " - " + UTF8ToString(saveData));
})

EM_JS(char*, retrieveCookie, (const char* cookieName), {
	return allocateUTF8(window.localStorage.getItem(UTF8ToString(cookieName)));
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
	
	if (loadSaveDefined() && userAuthenticated())
	{
		std::cout << "C++: Retrieving save from server" << std::endl;
		save = std::string(getSave());
		emscripten_sleep(0); // Handle promise "unwind" exception
	}

	if (save == "failed")
	{
		std::string cookieName = constants["save"]["cookieName"].get<std::string>();

		std::cout << "C++: Looking for cookie " << cookieName << std::endl;
		if (hasCookie(cookieName.c_str()))
		{
			std::cout << "C++: Found cookie! " << retrieveCookie(cookieName.c_str()) << std::endl;
			save = retrieveCookie(cookieName.c_str());
		}
		else
		{
			std::cout << "C++: No " << cookieName << " found" << std::endl;
			empty = true;
		}
	}

	if (!empty)
		data = nlohmann::json::parse(save);
#else
	std::ifstream file(constants["save"]["path"].get<std::string>());

	if (!file.is_open())
		empty = true;
	else
	{
		data = nlohmann::json::parse(file);
		file.close();
	}
#endif

	if (empty)
		data = nlohmann::json({});// Creates an empty JSON
}

void Save::save(const nlohmann::json& constants)
{
	const std::string strJson = data.dump(); // Turn JSON into string

#ifdef __EMSCRIPTEN__
	bool savedStatus = false;

	std::cout << strJson.c_str() << std::endl;

	if (saveDefined() && userAuthenticated())
	{
		std::cout << "C++: Saving to server" << std::endl;
		savedStatus = saveToServer(strJson.c_str());
	} 
	
	if (!savedStatus)
	{
		std::cout << "C++: Saving to cookies" << std::endl;
		setCookie(constants["save"]["cookieName"].get<std::string>().c_str(), strJson.c_str());
	}
#else
	std::string filePath = constants["save"]["path"].get<std::string>();
	std::ofstream file(filePath);

	if (file.is_open())
	{
		file << strJson;
		file.close();
	}
	else
		std::cout << "Unable to output save data to the file at " << filePath << std::endl;
#endif
}