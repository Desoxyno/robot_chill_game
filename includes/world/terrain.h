#pragma once

#include "math/vectors.h"

#include <cmath>
#include <cstdint>
#include <raylib.h>
#include <raymath.h>
#include <sys/types.h>
#include <vector>
#include "rlgl.h"

#include "utils/random.h"
#include "frustrum_culling.h"
#include "utils/eco.h"

struct Chunk
{
    EcoBoundingBox box = {};
    uint32_t seed = 0;
    uint8_t current_LOD = 0;
    bool visible = false;
};

class Terrain
{
public:

    std::vector<Chunk> chunks;

    customMath::Vector3 position = {0, 0, 0};

    uint8_t height = 1;

    uint16_t width = 5000;

    uint8_t chunkSize = 55;

    unsigned int chunkX = ceil(float(width) / float(chunkSize));

    unsigned int chunkZ = ceil(float(width) / float(chunkSize));

    Model grass_model_near;
    Model grass_model_far;
    Model grass_model_very_far;
    Model grass_model_farthest;

    Shader instancing_shader;
    Shader instancing_shader_far;

    int windTimeLoc = -1;

    int seed_loc = -1;

    int minx_loc = -1;
    int maxx_loc = -1;
    int minz_loc = -1;
    int maxz_loc = -1;
    int stride_loc = -1;

    int mvp_loc = -1;

    unsigned int chunks_visible = 0;

    unsigned int chunks_draw_LOD0 = 0;
    unsigned int chunks_draw_LOD1 = 0;
    unsigned int chunks_draw_LOD2 = 0;
    unsigned int chunks_draw_LOD3 = 0;

    Terrain(customMath::Vector3 position)
        : position(position)
    {
        grass_model_near =
            LoadModel("assets/grass/grass_LOD0.glb");

        grass_model_far =
            LoadModel("assets/grass/grass_LOD1.glb");

        grass_model_very_far =
            LoadModel("assets/grass/grass_LOD2.glb");

        grass_model_farthest =
            LoadModel("assets/grass/grass_LOD3.glb");

        instancing_shader =
            LoadShader(
                "assets/shaders/instancing.vs",
                "assets/shaders/instancing.fs"
            );

        instancing_shader_far =
            LoadShader(
                "assets/shaders/instancing_far.vs",
                "assets/shaders/instancing.fs"
            );

        seed_loc = GetShaderLocation(instancing_shader, "chunkSeed");

        minx_loc = GetShaderLocation(instancing_shader, "minx");
        maxx_loc = GetShaderLocation(instancing_shader, "maxx");
        minz_loc = GetShaderLocation(instancing_shader, "minz");
        maxz_loc = GetShaderLocation(instancing_shader, "maxz");
        stride_loc = GetShaderLocation(instancing_shader, "instanceStride");

        mvp_loc = GetShaderLocation(instancing_shader, "mvp");

        instancing_shader.locs[SHADER_LOC_MATRIX_MVP] =
            GetShaderLocation(
                instancing_shader,
                "mvp"
            );

        instancing_shader.locs[SHADER_LOC_VERTEX_POSITION] =
        GetShaderLocationAttrib(instancing_shader, "vertexPosition");

        windTimeLoc = GetShaderLocation(instancing_shader, "windTime");

        GenerateTerrain();
    }

    const uint32_t GetDistance(const Chunk& chunk, const Camera3D& camera) const {
            float centreX =
                (float(chunk.box.min.x) + float(chunk.box.max.x)) / 2;

            float centreY =
                (float(chunk.box.min.y) + float(chunk.box.max.y)) / 2;

            float centreZ =
                (float(chunk.box.min.z) + float(chunk.box.max.z)) / 2;


            float distance =
                (camera.position.x - centreX) * (camera.position.x - centreX)
                +
                (camera.position.y - centreY) * (camera.position.y - centreY)
                +
                (camera.position.z - centreZ) * (camera.position.z - centreZ);

            return distance;
    }


    void GenerateTerrain()
    {
        unsigned int terrainMinX = 0;

        unsigned int terrainMinZ = 0;

        for (int cz = 0; cz < chunkZ; cz++)
        {
            for (int cx = 0; cx < chunkX; cx++)
            {
                Chunk new_chunk;

                new_chunk.box.min.x = int16_t(
                    terrainMinX + cx * chunkSize);

                new_chunk.box.min.z = int16_t(
                    terrainMinZ + cz * chunkSize);


                new_chunk.box.max.x = int16_t(
                    new_chunk.box.min.x + chunkSize);

                new_chunk.box.max.z = int16_t(
                    new_chunk.box.min.z + chunkSize);

                new_chunk.box.min.y = 0;

                new_chunk.box.max.y = 3;

                new_chunk.seed = hash32(50 + cx * 73856093u + cz * 19349663u);

                chunks.push_back(new_chunk);

                }

        }
         
    }

