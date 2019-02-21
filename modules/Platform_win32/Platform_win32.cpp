
#include <eggRuntime/Platform.h>
#include <eggRuntime/Module.h>
#include <eggRuntime/Runtime.h>

#include <eggRuntime/Path.h>

// vc mingw cygwin

//#include <algorithm>

#include <windows.h>
#include <stdio.h>
#include <shlobj.h>
#include <direct.h>  
#include <io.h>  
#include <algorithm>  
#include <sys/stat.h>
#define ACCESS _access  
#define MKDIR(a) _mkdir((a))
#define RMDIR(a) _rmdir((a))
#define STRDUP(a) _strdup((a)) 
#define GETCWD(a,b) _getcwd((a),(b)) 

#ifdef __MINGW32__
#define _WIN32_IE 0x0500  //for SHGetSpecialFolderPath()
#endif //__MINGW32__


//#define MAX_PATH_SIZE 260

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))
#endif

static void to_wchar(const char *path, wchar_t *wbuf, size_t wbuf_len) {
    char buf[MAX_PATH * 2], buf2[MAX_PATH * 2], *p;

    strncpy(buf, path, sizeof(buf));
    buf[sizeof(buf) - 1] = '\0';

    // Trim trailing slashes. Leave backslash for paths like "X:\"
    p = buf + strlen(buf) - 1;
    while (p > buf && p[-1] != ':' && (p[0] == '\\' || p[0] == '/')) *p-- = '\0';

    // Convert to Unicode and back. If doubly-converted string does not
    // match the original, something is fishy, reject.
    memset(wbuf, 0, wbuf_len * sizeof(wchar_t));
    MultiByteToWideChar(CP_ACP, 0, buf, -1, wbuf, (int)wbuf_len);
    WideCharToMultiByte(CP_ACP, 0, wbuf, (int)wbuf_len, buf2, sizeof(buf2),
        NULL, NULL);
    if (strcmp(buf, buf2) != 0) {
        wbuf[0] = L'\0';
    }
}

struct dirent
{
    char d_name[MAX_PATH];
};

typedef struct DIR {
    HANDLE   handle;
    WIN32_FIND_DATAW info;
    struct dirent result;
} DIR;

