
#include "SZPArchive.h"

using namespace egg;

SZPEntry::SZPEntry()
{
}

SZPEntry::~SZPEntry()
{
}

std::string SZPEntry::getName()
{
    return "";
}

bool SZPEntry::isFile()
{
    return false;
}

bool SZPEntry::isDir()
{
    return false;
}

unsigned int SZPEntry::getLen()
{
    return 0;
}

void SZPEntry::read(char* data)
{
}



static int get_or_create_table_callback(void *NotUsed, int argc, char **argv, char **azColName)
{
    //int i;
    //for (i = 0; i < argc; i++)
    //{
    //    printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    //}
    //printf("\n");
    return 0;
}



SZPArchive::SZPArchive()
    :db(0)
{
}

SZPArchive::~SZPArchive()
{
}

std::string SZPArchive::getExt()
{
    return "";
}

void SZPArchive::open(const std::string path, int mode)
{
    //���Դ����ݿ�
    int rc = sqlite3_open( path.c_str() , &db );
    if (rc)
    {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        db = 0;
    }

    //������ݱ��Ƿ���� ��������ھͽ�����
    std::string sql_get_or_create_table = 
        "CREATE TABLE  if not exists [filelist] (\
        [id] INTEGER DEFAULT '1' NOT NULL PRIMARY KEY AUTOINCREMENT,\
        [abs_path] TEXT  NULL,\
        [is_dir] BOOLEAN DEFAULT 'true' NULL,\
        [size] INTEGER DEFAULT '1' NULL,\
        [comp_size] INTEGER DEFAULT '1' NULL,\
        [time] TEXT DEFAULT CURRENT_TIME NULL,\
        [crc32] VARCHAR(32)  NULL,\
        [file] BLOB  NULL\
        )";
    char *zErrMsg = 0;
    rc = sqlite3_exec(db, sql_get_or_create_table.c_str(), get_or_create_table_callback, 0, &zErrMsg); //�����ѯ��û�з���ֵ
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
}

void SZPArchive::close()
{
    sqlite3_close(db);
    db = 0;
}

bool SZPArchive::isOpen()
{
    return db!=0;
}

Entry* SZPArchive::getEntry(const std::string path)
{
    return 0;
}

void SZPArchive::setEntryAsFile(const std::string path, const char* data, unsigned int len)
{
}

bool SZPArchive::newEntryAsDir(const std::string path)
{
    //����һ��Ŀ¼����
    //INSERT INTO [filelist] VALUES(NULL,'path','true',111,222,datetime('now'),'abcd',x'66');

    //���������Ƿ����

    //������ھ͸���

    //��������ھͲ���


    //std::string sql_get_or_create_table =
    //        "CREATE TABLE  if not exists [filelist] (\
    //        [id] INTEGER DEFAULT '1' NOT NULL PRIMARY KEY AUTOINCREMENT,\
    //        [abs_path] TEXT  NULL,\
    //        [is_dir] BOOLEAN DEFAULT 'true' NULL,\
    //        [size] INTEGER DEFAULT '1' NULL,\
    //        [comp_size] INTEGER DEFAULT '1' NULL,\
    //        [ctime] TEXT DEFAULT CURRENT_TIME NULL,\
    //        [crc32] VARCHAR(32)  NULL,\
    //        [file] BLOB  NULL\
    //        )";
    //char *zErrMsg = 0;
    //int rc = sqlite3_exec(db, sql_get_or_create_table.c_str(), get_or_create_table_callback, 0, &zErrMsg); //�����ѯ��û�з���ֵ
    //if (rc != SQLITE_OK)
    //{
    //    fprintf(stderr, "SQL error: %s\n", zErrMsg);
    //    sqlite3_free(zErrMsg);
    //}

    return false;
}

void SZPArchive::delEntry(const std::string path)
{
}

std::vector<std::string> SZPArchive::listFile(const char* path, bool ergodic)
{

    return std::vector<std::string>();
}

