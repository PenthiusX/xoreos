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
 *  The context needed to run a The Witcher module.
 */

#ifndef ENGINES_WITCHER_MODULE_H
#define ENGINES_WITCHER_MODULE_H

#include <list>
#include <map>

#include "src/common/ustring.h"
#include "src/common/changeid.h"

#include "src/aurora/ifofile.h"

#include "src/events/types.h"

#include "src/engines/witcher/objectcontainer.h"
#include "src/engines/witcher/object.h"

namespace Engines {

class Console;

namespace Witcher {

class Area;

class Module : public Witcher::Object, public Witcher::ObjectContainer {
public:
	Module(::Engines::Console &console);
	~Module();

	// .--- Module management
	/** Is a module currently loaded and ready to run? */
	bool isLoaded() const;
	/** Is a module currently running? */
	bool isRunning() const;
	// '---

	// .--- Information about the current module
	/** Return the IFO of the currently loaded module. */
	const Aurora::IFOFile &getIFO() const;
	// '---

	// .--- Elements of the current module
	/** Return the area the PC is currently in. */
	Area *getCurrentArea();
	// '---

	// .--- Interact with the current campaign
	/** Refresh all localized strings. */
	void refreshLocalized();
	// '---

	// .--- PC management
	/** Move the player character to this area. */
	void movePC(const Common::UString &area);
	/** Move the player character to this position within the current area. */
	void movePC(float x, float y, float z);
	/** Move the player character to this position within this area. */
	void movePC(const Common::UString &area, float x, float y, float z);
	// '---

	// .--- Module main loop (called by the Campaign class)
	/** Clear the whole context. */
	void clear();

	/** Load a module. */
	void load(const Common::UString &module);
	/** Exit the currently running module. */
	void exit();

	/** Enter the loaded module, starting it. */
	void enter();
	/** Leave the running module, quitting it. */
	void leave();

	/** Add a single event for consideration into the event queue. */
	void addEvent(const Events::Event &event);
	/** Process the current event queue. */
	void processEventQueue();
	// '---

private:
	typedef std::map<Common::UString, Area *> AreaMap;

	typedef std::list<Events::Event> EventQueue;


	::Engines::Console  *_console;

	bool _hasModule; ///< Do we have a module?
	bool _running;   ///< Are we currently running a module?
	bool _exit;      ///< Should we exit the module?

	/** Resources added by the module. */
	Common::ChangeID _resModule;

	Aurora::IFOFile _ifo; ///< The module's IFO.

	AreaMap         _areas;           ///< The areas in the current module.
	Common::UString _newArea;         ///< The new area to enter.
	Area           *_currentArea;     ///< The current area.

	Common::UString _newModule; ///< The module we should change to.

	EventQueue  _eventQueue;


	// .--- Unloading
	void unload(); ///< Unload the whole shebang.

	void unloadModule();      ///< Unload the module.
	void unloadAreas();       ///< Unload the areas.
	// '---

	// .--- Loading
	void loadAreas();       ///< Load the areas.
	// '---

	void enterArea(); ///< Enter a new area.

	/** Load the actual module. */
	void loadModule(const Common::UString &module);
	/** Schedule a change to a new module. */
	void changeModule(const Common::UString &module);
	/** Actually replace the currently running module. */
	void replaceModule();

	void handleEvents();
};

} // End of namespace Witcher

} // End of namespace Engines

#endif // ENGINES_WITCHER_MODULE_H
