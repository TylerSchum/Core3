/*
 * WearablesDeltaVector.h
 *
 *  Created on: 09/04/2012
 *      Author: victor
 */

#ifndef WEARABLESDELTAVECTOR_H_
#define WEARABLESDELTAVECTOR_H_

#include "engine/engine.h"

#include "engine/util/json_utils.h"

#include "server/zone/objects/scene/variables/DeltaVector.h"
#include "server/zone/objects/tangible/TangibleObject.h"
#include "server/zone/objects/tangible/wearables/ArmorObject.h"
#include "templates/tangible/ArmorObjectTemplate.h"

class WearablesDeltaVector : public DeltaVector<ManagedReference<TangibleObject*> > {
private:
	// note: duplicate of CombatManager::hitlocations.
	enum HitLocation : int {
		HIT_BODY = 0,
		HIT_HEAD = 1,
		HIT_RARM = 2,
		HIT_LARM = 3,
		HIT_RLEG = 4,
		HIT_LLEG = 5,
		HIT_NUM = 6,
	};

protected:
	VectorMap<uint8, Vector<ManagedReference<ArmorObject*> > > protectionArmorMap;

public:

	WearablesDeltaVector() : DeltaVector<ManagedReference<TangibleObject*> >() {
		protectionArmorMap.setAllowOverwriteInsertPlan();

		//addSerializableVariable("protectionArmorMap", &protectionArmorMap);
	}

	bool readObjectMember(ObjectInputStream* stream, const String& name) {
		if (name == "protectionArmorMap") {
			TypeInfo<VectorMap<uint8, Vector<ManagedReference<ArmorObject*> > >>::parseFromBinaryStream(&protectionArmorMap, stream);

			return true;
		}

		return DeltaVector<ManagedReference<TangibleObject*> >::readObjectMember(stream, name);
	}

	int writeObjectMembers(ObjectOutputStream* stream) {
		static String _name = "protectionArmorMap";
		int _offset;
		uint32 _totalSize;

		_name.toBinaryStream(stream);
		_offset = stream->getOffset();
		stream->writeInt(0);
		TypeInfo<VectorMap<uint8, Vector<ManagedReference<ArmorObject*> > >>::toBinaryStream(&protectionArmorMap, stream);
		_totalSize = (uint32) (stream->getOffset() - (_offset + 4));
		stream->writeInt(_offset, _totalSize);

		return 1 + DeltaVector<ManagedReference<TangibleObject*> >::writeObjectMembers(stream);
	}

	friend void to_json(nlohmann::json& j, const WearablesDeltaVector& vec) {
		j["protectionArmorMap"] = vec.protectionArmorMap;

		const DeltaVector<ManagedReference<TangibleObject*> >& dv = vec;

		to_json(j, dv);
	}


	bool toBinaryStream(ObjectOutputStream* stream) override {
		int _currentOffset = stream->getOffset();
		stream->writeShort(0);
		int _varCount = writeObjectMembers(stream);
		stream->writeShort(_currentOffset, _varCount);

		return true;
	}

	bool parseFromBinaryStream(ObjectInputStream* stream) override {
		uint16 _varCount = stream->readShort();

		for (int i = 0; i < _varCount; ++i) {
			String _name;
			_name.parseFromBinaryStream(stream);

			uint32 _varSize = stream->readInt();

			int _currentOffset = stream->getOffset();

			if(readObjectMember(stream, _name)) {
			}

			stream->setOffset(_currentOffset + _varSize);
		}

		return true;
	}

	void insertItemToMessage(ManagedReference<TangibleObject*>* item, BaseMessage* msg) const override {
		TangibleObject* object = item->get();

		String custString;
		object->getCustomizationString(custString);

		msg->insertAscii(custString);
		msg->insertInt(object->getContainmentType()); //Equipped
		msg->insertLong(object->getObjectID()); //object id
		msg->insertInt(object->getClientObjectCRC()); //CRC of the object
	}

	bool add(const ManagedReference<TangibleObject*>& element, DeltaMessage* message = nullptr, int updates = 1) override {
		if (element->isArmorObject()) {
			ManagedReference<ArmorObject*> armor = cast<ArmorObject*>(element.get());
			uint8 hitLocations = armor->getHitLocation();

			if (hitLocations & ArmorObjectTemplate::CHEST)
				addArmor(ArmorObjectTemplate::CHEST, armor);

			if (hitLocations & ArmorObjectTemplate::ARMS)
				addArmor(ArmorObjectTemplate::ARMS, armor);

			if (hitLocations & ArmorObjectTemplate::LEGS)
				addArmor(ArmorObjectTemplate::LEGS, armor);

			if (hitLocations & ArmorObjectTemplate::HEAD)
				addArmor(ArmorObjectTemplate::HEAD, armor);
		}

		return DeltaVector<ManagedReference<TangibleObject*> >::add(element, message, updates);
	}

