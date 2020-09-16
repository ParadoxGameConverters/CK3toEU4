#ifndef EU4_COUNTRY_H
#define EU4_COUNTRY_H

#include "../../Mappers/LocalizationMapper/LocalizationMapper.h"
#include "CountryDetails.h"
#include <memory>
#include <string>

namespace CK3
{
class Title;
}

namespace mappers
{
class GovernmentsMapper;
class ReligionMapper;
class RegionMapper;
class CultureMapper;
class ColorScraper;
class ProvinceMapper;
class RulerPersonalitiesMapper;
} // namespace mappers

namespace EU4
{
class Province;
class Country
{
  public:
	Country() = default;

	Country(std::string theTag, const std::string& filePath);
	void loadHistory(const std::string& filePath);
	void initializeFromTitle(const std::string& theTag,
		 const std::pair<std::string, std::shared_ptr<CK3::Title>>& theTitle,
		 const mappers::GovernmentsMapper& governmentsMapper,
		 const mappers::ReligionMapper& religionMapper,
		 const mappers::CultureMapper& cultureMapper,
		 const mappers::ProvinceMapper& provinceMapper,
		 const mappers::LocalizationMapper& localizationMapper,
		 const mappers::RulerPersonalitiesMapper& rulerPersonalitiesMapper,
		 date theConversionDate);
	void setSunsetCountry(bool isSunsetCountry) { details.isSunsetCountry = isSunsetCountry; }
	void clearHistoryLessons() { details.historyLessons.clear(); }

	[[nodiscard]] const auto& getTitle() const { return title; }
	[[nodiscard]] const auto& getProvinces() const { return provinces; }
	[[nodiscard]] const auto& getConversionDate() const { return conversionDate; }

	friend std::ostream& operator<<(std::ostream& output, const Country& versionParser);

  private:
	void populateHistory(const mappers::GovernmentsMapper& governmentsMapper,
		 const mappers::ReligionMapper& religionMapper,
		 const mappers::ProvinceMapper& provinceMapper,
		 const mappers::CultureMapper& cultureMapper);
	void populateCommons(const mappers::CultureMapper& cultureMapper);
	void populateMisc();
	void populateLocs(const mappers::LocalizationMapper& localizationMapper);
	void populateRulers(const mappers::ReligionMapper& religionMapper,
		 const mappers::CultureMapper& cultureMapper,
		 const mappers::RulerPersonalitiesMapper& rulerPersonalitiesMapper);

	std::string tag;
	std::string commonCountryFile;
	std::string historyCountryFile;
	date conversionDate; // for dating the monarchs in history file.
	CountryDetails details;

	std::optional<std::pair<std::string, std::shared_ptr<CK3::Title>>> title;
	std::map<std::string, mappers::LocBlock> localizations; // Beware, these are UTF8 strings. If you are altering them be sure you know what you're doing.
	std::map<int, std::shared_ptr<Province>> provinces;
};
} // namespace EU4

#endif // EU4_COUNTRY_H
