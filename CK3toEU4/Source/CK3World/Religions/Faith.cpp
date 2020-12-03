#include "Faith.h"
#include "Log.h"
#include "ParserHelpers.h"

CK3::Faith::Faith(std::istream& theStream, long long theID): ID(theID)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void CK3::Faith::registerKeys()
{
	registerKeyword("tag", [this](const std::string& unused, std::istream& theStream) {
		tag = commonItems::singleString(theStream).getString();
	});
	registerKeyword("doctrine", [this](const std::string& unused, std::istream& theStream) {
		doctrines.emplace_back(commonItems::singleString(theStream).getString());
	});
	registerKeyword("religion", [this](const std::string& unused, std::istream& theStream) {
		religion = std::make_pair(commonItems::singleLlong(theStream).getLlong(), nullptr);
	});
	registerKeyword("color", [this](const std::string& unused, std::istream& theStream) {
		color = laFabricaDeColor.getColor(theStream);
	});
	registerKeyword("template", [this](const std::string& unused, std::istream& theStream) {
		religionTemplate = commonItems::singleString(theStream).getString();
	});
	registerKeyword("name", [this](const std::string& unused, std::istream& theStream) {
		customName = commonItems::singleString(theStream).getString();
	});
	registerKeyword("adjective", [this](const std::string& unused, std::istream& theStream) {
		customAdjective = commonItems::singleString(theStream).getString();
	});
	registerKeyword("desc", [this](const std::string& unused, std::istream& theStream) {
		description = commonItems::singleString(theStream).getString();
	});
	registerKeyword("icon", [this](const std::string& unused, std::istream& theStream) {
		iconPath = commonItems::singleString(theStream).getString();
	});
	registerKeyword("variables", [this](const std::string& unused, std::istream& theStream) {
		for (const auto& blob: commonItems::blobList(theStream).getBlobs())
		{
			if (blob.find("has_been_reformed")) // This tells you that this UNREFORMED religion has been reformed (This is not for the reformed religion)
			{
				reformedFlag = true;
				break;
			}
		}
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}
