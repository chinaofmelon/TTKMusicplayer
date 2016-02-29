#ifndef MUSICLRCSEARCHTABLEWIDGET_H
#define MUSICLRCSEARCHTABLEWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicquerytablewidget.h"

class MUSIC_LRC_EXPORT MusicLrcSearchTableWidget : public MusicQueryTableWidget
{
    Q_OBJECT
public:
    explicit MusicLrcSearchTableWidget(QWidget *parent = 0);
    virtual ~MusicLrcSearchTableWidget();

    virtual void startSearchQuery(const QString &text) override;
    virtual void musicDownloadLocal(int row) override;
    inline void setCurrentSongName(const QString &name){ m_currentSongName = name;}

Q_SIGNALS:
    void resolvedSuccess();
    void lrcDownloadStateChanged(const QString &name);

public Q_SLOTS:
    void clearAllItems() override;
    /*!
     * Clear All Items.
     */
    void creatSearchedItems(const QString &songname,
                            const QString &artistname, const QString &time) override;
    void itemDoubleClicked(int row, int column) override;

protected:
    virtual void contextMenuEvent(QContextMenuEvent *event) override;
    /*!
     * Override the widget event.
     */

    QString m_currentSongName;

};

#endif // MUSICLRCSEARCHTABLEWIDGET_H
