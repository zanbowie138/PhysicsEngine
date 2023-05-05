#pragma once
#include "GlobalTypes.h"
#include "../renderer/UBO.h"
#include "../renderer/Camera.h"
namespace Core {
    // Global Uniforms
    struct UniformBlock
    {
        glm::mat4 camMatrix;  // 0 // 64

        glm::vec4 camPos;	  // 64 // 16
        glm::vec4 lightPos;	  //    // 16
        glm::vec4 lightColor; // 112 // 16
    };

    class UniformBufferManager 
    {
    private:
        Camera* cam = nullptr;
        UBO UBO;
    public:
        inline void Allocate();
        inline void InitBind();
        inline void BindShader(const Shader& shader);
        inline void UpdateData();
        UniformBufferManager(){}
        inline void SetCamera(Camera* camera);
    };

    inline void UniformBufferManager::SetCamera(Camera* camera) {
        cam = camera;
    }

    inline void UniformBufferManager::Allocate()
    {
        UBO.Bind();
        UBO.AllocBuffer(112, GL_DYNAMIC_DRAW);
        UBO.Unbind();
    }

    inline void UniformBufferManager::InitBind()
    {
        UBO.Bind();
        UBO.BindUniformRange(0, 0, sizeof(glm::mat4));
        UBO.BindUniformRange(1, 64, 48);
    }

    inline void UniformBufferManager::BindShader(const Shader& shader)
    {
        UBO.Bind();

    	UBO.BindShader(shader, "Camera", 0);
        UBO.BindShader(shader, "Lighting", 1);
    }

    inline void UniformBufferManager::UpdateData()
    {
        UBO.Bind();
        UniformBlock ub = {};
        ub.camMatrix = cam->cameraMatrix;
        ub.camPos = glm::vec4(cam->position, 1.0);
        ub.lightPos = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
        ub.lightColor = glm::vec4(1.0, 1.0, 1.0, 1.0);

        char ub_char[sizeof(ub)];
        memcpy(ub_char, &ub, sizeof(ub));

        UBO.EditBuffer(ub_char, sizeof(ub));
    }
};