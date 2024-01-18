// SPDX-License-Identifier: Unlicense

#include <statusnotifieritem.h>

#include <QApplication>
#include <QMenu>
#include <QMessageBox>
#include <QProxyStyle>
#include <QTimer>
#include <QToolTip>
#include <QDBusInterface>

#include <algorithm>

QT_BEGIN_NAMESPACE
class QMenu;
class QMessageBox;
QT_END_NAMESPACE

class SNIExample : public QObject
{
    Q_OBJECT

public:
    SNIExample(QObject* parent = nullptr);
    ~SNIExample();

private:
    void onScroll(int delta, Qt::Orientation);
    void onActivateRequested(const QPoint&);
    void onSecondaryActivateRequested(const QPoint&);

    StatusNotifierItem* sni_;
    QMessageBox*        msgBox_;
    QMenu*              contextMenu_;
};

SNIExample::SNIExample(QObject* parent)
    : QObject(parent)
    , sni_(new StatusNotifierItem(QApplication::applicationName(), qApp))
    , msgBox_(new QMessageBox)
    , contextMenu_(new QMenu)
{
    sni_->setContextMenu(contextMenu_);
    sni_->setIconByName(QStringLiteral("face-smile"));
    sni_->setStatus(StatusNotifierItem::ItemStatus::Active);

    QAction* quit = new QAction(QIcon::fromTheme("application-exit"), tr("&Quit"), contextMenu_);
    contextMenu_->addAction(quit);
    connect(quit, &QAction::triggered, qApp, &QApplication::exit);

    connect(sni_, &StatusNotifierItem::scrollRequested,            this, &SNIExample::onScroll);
    connect(sni_, &StatusNotifierItem::activateRequested,          this, &SNIExample::onActivateRequested);
    connect(sni_, &StatusNotifierItem::secondaryActivateRequested, this, &SNIExample::onSecondaryActivateRequested);

    connect(qApp, &QApplication::aboutToQuit, msgBox_,      &QObject::deleteLater);
    connect(qApp, &QApplication::aboutToQuit, contextMenu_, &QObject::deleteLater);
}

SNIExample::~SNIExample()
{
}

void SNIExample::onScroll(int delta, Qt::Orientation)
{
    QString title = QString("Uhm... last delta was %1...").arg(delta);
    QString iconName;
    delta > 0
        ? iconName = QStringLiteral("face-smile")
        : iconName = QStringLiteral("face-sad");

    sni_->setIconByName(iconName);
    sni_->setToolTipTitle(title);

    QToolTip::showText(QCursor::pos(), title);
    QToolTip::hideText();
}

void SNIExample::onActivateRequested(const QPoint&)
{
    QMessageBox::Icon icon;
    if (sni_->iconName() == QStringLiteral("face-smile")) {
        icon = QMessageBox::Information;
        msgBox_->setWindowTitle(tr("Ahhh!"));
        msgBox_->setText(tr("What a beautiful day!"));
    } else {
        icon = QMessageBox::Warning;
        msgBox_->setWindowTitle(tr("Sigh!"));
        msgBox_->setText(tr("Oh no, how unfortunate!"));
    }
    msgBox_->setIcon(icon);
    msgBox_->setWindowIcon(QIcon::fromTheme(sni_->iconName()));
    msgBox_->show();
}

void SNIExample::onSecondaryActivateRequested(const QPoint&)
{
    const QString oldIconName = sni_->iconName();
    sni_->setIconByName(QStringLiteral("face-surprise"));
    QTimer::singleShot(1000, this, [this, oldIconName]{ sni_->setIconByName(oldIconName); });
}

int main(int argc, char **argv)
{
    QApplication::setQuitOnLastWindowClosed(false);
    QApplication app(argc, argv);
    new SNIExample(&app);
    return app.exec();
}
#include "example.moc"
