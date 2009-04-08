///////////////////////////////////////////////////////////
//  Processer.cpp
//  Implementation of the Class Processer
//  Created on:      02-Apr-2009 18:10:13
//  Original author: Tim Kuo
///////////////////////////////////////////////////////////
#include "common.h"
#include "messagedef.h"
#include "Worker.h"
#include "Staff.h"
#include "ThreadDatabase.h"
#include <QThread>
#include "TEvent.h"
#include "Message.h"

Worker::Worker(QObject *parent )
:QObject(parent)
{
//	QEvent::registerEventType(WorkerEventType::Db);
//	QEvent::registerEventType(WorkerEventType::Net);
	m_databaseThread = new ThreadDatabase(this); 
	int r = connect(m_databaseThread, SIGNAL(hasEvent(Message &)), this, SLOT(IncomingEvent(Message &)));//, Qt::DirectConnection);
}



Worker::~Worker(){

}

void Worker::StartAction(Message& Action) {
	switch(Action.type()) {
		case ACTION_SYSTEM_START:
		{
			Message* ev = new Message(EVENT_SYSTEM_START);
			BroadcastEvent(*ev);
			delete ev;
			break;
		}
		case ACTION_STAFFMGNT:
		{
			Message* ev = new Message(EVENT_STAFFMGNT);
			BroadcastEvent(*ev);
			delete ev;
			break;
		}
		case ACTION_LOGOFF:
		{
			Message* ev = new Message(EVENT_LOGGEDOFF);
			BroadcastEvent(*ev);
			delete ev;
			break;
		}
		case ACTION_EXIT:
		{
			m_databaseThread->quit();
			Message* ev = new Message(EVENT_EXIT);
			BroadcastEvent(*ev);
			delete ev;
			break;
		}
		case ACTION_MAINMENU:
		{
			m_databaseThread->quit();
			Message* ev = new Message(EVENT_MAINMENU);
			BroadcastEvent(*ev);
			delete ev;
			break;
		}
		default:
		{
			m_databaseThread->QueueAction(Action);
			break;
		}
	}
}

void Worker::IncomingEvent(Message& ev) {
	BroadcastEvent(ev);
}

bool Worker::event ( QEvent * e ) {
	switch(e->type()) {
		case EventDb:
			{
				Message* ev = dynamic_cast<TEvent*>(e)->data();
				BroadcastEvent(*ev);
			}
	}
	return true;
}