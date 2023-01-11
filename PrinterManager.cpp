#include "PrinterManager.h"
#include "printQ.h"
#include "listenerThread.h"

//--------------------------------------------------------------
void PrinterManager::setup() {
	ofSetLogLevel(OF_LOG_VERBOSE);

	std::vector<std::string> loadedPrinters;
	listenerObject.x.getPrinters(loadedPrinters);

	for (auto& item : loadedPrinters){
		printers.insert(std::pair<std::string, int>(item, 4));
	}

	gui.setup(nullptr, true, ImGuiConfigFlags_ViewportsEnable);
	backgroundColor = ofColor(114, 144, 154);

	listenerObject.setup();
	listenerObject.lock();
	listenerObject.printerVar = "1"; //przekazanie do watku nazwy drukarki ktorej kolejke chcemy nasluchiwac

	for (const auto& item : loadedPrinters){
		PrinterStats TempPrinterStats;
		listenerObject.GetPrinters().insert(std::pair<std::string, PrinterStats>(item, TempPrinterStats));
	}
	listenerObject.unlock();

	if (printersSettings.loadFile("printersSettings.xml")){

		for (const auto& item : loadedPrinters){
			if (listenerObject.GetPrinters().count(item))
				listenerObject.GetPrinters()[item].maxJobs = printersSettings.getValue("settings:printers:d" + removeSpaces(item) + ":maxJobs", 4);
		}

		listenerObject.setUpdateFrequency(printersSettings.getValue("settings:updateFrequency", 10000));
		updateFrequency = listenerObject.getUpdateFrequency();
		previousUpdateFrequency = listenerObject.getUpdateFrequency();
		listenerObject.automaticJobDelete = printersSettings.getValue("settings:deleteJobsAutomatically", false);
	}


}

//--------------------------------------------------------------
void PrinterManager::update() {
	if (updateFrequency != previousUpdateFrequency){
		listenerObject.lock();
		listenerObject.setUpdateFrequency(updateFrequency);
		listenerObject.unlock();
	}

	previousUpdateFrequency = updateFrequency;

}

//--------------------------------------------------------------
void PrinterManager::draw() {
	gui.begin();
	int test = 0;

	ImGui::BeginChild("Scrolling");

	ImGui::Text("Update Frequency: ");
	ImGui::InputInt("updateFrequency", &updateFrequency);
	ImGui::Separator();

	for (auto& item : printers){
		if (ImGui::Button(item.first.c_str())){
			selectedPrinter = item.first;
			selectedPrinterMaxJobs = listenerObject.GetPrinters()[selectedPrinter].maxJobs;
			std::cout << "ABC" << std::endl;
		}
	}

	if (selectedPrinter != ""){
		ImGui::OpenPopup("ManagePrinter");
		if (ImGui::BeginPopupModal("ManagePrinter", NULL)){
			if (listenerObject.GetPrinters().count(selectedPrinter)){
				ImGui::InputInt((selectedPrinter).c_str(), &selectedPrinterMaxJobs);
			}
			else
				ImGui::Text(("Printer " + selectedPrinter + " not found").c_str());
			if (ImGui::Button("Ok")){
				if (listenerObject.GetPrinters().count(selectedPrinter))
					listenerObject.GetPrinters()[selectedPrinter].maxJobs = selectedPrinterMaxJobs;

				selectedPrinter = "";

				ImGui::CloseCurrentPopup();
			}

			if (!automaticJobDelete && listenerObject.GetPrinters()[selectedPrinter].pendingJobs >= selectedPrinterMaxJobs)
				if (ImGui::Button("Clear Printer Queue")){
					listenerObject.PrinterJobDeleter.taskDeleter(selectedPrinter);
					listenerObject.GetPrinters()[selectedPrinter].pendingJobs = 0;
				}

			ImGui::EndPopup();
		}
	}

	ImGui::Checkbox("Automatic Job Deletetion", &listenerObject.automaticJobDelete);

	if (!listenerObject.automaticJobDelete){
		for (auto const& item : listenerObject.GetPrinters()){
			if (item.second.pendingJobs >= listenerObject.GetPrinters()[item.first].maxJobs)
				ImGui::Text(("Printer " + item.first + " exceeded jobs limit. Pending jobs %i").c_str(), item.second.pendingJobs);
		}
	}

	if (ImGui::Button("Save Settings")){
		for (const auto& item : listenerObject.GetPrinters()){
			printersSettings.setValue("settings:printers:d" + removeSpaces(item.first) + ":maxJobs", item.second.maxJobs);
		}

		printersSettings.setValue("settings:deleteJobsAutomatically", listenerObject.automaticJobDelete);
		printersSettings.setValue("settings:updateFrequency", listenerObject.getUpdateFrequency());
		printersSettings.saveFile("printersSettings.xml");
	}
	ImGui::EndChild();
	gui.end();
};

std::string PrinterManager::removeSpaces(std::string givenText){
	givenText.erase(remove(givenText.begin(), givenText.end(), ' '), givenText.end());
	return givenText;
}