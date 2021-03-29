#include "playcontrolpanel.hpp"
#include "ui_playcontrolpanel.h"

#include "gallerywidget.h"
#include "interactivebuttonbase.h"

#include "vlcplayer.hpp"

#include <QDragMoveEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QMenu>

#include <QDir>
#include <QStyleFactory>
#include <QStyle>

#include <QMessageBox>
#include <QTimer>
#include <QDebug>

static VlcPlayer sVlcPlayer;
static WINDOWID  sWID;

PlayControlPanel::PlayControlPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlayControlPanel),
    m_timerID(-1)
{
    if(!initPlayerEnvironment()) {
        hide(); // 不隐藏会有窗口闪过
        QTimer::singleShot(0,this,&PlayControlPanel::close);
        return;
    }
    ui->setupUi(this);
    QString vlcinfo = QStringLiteral("VLC 版本:") + QString::fromUtf8((VlcPlayer::version() + VlcPlayer::complier_version()).c_str());
    ui->_label->setText(vlcinfo);

    initContextMenu();
}

PlayControlPanel::~PlayControlPanel()
{
    qDebug() << __func__;
    stopPlay();
    delete ui;
}


bool PlayControlPanel::initPlayerEnvironment()
{
    static auto showMsgBox = [](const QString& msg) {
        QMessageBox msgBox(nullptr);
        msgBox.resize(300,200);
        msgBox.setWindowTitle(QStringLiteral("错误"));
        msgBox.setText(msg);
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setWindowIcon(QIcon(":/icon/VideoWallpaper.ico"));
        msgBox.setButtonText(QMessageBox::Ok,QStringLiteral("知道了"));
        msgBox.exec();
    };

    // 查找壁纸层窗口ID
    std::string errmsg;
    sWID = findWallpaperLayerWindow(errmsg);
    if(!errmsg.empty()) {
        QString text = QString::fromUtf8(errmsg.c_str());
        showMsgBox(text);
        return false;
    }
    qDebug() << "Window ID = "<<sWID;

    // 初始化 vlc 播放器
    if(!sVlcPlayer.init()) {
        QString text = QStringLiteral("VLC 初始化错误：\n") +
            QString::fromUtf8(sVlcPlayer.getLastError().c_str());
        showMsgBox(text);
        return false;
    }
    return true;
}
#include <QDir>
void PlayControlPanel::initContextMenu()
{
    QMenu* menu = new QMenu(QStringLiteral("界面风格"), this);
    // 加载所有 qss
    static QMap<QString,QString> styleSheets;
    QDir dir(":/qss");
    QStringList qssNames = dir.entryList(QStringList({"*.qss","*.css"}),QDir::Files);
    for(auto& name : qssNames) {
        QFile f(dir.filePath(name));
        f.open(QIODevice::ReadOnly);
        styleSheets.insert(name, QString::fromUtf8(f.readAll()));
    }

    auto styleNames = qssNames + QStyleFactory::keys();
    for(auto name : styleNames) {
        QAction* action = new QAction(name,this);
        connect(action, &QAction::triggered,[action](){
            QString name = action->text();
            if(styleSheets.contains(name)) {
                qApp->setStyleSheet(styleSheets[name]);
            } else {
                qApp->setStyle(action->text());
                qApp->setStyleSheet("");
            }
        });
        menu->addAction(action);
    }

    QMenu* menu2 = new QMenu(this);
    menu2->addMenu(menu);
    connect(this,&PlayControlPanel::customContextMenuRequested,
        [this,menu2](const QPoint& pos){
            menu2->exec(pos+this->pos());
        });

    setContextMenuPolicy(Qt::CustomContextMenu);
}

void PlayControlPanel::startPlay(const QString &path)
{
#ifdef Q_OS_WINDOWS
    auto path2 = path.toUtf8().replace('/','\\');
#else
    auto path2 = path.toUtf8().replace('/','\\');
#endif
    std::string filename = std::string(path2.data(), static_cast<size_t>(path2.size()));
    qDebug() << __func__ << path2;
    sVlcPlayer.startPlay(filename, sWID);

    // 启动定时器进行更新
    startUpdateTimer();
}

void PlayControlPanel::stopPlay()
{
    stopUpdateTimer();
    sVlcPlayer.stopPlay();
    leaveWallpaperWindow();
}

void PlayControlPanel::pausePlay()
{
    stopUpdateTimer();
    sVlcPlayer.pausePlay();
}

void PlayControlPanel::continuePlay()
{
    sVlcPlayer.continuePlay();
    startUpdateTimer();
}


void PlayControlPanel::dragEnterEvent(QDragEnterEvent *ev)
{
    if(ev->mimeData()->hasUrls()) {
        ev->acceptProposedAction();
    }
}

void PlayControlPanel::dragMoveEvent(QDragMoveEvent *ev)
{
    int line = ui->_hLayoutPlayControl->geometry().height();
    if ( ev->pos().y() < line) {
        ev->ignore();
        return;
    }
    ev->accept();
}

void PlayControlPanel::dropEvent(QDropEvent *ev)
{
    auto urls = ev->mimeData()->urls();
    startPlay(urls[0].toLocalFile());
    return QWidget::dropEvent(ev);
}

void PlayControlPanel::timerEvent(QTimerEvent *ev)
{
    float ppos = sVlcPlayer.getMediaPlayPosition();
    ui->_hSliderProgress->setValue(ppos*100);
}


void PlayControlPanel::on__btnPlayOrPause_clicked(bool checked)
{
    if(sVlcPlayer.isPlaying()) {
        pausePlay();
    } else {
        continuePlay();
    }
}

void PlayControlPanel::on__btnStop_clicked()
{
    stopPlay();
}

void PlayControlPanel::startUpdateTimer()
{
    if(m_timerID != -1) {
        return;
    }
    // 当前 VlcPlayer 不是集成自 QObject 的类
    // 不能支持 Qt 的信号槽处理
    // 如果使用 vlc 的事件处理回调，要把这里对界面的操作
    // 在回调中处理会使得代码比较丑陋，所以还是老套路，基
    // 于定时器的更新（状态编程）来更新界面
    m_timerID = startTimer(1000);
}

void PlayControlPanel::stopUpdateTimer()
{
    if(m_timerID != -1) {
        killTimer(m_timerID);
        m_timerID = -1;
    }
}


void PlayControlPanel::on__btnMute_clicked()
{
    bool mute = ui->_vSliderVolume->isEnabled();
    sVlcPlayer.setMute(mute);
    ui->_vSliderVolume->setEnabled(!mute);
}

void PlayControlPanel::on__vSliderVolume_sliderMoved(int position)
{
    qDebug()<<position;
    sVlcPlayer.setVolume(position);
}

void PlayControlPanel::on__hSliderProgress_sliderMoved(int position)
{
    sVlcPlayer.setMediaPlayPosition(0.01f * position);
}

void PlayControlPanel::on__hSliderProgress_actionTriggered(int action)
{
    switch (action) {
    case QAbstractSlider::SliderPageStepAdd:
    case QAbstractSlider::SliderPageStepSub:
        on__hSliderProgress_sliderMoved(ui->_hSliderProgress->value());
    }
}
