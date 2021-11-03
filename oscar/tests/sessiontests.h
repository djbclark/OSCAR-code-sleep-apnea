/* Session Testing Support
 *
 * Copyright (c) 2019-2022 The OSCAR Team
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License. See the file COPYING in the main directory of the source code
 * for more details. */

#ifndef SESSIONTESTS_H
#define SESSIONTESTS_H

#include "../SleepLib/session.h"

void SessionToYaml(QString filepath, Session* session, bool ok);

#endif // SESSIONTESTS_H
