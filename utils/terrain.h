#pragma once

#include "constant.h"
#include "vectors.h"

#include <cmath>
#include <raylib.h>
#include <raymath.h>
#include <vector>

#include "random.h"
#include "frustrum_culling.h"


struct GrassInstance
{
    customMath::Vector3 position = {0, 0, 0};

    float rotation_y = 0;

    customMath::Vector3 size = {1, 1, 1};

    GrassInstance(
        customMath::Vector3 position,
        float rotation_y,
        customMath::Vector3 size
    )
        : position(position),
          rotation_y(rotation_y),
          size(size)
    {
    }

    const Matrix getMatrix()
    {
        Quaternion rotation =
            QuaternionFromAxisAngle(
                {0, 1, 0},
                rotation_y * radian
            );

        return MatrixCompose(
            raylib_vec(position),
            rotation,
            raylib_vec(size)
        );
    }
};


struct Chunk
{
    BoundingBox box = {};

    std::vector<Matrix> transforms_LOD0;
    std::vector<Matrix> transforms_LOD1;
    std::vector<Matrix> transforms_LOD2;

    bool visible = false;

    int current_LOD = 0;
};


class Terrain
{
public:

    std::vector<Chunk> chunks;

    customMath::Vector3 position = {0, 0, 0};

    float height = 1;

    float width = 5000.0f;

    int chunkSize = 25;

    int chunkX = ceil(width / chunkSize);

    int chunkZ = ceil(width / chunkSize);


    Model grass_model_near;

    Model grass_model_far;

    Material grass_material = LoadMaterialDefault();

    Shader instancing_shader;

    int windTimeLoc = -1;

    int grass_n = 7000000;

    int current_LOD = 0;


    Terrain(customMath::Vector3 position)
        : position(position)
    {
        grass_model_near =
            LoadModel("assets/grass_LOD0.glb");

        grass_model_far =
            LoadModel("assets/grass_LOD1.glb");


        instancing_shader =
            LoadShader(
                "shaders/instancing.vs",
                "shaders/instancing.fs"
            );


        instancing_shader.locs[SHADER_LOC_MATRIX_MVP] =
            GetShaderLocation(
                instancing_shader,
                "mvp"
            );


        windTimeLoc =
            GetShaderLocation(
                instancing_shader,
                "windTime"
            );


        grass_material.shader =
            instancing_shader;

        grass_material.maps[MATERIAL_MAP_DIFFUSE].color =
            Color{70, 140, 50, 255};


        GenerateTerrain();
    }


    void GenerateTerrain()
    {
        int terrainMinX =
            position.x - width / 2;

        int terrainMinZ =
            position.z - width / 2;


        for (int cz = 0; cz < chunkZ; cz++)
        {
            for (int cx = 0; cx < chunkX; cx++)
            {
                Chunk new_chunk;


                new_chunk.box.min.x =
                    terrainMinX + cx * chunkSize;

                new_chunk.box.min.z =
                    terrainMinZ + cz * chunkSize;


                new_chunk.box.max.x =
                    new_chunk.box.min.x + chunkSize;

                new_chunk.box.max.z =
                    new_chunk.box.min.z + chunkSize;


                new_chunk.box.min.y = 0.5;

                new_chunk.box.max.y = 2.5;


                chunks.push_back(new_chunk);
            }
        }


        grass_material.shader =
            instancing_shader;


        for (int i = 0; i < grass_n; i++)
        {
            customMath::Vector3 grass_position =
            {
                generate_rd_n(
                    float(position.x - width / 2),
                    float(position.x + width / 2)
                ),

                0.5,

                generate_rd_n(
                    float(position.z - width / 2),
                    float(position.z + width / 2)
                )
            };


            customMath::Vector3 grass_size =
            {
                generate_rd_n(1.0f, 2.0f),
                generate_rd_n(1.0f, 2.0f),
                generate_rd_n(1.0f, 2.0f)
            };


            GrassInstance new_grass =
                GrassInstance(
                    grass_position,
                    generate_rd_n(-180, 180),
                    grass_size
                );


            int chunkXc =
                floor(
                    (grass_position.x - terrainMinX)
                    / chunkSize
                );

            int chunkZc =
                floor(
                    (grass_position.z - terrainMinZ)
                    / chunkSize
                );


            int index =
                chunkZc * chunkX + chunkXc;


            Matrix grass_matrix =
                new_grass.getMatrix();


            chunks[index]
                .transforms_LOD0
                .push_back(grass_matrix);


            if (i % 2 == 0)
            {
                chunks[index]
                    .transforms_LOD1
                    .push_back(grass_matrix);
            }


            if (i % 4 == 0)
            {
                chunks[index]
                    .transforms_LOD2
                    .push_back(grass_matrix);
            }
        }
    }


    void Update(Camera3D& camera)
    {
        Frustum camera_frustum =
            GetFrustum(camera);


        for (auto& chunk : chunks)
        {
            chunk.visible =
                !IsBoxOutsideFrustum(
                    chunk.box,
                    camera_frustum
                );
        }
    }


    void Draw(const Camera3D& camera)
    {
        DrawCube(
            raylib_vec(position),
            width,
            height,
            width,
            BROWN
        );


        float windTime =
            static_cast<float>(GetTime());


        SetShaderValue(
            instancing_shader,
            windTimeLoc,
            &windTime,
            SHADER_UNIFORM_FLOAT
        );


        for (auto& chunk : chunks)
        {
            if (!chunk.visible)
                continue;


            float centreX =
                (chunk.box.min.x + chunk.box.max.x) / 2;

            float centreY =
                (chunk.box.min.y + chunk.box.max.y) / 2;

            float centreZ =
                (chunk.box.min.z + chunk.box.max.z) / 2;


            double distance =
                (camera.position.x - centreX)
                * (camera.position.x - centreX)

                +

                (camera.position.y - centreY)
                * (camera.position.y - centreY)

                +

                (camera.position.z - centreZ)
                * (camera.position.z - centreZ);


            if (distance < 90 * 90)
            {
                chunk.current_LOD = 0;
            }


            if (distance >= 90 * 90)
            {
                chunk.current_LOD = 1;
            }


            if (distance >= 240 * 240)
            {
                chunk.current_LOD = 2;
            }


            switch (chunk.current_LOD)
            {
                case 0:

                    DrawMeshInstanced(
                        grass_model_near.meshes[0],
                        grass_material,
                        chunk.transforms_LOD0.data(),
                        static_cast<int>(
                            chunk.transforms_LOD0.size()
                        )
                    );

                    break;


                case 1:

                    DrawMeshInstanced(
                        grass_model_far.meshes[0],
                        grass_material,
                        chunk.transforms_LOD1.data(),
                        static_cast<int>(
                            chunk.transforms_LOD1.size()
                        )
                    );

                    break;


                case 2:

                    DrawMeshInstanced(
                        grass_model_far.meshes[0],
                        grass_material,
                        chunk.transforms_LOD2.data(),
                        static_cast<int>(
                            chunk.transforms_LOD2.size()
                        )
                    );

                    break;
            }
        }
    }
};