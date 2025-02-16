/* SleepLib Day Class Header
 *
 * Copyright (c) 2019-2024 The OSCAR Team
 * Copyright (C) 2011-2018 Mark Watkins 
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License. See the file COPYING in the main directory of the source code
 * for more details. */

#ifndef DAY_H
#define DAY_H

#include "SleepLib/common.h"
#include "SleepLib/machine_common.h"
#include "SleepLib/machine.h"
#include "SleepLib/event.h"
#include "SleepLib/session.h"

/*! \class OneTypePerDay
    \brief An Exception class to catch multiple device records per day
    */
class OneTypePerDay
{
};

class Machine;
class Session;

/*! \class Day
    \brief Contains a list of all Sessions for single date, for a single device
    */
class Day
{
  public:
    Day();
    ~Day();

    //! \brief Add a new device to this day record
    bool addMachine(Machine *m);

    //! \brief Returns a device record if present of specified device type
    Machine *machine(MachineType type);

    //! \brief Returns a list of sessions for the specified device type
    QList<Session *> getSessions(MachineType type, bool ignore_enabled = false);

    //! \brief Add Session to this Day object (called during Load)
    void addSession(Session *s);

    EventDataType rangeCount(ChannelID code, qint64 st, qint64 et);
    EventDataType rangeSum(ChannelID code, qint64 st, qint64 et);
    EventDataType rangeAvg(ChannelID code, qint64 st, qint64 et);
    EventDataType rangeWavg(ChannelID code, qint64 st, qint64 et);
    EventDataType rangePercentile(ChannelID code, float p, qint64 st, qint64 et);

    //! \brief Returns the count of all this days sessions' events for this day
    EventDataType count(ChannelID code);

    //! \brief Returns the Minimum of all this sessions' events for this day
    EventDataType Min(ChannelID code);

    //! \brief Returns the Maximum of all sessions' events for this day
    EventDataType Max(ChannelID code);

    //! \brief Returns the Minimum of all this sessions' events for this day
    EventDataType physMin(ChannelID code);

    //! \brief Returns the Maximum of all sessions' events for this day
    EventDataType physMax(ChannelID code);

    //! \brief Returns the Count-per-hour of all sessions' events for this day
    EventDataType cph(ChannelID code);

    //! \brief Returns the Sum-per-hour of all this sessions' events for this day
    EventDataType sph(ChannelID code);

    //! \brief Returns (and caches) the 90th Percentile of all this sessions' events for this day
    EventDataType p90(ChannelID code);

    //! \brief Returns the Average of all this sessions' events for this day
    EventDataType avg(ChannelID code);

    //! \brief Returns the Sum of all this sessions' events for this day
    EventDataType sum(ChannelID code);

    //! \brief Returns the Time-Weighted Average of all this sessions' events for this day
    EventDataType wavg(ChannelID code);

    //! \brief Returns a requested Percentile of all this sessions' events for this day
    EventDataType percentile(ChannelID code, EventDataType percentile);

    //! \brief Returns if the cache contains SummaryType information about the requested code
    bool hasData(ChannelID code, SummaryType type);

    //! \brief Returns true if Day has specific device type
    inline bool hasMachine(MachineType mt) const { return machines.contains(mt); }

    //! \brief Returns true if Day has specific device record
    bool hasMachine(Machine * mach);

    //! \brief Returns true if any sessions have records matching specific device type
    bool searchMachine(MachineType mt);

    //! \brief Removes any lingering references to a specific device record and emits a warning if there were any
    void removeMachine(Machine * mach);

    //! \brief Returns the Average of all Sessions setting 'code' for this day
    EventDataType settings_avg(ChannelID code);

    //! \brief Returns the Time-Weighted Average of all Sessions setting 'code' for this day
    EventDataType settings_wavg(ChannelID code);

    //! \brief Returns the Sum of all Sessions setting 'code' for this day
    EventDataType settings_sum(ChannelID code);

