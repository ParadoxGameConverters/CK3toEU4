#include "CoatCrafter.h"
#include "../../CK3World/CoatsOfArms/CoatOfArms.h"
#include "../../CK3World/CoatsOfArms/Emblem.h"
#include "../../CK3World/CoatsOfArms/EmblemInstance.h"
#include "../Country/Country.h"
#include "Magick++.h"
#include "OSCompatibilityLayer.h"

Magick::Image EU4::CoatCrafter::craftFlagFromCoA(const CK3::CoatOfArms& coa) const
{
	// Get a background image

	Magick::Image image;
	
	if (coa.getPattern())
	{
		Log(LogLevel::Debug) << "sending to craft pattern";
		image = craftPatternImage(*coa);
		Log(LogLevel::Debug) << "pattern crafted";		
		image.write("flags.tmp/rawpattern.dds");
		Log(LogLevel::Debug) << "ditched pattern";
	}
	Log(LogLevel::Debug) << "sending c emblems";
	const auto coloredEmblems = craftEmblemImages(coa.getColoredEmblems());
	Log(LogLevel::Debug) << "sending t emblems";
	const auto texturedEmblems = craftEmblemImages(coa.getTexturedEmblems());

	Log(LogLevel::Debug) << "processing cemblems";
	image = processEmblemsOnImage(image, coloredEmblems);
	Log(LogLevel::Debug) << "processing temblems";
	image = processEmblemsOnImage(image, texturedEmblems);

	Log(LogLevel::Debug) << "coa done";
	return image;
}

Magick::Image EU4::FlagCrafter::craftPatternImage(const CK3::CoatOfArms& coa) const
{
	Magick::Image pattern; // Pattern is the base of CoAs.
	Log(LogLevel::Debug) << "Using pattern: " << *coa.getPattern();
	std::vector<std::pair<COLOR, commonItems::Color>> replacementMatrix;
	pattern.read(ck3Source + "patterns/" + *coa.getPattern());
	if (coa.getColor1())
	{
		replacementMatrix.emplace_back(std::pair(COLOR::COLOR1, *coa.getColor1()));
		Log(LogLevel::Debug) << "using color1: " << coa.getColor1()->outputRgb();
		
	}
	if (coa.getColor2())
	{
		replacementMatrix.emplace_back(std::pair(COLOR::COLOR2, *coa.getColor2()));		
		Log(LogLevel::Debug) << "using color2: " << coa.getColor2()->outputRgb();
	}
	if (coa.getColor3())
	{
		replacementMatrix.emplace_back(std::pair(COLOR::COLOR3, *coa.getColor3()));		
		Log(LogLevel::Debug) << "using color3: " << coa.getColor3()->outputRgb();
	}
	pattern = recolorPattern(pattern, replacementMatrix);
	Log(LogLevel::Debug) << "returning pattern";
	return pattern;
}

std::vector<std::pair<CK3::Emblem, Magick::Image>> EU4::FlagCrafter::craftEmblemImages(const std::vector<CK3::Emblem>& emblems) const
{
	std::vector<std::pair<CK3::Emblem, Magick::Image>> toReturn;
	for (const auto& emblem: emblems)
	{
		Log(LogLevel::Debug) << "in emblem " << *emblem.getTexture();
		if (!emblem.getTexture())
		{
			Log(LogLevel::Warning) << "Emblem has no texture. Not crafting.";
			toReturn.emplace_back(std::pair(emblem, Magick::Image(Magick::Geometry(128, 128), Magick::ColorRGB(0, 0, 0))));
			continue;
		}
		Magick::Image emblemImage;
		Log(LogLevel::Debug) << "Using emblem: " << *emblem.getTexture();
		std::vector<std::pair<COLOR, commonItems::Color>> replacementMatrix;
		if (commonItems::DoesFileExist(ck3Source + "colored_emblems/" + *emblem.getTexture()))
			emblemImage.read(ck3Source + "colored_emblems/" + *emblem.getTexture());
		else if (commonItems::DoesFileExist(ck3Source + "textured_emblems/" + *emblem.getTexture()))
			emblemImage.read(ck3Source + "textured_emblems/" + *emblem.getTexture());
		else
		{
			Log(LogLevel::Warning) << "Emblem load fail, could not find: " << ck3Source << "textured_emblems/" << *emblem.getTexture();
			toReturn.emplace_back(std::pair(emblem, Magick::Image(Magick::Geometry(128, 128), Magick::ColorRGB(0, 0, 0))));
			continue;
		}
		if (emblem.getColor1())
		{
			replacementMatrix.emplace_back(std::pair(COLOR::COLOR1, *emblem.getColor1()));
			Log(LogLevel::Debug) << "using color1: " << emblem.getColor1()->outputRgb();
		}
		if (emblem.getColor2())
		{
			replacementMatrix.emplace_back(std::pair(COLOR::COLOR2, *emblem.getColor2()));
			Log(LogLevel::Debug) << "using color2: " << emblem.getColor2()->outputRgb();
		}
		if (emblem.getColor3())
		{
			replacementMatrix.emplace_back(std::pair(COLOR::COLOR3, *emblem.getColor3()));
			Log(LogLevel::Debug) << "using color3: " << emblem.getColor3()->outputRgb();
		}
		emblemImage = recolorEmblem(emblemImage, replacementMatrix);
		emblemImage.write("flags.tmp/recoloredemblem.dds");
		Log(LogLevel::Debug) << "ditched emblem";
		toReturn.emplace_back(std::pair(emblem, emblemImage));
	}
	Log(LogLevel::Debug) << "returning emblem images";
	return toReturn;
}


