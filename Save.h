#pragma once
#include <string>
class Save
{
private:
	std::wstring path;
	std::wstring name;
	double time;
public:
	bool SetPathAndName(std::wstring);
	bool SetLWTime();
	double GetLWTime() const;
	std::wstring GetPath() const;
	std::wstring GetName() const;
};