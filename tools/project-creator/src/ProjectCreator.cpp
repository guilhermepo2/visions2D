// ========================================================================
// ========================================================================
//
//
// 
// ========================================================================
// ========================================================================

#include "Log.h"
#include "Utilities/JsonHelper.h"
#include <fstream>

enum class PROJECTTYPE {
	NONE,
	TOOL,
	GAME
};

const static std::string Dependencies[] = {
	"%{prj.name}/src",
	"visions2D/src",
	"%{IncludeDirectories.SDL2}",
	"%{IncludeDirectories.SDL2_TTF}",
	"%{IncludeDirectories.GLAD}",
	"%{IncludeDirectories.spdlog}",
	"%{IncludeDirectories.DEARIMGUI}",
	"%{IncludeDirectories.STBIMAGE}",
	"%{IncludeDirectories.GLM}",
	"%{IncludeDirectories.RAPIDJSON}"
};

// ************************************************************************
// ************************************************************************
PROJECTTYPE GetProjectType(const std::string& ProjectType) {
	if (ProjectType == "tool") return PROJECTTYPE::TOOL;
	if (ProjectType == "game") return PROJECTTYPE::GAME;

	LOG_ERROR("Invalid Project type: {0}", ProjectType);
	return PROJECTTYPE::NONE;
}

// ************************************************************************
// ************************************************************************
// TODO: This works, but there's probably a better way to do it.
// 1. Have a template?
void CreateGameFileProject(const std::string& Name, const std::string& Location) {
	LOG_INFO("Creating GAME file");

	// 1. create the file
	std::string Filename = std::string("premake_") + Name + std::string(".lua");
	std::ofstream File(Filename.c_str());
	if (!File.is_open()) {
		LOG_ERROR("couldn't create {0} file", Filename);
	}

	// ==================================================
	// doing the dirt work
	File << "project \"" + Name + "\"\n";
	File << "\tlocation \"" + Location + "\"\n";
	File << "\tkind \"ConsoleApp\"\n";
	File << "\tlanguage \"C++\"\n";
	File << "\twarnings \"Extra\"\n";
	File << "\n";
	File << "\ttargetdir(\"bin/\" ..outputdir .. \"/%{prj.name}\")\n";
	File << "\tobjdir(\"binobj/\" ..outputdir .. \"/%{prj.name}\")\n";
	File << "\n";
	File << "\tfiles\n\t{\n";
	File << "\t\t\"%{prj.location}/**.h\",\n";
	File << "\t\t\"%{prj.location}/**.cpp\"\n";
	File << "\t}";
	File << "\n\n";
	
	File << "\tincludedirs\n\t{\n";
	for(auto dependency : Dependencies) {
		File << "\t\t\"" + dependency + "\"\n";
	}
	File << "\t}\n\n";

	File << "\tfilter \"system:windows\"\n";
	File << "\t\tcppdialect \"C++17\"\n";
	File << "\t\tstaticruntime \"On\"\n";
	File << "\t\tsystemversion \"latest\"\n";
	File << "\n";
	File << "\t\tdefines\n\t\t{\n";
	File << "\t\t\t\"visions2D_DEBUG\",\n";
	File << "\t\t\t\"sandbox_DEBUG\"\n";
	File << "\t\t}\n";

	// ==================================================
	// 2. Open "premake5.lua"
	/*
	std::ifstream PremakeFile("../../../premake5.lua");
	if (!PremakeFile.is_open()) {
		LOG_ERROR("Couldn't open premake5.lua file");
	}
	*/

	// TODO
	// fill contents
	// add it to "premake5.lua"
}

// ************************************************************************
// ************************************************************************
void CreateToolFileProject(const std::string& Name, const std::string& Location) {
	LOG_INFO("Creating TOOL file");
	
	// at this point in time there is no difference between creating a tool and a game lol
	CreateGameFileProject(Name, Location);
}

// ************************************************************************
// ************************************************************************
void CreateWithSettings(const std::string& ProjectName, const std::string& Location, PROJECTTYPE ProjectType) {
	LOG_INFO("Creating ({0}, {1}, {2})", ProjectName, Location, ProjectType);

	switch (ProjectType) {
	case PROJECTTYPE::TOOL: {
		CreateToolFileProject(ProjectName, Location);
	} break;
	case PROJECTTYPE::GAME: {
		CreateGameFileProject(ProjectName, Location);
	} break;
	default:
		LOG_ERROR("INVALID PROJECT TYPE!");
		break;
	}
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
	// TODO: actually have a name?
	LOG_INFO("project creator v0.1");
	LOG_INFO("arg count: {0}", argc);
	for (int i = 0; i < argc; i++) {
		LOG_INFO("arg {0}: {1}", i, argv[i]);
	}

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