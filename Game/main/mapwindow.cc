#include "mapwindow.hh"
#include "ui_mapwindow.h"

#include "core/gamemanager.hh"
#include "interfaces/gameeventhandler.hh"
#include "graphics/gamescene.hh"
#include "interfaces/objectmanager.hh"
#include "core/resourcemaps.h"
#include "constants/resourcemaps2.h"

#include "iostream"


MapWindow::MapWindow(QWidget *parent) :
    QMainWindow(parent),
    ui_(new Ui::MapWindow)
{
    ui_->setupUi(this);
    ui_->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
    ui_->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui_->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui_->graphicsView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    ui_->graphicsView->setRenderHint(QPainter::Antialiasing);
    adjustSettings();

    // Add buildings to UI
    ui_->buildingsBox->addItem(FARM);
    ui_->buildingsBox->addItem(HQ);
    ui_->buildingsBox->addItem(OUTPOST);
    ui_->buildingsBox->addItem(MINE);
    ui_->buildingsBox->addItem(FISHING_BOAT);
    ui_->buildingsBox->addItem(LAKE_COTTAGE);

    // Add workers to UI
    ui_->workersBox->addItem(WORKER_BASIC);
    ui_->workersBox->addItem(WORKER_FARMER);
    ui_->workersBox->addItem(WORKER_MINER);
    //ui_->workersBox->addItem(WORKER_SABOTEUR);

    // Add resources to shop
    ui_->shopBox->addItem("Wood");
    ui_->shopBox->addItem("Food");
    ui_->shopBox->addItem("Stone");
    ui_->shopBox->addItem("Ore");

    ui_->selectedBuildingImage->setPixmap(QPixmap(BUILDING_IMAGE).scaled(50,50,
                                                         Qt::KeepAspectRatio));
    resetGame();
}

MapWindow::~MapWindow()
{
    delete ui_;
}

void MapWindow::gameButtonClicked()
{   
    if(settingsDialog_->getPlayers().size() == 0){
        settingsDialog_->open();
        return;
    }

    if(gameStarted_ == false){
        newGame();
    }else{
        gManager_->endTurn();
        if(gManager_->gameOver_){
            openScoreDialog();
            setButtonStateEnabled(false);
        }
    }
    // All these happen for the next player
    updatePlayerInfo(gManager_->getCurrentPlayer());
    resetElements();
}


void MapWindow::openDialog()
{
    settingsDialog_->show();
}

void MapWindow::openScoreDialog()
{
    if(!gameStarted_ or gManager_->getTurnCount() > gManager_->getCurrentTurnNumber() ){
        return;
    }
    scoreDialog_->insertGameScoreData(gManager_->getScores());
    scoreDialog_->exec();
}

void MapWindow::newGame()
{
    geHandler_ = std::make_shared<Game::GameEventHandler>();
	objManager_ = std::make_shared<Game::ObjectManager>();
	gScene_ = std::make_shared<Game::GameScene>(30,20,20, objManager_.get());

    Game::GameScene* gsRawptr = gScene_.get();
    ui_->graphicsView->setScene(dynamic_cast<QGraphicsScene*>(gsRawptr));

    // Update tileInfo from scene's event
    connect(gsRawptr,&Game::GameScene::tileInfo,this,
            &MapWindow::updateTileInfo);

    currentZoomLevel = 1;

    // Start the game => create map etc.
    std::shared_ptr<Game::GameManager> gm(new Game::GameManager(
                          settingsDialog_, geHandler_,
                                              objManager_,
                                              gScene_,
                                              this));
    gManager_ = gm;
    ui_->gameButton->setText(GAME_BUTTON_WHILE_GAME);
    setButtonStateEnabled(true);
    // Resize the graphicsview and fit map to it
    std::pair<int, int> mapSize = gm->getMapSize();
	resizeGameView(mapSize.first, mapSize.second);

    // Update GUI
    updatePlayerInfo(gManager_->getCurrentPlayer());
    resetElements();

    gameStarted_ = true;
}

void MapWindow::resetGame()
{
    gameStarted_ = false;

    gManager_ = nullptr;
    geHandler_ = nullptr;
    gScene_ = nullptr;
    objManager_ = nullptr;
    currentItem_ = nullptr;

    ui_->gameButton->setText("Start game");
    ui_->buildingsOnTile->clear();
    ui_->workersOnTileList->clear();

    settingsDialog_ = std::make_shared<Dialog>();
    scoreDialog_ = std::make_shared<ScoreDialog>();
    settingsDialog_->exec();
}

