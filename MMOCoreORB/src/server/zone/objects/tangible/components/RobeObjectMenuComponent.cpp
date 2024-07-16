/*
 * RobeObjectMenuComponent.cpp
 *
 *  Created on: Apr 5, 2012
 *      Author: katherine
 */

#include "RobeObjectMenuComponent.h"

#include "server/zone/objects/creature/CreatureObject.h"
#include "server/zone/objects/player/PlayerObject.h"
#include "server/zone/objects/tangible/weapon/WeaponObject.h"
#include "LightsaberObjectMenuComponent.h"
#include "server/zone/packets/object/ObjectMenuResponse.h"
#include "server/zone/objects/player/sessions/SlicingSession.h"

void RobeObjectMenuComponent::fillObjectMenuResponse(SceneObject* sceneObject, ObjectMenuResponse* menuResponse, CreatureObject* player) const {
	TangibleObjectMenuComponent::fillObjectMenuResponse(sceneObject, menuResponse, player);


//	String text = "open robe storage";
//	menuResponse->addRadialMenuItem(89, 3, text);

}


int RobeObjectMenuComponent::handleObjectMenuSelect(SceneObject* sceneObject, CreatureObject* player, byte selectedID) const {

//	ManagedReference<WeaponObject*> weapon = cast<WeaponObject*>(sceneObject);
//
//	if (weapon == nullptr)
//		return 1;
//
//	if (selectedID == 89) {
//
//		ManagedReference<SceneObject*> parent = weapon->getParent().get();
////		if (parent != nullptr && parent->isPlayerCreature()){
////			player->sendSystemMessage("@jedi_spam:saber_not_while_equpped");
////			return 0;
////		}
//
//		weapon->sendContainerTo(player);
//	}

	return TangibleObjectMenuComponent::handleObjectMenuSelect(sceneObject, player, selectedID);
}
