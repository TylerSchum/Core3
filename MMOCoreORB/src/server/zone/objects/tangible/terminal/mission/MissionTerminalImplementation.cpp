/*
 * MissionTerminalImplementation.cpp
 *
 *  Created on: 03/05/11
 *      Author: polonel
 */

#include "server/zone/objects/tangible/terminal/mission/MissionTerminal.h"
#include "server/zone/objects/creature/CreatureObject.h"
#include "server/zone/packets/object/ObjectMenuResponse.h"
#include "server/zone/objects/region/CityRegion.h"
#include "server/zone/managers/city/CityManager.h"
#include "server/zone/managers/city/CityRemoveAmenityTask.h"
#include "server/zone/objects/player/sessions/SlicingSession.h"
#include "server/zone/managers/player/PlayerManager.h"
#include "server/zone/managers/director/DirectorManager.h"
#include "server/zone/objects/player/PlayerObject.h"

void MissionTerminalImplementation::fillObjectMenuResponse(ObjectMenuResponse* menuResponse, CreatureObject* player) {
	TerminalImplementation::fillObjectMenuResponse(menuResponse, player);

	ManagedReference<CityRegion*> city = player->getCityRegion().get();

	if (city != nullptr && city->isMayor(player->getObjectID()) && getParent().get() == nullptr) {

		menuResponse->addRadialMenuItem(72, 3, "@city/city:mt_remove"); // Remove

		menuResponse->addRadialMenuItem(73, 3, "@city/city:align"); // Align
		menuResponse->addRadialMenuItemToRadialID(73, 74, 3, "@city/city:north"); // North
		menuResponse->addRadialMenuItemToRadialID(73, 75, 3, "@city/city:east"); // East
		menuResponse->addRadialMenuItemToRadialID(73, 76, 3, "@city/city:south"); // South
		menuResponse->addRadialMenuItemToRadialID(73, 77, 3, "@city/city:west"); // West
	}
	menuResponse->addRadialMenuItem(94, 3, "set mission level");
	menuResponse->addRadialMenuItemToRadialID(94, 82, 3, "reset");
	menuResponse->addRadialMenuItemToRadialID(94, 78, 3, "5");
	menuResponse->addRadialMenuItemToRadialID(94, 79, 3, "10");
	menuResponse->addRadialMenuItemToRadialID(94, 80, 3, "15");
	menuResponse->addRadialMenuItemToRadialID(94, 81, 3, "25");
	menuResponse->addRadialMenuItemToRadialID(94, 83, 3, "35");
	menuResponse->addRadialMenuItemToRadialID(94, 84, 3, "50");
	menuResponse->addRadialMenuItemToRadialID(94, 85, 3, "75");
	menuResponse->addRadialMenuItemToRadialID(94, 86, 3, "100");
	menuResponse->addRadialMenuItemToRadialID(94, 87, 3, "150");
	menuResponse->addRadialMenuItemToRadialID(94, 88, 3, "200");
	menuResponse->addRadialMenuItemToRadialID(94, 89, 3, "300");
	menuResponse->addRadialMenuItemToRadialID(94, 90, 3, "400");
	menuResponse->addRadialMenuItemToRadialID(94, 91, 3, "500");
}

int MissionTerminalImplementation::handleObjectMenuSelect(CreatureObject* player, byte selectedID) {
	ManagedReference<CityRegion*> city = player->getCityRegion().get();

	if (selectedID == 69 && player->hasSkill("combat_smuggler_slicing_01")) {
		if (isBountyTerminal())
			return 0;

		if (city != nullptr && !city->isClientRegion() && city->isBanned(player->getObjectID())) {
			player->sendSystemMessage("@city/city:banned_services"); // You are banned from using this city's services.
			return 0;
		}

		if (player->containsActiveSession(SessionFacadeType::SLICING)) {
			player->sendSystemMessage("@slicing/slicing:already_slicing");
			return 0;
		}

		if (!player->checkCooldownRecovery("slicing.terminal")) {
			StringIdChatParameter message;
			message.setStringId("@slicing/slicing:not_yet"); // You will be able to hack the network again in %DI seconds.
			message.setDI(player->getCooldownTime("slicing.terminal")->getTime() - Time().getTime());
			player->sendSystemMessage(message);
			return 0;
		}

		//Create Session
		ManagedReference<SlicingSession*> session = new SlicingSession(player);
		session->initalizeSlicingMenu(player, _this.getReferenceUnsafeStaticCast());

		return 0;

	} else if (selectedID == 72) {

		if (city != nullptr && city->isMayor(player->getObjectID())) {
			CityRemoveAmenityTask* task = new CityRemoveAmenityTask(_this.getReferenceUnsafeStaticCast(), city);
			task->execute();

			player->sendSystemMessage("@city/city:mt_removed"); // The object has been removed from the city.
		}

		return 0;

	} else if (selectedID == 74 || selectedID == 75 || selectedID == 76 || selectedID == 77) {

		CityManager* cityManager = getZoneServer()->getCityManager();
		cityManager->alignAmenity(city, player, _this.getReferenceUnsafeStaticCast(), selectedID - 74);

		return 0;
	}

	if (selectedID >= 78) {
		int selectedLevel = 0;

		if (selectedID == 78) selectedLevel = 5;
		if (selectedID == 79) selectedLevel = 10;
		if (selectedID == 80) selectedLevel = 15;
		if (selectedID == 81) selectedLevel = 25;
		if (selectedID == 82) selectedLevel = 0;
		if (selectedID == 83) selectedLevel = 35;
		if (selectedID == 84) selectedLevel = 50;
		if (selectedID == 85) selectedLevel = 75;
		if (selectedID == 86) selectedLevel = 100;
		if (selectedID == 87) selectedLevel = 150;
		if (selectedID == 88) selectedLevel = 200;
		if (selectedID == 89) selectedLevel = 300;
		if (selectedID == 90) selectedLevel = 400;
		if (selectedID == 91) selectedLevel = 500;

		ManagedReference<PlayerManager*> playerManager = server->getPlayerManager();

		ManagedReference<PlayerObject* > ghost = player->getPlayerObject();

		int currentlevel = ghost->getExperience("mission_level_choice");

		playerManager->awardExperience(player, "mission_level_choice", currentlevel * -1, false, false, false);

		playerManager->awardExperience(player, "mission_level_choice", selectedLevel, false, false, false);

		return 0;
	}

	return TangibleObjectImplementation::handleObjectMenuSelect(player, selectedID);
}

String MissionTerminalImplementation::getTerminalName() {
	String name = "@terminal_name:terminal_mission";

	if (terminalType == "artisan" || terminalType == "entertainer" || terminalType == "bounty" || terminalType == "imperial" || terminalType == "rebel" || terminalType == "scout")
		name = name + "_" + terminalType;

	return name;
}
