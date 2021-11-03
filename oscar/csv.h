/* OSCAR CSV Reader Implementation
 *
 * Copyright (c) 2020-2022 The OSCAR Team
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License. See the file COPYING in the main directory of the source code
 * for more details. */

#include <QTextStream>

class CSVReader
{
public:
    CSVReader(QIODevice & stream, const QString & delim=",", const QString & comment=QString());
    virtual ~CSVReader() = default;
    
    QStringList readRow();
    virtual bool readRow(QStringList & fields);  // override this for more complicated processing
    void setFieldNames(QStringList & header);
    bool readRow(QHash<QString,QString> & row);

protected:
    QTextStream m_stream;
    QString m_delim;
    QString m_comment;
    QStringList m_field_names;
};