    //! \brief Returns the Minimum of all Sessions setting 'code' for this day
    EventDataType settings_min(ChannelID code);

    //! \brief Returns the Maximum of all Sessions setting 'code' for this day
    EventDataType settings_max(ChannelID code);

    //! \brief Returns the amount of time (in decimal minutes) the Channel spent above the threshold
    EventDataType timeAboveThreshold(ChannelID code, EventDataType threshold);

    //! \brief Returns the amount of time (in decimal minutes) the Channel spent below the threshold
    EventDataType timeBelowThreshold(ChannelID code, EventDataType threshold);

    //! \brief Returns the value for Channel code at a given time
    EventDataType lookupValue(ChannelID code, qint64 time, bool square);

    //! \brief Returns the count of code events inside span flag event durations
    EventDataType countInsideSpan(ChannelID span, ChannelID code);


    //! \brief Returns the first session time of this day
    qint64 first();

    //! \brief Returns the last session time of this day
    qint64 last();

    //! \brief Returns the first session time of this device type for this day
    qint64 first(MachineType type);

    //! \brief Returns the last session time of this device type for this day
    qint64 last(MachineType type);


    // //! \brief Sets the first session time of this day
    // void setFirst(qint64 val) { d_first=val; }

    // //! \brief Sets the last session time of this day
    // void setLast(qint64 val) { d_last=val; }

    //! \brief Returns the last session time of this day for the supplied Channel code
    qint64 first(ChannelID code);

    //! \brief Returns the last session time of this day for the supplied Channel code
    qint64 last(ChannelID code);

    //! \brief Returns the total time in milliseconds for this day
    qint64 total_time();

    //! \brief Returns the total time in milliseconds for this day for given device type
    qint64 total_time(MachineType type);

    //! \brief Returns true if this day has enabled sessions for supplied device type
    bool hasEnabledSessions(MachineType);

    //! \brief Returns true if this day has enabled sessions
    bool hasEnabledSessions();

    //! \brief Return the total time in decimal hours for this day
    EventDataType hours() {
        if (!d_invalidate) return d_hours;
        d_invalidate = false;
        return d_hours = double(total_time()) / 3600000.0;
    }
    EventDataType hours(MachineType type) {
        auto it = d_machhours.find(type);
        if (it == d_machhours.end()) {
            return d_machhours[type] = double(total_time(type)) / 3600000.0;
        }
        return it.value();
    }

    //! \brief Return the session indexed by i
    Session *operator [](int i) { return sessions[i]; }

    //! \brief Return the first session as a QVector<Session*>::iterator
    QList<Session *>::iterator begin() { return sessions.begin(); }
    //! \brief Return the end session record as a QVector<Session*>::iterator
    QList<Session *>::iterator end() { return sessions.end(); }

    //! \brief Check if day contains SummaryOnly records
    bool summaryOnly(Machine * mach = nullptr);

    //! \brief Check if day has missing Summary/Settings records
    bool noSettings(Machine * mach = nullptr);

    //! \brief Finds and returns the index of a session, otherwise -1 if it's not there
    int find(Session *sess) { return sessions.indexOf(sess); }

    Session *find(SessionID sessid);

    Session *find(SessionID sessid, MachineType mt);

    //! \brief Returns the number of Sessions in this day record
    int size() { return sessions.size(); }

    //! \brief Loads all Events files for this Days Sessions
    void OpenEvents();
    void OpenSummary();


    //! \brief Closes all Events files for this Days Sessions
    void CloseEvents();

    //! \brief Get the ChannelID to be used for reporting pressure
    ChannelID getPressureChannelID();

    //! \brief Returns true if this Day contains loaded Event Data for this channel.
    bool channelExists(ChannelID id);

    //! \brief Returns true if session events are loaded
    bool eventsLoaded();

    //! \brief Returns true if this Day contains loaded Event Data or a cached count for this channel
    bool channelHasData(ChannelID id);

