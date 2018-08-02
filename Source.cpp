#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
#include <windows.h>
#include <dirent.h>
#include <stdio.h>

bool copyFile(std::string initialPath, std::string targetPath, std::string fileName)
{
	std::ifstream source(initialPath + fileName, std::ios::binary);
	std::ofstream dest(targetPath + fileName, std::ios::binary);

	dest << source.rdbuf();

	source.close();
	dest.close();

	return false;
}

std::vector<std::string> dirlist(std::string path)
{
	char c_path[1024]; //We assume the path cannot be longer than 1024 characters
	struct dirent* ent;
	DIR* dir;
	std::vector<std::string> fileList;

	strcpy_s(c_path, path.c_str());

	if ((dir = opendir(c_path)) != NULL)
	{
		while ((ent = readdir(dir)) != NULL)
		{
			if (ent->d_type == 16384) //if the file is a folder, do nothing
				continue;
			fileList.push_back(ent->d_name);
		}
		closedir(dir);
	}
	return fileList;
}

void removeFile(std::string filePath)
{
	if (remove(filePath.c_str()))
		std::cout << "Error deleting file" << std::endl;
	return;
}

int main(int argc, char** argv[])
{
	std::cout << argc << std::endl;

	if (argc != 3)
	{
		std::cout << "Not enough arguments" << std::endl;
		return 0;
	}

	int fileNo = 0;
	std::string initialPath((std::string)(char*) argv[1]);
	std::string targetPath((std::string)(char*) argv[2]);

	if (!(initialPath.substr(initialPath.length() - 1, 1) == "\\"))
		initialPath += "\\";
	if (!(targetPath.substr(targetPath.length() - 1, 1) == "\\"))
		targetPath += "\\";

	std::cout << "From: " << initialPath << std::endl << "To: " << targetPath << std::endl;

	std::vector<std::string> fileList = dirlist(initialPath);

	while (!fileList.empty())
	{
		std::string fileName = fileList.back();
		fileList.pop_back();

		copyFile(initialPath, targetPath, fileName);
		removeFile(initialPath + fileName);
		fileNo++;
	}
	std::cout << "All " << fileNo <<  " Done." << std::endl;

	system("PAUSE");

	return 0;
}