Magick::Image EU4::FlagCrafter::recolorPattern(const Magick::Image& pattern, const std::vector<std::pair<COLOR, commonItems::Color>>& replacementMatrix) const
{
	std::vector<std::pair<commonItems::Color, commonItems::Color>> replacementColors;
	for (const auto& entry: replacementMatrix)
		switch (entry.first)
		{
			case COLOR::COLOR1:
				replacementColors.emplace_back(std::pair(patternColorMasks.color1, entry.second));
				break;
			case COLOR::COLOR2:
				replacementColors.emplace_back(std::pair(patternColorMasks.color2, entry.second));
				break;
			case COLOR::COLOR3:
				replacementColors.emplace_back(std::pair(patternColorMasks.color3, entry.second));
				break;
			case COLOR::COLOR4: // not sure what to do here yet.
			case COLOR::COLOR5: // not sure what to do here yet.
				break;
		}
	Log(LogLevel::Debug) << "Recolored pattern";
	return recolorImage(pattern, replacementColors);
}
Magick::Image EU4::FlagCrafter::recolorEmblem(const Magick::Image& emblem, const std::vector<std::pair<COLOR, commonItems::Color>>& replacementMatrix) const
{
	std::vector<std::pair<commonItems::Color, commonItems::Color>> replacementColors;
	for (const auto& entry: replacementMatrix)
		switch (entry.first)
		{
			case COLOR::COLOR1:
				replacementColors.emplace_back(std::pair(emblemColorMasks.color1, entry.second));
				break;
			case COLOR::COLOR2:
				replacementColors.emplace_back(std::pair(emblemColorMasks.color2, entry.second));
				break;
			case COLOR::COLOR3:
				replacementColors.emplace_back(
					 std::pair(emblemColorMasks.color3, commonItems::Color(std::array<int, 3>{255, 255, 255}))); // rendering to white per PDX spec.
				break;
			case COLOR::COLOR4: // not sure what to do here yet.
			case COLOR::COLOR5: // not sure what to do here yet.
				break;
		}
	Log(LogLevel::Debug) << "Recolored emblem";
	return recolorImage(emblem, replacementColors);
}

Magick::Image EU4::FlagCrafter::recolorImage(const Magick::Image& image,
	 const std::vector<std::pair<commonItems::Color, commonItems::Color>>& replacementColors) const
{
	auto workingImage = image;
	workingImage.write("flags.tmp/rawpattern-orig.dds");
	Log(LogLevel::Debug) << "ditched pattern - orig";	
	auto counter = 0;
	workingImage.modifyImage(); // Lock image from outside influences.
	// I *think* this means colors distanced by "25" (in rgb ints maybe) are considered equal for purposes of replacement.
	workingImage.colorFuzz(25.0 * 65535 / 100.0);

	for (const auto& color: replacementColors)
	{
		// color to be replaced
		const Magick::ColorRGB swapMask(color.first.r() / 255.0, color.first.g() / 255.0, color.first.b() / 255.0);

		// color that replaces it
		const Magick::ColorRGB fillColor(color.second.r() / 255.0, color.second.g() / 255.0, color.second.b() / 255.0);

		// and.... Magick++
		workingImage.opaque(swapMask, fillColor);
		workingImage.write("flags.tmp/rawpattern-c" + std::to_string(counter) + ".dds");
		Log(LogLevel::Debug) << "ditched pattern " << counter << " post color " << color.second.outputRgb();
		counter++;
	}

	workingImage.flip();
	Log(LogLevel::Debug) << "returning recolored image";
	return workingImage;
}

