// ========================================================================
// ========================================================================
//
//
// 
// ========================================================================
// ========================================================================

#include "Log.h"
#include "Utilities/JsonHelper.h"

enum class PROJECTTYPE {
	NONE,
	TOOL,
	GAME
};

// ************************************************************************
// ************************************************************************
PROJECTTYPE GetProjectType(const std::string& ProjectType) {
	if (ProjectType == "tool") return PROJECTTYPE::TOOL;
	if (ProjectType == "game") return PROJECTTYPE::GAME;

	return PROJECTTYPE::NONE;
}

// ************************************************************************
// ************************************************************************
void CreateToolFileProject(const std::string& Name, const std::string& Location) {
	LOG_INFO("Creating TOOL file");
}

// ************************************************************************
// ************************************************************************
void CreateGameFileProject(const std::string& Name, const std::string& Location) {
	LOG_INFO("Creating GAME file");

	// create file (premake_{Name}.lua)
	// fill contents
	// add it to "premake5.lua"
}

// ************************************************************************
// ************************************************************************
void CreateWithSettings(const std::string& ProjectName, const std::string& Location, PROJECTTYPE ProjectType) {
	LOG_INFO("Creating ({0}, {1}, {2})", ProjectName, Location, ProjectType);
}

// ************************************************************************
// ************************************************************************
void CreateFromFilename(const std::string& Filename) {
	rapidjson::Document doc;

	if (!visions2D::JsonHelper::LoadJSON(Filename, doc)) {
		LOG_ERROR("couldn't load filename: {0}", Filename);
		return;
	}

	std::string ProjectName;
	std::string ProjectLocation;
	std::string ProjectType;

	visions2D::JsonHelper::GetString(doc, "projectName", ProjectName);
	visions2D::JsonHelper::GetString(doc, "projectLocation", ProjectLocation);
	visions2D::JsonHelper::GetString(doc, "projectType", ProjectType);

	CreateWithSettings(ProjectName, ProjectLocation, GetProjectType(ProjectType));
}

// ************************************************************************
// ************************************************************************
int main(int argc, char* argv[]) {
	Log::Initialize();
	LOG_INFO("project creator v0.1");
	LOG_INFO("arg count: {0}", argc);
	for (int i = 0; i < argc; i++) {
		LOG_INFO("arg {0}: {1}", i, argv[i]);
	}

	// what is needed to create a project?
	// project name
	// location name
	// tools or game
	// that's it?
	if (argc == 2) {
		CreateFromFilename(argv[1]);
	}
	else if (argc == 4) {
		PROJECTTYPE prj = GetProjectType(argv[3]);
		CreateWithSettings(argv[1], argv[2], prj);
	}
	else {
		LOG_WARNING("Please specify a filename or project name, location and type.");
	}

	return 0;
}