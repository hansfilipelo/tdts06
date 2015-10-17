#ifndef LocalHandler_H
#define LocalHandler_H

#include <iostream>

#include <QVector>
#include <QString>
#include <QByteArray>

class Communicator;
class RemoteCommunicator;

class Handler
{
 public:
  //Constructors
  Handler();
  Handler(const QVector<QString>*, Communicator* localcom);
  Handler(const Handler&);
  Handler& operator=(const Handler&);

  //Destructor
  ~Handler();

 private:
  //Variables
  const QVector<QString>* censur_vector;
  Communicator* localcom;
  RemoteCommunicator* remcom;
  bool censurOn = false;

 public:
  bool censur(const QString message);
  void PrintBadWords();
  void CreateRemoteCom(const QString address);
  void SendMessage(const QString message);
  QString GetAddress(QList<QString> message);
  bool censurAnswer(QByteArray answer);
  void ReceiveMessage(const QByteArray message);
  void stripGZIP(QList<QString>& message);
  const QString prepareMessageForSend(QList<QString> splitMessage);
};


#endif
