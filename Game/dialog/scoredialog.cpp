#include "scoredialog.h"
#include "ui_scoredialog.h"

ScoreDialog::ScoreDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ScoreDialog)
{
    ui->setupUi(this);
    this->setWindowTitle(SCOREDIALOG_TITLE);
}

ScoreDialog::~ScoreDialog()
{
    delete ui;
}

void ScoreDialog::setAlignment()
{
    ui->player1->setAlignment(Qt::AlignCenter);
    ui->player2->setAlignment(Qt::AlignCenter);
    ui->player3->setAlignment(Qt::AlignCenter);
    ui->player4->setAlignment(Qt::AlignCenter);

    ui->score1->setAlignment(Qt::AlignCenter);
    ui->score2->setAlignment(Qt::AlignCenter);
    ui->score3->setAlignment(Qt::AlignCenter);
    ui->score4->setAlignment(Qt::AlignCenter);
}

void ScoreDialog::insertGameScoreData(std::map<int, std::string>
                                      playerScores)
{
        ui->informationLabel->setText(GAME_END);

        int i = 0;

        for (auto it = playerScores.rbegin(); it != playerScores.rend(); it++) {
            i++;
            if (i==1) {
                ui->player1->setText(QString::fromStdString(it->second));
                ui->score1->setText(QString::number(it->first));
            } else if (i == 2) {
                ui->player2->setText(QString::fromStdString(it->second));
                ui->score2->setText(QString::number(it->first));
            } else if (i == 3) {
                ui->player3->setText(QString::fromStdString(it->second));
                ui->score3->setText(QString::number(it->first));
            } else if (i==4) {
                ui->player4->setText(QString::fromStdString(it->second));
                ui->score4->setText(QString::number(it->first));
            }
        }
        setAlignment();
}

