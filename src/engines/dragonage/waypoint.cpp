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
 *  A waypoint in a Dragon Age: Origins area.
 */

#include "src/common/util.h"
#include "src/common/maths.h"

#include "src/aurora/gff3file.h"
#include "src/aurora/2dareg.h"
#include "src/aurora/gdafile.h"

#include "src/engines/dragonage/waypoint.h"

namespace Engines {

namespace DragonAge {

Waypoint::Waypoint(const Aurora::GFF3Struct &waypoint) {
	load(waypoint);
}

Waypoint::~Waypoint() {
}

int32 Waypoint::getGroup() const {
	return _group;
}

bool Waypoint::hasMapNote() const {
	return _hasMapNote;
}

const Aurora::LocString &Waypoint::getMapNote() const {
	return _mapNote;
}

const Common::UString &Waypoint::getIcon() const {
	return _icon;
}

bool Waypoint::enabledMapNote() const {
	return _enabledMapNote;
}

void Waypoint::enableMapNote(bool enabled) {
	_enabledMapNote = enabled;
}

void Waypoint::load(const Aurora::GFF3Struct &waypoint) {
	_tag = waypoint.getString("Tag");

	_group = waypoint.getSint("Group", -1);

	_hasMapNote     = waypoint.getBool("HasMapNote");
	_enabledMapNote = waypoint.getBool("MapNoteEnabled");

	_type = (uint32) ((int32) waypoint.getSint("MapNoteType", -1));

	waypoint.getLocString("MapNote", _mapNote);

	if (waypoint.hasField("VarTable"))
		readVarTable(waypoint.getList("VarTable"));

	const float position[3] = {
		(float) waypoint.getDouble("XPosition"),
		(float) waypoint.getDouble("YPosition"),
		(float) waypoint.getDouble("ZPosition")
	};

	setPosition(position[0], position[1], position[2]);

	const float orientation[4] = {
		(float) waypoint.getDouble("XOrientation"),
		(float) waypoint.getDouble("YOrientation"),
		(float) waypoint.getDouble("ZOrientation"),
		(float) Common::rad2deg(acos(waypoint.getDouble("WOrientation")) * 2.0)
	};

	setOrientation(orientation[0], orientation[1], orientation[2], orientation[3]);

	const Aurora::GDAFile &gda = TwoDAReg.getGDA("waypointtypes");

	_icon = gda.getString(_type, "Icon");
}

} // End of namespace DragonAge

} // End of namespace Engines
