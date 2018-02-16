#include "File.h"
#include <string>
#include <windows.h>
#include <QDebug>
#include <QString>

File::File(){}

File::File(std::wstring path)
{
	SetPathAndName(path);
}

bool File::SetLWTime()
{
	HANDLE hFile = CreateFile(path.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL,
		OPEN_ALWAYS, 0, NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
        qInfo() << "CreateFile failed with " << GetLastError();
		return 0;
	}

	FILETIME ftCreate, ftAccess, ftWrite;
    SYSTEMTIME stUTC;
	double dblt;

	// Retrieve the file times for the file.
	if (!GetFileTime(hFile, &ftCreate, &ftAccess, &ftWrite))
		return 0;

	// Convert the last-write time to local time.
	FileTimeToSystemTime(&ftWrite, &stUTC);
	SystemTimeToVariantTime(&stUTC, &dblt);

	CloseHandle(hFile);
	time = dblt;

	return 1;
}

double File::GetLWTime() const
{
	return time;
}

bool File::SetPathAndName(std::wstring dir)
{
	path = dir;
	std::size_t i = dir.find_last_of(L"/\\");
	if (i >= 0)
	{
		name = dir.substr(i + 1); // Name
		return 1;
	}
	else
	{
        qInfo() << "Your path is probably broken";
		return 0;
	}
}

std::wstring File::GetPath() const
{
	return path;
}

std::wstring File::GetName() const
{
	return name;
}

// Creating BackUpFolder only if not already exists
bool File::CreateBackUpsFolder() const
{
	bool flag = CreateDirectory(BackUpsFolder.c_str(), NULL);
	if (!flag)
	{
		if (GetLastError() == ERROR_ALREADY_EXISTS)
		{
			// it's okay
            qInfo() << "You already have a BackUps folder";
			return 1;
		}
		else
		{
			// smth went wrong, we need a valid "path"
            qInfo() << "Looks like your path is broken. We need a new one";
			return 0;
		}
	}
    qInfo() << "BackUps folder was successfully created";
	return 1;
}

// Creating a backup
bool File::CreateBackUp() const
{
	std::wstring name = GetName();
	std::wstring path = GetPath();
	std::wstring NewName = BackUpsFolder + L"\\" + name + std::to_wstring(GetLWTime());

	if (CopyFile(path.c_str(), NewName.c_str(), 0))
	{
        qInfo() << "New backup has been successfully created";
		return 1;
	}
	else
	{
        qInfo() << "Creating a new backup has failed";
		return 0;
	}
}

// Updating files
bool File::UpdateFile()
{
	std::wstring name = GetName();
	std::wstring NewName = CloudFolder + L"\\" + name;
	File CloudSave(NewName);
	CloudSave.SetLWTime();
	SetLWTime();
	if (GetLWTime() == CloudSave.GetLWTime())
	{
        qInfo() << "No need to update " << GetName();
        return 0;
	}
	else
	{	
		if (CreateBackUp())
		{
			if (GetLWTime() > CloudSave.GetLWTime())
			{
				if (CopyFile(GetPath().c_str(), CloudSave.GetPath().c_str(), 0))
				{
                    qInfo() << "File has been successfully updated (stored to Cloud Folder)";
					return 1;
				}
				else
				{
                    qInfo() << "Updating a file has failed";
					return 0;
				}
			}
			else if (GetLWTime() < CloudSave.GetLWTime())
			{
				if (CopyFile(CloudSave.GetPath().c_str(), GetPath().c_str(), 0))
				{
                    qInfo() << "File has been successfully updated (stored from Cloud Folder)";
					return 1;
				}
				else
				{
                    qInfo() << "Updating a file has failed";
					return 0;
				}
			}
		}
        else
        {
            return 0;
        }
	}
}

bool File::SetCloudFolder(std::wstring path)
{
	std::size_t i = path.find_last_of(L"/\\");
	if (i >= 0)
	{
		CloudFolder = path;
		BackUpsFolder = path + L"\\BackUps";
		CreateBackUpsFolder();
		return 1;
	}
	else
	{
        qInfo() << "Your path is broken";
		return 0;
	}
}

bool File::SetBackUpsFolder(std::wstring path)
{
	std::size_t i = path.find_last_of(L"/\\");
	if (i >= 0)
	{
		BackUpsFolder = path;
		return 1;
	}
	else
	{
        qInfo() << "Your path is broken";
		return 0;
	}
}

void File::SetSynch(bool arg)
{
    bSynch = arg;
}

bool File::isSynched()
{
    return bSynch;
}

QDataStream &operator <<(QDataStream& out, const File& file)
{
//    out << file.path;
//    out << file.name;
//    out << file.BackUpsFolder;
//    out << file.CloudFolder;
//    out << file.time;
    return out;
}
