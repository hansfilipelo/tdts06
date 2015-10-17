#ifndef Communicator_H
#define Communicator_H

#include <QThread>
#include <QObject>
#include <QDebug>
#include <QTcpServer>
#include <QTcpSocket>
#include <QVector>
#include <QByteArray>
#include "Handler.h"


class Server : public QTcpServer {
public:
  Server(const int inLocalPort, const QVector<QString> inBadWords, QObject *parent=nullptr) : QTcpServer(parent), localPort(inLocalPort), badWords(inBadWords){};
  void start();

protected:
  void incomingConnection(qintptr handle);
  const int localPort;
  const QVector<QString> badWords;
};

// ---------

class Communicator : public QThread {

  Q_OBJECT
public:
  Communicator(qintptr ID, int inLocalPort, const QVector<QString> *inBadWords, QObject *parent=nullptr);
  ~Communicator();
  void run();
  bool receiveMessage(const QByteArray message) const;

// Thread communicates over slots for messages, not functions
public slots:
  void readyRead();
  void disconnected();

protected:
  int localPort;
  const QVector<QString> *badWords;
  int socketDescriptor;
  QTcpSocket *localSocket;
  Handler* handler;
};


#endif
