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
#include "illusions/bbdou/illusions_bbdou.h"
#include "illusions/duckman/illusions_duckman.h"

#include "common/config-manager.h"
#include "engines/advancedDetector.h"
#include "common/savefile.h"
#include "common/system.h"
#include "base/plugins.h"
#include "graphics/thumbnail.h"

static const PlainGameDescriptor illusionsGames[] = {
	{ "illusions", "Illusions engine game" },
	{ "bbdou", "Beavis and Butthead Do U" },
	{ "duckman", "Duckman" },
	{ 0, 0 }
};

namespace Illusions {

static const IllusionsGameDescription gameDescriptions[] = {
	{
		{
			"bbdou",
			0,
			AD_ENTRY1s("000D0001.scr", "d0c846d5dccc5607a482c7dcbdf06973", 601980),
			Common::EN_ANY,
			Common::kPlatformWindows,
			ADGF_NO_FLAGS,
			GUIO0()
		},
		kGameIdBBDOU
	},

	{
		{
			"duckman",
			0,
			AD_ENTRY1s("duckman.gam", "172c0514f3793041718159cf9cf9935f", 29560832),
			Common::EN_ANY,
			Common::kPlatformWindows,
			ADGF_NO_FLAGS,
			GUIO0()
		},
		kGameIdDuckman
	},

	{AD_TABLE_END_MARKER, 0}
};

} // End of namespace Illusions

static const char * const directoryGlobs[] = {
	"resource",
	0
};

class IllusionsMetaEngine : public AdvancedMetaEngine {
public:
	IllusionsMetaEngine() : AdvancedMetaEngine(Illusions::gameDescriptions, sizeof(Illusions::IllusionsGameDescription), illusionsGames) {
		_singleid = "illusions";
		_maxScanDepth = 2;
		_directoryGlobs = directoryGlobs;
	}

	virtual const char *getName() const {
		return "Illusions Engine";
	}

	virtual const char *getOriginalCopyright() const {
		return "(C) The Illusions Gaming Company";
	}

	virtual bool hasFeature(MetaEngineFeature f) const;
	virtual bool createInstance(OSystem *syst, Engine **engine, const ADGameDescription *desc) const;
#if 0
	virtual int getMaximumSaveSlot() const;
	virtual SaveStateList listSaves(const char *target) const;
	SaveStateDescriptor querySaveMetaInfos(const char *target, int slot) const;
	virtual void removeSaveState(const char *target, int slot) const;
#endif
};

bool IllusionsMetaEngine::hasFeature(MetaEngineFeature f) const {
	return
		false;
		/*
		(f == kSupportsListSaves) ||
		(f == kSupportsDeleteSave) ||
		(f == kSupportsLoadingDuringStartup) ||
		(f == kSavesSupportMetaInfo) ||
		(f == kSavesSupportThumbnail) ||
		(f == kSavesSupportCreationDate);
		*/
}

#if 0

void IllusionsMetaEngine::removeSaveState(const char *target, int slot) const {
	Common::String fileName = Common::String::format("%s.%03d", target, slot);
	g_system->getSavefileManager()->removeSavefile(fileName);
}

int IllusionsMetaEngine::getMaximumSaveSlot() const {
	return 999;
}

SaveStateList IllusionsMetaEngine::listSaves(const char *target) const {
	Common::SaveFileManager *saveFileMan = g_system->getSavefileManager();
	Illusions::IllusionsEngine::SaveHeader header;
	Common::String pattern = target;
	pattern += ".???";
	Common::StringArray filenames;
	filenames = saveFileMan->listSavefiles(pattern.c_str());
	Common::sort(filenames.begin(), filenames.end());	// Sort (hopefully ensuring we are sorted numerically..)
	SaveStateList saveList;
	for (Common::StringArray::const_iterator file = filenames.begin(); file != filenames.end(); ++file) {
		// Obtain the last 3 digits of the filename, since they correspond to the save slot
		int slotNum = atoi(file->c_str() + file->size() - 3);
		if (slotNum >= 0 && slotNum <= 999) {
			Common::InSaveFile *in = saveFileMan->openForLoading(file->c_str());
			if (in) {
				if (Illusions::IllusionsEngine::readSaveHeader(in, false, header) == Illusions::IllusionsEngine::kRSHENoError) {
					saveList.push_back(SaveStateDescriptor(slotNum, header.description));
				}
				delete in;
			}
		}
	}
	return saveList;
}

SaveStateDescriptor IllusionsMetaEngine::querySaveMetaInfos(const char *target, int slot) const {
	Common::String filename = Illusions::IllusionsEngine::getSavegameFilename(target, slot);
	Common::InSaveFile *in = g_system->getSavefileManager()->openForLoading(filename.c_str());
	if (in) {
		Illusions::IllusionsEngine::SaveHeader header;
		Illusions::IllusionsEngine::kReadSaveHeaderError error;
		error = Illusions::IllusionsEngine::readSaveHeader(in, true, header);
		delete in;
		if (error == Illusions::IllusionsEngine::kRSHENoError) {		
			SaveStateDescriptor desc(slot, header.description);
			// Slot 0 is used for the "Continue" save
			desc.setDeletableFlag(slot != 0);
			desc.setWriteProtectedFlag(slot == 0);
			desc.setThumbnail(header.thumbnail);
			desc.setSaveDate(header.saveDate & 0xFFFF, (header.saveDate >> 16) & 0xFF, (header.saveDate >> 24) & 0xFF);
			desc.setSaveTime((header.saveTime >> 16) & 0xFF, (header.saveTime >> 8) & 0xFF);
			desc.setPlayTime(header.playTime * 1000);
			return desc;
		}
	}
	return SaveStateDescriptor();
}

#endif

bool IllusionsMetaEngine::createInstance(OSystem *syst, Engine **engine, const ADGameDescription *desc) const {
	const Illusions::IllusionsGameDescription *gd = (const Illusions::IllusionsGameDescription *)desc;
	if (gd) {
		switch (gd->gameId) {
		case Illusions::kGameIdBBDOU:
			*engine = new Illusions::IllusionsEngine_BBDOU(syst, gd);
			break;
		case Illusions::kGameIdDuckman:
			*engine = new Illusions::IllusionsEngine_Duckman(syst, gd);
			break;
		default:
			error("Unknown game id");
			break;
		}
	}
	return desc != 0;
}

#if PLUGIN_ENABLED_DYNAMIC(ILLUSIONS)
	REGISTER_PLUGIN_DYNAMIC(ILLUSIONS, PLUGIN_TYPE_ENGINE, IllusionsMetaEngine);
#else
	REGISTER_PLUGIN_STATIC(ILLUSIONS, PLUGIN_TYPE_ENGINE, IllusionsMetaEngine);
#endif