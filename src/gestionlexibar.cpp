#include "gestionlexibar.h"
#include <QDrag>
#include <QMimeData>
#include <QGuiApplication>

GestionLexibar::GestionLexibar(QObject *parent)
    : QObject(parent)
{
}

void GestionLexibar::copyToClipboard(const QString &text)
{
   QClipboard *clipboard = QGuiApplication::clipboard();
   clipboard->setText(text);
   emit characterCopied();
}

bool GestionLexibar::startDrag(const QString &text)
{
   QMimeData *mimeData = new QMimeData;
   mimeData->setText(text);

   QDrag *drag = new QDrag(this);
   drag->setMimeData(mimeData);

   Qt::DropAction dropAction = drag->exec(Qt::CopyAction);
   return dropAction == Qt::CopyAction;
}