#include "AssetManager.h"
#include <string>
#include <iostream>

#include "Scene.h"
#include "Entity.h"
#include "Components\MeshComponent.h"
#include "Components\CameraComponent.h"
#include "Components\LightComponent.h"

namespace Sleepy
{

    bool AssetManager::SaveScene(const Scene& scene)
    {

        std::string sceneName = scene.m_SceneEntity->m_Name;
        std::ofstream sceneFile(_SOLUTIONDIR"Sandbox/Assets/" + sceneName + ".json");
        sceneFile << "{ \n \"Entities\" : \n [\n";
        SaveEntity(*scene.m_SceneEntity,sceneFile);

        sceneFile << " ]\n}";
        sceneFile.close();
        return true;
    }

    Scene* AssetManager::LoadScene(std::string file)
    {
        std::fstream sceneFile(file);
        Scene* scene = new Scene();
        std::string text;
        std::string line;
        int count;
        while (std::getline(sceneFile, line))
        {
            text.append(line);
        }

        std::string::iterator it = text.begin();
        LoadEntity(it, text, *scene->m_SceneEntity);
        return scene;
    }

    bool AssetManager::LoadEntity(std::string::iterator& it, std::string& text, Entity& entity)
    {
        std::string::iterator curr_it = text.begin();
        int openBrackets = 0;
        while (it != text.end())
        {

            if (it == text.end())
                return true;
            if (*it == '{')
            {
                openBrackets++;
                it++;
            }
            else if (*it == '}')
            {
                if (--openBrackets <= 0)
                {
                    return true;
                }
            }
            else if (*it == '\"')
            {
                curr_it = ++it;
                while (*it != '\"')
                    it++;

                std::string key = text.substr(curr_it - text.begin(), it - curr_it);
                if (key == "Name")
                {
                    do { it++; } while (*it != '\"');
                    curr_it = ++it;
                    while (*it != '\"')
                        it++;
                    std::string value = text.substr(curr_it - text.begin(), it - curr_it);
                    entity.m_Name = value;
                }
                else if (key == "Transform")
                {
                    while (*it != '}' && *++it != '}')
                    {
                        if (*it == '\"')
                        {
                            curr_it = ++it;

                            while (*it != '\"')
                            {
                                it++;
                            }
                            key = text.substr(curr_it - text.begin(), it - curr_it);
                            if (key == "Position")
                            {
                                glm::vec3 pos = glm::vec3(0.0f);
                                do { it++; } while (*it != '\"');
                                curr_it = ++it;
                                while (*it != ',')
                                    it++;
                                pos.x = std::stof(text.substr(curr_it - text.begin(), it - curr_it));
                                curr_it = ++it;
                                while (*it != ',')
                                    it++;
                                pos.y = std::stof(text.substr(curr_it - text.begin(), it - curr_it));
                                curr_it = ++it;
                                while (*it != ',')
                                    it++;
                                pos.z = std::stof(text.substr(curr_it - text.begin(), it - curr_it));
                                entity.SetPosition(pos);
                            }
                            if (key == "Rotation")
                            {
                                glm::vec3 rot = glm::vec3(0.0f);
                                do { it++; } while (*it != '\"');
                                curr_it = ++it;
                                while (*it != ',')
                                    it++;
                                rot.x = std::stof(text.substr(curr_it - text.begin(), it - curr_it));
                                curr_it = ++it;
                                while (*it != ',')
                                    it++;
                                rot.y = std::stof(text.substr(curr_it - text.begin(), it - curr_it));
                                curr_it = ++it;
                                while (*it != ',')
                                    it++;
                                rot.z = std::stof(text.substr(curr_it - text.begin(), it - curr_it));
                                entity.SetRotation(rot);
                            }
                            if (key == "Scale")
                            {
                                glm::vec3 scale = glm::vec3(0.0f);
                                do { it++; } while (*it != '\"');
                                curr_it = ++it;
                                while (*it != ',')
                                    it++;
                                scale.x = std::stof(text.substr(curr_it - text.begin(), it - curr_it));
                                curr_it = ++it;
                                while (*it != ',')
                                    it++;
                                scale.y = std::stof(text.substr(curr_it - text.begin(), it - curr_it));
                                curr_it = ++it;
                                while (*it != '\"')
                                    it++;
                                scale.z = std::stof(text.substr(curr_it - text.begin(), it - curr_it));
                                entity.SetScale(scale);
                            }
                        }
                    }
                }
                else if (key == "DirLightComponent")
                {
                    DirLightComponent& dirLight = entity.AddComponent<DirLightComponent>();
                    while (*it != '}' && *++it != '}')
                    {
                        if (*it == '\"')
                        {
                            curr_it = ++it;

                            while (*it != '\"')
                            {
                                it++;
                            }
                            key = text.substr(curr_it - text.begin(), it - curr_it);
                            if (key == "Ambient")
                            {
                                glm::vec3 ambient = glm::vec3(0.0f);
                                do { it++; } while (*it != '\"');
                                curr_it = ++it;
                                while (*it != ',')
                                    it++;
                                ambient.x = std::stof(text.substr(curr_it - text.begin(), it - curr_it));
                                curr_it = ++it;
                                while (*it != ',')
                                    it++;
                                ambient.y = std::stof(text.substr(curr_it - text.begin(), it - curr_it));
                                curr_it = ++it;
                                while (*it != '\"')
                                    it++;
                                ambient.z = std::stof(text.substr(curr_it - text.begin(), it - curr_it));
                                dirLight.m_ambient = ambient;
                            }
                            if (key == "Diffuse")
                            {
                                glm::vec3 diffuse = glm::vec3(0.0f);
                                do { it++; } while (*it != '\"');
                                curr_it = ++it;
                                while (*it != ',')
                                    it++;
                                diffuse.x = std::stof(text.substr(curr_it - text.begin(), it - curr_it));
                                curr_it = ++it;
                                while (*it != ',')
                                    it++;
                                diffuse.y = std::stof(text.substr(curr_it - text.begin(), it - curr_it));
                                curr_it = ++it;
                                while (*it != '\"')
                                    it++;
                                diffuse.z = std::stof(text.substr(curr_it - text.begin(), it - curr_it));
                                dirLight.m_diffuse = diffuse;
                            }
                            if (key == "Specular")
                            {
                                glm::vec3 specular = glm::vec3(0.0f);
                                do { it++; } while (*it != '\"');
                                curr_it = ++it;
                                while (*it != ',')
                                    it++;
                                specular.x = std::stof(text.substr(curr_it - text.begin(), it - curr_it));
                                curr_it = ++it;
                                while (*it != ',')
                                    it++;
                                specular.y = std::stof(text.substr(curr_it - text.begin(), it - curr_it));
                                curr_it = ++it;
                                while (*it != '\"')
                                    it++;
                                specular.z = std::stof(text.substr(curr_it - text.begin(), it - curr_it));
                                dirLight.m_diffuse = specular;
                            }
                            if (key == "Direction")
                            {
                                glm::vec3 dir = glm::vec3(0.0f);
                                do { it++; } while (*it != '\"');
                                curr_it = ++it;
                                while (*it != ',')
                                    it++;
                                dir.x = std::stof(text.substr(curr_it - text.begin(), it - curr_it));
                                curr_it = ++it;
                                while (*it != ',')
                                    it++;
                                dir.y = std::stof(text.substr(curr_it - text.begin(), it - curr_it));
                                curr_it = ++it;
                                while (*it != '\"')
                                    it++;
                                dir.z = std::stof(text.substr(curr_it - text.begin(), it - curr_it));
                                dirLight.m_diffuse = dir;
                            }
                        }
                    }

                }
                else if (key == "PointLightComponent")
                {
                    PointLightComponent& pointLight = entity.AddComponent<PointLightComponent>();
                    while (*it != '}' && *++it != '}')
                    {
                        if (*it == '\"')
                        {
                            curr_it = ++it;

                            while (*it != '\"')
                            {
                                it++;
                            }
                            key = text.substr(curr_it - text.begin(), it - curr_it);

                            if (key == "Ambient")
                            {
                                glm::vec3 ambient = glm::vec3(0.0f);
                                do { it++; } while (*it != '\"');
                                curr_it = ++it;
                                while (*it != ',')
                                    it++;
                                ambient.x = std::stof(text.substr(curr_it - text.begin(), it - curr_it));
                                curr_it = ++it;
                                while (*it != ',')
                                    it++;
                                ambient.y = std::stof(text.substr(curr_it - text.begin(), it - curr_it));
                                curr_it = ++it;
                                while (*it != '\"')
                                    it++;
                                ambient.z = std::stof(text.substr(curr_it - text.begin(), it - curr_it));
                                pointLight.m_ambient = ambient;
                            }
                            if (key == "Diffuse")
                            {
                                glm::vec3 diffuse = glm::vec3(0.0f);
                                do { it++; } while (*it != '\"');
                                curr_it = ++it;
                                while (*it != ',')
                                    it++;
                                diffuse.x = std::stof(text.substr(curr_it - text.begin(), it - curr_it));
                                curr_it = ++it;
                                while (*it != ',')
                                    it++;
                                diffuse.y = std::stof(text.substr(curr_it - text.begin(), it - curr_it));
                                curr_it = ++it;
                                while (*it != '\"')
                                    it++;
                                diffuse.z = std::stof(text.substr(curr_it - text.begin(), it - curr_it));
                                pointLight.m_diffuse = diffuse;
                            }
                            if (key == "Specular")
                            {
                                glm::vec3 specular = glm::vec3(0.0f);
                                do { it++; } while (*it != '\"');
                                curr_it = ++it;
                                while (*it != ',')
                                    it++;
                                specular.x = std::stof(text.substr(curr_it - text.begin(), it - curr_it));
                                curr_it = ++it;
                                while (*it != ',')
                                    it++;
                                specular.y = std::stof(text.substr(curr_it - text.begin(), it - curr_it));
                                curr_it = ++it;
                                while (*it != '\"')
                                    it++;
                                specular.z = std::stof(text.substr(curr_it - text.begin(), it - curr_it));
                                pointLight.m_diffuse = specular;
                            }
                            if (key == "Constant")
                            {
                                do { it++; } while (*it != '\"');
                                curr_it = ++it;
                                while (*it != '\"')
                                    it++;
                                pointLight.m_constant = std::stof(text.substr(curr_it - text.begin(), it - curr_it));
                            }
                            if (key == "Linear")
                            {
                                do { it++; } while (*it != '\"');
                                curr_it = ++it;
                                while (*it != '\"')
                                    it++;
                                pointLight.m_linear = std::stof(text.substr(curr_it - text.begin(), it - curr_it));
                            }
                            if (key == "Quadratic")
                            {
                                do { it++; } while (*it != '\"');
                                curr_it = ++it;
                                while (*it != '\"')
                                    it++;
                                pointLight.m_quadratic = std::stof(text.substr(curr_it - text.begin(), it - curr_it));
                            }
                        }
                    }

                    }
                else if (key == "CameraComponent")
                {
                    CameraComponent& camComp = entity.AddComponent<CameraComponent>();
                    while (*it != '}' && *++it != '}')
                    {
                        if (*it == '\"')
                        {
                            curr_it = ++it;

                            while (*it != '\"')
                            {
                                it++;
                            }
                            key = text.substr(curr_it - text.begin(), it - curr_it);

                            if (key == "FOV")
                            {
                                do { it++; } while (*it != '\"');
                                curr_it = ++it;
                                while (*it != '\"')
                                    it++;
                                camComp.fov = std::stof(text.substr(curr_it - text.begin(), it - curr_it));
                            }
                            if (key == "Diffuse")
                            {
                                do { it++; } while (*it != '\"');
                                curr_it = ++it;
                                while (*it != '\"')
                                    it++;
                                camComp.renderDistance = std::stof(text.substr(curr_it - text.begin(), it - curr_it));
                            }
                        }
                    }
                }
                else if (key == "MeshComponent")
                {
                    MeshComponent& meshComp = entity.AddComponent<MeshComponent>();
                    while (*it != '}' && *++it != '}')
                    {

                        if (*it == '\"')
                        {
                            curr_it = ++it;

                            while (*it != '\"')
                            {
                                it++;
                            }
                            key = text.substr(curr_it - text.begin(), it - curr_it);

                            if (key == "Mesh")
                            {
                                do { it++; } while (*it != '\"');
                                curr_it = ++it;
                                while (*it != '\"')
                                    it++;
                                meshComp.m_meshID = text.substr(curr_it - text.begin(), it - curr_it);
                            }
                            if (key == "Material")
                            {
                                do { it++; } while (*it != '\"');
                                curr_it = ++it;
                                while (*it != '\"')
                                    it++;
                                meshComp.m_materialID = text.substr(curr_it - text.begin(), it - curr_it);
                            }
                            if (key == "Shader")
                            {
                                do { it++; } while (*it != '\"');
                                curr_it = ++it;
                                while (*it != '\"')
                                    it++;
                                meshComp.m_shaderID = text.substr(curr_it - text.begin(), it - curr_it);
                            }
                        }
                    }
                }
                else if (key == "Children")
                {
                    while (*it !=']' && *++it != ']')
                    {
                        Entity& child = entity.GetScene()->CreateGameObject<Entity>("child");
                        LoadEntity(it, text, child);
                        child.SetParent(entity);
                        if (it == text.end())
                            return true;
                    }
                    it++;
                    return true;
                }

                it++;
            }
            else
                it++;

}
        return true;
    }


