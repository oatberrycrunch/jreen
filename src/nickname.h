/****************************************************************************
 *  nickname.h
 *
 *  Copyright (c) 2009 by Nigmatullin Ruslan <euroelessar@gmail.com>
 *
 ***************************************************************************
 *                                                                         *
 *   This library is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*****************************************************************************/

#ifndef NICKNAME_H
#define NICKNAME_H

#include "stanzaextension.h"

namespace jreen
{

class Nickname : public StanzaExtension
{
	J_EXTENSION(jreen::Nickname,
		   "/presence/nick[@xmlns='http://jabber.org/protocol/nick']"
		   "|/message/nick[@xmlns='http://jabber.org/protocol/nick']")
public:
	Nickname(const QDomElement &node = QDomElement());
	Nickname(const QString &nick);
	const QString &nick() { return m_nick; }
	QDomElement node(QDomDocument *doc) const;
private:
	QString m_nick;
};

}

#endif // NICKNAME_H