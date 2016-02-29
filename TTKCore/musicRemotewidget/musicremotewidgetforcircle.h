#ifndef MUSICREMOTEWIDGETFORCIRCLE_H
#define MUSICREMOTEWIDGETFORCIRCLE_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicremotewidget.h"

class MUSIC_REMOTE_EXPORT MusicRemoteWidgetForCircle : public MusicRemoteWidget
{
    Q_OBJECT
public:
    explicit MusicRemoteWidgetForCircle(QWidget *parent = 0);

protected:
    virtual void paintEvent(QPaintEvent *event) override;
    /*!
     * Override the widget event.
     */

};

#endif // MUSICREMOTEWIDGETFORCIRCLE_H
