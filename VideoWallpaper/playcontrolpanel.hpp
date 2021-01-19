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
    ~PlayControlPanel();

private slots:
    void on__btnPlayOrPause_clicked(bool checked);

private:
    Ui::PlayControlPanel *ui;
};

#endif // PLAYCONTROLPANEL_HPP
