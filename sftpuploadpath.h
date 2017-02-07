#ifndef SFTPUPLOADPATH_H
#define SFTPUPLOADPATH_H

#include <QDir>
#include <QString>
#include <QVector>
#include "sftpwindow.h"

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

    int init_subfile(QString file_path);

    void set_game_index(int game_index){m_game_index = game_index;};

    int get_game_index(){return m_game_index;};

    QVector<FILE_HANDLE*> get_all_file(){return m_all_file;};

private:
    int add_file(QString file_path, bool is_file);

    int make_remote_path(FILE_HANDLE* handle);

private:
    int m_game_index;

    QVector<FILE_HANDLE*> m_all_file;
};

class SftpUploadPath
{
public:
    SftpUploadPath(SftpWindow* sfpt_window);

    ~SftpUploadPath();

    int upload_path_file(QString file_path, int game_index);

private:
    int load_all_file(QString file_path);

    int upload_all_file();

private:
    SftpFileTree file_tree_handle;

    SftpWindow* m_sftp_window;

    QHash<int, SftpList*> m_connector_list;
};

#endif // SFTPUPLOADPATH_H