void MapWindow::setButtonStateEnabled(const bool state)
{
    ui_->claimButton->setEnabled(state);
    ui_->buildButton->setEnabled(state);
    ui_->assignButton->setEnabled(state);
    ui_->freeButton->setEnabled(state);
    ui_->deleteBuildingButton->setEnabled(state);
    ui_->sellButton->setEnabled(state);
}

void MapWindow::resizeGameView(int relativeWidth, int relativeHeight)
{
    int oldViewWidth = ui_->graphicsView->width();
    int oldWindowWidth = this->width();

    // Resize graphicsView
    int newViewWidth = float(relativeWidth)/
            float(relativeHeight)*ui_->graphicsView->height();
    ui_->graphicsView->setFixedWidth(newViewWidth);

    // Resize whole window
    this->setFixedWidth(oldWindowWidth+(newViewWidth-oldViewWidth));

    // Move controlsFrame
    ui_->controlsFrame->move(ui_->controlsFrame->x()+(newViewWidth-oldViewWidth),
                               ui_->controlsFrame->y());

    // Move zoom buttons
    ui_->mapZoomInButton->move(ui_->mapZoomInButton->x()+(newViewWidth-oldViewWidth),
                               ui_->mapZoomInButton->y());
    ui_->mapZoomOutButton->move(ui_->mapZoomOutButton->x()+(newViewWidth-oldViewWidth),
                               ui_->mapZoomOutButton->y());

    // Fit scene
    ui_->graphicsView->fitInView(gScene_->sceneRect(), Qt::KeepAspectRatio);
}

void MapWindow::setTurnCount(int turnNumber)
{
    QString text = QString::fromStdString("Turn " + std::to_string(turnNumber) +
            "/" + std::to_string(gManager_->getTurnCount()));
    ui_->turnCountLabel->setText(text);
}

void MapWindow::adjustSettings()
{
    // Update sliders and spinboxes
    connect(ui_->shopAmountBox,SIGNAL(valueChanged(int)),this,
            SLOT(updateShopSlider()));
    connect(ui_->shopAmountSlider,SIGNAL(valueChanged(int)),this,
            SLOT(updateShopSpinBox()));
    connect(ui_->shopAmountBox,SIGNAL(valueChanged(int)),this,
            SLOT(updateSellingValue()));
    connect(ui_->shopAmountSlider,SIGNAL(sliderReleased()),this,
            SLOT(updateSellingValue()));
    connect(ui_->workersBox, SIGNAL(currentIndexChanged(int)), this,
            SLOT(updateWorkersCost()));
    connect(ui_->workerSlider,SIGNAL(valueChanged(int)),this,
            SLOT(updateWorkerSpinBox()));
    connect(ui_->workerSpinBox,SIGNAL(valueChanged(int)),this,
            SLOT(updateWorkerSlider()));

    // Gamebutton, dialogs and reset game
    connect(ui_->gameButton, SIGNAL(clicked(bool)),this,
            SLOT(gameButtonClicked()));
    connect(ui_->openDialog,SIGNAL(triggered(bool)),this,SLOT(openDialog()));
    connect(ui_->newGame,SIGNAL(triggered(bool)),this,SLOT(resetGame()));
    connect(ui_->scoreBoard,SIGNAL(triggered(bool)),this,SLOT(openScoreDialog()));

    // Buildings
    connect(ui_->buildButton,SIGNAL(clicked(bool)),this,
            SLOT(buildClicked()));
    connect(ui_->deleteBuildingButton, SIGNAL(clicked(bool)), this,
            SLOT(removeBuildingClicked()));
    connect(ui_->buildingsBox,SIGNAL(currentIndexChanged(int)),this,
            SLOT(updateBuildingCost()));

    // Workers
    connect(ui_->assignButton,SIGNAL(clicked(bool)),this,
            SLOT(workerAssigned()));
    connect(ui_->freeButton, SIGNAL(clicked(bool)), this,
            SLOT(freeWorkerClicked()));
    connect(ui_->claimButton,SIGNAL(clicked(bool)),this,
            SLOT(claimClicked()));

    // Shop
    connect(ui_->sellButton, SIGNAL(clicked(bool)), this,
            SLOT(sellClicked()));
    connect(ui_->shopBox, SIGNAL(currentIndexChanged(int)), this,
            SLOT(updateShopLimits()));

    // Zoom buttons
    connect(ui_->mapZoomInButton,SIGNAL(clicked(bool)),this,
            SLOT(mapZoomInButtonClicked()));
    connect(ui_->mapZoomOutButton,SIGNAL(clicked(bool)),this,
            SLOT(mapZoomOutButtonClicked()));
}

