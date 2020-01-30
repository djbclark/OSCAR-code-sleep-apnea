/* SleepLib Dreem Loader Header
 *
 * Copyright (c) 2020 The OSCAR Team
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License. See the file COPYING in the main directory of the source code
 * for more details. */

#ifndef DREEMLOADER_H
#define DREEMLOADER_H

#include "SleepLib/machine_loader.h"

const QString dreem_class_name = "Dreem";
const int dreem_data_version = 2;


/*! \class DreemLoader
*/
class DreemLoader : public MachineLoader
{
  public:
    DreemLoader() { m_type = MT_SLEEPSTAGE; }
    virtual ~DreemLoader() { }

    virtual bool Detect(const QString & path);

    virtual int Open(const QString & path);
    virtual int OpenFile(const QString & path);
    static void Register();

    virtual int Version() { return dreem_data_version; }
    virtual const QString &loaderName() { return dreem_class_name; }

    virtual MachineInfo newInfo() {
        return MachineInfo(MT_SLEEPSTAGE, 0, dreem_class_name, QObject::tr("Dreem"), QString(), QString(), QString(), QObject::tr("Dreem"), QDateTime::currentDateTime(), dreem_data_version);
    }
    
    bool openCSV(const QString & filename);
    void closeCSV();
    Session* readNextSession();

  protected:
    QDateTime readDateTime(const QString & text);
    int readDuration(const QString & text);
    int readInt(const QString & text);

  private:
    QFile file;
    class CSVReader* csv;
    Machine *mach;
    bool invalid_fields;
};

#endif // DREEMLOADER_H
