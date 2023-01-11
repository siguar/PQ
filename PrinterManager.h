#pragma once

#include "ofMain.h"
#include "listenerThread.h"
#include "deleterThread.h"
#include "ofxXmlSettings.h"
#include "ofxImGui.h"

class PrinterManager {
public:
	void setup();
	void update();
	void draw();

	std::string removeSpaces(std::string givenText);

	inline ListenerThreadObject& getListener() {
		return listenerObject;
	}

	inline ofxImGui::Gui& GetGui() { //zwraca gui
		return gui;
	}

protected:
	ListenerThreadObject listenerObject;
	DeleterThreadObject deleteTaskFunction;
	ofxImGui::Gui gui;
	ImVec4 backgroundColor;

	int updateFrequency = 1000;
	int previousUpdateFrequency = 1000;

	std::map<std::string, int> printers;

	bool automaticJobDelete = false;

	ofxXmlSettings printersSettings;

	std::string selectedPrinter = "";
	int selectedPrinterMaxJobs = 4;
	
};

