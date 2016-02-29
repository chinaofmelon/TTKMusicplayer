#ifndef MUSICUSERMANAGER_H
#define MUSICUSERMANAGER_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QTime>
#include <QMenu>
#include "musicabstractmovedialog.h"

class MusicUserModel;

namespace Ui {
class MusicUserManager;
}

class MUSIC_USER_EXPORT MusicUserManager : public QDialog
{
    Q_OBJECT
public:
    explicit MusicUserManager(QWidget *parent = 0);
    ~MusicUserManager();

    void setUserUID(const QString &uid);

Q_SIGNALS:
    void userStateChanged(const QString &uid, const QString &icon);

public Q_SLOTS:
    void musicUserLogoff();
    void popupUserRecordWidget();
    void resetUserName(const QString &name);
    virtual int exec();
    /*!
     * Override exec function.
     */

protected:
    virtual void leaveEvent(QEvent *event) override;
    /*!
     * Override the widget event.
     */
    void createUserTime() const;
    void createButtonPopMenu();

    Ui::MusicUserManager *ui;
    MusicUserModel* m_userModel;
    QString m_currentUserUID;
    QTime m_time;
    QMenu m_popMenu;


};

#endif // MUSICUSERMANAGER_H
