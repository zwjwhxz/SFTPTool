#include "sftpuploadpath.h"

//file tree
SftpFileTree::SftpFileTree()
{

}

SftpFileTree::~SftpFileTree()
{
    QVector<FILE_HANDLE*>::iterator it = m_all_file.begin();
    for ( ; it != m_all_file.end(); ++it)
    {
        delete it;
    }
}

int SftpFileTree::get_subfile(QString file_path)
{
    QDir root_file(file_path);

    QFileInfoList list = root_file.entryInfoList();
    QFileInfoList::iterator it = list.begin();

    for (; it != list.end(); ++it)
    {
        if (it->isDir())
        {
            get_subfile(it->filePath());
            add_file(it->filePath(), false);
        }
        add_file(it->filePath(), true);
    }
}

void SftpFileTree::add_file(QString file_path, bool is_file)
{
    FILE_HANDLE* handle = new FILE_HANDLE;
    handle->is_file = is_file;
    handle->local_path = file_path;
    m_all_file.push_back(handle);
}

SftpUploadPath::SftpUploadPath()
{

}

SftpUploadPath::~SftpUploadPath()
{

}

int SftpUploadPath::upload_path_file(QString file_path)
{
    load_all_file(file_path);
    upload_all_file();
}

int SftpUploadPath::load_all_file(QString file_path)
{
    file_tree_handle.get_subfile(file_path);
}

int SftpUploadPath::upload_all_file()
{

}