void MapWindow::mapZoomInButtonClicked()
{
    ui_->graphicsView->scale(zoomFactor_, zoomFactor_);
    currentZoomLevel = currentZoomLevel*zoomFactor_;
}

void MapWindow::mapZoomOutButtonClicked()
{
    if(currentZoomLevel > 0.9*zoomFactor_) // safe with floating point precision
    {
        ui_->graphicsView->scale(1/zoomFactor_, 1/zoomFactor_);
        currentZoomLevel = currentZoomLevel/zoomFactor_;
    }
}

void MapWindow::updatePlayerInfo(const std::shared_ptr<Game::Player> &player)
{
    // Update name
    ui_->currentPlayerNameLabel->setText(QString::fromStdString(
                                             player->getName()));
    QColor playerColor = player->getColor();
    QString style = "background-color:rgb(" +
            QString::number(playerColor.red()) + "," +
            QString::number(playerColor.green()) + "," +
            QString::number(playerColor.blue()) + ");";
    ui_->currentPlayerNameLabel->setStyleSheet(style);
    // Update turn number
    setTurnCount(gManager_->getCurrentTurnNumber());
    // Update resources
    std::shared_ptr<Course::ResourceMap> resources =
            gManager_->getCurrentPlayer()->getResourceMap();

    ui_->woodLabel->setText(QString::number(resources->at(Course::WOOD)));
    ui_->foodLabel->setText(QString::number(resources->at(Course::FOOD)));
    ui_->stoneLabel->setText(QString::number(resources->at(Course::STONE)));
    ui_->oreLabel->setText(QString::number(resources->at(Course::ORE)));
    ui_->moneyLabel->setText(QString::number(resources->at(Course::MONEY)));

    if (currentItem_ != nullptr) {
        // Update tileinfo if player in turn is not the same anymore
        std::shared_ptr<PlayerBase> owner = currentItem_->getTileObject()->getOwner();
        std::string ownerName = "";

        if(owner != nullptr){
            ownerName = owner->getName();
        }
        updateTileInfo(QString::fromStdString(currentItem_->getTileObject()->
                                              getType()),
                       currentItem_, currentItem_->getItem(),
                       ownerName
                       );
    }
}

