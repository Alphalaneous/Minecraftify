#pragma once

#include <Geode/Geode.hpp>
#include <Geode/modify/CCDirector.hpp>

class $modify(MyCCDirector, CCDirector){

	bool replaceScene(CCScene *pScene){
		return CCDirector::replaceScene(pScene);
	}
};