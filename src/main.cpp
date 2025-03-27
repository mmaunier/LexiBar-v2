#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickWindow>
#include <QIcon>
#include <QTimer>
#include <QFile>
#include <QVariant>
#include <QMargins>
#include <QDebug>
#include "gestionlexibar.h"

int main(int argc, char *argv[])
{
    // Nom de l'exécutable interne
    QGuiApplication::setApplicationName("lexibarv2");
    // Nom du fichier .desktop sans l'extension
    QGuiApplication::setDesktopFileName("lexibarv2");

    // Force l'utilisation d'échelle 1:1
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);

    // Configuration de l'application
    QGuiApplication app(argc, argv);
    
    // Détection de Wayland
    bool isWayland = QGuiApplication::platformName().toLower().contains("wayland");
    qDebug() << "Plateforme détectée:" << QGuiApplication::platformName();
    
    if (isWayland) {
        // Tentative d'activer le layer shell avec décorations activées
        QFile layerShellCheck("/usr/lib/qt6/plugins/wayland-shell-integration/libwayland-layer-shell.so");
        if (layerShellCheck.exists()) {
            qDebug() << "Layer Shell trouvé, activation...";
            qputenv("QT_WAYLAND_SHELL_INTEGRATION", "layer-shell");
            // Activer les décorations pour le layer shell
            qputenv("QT_WAYLAND_LAYER_SHELL_DECORATION", "1");
        } else {
            qDebug() << "Layer Shell non trouvé, utilisation de l'intégration standard";
        }
    }
    
    // Configuration de l'icône de l'application
    QIcon appIcon(":/media/drapeau.png");
    if (appIcon.isNull()) {
        qWarning() << "Impossible de charger l'icône de l'application";
    } else {
        app.setWindowIcon(appIcon);
    }
    
    // Enregistrer le type GestionLexibar
    qmlRegisterType<GestionLexibar>("LexiBar", 1, 0, "GestionLexibar");
    
    // Créer une instance de GestionLexibar
    GestionLexibar lexibar;
    
    // Configurer le moteur QML
    QQmlApplicationEngine engine;
    
    // Exposer l'instance lexibar et la détection de Wayland
    engine.rootContext()->setContextProperty("lexibar", &lexibar);
    engine.rootContext()->setContextProperty("isWayland", isWayland);
    
    const QUrl url(QStringLiteral("qrc:/qml/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url, isWayland](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl) {
            qWarning() << "Échec de la création de l'objet QML!";
            QCoreApplication::exit(-1);
            return;
        }
        
        QQuickWindow *window = qobject_cast<QQuickWindow*>(obj);
        if (window) {
            qDebug() << "Fenêtre créée avec succès, taille:" << window->width() << "x" << window->height();
            
            // Positionner la fenêtre en haut de l'écran
            window->setX((QGuiApplication::primaryScreen()->availableGeometry().width() - window->width()) / 2);
            window->setY(0);
            
            // Sous Wayland, nous utilisons un timer pour maintenir la fenêtre active
            if (isWayland) {
                QTimer *activationTimer = new QTimer(window);
                QObject::connect(activationTimer, &QTimer::timeout, [window]() {
                    if (window && window->isVisible()) {
                        window->raise();
                        window->requestActivate();
                    }
                });
                activationTimer->start(1000); // Toutes les secondes
            }
        }
    }, Qt::QueuedConnection);
    
    engine.load(url);
    
    // Si rien n'a été chargé, quitter
    if (engine.rootObjects().isEmpty()) {
        qWarning() << "Aucun objet root chargé!";
        return -1;
    }
    
    return app.exec();
}