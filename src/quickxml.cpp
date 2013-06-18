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
#include <QMap>
#include <QXmlStreamReader>

struct QuickXmlPrivate
{
    static QString getTagName(QXmlStreamReader& xml)
    {
        return xml.name().toString();
    }

    bool createChild(QXmlStreamReader &xml)
    {
        QuickXml child;
        child._p->parent = this;
    
        childrenByTagName[getTagName(xml)].append(child);
        children.append(child);
        return child._p->load(xml);
    }

    bool load(QXmlStreamReader &xml)
    {
        tagName = getTagName(xml);
    
        foreach (QXmlStreamAttribute attr, xml.attributes()) {
            attrs[attr.name().toString()] = attr.value().toString();
        }

        while (!xml.atEnd() && !xml.hasError()) {
            QXmlStreamReader::TokenType tokenType = xml.readNext();
            switch (tokenType) {
            case QXmlStreamReader::Invalid:
            case QXmlStreamReader::NoToken:
            case QXmlStreamReader::StartDocument:
                errorString = QString("Unexpected state found at line %1, column %2")
                    .arg(xml.lineNumber()).arg(xml.columnNumber());
                return false;
                
            case QXmlStreamReader::EndDocument:
            case QXmlStreamReader::Comment:
            case QXmlStreamReader::EntityReference:
            case QXmlStreamReader::ProcessingInstruction:
            case QXmlStreamReader::DTD:
                break;
                
            case QXmlStreamReader::StartElement:
                if (!createChild(xml)) {
                    return false;
                }
                break;
                
            case QXmlStreamReader::EndElement:
                return true;
                
            case QXmlStreamReader::Characters:
                text = xml.text().toString();
                break;
            }
        }
        errorString = QString("Could not find the end of element '%1'")
            .arg(tagName);
        return false;
    }

    QString tagName;
    QString text;
    QuickXmlPrivate* parent;
    QList<QuickXml> children;
    QMap<QString, QList<QuickXml> > childrenByTagName;
    QMap<QString, QString> attrs;
    QString errorString;
};

QuickXml::QuickXml()
{
    _p = QuickXmlPrivatePtr(new QuickXmlPrivate);
    _p->parent = 0;
}

QuickXml::QuickXml(const QuickXml& other)
{
    _p = other._p;
}

QuickXml::~QuickXml()
{
}

QuickXml& QuickXml::operator()(const QString& tagName)
{
    return _p->childrenByTagName[tagName][0];
}

QuickXml& QuickXml::operator[](int index)
{
    return _p->parent->childrenByTagName[_p->tagName][index];
}

QString QuickXml::tagName() const
{
    return _p->tagName;
}

QString QuickXml::text() const
{
    return _p->text;
}

QString QuickXml::attr(const QString& attrName) const
{
    return _p->attrs[attrName];
}

int QuickXml::count() const
{
    if (_p->parent == 0) {
        return 1;
    }
    return _p->parent->childrenByTagName[_p->tagName].count();
}

QString QuickXml::errorString() const
{
    if (_p->errorString.isEmpty()) {
        foreach (QuickXml child, _p->children) {
            QString msg = child.errorString();
            if (!msg.isEmpty()) {
                return msg;
            }
        }
    }
    return _p->errorString;
}

QuickXml::const_iterator QuickXml::begin() const
{
    return _p->parent->childrenByTagName[_p->tagName].begin();
}

QuickXml::const_iterator QuickXml::end() const
{
    return _p->parent->childrenByTagName[_p->tagName].end();
}

bool QuickXml::load(QIODevice* device)
{
    QXmlStreamReader xml(device);
    while (!xml.atEnd() && !xml.hasError()) {
        QXmlStreamReader::TokenType tokenType = xml.readNext();
        switch (tokenType) {
        case QXmlStreamReader::Invalid:
        case QXmlStreamReader::NoToken:
        case QXmlStreamReader::EndDocument:
        case QXmlStreamReader::EndElement:
        case QXmlStreamReader::Characters:
            _p->errorString = "Unexpected state found at ";
            _p->errorString += QString("line %1, column %2")
                .arg(xml.lineNumber()).arg(xml.columnNumber());
            return false;

        case QXmlStreamReader::Comment:
        case QXmlStreamReader::EntityReference:
        case QXmlStreamReader::ProcessingInstruction:
        case QXmlStreamReader::DTD:
        case QXmlStreamReader::StartDocument:
            break;

        case QXmlStreamReader::StartElement:
            return _p->load(xml);
        }
    }
    _p->errorString = "Can't find the start of the xml document.";
    return false;
}
