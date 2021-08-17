#pragma once

#include "NppDarkMode.h"
#include "tinyxml2.h"
#include "../Utils.h"

typedef tinyxml2::XMLDocument TXMLDocument;
typedef tinyxml2::XMLNode TXMLNode;
typedef tinyxml2::XMLElement TXMLElement;
typedef tinyxml2::XMLError TXMLError;

using NppDarkMode::DarkModeConf;

class NppParameters final
{
public:
	enum toolBarStatusType { TB_SMALL, TB_LARGE, TB_SMALL2, TB_LARGE2, TB_STANDARD };

	static NppParameters& getInstance() {
		return *getInstancePointer();
	};

	void init();
	DarkModeConf getDarkModeConf();

	static COLORREF nppBackgroundRGB();

private:
	bool _isLocal{};
	wstring _nppPath{};

	DarkModeConf _darkmode;
	toolBarStatusType _toolBarStatus = TB_STANDARD;

	static NppParameters* getInstancePointer() {
		static NppParameters* instance = new NppParameters;
		return instance;
	};

	bool isLocal();
	wstring getSettingsFolder();
	bool loadConfigSettings();
};

