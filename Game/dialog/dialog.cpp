#include "dialog.h"
#include "ui_dialog.h"
#include "constants/constants.hh"

#include <QKeyEvent>
#include <sstream>
#include <QTime>

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

    // connect the color buttons
    connect(ui->red, SIGNAL(clicked()), this, SLOT(addColor()));
    connect(ui->lime, SIGNAL(clicked()), this, SLOT(addColor()));
    connect(ui->blue, SIGNAL(clicked()), this, SLOT(addColor()));
    connect(ui->yellow, SIGNAL(clicked()), this, SLOT(addColor()));
    connect(ui->pink, SIGNAL(clicked()), this, SLOT(addColor()));
    connect(ui->darkgreen, SIGNAL(clicked()), this, SLOT(addColor()));
    connect(ui->navy, SIGNAL(clicked()), this, SLOT(addColor()));
    connect(ui->orange_yellow, SIGNAL(clicked()), this, SLOT(addColor()));
    connect(ui->violet, SIGNAL(clicked()), this, SLOT(addColor()));
    connect(ui->turq, SIGNAL(clicked()), this, SLOT(addColor()));
    connect(ui->light_violet, SIGNAL(clicked()), this, SLOT(addColor()));
    connect(ui->orange, SIGNAL(clicked()), this, SLOT(addColor()));

    ui->roundCount->setMinimum(MIN_ROUND_COUNT);
    ui->roundCount->setMaximum(MAX_ROUND_COUNT);
    ui->roundCount->setValue(DEFAULT_ROUND_COUNT);
}
Dialog::~Dialog()
{
    delete ui;
}

std::unordered_map<QString, QColor> Dialog::getPlayers()
{
    return players_;
}

int Dialog::getSeed()
{
    if (seed_ == 0) {
        const QTime &currentTime = QTime().currentTime();
        std::string seed = currentTime.toString().toStdString();

        for(unsigned int i=0; i<seed.length(); i++){
            seed_ += (int)seed[i];
        }
    }

    return seed_;
}

int Dialog::getRounds()
{
    return rounds_;
}

bool Dialog::isPlayernameValid()
{
    const QString &playerName = ui->player->text().trimmed();

    if (playerName.length() > MAX_PLAYERNAME_LENGTH or
            playerName.length() == 0) {
        return false;
    }

    return true;
}

bool Dialog::isValidColor()
{
    for (const auto &player: players_) {

        if (player.second == pickedColor_) {
            return false;
            break;
        }
    }

    return true;
}

QString Dialog::getPlayersString()
{
    QString players = EMPTY;
    int i = 0;

    for (const auto &player: players_) {
        players += QString::number(i+1) + " " + player.first + "\n";
        i++;
    }

    return players;
}

void Dialog::keyPressEvent(QKeyEvent *event)
{
    Q_UNUSED(event);
}
void Dialog::verifyDialogData()
{
    const int &playerCount = players_.size();

    if (playerCount == playerCount_) {
        emit dialogVerified();
        QDialog::accept();
    }
}

void Dialog::addColor()
{
    if (playersAdded_) {
        return;
    }

    const QString &buttonName = QObject::sender()->objectName();
    QLabel *colorLabel = ui->colorLabel;

    if (buttonName == "red") {
        pickedColor_ = QColor(255,0,0);
    } if (buttonName == "lime") {
        pickedColor_ = QColor(0,255,0);
    } if (buttonName == "blue") {
        pickedColor_ = QColor(0,127,255);
    } if (buttonName == "yellow") {
        pickedColor_ = QColor(255,255,0);
    } if (buttonName == "pink") {
        pickedColor_ = QColor(255,0,127);
    } if (buttonName == "darkgreen") {
        pickedColor_ = QColor(0,85,0);
    } if (buttonName == "navy") {
        pickedColor_ = QColor(0,0,127);
    } if (buttonName == "orange_yellow") {
        pickedColor_ = QColor(255,207,64);
    } if (buttonName == "violet") {
        pickedColor_ = QColor(85,0,127);
    } if (buttonName == "turq") {
        pickedColor_ = QColor(0,255,255);
    } if (buttonName == "light_violet") {
        pickedColor_ = QColor(79,73,255);
    } if (buttonName == "orange") {
        pickedColor_ = QColor(255,99,71);
    }
    QString background = " background-color : ";
    background += pickedColor_.name();
    ui->preferredColor->setStyleSheet(background);

    if (!isValidColor()) {
        colorLabel->setText(COLOR_TAKEN);
        return;
    }

    colorLabel->setText("Picked ");
}

