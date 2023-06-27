#include "test.h"

void Test::isEliminate_data()
{
    gamescene.loadFile("../demo01/test.txt");
    QTest::addColumn<int>("x1");
    QTest::addColumn<int>("x2");
    QTest::addColumn<int>("y1");
    QTest::addColumn<int>("y2");
    QTest::addColumn<bool>("output");
    QTest::addColumn<int>("character");
    QTest::addColumn<bool>("result");

    //不转弯
    QTest::newRow("1") << 5 << 6 << 1 << 1 << true << 1 << true; // x方向相邻相连
    QTest::newRow("2") << 3 << 5 << 1 << 1 << true << 1 << true; // x方向有空隙相连
    QTest::newRow("3") << 1 << 1 << 5 << 6 << true << 1 << true; // x方向相邻相连
    QTest::newRow("4") << 1 << 1 << 3 << 5 << true << 1 << true; // y方向有空隙相连
    // 转一次弯
    QTest::newRow("5") << 5 << 6 << 2 << 1 << true << 1 << true; // 对角上方
    QTest::newRow("6") << 5 << 6 << 2 << 3 << true << 1 << true; // 对角下方
    QTest::newRow("7") << 6 << 8 << 1 << 2 << true << 1 << true; // y相邻x间隔下方
    QTest::newRow("8") << 6 << 8 << 3 << 2 << true << 1 << true; // y相邻x间隔上方
    QTest::newRow("9") << 1 << 2 << 5 << 3 << true << 1 << true; // x相邻y间隔上方
    QTest::newRow("10") << 1 << 2 << 5 << 7 << true << 1 << true; // x相邻y间隔下方
    QTest::newRow("11") << 3 << 5 << 5 << 3 << true << 1 << true; // x间隔y间隔上方
    QTest::newRow("12") << 3 << 5 << 5 << 7 << true << 1 << true; // x间隔y间隔下方
    // 从1到2被阻
    QTest::newRow("13") << 5 << 6 << 7 << 6 << true << 1 << true; // 对角上方
    QTest::newRow("14") << 5 << 6 << 7 << 8 << true << 1 << true; // 对角下方
    QTest::newRow("15") << 6 << 8 << 4 << 5 << true << 1 << true; // y相邻x间隔下方
    QTest::newRow("16") << 6 << 8 << 6 << 5 << true << 1 << true; // y相邻x间隔上方
    QTest::newRow("17") << 1 << 2 << 9 << 7 << true << 1 << true; // x相邻y间隔上方
    QTest::newRow("18") << 1 << 2 << 9 << 11 << true << 1 << true; // x相邻y间隔下方
    QTest::newRow("19") << 3 << 5 << 11 << 9 << true << 1 << true; // x间隔y间隔上方
    QTest::newRow("20") << 3 << 5 << 11 << 13 << true << 1 << true; // x间隔y间隔下方

    // 转两次弯
    QTest::newRow("21") << 9 << 11 << 1 << 1 << true << 1 << true; // 同行两次弯上拐
    QTest::newRow("22") << 9 << 11 << 2 << 2 << true << 1 << true; // 同行两次弯下拐
    QTest::newRow("23") << 9 << 9 << 4 << 6 << true << 1 << true; // 同列两次弯右拐
    QTest::newRow("24") <<11 << 11 << 4 << 6 << true << 1 << true; // 同行两次弯左拐
    QTest::newRow("25") << 9 << 11 << 13 << 15 << true << 1 << true; // 不在同排上拐
    QTest::newRow("26") << 9 << 11 << 9 << 6 << true << 1 << true; // 不在同排右拐
    QTest::newRow("27") << 9 << 11 << 6 << 7 << true << 1 << true; // 不在同排下拐
    QTest::newRow("28") << 9 << 10 << 9 << 11 << true << 1 << true; // 不在同排左拐
    QTest::newRow("29") << 12 << 14 << 13 << 15 << true << 1 << true; // 先x的z型
    QTest::newRow("30") << 10 << 12 << 11 << 13 << true << 1 << true; // 先y的z型

    // 不能连接
    QTest::newRow("31") << 1 << 1 << 1 << 1 << true << 1 << false; // 消自己
    QTest::newRow("32") << 1 << 2 << 1 << 1 << true << 1 << false; // 不同种类相连
    QTest::newRow("33") << 1 << 2 << 13 << 12 << true << 1 << false; // 对角上方被阻
    QTest::newRow("34") << 1 << 2 << 12 << 13 << true << 1 << false; // 对角下方被阻
    QTest::newRow("35") << 3 << 5 << 14 << 15 << true << 1 << false; // y相邻x间隔下方被阻
    QTest::newRow("36") << 1 << 3 << 15 << 14 << true << 1 << false; // y相邻x间隔上方被阻
    QTest::newRow("37") << 6 << 7 << 9 << 7 << true << 1 << false; // x相邻y间隔上方被阻
    QTest::newRow("38") << 6 << 7 << 9 << 11 << true << 1 << false; // x相邻y间隔下方被阻
    QTest::newRow("39") << 5 << 7 << 13 << 11 << true << 1 << false; // x间隔y间隔上方被阻
    QTest::newRow("40") << 5 << 7 << 9 << 11 << true << 1 << false; // x间隔y间隔下方被阻
    QTest::newRow("41") << 1 << 3 << 9 << 1 << true << 1 << false; // 三次弯
    QTest::newRow("42") << 3 << 8 << 11 << 5 << true << 1 << false; // 四次弯
}

void Test::isEliminate()
{

    QFETCH(int,x1);
    QFETCH(int,x2);
    QFETCH(int,y1);
    QFETCH(int,y2);
    QFETCH(bool,output);
    QFETCH(int,character);
    QFETCH(bool,result);

    QCOMPARE(gamescene.isEliminated(x1, x2, y1, y2, output, character), result);
}

//QTEST_MAIN(Test)
