/****************************************************************************
**
** Jreen
**
** Copyright © 2011 Ruslan Nigmatullin <euroelessar@yandex.ru>
**
*****************************************************************************
**
** $JREEN_BEGIN_LICENSE$
** Jreen is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** Jreen is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with Jreen.  If not, see <http://www.gnu.org/licenses/>.
** $JREEN_END_LICENSE$
**
****************************************************************************/

#include "datastream.h"

namespace Jreen
{
	
	DataStream::DataStream() : m_device(0)
	{
	}
	
	DataStream::~DataStream()
	{
	}
	
	void DataStream::setDevice(QIODevice *device)
	{
		if (m_device)
			disconnect(m_device, 0, this, 0);
		m_device = device;
		connect(m_device, SIGNAL(readyRead()), SLOT(incomingDataReady()));
	}
	
	QIODevice *DataStream::device()
	{
		return m_device;
	}
	
}
