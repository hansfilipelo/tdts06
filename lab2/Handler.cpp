#include "Handler.h"
#include "Communicator.h"
#include "RemoteCommunicator.h"


// --------------------HANDLER------------------------------
/*

Class that takes a look at messages passed in order to do censouring and strip Accept-Encoding from outgoing messages.

*/
//---------------------Constructors-------------------------
Handler::Handler(){};

Handler::Handler(const QVector<QString>* cen, Communicator* com): censur_vector(cen), localcom(com)
{}

Handler::Handler(const Handler& other)
{
  censur_vector = other.censur_vector;
  localcom = other.localcom;
}

Handler& Handler::operator=(const Handler& other)
{
  censur_vector = other.censur_vector;
  localcom = other.localcom;
  return *this;
}
//----------------------Destructor-------------------------

Handler::~Handler(){
  delete(remcom);
}

//----------------------Functionality----------------------
// Censour message

bool Handler::censur(const QString message)
{
  for(int i = 0; i < censur_vector->size(); i++)
    {
      if(message.contains((*censur_vector)[i], Qt::CaseInsensitive))
	     return true;
    }
  return false;
}

// ---------
// Debug function that prints the list of bad words.

void Handler::PrintBadWords()
{
  for(int i = 0; i < censur_vector->size(); i++)
    {
      std::cout << (*censur_vector)[i].toStdString() << std::endl;
    }
}

// ---------
// Create a RemoteConn object upon initiation

void Handler::CreateRemoteCom(const QString address)
{
  remcom = new RemoteCommunicator(address, this);
  if (!remcom->connectToServer()) {
    qDebug() << "Failed to connect to " + address;
  }
}

// ---------
// Send a message from client to server

void Handler::SendMessage(const QString message)
{
  QString address;
  QList<QString> splitMessage = message.split("\r\n");
  stripGZIP(splitMessage);

  address = GetAddress(splitMessage);
  CreateRemoteCom(address);

  if(!censur(message))
    {
      bool status = remcom->sendMessage(prepareMessageForSend(splitMessage));
    }
  else
    {
      QByteArray errorAnswer="HTTP/1.1 302 Moved Temporarily\r\nLocation: http://www.ida.liu.se/~TDTS04/labs/2011/ass2/error1.html\r\nConnection: Close\r\n\r\n";
      localcom->receiveMessage(errorAnswer);
      localcom->disconnect();
      //Ought we stop the socket?
    }
}

// ---------
// Read hostname of server on outgoing messages

QString Handler::GetAddress(QList<QString> message)
{
  QString hostName;

  for (QList<QString>::iterator headerLine = message.begin(); headerLine < message.end(); headerLine++){

    if (headerLine->contains("Host: ")){

      for (QString::iterator hostChar = headerLine->begin()+6; hostChar < headerLine->end(); hostChar++) {
        hostName += *hostChar;
      }
      return hostName;
    }
  }
  return QString("Can't find hostname");
}

// ---------
// Checks if the message is a message that should be censoured

bool Handler::censurAnswer(const QByteArray answerArray)
{
  QString answer = answerArray;

  if(!censurOn && answer.contains("text/"))
  {
    censurOn = true;
  }

  if(censurOn)
    {
      return censur(answer);
    }
  return false;
}

// ---------
// Receive message from remote server

void Handler::ReceiveMessage(const QByteArray answer)
{
  if(!censurAnswer(answer))
    {
      localcom->receiveMessage(answer);
    }
  else
    {
      QByteArray errorAnswer="HTTP/1.1 302 Moved Temporarily\r\nLocation: http://www.ida.liu.se/~TDTS04/labs/2011/ass2/error2.html\r\nConnection: Close\r\n\r\n";
      localcom->receiveMessage(errorAnswer);
      localcom->disconnect();
      qDebug() << "Censur";
      //Ought we stop the socket?
    }
}

 // ---------
/*
Strip outgoing messages so that we only accept clear text encoding and no compression.
This so that we easier can censour received content.

*/
void Handler::stripGZIP(QList<QString>& message){
  for (size_t i = 0; i < message.length(); i++) {
    QString temp = message.at(i);
    if (temp.contains("Accept-Encoding") | temp.contains("accept-encoding")) {
      message.removeAt(i);
      return;
    }
  }
}

// ---------
// Assemble a QString from a list of QStrings

const QString Handler::prepareMessageForSend(QList<QString> splitMessage){
  QString sendMessage;
  for (size_t i = 0; i < splitMessage.length(); i++) {
    sendMessage += splitMessage.at(i);
    sendMessage += "\r\n";
  }
  return sendMessage;
}