    bool AssetManager::SaveEntity(const Entity& entity, std::ofstream& file)
    {
        file << "{\n\"Name\":\"" + entity.m_Name + "\",\n";
        file << "  \"Transform\":{ \n";

        glm::vec3 v = entity.GetPosition();
        file << "\"Position\":\"" + std::to_string(v.x) + "," + std::to_string(v.y) + "," + std::to_string(v.z) + "\",\n";

        v = entity.GetRotation();
        file << "\"Rotation\":\"" + std::to_string(v.x) + "," + std::to_string(v.y) + "," + std::to_string(v.z) + "\", \n";

        v = entity.GetScale();
        file << "\"Scale\":\"" + std::to_string(v.x) + "," + std::to_string(v.y) + "," + std::to_string(v.z) + "\"\n";
        file << "}";
        if (entity.Has<MeshComponent>())
        {
            file << ",\n";
            file << "\"MeshComponent\":{\n";
            const MeshComponent& meshComp = entity.GetComponent<MeshComponent>();

            file << "\"Mesh\":\"" + meshComp.m_meshID + "\",\n";
            file << "\"Material\":\"" + meshComp.m_materialID + "\",\n";
            file << "\"Shader\":\"" + meshComp.m_shaderID + "\"\n";
            file << "}";
        }
        if (entity.Has<CameraComponent>())
        {
            file << ",\n";
            file << "\"CameraComponent\":{\n";
            const CameraComponent& cameraComp = entity.GetComponent<CameraComponent>();

            file << "\"FOV\":\"" + std::to_string(cameraComp.fov) + "\",\n";
            file << "\"RenderDistance\":\"" + std::to_string(cameraComp.renderDistance) + "\"\n";
            file << "}";
        }

        if (entity.Has<PointLightComponent>())
        {
            file << ",\n";
            file << "\"PointLightComponent\":{\n";
            const PointLightComponent& pointLightComp = entity.GetComponent<PointLightComponent>();
            glm::vec3 v = pointLightComp.m_ambient;
            file << "\"Ambient\":\"" + std::to_string(v.x) + "," + std::to_string(v.y) + "," + std::to_string(v.z) + "\",\n";

            v = pointLightComp.m_diffuse;
            file << "\"Diffuse\":\"" + std::to_string(v.x) + "," + std::to_string(v.y) + "," + std::to_string(v.z) + "\",\n";

            v = pointLightComp.m_specular;
            file << "\"Specular\":\"" + std::to_string(v.x) + "," + std::to_string(v.y) + "," + std::to_string(v.z) + "\",\n";

            file << "\"Constant\":\"" + std::to_string(pointLightComp.m_constant) + "\",\n";
            file << "\"Linear\":\"" + std::to_string(pointLightComp.m_linear) + "\",\n";
            file << "\"Quadratic\":\"" + std::to_string(pointLightComp.m_quadratic) + "\"\n";
            file << "}";
        }

        if (entity.Has<DirLightComponent>())
        {
            file << ",\n";
            file << "\"DirLightComponent\":{\n";
            const DirLightComponent& dirLightComp = entity.GetComponent<DirLightComponent>();
            glm::vec3 v = dirLightComp.m_ambient;
            file << "\"Ambient\":\"" + std::to_string(v.x) + "," + std::to_string(v.y) + "," + std::to_string(v.z) + "\", \n";

            v = dirLightComp.m_diffuse;
            file << "\"Diffuse\":\"" + std::to_string(v.x) + "," + std::to_string(v.y) + "," + std::to_string(v.z) + "\", \n";

            v = dirLightComp.m_specular;
            file << "\"Specular\":\"" + std::to_string(v.x) + "," + std::to_string(v.y) + "," + std::to_string(v.z) + "\",\n";
            
            v = dirLightComp.m_direction;
            file << "\"Direction\":\"" + std::to_string(v.x) + "," + std::to_string(v.y) + "," + std::to_string(v.z) + "\"\n";
            file << "}";
        }
        const std::vector<const Entity*> children = entity.GetChildren();
        
        if (children.size() > 0)
        {
            file << ",\n";
            file << "\n\"Children\":[";
            bool firstChild = true;
            for (auto e : children)
            {
                if (firstChild)
                    firstChild = false;
                else
                    file << ",\n";

                SaveEntity(*e,file);
            }
            file << "]";
        }
        file << "\n}";
        return true;
    }

}