void MapWindow::updateBuildingCost()
{
    /* Get building cost and update it
     * Building button can change color based on this
     */
    QString path;

    if(ui_->buildingsBox->currentText() == FARM){
        setBuildingCost(Course::ConstResourceMaps::FARM_BUILD_COST);
        path = FARM_IMAGE;
    }else if(ui_->buildingsBox->currentText() == HQ){
        setBuildingCost(Course::ConstResourceMaps::HQ_BUILD_COST);
        path = HQ_IMAGE;
    }else if(ui_->buildingsBox->currentText() == OUTPOST){
        setBuildingCost(Course::ConstResourceMaps::OUTPOST_BUILD_COST);
        path = OUTPOST_IMAGE;
    }else if(ui_->buildingsBox->currentText() == MINE){
        setBuildingCost(Game::MINE_BUILD_COST);
        path = MINE_IMAGE;
    }else if(ui_->buildingsBox->currentText() == FISHING_BOAT){
        setBuildingCost(Game::FISHING_BOAT_BUILD_COST);
        path = SHIP_IMAGE;
    }else if(ui_->buildingsBox->currentText() == LAKE_COTTAGE){
        setBuildingCost(Game::LAKE_COTTAGE_BUILD_COST);
        path = LAKE_COTTAGE_IMAGE;
    }else{
        setBuildingCost(Game::RESOURCEMAP_ZERO);
        path = BUILDING_IMAGE;
    }

    ui_->buildCostLabel->setText("");
    ui_->selectedBuildingImage->setPixmap(QPixmap(path).
                                          scaled(50,50,Qt::KeepAspectRatio));
}
void MapWindow::setBuildingCost(ResourceMap resources)
{
    // Update amounts
    ui_->woodCostLabel->setText(QString::number(resources[WOOD]));
    ui_->FoodCostLabel->setText(QString::number(resources[FOOD]));
    ui_->stoneCostLabel->setText(QString::number(resources[STONE]));
    ui_->oreCostLabel->setText(QString::number(resources[ORE]));
    ui_->moneyCostLabel->setText(QString::number(resources[MONEY]));

    // Test if player has enough reources
    if(resources[WOOD] > ui_->woodLabel->text().toInt()){
        ui_->woodCostLabel->setStyleSheet("color: red;");
    }else{
        ui_->woodCostLabel->setStyleSheet("color: black;");
    }
    if(resources[FOOD] > ui_->foodLabel->text().toInt()){
        ui_->FoodCostLabel->setStyleSheet("color: red;");
    }else{
        ui_->FoodCostLabel->setStyleSheet("color: black;");
    }
    if(resources[STONE] > ui_->stoneLabel->text().toInt()){
        ui_->stoneCostLabel->setStyleSheet("color: red;");
    }else{
        ui_->stoneCostLabel->setStyleSheet("color: black;");
    }
    if(resources[ORE] > ui_->oreLabel->text().toInt()){
        ui_->oreCostLabel->setStyleSheet("color: red;");
    }else{
        ui_->oreCostLabel->setStyleSheet("color: black;");
    }
    if(resources[MONEY] > ui_->moneyLabel->text().toInt()){
        ui_->moneyCostLabel->setStyleSheet("color: red;");
    }else{
        ui_->moneyCostLabel->setStyleSheet("color: black;");
    }
}

void MapWindow::updateWorkersCost()
{
    double count = ui_->workerSpinBox->value();
    QString selectedWorker = ui_->workersBox->currentText();
    ResourceMap workerCost;
    ResourceMapDouble multiplyMap = {{BasicResource::MONEY, count},
                                     {BasicResource::FOOD, count},
                                     {BasicResource::ORE, count},
                                     {BasicResource::STONE, count},
                                     {BasicResource::WOOD,count}};

    if (selectedWorker == WORKER_BASIC) {
        workerCost = Course::ConstResourceMaps::BW_RECRUITMENT_COST;
    } else if (selectedWorker == WORKER_FARMER) {
        workerCost = Game::FARMER_RECRUITMENT_COST;
    } else if (selectedWorker == WORKER_MINER) {
        workerCost = Game::MINER_RECRUITMENT_COST;
    }
    /* Waiting for later implementation
     * else if (selectedWorker == WORKER_SABOTEUR) {

    }*/
    setWorkersCost(multiplyResourceMap(workerCost,multiplyMap));
}

void MapWindow::setWorkersCost(Course::ResourceMap resources)
{
    // Update amounts
    ui_->woodCostLabelWorkers->setText(QString::number(resources[WOOD]));
    ui_->FoodCostLabelWorkers->setText(QString::number(resources[FOOD]));
    ui_->stoneCostLabelWorkers->setText(QString::number(resources[STONE]));
    ui_->oreCostLabelWorkers->setText(QString::number(resources[ORE]));
    ui_->moneyCostLabelWorkers->setText(QString::number(resources[MONEY]));

    // Test if player has enough reources
    if(resources[WOOD] > ui_->woodLabel->text().toInt()){
        ui_->woodCostLabelWorkers->setStyleSheet("color: red;");
    }else{
        ui_->woodCostLabelWorkers->setStyleSheet("color: black;");
    }
    if(resources[FOOD] > ui_->foodLabel->text().toInt()){
        ui_->FoodCostLabelWorkers->setStyleSheet("color: red;");
    }else{
        ui_->FoodCostLabelWorkers->setStyleSheet("color: black;");
    }
    if(resources[STONE] > ui_->stoneLabel->text().toInt()){
        ui_->stoneCostLabelWorkers->setStyleSheet("color: red;");
    }else{
        ui_->stoneCostLabelWorkers->setStyleSheet("color: black;");
    }
    if(resources[ORE] > ui_->oreLabel->text().toInt()){
        ui_->oreCostLabelWorkers->setStyleSheet("color: red;");
    }else{
        ui_->oreCostLabelWorkers->setStyleSheet("color: black;");
    }
    if(resources[MONEY] > ui_->moneyLabel->text().toInt()){
        ui_->moneyCostLabelWorkers->setStyleSheet("color: red;");
    }else{
        ui_->moneyCostLabelWorkers->setStyleSheet("color: black;");
    }
}

