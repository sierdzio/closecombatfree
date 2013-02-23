/****************************************************************************
** Close Combat Free is a free, easily moddable CC-like game.
** Copyright (C) 2011 Tomasz Siekierda
** Email: sierdzio@gmail.com, website: http://www.sierdzio.com
**
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program (in doc/ directory).
** If not, see <http://www.gnu.org/licenses/gpl-3.0.html>.
****************************************************************************/

#include "ccferror.h"

/*!
  Constructor, sets error's state to "false".
  */
CcfError::CcfError()
{
    mErrorState = false;
}

/*!
  Returns true is object is in error state.
  */
bool CcfError::isErrorState() const
{
    return mErrorState;
}

/*!
  Returns error message.
  */
QString CcfError::errorMessage() const
{
    return mErrorMessage;
}

/*!
  Enters error state.

  Error message can be read with errorMessage(), and current state
  with isErrorState().

  \sa errorMessage, isErrorState
  */
void CcfError::enterErrorState(const QString &errorMsg)
{
    mErrorState = true;
    mErrorMessage = errorMsg;
}
