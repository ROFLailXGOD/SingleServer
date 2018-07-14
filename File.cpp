#include "File.h"
#include <windows.h>
#include <QDebug>

File::File(){}

File::File(QString path)
{
	SetPathAndName(path);
    bSynch = 0;
}


bool File::SetLWTime()
{
    HANDLE hFile = CreateFile(path.toStdWString().c_str(), GENERIC_READ, FILE_SHARE_READ, NULL,
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

bool File::SetPathAndName(QString dir)
{
    path = dir;
    int i = dir.lastIndexOf("/");
	if (i >= 0)
	{
        name = dir.right(dir.size()-i-1); // Name
		return 1;
	}
	else
	{
        qInfo() << "Your path is probably broken";
		return 0;
	}
}

QString File::GetPath() const
{
	return path;
}

QString File::GetName() const
{
	return name;
}

QString File::GetBUFolder() const
{
    return BackUpsFolder;
}

QString File::GetCFolder() const
{
    return CloudFolder;
}

QString File::GetCategory() const
{
    return Category;
}

double File::GetLWTime() const
{
    return time;
}

// Creating BackUpFolder only if not already exists
bool File::CreateBackUpsFolder() const
{
    bool flag = CreateDirectory(BackUpsFolder.toStdWString().c_str(), NULL);
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
    QString name = GetName();
    QString path = GetPath();
    QString NewName = BackUpsFolder + "/" + name + QString::number(time);

    if (CopyFile(path.toStdWString().c_str(), NewName.toStdWString().c_str(), 0))
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
    QString name = GetName();
    QString NewName = CloudFolder + "/" + name;
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
                if (CopyFile(GetPath().toStdWString().c_str(), CloudSave.GetPath().toStdWString().c_str(), 0))
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
                if (CopyFile(CloudSave.GetPath().toStdWString().c_str(), GetPath().toStdWString().c_str(), 0))
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

bool File::SetCloudFolder(QString path)
{
    int i = path.lastIndexOf("/");
	if (i >= 0)
	{
		CloudFolder = path;
        BackUpsFolder = path + "/BackUps";
		CreateBackUpsFolder();
		return 1;
	}
	else
	{
        qInfo() << "Your path is broken";
		return 0;
	}
}

bool File::SetBackUpsFolder(QString path)
{
    int i = path.lastIndexOf("/");
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

bool File::isSynched() const
{
    return bSynch;
}

void File::SetCategory(QString cat)
{
    Category = cat;
}

QDataStream &operator <<(QDataStream& out, const File *file)
{
    out << file->GetPath();
    out << file->GetName();
    out << file->GetBUFolder();
    out << file->GetCFolder();
    out << file->GetCategory();
    out << file->isSynched();
    return out;
}

QDataStream &operator >>(QDataStream& in, File* &file)
{
    QString PathAndName;
    QString Name;
    QString BackUpsFolder;
    QString CloudFolder;
    QString Category;
    bool bSynch;

    in >> PathAndName;
    in >> Name;
    PathAndName += "/" + Name;
    file = new File(PathAndName);
    in >> BackUpsFolder;
    in >> CloudFolder;
    in >> Category;
    in >> bSynch;

    file->SetBackUpsFolder(BackUpsFolder);
    file->SetCloudFolder(CloudFolder);
    file->SetCategory(Category);
    file->SetSynch(bSynch);
    return in;
}

