
// Interface for Material classes to store shading parameters

#pragma once

class IMaterial
{
public:

    // @brief
    // To send material data to its shader
    virtual void SendDataToShader();

};