// Implementation of POSIX opendir/closedir/readdir for Windows.
static DIR *opendir(const char *name) {
    DIR *dir = NULL;
    wchar_t wpath[MAX_PATH];
    DWORD attrs;

    if (name == NULL) {
        SetLastError(ERROR_BAD_ARGUMENTS);
    }
    else if ((dir = (DIR *)malloc(sizeof(*dir))) == NULL) {
        SetLastError(ERROR_NOT_ENOUGH_MEMORY);
    }
    else {
        to_wchar(name, wpath, ARRAY_SIZE(wpath));
        attrs = GetFileAttributesW(wpath);
        if (attrs != 0xFFFFFFFF &&
            ((attrs & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY)) {
            (void)wcscat(wpath, L"\\*");
            dir->handle = FindFirstFileW(wpath, &dir->info);
            dir->result.d_name[0] = '\0';
        }
        else {
            free(dir);
            dir = NULL;
        }
    }

    return dir;
}

static int closedir(DIR *dir) {
    int result = 0;

    if (dir != NULL) {
        if (dir->handle != INVALID_HANDLE_VALUE)
            result = FindClose(dir->handle) ? 0 : -1;

        free(dir);
    }
    else {
        result = -1;
        SetLastError(ERROR_BAD_ARGUMENTS);
    }

    return result;
}

static struct dirent *readdir(DIR *dir) {
    struct dirent *result = 0;

    if (dir) {
        if (dir->handle != INVALID_HANDLE_VALUE) {
            result = &dir->result;
            (void)WideCharToMultiByte(CP_ACP, 0,
                dir->info.cFileName, -1, result->d_name,
                sizeof(result->d_name), NULL, NULL);

            if (!FindNextFileW(dir->handle, &dir->info)) {
                (void)FindClose(dir->handle);
                dir->handle = INVALID_HANDLE_VALUE;
            }

        }
        else {
            SetLastError(ERROR_FILE_NOT_FOUND);
        }
    }
    else {
        SetLastError(ERROR_BAD_ARGUMENTS);
    }

    return result;
}




class Platform_win32
    :public egg::Platform
{
public:
    Platform_win32() {}
    ~Platform_win32() {}

    int createDirectory(const char *pDir)override
    {
        //LOGI("creatDir %s;",pDir);

        int i = 0;
        int iRet;
        int iLen;
        char* pszDir;

        if (NULL == pDir)
        {
            return 0;
        }

        pszDir = STRDUP(pDir);
        iLen = strlen(pszDir);
        //LOGI("pszDir %d %s;",iLen,pszDir);

        // �����м�Ŀ¼  
        for (i = 0; i < iLen; i++)
        {
            if (
                (i > 0)
                &&
                (pszDir[i] == '\\' || pszDir[i] == '/')
                )
            {
                pszDir[i] = '\0';

                //���������,����
                //LOGI("ACCESS %s;",pszDir);
                iRet = ACCESS(pszDir, 0);
                if (iRet != 0)
                {
                    //LOGI("dir no isesit %s;",pszDir);
                    iRet = MKDIR(pszDir);
                    if (iRet != 0)
                    {
                        //LOGI("error mkdir %s;",pszDir);
                        return -1;
                    }
                }

                //֧��linux,������\����/  
                pszDir[i] = '/';
            }
        }

        iRet = MKDIR(pszDir);
        free(pszDir);
        return iRet;
    }

    int isExist(const char* path)override
    {
        egg::Path p(path);
        std::string p2 = p.getNativePath();

        struct stat fileStat;
        int s = stat(p2.c_str(), &fileStat);
        if (s == 0)
        {
            if (fileStat.st_mode & S_IFREG)
            {
                return 1;
            }
            if (fileStat.st_mode & S_IFDIR)
            {
                return 2;
            }
        }
        return 0;
    }

    unsigned int Length(const char* path)override
    {
        struct stat fileStat;
        int s = stat(path, &fileStat);
        if (s == 0)
        {
            return fileStat.st_size;
        }
        return 0;
    }

    void removeFile(const char* path, bool force_delete_non_enpty_dir)override
    {
        //Ŀǰ��û֧��ɾ���ǿ�Ŀ¼

        int s = isExist(path);
        if (s == 1)
        {
            remove(path);
        }
        if (s == 2)
        {
            RMDIR(path);
        }
    }

    std::vector<std::string> listFile(const char* dir, const char* path, bool ergodic)override
    {
        //�洢�б�
        std::vector<std::string> file_list;
        std::vector<std::string> dir_list;

        //����·��
        egg::Path lp(dir);
        lp.push(path);

        //��Ŀ¼
        DIR* d = opendir(lp.getNativePath().c_str());
        if (d)
        {
            struct dirent* ent;
            while ((ent = readdir(d)) != NULL)
            {
                if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0)
                {
                    continue;
                }

                //��ǰ�ļ���������·��
                egg::Path cp(lp.getNativePath().c_str());
                cp.push(ent->d_name);

                //���dir���ڲ�·��
                egg::Path ip(path);
                ip.push(ent->d_name);

                int s = isExist(cp.getNativePath().c_str());
                if (s == 1)
                {
                    file_list.push_back(ip.getNativePath());
                }
                if (s == 2)
                {
                    dir_list.push_back(ip.getNativePath());

                    if (ergodic)
                    {
                        std::vector<std::string> sub_list = listFile(dir, ip.getNativePath().c_str(), ergodic);
                        dir_list.insert(dir_list.end(), sub_list.begin(), sub_list.end());
                    }
                }
                //printf("%s\n", ent->d_name);
            }
        }
        //else
        //{
        //    printf("%s\n", "Cannot open directory!");
        //}
        //char ch = getchar();
        //return 0;

        dir_list.insert(dir_list.end(), file_list.begin(), file_list.end());
        return dir_list;
    }

    std::string getApplicationDir()override
    {
        char buff[MAX_PATH];
        GETCWD(buff, MAX_PATH);
        return std::string(buff);
    }

    std::string getApplicationUrl()override
    {
        egg::Path p(getApplicationDir().c_str());
        return std::string("file:/") + p.getPathUnix();
    }

    std::string getAppStorageDir()override
    {
        return getenv("appdata");
        return "";
    }

    std::string getAppStorageUrl()override
    {
        egg::Path p(getAppStorageDir().c_str());
        return std::string("file:/") + p.getPathUnix();
    }

    std::string getCacheDir()override
    {
        return getenv("temp");
        return "";
    }

    std::string getCacheUrl()override
    {
        egg::Path p(getCacheDir().c_str());
        return std::string("file:/") + p.getPathUnix();
    }

    std::string getDesktopDir()override
    {
        char path[MAX_PATH];
        if (SHGetSpecialFolderPathA(HWND_DESKTOP, path, CSIDL_DESKTOP, FALSE))
        {
            return path;
        }
        return "";
    }

    std::string getDesktopUrl()override
    {
        egg::Path p(getDesktopDir().c_str());
        return std::string("file:/") + p.getPathUnix();
    }

    std::string getDocumentsDir() override
    {
        char my_documents[MAX_PATH];
        if (SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, my_documents) == S_OK)
        {
            return my_documents;
        }
        return "";
    }

    std::string getDocumentsUrl() override
    {
        egg::Path p(getDocumentsDir().c_str());
        return std::string("file:/") + p.getPathUnix();
    }


private:

};


INSTALL_MODULE(Platform_win32)
{
    egg::Platform::setInstance(new Platform_win32());

    return true;
}

UNINSTALL_MODULE(Platform_win32)
{
    egg::Platform::setInstance(0);

    return true;
}


