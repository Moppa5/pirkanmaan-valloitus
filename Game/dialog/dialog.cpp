#include "dialog.h"
#include "ui_dialog.h"
#include "constants/constants.hh"

#include <QKeyEvent>
#include <sstream>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog),
    playerCount_(1)
{
    ui->setupUi(this);
    this->setWindowTitle(DIALOG_TITLE);
    ui->addPlayerBox->setDisabled(true);
    ui->resetPlayers->hide();
    ui->playerBrowser->hide();
    ui->playerBrowser->setMaximumHeight(75);
    connect(ui->buttonBox,SIGNAL(clicked(QAbstractButton*)),this,
            SLOT(verifyDialogData()));
    connect(ui->addPlayerBox,SIGNAL(clicked(bool)),this,SLOT(addPlayer()));
    connect(ui->playerCount,SIGNAL(editingFinished()),this,
            SLOT(playerCount()));
    connect(ui->resetPlayers,SIGNAL(clicked(bool)),this,
            SLOT(resetPlayersButton()));
    connect(ui->player,SIGNAL(textEdited(QString)),this,
            SLOT(playerLineEdit()));
    connect(ui->player,SIGNAL(returnPressed()),this,SLOT(addPlayer()));
    connect(ui->seed, SIGNAL(textChanged(QString)), this,
            SLOT(setSeed()));
    connect(ui->roundCount, SIGNAL(valueChanged(int)), this,
            SLOT(setRounds()));

    ui->roundCount->setMinimum(MIN_ROUND_COUNT);
    ui->roundCount->setMaximum(MAX_ROUND_COUNT);
    ui->roundCount->setValue(DEFAULT_ROUND_COUNT);
}
Dialog::~Dialog()
{
    delete ui;
}

std::vector<QString> Dialog::getPlayers()
{
    return players_;
}

int Dialog::getSeed()
{
    return seed_;
}

int Dialog::getRounds()
{
    return rounds_;
}

bool Dialog::isPlayernameValid()
{
    QString playerName = ui->player->text().trimmed();

    if (playerName.length() > MAX_PLAYERNAME_LENGTH or
            playerName.length() == 0) {
        return false;
    }

    return true;
}

QString Dialog::getPlayersString()
{
        QString players = "";

        for (unsigned int i = 0; i<players_.size(); i++) {
            players += QString::number(i+1) + " " + players_.at(i) + "\n";
        }
        return players;
}

void Dialog::keyPressEvent(QKeyEvent *event)
{
    Q_UNUSED(event);
}
void Dialog::verifyDialogData()
{
    int playerCount = players_.size();

    if (playerCount == playerCount_) {
        QDialog::accept();
    }
}

void Dialog::addPlayer()
{
    QLineEdit* player = ui->player;
    QLabel* labelText = ui->playerLabel;
    int size = players_.size();
    ui->playerCount->setDisabled(true);
    QString playerName = player->text().trimmed();

    if (isPlayernameValid()) {
        // Length valid or not
        if (size == 0 && !playerName.isEmpty()) {
            players_.push_back(playerName);
            if (playerCount_ > 1) {
                labelText->setText("Player 2");
            } else {
                playersAdded();
            }
            player->setText("");
        } else {
            // More players than one
            bool valid = true;
            for (QString s: players_) {
                if (s == playerName) {
                    valid = false;
                    labelText->setText(PLAYERNAME_TAKEN);
                    break;
                }
            }
            if (valid && playerName != "") {
                players_.push_back(playerName);
                QString playerNumber = QString::number(players_.size()+1);

                if (static_cast<int>(players_.size()) == playerCount_) {
                    playersAdded();
                } else {
                    labelText->setText("Player "+playerNumber);
                }
                player->setText("");
            }
        }
    } else if (playerName == "")  {
        labelText->setText(PLAYERNAME_EMPTY);
        player->setText("");
    }
    else {
        labelText->setText(TOO_LONG_NAME);
        player->setText("");
    }
}
void Dialog::playerCount()
{
    playerCount_ = ui->playerCount->value();

    if (players_.size() == 0) {
        ui->playerLabel->setText("Player 1");
        ui->addPlayerBox->setEnabled(true);
    } else if (playerCount_ == static_cast<int>(players_.size())) {
        ui->addPlayerBox->setDisabled(true);
    }
}

void Dialog::playersAdded()
{
    ui->addPlayerBox->setDisabled(true);
    ui->playerLabel->setText(PLAYERS_ADDED);
    ui->playerBrowser->show();
    ui->player->hide();
    ui->addPlayerBox->hide();
    ui->resetPlayers->show();
    ui->playerBrowser->setText(getPlayersString());
}

void Dialog::resetPlayersButton()
{
    ui->resetPlayers->hide();
    players_.clear();
    ui->addPlayerBox->setEnabled(true);
    ui->player->show();
    ui->addPlayerBox->show();
    ui->playerLabel->setText("Player 1");
    ui->playerBrowser->setText("");
    ui->playerBrowser->hide();
    ui->playerCount->setEnabled(true);
}

void Dialog::playerLineEdit()
{
    if (playerCount_ == 1 and players_.size() == 0) {
        ui->playerLabel->setText("Player 1");
        ui->addPlayerBox->setEnabled(true);
    }
}

void Dialog::setSeed()
{
    std::string seed = ui->seed->text().toStdString();

    for(unsigned int i=0; i<seed.length(); i++){
        seed_ += (int)seed[i];
    }
}

void Dialog::setRounds()
{
    rounds_ = ui->roundCount->value();
}

