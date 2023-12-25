#include "musickwqueryrequest.h"

MusicKWQueryRequest::MusicKWQueryRequest(QObject *parent)
    : MusicAbstractQueryRequest(parent)
{
    m_pageSize = 40;
    m_queryServer = QUERY_KW_INTERFACE;
}

void MusicKWQueryRequest::startToPage(int offset)
{
    TTK_INFO_STREAM(QString("%1 startToPage %2").arg(className()).arg(offset));

    deleteAll();
    m_totalSize = 0;
    m_pageIndex = offset;

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(KW_SONG_SEARCH_URL, false).arg(m_queryValue).arg(offset).arg(m_pageSize));
    MusicKWInterface::makeRequestRawHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicKWQueryRequest::startToSearch(const QString &value)
{
    TTK_INFO_STREAM(QString("%1 startToSearch %2").arg(className(), value));

    m_queryValue = value.trimmed();
    MusicAbstractQueryRequest::downLoadFinished();

    startToPage(0);
}

void MusicKWQueryRequest::startToSingleSearch(const QString &value)
{
    TTK_INFO_STREAM(QString("%1 startToSingleSearch %2").arg(className(), value));

    deleteAll();

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(KW_SONG_INFO_URL, false).arg(value));
    MusicKWInterface::makeRequestRawHeader(&request);

    QNetworkReply *reply = m_manager.get(request);
    connect(reply, SIGNAL(finished()), SLOT(downLoadSingleFinished()));
    QtNetworkErrorConnect(reply, this, replyError, TTK_SLOT);
}

void MusicKWQueryRequest::startToQueryResult(TTK::MusicSongInformation *info, int bitrate)
{
    MusicPageQueryRequest::downLoadFinished();
    TTK_INFO_STREAM(QString("%1 startToQueryResult %2 %3kbps").arg(className(), info->m_songId).arg(bitrate));

    TTK_NETWORK_QUERY_CHECK();
    MusicKWInterface::parseFromSongProperty(info, bitrate);
    TTK_NETWORK_QUERY_CHECK();

    findUrlFileSize(&info->m_songProps, info->m_duration);
    MusicAbstractQueryRequest::startToQueryResult(info, bitrate);
}

void MusicKWQueryRequest::downLoadFinished()
{
    TTK_INFO_STREAM(QString("%1 downLoadFinished").arg(className()));

    MusicPageQueryRequest::downLoadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJson::Parser json;
        bool ok = false;
        const QVariant &data = json.parse(m_reply->readAll().replace("'", "\""), &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value.contains("abslist"))
            {
                m_totalSize = value["TOTAL"].toInt();

                const QVariantList &datas = value["abslist"].toList();
                for(const QVariant &var : qAsConst(datas))
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    TTK_NETWORK_QUERY_CHECK();

                    TTK::MusicSongInformation info;
                    info.m_singerName = TTK::String::charactersReplace(value["ARTIST"].toString());
                    info.m_songName = TTK::String::charactersReplace(value["SONGNAME"].toString());
                    info.m_duration = TTKTime::formatDuration(value["DURATION"].toInt() * TTK_DN_S2MS);

                    info.m_songId = value["MUSICRID"].toString().replace("MUSIC_", "");
                    info.m_artistId = value["ARTISTID"].toString();
                    info.m_albumId = value["ALBUMID"].toString();

                    info.m_year = value["RELEASEDATE"].toString();
                    info.m_trackNumber = "0";

                    info.m_coverUrl = value["web_albumpic_short"].toString();
                    MusicKWInterface::makeCoverPixmapUrl(info.m_coverUrl);
                    info.m_lrcUrl = TTK::Algorithm::mdII(KW_SONG_LRC_URL, false).arg(info.m_songId);
                    info.m_albumName = TTK::String::charactersReplace(value["ALBUM"].toString());

                    if(m_queryMode != QueryMode::Meta)
                    {
                        TTK_NETWORK_QUERY_CHECK();
                        MusicKWInterface::parseFromSongProperty(&info, value["FORMATS"].toString());
                        TTK_NETWORK_QUERY_CHECK();

                        MusicResultInfoItem item;
                        item.m_songName = info.m_songName;
                        item.m_singerName = info.m_singerName;
                        item.m_albumName = info.m_albumName;
                        item.m_duration = info.m_duration;
                        item.m_type = mapQueryServerString();
                        Q_EMIT createSearchedItem(item);
                    }
                    m_songInfos << info;
                }
            }
        }
    }

    Q_EMIT downLoadDataChanged({});
    deleteAll();
}

void MusicKWQueryRequest::downLoadSingleFinished()
{
    TTK_INFO_STREAM(QString("%1 downLoadSingleFinished").arg(className()));

    MusicAbstractQueryRequest::downLoadFinished();
    QNetworkReply *reply = TTKObjectCast(QNetworkReply*, sender());
    if(reply && reply->error() == QNetworkReply::NoError)
    {
        QJson::Parser json;
        bool ok = false;
        const QVariant &data = json.parse(reply->readAll(), &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value.contains("data"))
            {
                value = value["data"].toMap();

                TTK::MusicSongInformation info;
                info.m_singerName = TTK::String::charactersReplace(value["artist"].toString());
                info.m_songName = TTK::String::charactersReplace(value["name"].toString());
                info.m_duration = TTKTime::formatDuration(value["duration"].toInt() * TTK_DN_S2MS);

                info.m_songId = value["rid"].toString();
                info.m_artistId = value["artistid"].toString();
                info.m_albumId = value["albumid"].toString();

                info.m_year = value["releaseDate"].toString();
                info.m_trackNumber = value["track"].toString();

                info.m_coverUrl = value["pic"].toString();
                info.m_lrcUrl = TTK::Algorithm::mdII(KW_SONG_LRC_URL, false).arg(info.m_songId);
                info.m_albumName = TTK::String::charactersReplace(value["album"].toString());

                TTK_NETWORK_QUERY_CHECK();
                MusicKWInterface::parseFromSongProperty(&info, "MP3128|MP3192|MP3H");
                TTK_NETWORK_QUERY_CHECK();

                MusicResultInfoItem item;
                item.m_songName = info.m_songName;
                item.m_singerName = info.m_singerName;
                item.m_albumName = info.m_albumName;
                item.m_duration = info.m_duration;
                item.m_type = mapQueryServerString();
                Q_EMIT createSearchedItem(item);
                m_songInfos << info;
            }
        }
    }

    Q_EMIT downLoadDataChanged({});
    deleteAll();
}
