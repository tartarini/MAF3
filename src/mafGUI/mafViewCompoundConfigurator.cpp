/*
 *  mafViewCompoundConfigurator.cpp
 *  mafGUI
 *
 *  Created by Paolo Quadrani on 26/10/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafViewCompoundConfigurator.h"
#include "mafSplitter.h"

#include <mafObjectFactory.h>

using namespace mafEventBus;
using namespace mafGUI;

mafViewCompoundConfigurator::mafViewCompoundConfigurator() : m_RootObject(NULL) {
}

void mafViewCompoundConfigurator::parseDocument(QDomNode current, mafSplitter *parent) {
    QDomNodeList dnl = current.childNodes();
    for (int n=0; n < dnl.count(); ++n) {
        QDomNode node = dnl.item(n);
        if (node.nodeType() == QDomNode::ElementNode) {
            QDomElement ce = node.toElement();
            QDomNamedNodeMap attributes = ce.attributes();
            QString elem_name = ce.tagName();
            if (elem_name == "splitter") {
                mafSplitter *splitter = new mafSplitter(Qt::Horizontal, parent);
                if (parent == NULL) {
                    // parent is the root object, this is the first splitter created
                    m_RootObject = splitter;
                }
                QString orientation = attributes.namedItem("orientation").nodeValue();
                if (orientation == "vertical") {
                    // Check only for vertical, horizontal is the default.
                    splitter->setOrientation(Qt::Vertical);
                }
                QStringList sizesString = attributes.namedItem("sizes").nodeValue().split(",", QString::SkipEmptyParts);
                QList<int> sizes;
                sizes.append(sizesString[0].toInt());
                sizes.append(sizesString[1].toInt());
                splitter->setSizes(sizes);
                // Check if there are children and parse them...
                QDomNodeList splitter_dnl = node.childNodes();
                if (splitter_dnl.count() > 0) {
                    parseDocument(node, splitter);
                }
            } else if (elem_name == "view") {
                // We are at the leaf of the hierarchy tree: view objects are added at the parent splitter.
                QString viewClassType = attributes.namedItem("classtype").nodeValue();
                mafCore::mafObjectBase *obj = mafNEWFromString(viewClassType);
                parent->addView(obj);
                // The splitter retain the view object, so we can release the instance.
                mafDEL(obj);
            } else {
                qWarning() << mafTr("Unrecognized element named: ") << elem_name;
            }
        }
    }
}

bool mafViewCompoundConfigurator::parseConfigurationFile(QString configFile) {
    int errorLine, errorColumn;
    QString errorMsg;
    QFile cfgFile(configFile);
    if (!cfgFile.exists()) {
        qWarning() << configFile << " " << mafTr("doesn't exists. The default plug will be called.");
        return false;
    }

    QDomDocument document;
    if (!document.setContent(&cfgFile, &errorMsg, &errorLine, &errorColumn)) {
        QString error(mafTr("Syntax error line %1, column %2:\n%3. Default plug will be performed."));
        error = error
            .arg(errorLine)
            .arg(errorColumn)
            .arg(errorMsg);
        qCritical() << error;
        return false;
    }

    QDomNode currentNode = document.firstChild();
    parseDocument(currentNode, m_RootObject);
	return true;
}