    //! \brief Returns true if this day contains the supplied settings Channel id
    bool settingExists(ChannelID id);

    //! \brief Removes a session from this day
    bool removeSession(Session *sess);

    //! \brief Returns a list of channels of supplied types, according to channel orders
    QList<ChannelID> getSortedMachineChannels(quint32 chantype);

    //! \brief Returns a list of device specific channels of supplied types, according to channel orders
    QList<ChannelID> getSortedMachineChannels(MachineType type, quint32 chantype);

    // Some ugly CPAP specific stuff
    int getCPAPMode();
    QString getCPAPModeStr();
    QString getPressureRelief();
    QString getPressureSettings();
    QString validPressure(float pressure);

    // Some more very much CPAP only related stuff

    //! \brief Calculate AHI (Apnea Hypopnea Index)
    EventDataType calcAHI() {
        EventDataType c = count(AllAhiChannels);
        EventDataType minutes = hours(MT_CPAP) * 60.0;
        return (c * 60.0) / minutes;
    }

    //! \brief Calculate RDI (Respiratory Disturbance Index)
    EventDataType calcRDI() {
        EventDataType c = count(AllAhiChannels) + count(CPAP_RERA);
        EventDataType minutes = hours(MT_CPAP) * 60.0;
        return (c * 60.0) / minutes;
    }

    //! \brief Percent of night for specified channel
    EventDataType calcPON(ChannelID code) {
        EventDataType c = sum(code);
        EventDataType minutes = hours(MT_CPAP) * 60.0;

        return (100.0 / minutes) * (c / 60.0);
    }

    //! \brief Calculate index (count per hour) for specified channel
    EventDataType calcIdx(ChannelID code) {
        EventDataType c = count(code);
        EventDataType minutes = hours(MT_CPAP) * 60.0;

        return (c * 60.0) / minutes;
    }

    //! \brief SleepyyHead Events Index, AHI combined with OSCAR detected events.. :)
    EventDataType calcSHEI() {
        EventDataType c = count(AllAhiChannels) + count(CPAP_UserFlag1) + count(CPAP_UserFlag2);
        EventDataType minutes = hours(MT_CPAP) * 60.0;
        return (c * 60.0) / minutes;
    }
    //! \brief Total duration of all Apnea/Hypopnea events in seconds,
    EventDataType calcTTIA() {
        EventDataType c = sum(AllAhiChannels);
        return c;
    }
    bool hasEvents();

    // According to preferences..
    EventDataType calcMiddle(ChannelID code);
    EventDataType calcMax(ChannelID code);
    EventDataType calcPercentile(ChannelID code);
    static QString calcMiddleLabel(ChannelID code);
    static QString calcMaxLabel(ChannelID code);
    static QString calcPercentileLabel(ChannelID code);

    EventDataType calc(ChannelID code, ChannelCalcType type);

    Session * firstSession(MachineType type);

    //! \brief A QList containing all Sessions objects for this day
    QList<Session *> sessions;

    QHash<MachineType, Machine *> machines;

    void incUseCounter() { d_useCounter++; }
    void decUseCounter() { d_useCounter--; if (d_useCounter<0) d_useCounter = 0; }
    int useCounter() { return d_useCounter; }


    void invalidate() {
        d_invalidate = true;
        d_machhours.clear();
    }

    void updateCPAPCache();

    inline QDate date() const { return d_date; }
    void setDate(QDate date) { d_date = date; }
  protected:



    QHash<ChannelID, QHash<EventDataType, EventDataType> > perc_cache;
    //qint64 d_first,d_last;
  private:
    bool d_firstsession;
    int d_useCounter;
    bool d_summaries_open;
    bool d_events_open;
    float d_hours;
    QHash<MachineType, EventDataType> d_machhours;
    QHash<ChannelID, long> d_count;
    QHash<ChannelID, double> d_sum;
    bool d_invalidate;
    QDate d_date;
};


#endif // DAY_H
