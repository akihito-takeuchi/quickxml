/****************************************************************************
**
** The MIT License
**
** Copyright (c) 2013 Akihito Takeuchi
**
** Permission is hereby granted, free of charge, to any person obtaining a copy
** of this software and associated documentation files (the "Software"), to deal
** in the Software without restriction, including without limitation the rights
** to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
** copies of the Software, and to permit persons to whom the Software is
** furnished to do so, subject to the following conditions:
**
** The above copyright notice and this permission notice shall be included in
** all copies or substantial portions of the Software.
**
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
** AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
** FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
** IN THE SOFTWARE.
**
****************************************************************************/

#include "quickxml.h"
#include <QFile>
#include <QtTest>

class tst_QuickXml : public QObject
{
    Q_OBJECT;
private slots:
    void initTestCases();
    void cleanupTestCase();
    void quickXmlReadTest();
    void foreachTest();
private:
    QuickXml xml;
};

void tst_QuickXml::initTestCases()
{
    QFile file("testdata/test.xml");
    if (!file.open(QIODevice::Text | QIODevice::ReadOnly)) {
        qDebug() << "File:" << file.fileName();
        qDebug() << file.errorString();
        QVERIFY(0);
    }
    if (!xml.load(&file)) {
        qDebug() << xml.errorString();
        QVERIFY(0);
    }
}

void tst_QuickXml::cleanupTestCase()
{
}

void tst_QuickXml::quickXmlReadTest()
{
    QCOMPARE(xml.tagName(), QString("test"));
    QCOMPARE(xml("book").count(), 3);
    QCOMPARE(xml("info")("contributor").count(), 4);
    QCOMPARE(xml("info").tagName(), QString("info"));

    QCOMPARE(xml("info")("name").text(), QString("Book list"));
    QCOMPARE(xml("info")("contributor")[1].text(), QString("Mr. Lizard"));
    QCOMPARE(xml("info")("contributor")[3].text(), QString("Godzilla"));
    QCOMPARE(xml("book")[0].attr("id"), QString("1"));
    QCOMPARE(xml("book")[1]("price").text(), QString("100yen"));
}

void tst_QuickXml::foreachTest()
{
    QString contributorResult;
    foreach (QuickXml node, xml("info")("contributor")) {
        contributorResult += QString("%1:%2\n").arg(node.tagName()).arg(node.text());
    }
    QString contributorExpect;
    contributorExpect += "contributor:Uncle Frog\n";
    contributorExpect += "contributor:Mr. Lizard\n";
    contributorExpect += "contributor:Super monkey\n";
    contributorExpect += "contributor:Godzilla\n";
    QCOMPARE(contributorResult, contributorExpect);

    QuickXml& bookElements = xml("book");
    QString bookTitleResult;
    foreach (QuickXml node, bookElements) {
        bookTitleResult += QString("id%1:%2\n")
            .arg(node.attr("id")).arg(node("title").text());
    }
    QString bookTitleExpect;
    bookTitleExpect += "id1:Qt C++\n";
    bookTitleExpect += "id2:QuickXml\n";
    bookTitleExpect += "id3:B-class gourmet selection\n";
    QCOMPARE(bookTitleResult, bookTitleExpect);
}

QTEST_MAIN(tst_QuickXml);

#include "tst_quickxml.moc"
