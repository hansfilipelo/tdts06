#include "RemoteCommunicator.h"

// ----------- RemoteCommunicator ---------
// Class that handles communications with remote servers.
// ----------- Constructors ---------------

RemoteCommunicator::RemoteCommunicator(QString address, Handler* parent, QObject* qtParent) : QObject(qtParent){
  handler = parent;
  hostInfo = address;
  remoteSocket = new QTcpSocket(this);

  // Connect TCP-socket signals to slots in this object
  connect(remoteSocket,SIGNAL(disconnected()),this,SLOT(disconnected()));
  connect(remoteSocket,SIGNAL(readyRead()),this,SLOT(readyRead()));
}

// --------- Destructor ------------------

RemoteCommunicator::~RemoteCommunicator(){
  // On destruct - disconnect TCP socket so it doesn't call this thread when it deletes itself
  disconnect(this,SLOT(readyRead()));
  disconnect(this,SLOT(disconnected()));
  remoteSocket->deleteLater();
}


// --------------- Interfaces -------------
// Name explains that it connects to the remote server.

bool RemoteCommunicator::connectToServer(){

  remoteSocket->connectToHost(hostInfo,80);
  return remoteSocket->waitForConnected();
}

// ----
// Sends message to remote server

bool RemoteCommunicator::sendMessage(QString message){

  if(remoteSocket->state() == QTcpSocket::ConnectedState){

    remoteSocket->write(message.toUtf8());
    remoteSocket->flush();
    return remoteSocket->waitForBytesWritten();
  }

  qDebug() << "Socket not connected.";
  return false;
}

// ----
// Runs when there's data in the TCP buffer. Reads data from TCP socket.

void RemoteCommunicator::readyRead(){

  if(remoteSocket->state() == QTcpSocket::ConnectedState){
    QByteArray buffer;
    while (remoteSocket->bytesAvailable() > 0) {
      buffer += remoteSocket->readAll();
    }
    handler->ReceiveMessage(buffer);
  }
}

// ----
// Upon disconnect from server - close TCP socket and set it to delete later (with thread collapse)

void RemoteCommunicator::disconnected() {
  remoteSocket->close();
  remoteSocket->disconnect();
}
