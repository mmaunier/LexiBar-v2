#ifndef GESTIONLEXIBAR_H
#define GESTIONLEXIBAR_H

#include <QObject>
#include <QString>
#include <QClipboard>
#include <QGuiApplication>

class GestionLexibar : public QObject
{
   Q_OBJECT

public:
   explicit GestionLexibar(QObject *parent = nullptr);

public slots:
   void copyToClipboard(const QString &text);
   bool startDrag(const QString &text);

signals:
   void characterCopied();
};

#endif // GESTIONLEXIBAR_H