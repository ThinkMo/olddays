#include <QtGui/QApplication>
#include <QMessageBox>
#include <QTextStream>
#include <QString>
#include <QTextCodec>
#include <cstring>
#include <windows.h>

//#include "Log.h"
#include "mainwindow.h"

int GetOSVer()
{
    OSVERSIONINFO   osver;
    osver.dwOSVersionInfoSize   =   sizeof(OSVERSIONINFO);
    GetVersionEx(&osver);
    if(osver.dwPlatformId == 2)
    {
        if(osver.dwMajorVersion == 5 && osver.dwMinorVersion == 1)
        {
            //printf("xp\n");
            return(2);
        }
        if(osver.dwMajorVersion == 5 && osver.dwMinorVersion == 2)
        {
            //printf("windows 2003\n");
            return(3);
        }
        if(osver.dwMajorVersion ==  6 && osver.dwMinorVersion == 0)
        {
            //printf("vista and 2008\n");
            return(4);
        }
        if(osver.dwMajorVersion ==  6 && osver.dwMinorVersion == 1)
        {
            //printf("2008 R2 and Windows 7\n");
            return(5);
        }
    }
    return 0;
}
/*
int geturl(char *arg,char *url)
{
    //|userid=xxx|username=xxx|passportdes=xxx|passwordrsa=xxx|ccip=xxx|
    //http://ip/cu/postcustd.php?userid=xx&pass=xx
    char ccip[20] = {0};
    char userid[10] = {0};
    char password[2000] = {0};

    char *start,*end;
    start = strstr(arg,"userid=")+7;
    end = strchr(start,'|');
    if(start == NULL || end == NULL)
        return -1;
    strncpy(userid,start,end-start);

    start = strstr(arg,"ccip=")+5;
    end = strchr(start,'|');
    if(start == NULL || end == NULL)
        return -1;
    strncpy(ccip,start,end-start);

    start = strstr(arg,"passwordrsa=")+12;
    end = strchr(start,'|');
    if(start == NULL || end == NULL)
        return -1;
    strncpy(password,start,end-start);

    strcat(url,"http://");
    strcat(url,ccip);
    strcat(url,"/cu/postcustd.php?userid=");
    strcat(url,userid);
    strcat(url,"&pass=");
    strcat(url,password);
    return 0;
}
*/
int main(int argc, char *argv[])
{
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);

    QApplication a(argc, argv);
    int flag = 0;
#ifdef Q_WS_WIN
    flag = GetOSVer();
#endif
    /*
    if(argc != 2)
        exit(-1);
    */
/*
    char url[2048] = {0};
    if(geturl(argv[1],url) < 0)
        exit(-1);
*/
    //CLog::Log(LL_DEBUG,"msg",81,2,"%s",url);

    MainWindow w(argv,flag);
    w.show();

    return a.exec();
}
