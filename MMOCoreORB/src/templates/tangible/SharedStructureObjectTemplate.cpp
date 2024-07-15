/*
 * SharedStructureObjectTemplate.cpp
 *
 *  Created on: May 22, 2010
 *      Author: crush
 */


#include "SharedStructureObjectTemplate.h"


void SharedStructureObjectTemplate::readObject(LuaObject* templateData) {
	SharedTangibleObjectTemplate::readObject(templateData);

	lotSize = templateData->getByteField("lotSize");

	if (lotSize < 1)
		lotSize = 1;

	baseMaintenanceRate = templateData->getIntField("baseMaintenanceRate");

	basePowerRate = templateData->getIntField("basePowerRate");

	allowedZones = {"corellia", "talus", "dathomir", "endor", "lok", "naboo", "rori", "tatooine", "yavin4", "dantooine"};

	constructionMarkerTemplate = templateData->getStringField("constructionMarker");

	abilityRequired = "";

	uniqueStructure = templateData->getBooleanField("uniqueStructure");

	cityMaintenanceBase = templateData->getIntField("cityMaintenanceBase");

	cityMaintenanceRate = templateData->getIntField("cityMaintenanceRate");
}
