#pragma once
#include <filesystem>

/*
What do I need in a file system?
- static and accessible everywhere
- Have multiple working paths?
	- visions2D::FileSystem::EnginePath("DefaultAssets/Shaders/DefaultSprite.vert")
	- visions2D::Filesystem::RootPath("sandbox/assets/bla.png")
	- visions2D::FileSystem::CurrentProjectPath("assets/bla.png")
- Question: How do I handle all these when releasing a project?
	- maybe I can just change the root path?

*/

namespace visions2D {
	
	class FileSystem {
	public:
		static void Init();
		static void Deinit();

		static std::string GetFromRootPath(const std::string& Path);
		static std::string GetFromEnginePath(const std::string& Path);

		static std::filesystem::path RootPath;
		static std::filesystem::path EnginePath;
	};
}