void MapWindow::updateSellingValue()
{
    if(ui_->shopBox->currentIndex() == 0){
        return;
    }

    QString resource = ui_->shopBox->currentText();
    int value = 0;

    if(resource == "Wood" || resource == "Food" || resource == "Stone"){
        value = ui_->shopAmountBox->value();
    }
    if(resource == "Ore"){
        value = ui_->shopAmountBox->value() * 2;
    }

    ui_->shopCostLabel->setText("Value: " + QString::number(value) + " gold");
}

void MapWindow::updateWorkersInfo(Game::MapItem* &item)
{
    // Get data for workers amount etc.
    // Also update maximums for slider etc
    std::shared_ptr<TileBase> tile = objManager_->getTile(
                item->getTileObject()->getCoordinate());
    int maxWorkers = tile->MAX_WORKERS;
    ui_->workerSpinBox->setMaximum(maxWorkers-tile->getWorkerCount());
    ui_->workerSlider->setMaximum(maxWorkers-tile->getWorkerCount());
    ui_->workerCountBrowser->setText(QString::fromStdString(
                                   std::to_string(tile->getWorkerCount())
                                    + "/" + std::to_string(maxWorkers)));

    // Populate workers list
    ui_->workersOnTileList->clear();
    for(std::shared_ptr<WorkerBase> worker : tile->getWorkers()){
        ui_->workersOnTileList->addItem(QString::fromStdString(
                                            worker->getType()));
    }
    // Centers the text
    ui_->workerCountBrowser->setAlignment(Qt::AlignCenter);

    ui_->assignButton->setEnabled(true);
    ui_->freeButton->setEnabled(true);

    if(tile->getOwner() != nullptr){
        if(tile->getOwner()->getName() != gManager_->getCurrentPlayer()->getName()){
            ui_->assignButton->setEnabled(false);
            ui_->freeButton->setEnabled(false);
        }
    }
}

void MapWindow::updateBuildingsList(Game::MapItem* &item)
{
    std::shared_ptr<TileBase> tile = objManager_->getTile(
                item->getTileObject()->getCoordinate());
    std::vector<std::shared_ptr<BuildingBase>> tileBuildings = tile->getBuildings();
    ui_->buildingsOnTile->clear();

    for (std::shared_ptr<BuildingBase> building: tileBuildings) {
        ui_->buildingsOnTile->addItem(QString::fromStdString(
                                            building->getType()));
    }

    ui_->buildButton->setEnabled(true);
    ui_->deleteBuildingButton->setEnabled(true);

    if(tile->getOwner() != nullptr){
        if(tile->getOwner()->getName() != gManager_->getCurrentPlayer()->getName()){
            ui_->buildButton->setEnabled(false);
            ui_->deleteBuildingButton->setEnabled(false);
        }
    }
}

void MapWindow::updateTileInfo(const QString &type, Game::MapItem* &item,
                               const QPixmap &tileImage, std::string player)
{
    // MapItem -> Tile
    std::shared_ptr<TileBase> tile = objManager_->getTile(
                item->getTileObject()->getCoordinate());

    // Update the tile info box
    ui_->typeBrowser->setText(type);
    if (player == "") {
        ui_->ownerBrowser->setText("None");
    } else {
        if (QString::fromStdString(player) == ui_->currentPlayerNameLabel->text()) {
            player = "You";
        }
        ui_->ownerBrowser->setText(QString::fromStdString(player));
    }

    if (tile->getBuildingCount() == 0) {
        ui_->buildingsBrowser->setText("None");
    } else {
        ui_->buildingsBrowser->setText(QString::number(tile->getBuildingCount()));
    }

    if (tile->getWorkerCount() == 0) {
        ui_->workersBrowser->setText("None");
    } else {
        ui_->workersBrowser->setText(QString::number(tile->getWorkerCount()));
    }
    ui_->typeBrowser->setAlignment(Qt::AlignCenter);
    ui_->ownerBrowser->setAlignment(Qt::AlignCenter);
    ui_->buildingsBrowser->setAlignment(Qt::AlignCenter);
    ui_->workersBrowser->setAlignment(Qt::AlignCenter);
    ui_->claimCostLabel->setText("");

    // Remove border from previously clicked tile
    if(currentItem_ != nullptr && item->getTileObject()
            != currentItem_->getTileObject()){
		gScene_->highlightTile(currentItem_, false);
		gScene_->update(currentItem_->sceneBoundingRect());
    }
    // Highlight clicked tile
	gScene_->highlightTile(item, true);
	gScene_->update(item->sceneBoundingRect());

    // Set current mapitem
    currentItem_ = item;
    ui_->tileInfoLabel->setPixmap(tileImage.scaled(160,160,
                                                       Qt::KeepAspectRatio));
    ui_->claimButton->setStyleSheet("background-color: rgb(78, 154, 6);");

    updateTileProductionValues(item);
    updateWorkersInfo(item);
    updateBuildingsList(item);
}

