#ifndef SCOREDIALOG_H
#define SCOREDIALOG_H

#include "algorithm"
#include "constants/constants.hh"

#include <QDialog>

namespace Ui {
class ScoreDialog;
}

/**
 * @brief The ScoreDialog class is a simple QDialog based class
 * for showing the player scores when the game ends
 */
class ScoreDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ScoreDialog(QWidget *parent = 0);
    ~ScoreDialog();

public slots:
    /**
     * @brief Sets Alignment for some ui elements
     */
    void setAlignment();

    /**
     * @brief Shows the game scoreboard
     * @param playerScores to show
     */
    void insertGameScoreData(std::map<int, std::string> playerScores);

private:
    Ui::ScoreDialog *ui;
};

#endif // SCOREDIALOG_H
