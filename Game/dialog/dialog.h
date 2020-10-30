#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QDebug>
#include <vector>

namespace Ui {
class Dialog;
}

/**
 * @brief The Dialog class is a simple QDialog based class for
 * game initialization data like playernames, seed, rounds
 */
class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
    /**
     * @brief Fetches the player vector
     * @return vector that contains the players
     */
    std::vector<QString> getPlayers();

    /**
     * @brief Fetches the game seed if set
     * @return returns the seed
     */
    int getSeed();

    /**
     * @brief Fetches the rounds if set
     * @return returns the rounds
     */
    int getRounds();

    /**
     * @brief Checks if the length is valid
     * @return Valid or not
     */
    bool isPlayernameValid();

    /**
     * @brief Gets players as strings
     * @return QSTring with players on it
     */
    QString getPlayersString();

    virtual void keyPressEvent(QKeyEvent* event);

public slots:

    /**
     * @brief Verifies that the input dialog data is valid
     * Data is valid at least if playernames has been set
     */
    void verifyDialogData();

    /**
     * @brief Adds player to the vector
     */
    void addPlayer();

    /**
     * @brief Handles the set amount of players
     */
    void playerCount();

    /**
     * @brief Sets the Dialog in status that players are added
     */
    void playersAdded();

    /**
     * @brief Resets players so that they can be set again
     */
    void resetPlayersButton();

    /**
     * @brief Updates playerbox element and sets text for Player 1
     */
    void playerLineEdit();

    /**
     * @brief Sets the current seed for the game
     */
    void setSeed();

    /**
     * @brief Sets amount of rounds
     */
    void setRounds();

private:
    Ui::Dialog *ui;
    std::vector<QString> players_;
    int playerCount_ = 0;
    int seed_;
    int rounds_;
};

#endif // DIALOG_H
