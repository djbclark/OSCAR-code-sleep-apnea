/* Session Testing Support
 *
 * Copyright (c) 2019-2020 The OSCAR Team
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License. See the file COPYING in the main directory of the source code
 * for more details. */

#include <QFile>
#include "sessiontests.h"

static QString ts(qint64 msecs)
{
    // TODO: make this UTC so that tests don't vary by where they're run
    return QDateTime::fromMSecsSinceEpoch(msecs).toString(Qt::ISODate);
}

static QString hex(int i)
{
    return QString("0x") + QString::number(i, 16).toUpper();
}

static QString dur(qint64 msecs)
{
    qint64 s = msecs / 1000L;
    int h = s / 3600; s -= h * 3600;
    int m = s / 60; s -= m * 60;
    return QString("%1:%2:%3")
        .arg(h, 2, 10, QChar('0'))
        .arg(m, 2, 10, QChar('0'))
        .arg(s, 2, 10, QChar('0'));
}

#define ENUMSTRING(ENUM) case ENUM: s = #ENUM; break
static QString eventListTypeName(EventListType t)
{
    QString s;
    switch (t) {
        ENUMSTRING(EVL_Waveform);
        ENUMSTRING(EVL_Event);
        default:
            s = hex(t);
            qDebug() << "EVL" << qPrintable(s);
    }
    return s;
}

// ChannelIDs are not enums. Instead, they are global variables of the ChannelID type.
// This allows definition of different IDs within different loader plugins, while
// using Qt templates (such as QHash) that require a consistent data type for their key.
//
// Ideally there would be a central ChannelID registry class that could be queried
// for names, make sure there aren't duplicate values, etc. For now we just fill the
// below by hand.
#define CHANNELNAME(CH) if (i == CH) { s = #CH; break; }
extern ChannelID PRS1_TimedBreath, PRS1_HumidMode, PRS1_TubeTemp;

static QString settingChannel(ChannelID i)
{
    QString s;
    do {
        CHANNELNAME(CPAP_Mode);
        CHANNELNAME(CPAP_Pressure);
        CHANNELNAME(CPAP_PressureMin);
        CHANNELNAME(CPAP_PressureMax);
        CHANNELNAME(CPAP_EPAP);
        CHANNELNAME(CPAP_IPAP);
        CHANNELNAME(CPAP_PS);
        CHANNELNAME(CPAP_EPAPLo);
        CHANNELNAME(CPAP_EPAPHi);
        CHANNELNAME(CPAP_IPAPLo);
        CHANNELNAME(CPAP_IPAPHi);
        CHANNELNAME(CPAP_PSMin);
        CHANNELNAME(CPAP_PSMax);
        CHANNELNAME(CPAP_RampTime);
        CHANNELNAME(CPAP_RampPressure);
        CHANNELNAME(CPAP_RespRate);
        CHANNELNAME(OXI_Pulse);
        CHANNELNAME(PRS1_FlexMode);
        CHANNELNAME(PRS1_FlexLevel);
        CHANNELNAME(PRS1_HumidStatus);
        CHANNELNAME(PRS1_HumidMode);
        CHANNELNAME(PRS1_TubeTemp);
        CHANNELNAME(PRS1_HumidLevel);
        CHANNELNAME(PRS1_SysLock);
        CHANNELNAME(PRS1_SysOneResistSet);
        CHANNELNAME(PRS1_SysOneResistStat);
        CHANNELNAME(PRS1_TimedBreath);
        CHANNELNAME(PRS1_HoseDiam);
        CHANNELNAME(PRS1_AutoOn);
        CHANNELNAME(PRS1_AutoOff);
        CHANNELNAME(PRS1_MaskAlert);
        CHANNELNAME(PRS1_ShowAHI);
        CHANNELNAME(CPAP_BrokenSummary);
        CHANNELNAME(ZEO_Awakenings);
        CHANNELNAME(ZEO_MorningFeel);
        CHANNELNAME(ZEO_TimeInWake);
        CHANNELNAME(ZEO_TimeInREM);
        CHANNELNAME(ZEO_TimeInLight);
        CHANNELNAME(ZEO_TimeInDeep);
        CHANNELNAME(ZEO_TimeToZ);
        CHANNELNAME(ZEO_ZQ);
        s = hex(i);
        qDebug() << "setting channel" << qPrintable(s);
    } while(false);
    return s;
}

