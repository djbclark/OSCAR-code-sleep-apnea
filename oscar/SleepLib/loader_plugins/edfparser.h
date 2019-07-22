/* EDF Parser Header
 *
 * Copyright (c) 2019 The OSCAR Team
 * Copyright (c) 2011-2018 Mark Watkins <mark@jedimark.net>
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License. See the file COPYING in the main directory of the source code
 * for more details. */

#ifndef EDFPARSER_H
#define EDFPARSER_H

#include <QString>
#include <QVector>
#include <QHash>
#include <QList>

#include "SleepLib/common.h"

const QString STR_ext_EDF = "edf";
const QString STR_ext_gz = ".gz";

const char AnnoSep = 20;
const char AnnoDurMark = 21;
const char AnnoEnd = 0;

/*! \struct EDFHeader
    \brief  Represents the EDF+ header structure, used as a place holder while processing the text data.
    \note More information on the EDF+ file format can be obtained from http://edfplus.info
*/
struct EDFHeaderRaw {
    char version[8];
    char patientident[80];
    char recordingident[80];
    char datetime[16];
    char num_header_bytes[8];
    char reserved[44];
    char num_data_records[8];
    char dur_data_records[8];
    char num_signals[4];
}
#ifndef _MSC_VER
__attribute__((packed))
#endif
;
const int EDFHeaderSize = sizeof(EDFHeaderRaw);

/*! \struct EDFHeaderQT
    \brief Contains the QT version of the EDF header information
    */
struct EDFHeaderQT {
  public:
    long version;
    QString patientident;
    QString recordingident;
    QDateTime startdate_orig;
    long num_header_bytes;
    QString reserved44;
    long num_data_records;
    long duration_Seconds;
    long num_signals;
};

/*! \struct EDFSignal
    \brief Contains information about a single EDF+ Signal
    \note More information on the EDF+ file format can be obtained from http://edfplus.info
    */
struct EDFSignal {
  public:
    QString label; //! \brief Name of this Signal
    QString transducer_type; //! \brief Tranducer Type (source of the data, usually blank)
    QString physical_dimension; //! \brief The units of measurements represented by this signal
    EventDataType physical_minimum; //! \brief The minimum limits of the ungained data
    EventDataType physical_maximum; //! \brief The maximum limits of the ungained data
    EventDataType digital_minimum; //! \brief The minimum limits of the data with gain and offset applied
    EventDataType digital_maximum; //! \brief The maximum limits of the data with gain and offset applied
    EventDataType gain; //! \brief Raw integer data is multiplied by this value
    EventDataType offset; //! \brief This value is added to the raw data
    QString prefiltering; //! \brief Any prefiltering methods used (usually blank)
    long nr; //! \brief Number of records
    QString reserved; //! \brief Reserved (usually blank)
    qint16 *value; //! \brief Pointer to the signals sample data
    int pos; //! \brief a non-EDF extra used internally to count the signal data
};


/*! \class EDFParser
    \author Mark Watkins <mark@jedimark.net>
    \brief Parse an EDF+ data file into a list of EDFSignal's
    \note More information on the EDF+ file format can be obtained from http://edfplus.info
    */
class EDFParser
{
  public:
    //! \brief Constructs an EDFParser object, opening the filename if one supplied
    EDFParser(QString filename = "");

    ~EDFParser();

    //! \brief Open the EDF+ file, and read it's header
    bool Open(const QString & name);

    //! \brief Parse the EDF+ file into the list of EDFSignals.. Must be call Open(..) first.
    bool Parse();

    //! \brief Read n bytes of 8 bit data from the EDF+ data stream
    QString Read(unsigned n);

    //! \brief Read 16 bit word of data from the EDF+ data stream
    qint16 Read16();

    //! \brief Return a ptr to the i'th signal with the given name (if multiple signal with the same name(?!))
    EDFSignal *lookupLabel(const QString & name, int index=0);

    //! \brief Returns the number of signals contained in this EDF file
    long GetNumSignals() { return edfHdr.num_signals; }

    //! \brief Returns the number of data records contained per signal.
    long GetNumDataRecords() { return edfHdr.num_data_records; }

    //! \brief Returns the duration represented by this EDF file (in milliseconds)
    qint64 GetDuration() { return dur_data_record; }

    //! \brief Returns the patientid field from the EDF header
    QString GetPatient() { return edfHdr.patientident; }

//  The data members follow

    //! \brief The header in a QT friendly form
    EDFHeaderQT edfHdr;

    //! \brief Vector containing the list of EDFSignals contained in this edf file
    QVector<EDFSignal> edfsignals;

    //! \brief An by-name indexed into the EDFSignal data
    QStringList signal_labels;

    //! \brief ResMed sometimes re-uses the SAME signal name
    QHash<QString, QList<EDFSignal *> > signalList;

    //! \brief The following are computed from the edfHdr data
    QString serialnumber;
    qint64 dur_data_record;
    qint64 startdate;
    qint64 enddate;

//  the following could be private
    //! \brief This is the array holding the EDF file data
    QByteArray fileData;
    //! \brief  The EDF+ files header structure, used as a place holder while processing the text data.
    EDFHeaderRaw *hdrPtr;
    //! \brief This is the array of signal descriptors and values
    char *signalPtr;

    QString filename;
    long filesize;
    long datasize;
    long pos;
    bool eof;
};


#endif // EDFPARSER_H
