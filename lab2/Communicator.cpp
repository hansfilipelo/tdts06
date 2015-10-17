#include "Communicator.h"

// -------- SERVER -------------------------
// Class that listens for incomming connections from browsers and initiates Communcator class
// who handles transfer of data.

// Starts the server who listens for incomming connections from browsers
void Server::start()
{
  if(!this->listen(QHostAddress::Any, localPort))
  {
    qDebug() <<"could not start server";
    exit(10);
  }
}

// --------
// Each time an incoming call arrives from a browser - this function creates and spawns a new thread

void Server::incomingConnection(qintptr handle)
{
  Communicator *newCall = new Communicator(handle, localPort, &badWords, this);

  connect(newCall, SIGNAL(finished()), newCall, SLOT(quit()));
  connect(newCall,SIGNAL(finished()),newCall,SLOT(deleteLater()));

  newCall->start();
}


// -------- COMMUNICATOR -------------------
// Class who spawns a thread and handles communications with browser.

Communicator::Communicator(qintptr ID, int inLocalPort, const QVector<QString>* inBadWords, QObject *parent) : QThread(parent)
{
  this->socketDescriptor = ID;

  badWords = inBadWords;
  localPort = inLocalPort;

  handler = new Handler(badWords,this);
}

// Destructor --------

Communicator::~Communicator(){
  // On destruct - disconnect TCP socket so it doesn't call this thread when it deletes itself
  disconnect(this,SLOT(readyRead()));
  disconnect(this,SLOT(disconnected()));
  localSocket->deleteLater();
}

// --------
// A new thread starts here

void Communicator::run() //thread starts here
{
  // Create a new TCP socket for each call
  localSocket = new QTcpSocket();

  // Check if socket is open
  if(!localSocket->setSocketDescriptor(this->socketDescriptor))
  {
    qDebug() << "Unable to open socket";
    exit(11);
  }

  // Set up so that readyRead and Disconnect signals are connected to the threads signals with the same name
  connect(localSocket,SIGNAL(readyRead()),this,SLOT(readyRead()),Qt::DirectConnection);
  connect(localSocket,SIGNAL(disconnected()),this,SLOT(disconnected()),Qt::DirectConnection);

  //Starts thread event handling
  exec();
}

// ---
// Reads data when there's data in the TCP buffer

void Communicator::readyRead(){

  QByteArray buffer;
  while (localSocket->bytesAvailable() > 0) {
    buffer += localSocket->readAll();
  }
  handler->SendMessage(buffer);
}

// ---
// Disconnects the socket when browser sends message to close() the TCP connection. Then exits so that the thread destroys itself.

void Communicator::disconnected(){
  localSocket->close();
  delete(handler);
  sleep(3); // Wait for QTcpSocket to time out and delete itself.
  this->quit(); // <-- Kill thread
}

// ------
// Receive stuff from server and forward to client

bool Communicator::receiveMessage(QByteArray message) const {

  if(localSocket->state() == QTcpSocket::ConnectedState){
    localSocket->write(message);
    localSocket->flush();
    return localSocket->waitForBytesWritten();
  }

  qDebug() << "Socket not connected.";
  return false;
}
