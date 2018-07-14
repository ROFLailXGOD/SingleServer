#pragma once
#include <QMetaType>
#include <QDataStream>
class File
{
private:
    QString path;
    QString name;
    QString BackUpsFolder; // BackUps folder Path
    QString CloudFolder; // Cloud Folder Path
    QString Category;

    bool bSynch;

	double time;

	bool CreateBackUpsFolder() const; // Creating BackUpsFolder only if not already exists
	bool CreateBackUp() const; // Creating a backup
public:
	File();
    File(QString);

    bool SetPathAndName(QString);
	bool SetLWTime();
    bool SetCloudFolder(QString);
    bool SetBackUpsFolder(QString);
    void SetCategory(QString);

    QString GetPath() const;
    QString GetName() const;
    QString GetBUFolder() const;
    QString GetCFolder() const;
    QString GetCategory() const;
    double GetLWTime() const;

	bool UpdateFile(); // Updating files

    bool isSynched() const; // Is file synched?
    void SetSynch(bool);
};
Q_DECLARE_METATYPE(File*)

QDataStream &operator<<(QDataStream&, const File *);
QDataStream &operator>>(QDataStream&, File* &);
