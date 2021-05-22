#ifndef MAPWINDOW_HH
#define MAPWINDOW_HH

#include <dialog/dialog.h>
#include <dialog/scoredialog.h>
#include <constants/constants.hh>
#include "core/gamemanager.hh"
#include "interfaces/gameeventhandler.hh"
#include "graphics/gamescene.hh"
#include "interfaces/objectmanager.hh"
#include "core/player.hh"
#include "core/basicresources.h"

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <map>
#include <QPropertyAnimation>

namespace Ui {
class MapWindow;
}

/**
 * @brief The MapWindow class is QMainWindow based class for game's
 * graphical user interface.
 */
class MapWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MapWindow(QWidget *parent = 0);
    ~MapWindow();

public slots:

    /**
     * @brief Starts the game
     */
    void gameButtonClicked();

    /**
     * @brief Opens the dialog if players want to config the game etc.
     */
    void openDialog();

    /**
     * @brief Opens the game scoreboard
     */
    void openScoreDialog();

    /**
     * @brief Creates a new game
     */
    void newGame();

    /**
     * @brief Deletes all data saved in previous game and brings up dialog
     */
    void resetGame();

    /**
     * @brief Helper to set game buttons for state
     * @param state true or false
     */
    void setButtonStateEnabled(const bool &state);

    /**
     * @brief Resizes the window based on MapView
     * @param relativeWidth
     * @param relativeHeight
     */
    void resizeGameView(const int &relativeWidth, const int &relativeHeight);

    /**
     * @brief Zooms the view in
     */
    void mapZoomInButtonClicked();

    /**
     * @brief Zooms the view out
     */
    void mapZoomOutButtonClicked();

    /**
     * @brief Updates player related infos
     * @param player on which the update is based on
     */
    void updatePlayerInfo(const std::shared_ptr<Game::Player> &player);

    /**
     * @brief Updates the building cost when player selects building
     */
    void updateBuildingCost();

    /**
     * @brief Updates building cost values to GUI
     * @param resources - values to update
     */
    void setBuildingCost(ResourceMap resources);

    /**
     * @brief Fetches the workers cost for later use
     */
    void updateWorkersCost();

    /**
     * @brief Sets the worker cost for GUI based on updateWorkersCost()
     * @param Resources to be used
     */
    void setWorkersCost(Course::ResourceMap resources);

    /**
     * @brief Updates shopping costs when slider or spinbox changes
     */
    void updateSellingValue();

    /**
     * @brief Updates tile info on to GUI
     * @param type of the tile
     * @param item that has been clicked on scene
     * @param tileImage of the tile item
     * @param player as the owner
     */
    void updateTileInfo(const QString &type,
                        Game::MapItem *&item,
                        const QPixmap &tileImage,
                        std::string player = "");

    /**
     * @brief Updates the worker info on to GUI
     * @param item to fetch object from
     */
    void updateWorkersInfo(Game::MapItem *&item);

    /**
     * @brief Updates the building list on GUI view
     * @param item to fetch the buildings from
     */
    void updateBuildingsList(Game::MapItem *&item);

    /**
     * @brief Updates production values for GUI element
     * @param item to which update is based on
     */
    void updateTileProductionValues(Game::MapItem* item);

    /**
     * @brief Updates shop slider
     * @param value optional to put the slider value
     */
    void updateShopSlider(const int &value = 0);

    /**
     * @brief Updates the shop spinbox
     * @param value optional to put the spinbox value
     */
    void updateShopSpinBox(const int &value = 0);

    /**
     * @brief Updates worker slider
     * @param value optional to put the slider value
     */
    void updateWorkerSlider(const int &value = 0);

    /**
     * @brief Updates the worker spinbox
     * @param value optional to put the spinbox value
     */
    void updateWorkerSpinBox(const int &value = 0);

    /**
     * @brief Resets elements like spinboxes, sliders etc.
     * Can be used for example when the player turn cahnges
     */
    void resetElements();

    /**
     * @brief Prompts for building process
     */
    void buildClicked();

    /**
     * @brief Handles the deleting of the building item selected on ListView
     */
    void removeBuildingClicked();

    /**
     * @brief Prompts for buying process
     */
    void sellClicked();

    /**
     * @brief Updates resource limits for shop
     */
    void updateShopLimits();

    /**
     * @brief Prompts for the assignment of selected workers
     */
    void workerAssigned();

    /**
     * @brief Prompts for tile claiming
     */
    void claimClicked();

    /**
     * @brief Handles the worker freement based on ListView item
     */
    void freeWorkerClicked();

private:
    /**
     * @brief Adjusts the current turn count
     * @param Turn number to put on
     */
    void setTurnCount(const int &turnNumber);

    /**
     * @brief Adjusts MapWindow GUI settings etc.
     * Also includes connects for elements
     */
    void adjustSettings();

    Ui::MapWindow* ui_;
    std::shared_ptr<Dialog> settingsDialog_ = nullptr;
    std::shared_ptr<ScoreDialog> scoreDialog_ = nullptr;

    std::shared_ptr<Game::GameManager> gManager_ = nullptr;
    std::shared_ptr<Game::GameEventHandler> geHandler_ = nullptr;
    std::shared_ptr<Game::GameScene> gScene_ = nullptr;
    std::shared_ptr<Game::ObjectManager> objManager_ = nullptr;

    Game::MapItem* currentItem_ = nullptr;

    // Used to track gamestate for gameButton
    bool gameStarted_ = false;
    float zoomFactor_ = 1.5;
    float currentZoomLevel = 1;
};

#endif // MAPWINDOW_HH
