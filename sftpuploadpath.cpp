#include "sftpuploadpath.h"
#include "sftpconfigmanager.h"
#include "sftpconnector.h"

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
    QString local_path_base = SftpConfigManager::instance()->get_local_file_path(m_game_index);
    file_path = local_path_base + file_path;
    QFileInfo fi(file_path);
    //文件路径直接添加返回
    if (fi.isFile())
    {
        if (!add_file(file_path, true))
        {
            return 0;
        }
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
        if (!add_file(file_path, false))
        {
            return 0;
        }
        return 1;
    }

    QFileInfoList::iterator it = list.begin();
    for (; it != list.end(); ++it)
    {
        if (it->isDir())
        {
            if (!init_subfile(it->filePath()))
            {
                 return 0;
            }
            if (!add_file(it->filePath(), false))
            {
                return 0;
            }
            continue;
        }
        if (!add_file(it->filePath(), true))
        {
            return 0;
        }
    }
    return 1;
}

int SftpFileTree::add_file(QString file_path, bool is_file)
{
    FILE_HANDLE* handle = new FILE_HANDLE;
    handle->is_file = is_file;
    handle->local_path = file_path;
    if (!make_remote_path(handle))
    {
        return 0;
    }
    m_all_file.push_back(handle);
    return 1;
}

int SftpFileTree::make_remote_path(FILE_HANDLE* handle)
{
    QString local_path_base = SftpConfigManager::instance()->get_local_file_path(m_game_index);
    QString remote_path_base = SftpConfigManager::instance()->get_remote_file_path(m_game_index);
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
    m_sftp_window = sfpt_window;
}

SftpUploadPath::~SftpUploadPath()
{

}

int SftpUploadPath::load_path_file(QString file_path, int game_index)
{
    file_tree_handle.set_game_index(game_index);
    if (!load_all_file(file_path))
    {
        return 0;
    }
}

int SftpUploadPath::load_all_file(QString file_path)
{
    if (!file_tree_handle.init_subfile(file_path))
    {
        return 0;
    }
}

int SftpUploadPath::upload_all_file()
{
    QVector<FILE_HANDLE*> file_handles = file_tree_handle.get_all_file();
    QVector<FILE_HANDLE*>::iterator it = file_handles.begin();

    int game_index = file_tree_handle.get_game_index();
    QSet<SftpConnector*> sftp_session;
    for ( ; it != file_handles.end(); ++it)
    {
        FILE_HANDLE* handle = *it;
        if (handle->is_file)
        {
            if (0 == game_index)
            {
                QTreeWidgetItemIterator item_it(m_sftp_window->m_ui_context->TREE_99_SERVER);
                while (*item_it)
                {
                    if (Qt::Checked == (*item_it)->checkState(1))
                    {
                        QString server_name = (*item_it)->text(0);
                        if (-1 == m_connector_list.value(game_index)->value(server_name)->upload_path(handle->local_path, handle->remote_path))
                        {
                            return -1;
                        }
                        sftp_session.insert(m_connector_list.value(game_index)->value(server_name));
                    }
                    ++item_it;
                }
            }
            if (1 == game_index)
            {
                QTreeWidgetItemIterator item_it(m_sftp_window->m_ui_context->TREE_DUMMY_SERVER);
                while (*item_it)
                {
                    if (Qt::Checked == (*item_it)->checkState(1))
                    {
                        QString server_name = (*item_it)->text(0);
                        if (-1 == m_connector_list.value(game_index)->value(server_name)->upload_path(handle->local_path, handle->remote_path))
                        {
                            return -1;
                        }
                        sftp_session.insert(m_connector_list.value(game_index)->value(server_name));
                    }
                    ++item_it;
                }
            }
            if (2 == game_index)
            {
                QTreeWidgetItemIterator item_it(m_sftp_window->m_ui_context->TREE_KUNKA_SERVER);
                while (*item_it)
                {
                    if (Qt::Checked == (*item_it)->checkState(1))
                    {
                        QString server_name = (*item_it)->text(0);
                        if (-1 == m_connector_list.value(game_index)->value(server_name)->upload_path(handle->local_path, handle->remote_path))
                        {
                            return -1;
                        }
                        sftp_session.insert(m_connector_list.value(game_index)->value(server_name));
                    }
                    ++item_it;
                }
            }
        }
    }
    QSet<SftpConnector*>::iterator session_it = sftp_session.begin();
    for (; session_it != sftp_session.end(); ++session_it)
    {
        (*session_it)->run_upload();
    }
}
