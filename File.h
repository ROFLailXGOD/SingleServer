#pragma once
#include <string>
#include <QMetaType>
#include <QDataStream>
class File
{
private:
	std::wstring path;
	std::wstring name;
	std::wstring BackUpsFolder; // BackUps folder Path
	std::wstring CloudFolder; // Cloud Folder Path

    bool bSynch;

	double time;

	bool CreateBackUpsFolder() const; // Creating BackUpsFolder only if not already exists
	bool CreateBackUp() const; // Creating a backup
public:
	File();
	File(std::wstring);

	bool SetPathAndName(std::wstring);
	bool SetLWTime();
	bool SetCloudFolder(std::wstring);
	bool SetBackUpsFolder(std::wstring);

	double GetLWTime() const;
	std::wstring GetPath() const;
	std::wstring GetName() const;

	bool UpdateFile(); // Updating files

    bool isSynched(); // Is file synched?
    void SetSynch(bool);
};
Q_DECLARE_METATYPE(File*)

QDataStream &operator<<(QDataStream&, const File&);
QDataStream &operator>>(QDataStream&, File&);
