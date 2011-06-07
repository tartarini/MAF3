/*
 *  mafPluginConfigurator.cpp
 *  mafPluginVTK
 *
 *  Created by Paolo Quadrani on 06/06/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafPluginConfigurator.h"

using namespace mafEventBus;
using namespace mafResources;

void mafPluginConfigurator::parseConfigurationFile(QDomNode current) {
    mafEventBus::mafEventArgumentsList argList;
    mafCore::mafPluggedObjectsHash pluginHash;
    char *name = current.nodeName().toAscii().data();
    QDomNodeList dnl = current.childNodes();
    for (int n=0; n < dnl.count(); ++n) {
        QDomNode node = dnl.item(n);
        if (node.nodeType() == QDomNode::ElementNode) {
            QDomElement ce = node.toElement();
            QDomNamedNodeMap attributes = ce.attributes();
            QString elem_name = ce.tagName();
            if (elem_name == "plug") {
                QString label = attributes.namedItem("label").nodeValue();
                QString classType = attributes.namedItem("classtype").nodeValue();
                QString baseClass = attributes.namedItem("baseclass").nodeValue();
                mafCore::mafPluggedObjectInformation plugInfo(label, classType);
                pluginHash.insertMulti(baseClass, plugInfo);
            } else if (elem_name == "codec") {
                argList.clear();
                QString encodeType = attributes.namedItem("encodetype").nodeValue();
                QString codec = attributes.namedItem("classtype").nodeValue();
                argList.append(mafEventArgument(QString, encodeType));
                argList.append(mafEventArgument(QString, codec));
                mafEventBusManager::instance()->notifyEvent("maf.local.serialization.plugCodec", mafEventTypeLocal, &argList);
            } else if (elem_name == "serializer") {
                argList.clear();
                QString schemaType = attributes.namedItem("schematype").nodeValue();
                QString serializer = attributes.namedItem("classtype").nodeValue();
                argList.append(mafEventArgument(QString, schemaType));
                argList.append(mafEventArgument(QString, serializer));
                mafEventBusManager::instance()->notifyEvent("maf.local.serialization.plugSerializer", mafEventTypeLocal, &argList);
            } else {
                qWarning() << mafTr("Unrecognized element named: ") << elem_name;
            }
        }
    }

    if (pluginHash.size() != 0) {
        argList.clear();
        argList.append(mafEventArgument(mafCore::mafPluggedObjectsHash, pluginHash));
        mafEventBusManager::instance()->notifyEvent("maf.local.resources.plugin.registerLibrary", mafEventTypeLocal, &argList);
    }
}

bool mafPluginConfigurator::parseConfigurationFile(QString configFile) {
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
    parseConfigurationFile(currentNode);
	return true;
}
