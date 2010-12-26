/****************************************************************************
 *
 *  This file is part of qutIM
 *
 *  Copyright (c) 2010 by Nigmatullin Ruslan <euroelessar@gmail.com>
 *
 ***************************************************************************
 *                                                                         *
 *   This file is part of free software; you can redistribute it and/or    *
 *   modify it under the terms of the GNU General Public License as        *
 *   published by the Free Software Foundation; either version 2 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 ***************************************************************************
 ****************************************************************************/

#include "buffereddatastream.h"
#include "client.h"
#include <QBasicTimer>
#include <QTimerEvent>
#include <QDebug>

namespace jreen
{
class BufferedDataStreamPrivate
{
public:
	QList<XmlStreamHandler *> *handlers;
	QByteArray buffer;
	int offset;
	int len;
	QByteArray outBuffer;
	QBasicTimer timer;
	void ensureSize(int size)
	{
		if (buffer.size() - offset - len < size)
			buffer.resize(offset + len + size);
	}
};

BufferedDataStream::BufferedDataStream(QList<XmlStreamHandler *> *handlers) :
		d_ptr(new BufferedDataStreamPrivate)
{
	Q_D(BufferedDataStream);
	d->offset = 0;
	d->len = 0;
	d->handlers = handlers;
}

BufferedDataStream::~BufferedDataStream()
{
}

qint64 BufferedDataStream::bytesAvailable() const
{
	Q_D(const BufferedDataStream);
	return d->len + QIODevice::bytesAvailable();
}

bool BufferedDataStream::open(OpenMode mode)
{
	QIODevice::open(mode);
	return true;
}

void BufferedDataStream::close()
{
	flush();
}

void BufferedDataStream::timerEvent(QTimerEvent *event)
{
	if (event->timerId() == d_func()->timer.timerId()) {
		flush();
		d_func()->timer.stop();
		return;
	}
	return QIODevice::timerEvent(event);
}

void BufferedDataStream::incomingDataReady()
{
	Q_D(BufferedDataStream);
	int bytes = device()->bytesAvailable();
	d->ensureSize(bytes);
	device()->read(d->buffer.data() + d->offset + d->len, bytes);
//	qDebug("< \"%s\"", QByteArray(d->buffer.constData() + d->offset + d->len, bytes).constData());
	d->len += bytes;
	emit readyRead();
}

qint64 BufferedDataStream::writeData(const char *data, qint64 len)
{
	Q_D(BufferedDataStream);
	if (len <= 0)
		return 0;
	d->outBuffer.append(data, len);
	if (!d->timer.isActive())
		d->timer.start(0, this);
	return len;
}

void BufferedDataStream::flush()
{
	Q_D(BufferedDataStream);
	foreach (XmlStreamHandler *handler, *d->handlers)
		handler->handleOutgoingData(d->outBuffer.constData(), d->outBuffer.size());
	device()->write(d->outBuffer.constData(), d->outBuffer.size());
//	qDebug("> \"%s\"", d->outBuffer.constData());
	d->outBuffer.clear();
}

qint64 BufferedDataStream::readData(char *data, qint64 maxlen)
{
	Q_D(BufferedDataStream);
	int len = qMin<int>(maxlen, d->len);
	qMemCopy(data, d->buffer.data() + d->offset, len);
	foreach (XmlStreamHandler *handler, *d->handlers)
		handler->handleIncomingData(d->buffer.data() + d->offset, len);
	if (maxlen < d->len) {
		d->len -= maxlen;
		d->offset += maxlen;
	} else {
		d->offset = 0;
		d->len = 0;
		d->buffer.resize(qMin(1024, d->buffer.size()));
		d->buffer.squeeze();
	}
	return len;
}
}
