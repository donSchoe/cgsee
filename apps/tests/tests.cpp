#include <glm/glm.hpp>

#include <QtTest/QtTest>
#include <QString>
#include <QMetaType>

#include "tests.h"

glm::vec3 pointsArray[] = {
    glm::vec3()
,   glm::vec3(1.0f)
,   glm::vec3(0.5f, 0.5f, 0.8f)
,   glm::vec3(-1.0f)
,   glm::vec3(2.0f, 3.0f, 4.0f)
};

template <typename iter_type>
void populateVListFrom(t_VertexListP vList, iter_type start, iter_type end, QString attrName)
{
    typedef typename std::iterator_traits<iter_type>::value_type attributeType;

    vList->setVertexAttributes<attributeType>(0, std::distance(start, end), attrName,
        [&](int i, attributeType& value)
        {
            value = *start++;
        });
}

void DataCoreTest::initVertexList()
{
    QList<AttributeSpec> attributeSpec;
    qRegisterMetaType<glm::vec3>("glm::vec3");
    qRegisterMetaType<glm::vec2>("glm::vec2");
    attributeSpec.append(AttributeSpec("position", "glm::vec3"));
    attributeSpec.append(AttributeSpec("normal", "glm::vec3"));
    attributeSpec.append(AttributeSpec("texcoord", "glm::vec2"));
    vertexList->initialize(attributeSpec);
}

void DataCoreTest::initTestCase()
{
    vertexList = DataBlock::createDataBlockWithName<VertexList> ("vertex", registry);
    QVERIFY(vertexList);
    QVERIFY(vertexList->objectName() == "vertex");
    QVERIFY(registry.getDataBlockByName("vertex") == vertexList);

    initVertexList();
}

void DataCoreTest::init()
{
    vertexList->resize(5);

    populateVListFrom(vertexList, static_cast<glm::vec3*>(pointsArray), &pointsArray[5], "position");
}

void DataCoreTest::cleanup()
{
    vertexList->resize(0);
    QVERIFY(vertexList->size() == 0);
}

void DataCoreTest::writeReadTest()
{
    // Some values have been written by init function, lets try to read them
    QVERIFY(vertexList->size() == 5);

    glm::vec3 * currentValue = vertexList->getVertexAttribute<glm::vec3>(2, "position");
    QVERIFY(currentValue != nullptr);
    QVERIFY(glm::distance(*currentValue, pointsArray[2]) < 0.0001);
    currentValue = vertexList->getVertexAttribute<glm::vec3>(3, "position");
    QVERIFY(currentValue != nullptr);
    QVERIFY(glm::distance(*currentValue, pointsArray[3]) < 0.0001);

    // try to read false type
    glm::vec2 * wrongValue = vertexList->getVertexAttribute<glm::vec2>(2, "position");
    QVERIFY(wrongValue == nullptr);

    // Try to read using SFML function (single function multiple data)
    unsigned int readTimes = 0;
    vertexList->foreachVertexAttribute<glm::vec3>(0, vertexList->size(), "position", nullptr,
        [&](unsigned int i, const glm::vec3 &value)
        {
            QVERIFY(i < vertexList->size());
            QVERIFY(glm::distance(value, pointsArray[i]) < 0.0001);
            ++readTimes;
        });
    QVERIFY(readTimes == 5);
}

void DataCoreTest::sillyTest()
{
    glm::vec3 * pos1 = vertexList->getVertexAttribute<glm::vec3>(0, "position");
    QVERIFY(pos1 != nullptr);
    (*pos1) = glm::vec3(1.0f, 2.0f, 3.0f); // you can now safely access the vertex attribute, but thats not easy
};

void DataCoreTest::iteratorTest()
{
    AttributeIterator<glm::vec3> start(vertexList->begin<glm::vec3>("position"));
    AttributeIterator<glm::vec3> end(vertexList->end<glm::vec3>("position"));
    unsigned int currentIdx = 0;

    for (;start != end; ++start, ++currentIdx)
    {
        QVERIFY(glm::distance(*start, pointsArray[currentIdx]) < 0.0001);
    }
}

void DataCoreTest::constIteratorTest()
{
    VertexList const * constVList(vertexList);

    AttributeIterator<const glm::vec3> start, end(constVList->end<glm::vec3>("position"));
    unsigned int currentIdx = 0;

    for (start=vertexList->begin<glm::vec3>("position"); start != end; ++start, ++currentIdx)
    {
        QVERIFY(glm::distance(*start, pointsArray[currentIdx]) < 0.0001);
    }
}

void DataCoreTest::cleanupTestCase()
{
    DataBlock::destroyDataBlock<VertexList>(vertexList);
    vertexList = nullptr;

    QVERIFY(registry.getDataBlockByName("vertex") == nullptr);
}

QTEST_MAIN(DataCoreTest);
