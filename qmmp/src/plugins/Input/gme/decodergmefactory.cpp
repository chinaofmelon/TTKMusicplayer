#include "gmehelper.h"
#include "decoder_gme.h"
#include "decodergmefactory.h"
#include "settingsdialog.h"

bool DecoderGmeFactory::canDecode(QIODevice *) const
{
    return false;
}

DecoderProperties DecoderGmeFactory::properties() const
{
    DecoderProperties properties;
    properties.name = tr("GME Plugin");
    properties.filters << "*.ay" << "*.gbs" << "*.gym" << "*.hes" << "*.kss" << "*.nsf" << "*.nsfe";
    properties.filters << "*.sap" << "*.spc" << "*.vgm" << "*.vgz";
    properties.description = tr("Game Music Emulator Files");
    properties.shortName = "gme";
    properties.hasSettings = true;
    properties.noInput = true;
    properties.protocols << "gme";
    return properties;
}

Decoder *DecoderGmeFactory::create(const QString &path, QIODevice *input)
{
    Q_UNUSED(input);
    return new DecoderGme(path);
}

QList<TrackInfo*> DecoderGmeFactory::createPlayList(const QString &path, TrackInfo::Parts parts, QStringList *ignoredFiles)
{
    if(path.contains("://")) //is it one track?
    {
        QString filePath = path;
        filePath.remove("gme://");
        filePath.remove(RegularWrapper("#\\d+$"));

        const int track = path.section("#", -1).toInt();
        QList<TrackInfo*> list = createPlayList(filePath, parts, ignoredFiles);
        if(list.isEmpty() || track <= 0 || track > list.count())
        {
            qDeleteAll(list);
            list.clear();
            return list;
        }

        TrackInfo *info = list.takeAt(track - 1);
        qDeleteAll(list);
        return QList<TrackInfo*>() << info;
    }
    else
    {
        if(ignoredFiles)
            ignoredFiles->push_back(path);
    }

    GmeHelper helper;
    Music_Emu *emu = helper.load(path);
    if(!emu)
    {
        qWarning("DecoderGmeFactory: unable to open file");
        return QList<TrackInfo*>();
    }
    return helper.createPlayList(parts);
}

MetaDataModel* DecoderGmeFactory::createMetaDataModel(const QString &path, bool readOnly)
{
    Q_UNUSED(path);
    Q_UNUSED(readOnly);
    return nullptr;
}

void DecoderGmeFactory::showSettings(QWidget *parent)
{
    SettingsDialog *s = new SettingsDialog(parent);
    s->show();
}

#ifndef QMMP_GREATER_NEW
#include <QtPlugin>
Q_EXPORT_PLUGIN2(gme, DecoderGmeFactory)
#endif