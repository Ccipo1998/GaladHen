
#pragma once

#include <utils/singleton.hpp>
#include <map>
#include <string>

class Model;
class Material;
class TextureImage;
class Texture;
class AssetsManagerInternal;

typedef Singleton<AssetsManagerInternal> AssetsManager;

class AssetsManagerInternal
{
    friend class Singleton<AssetsManagerInternal>;

public:
    // @brief
    // Save a model as an asset owned by the assets manager
    // @param modelToSave: the pointer to the model instance to mark as owned by assets manager
    // @param assetName: the unique name of the asset
    void SaveModelWithName(Model* modelToSave, const std::string& assetName);

    // @brief
    // Get a reference to a model by its name
    // @param assetName: the name used when saving the asset
    Model* GetModelByName(const std::string& assetName);

    // @brief
    // Save a material as an asset owned by the assets manager
    // @param materialToSave: the pointer to the material instance to mark as owned by assets manager
    // @param assetName: the unique name of the asset
    void SaveMaterialWithName(Material* materialToSave, const std::string& assetName);

    // @brief
    // Get a reference to a material by its name
    // @param assetName: the name used when saving the asset
    Material* GetMaterialByName(const std::string& assetName);

    // @brief
    // Save a texture image as an asset owned by the assets manager
    // @param texImgToSave: the pointer to the texture image instance to mark as owned by assets manager
    // @param assetName: the unique name of the asset
    void SaveTexImageWithName(TextureImage* texImgToSave, const std::string& assetName);

    // @brief
    // Get a reference to a texture image by its name
    // @param assetName: the name used when saving the asset
    TextureImage* GetTexImageByName(const std::string& assetName);

    // @brief
    // Save a texture as an asset owned by the assets manager
    // @param textureToSave: the pointer to the texture instance to mark as owned by assets manager
    // @param assetName: the unique name of the asset
    void SaveTextureWithName(Texture* textureToSave, const std::string& assetName);

    // @brief
    // Get a reference to a texture by its name
    // @param assetName: the name used when saving the asset
    Texture* GetTextureByName(const std::string& assetName);

    ~AssetsManagerInternal();

    std::map<const std::string, Model*> Models;
    std::map<const std::string, Material*> Materials;
    std::map<const std::string, TextureImage*> TextureImages;
    std::map<const std::string, Texture*> Textures;

private:

    AssetsManagerInternal();

};
