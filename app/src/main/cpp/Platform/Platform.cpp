#include "Platform.hpp"

#include <array>






namespace mrange {


PlatformHelper::PlatformHelper(AAssetManager* assetManager) : m_assetManager(assetManager) {
}


std::string PlatformHelper::LoadShaderFile(std::string_view fileName) const {
	AAsset* asset = AAssetManager_open(m_assetManager, fileName.data(), AASSET_MODE_STREAMING);

	std::array<char, 64> buffer;
	std::string content;

	int bytesRead = 0;
	while ((bytesRead = AAsset_read(asset, buffer.data(), buffer.size())) > 0) {
		content.insert(content.end(), buffer.begin(), buffer.begin() + bytesRead);
	}

	AAsset_close(asset);

	return content;
}


}