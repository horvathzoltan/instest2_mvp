#ifndef COMMANDLINEARGS_H
#define COMMANDLINEARGS_H

#include <QCoreApplication>
#include <QString>


struct CommadLineArgs{
    QString working_dir;
    bool isTest = false;

    bool IsValid();
    static CommadLineArgs Parse(const QCoreApplication& app);
};

#endif // COMMANDLINEARGS_H