static QString eventChannel(ChannelID i)
{
    QString s;
    do {
        CHANNELNAME(CPAP_Obstructive);
        CHANNELNAME(CPAP_Hypopnea);
        CHANNELNAME(CPAP_PB);
        CHANNELNAME(CPAP_LeakTotal);
        CHANNELNAME(CPAP_Leak);
        CHANNELNAME(CPAP_LargeLeak);
        CHANNELNAME(CPAP_IPAP);
        CHANNELNAME(CPAP_EPAP);
        CHANNELNAME(CPAP_PS);
        CHANNELNAME(CPAP_IPAPLo);
        CHANNELNAME(CPAP_IPAPHi);
        CHANNELNAME(CPAP_RespRate);
        CHANNELNAME(CPAP_PTB);
        CHANNELNAME(PRS1_TimedBreath);
        CHANNELNAME(CPAP_MinuteVent);
        CHANNELNAME(CPAP_TidalVolume);
        CHANNELNAME(CPAP_ClearAirway);
        CHANNELNAME(CPAP_FlowLimit);
        CHANNELNAME(CPAP_Snore);
        CHANNELNAME(CPAP_VSnore);
        CHANNELNAME(CPAP_VSnore2);
        CHANNELNAME(CPAP_NRI);
        CHANNELNAME(CPAP_RERA);
        CHANNELNAME(OXI_Pulse);
        CHANNELNAME(OXI_SPO2);
        CHANNELNAME(PRS1_BND);
        CHANNELNAME(CPAP_MaskPressureHi);
        CHANNELNAME(CPAP_FlowRate);
        CHANNELNAME(CPAP_Test1);
        CHANNELNAME(CPAP_Test2);
        CHANNELNAME(CPAP_PressurePulse);
        CHANNELNAME(CPAP_Pressure);
        CHANNELNAME(PRS1_0E);
        CHANNELNAME(CPAP_PressureSet);
        CHANNELNAME(CPAP_IPAPSet);
        CHANNELNAME(CPAP_EPAPSet);
        CHANNELNAME(POS_Movement);
        CHANNELNAME(ZEO_SleepStage);
        // Resmed-specific channels
        CHANNELNAME(CPAP_Apnea);
        CHANNELNAME(CPAP_MaskPressure);
        CHANNELNAME(CPAP_Te);
        CHANNELNAME(CPAP_Ti);
        CHANNELNAME(CPAP_IE);
        CHANNELNAME(CPAP_FLG);
        CHANNELNAME(CPAP_AHI);
        CHANNELNAME(CPAP_TgMV);
        s = hex(i);
        qDebug() << "event channel" << qPrintable(s);
    } while(false);
    return s;
}

static QString intList(EventStoreType* data, int count, int limit=-1)
{
    if (limit == -1 || limit > count) limit = count;
    int first = limit / 2;
    int last = limit - first;
    QStringList l;
    for (int i = 0; i < first; i++) {
        l.push_back(QString::number(data[i]));
    }
    if (limit < count) l.push_back("...");
    for (int i = count - last; i < count; i++) {
        l.push_back(QString::number(data[i]));
    }
    QString s = "[ " + l.join(",") + " ]";
    return s;
}

static QString intList(quint32* data, int count, int limit=-1)
{
    if (limit == -1 || limit > count) limit = count;
    int first = limit / 2;
    int last = limit - first;
    QStringList l;
    for (int i = 0; i < first; i++) {
        l.push_back(QString::number(data[i] / 1000));
    }
    if (limit < count) l.push_back("...");
    for (int i = count - last; i < count; i++) {
        l.push_back(QString::number(data[i] / 1000));
    }
    QString s = "[ " + l.join(",") + " ]";
    return s;
}

