#include "../../CK3toEU4/Source/Mappers/CultureMapper/CultureMappingRule.h"
#include "gtest/gtest.h"
#include <sstream>

TEST(Mappers_CultureMappingTests, eu4CultureDefaultsToBlank)
{
	std::stringstream input;
	input << "= {}";

	const mappers::CultureMappingRule theMapping(input);

	ASSERT_TRUE(theMapping.getEU4Culture().empty());
}

TEST(Mappers_CultureMappingTests, eu4CultureCanBeSet)
{
	std::stringstream input;
	input << "= { eu4 = eu4Culture }";

	const mappers::CultureMappingRule theMapping(input);

	ASSERT_EQ("eu4Culture", theMapping.getEU4Culture());
}

TEST(Mappers_CultureMappingTests, ck3CulturesDefaultToEmpty)
{
	std::stringstream input;
	input << "= {}";

	const mappers::CultureMappingRule theMapping(input);

	ASSERT_TRUE(theMapping.getCK3Cultures().empty());
}

TEST(Mappers_CultureMappingTests, ck3CulturesCanBeSet)
{
	std::stringstream input;
	input << "= { ck3 = culture1 ck3 = culture2 }";

	const mappers::CultureMappingRule theMapping(input);

	ASSERT_EQ(theMapping.getCK3Cultures().size(), 2);
	ASSERT_EQ(*theMapping.getCK3Cultures().find("culture1"), "culture1");
	ASSERT_EQ(*theMapping.getCK3Cultures().find("culture2"), "culture2");
}

TEST(Mappers_CultureMappingTests, ReligionsDefaultToEmpty)
{
	std::stringstream input;
	input << "= {}";

	const mappers::CultureMappingRule theMapping(input);

	ASSERT_TRUE(theMapping.getReligions().empty());
}

TEST(Mappers_CultureMappingTests, ReligionsCanBeSet)
{
	std::stringstream input;
	input << "= { religion = religion1 religion = religion2 }";

	const mappers::CultureMappingRule theMapping(input);

	ASSERT_EQ(theMapping.getReligions().size(), 2);
	ASSERT_EQ("religion1", *theMapping.getReligions().find("religion1"));
	ASSERT_EQ("religion2", *theMapping.getReligions().find("religion2"));
}


TEST(Mappers_CultureMappingTests, RegionsDefaultToEmpty)
{
	std::stringstream input;
	input << "= {}";

	const mappers::CultureMappingRule theMapping(input);

	ASSERT_TRUE(theMapping.getReligions().empty());
}

TEST(Mappers_CultureMappingTests, RegionsCanBeSet)
{
	std::stringstream input;
	input << "= { region = region1 region = region2 }";

	const mappers::CultureMappingRule theMapping(input);

	ASSERT_EQ(theMapping.getRegions().size(), 2);
	ASSERT_EQ("region1", *theMapping.getRegions().find("region1"));
	ASSERT_EQ("region2", *theMapping.getRegions().find("region2"));
}

TEST(Mappers_CultureMappingTests, OwnersDefaultToEmpty)
{
	std::stringstream input;
	input << "= {}";

	const mappers::CultureMappingRule theMapping(input);

	ASSERT_TRUE(theMapping.getOwners().empty());
}

TEST(Mappers_CultureMappingTests, OwnersCanBeSet)
{
	std::stringstream input;
	input << "= { owner = TAG1 owner = TAG2 }";

	const mappers::CultureMappingRule theMapping(input);

	ASSERT_EQ(theMapping.getOwners().size(), 2);
	ASSERT_EQ("TAG1", *theMapping.getOwners().find("TAG1"));
	ASSERT_EQ("TAG2", *theMapping.getOwners().find("TAG2"));
}

TEST(Mappers_CultureMappingTests, ProvincesDefaultToEmpty)
{
	std::stringstream input;
	input << "= {}";

	const mappers::CultureMappingRule theMapping(input);

	ASSERT_TRUE(theMapping.getProvinces().empty());
}

TEST(Mappers_CultureMappingTests, ProvincesCanBeSet)
{
	std::stringstream input;
	input << "= { province = 3 province = 4 }";

	const mappers::CultureMappingRule theMapping(input);

	ASSERT_EQ(2, theMapping.getProvinces().size());
	ASSERT_EQ(3, *theMapping.getProvinces().find(3));
	ASSERT_EQ(4, *theMapping.getProvinces().find(4));
}

TEST(Mappers_CultureMappingTests, TechGroupDefaultToEmpty)
{
	std::stringstream input;
	input << "= {}";

	const mappers::CultureMappingRule theMapping(input);

	ASSERT_TRUE(theMapping.getTechGroupField().empty());
}

TEST(Mappers_CultureMappingTests, TechGroupCanBeSet)
{
	std::stringstream input;
	input << "= { tech = high_american }";

	const mappers::CultureMappingRule theMapping(input);

	ASSERT_EQ("high_american", theMapping.getTechGroupField());
}

TEST(Mappers_CultureMappingTests, TechGroupCanBeMatched)
{
	std::stringstream input;
	input << "= { eu4 = aztec tech = high_american }";

	const mappers::CultureMappingRule theMapping(input);

	ASSERT_EQ("high_american", *theMapping.getTechGroup("aztec"));
}


TEST(Mappers_CultureMappingTests, TechGroupFailsForWrongMatch)
{
	std::stringstream input;
	input << "= { eu4 = aztec tech = high_american }";

	const mappers::CultureMappingRule theMapping(input);

	ASSERT_FALSE(theMapping.getTechGroup("incan"));
}


TEST(Mappers_CultureMappingTests, GFXDefaultToEmpty)
{
	std::stringstream input;
	input << "= {}";

	const mappers::CultureMappingRule theMapping(input);

	ASSERT_TRUE(theMapping.getGFXField().empty());
}

TEST(Mappers_CultureMappingTests, GFXCanBeSet)
{
	std::stringstream input;
	input << "= { gfx = test }";

	const mappers::CultureMappingRule theMapping(input);

	ASSERT_EQ("test", theMapping.getGFXField());
}

TEST(Mappers_CultureMappingTests, GFXCanBeMatched)
{
	std::stringstream input;
	input << "= { eu4 = aztec gfx = test }";

	const mappers::CultureMappingRule theMapping(input);

	ASSERT_EQ("test", *theMapping.getGFX("aztec"));
}


TEST(Mappers_CultureMappingTests, GFXFailsForWrongMatch)
{
	std::stringstream input;
	input << "= { eu4 = aztec gfx = test }";

	const mappers::CultureMappingRule theMapping(input);

	ASSERT_FALSE(theMapping.getGFX("incan"));
}
