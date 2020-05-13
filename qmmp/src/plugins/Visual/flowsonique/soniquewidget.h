/* =================================================
 * This file is part of the TTK qmmp plugin project
 * Copyright (C) 2015 - 2020 Greedysky Studio

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; If not, see <http://www.gnu.org/licenses/>.
 ================================================= */

#ifndef SONIQUEWIDGET_H
#define SONIQUEWIDGET_H

#include <QMutex>
#include <QGLWidget>
#include "vis.h"
#include "kiss_fft.h"

/*!
 * @author Greedysky <greedysky@163.com>
 */
class SoniqueWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit SoniqueWidget(QWidget *parent = nullptr);
    virtual ~SoniqueWidget();

    void addBuffer(float *left, float *right);

protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int width, int height) override;
    virtual void paintGL() override;

public slots:
    void nextPreset();
    void previousPreset();
    void randomPreset();

private:
    void closePreset();
    void generatePreset();

    VisInfo *m_sonique;
    VisData *m_visData;
    unsigned long *m_texture;
    unsigned long *m_visproc;
#ifdef Q_OS_UNIX
    void *m_instance;
#else
    HINSTANCE m_instance;
#endif

    QMutex m_mutex;
    int m_currentIndex;
    QStringList m_presetList;

    kiss_fft_cfg m_kiss_cfg;
    kiss_fft_cpx *m_in_freq_data;
    kiss_fft_cpx *m_out_freq_data;
};

#endif
