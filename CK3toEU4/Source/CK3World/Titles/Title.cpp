#include "Title.h"
#include "../Characters/Character.h"
#include "../Geography/CountyDetail.h"
#include "LandedTitles.h"
#include "Log.h"
#include "ParserHelpers.h"

CK3::Title::Title(std::istream& theStream, int ID): ID(ID)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void CK3::Title::registerKeys()
{
	registerKeyword("key", [this](const std::string& unused, std::istream& theStream) {
		name = commonItems::singleString(theStream).getString();
	});
	registerKeyword("name", [this](const std::string& unused, std::istream& theStream) {
		displayName = commonItems::singleString(theStream).getString();
	});
	registerKeyword("adj", [this](const std::string& unused, std::istream& theStream) {
		adjective = commonItems::singleString(theStream).getString();
	});
	registerKeyword("date", [this](const std::string& unused, std::istream& theStream) {
		creationDate = date(commonItems::singleString(theStream).getString());
	});
	registerKeyword("claim", [this](const std::string& unused, std::istream& theStream) {
		for (auto claimantID: commonItems::intList(theStream).getInts())
			claimants.insert(std::make_pair(claimantID, nullptr));
	});
	registerKeyword("history_government", [this](const std::string& unused, std::istream& theStream) {
		historyGovernment = commonItems::singleString(theStream).getString();
	});
	registerKeyword("theocratic_lease", [this](const std::string& unused, std::istream& theStream) {
		theocraticLease = commonItems::singleString(theStream).getString() == "yes";
	});
	registerKeyword("capital_barony", [this](const std::string& unused, std::istream& theStream) {
		cCapitalBarony = commonItems::singleString(theStream).getString() == "yes";
	});
	registerKeyword("duchy_capital_barony", [this](const std::string& unused, std::istream& theStream) {
		dCapitalBarony = commonItems::singleString(theStream).getString() == "yes";
	});
	registerKeyword("capital", [this](const std::string& unused, std::istream& theStream) {
		capital = std::pair(commonItems::singleInt(theStream).getInt(), nullptr);
	});
	registerKeyword("de_facto_liege", [this](const std::string& unused, std::istream& theStream) {
		dfLiege = std::pair(commonItems::singleInt(theStream).getInt(), nullptr);
	});
	registerKeyword("de_jure_liege", [this](const std::string& unused, std::istream& theStream) {
		djLiege = std::pair(commonItems::singleInt(theStream).getInt(), nullptr);
	});
	registerKeyword("de_jure_vassals", [this](const std::string& unused, std::istream& theStream) {
		for (auto vassalID: commonItems::intList(theStream).getInts())
			djVassals.insert(std::make_pair(vassalID, nullptr));
	});
	registerKeyword("heir", [this](const std::string& unused, std::istream& theStream) {
		for (auto heirID: commonItems::intList(theStream).getInts())
			heirs.emplace_back(std::pair(heirID, nullptr));
	});
	registerKeyword("laws", [this](const std::string& unused, std::istream& theStream) {
		const auto& theLaws = commonItems::stringList(theStream).getStrings();
		laws = std::set(theLaws.begin(), theLaws.end());
	});
	registerKeyword("holder", [this](const std::string& unused, std::istream& theStream) {
		holder = std::pair(commonItems::singleInt(theStream).getInt(), nullptr);
	});
	registerKeyword("coat_of_arms_id", [this](const std::string& unused, std::istream& theStream) {
		coa = std::pair(commonItems::singleInt(theStream).getInt(), nullptr);
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}

int CK3::Title::flagDeJureHREProvinces()
{
	auto counter = 0;
	// Flag our dejure counties, or our dejure vassals' counties, wherever those may be.
	// Actual flagging will happen on county level as higher titles have no dejure provinces linked to them at this stage.
	// We're leaving baronies out of this, counties are sufficient.
	for (const auto& deJureVassal: djVassals)
	{
		counter += deJureVassal.second->flagDeJureHREProvinces();
	}
	if (!clay) // We really need the county clay to confirm we're a landful proper county.
		return counter;
	if (!clay->getCounty() || name.find("c_") != 0)
		return counter;

	clay->getCounty()->second->setDeJureHRE();
	++counter;

	return counter;
}

void CK3::Title::brickTitle()
{
	grantIndependence();

	// Drop from own holder's domain.
	if (holder)
		holder->second->dropTitleFromDomain(ID);
	// Drop holder
	holder.reset();

	// release all vassals
	for (const auto& vassal: dfVassals)
		vassal.second->grantIndependence();
	dfVassals.clear(); // just in case?
}

void CK3::Title::dropTitleFromDFVassals(int titleID)
{
	const auto& dfvItr = dfVassals.find(titleID);
	if (dfvItr != dfVassals.end())
		dfVassals.erase(dfvItr);
}

void CK3::Title::grantIndependence()
{
	// Drop this title from liege holder's vassals
	if (dfLiege)
		dfLiege->second->dropTitleFromDFVassals(ID);

	// Drop liege
	dfLiege.reset();
}
