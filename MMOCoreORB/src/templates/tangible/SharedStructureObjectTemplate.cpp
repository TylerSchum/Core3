/*
 * SharedStructureObjectTemplate.cpp
 *
 *  Created on: May 22, 2010
 *      Author: crush
 */


#include "SharedStructureObjectTemplate.h"


void SharedStructureObjectTemplate::readObject(LuaObject* templateData) {
	SharedTangibleObjectTemplate::readObject(templateData);

	int newlotsize = templateData->getByteField("lotSize");

	if (newlotsize >= 1) newlotsize = 1;

	lotSize = newlotsize;

	if (lotSize < 1)
		lotSize = 1;

	baseMaintenanceRate = templateData->getIntField("baseMaintenanceRate");

	basePowerRate = templateData->getIntField("basePowerRate");

	allowedZones = {"corellia", "talus", "dathomir", "endor", "lok", "naboo", "rori", "tatooine", "yavin4", "dantooine"};
	
	int newcityrankreq = templateData->getByteField("cityRankRequired");

	if (newcityrankreq >= 1) newcityrankreq = 1;

	cityRankRequired = newcityrankreq;

	constructionMarkerTemplate = templateData->getStringField("constructionMarker");

	abilityRequired = "";

	uniqueStructure = templateData->getBooleanField("uniqueStructure");

	cityMaintenanceBase = templateData->getIntField("cityMaintenanceBase");

	cityMaintenanceRate = templateData->getIntField("cityMaintenanceRate");
}