void MapWindow::updateTileProductionValues(Game::MapItem* item)
{
    std::shared_ptr<TileBase> tile = objManager_->getTile(
                item->getTileObject()->getCoordinate());

    Course::ResourceMap resources = gManager_->calculateResourceProduction(tile);
    ui_->woodProductionLabel->setText(QString::number(resources[WOOD]));
    ui_->foodProductionLabel->setText(QString::number(resources[FOOD]));
    ui_->stoneProductionLabel->setText(QString::number(resources[STONE]));
    ui_->oreProductionLabel->setText(QString::number(resources[ORE]));
    ui_->moneyProductionLabel->setText(QString::number(resources[MONEY]));
}

void MapWindow::updateShopSlider(int value)
{
    if (value != 0) {
        ui_->shopAmountSlider->setValue(value);
    }
    ui_->shopAmountSlider->setValue(ui_->shopAmountBox->value());
}

void MapWindow::updateShopSpinBox(int value)
{
    if (value != 0) {
        ui_->shopAmountBox->setValue(value);
    }
    ui_->shopAmountBox->setValue(ui_->shopAmountSlider->value());
}

void MapWindow::updateWorkerSlider(int value)
{
    if (value != 0) {
        ui_->workerSlider->setValue(value);
    }
    ui_->workerSlider->setValue(ui_->workerSpinBox->value());
}

void MapWindow::updateWorkerSpinBox(int value)
{
    if (value != 0) {
        ui_->workerSpinBox->setValue(value);
    }
    ui_->workerSpinBox->setValue(ui_->workerSlider->value());

    updateWorkersCost();
}

void MapWindow::resetElements()
{
    ui_->buildingsBox->setCurrentIndex(0);
    ui_->buildCostLabel->setText("");

    ui_->shopBox->setCurrentIndex(0);
    ui_->shopCostLabel->setText("");
    ui_->shopAmountBox->setValue(0);
    ui_->shopAmountSlider->setValue(0);

    ui_->workersBox->setCurrentIndex(0);
    ui_->workerSlider->setValue(0);
    ui_->workerSpinBox->setValue(0);
    ui_->hireCostLabel->setText("");
}

void MapWindow::buildClicked()
{
    if (ui_->buildingsBox->currentIndex() == 0) {
        ui_->buildCostLabel->setText(NO_BUILDING_SELECTED);
        return;
    }

    if (currentItem_ == nullptr) {
        return;
    }

    try {
        gManager_->addBuildingOnTile(
                    currentItem_,ui_->buildingsBox->currentText());

        updatePlayerInfo(gManager_->getCurrentPlayer());
        updateTileInfo(QString::fromStdString(
                           currentItem_->getTileObject()->getType()),
                       currentItem_,
                       currentItem_->getItem(),
                       currentItem_->getTileObject()->getOwner()->getName());
        updateTileProductionValues(currentItem_);
        updateBuildingCost();

        ui_->buildCostLabel->setText(BUILDING_ADDED);
    }
    catch (const Course::BaseException &e){
        ui_->buildCostLabel->setText(QString::fromStdString(e.msg()));
    }
}

void MapWindow::removeBuildingClicked()
{
    int index = ui_->buildingsOnTile->currentRow();
    if(index == -1){
        return;
    }

    try{
        std::shared_ptr<TileBase> tile = objManager_->getTile(currentItem_->getTileObject()->getCoordinate());
        std::shared_ptr<BuildingBase> building = tile->getBuildings()[index];

        gManager_->removeBuildingOnTile(currentItem_, building);

        ui_->buildingsOnTile->clear();
        updateBuildingsList(currentItem_);
        updateTileProductionValues(currentItem_);
    }
    catch (const Course::BaseException &e){

    }
}

