#pragma once

#ifndef __NGALERT_HPP
#define __NGALERT_HPP

#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>

class NewgroundsAlertProtocol : public FLAlertLayerProtocol {

public:

	void FLAlert_Clicked(FLAlertLayer*, bool btn2) {
		if (btn2) {
			web::openLinkInBrowser("https://newgrounds.com/");
		}
	};

};

#endif