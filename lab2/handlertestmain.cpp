#include <QtCore/QCoreApplication>
#include <QCommandLineParser>
#include <QVector>
#include <QString>
#include <QDebug>
#include <fstream>
#include <iostream>
#include <string>
#include <QFile>
#include "Handler.h"

using namespace std;

int printHelp(QString scriptName){
  qDebug() << " ";
  qDebug() << "proxYServer built in help";
  qDebug() << "";
  qDebug() << scriptName + " [-p, --port localport (60000)] | [-c fileWithWordsNotAllowed (empty)]";
  qDebug() << "";

  exit(1);
}

int main(int argc, char *argv[])
{
  QCoreApplication app(argc,argv);
  QCoreApplication::setApplicationName("proxYserver");

  int localPort = 60000;
  QVector<QString> unAllowedWords;

  // All of this is argument handling
  // --------------------------------
  QVector<QString> arguments;

  for (int i = 0; i < argc; i++){
    QString temp = argv[i];
    arguments.append(temp);
  }

  // See if user WANTS help
  if(arguments.contains("-h") || arguments.contains("--help") ){
    printHelp(QString(argv[0]));
  }
  // ----

  // Port
  if(arguments.contains("-p")){
    QString portTmp = arguments.at(arguments.indexOf("-p")+1);
    bool portIsNumber;
    localPort = portTmp.toInt(&portIsNumber);
    if(!portIsNumber){
      printHelp(QString(argv[0]));
    }
  }
  else if (arguments.contains("--port")) {
    QString portTmp = arguments.at(arguments.indexOf("--port")+1);
    bool portIsNumber;
    localPort = portTmp.toInt(&portIsNumber);
    if(!portIsNumber){
      printHelp(QString(argv[0]));
    }
  }
  // ------

  if (arguments.contains("-c")) {
    QString fileName = arguments.at(arguments.indexOf("-c")+1);
    QFile wordFile(fileName);

    if(!wordFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
      qDebug() << "Error - wordFile not readable!";
      printHelp(QString(argv[0]));
    }

    QTextStream in(&wordFile);

    while (!in.atEnd()) {
      QString badWord = in.readLine();
      unAllowedWords.append(badWord);
    }
  }
  // --------------------------------

  Handler test_handler(&unAllowedWords);
  Handler copy_test;
  test_handler.PrintBadWords();
  cout << "Test" << endl;
  copy_test = test_handler;
  copy_test.PrintBadWords();
  if(test_handler.censur("rumpa"))
    cout << "Vector contains rumpa" << endl;

  if(test_handler.censur("RUMPA"))
    cout << "Vector contains RUMPA" << endl;

  if(!test_handler.censur("MAI"))
    cout << "Vector do not contain MAI" << endl;

  //---------------------------------
  // Printing to show available variables
  qDebug() << " ";
  qDebug() << QString("Words not allowed: ");
  while (!unAllowedWords.isEmpty()) {
    qDebug() << unAllowedWords.first();
    unAllowedWords.pop_front();
  }
  qDebug() << " ";
  qDebug() << "Port: ";
  qDebug() << localPort;
  qDebug() << " ";

  
  

  return 0;
  //return app.exec();
}