void Dialog::addPlayer()
{
    QLineEdit* player = ui->player;
    QLabel* labelText = ui->playerLabel;
    QLabel* colorLabel = ui->colorLabel;
    const int &size = players_.size();
    ui->playerCount->setDisabled(true);
    const QString &playerName = player->text().trimmed();

    if (!isValidColor()) {
        colorLabel->setText(COLOR_TAKEN);
        return;
    }

    if (!pickedColor_.isValid()) {
        colorLabel->setText(COLOR);
        return;
    }

    if (isPlayernameValid() ) {
        // Length valid or not

        if (size == 0 && !playerName.isEmpty()) {
            players_.insert({playerName, pickedColor_});
            ui->preferredColor->setStyleSheet("background-color: qlineargradient(spread:pad, x1:1, y1:0,"
                                              " x2:1, y2:0, stop:0 rgba(21, 40, 38, 255),"
                                              " stop:1 rgba(255, 255, 255, 255));");
            pickedColor_ = nullptr;

            if (playerCount_ > 1) {
                labelText->setText("Player 2");
                colorLabel->setText("Pick a color");
            } else {
                playersAdded();
            }
            player->setText(EMPTY);

        } else {
            // More players than one
            bool valid = true;
            for (const auto &player: players_) {
                if (player.first == playerName) {
                    valid = false;
                    labelText->setText(PLAYERNAME_TAKEN);
                    break;
                }
            }
            if (valid && playerName != EMPTY) {
                players_.insert({playerName, pickedColor_});
                pickedColor_ = nullptr;
                ui->preferredColor->setStyleSheet("background-color: qlineargradient(spread:pad, x1:1, y1:0,"
                                                  " x2:1, y2:0, stop:0 rgba(21, 40, 38, 255),"
                                                  " stop:1 rgba(255, 255, 255, 255));");
                QString playerNumber = QString::number(players_.size()+1);

                if (static_cast<int>(players_.size()) == playerCount_) {
                    playersAdded();
                } else {
                    labelText->setText("Player "+playerNumber);
                    colorLabel->setText("Pick a color");
                }
                player->setText(EMPTY);
            }
        }
    } else if (playerName == EMPTY)  {
        labelText->setText(PLAYERNAME_EMPTY);
        player->setText(EMPTY);
    }
    else {
        labelText->setText(TOO_LONG_NAME);
        player->setText(EMPTY);
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
    ui->colorLabel->hide();
    ui->preferredColor->setStyleSheet("background-color: qlineargradient(spread:pad, x1:1, y1:0,"
                                      " x2:1, y2:0, stop:0 rgba(21, 40, 38, 255),"
                                      " stop:1 rgba(255, 255, 255, 255));");
    ui->addPlayerBox->hide();
    ui->resetPlayers->show();
    ui->playerBrowser->setText(getPlayersString());
    playersAdded_ = true;
}

void Dialog::resetPlayersButton()
{
    ui->resetPlayers->hide();
    players_.clear();
    ui->addPlayerBox->setEnabled(true);
    ui->player->show();
    ui->addPlayerBox->show();
    ui->colorLabel->show();
    ui->playerLabel->setText("Player 1");
    ui->playerBrowser->setText(EMPTY);
    ui->playerBrowser->hide();
    ui->playerCount->setEnabled(true);
    ui->colorLabel->setText("Pick a color");
    playersAdded_ = false;
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
    const std::string &seed = ui->seed->text().toStdString();

    for(unsigned int i=0; i<seed.length(); i++){
        seed_ += (int)seed[i];
    }
}

void Dialog::setRounds()
{
    rounds_ = ui->roundCount->value();
}

