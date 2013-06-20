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

#ifndef QUICKXML_H
#define QUICKXML_H

#include <QString>
#include <QList>
#include <QSharedPointer>

class QIODevice;
class QuickXml;
class QuickXmlPrivate;

class QuickXml
{
public:
    typedef QList<QuickXml>::const_iterator const_iterator;
    QuickXml();
    QuickXml(const QuickXml& other);
    virtual ~QuickXml();
    virtual QuickXml& operator()(const QString& tagName);
    virtual QuickXml& operator[](int index);
    virtual QuickXml& operator()(const QString& tagName) const;
    virtual QuickXml& operator[](int index) const;
    virtual QString tagName() const;
    virtual QString text() const;
    virtual QString attr(const QString& attrName) const;
    virtual int count() const;
    virtual QString errorString() const;
    virtual bool load(QIODevice* device);
    virtual const_iterator begin() const;
    virtual const_iterator end() const;

private:
    typedef QSharedPointer<QuickXmlPrivate> QuickXmlPrivatePtr;
    QuickXmlPrivatePtr _p;
    friend class QuickXmlPrivate;
};

#endif
