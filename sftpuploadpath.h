#ifndef SFTPUPLOADPATH_H
#define SFTPUPLOADPATH_H

#include <QDir>
#include <QString>
#include <QVector>

typedef struct file_handle
{
    bool is_file;

    QString local_path;

    QString remote_path;
}FILE_HANDLE;

//file tree
class SftpFileTree
{
public:
    SftpFileTree();

    ~SftpFileTree();

    int get_subfile(QString file_path);

private:
    void add_file(QString file_path, bool is_file);

private:
    QVector<FILE_HANDLE*> m_all_file;
};

class SftpUploadPath
{
public:
    SftpUploadPath();

    ~SftpUploadPath();

    int upload_path_file(QString file_path);

private:
    int load_all_file(QString file_path);

    int upload_all_file();

private:
    SftpFileTree file_tree_handle;
};

#endif // SFTPUPLOADPATH_H
