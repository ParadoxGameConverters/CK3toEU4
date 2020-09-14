#include "../CK3toEU4/Source/Mappers/ProvinceMapper/ProvinceMapper.h"
#include "gtest/gtest.h"
#include <sstream>

TEST(Mappers_ProvinceMapperTests, emptyMappingsDefaultToEmpty)
{
	std::stringstream input;
	input << "0.0.0.0 = {\n";
	input << "}";

	const mappers::ProvinceMapper theMapper(input);

	ASSERT_TRUE(theMapper.getCK3ProvinceNumbers(1).empty());
}

TEST(Mappers_ProvinceMapperTests, canLookupCK3Provinces)
{
	std::stringstream input;
	input << "0.0.0.0 = {\n";
	input << "	link = { eu4 = 2 eu4 = 1 ck3 = 2 ck3 = 1 }\n";
	input << "}";

	const mappers::ProvinceMapper theMapper(input);

	ASSERT_EQ(2, theMapper.getCK3ProvinceNumbers(1).size());
	ASSERT_EQ(2, theMapper.getCK3ProvinceNumbers(1)[0]);
	ASSERT_EQ(1, theMapper.getCK3ProvinceNumbers(1)[1]);
	ASSERT_EQ(2, theMapper.getCK3ProvinceNumbers(2).size());
	ASSERT_EQ(2, theMapper.getCK3ProvinceNumbers(2)[0]);
	ASSERT_EQ(1, theMapper.getCK3ProvinceNumbers(2)[1]);
}

TEST(Mappers_ProvinceMapperTests, canLookupEU4Provinces)
{
	std::stringstream input;
	input << "0.0.0.0 = {\n";
	input << "	link = { eu4 = 2 eu4 = 1 ck3 = 2 ck3 = 1 }\n";
	input << "}";

	const mappers::ProvinceMapper theMapper(input);

	ASSERT_EQ(2, theMapper.getEU4ProvinceNumbers(1).size());
	ASSERT_EQ(2, theMapper.getEU4ProvinceNumbers(1)[0]);
	ASSERT_EQ(1, theMapper.getEU4ProvinceNumbers(1)[1]);
	ASSERT_EQ(2, theMapper.getEU4ProvinceNumbers(2).size());
	ASSERT_EQ(2, theMapper.getEU4ProvinceNumbers(2)[0]);
	ASSERT_EQ(1, theMapper.getEU4ProvinceNumbers(2)[1]);
}
