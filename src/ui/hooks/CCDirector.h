#include <Geode/Geode.hpp>
#include <Geode/modify/CCDirector.hpp>

class $modify(MyCCDirector, CCDirector){


	bool replaceScene(CCScene *pScene){

		if(!GlobalVars::getSharedInstance()->isInitialLaunch){
			return CCDirector::replaceScene(pScene);
		}
		return true;
	}
};