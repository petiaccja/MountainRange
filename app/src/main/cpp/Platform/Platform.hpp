#pragma once

#include <string>
#include <string_view>

#include <android/asset_manager.h>


namespace mrange {


class PlatformHelper {
public:
	PlatformHelper(AAssetManager* assetManager);

	std::string LoadShaderFile(std::string_view fileName) const;

private:
	AAssetManager *m_assetManager;
};


}