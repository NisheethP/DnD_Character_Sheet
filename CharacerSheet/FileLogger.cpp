#include "PreCompiledHeader.h"
#include "FileLogger.h"

FileLogger::FileLogger(std::string pFile, std::string pFolder, bool pLogTime)
	:
	fileName(pFile),
	Folder(pFolder),
	logTime(pLogTime),
	file(new wxTextFile(getPath()))
{
	if (!wxDirExists(Folder))
		wxMkDir(Folder);
}

FileLogger::FileLogger():
	fileName(""),
	file(nullptr),
	Folder(""),
	logTime(true)
{}

void FileLogger::Log(std::string str)
{
	std::string log = "";
	if (logTime)
		log += getCurTime();

	log += " - ";
	log += str;

	file->AddLine(log);
}

void FileLogger::LogWithoutTime(std::string str)
{
	file->AddLine(str);
}

void FileLogger::operator<<(std::string str)
{
	std::string log = "";
	if (logTime)
		log += getCurTime() + " - ";

	log += str;

	file->AddLine(log);
	file->Write();
}

void FileLogger::operator<<(int val)
{
	std::string log = "";
	if (logTime)
		log += getCurTime() + " - ";

	log += std::to_string(val);

	file->AddLine(log);
	file->Write();
}

void FileLogger::Clear()
{
	file->Clear();
	file->Write();
}

void FileLogger::OpenFile()
{
	if (file == nullptr)
		file = new wxTextFile(getPath());

	auto path = getPath();
	bool res = file->Create();
	file->Open();
}

void FileLogger::CloseFile()
{
	file->Close();
}

const bool FileLogger::isOpen() const
{
	return file->IsOpened();
}

const std::string FileLogger::getPath() const
{
	return Folder + "\\" + fileName;
}

const std::string FileLogger::getCurTime() const
{
	using namespace std;

	
	auto const time = std::chrono::current_zone()->to_local(std::chrono::system_clock::now());

	string timeStr = std::format("{:%T}", time);

	return timeStr;
}
