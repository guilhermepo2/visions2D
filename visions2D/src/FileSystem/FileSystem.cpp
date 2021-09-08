#include "FileSystem.h"
#include "Log.h"

namespace visions2D {

	std::filesystem::path FileSystem::RootPath;
	std::filesystem::path FileSystem::EnginePath;

	void FileSystem::Init() {
		std::filesystem::path WorkingPath(std::filesystem::current_path());

#ifdef VISIONS2D_RELEASE
		// TODO: SET PROPER PATHS ON RELEASE MODE
#else
		// TODO: This relies on the root folder being called "visions2D" - also, this might break when working in the engine
		// Option 1 to fix this: have a config.json file on the root folder and just search for it
		while (WorkingPath.stem() != "visions2D") {
			WorkingPath = WorkingPath.parent_path();
		}

		// edge-case, if we are working on the engine, stem and parent stem are "visions2D" - adding a workaround for that
		if (WorkingPath.parent_path().stem() == "visions2D") {
			WorkingPath = WorkingPath.parent_path();
		}
		
		RootPath = WorkingPath;
		EnginePath = RootPath/"visions2D";
#endif

		LOG_INFO("[visions2D::fs] FileSystem Initialized");
		LOG_INFO("[visions2D::fs] Root Path: {0}", RootPath);
		LOG_INFO("[visions2D::fs] Engine path: {0}", EnginePath);
	}

	std::string FileSystem::GetFromRootPath(const std::string& Path) {
		std::filesystem::path PathToAdd(Path);
		std::filesystem::path PathToFind = RootPath/PathToAdd;
		return PathToFind.string();
	}

	std::string FileSystem::GetFromEnginePath(const std::string& Path) {
		std::filesystem::path NewPath = EnginePath/Path;
		return NewPath.string();
	}

	void FileSystem::Deinit() {

	}
}