/*
 * libMaia - maiaObject.cpp
 * Copyright (c) 2003 Frerich Raabe <raabe@kde.org> and
 *                    Ian Reinhart Geiser <geiseri@kde.org>
 * Copyright (c) 2007 Sebastian Wiedenroth <wiedi@frubar.net>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "maiaObject.h"

MaiaObject::MaiaObject(QObject* parent) : QObject(parent){
	QDomImplementation::setInvalidDataPolicy(QDomImplementation::DropInvalidChars);
}

QDomElement MaiaObject::toXml(QVariant arg) {
	
	//dummy document
	QDomDocument doc;
	//value element, we need this in each case
	QDomElement tagValue = doc.createElement("value");

	/* qDebug("type: %d (%s)", arg.type(), arg.typeName()); */

	switch(arg.type()) {
	case QVariant::String: {

		QDomElement tagString = doc.createElement("string"); 
		QDomText textString = doc.createTextNode(arg.toString());
		
		tagValue.appendChild(tagString);
		tagString.appendChild(textString);

		return tagValue;

	} case QVariant::Int: {

		QDomElement tagInt = doc.createElement("int"); 
		QDomText textInt = doc.createTextNode(QString::number(arg.toInt()));
		
		tagValue.appendChild(tagInt);
		tagInt.appendChild(textInt);

		return tagValue;

	} case QVariant::Double: {

		QDomElement tagDouble = doc.createElement("double"); 
		QDomText textDouble = doc.createTextNode(QString::number(arg.toDouble()));
		
		tagValue.appendChild(tagDouble);
		tagDouble.appendChild(textDouble);

		return tagValue;

	} case QVariant::Bool: {
	
		QString textValue = arg.toBool() ? "1" : "0";

		QDomElement tag = doc.createElement("boolean"); 
		QDomText text = doc.createTextNode(textValue);
		
		tagValue.appendChild(tag);
		tag.appendChild(text);

		return tagValue;

	} case QVariant::ByteArray: {

		QString textValue = arg.toByteArray().toBase64();

		QDomElement tag = doc.createElement("base64"); 
		QDomText text = doc.createTextNode(textValue);
		
		tagValue.appendChild(tag);
		tag.appendChild(text);

		return tagValue;

	} case QVariant::DateTime: {
	
		QString textValue = arg.toDateTime().toString("yyyyMMddThh:mm:ss");

		QDomElement tag = doc.createElement("datetime.iso8601"); 
		QDomText text = doc.createTextNode(textValue);
		
		tagValue.appendChild(tag);
		tag.appendChild(text);

		return tagValue;

	} case QVariant::List: {

		QDomElement tagArray = doc.createElement("array");
		QDomElement tagData = doc.createElement("data");
		tagArray.appendChild(tagData);
		tagValue.appendChild(tagArray);

		const QList<QVariant> args = arg.toList();
		for(int i = 0; i < args.size(); ++i) {
			tagData.appendChild(toXml(args.at(i)));
		}
	
		return tagValue;

	} case QVariant::Map: {

		QDomElement tagStruct = doc.createElement("struct");
		QDomElement member;
		QDomElement name;

		tagValue.appendChild(tagStruct);

		QMap<QString, QVariant> map = arg.toMap();
		QMapIterator<QString, QVariant> i(map);
		while(i.hasNext()) {
			i.next();

			member = doc.createElement("member");
			name = doc.createElement("name");

			// (key) -> name -> member -> struct
			tagStruct.appendChild(member);
			member.appendChild(name);
			name.appendChild(doc.createTextNode(i.key()));

			// add variables by recursion
			member.appendChild(toXml(i.value()));
		}

		return tagValue;

	} default:
		qDebug() << "Failed to marshal unknown variant type: " << arg.type() << endl;
	}
	return QDomElement(); //QString::null;
}

