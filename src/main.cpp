#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickWindow>
#include <QIcon>
#include "gestionlexibar.h"

int main(int argc, char *argv[])
{
   // Force l'utilisation d'échelle 1:1
   QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);

   QGuiApplication app(argc, argv);
   
   // Définir l'icône de l'application
   app.setWindowIcon(QIcon(":/media/drapeau.png"));

   // Créer une instance de GestionLexibar
   GestionLexibar lexibar;

   // Utiliser QQmlApplicationEngine
   QQmlApplicationEngine engine;

   // Exposer GestionLexibar au QML
   engine.rootContext()->setContextProperty("lexibar", &lexibar);

   // Charger le fichier QML
   const QUrl url(QStringLiteral("qrc:/qml/main.qml"));

   QObject::connect(&engine, &QQmlApplicationEngine::objectCreated, &app, [&url](QObject *obj, const QUrl &objUrl)
                    {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
        
        // Ajuster précisément les marges de la fenêtre après sa création
        if (QQuickWindow *window = qobject_cast<QQuickWindow*>(obj)) {
            // Désactiver les marges système de la fenêtre si possible
            window->setProperty("contentItem.anchors.margins", 0);
        } }, Qt::QueuedConnection);

   engine.load(url);

   return app.exec();
}