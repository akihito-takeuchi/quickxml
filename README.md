## QuickXml

Very simple C++ XML API using Qt library. This proejct is started to support programmer to implement XML access code in their project quickly.

This project currently supports XML data read only. No write function.g

## Requirement

This project uses [Qt C++](http://qt-project.org/) library. The development is done with Qt 4.8.4.
g
## Usage

Here is the example XML file, with the file name 'test.xml'.

```
<test>
  <info>
    <name>Book list</name>
    <description>Just sample</description>
    <contributor>Uncle Frog</contributor>
    <contributor>Mr. Lizard</contributor>
    <contributor>Super monkey</contributor>
    <contributor>Godzilla</contributor>
  </info>
  <book id="1">
    <title>Qt C++</title>
    <price>3000yen</price>
    <category>Programming</category>
  </book>
  <book id="2">
    <title>QuickXml</title>
    <price>100yen</price>
    <category>Programming</category>
  </book>
  <book id="3">
    <title>B-class gourmet selection</title>
    <price>10000yen</price>
    <category>Food</category>
  </book>
</test>
```

The XML data can be loaded using 'load' method. The argument is QIODevice, so you can use QFile in this case.

```
QFile file("test.xml");
file.open(QIODevice::Text | QIODevice::ReadOnly);

QuickXml xml;
if (!xml.load(&file)) {
    qDebug() << xml.errorString();
    // Error handling...
}
```

If you need the description of the file:

```
xml("info")("description").text();   // This returns "Just sample" (QString)
```

If you need the category of 3rd book:

```
xml("book")[2]("category").text();   // This returns "Food" (QString)
```

If you need the number of books:

```
xml("book").count();   // This returns "3" (int)
```

You like to loop for contributors:

```
foreach (QuickXml contributor, xml("info")("contributor")) {
    qDebug() << contributor.text();
}
```

To get the attribute:

```
xml("book")[2].attr("id");   // This returns "3" (int)
```

You can get the tag name using 'tagName()' method:

```
xml("info").tagName();    // This returns "info" (QString)...
xml.tagName();            // This returns "test"
```

You can find the samples in `tests/tst_quickxml/tst_quickxml.cpp`

## Future plan

Creating the plan.
