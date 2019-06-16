#ifndef APPINFO_H
#define APPINFO_H

#include <QtCore>

class AppInfo
{
public:
        static AppInfo& getInstance()
        {
            static AppInfo    instance; // Guaranteed to be destroyed.
                                  // Instantiated on first use.
            return instance;
        }

        QString getAppConfigFileName(){
            return appConfigFileName;
        }
        void setAppConfigFileName(QString _appConfigFileName){
            appConfigFileName = _appConfigFileName;
        }

        QCoreApplication* getApp(){
            return app;
        }
        void setApp(QCoreApplication* _app){
            app = _app;
        }

        QString getAppRunningFolder(){
            return appRunningFolder;
        }
        void setAppRunningFolder(QString _appRunningFolder){
            appRunningFolder = _appRunningFolder;
        }

private:
        AppInfo() {}                    // Constructor? (the {} brackets) are needed here.

        // C++ 03
        // ========
        // Don't forget to declare these two. You want to make sure they
        // are unacceptable otherwise you may accidentally get copies of
        // your singleton appearing.
        AppInfo(AppInfo const&);              // Don't Implement
        void operator=(AppInfo const&); // Don't implement

        // C++ 11
        // =======
        // We can use the better technique of deleting the methods
        // we don't want.
//public:
//        AppInfo(AppInfo const&)               = delete;
//        void operator=(AppInfo const&)  = delete;

        // Note: Scott Meyers mentions in his Effective Modern
        //       C++ book, that deleted functions should generally
        //       be public as it results in better error messages
        //       due to the compilers behavior to check accessibility
        //       before deleted status

        QString appConfigFileName;
        QCoreApplication* app;
        QString appRunningFolder;
};

#endif // APPINFO_H
