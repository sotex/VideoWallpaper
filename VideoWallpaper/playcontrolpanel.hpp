#ifndef PLAYCONTROLPANEL_HPP
#define PLAYCONTROLPANEL_HPP

#include <QWidget>

namespace Ui {
class PlayControlPanel;
}

class PlayControlPanel : public QWidget
{
    Q_OBJECT

public:
    explicit PlayControlPanel(QWidget *parent = nullptr);
    ~PlayControlPanel() override;

    bool initPlayerEnvironment();
    void initContextMenu();

    void startPlay(const QString& path);

    void stopPlay();
    void pausePlay();
    void continuePlay();

public:
    virtual void dragEnterEvent(QDragEnterEvent *ev) override;
    virtual void dragMoveEvent(QDragMoveEvent *ev) override;
    virtual void dropEvent(QDropEvent *ev) override;
    virtual void timerEvent(QTimerEvent *ev);
private slots:
    void on__btnPlayOrPause_clicked(bool checked);

    void on__btnStop_clicked();


    void on__btnMute_clicked();

    void on__vSliderVolume_sliderMoved(int position);

    void on__hSliderProgress_sliderMoved(int position);

    void on__hSliderProgress_actionTriggered(int action);

private:
    void startUpdateTimer();
    void stopUpdateTimer();

private:
    Ui::PlayControlPanel *ui;
    int     m_timerID;
};

#endif // PLAYCONTROLPANEL_HPP
