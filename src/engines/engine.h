/* eos - A reimplementation of BioWare's Aurora engine
 * Copyright (c) 2010 Sven Hesse (DrMcCoy), Matthew Hoops (clone2727)
 *
 * The Infinity, Aurora, Odyssey and Eclipse engines, Copyright (c) BioWare corp.
 * The Electron engine, Copyright (c) Obsidian Entertainment and BioWare corp.
 *
 * This file is part of eos and is distributed under the terms of
 * the GNU General Public Licence. See COPYING for more informations.
 */

/** @file engines/engine.h
 *  Generic engine interface.
 */

#ifndef ENGINES_ENGINE_H
#define ENGINES_ENGINE_H

#include "common/types.h"
#include "common/ustring.h"

#include "aurora/types.h"

namespace Common {
	class SeekableReadStream;
}

namespace Engines {

/** The base class for an engine within BioWare's Aurora family. */
class Engine {
public:
	Engine();
	virtual ~Engine();

	/** Run the game. */
	virtual void run(const Common::UString &target) = 0;

protected:
	/** Dump a resource onto disk. */
	bool dumpResource(const Common::UString &name, Aurora::FileType type, Common::UString file = "");

	/** Add an archive file to the resource manager, erroring out if the file does not exist. */
	void indexMandatoryArchive(Aurora::ArchiveType archive, const Common::UString &file, uint32 priority = 10);

	/** Add an archive file to the resource manager, if it exists. */
	bool indexOptionalArchive(Aurora::ArchiveType archive, const Common::UString &file, uint32 priority = 10);

	/** Add a directory to the resource manager, erroring out if it does not exist. */
	void indexMandatoryDirectory(const Common::UString &dir,
			const char *glob = 0, int depth = -1, uint32 priority = 10);

	/** Add a directory to the resource manager, if it exists. */
	bool indexOptionalDirectory(const Common::UString &dir,
			const char *glob = 0, int depth = -1, uint32 priority = 10);
};

} // End of namespace Engines

#endif // ENGINES_ENGINE_H
