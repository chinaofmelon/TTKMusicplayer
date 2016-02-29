#ifndef MUSICPREVIEWLABEL_H
#define MUSICPREVIEWLABEL_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QLabel>
#include "musicglobaldefine.h"

/*! @brief The class of the setting preview label.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicPreviewLabel : public QLabel
{
public:
    explicit MusicPreviewLabel(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */

    void setParameter(const QStringList &para);
    /*!
     * Set preview parameters(Family\PointSize\Bold or Italic).
     */
    void setLinearGradient(QColor &fg, QColor &bg);
    /*!
     * Set linear gradient and fg and bg.
     */
    void setTransparent(int trans) { m_transparent = trans;}
    /*!
     * Set current transparent.
     */

protected:
    virtual void paintEvent(QPaintEvent *event) override;
    /*!
     * Override the widget event.
     */

    QFont m_font;
    QLinearGradient m_linearGradient;
    QLinearGradient m_maskLinearGradient;
    int m_transparent;

};

#endif // MUSICPREVIEWLABEL_H