QVariant MaiaObject::fromXml(const QDomElement &elem) {
	if(elem.tagName().toLower() != "value") {
		return QVariant();
	}
	
	// If no type is indicated, the type is string.
	if(!elem.firstChild().isElement()) {
		return QVariant(elem.text());
	}
	
	const QDomElement typeElement = elem.firstChild().toElement();	
	const QString typeName = typeElement.tagName().toLower();

	if(typeName == "string")
		return QVariant(typeElement.text());
	else if(typeName == "i4" || typeName == "int")
		return QVariant(typeElement.text().toInt());
	else if(typeName == "double")
		return QVariant(typeElement.text().toDouble());
	else if (typeName == "boolean") {
		if(typeElement.text().toLower() == "true" || typeElement.text() == "1")
			return QVariant(true);
		else
			return QVariant(false);
	} else if(typeName == "base64")
		return QVariant(QByteArray::fromBase64( typeElement.text().toLatin1()));
	else if(typeName == "datetime" || typeName == "datetime.iso8601")
		return QVariant(QDateTime::fromString(typeElement.text(), "yyyyMMddThh:mm:ss"));
	else if(typeName == "nil") // Non-standard extension: http://ontosys.com/xml-rpc/extensions.php
		return QVariant();
	else if ( typeName == "array" ) {
		QList<QVariant> values;
		QDomNode valueNode = typeElement.firstChild().firstChild();
		while(!valueNode.isNull()) {
			values << fromXml(valueNode.toElement());
			valueNode = valueNode.nextSibling();
		}
		return QVariant(values);
	}
	else if ( typeName == "struct" ) {
		QMap<QString, QVariant> map;
		QDomNode memberNode = typeElement.firstChild();
		while(!memberNode.isNull())	{
			const QString key = memberNode.toElement().elementsByTagName("name").item(0).toElement().text();
			const QVariant data = fromXml(memberNode.toElement().elementsByTagName("value").item(0).toElement());
			map[key] = data;
			memberNode = memberNode.nextSibling();
		}
		return QVariant(map);
	} else {
		qDebug() << "Cannot demarshal unknown type " << typeElement.tagName().toLower();
	}
	return QVariant();
}


QString MaiaObject::prepareCall(QString method, QList<QVariant> args) {
	

	QDomDocument doc;

	QDomProcessingInstruction header = doc.createProcessingInstruction( "xml", QString("version=\"1.0\" encoding=\"UTF-8\"" ));
	doc.appendChild(header);
	
	QDomElement methodCall = doc.createElement("methodCall");
	QDomElement methodName = doc.createElement("methodName");
	QDomElement params = doc.createElement("params");
	QDomElement param;

	doc.appendChild(methodCall);
	methodCall.appendChild(methodName);
	methodName.appendChild(doc.createTextNode(method));

	methodCall.appendChild(params);

	for(int i = 0; i < args.size(); ++i) {
		param = doc.createElement("param");
		param.appendChild(toXml(args.at(i)));
		params.appendChild(param);
	}

	return doc.toString();
}

QString MaiaObject::prepareResponse(QVariant arg) {

	QDomDocument doc;

	QDomProcessingInstruction header = doc.createProcessingInstruction( "xml", QString("version=\"1.0\" encoding=\"UTF-8\"" )); 
	doc.appendChild(header);
	
	QDomElement methodResponse = doc.createElement("methodResponse");
	QDomElement params = doc.createElement("params");
	QDomElement param;

	doc.appendChild(methodResponse);

	methodResponse.appendChild(params);

	if(!arg.isNull()) {
		param = doc.createElement("param");
		param.appendChild(toXml(arg));
		params.appendChild(param);
	}
	return doc.toString();
}

void MaiaObject::parseResponse(QString response, QNetworkReply* reply) {
	QDomDocument doc;
	QVariant arg;
	QString errorMsg;
	int errorLine;
	int errorColumn;
	if(!doc.setContent(response, &errorMsg, &errorLine, &errorColumn)) {
		emit fault(-32700, QString("parse error: response not well formed at line %1: %2").arg(errorLine).arg(errorMsg), reply);
		delete this;
		return;
	}
	if(doc.documentElement().firstChild().toElement().tagName().toLower() == "params") {
		QDomNode paramNode = doc.documentElement().firstChild().firstChild();
		if(!paramNode.isNull()) {
			arg = fromXml( paramNode.firstChild().toElement() );
		}
		emit aresponse(arg, reply);
	} else if(doc.documentElement().firstChild().toElement().tagName().toLower() == "fault") {
		const QVariant errorVariant = fromXml(doc.documentElement().firstChild().firstChild().toElement());
		emit fault(errorVariant.toMap() [ "faultCode" ].toInt(),
		           errorVariant.toMap() [ "faultString" ].toString(),
		           reply);
	} else {
		emit fault(-32600,
		           tr("parse error: invalid xml-rpc. not conforming to spec."),
		           reply);
	}
	delete this;
	return;
}

#include "moc_maiaObject.cpp"