Magick::Image EU4::FlagCrafter::processEmblemsOnImage(const Magick::Image& image, const std::vector<std::pair<CK3::Emblem, Magick::Image>>& emblems) const
{
	if (emblems.empty())
		return image;

	auto workingImage = image;

	for (const auto& emblemPair: emblems)
	{
		Log(LogLevel::Debug) << "in emblem " << *emblemPair.first.getTexture();
		if (emblemPair.first.getInstances().empty())
		{
			Log(LogLevel::Debug) << "generating single instance";
			// We need at least a nominal instance.
			CK3::EmblemInstance emblemInstance;
			Log(LogLevel::Debug) << "into vector";
			std::vector<CK3::EmblemInstance> emblemVector = {emblemInstance};
			Log(LogLevel::Debug) << "generating single instance";
			workingImage = imposeEmblemInstancesOnImage(workingImage, emblemVector, emblemPair.second);
		}
		else
		{
			// Run them all.
			Log(LogLevel::Debug) << "pushing instances";
			workingImage = imposeEmblemInstancesOnImage(workingImage, emblemPair.first.getInstances(), emblemPair.second);
		}
	}
	
	return workingImage;
}

Magick::Image EU4::FlagCrafter::imposeEmblemInstancesOnImage(const Magick::Image& image,
	 const std::vector<CK3::EmblemInstance>& instances,
	 const Magick::Image& emblem) const
{
	auto workingImage = image;
	workingImage.modifyImage();

	const auto height = workingImage.baseRows();
	const auto width = workingImage.baseColumns();
	
	for (const auto& instance: instances)
	{
		auto workingEmblem = image;
		workingEmblem.modifyImage();
		
		// Rescale emblem
		if (!instance.getScale().empty())
		{
			if (instance.getScale().size() != 2)
			{
				Log(LogLevel::Warning) << "Invalid emblem instance scale command, array size: " << instance.getScale().size();
				continue;
			}
			const auto targetWidth = instance.getScale()[0] * static_cast<double>(width);
			const auto targetHeight = instance.getScale()[1] * static_cast<double>(height);
			Log(LogLevel::Debug) << "scaling insance";
			workingEmblem.scale(Magick::Geometry(static_cast<size_t>(targetWidth), static_cast<size_t>(targetHeight)));
			Log(LogLevel::Debug) << "scaled";
			workingEmblem.write("flags.tmp/scaledemblem.dds");
		}
		
		// Rotate emblem
		if (instance.getRotation() == 0.0)
		{
			Log(LogLevel::Debug) << "rotating insance";
			workingEmblem.rotate(0 - instance.getRotation()); // I *think* PDX stores rotation clockwise.
			Log(LogLevel::Debug) << "rotated";
			workingEmblem.write("flags.tmp/rotatedemblem.dds");
		}
		
		// Position emblem
		if (!instance.getPosition().empty()) // We won't paste anything without a position - which should only happen on user error.
		{
			if (instance.getPosition().size() != 2)
			{
				Log(LogLevel::Warning) << "Invalid emblem instance position command, array size: " << instance.getPosition().size();
				continue;
			}
			const auto targetX = static_cast<size_t>(instance.getPosition()[0] * static_cast<double>(width));
			const auto targetY = static_cast<size_t>(instance.getPosition()[1] * static_cast<double>(height));
			Log(LogLevel::Debug) << "compositing instance";
			workingImage.composite(workingEmblem, Magick::Geometry(targetX, targetY), MagickCore::BlendCompositeOp);
			Log(LogLevel::Debug) << "composited";
		}		
		
	}
	Log(LogLevel::Debug) << "returning pattern";
	return workingImage;
}