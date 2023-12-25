#include "musickgquerytoplistrequest.h"

MusicKGQueryToplistRequest::MusicKGQueryToplistRequest(QObject *parent)
    : MusicQueryToplistRequest(parent)
{
    m_queryServer = QUERY_KG_INTERFACE;
}

void MusicKGQueryToplistRequest::startToSearch(const QString &value)
{
    TTK_INFO_STREAM(QString("%1 startToSearch %2").arg(className(), value));

    deleteAll();
    m_queryValue = value.isEmpty() ? "6666" : value;

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(KG_TOPLIST_URL, false).arg(m_queryValue));
    MusicKGInterface::makeRequestRawHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicKGQueryToplistRequest::startToQueryResult(TTK::MusicSongInformation *info, int bitrate)
{
    MusicPageQueryRequest::downLoadFinished();
    TTK_INFO_STREAM(QString("%1 startToQueryResult %2 %3kbps").arg(className(), info->m_songId).arg(bitrate));

    TTK_NETWORK_QUERY_CHECK();
    MusicKGInterface::parseFromSongProperty(info, bitrate);
    TTK_NETWORK_QUERY_CHECK();

    MusicQueryToplistRequest::startToQueryResult(info, bitrate);
}

void MusicKGQueryToplistRequest::downLoadFinished()
{
    TTK_INFO_STREAM(QString("%1 downLoadFinished").arg(className()));

    MusicQueryToplistRequest::downLoadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJson::Parser json;
        bool ok = false;
        const QVariant &data = json.parse(m_reply->readAll(), &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value.contains("data"))
            {
                value = value["data"].toMap();

                queryToplistInfo(value);

                const QVariantList &datas = value["info"].toList();
                for(const QVariant &var : qAsConst(datas))
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    TTK_NETWORK_QUERY_CHECK();

                    TTK::MusicSongInformation info;
                    info.m_songName = TTK::String::charactersReplace(value["filename"].toString());
                    info.m_duration = TTKTime::formatDuration(value["duration"].toInt() * TTK_DN_S2MS);

                    if(info.m_songName.contains(TTK_DEFAULT_STR))
                    {
                        const QStringList &ll = info.m_songName.split(TTK_DEFAULT_STR);
                        info.m_singerName = TTK::String::charactersReplace(ll.front().trimmed());
                        info.m_songName = TTK::String::charactersReplace(ll.back().trimmed());
                    }

                    info.m_songId = value["hash"].toString();
                    info.m_albumId = value["album_id"].toString();

                    info.m_year.clear();
                    info.m_trackNumber = "0";

                    MusicResultDataItem albumInfo;
                    TTK_NETWORK_QUERY_CHECK();
                    MusicKGInterface::parseFromSongAlbumInfo(&albumInfo, info.m_albumId);
                    info.m_albumName = albumInfo.m_nickName;      
                    TTK_NETWORK_QUERY_CHECK();
                    MusicKGInterface::parseFromSongLrcAndPicture(&info);
                    TTK_NETWORK_QUERY_CHECK();
                    MusicKGInterface::parseFromSongProperty(&info, value);
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
    }

    Q_EMIT downLoadDataChanged({});
    deleteAll();
}

void MusicKGQueryToplistRequest::queryToplistInfo(const QVariantMap &input)
{
    Q_UNUSED(input);

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(KG_TOPLIST_INFO_URL, false));
    MusicKGInterface::makeRequestRawHeader(&request);

    const QByteArray &bytes = TTK::syncNetworkQueryForGet(&request);
    if(bytes.isEmpty())
    {
        return;
    }

    QJson::Parser json;
    bool ok = false;
    const QVariant &data = json.parse(bytes, &ok);
    if(ok)
    {
        QVariantMap value = data.toMap();
        if(value.contains("data"))
        {
            value = value["data"].toMap();
            MusicResultDataItem result;
            result.m_updateTime = TTKDateTime::format(value["timestamp"].toLongLong() * TTK_DN_S2MS, TTK_YEAR_FORMAT);

            const QVariantList &datas = value["info"].toList();
            for(const QVariant &var : qAsConst(datas))
            {
                if(var.isNull())
                {
                    continue;
                }

                value = var.toMap();
                TTK_NETWORK_QUERY_CHECK();

                if(m_queryValue != value["rankid"])
                {
                    continue;
                }

                result.m_name = value["rankname"].toString();
                result.m_coverUrl = value["banner7url"].toString().replace("{size}", "400");
                result.m_playCount = value["play_times"].toString();
                result.m_description = value["intro"].toString();
                Q_EMIT createToplistItem(result);
                return;
            }
        }
    }
}
