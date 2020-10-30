#include <QString>
#include <QtTest>
#include <interfaces/gameeventhandler.hh>
#include "core/player.hh"

using namespace Game;

/**
 * @brief The TestGameEventHandler class is for unit testing
 * GameEventHandler class. It tests that the methods work properly
 * under valid conditions.
 */
class TestGameEventHandler : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Creates the GameEventHandler to be tested
     */
    TestGameEventHandler();

private:
    GameEventHandler* geHandler;

    const Course::ResourceMap RM1{
        {WOOD, 10},
        {FOOD, 20},
        {STONE, 30},
        {ORE, 40},
        {MONEY, 50}
    };
    const Course::ResourceMap RMEmpty{
    };

private Q_SLOTS:

    /**
     * @brief Tests modifyResources if it's successful
     * RM1 resourcemap with parameter false so no changes
     * RM1 with parameter true so resources change
     * Empty resourcemap does nothing
    */
    void testModifyResources();

    /**
     * @brief Tests modifyResource if it's succesful
     * False case should not make any changes
     * True case with negative result => no changes
     * True case with valid result => changes made
     */
    void testModifyResource();

    /**
     * @brief Tests if the resourcemap to be applied for a player is
     * valid. No negative resources allowed to be changed
     */
    void testValidResourceMap();

};

TestGameEventHandler::TestGameEventHandler()
{
    geHandler = new GameEventHandler();
}

void TestGameEventHandler::testModifyResources()
{
    std::vector<std::shared_ptr<Player>> players;
    std::shared_ptr<Player> player = std::make_shared<Player>("name");
    players.push_back(player);
    geHandler->setPlayers(players);

    // Should not apply change
    ResourceMap oldResources = *(player->getResourceMap());
    geHandler->modifyResources(player, RM1, false);
    QVERIFY(*(player->getResourceMap()) == oldResources);

    // Should apply change
    geHandler->modifyResources(player, RM1, true);
    QVERIFY(*(player->getResourceMap()) == RM1);

    // Applying empty should not change resources
    geHandler->modifyResources(player, RMEmpty, true);
    QVERIFY(*(player->getResourceMap()) == RM1);
}

void TestGameEventHandler::testModifyResource()
{
    std::vector<std::shared_ptr<Player>> players;
    std::shared_ptr<Player> player = std::make_shared<Player>("name");
    players.push_back(player);
    geHandler->setPlayers(players);

    // Should not apply change
    ResourceMap oldResources = *(player->getResourceMap());
    geHandler->modifyResource(player, WOOD, 0, false);
    QVERIFY(*(player->getResourceMap()) == oldResources);
    geHandler->modifyResource(player, WOOD, 20, false);
    QVERIFY(*(player->getResourceMap()) == oldResources);

    // Should apply change if valid
    geHandler->modifyResource(player, WOOD, -100, true);
    QVERIFY(*(player->getResourceMap()) == oldResources);
    geHandler->modifyResource(player, WOOD, 1.5, true);
    QVERIFY((*(player->getResourceMap()))[WOOD] == 1);
}

void TestGameEventHandler::testValidResourceMap()
{    
    std::vector<std::shared_ptr<Player>> players;
    std::shared_ptr<Player> player = std::make_shared<Player>("name");
    players.push_back(player);
    geHandler->setPlayers(players);

    QVERIFY(geHandler->modifyResource(player, WOOD, 0, false) == true);

    QVERIFY(geHandler->modifyResource(player, WOOD, 20, false) == true);

    QVERIFY(geHandler->modifyResource(player, WOOD, -100, true) == false);

    QVERIFY(geHandler->modifyResource(player, WOOD, 1.5, true) == true);
}

QTEST_APPLESS_MAIN(TestGameEventHandler)

#include "testgameeventhandler.moc"
