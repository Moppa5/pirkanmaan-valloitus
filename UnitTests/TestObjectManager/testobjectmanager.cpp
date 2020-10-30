#include <QString>
#include <QtTest>
#include <interfaces/objectmanager.hh>
#include <interfaces/gameeventhandler.hh>
#include <workers/basicworker.h>
#include <buildings/farm.h>
#include <tiles/grassland.h>

using namespace Game;

/**
 * @brief The TestObjectManager class is for unit testing
 * ObjectManager class. It tests that the methods work properly
 * under valid conditions.
 */
class TestObjectManager : public QObject
{
    Q_OBJECT

public:

    /**
     * @brief Constructs the test environment
     * Creates ObjectManager and GameEventHandler
     * Creates a few test tiles
     */
    TestObjectManager();

private:
    std::shared_ptr<ObjectManager> objManager = nullptr;
    std::shared_ptr<GameEventHandler> geHandler = nullptr;
    std::vector<std::shared_ptr<Course::TileBase> > tiles;
    std::vector<Course::Coordinate> coordinateVec;

    std::shared_ptr<Course::TileBase> testTile = nullptr;
    std::shared_ptr<Course::TileBase> anotherTile = nullptr;

private Q_SLOTS:

    /**
     * @brief Tests if the tiles added in constructor matches
     */
    void testGetTiles();

    /**
     * @brief Tests if the getTiles coordinate method matches with
     * CoordinateVec based values
     */
    void testGetTile();

    /**
     * @brief Tests if the  building removing is successful
     * Uses testTile created in constructor
     * Creates Player for tile
     * Also tests ObjectManager addBuilding by adding the building to the vector
     */
    void testRemoveBuilding();

    /**
     * @brief Tests if the worker removing is successful
     * Creates Player for tile
     * Uses anotherTile created in constructor
     * Also tests ObjectManager addWorker by adding the worker to the vector
     */
    void testRemoveWorker();
};

TestObjectManager::TestObjectManager()
{
    objManager = std::make_shared<ObjectManager>();
    geHandler = std::make_shared<GameEventHandler>();

    // Create a few test tiles for unit tests

    testTile = std::make_shared<TileBase>(
                Course::Coordinate(0,0),
                geHandler,
                objManager
                );

    anotherTile = std::make_shared<Grassland>(
                Course::Coordinate(1,3),
                geHandler,
                objManager
                );

    coordinateVec.push_back(Coordinate(0,0));
    tiles.push_back(testTile);
    tiles.push_back(anotherTile);

    // Technically tests also addTiles now
    objManager->addTiles(tiles);
}

void TestObjectManager::testGetTiles()
{
    /* Vectors should be the same now
     * Also addTiles works correctly
     */
    QVERIFY(tiles == objManager->getTiles());
}

void TestObjectManager::testGetTile()
{
    coordinateVec.push_back(Course::Coordinate(1,3));
    QVERIFY(tiles == objManager->getTiles(coordinateVec));
}

void TestObjectManager::testRemoveBuilding()
{
    std::shared_ptr<Player> testPlayer = std::make_shared<Player>("PlayerTest");
    std::shared_ptr<Course::Farm> testBuilding = std::make_shared<Course::Farm>(
                geHandler,
                objManager,
                testPlayer
                );
    testBuilding->setCoordinate(Course::Coordinate(0,0));
    testTile->addBuilding(testBuilding);
    objManager->addBuilding(testBuilding);

    objManager->removeBuilding(testBuilding);
}

void TestObjectManager::testRemoveWorker()
{
    std::shared_ptr<Player> testPlayer = std::make_shared<Player>("PlayerTest");
    anotherTile->setOwner(testPlayer);
    std::shared_ptr<Course::BasicWorker> unSuccessfulWorker = std::make_shared
            <Course::BasicWorker>(
                geHandler,
                objManager,
                testPlayer
                );
    unSuccessfulWorker->setCoordinate(Course::Coordinate(1,3));
    anotherTile->addWorker(unSuccessfulWorker);
    objManager->addWorker(unSuccessfulWorker);

    objManager->removeWorker(unSuccessfulWorker);
}

QTEST_APPLESS_MAIN(TestObjectManager)

#include "testobjectmanager.moc"
