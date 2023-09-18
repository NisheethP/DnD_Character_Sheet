#pragma once

class FileLogger
{
	std::string fileName ="";
	wxTextFile* file;
	std::string Folder = "Health Logs";
	bool logTime = true;

public:
	FileLogger(std::string pFile, std::string pFolder, bool pLogTime = true);
	FileLogger();

	void Log(std::string str);
	void LogWithoutTime(std::string str);
	void operator<< (std::string str);
	void operator<< (int val);

	void Clear();
	void OpenFile();
	void CloseFile();
	
	void setFileName(std::string pFile) { fileName = pFile; }
	void setFolder(std::string pFolder) { Folder = pFolder; }
	void setPath(std::string pFile, std::string pFolder) 
	{
		fileName = pFile;
		Folder = pFolder;
	}

	const bool isOpen() const;
	const std::string getPath() const;
	const std::string getCurTime() const;
	const std::string& getFileName() const { return fileName; }
	const std::string& getFolder() const { return Folder; }
};

