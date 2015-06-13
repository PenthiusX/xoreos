/* xoreos - A reimplementation of BioWare's Aurora engine
 *
 * xoreos is the legal property of its developers, whose names
 * can be found in the AUTHORS file distributed with this source
 * distribution.
 *
 * xoreos is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3
 * of the License, or (at your option) any later version.
 *
 * xoreos is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with xoreos. If not, see <http://www.gnu.org/licenses/>.
 */

/** @file
 *  XML parsing helpers, using libxml2.
 */

#include <cstdarg>
#include <cstdio>

#include <libxml/parser.h>
#include <libxml/xmlerror.h>

#include "src/common/xml.h"
#include "src/common/util.h"
#include "src/common/error.h"
#include "src/common/stream.h"

namespace Common {

static void errorFuncThrow(void *UNUSED(ctx), const char *msg, ...) {
	char buf[STRINGBUFLEN];
	va_list va;

	va_start(va, msg);
	vsnprintf(buf, STRINGBUFLEN, msg, va);
	va_end(va);

	throw Exception("libxml2 error: %s", buf);
}

static int readStream(void *context, char *buffer, int len) {
	SeekableReadStream *stream = (SeekableReadStream *) context;
	if (!stream)
		return -1;

	return stream->read(buffer, len);
}

static int closeStream(void *UNUSED(context)) {
	return 0;
}

void initXML() {
	xmlSetGenericErrorFunc(0, errorFuncThrow);

	// Initialize libxml2 and make sure the library version matches
	LIBXML_TEST_VERSION
}

void deinitXML() {
	xmlCleanupParser();
}

XMLParser::XMLParser(SeekableReadStream &stream, bool makeLower) : _rootNode(0) {
	xmlDocPtr xml = 0;

	try {

		const int options = XML_PARSE_NOWARNING | XML_PARSE_NOBLANKS | XML_PARSE_NONET |
		                    XML_PARSE_NSCLEAN   | XML_PARSE_NOCDATA;

		xml = xmlReadIO(readStream, closeStream, (void *) &stream, "stream.xml", 0, options);
		if (!xml)
			throw Exception("XML document failed to parse");

		xmlNodePtr root = xmlDocGetRootElement(xml);
		if (!root)
			throw Exception("XML document has no root node");

		_rootNode = new XMLNode(*root, makeLower);

	} catch (...) {
		delete _rootNode;

		xmlFreeDoc(xml);
		throw;
	}

	xmlFreeDoc(xml);
}

XMLParser::~XMLParser() {
	delete _rootNode;
}

const XMLNode &XMLParser::getRoot() const {
	return *_rootNode;
}


XMLNode::XMLNode(_xmlNode &node, bool makeLower, XMLNode *parent) : _parent(parent) {
	try {

		load(node, makeLower);

	} catch (...) {
		clean();

		throw;
	}
}

XMLNode::~XMLNode() {
	clean();
}

void XMLNode::clean() {
	for (Children::iterator c = _children.begin(); c != _children.end(); ++c)
		delete *c;
}

const UString &XMLNode::getName() const {
	return _name;
}

const UString &XMLNode::getContent() const {
	return _content;
}

const XMLNode *XMLNode::getParent() const {
	return _parent;
}

const XMLNode::Children &XMLNode::getChildren() const {
	return _children;
}

const XMLNode *XMLNode::findChild(const UString &name) const {
	ChildMap::const_iterator child = _childMap.find(name);
	if (child != _childMap.end())
		return child->second;

	return 0;
}

const XMLNode::Properties &XMLNode::getProperties() const {
	return _properties;
}

const UString &XMLNode::getProperty(const UString &name, const UString &def) const {
	Properties::const_iterator property = _properties.find(name);
	if (property != _properties.end())
		return property->second;

	return def;
}

void XMLNode::load(_xmlNode &node, bool makeLower) {
	_name    = node.name    ? (const char *) node.name    : "";
	_content = node.content ? (const char *) node.content : "";

	if (makeLower)
		_name.makeLower();

	for (xmlAttrPtr attrib = node.properties; attrib; attrib = attrib->next) {
		Common::UString name (attrib->name     ? (const char *) attrib->name              : "");
		Common::UString value(attrib->children ? (const char *) attrib->children->content : "");

		if (makeLower)
			name.makeLower();

		_properties.insert(std::make_pair(name, value));
	}

	for (xmlNodePtr child = node.children; child; child = child->next) {
		_children.push_back(new XMLNode(*child, makeLower, this));

		_childMap.insert(std::make_pair(_children.back()->getName(), _children.back()));
	}
}

} // End of namespace Common