void SessionToYaml(QString filepath, Session* session, bool ok)
{
    QFile file(filepath);
    if (!file.open(QFile::WriteOnly | QFile::Truncate)) {
        qDebug() << filepath;
        Q_ASSERT(false);
    }
    QTextStream out(&file);

    out << "session:" << endl;
    out << "  id: " << session->session() << endl;
    out << "  start: " << ts(session->first()) << endl;
    out << "  end: " << ts(session->last()) << endl;
    out << "  valid: " << ok << endl;
    
    if (!session->m_slices.isEmpty()) {
        out << "  slices:" << endl;
        for (auto & slice : session->m_slices) {
            QString s;
            switch (slice.status) {
            case MaskOn: s = "mask on"; break;
            case MaskOff: s = "mask off"; break;
            case EquipmentOff: s = "equipment off"; break;
            default: s = "unknown"; break;
            }
            out << "  - status: " << s << endl;
            out << "    start: " << ts(slice.start) << endl;
            out << "    end: " << ts(slice.end) << endl;
        }
    }
    qint64 total_time = 0;
    if (session->first() != 0) {
        Day day;
        day.addSession(session);
        total_time = day.total_time();
        day.removeSession(session);
    }
    out << "  total_time: " << dur(total_time) << endl;

    out << "  settings:" << endl;

    // We can't get deterministic ordering from QHash iterators, so we need to create a list
    // of sorted ChannelIDs.
    QList<ChannelID> keys = session->settings.keys();
    std::sort(keys.begin(), keys.end());
    for (QList<ChannelID>::iterator key = keys.begin(); key != keys.end(); key++) {
        QVariant & value = session->settings[*key];
        QString s;
        if ((QMetaType::Type) value.type() == QMetaType::Float) {
            s = QString::number(value.toFloat());  // Print the shortest accurate representation rather than QVariant's full precision.
        } else {
            s = value.toString();
        }
        out << "    " << settingChannel(*key) << ": " << s << endl;
    }

    out << "  events:" << endl;

    keys = session->eventlist.keys();
    std::sort(keys.begin(), keys.end());
    for (QList<ChannelID>::iterator key = keys.begin(); key != keys.end(); key++) {
        out << "    " << eventChannel(*key) << ": " << endl;

        // Note that this is a vector of lists
        QVector<EventList *> &ev = session->eventlist[*key];
        int ev_size = ev.size();
        if (ev_size == 0) {
            continue;
        }
        EventList &e = *ev[0];
        
        // Multiple eventlists in a channel are used to account for discontiguous data.
        // See CoalesceWaveformChunks for the coalescing of multiple contiguous waveform
        // chunks and ParseWaveforms/ParseOximetry for the creation of eventlists per
        // coalesced chunk.
        //
        // This can also be used for other discontiguous data, such as PRS1 statistics
        // that are omitted when breathing is not detected.

        for (int j = 0; j < ev_size; j++) {
            e = *ev[j];
            out << "    - count: "  << (qint32)e.count() << endl;
            if (e.count() == 0)
                continue;
            out << "      first: " << ts(e.first()) << endl;
            out << "      last: " << ts(e.last()) << endl;
            out << "      type: " << eventListTypeName(e.type()) << endl;
            out << "      rate: " << e.rate() << endl;
            out << "      gain: " << e.gain() << endl;
            out << "      offset: " << e.offset() << endl;
            if (!e.dimension().isEmpty()) {
                out << "      dimension: " << e.dimension() << endl;
            }
            out << "      data:" << endl;
            out << "        min: " << e.Min() << endl;
            out << "        max: " << e.Max() << endl;
            out << "        raw: " << intList((EventStoreType*) e.m_data.data(), e.count(), 100) << endl;
            if (e.type() != EVL_Waveform) {
                out << "        delta: " << intList((quint32*) e.m_time.data(), e.count(), 100) << endl;
            }
            if (e.hasSecondField()) {
                out << "      data2:" << endl;
                out << "        min: " << e.min2() << endl;
                out << "        max: " << e.max2() << endl;
                out << "        raw: " << intList((EventStoreType*) e.m_data2.data(), e.count(), 100) << endl;
            }
        }
    }
    file.close();
}
