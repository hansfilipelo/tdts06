#include <QTcpSocket>
#include <QString>
#include <QByteArray>
#include <QDebug>
#include <QHostAddress>
#include <QHostInfo>

#include "Handler.h"

class RemoteCommunicator : public QObject {
  Q_OBJECT
public:
  RemoteCommunicator(QString address, Handler* parent, QObject* qtParent=nullptr);
  ~RemoteCommunicator();
  bool connectToServer();
  bool sendMessage(QString message);

private slots:
  void readyRead();
  void disconnected();

protected:
  Handler* handler;
  QTcpSocket* remoteSocket;
  QString hostInfo;
  bool socketDestroyed;
};