	ManagedReference<TangibleObject*> remove(int index, DeltaMessage* message = nullptr, int updates = 1) override {
		ManagedReference<TangibleObject*> element = get(index);

		if (element->isArmorObject()) {
			ManagedReference<ArmorObject*> armor = cast<ArmorObject*>(element.get());
			uint8 hitLocations = armor->getHitLocation();

			if (hitLocations & ArmorObjectTemplate::CHEST)
				removeArmor(ArmorObjectTemplate::CHEST, armor);

			if (hitLocations & ArmorObjectTemplate::ARMS)
				removeArmor(ArmorObjectTemplate::ARMS, armor);

			if (hitLocations & ArmorObjectTemplate::LEGS)
				removeArmor(ArmorObjectTemplate::LEGS, armor);

			if (hitLocations & ArmorObjectTemplate::HEAD)
				removeArmor(ArmorObjectTemplate::HEAD, armor);
		}

		return DeltaVector<ManagedReference<TangibleObject*> >::remove(index, message, updates);
	}

	Vector<ManagedReference<ArmorObject*> > getAvailableArmor(Vector<ManagedReference<ArmorObject*> priorityArmor, Vector<ManagedReference<ArmorObject*> fallback1Armor, Vector<ManagedReference<ArmorObject*> fallback2Armor, Vector<ManagedReference<ArmorObject*> fallback3Armor, Vector<ManagedReference<ArmorObject*> fallback4Armor) const {
		if(!priorityArmor.isEmpty())
			return priorityArmor;
		if(!fallback1Armor.isEmpty())
			return fallback1Armor;
		if(!fallback2Armor.isEmpty())
			return fallback2Armor;
		if(!fallback3Armor.isEmpty())
			return fallback3Armor;
		return fallback4Armor;
	}


	Vector<ManagedReference<ArmorObject*> > getArmorAtHitLocation(uint8 hitLocation) const {

		Vector<ManagedReference<ArmorObject*>> noLocationArmor= protectionArmorMap.get((uint8)ArmorObjectTemplate::NOLOCATION);
		Vector<ManagedReference<ArmorObject*>> chestArmor = protectionArmorMap.get((uint8)ArmorObjectTemplate::CHEST);
		Vector<ManagedReference<ArmorObject*>> armArmor = protectionArmorMap.get((uint8)ArmorObjectTemplate::ARMS);
		Vector<ManagedReference<ArmorObject*>> legArmor = protectionArmorMap.get((uint8)ArmorObjectTemplate::LEGS);
		Vector<ManagedReference<ArmorObject*>> headArmor = protectionArmorMap.get((uint8)ArmorObjectTemplate::HEAD);
		// TODO: Migrate and remove this when the object versioning and migration system is in place
		if (hl == 1) {//chest
			getAvailableArmor(chestArmor, armArmor, legArmor, headArmor, noLocationArmor);
		}
		if (hl == 2 || hl == 3) {//arms
			getAvailableArmor(armArmor, chestArmor, legArmor, headArmor, noLocationArmor);
		}
		if (hl == 4 || hl == 5) {//legs
			getAvailableArmor(legArmor, armArmor, chestArmor, headArmor, noLocationArmor);
		}
		if (hl == 6) {//head
			getAvailableArmor(headArmor, armArmor, legArmor, chestArmor, noLocationArmor);
		}
		return noLocationArmor;
	}

	void addArmor(uint8 hitLocation, ManagedReference<ArmorObject*> armor) {
		Vector<ManagedReference<ArmorObject*> > armors = protectionArmorMap.get(hitLocation);
		armors.add(armor);

		protectionArmorMap.drop(hitLocation);
		protectionArmorMap.put(hitLocation, armors);
	}

	void removeArmor(uint8 hitLocation, ManagedReference<ArmorObject*> armor) {
		Vector<ManagedReference<ArmorObject*> > armors = protectionArmorMap.get(hitLocation);
		armors.removeElement(armor);

		protectionArmorMap.drop(hitLocation);
		protectionArmorMap.put(hitLocation, armors);
	}
};


#endif /* WEARABLESDELTAVECTOR_H_ */