    void SetShaders(const Shader& shader, const Chunk &chunk, const Matrix mvp, const int step, const float windTime, const bool far) {
            if (!far) {
                SetShaderValue(shader, windTimeLoc, &windTime, SHADER_UNIFORM_FLOAT);
            }

            unsigned int tempMiX = unsigned(int(chunk.box.min.x));
            unsigned int tempMaX = unsigned(int(chunk.box.max.x));
            unsigned int tempMiZ = unsigned(int(chunk.box.min.z));
            unsigned int tempMaZ = unsigned(int(chunk.box.max.z));
            
            SetShaderValueMatrix(shader, mvp_loc, mvp);
            SetShaderValue(shader, seed_loc, &chunk.seed, SHADER_UNIFORM_UINT);
            SetShaderValue(shader, minx_loc, &tempMiX, SHADER_UNIFORM_UINT);
            SetShaderValue(shader, maxx_loc, &tempMaX, SHADER_UNIFORM_UINT);
            SetShaderValue(shader, minz_loc, &tempMiZ, SHADER_UNIFORM_UINT);
            SetShaderValue(shader, maxz_loc, &tempMaZ, SHADER_UNIFORM_UINT);
            SetShaderValue(shader, stride_loc, &step, SHADER_UNIFORM_UINT);
    }


    void Update(Camera3D& camera)
    {
        Frustum camera_frustum =
            GetFrustum(camera);


        for (auto& chunk : chunks)
        {
            chunk.visible = !IsBoxOutsideFrustum(chunk.box, camera_frustum);
            if (chunk.visible) {
                uint32_t distance = GetDistance(chunk, camera);
                if (distance < 250 * 250) {chunk.current_LOD = 0;}
                else if (distance < 600 * 600) {chunk.current_LOD = 1;}
                else if (distance < 1000 * 1000) {chunk.current_LOD = 2;}
                else if (distance < 2000 * 2000) {chunk.current_LOD = 3;}
                else {chunk.current_LOD = 4;}
        }
    }

    }

    void Draw(const Camera3D& camera)
    {
        DrawCube(raylib_vec({float(width) / 2, 0, float(width) / 2}), width, height, width, BROWN);

        float windTime = static_cast<float>(GetTime());

        chunks_visible = 0;
        chunks_draw_LOD0 = 0;
        chunks_draw_LOD1 = 0;
        chunks_draw_LOD2 = 0;
        chunks_draw_LOD3 = 0;

        Matrix view_matrix = rlGetMatrixModelview();
        Matrix projection_matrix = rlGetMatrixProjection();

        Matrix mvp = MatrixMultiply(view_matrix, projection_matrix);

        BeginShaderMode(instancing_shader);

        for (auto& chunk : chunks)
        {
            if (!chunk.visible || chunk.current_LOD != 0)
                continue;

            chunks_visible++;
            chunks_draw_LOD0++;

            uint8_t step = 1;
            uint16_t grass = 800;
            uint8_t multiplicator = 60;
            bool far = false;

            unsigned int grass_vaoId = grass_model_near.meshes[0].vaoId;
            unsigned int grass_tri_count = grass_model_near.meshes[0].triangleCount;

            SetShaders(instancing_shader, chunk, mvp, step, windTime, false);

            rlEnableVertexArray(grass_vaoId);
            rlDrawVertexArrayElementsInstanced(0, grass_tri_count * 3, nullptr, grass * multiplicator);
            rlDisableVertexArray();

        }

        EndShaderMode();

        BeginShaderMode(instancing_shader_far);

        for (auto& chunk : chunks)
        {
            if (!chunk.visible || chunk.current_LOD == 0)
                continue;

            chunks_visible++;

            Mesh* grass_mesh;

            u_int8_t step = 1;
            u_int16_t grass = 0;
            u_int8_t multiplicator = 70;
            bool far = true;

            switch (chunk.current_LOD) {
                case 1: 
                    step = 2;
                    grass = 600;
                    multiplicator = 8;
                    grass_mesh = &grass_model_far.meshes[0];
                    chunks_draw_LOD1++;
                    break;
                case 2:
                    step = 4;
                    grass = 400;
                    multiplicator = 7;
                    grass_mesh = &grass_model_very_far.meshes[0];
                    chunks_draw_LOD2++;
                    break;
                case 3:
                    step = 8;
                    grass = 50;
                    multiplicator = 6;
                    grass_mesh = &grass_model_farthest.meshes[0];
                    chunks_draw_LOD3++;
                    break;
            }

            unsigned int grass_vaoId = grass_mesh->vaoId;
            unsigned int grass_tri_count = grass_mesh->triangleCount;

            SetShaders(instancing_shader_far, chunk, mvp, step, windTime, true);

            rlEnableVertexArray(grass_vaoId);
            rlDrawVertexArrayElementsInstanced(0, grass_tri_count * 3, nullptr, grass * multiplicator);
            rlDisableVertexArray();

        }

        EndShaderMode();
    }
};