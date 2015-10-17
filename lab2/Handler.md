# Handler

External interfaces:

- Initiator(QString httpReq, QVector<QString> censorWords)
- print_bad_words()

Internal interfaces:

- bool censor(QString message, QVector<QString> wordsNotAllowed)
- void redirectLocal(QString message)
- void redirectRemote(QString message)
