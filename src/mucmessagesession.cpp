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

#include "mucmessagesession_p.h"
#include "mucroom_p.h"
#include "client.h"

namespace Jreen
{
	MUCMessageSession::MUCMessageSession(MUCRoom *room) :
			MessageSession(MUCRoomPrivate::get(room)->client->messageSessionManager(), room->id())
	{
		m_room = MUCRoomPrivate::get(room);
	}
	
	void MUCMessageSession::setSubject(const QString &subject)
	{
		sendMessage(QString(), subject);
	}
	
	void MUCMessageSession::sendMessage(const QString &body, const QString &subject)
	{
		Message message(Message::Groupchat, jid(), body, subject);
		m_manager->send(message);
	}
	
	void MUCMessageSession::handleMessage(const Message &message)
	{
		m_room->handleMessage(message);
	}
}
