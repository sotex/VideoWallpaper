#include "playcontrolpanel.hpp"
#include "ui_playcontrolpanel.h"

#include <QDebug>

PlayControlPanel::PlayControlPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlayControlPanel)
{
    ui->setupUi(this);
}

PlayControlPanel::~PlayControlPanel()
{
    delete ui;
}

void PlayControlPanel::on__btnPlayOrPause_clicked(bool checked)
{


}
