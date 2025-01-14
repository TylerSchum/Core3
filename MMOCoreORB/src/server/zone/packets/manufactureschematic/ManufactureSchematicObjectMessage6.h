/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions.*/

#ifndef MANUFACTURESCHEMATICOBJECTMESSAGE6_H_
#define MANUFACTURESCHEMATICOBJECTMESSAGE6_H_

#include "server/zone/packets/BaseLineMessage.h"

class ManufactureSchematicObjectMessage6 : public BaseLineMessage {
public:
	ManufactureSchematicObjectMessage6(uint64 oid, uint32 schematicCRC, bool activeCraft) : BaseLineMessage(oid, 0x4D53434F, 6, 0x07) {
		insertInt(0x76); // THIS IS UNKNOWN BUT THIS NUMBER IS FOUND IN TANO6 PACKET
		insertInt(0);

		insertInt(schematicCRC);
		// Only mark the schematic active if it is currently being used in a crafting session
		insertShort((activeCraft ? 0x01 : 0x00)); // Actively using this schematic to craft

		setSize();
	}
};

#endif /*MANUFACTURESCHEMATICOBJECTMESSAGE6_H_*/
