/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#include "illusions/illusions.h"
#include "illusions/resources/midiresource.h"

namespace Illusions {

// MidiGroupResourceLoader

void MidiGroupResourceLoader::load(Resource *resource) {
	debug("MidiGroupResourceLoader::load() Loading midi group %08X...", resource->_resId);

    // TODO
	
}

void MidiGroupResourceLoader::buildFilename(Resource *resource) {
	resource->_filename = Common::String::format("%08X.fnt", resource->_resId);
}

bool MidiGroupResourceLoader::isFlag(int flag) {
	return false;
}

} // End of namespace Illusions