void MapWindow::sellClicked()
{
    // Selling resources
    if (ui_->shopBox->currentIndex() == 0 || !gameStarted_) {
        return;
    }

    // Prevents selling too many resources
    updateShopLimits();

    QString resource = ui_->shopBox->currentText();
    BasicResource resourceType = NONE;
    int value = 0;

    if(resource == "Wood"){
        value = ui_->shopAmountBox->value();
        resourceType = WOOD;
    } else if(resource == "Food"){
        value = ui_->shopAmountBox->value();
        resourceType = FOOD;
    } else if(resource == "Stone"){
        value = ui_->shopAmountBox->value();
        resourceType = STONE;
    } else if(resource == "Ore"){
        value = ui_->shopAmountBox->value() * 2;
        resourceType = ORE;
    }

    geHandler_->modifyResource(gManager_->getCurrentPlayer(), MONEY, value);
    geHandler_->modifyResource(gManager_->getCurrentPlayer(), resourceType, -value);

    ui_->shopAmountBox->setValue(0);
    ui_->shopBox->setCurrentIndex(0);

    updatePlayerInfo(gManager_->getCurrentPlayer());
}

void MapWindow::updateShopLimits()
{
    if(ui_->shopBox->currentIndex() == 0 || !gameStarted_){
        return;
    }

    QString resource = ui_->shopBox->currentText();

    // Set limits to sliders
    std::shared_ptr<Course::ResourceMap> playerResources = gManager_->getCurrentPlayer()->getResourceMap();
    int resourceCount = 0;

    if(resource == "Wood"){
        resourceCount = (*playerResources)[WOOD];
    } else if(resource == "Food"){
        resourceCount = (*playerResources)[FOOD];
    } else if(resource == "Stone"){
        resourceCount = (*playerResources)[STONE];
    } else if(resource == "Ore"){
        resourceCount = (*playerResources)[ORE];
    }

    ui_->shopAmountBox->setMaximum(resourceCount);
    ui_->shopAmountSlider->setMaximum(resourceCount);
}

void MapWindow::workerAssigned()
{
    if(ui_->workersBox->currentIndex() == 0){
        return;
    }

    // If tile selected
    if(currentItem_ == nullptr){
        return;
    }
    try{
        int loopValue = ui_->workerSpinBox->value();
        for(int i=0; i<loopValue; i++){
           gManager_->addWorkerOnTile(
                        currentItem_, ui_->workersBox->currentText());
            ui_->hireCostLabel->setText(WORKER_ADDED);
        }
    }
    catch (const Course::BaseException &e){
        ui_->hireCostLabel->setText(QString::fromStdString(e.msg()));
    }

    updatePlayerInfo(gManager_->getCurrentPlayer());
    updateTileProductionValues(currentItem_);
    updateWorkersInfo(currentItem_);
}

void MapWindow::claimClicked()
{
    // Pass information to gmanager
    if (currentItem_ == nullptr) {
        return;
    }

    try {
        gManager_->claimArea(currentItem_);

        updateTileInfo(QString::fromStdString(
                           currentItem_->getTileObject()->getType()),
                       currentItem_,
                       currentItem_->getItem(),
                       currentItem_->getTileObject()->getOwner()->getName());
        updatePlayerInfo(gManager_->getCurrentPlayer());
		gScene_->drawClaimBorders();
    } catch (const Course::BaseException &e){
        ui_->claimCostLabel->setText(QString::fromStdString(e.msg()));
        ui_->claimButton->setStyleSheet("background-color: red;");
    }
}

void MapWindow::freeWorkerClicked()
{
    int index = ui_->workersOnTileList->currentRow();
    if(index == -1){
        return;
    }

    try{
        std::shared_ptr<TileBase> tile = objManager_->getTile(currentItem_->getTileObject()->getCoordinate());
        std::shared_ptr<WorkerBase> worker = tile->getWorkers()[index];

        gManager_->removeWorkerOnTile(currentItem_, worker);

        ui_->workersOnTileList->clear();
        updateWorkersInfo(currentItem_);
        updateTileProductionValues(currentItem_);
    }
    catch (const Course::BaseException &e){

    }
}
