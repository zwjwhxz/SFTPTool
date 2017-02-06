#include "sftpuploadpath.h"
#include "sftpconfigmanager.h"
#include <QMessageBox>

//file tree
SftpFileTree::SftpFileTree()
{

}

SftpFileTree::~SftpFileTree()
{
    QVector<FILE_HANDLE*>::iterator it = m_all_file.begin();
    for ( ; it != m_all_file.end(); ++it)
    {
        delete *it;
    }
}

int SftpFileTree::init_subfile(QString file_path)
{
    QFileInfo fi(file_path);
    //文件路径直接添加返回
    if (fi.isFile())
    {
        add_file(file_path, true);
        return 1;
    }
    //既不是文件也不是目录返回错误
    if (!fi.isDir())
    {
        return 0;
    }

    QDir root_file(file_path);
    root_file.setFilter( QDir::Dirs | QDir::Files | QDir::NoDot | QDir::NoDotDot );
    QFileInfoList list = root_file.entryInfoList();
    //空文件夹
    if (0 == list.size())
    {
        add_file(file_path, false);
        return 1;
    }

    QFileInfoList::iterator it = list.begin();
    for (; it != list.end(); ++it)
    {
        if (it->isDir())
        {
            init_subfile(it->filePath());
            add_file(it->filePath(), false);
            continue;
        }
        add_file(it->filePath(), true);
    }
    return 1;
}

void SftpFileTree::add_file(QString file_path, bool is_file)
{
    FILE_HANDLE* handle = new FILE_HANDLE;
    handle->is_file = is_file;
    handle->local_path = file_path;
    if (!make_remote_path(handle))
    {
        return;
    }
    m_all_file.push_back(handle);
}

int SftpFileTree::make_remote_path(FILE_HANDLE* handle)
{
    QString local_path_base = SftpConfigManager::instance()->get_local_file_path(m_game_index);
    QString remote_path_base = SftpConfigManager::instance()->get_remote_file_path(m_game_index);
    remote_path_base += '/';
    QString remote_path = handle->local_path;
    remote_path.replace(QString("\\"), QString("/"));
    remote_path.replace(QString("//"), QString("/"));
    local_path_base.replace(QString("\\"), QString("/"));
    local_path_base.replace(QString("//"), QString("/"));
    remote_path.replace(local_path_base, remote_path_base);
    if (-1 == remote_path.indexOf(remote_path_base))
    {
        return 0;
    }
    handle->remote_path = remote_path;
    return 1;
}

SftpUploadPath::SftpUploadPath(SftpWindow* sfpt_window)
{
    m_connector_list = sfpt_window->m_connector_list;
}

SftpUploadPath::~SftpUploadPath()
{

}

int SftpUploadPath::upload_path_file(QString file_path, int game_index)
{
    file_tree_handle.set_game_index(game_index);
    load_all_file(file_path);
    upload_all_file();
}

int SftpUploadPath::load_all_file(QString file_path)
{
    file_tree_handle.init_subfile(file_path);
}

int SftpUploadPath::upload_all_file()
{
    QVector<FILE_HANDLE*> file_handles = file_tree_handle.get_all_file();
    QVector<FILE_HANDLE*>::iterator it = file_handles.begin();

    for ( ; it != file_handles.end(); ++it)
    {
        FILE_HANDLE* handle = *it;
        if (handle->is_file)
        {

        }
    }
}
