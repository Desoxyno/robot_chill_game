#pragma once

#include "math/vectors.h"

#include <cmath>
#include <cstdint>
#include <cstring>
#include <raylib.h>
#include <raymath.h>
#include <sys/types.h>
#include <vector>
#include "rlgl.h"

#include "utils/random.h"
#include "frustrum_culling.h"
#include "utils/eco.h"

struct GrassInstance {
    uint random_seed;
};

struct ChunkCPU
{
    EcoBoundingBox box = {};
    uint32_t seed = 0;
    uint8_t current_LOD = 0;
    bool visible = false;
    std::vector<GrassInstance> instances;
};

class TerrainGeneration {
    public:

    unsigned int resolution;
    unsigned int width;
    unsigned int heightmap;
    unsigned int chunkSize;
    unsigned int chunkX;
    unsigned int chunkZ;

    Mesh floor;

    TerrainGeneration(float width, float chunkSize, unsigned int resolution) : width(width), chunkSize(chunkSize), resolution(resolution) {
            chunkX = ceil(width / chunkSize);
            chunkZ = ceil(width / chunkSize);
            GenerateTerrain();
    }

   
    std::vector<ChunkCPU> chunks;

    float getHeight(float x, float z) {
        float gx = x / 100.0f;
        float gz = z / 100.0f;

        int cellX = int(std::floor(gx));
        int cellZ = int(std::floor(gz));

        float fracX = gx - std::floor(gx);
        float fracZ = gz - std::floor(gz);

        uint hashA = uint(cellX) * 0x45d9f3bu + uint(cellZ) * 0x23e8f9cu;
        uint hashB = uint(cellX) * 0x45d9f3bu + uint(cellZ + 1) * 0x23e8f9cu;
        uint hashC = uint(cellX + 1) * 0x45d9f3bu + uint(cellZ) * 0x23e8f9cu;
        uint hashD = uint(cellX + 1) * 0x45d9f3bu + uint(cellZ + 1) * 0x23e8f9cu;

        hashA = hash32(hashA);
        hashB = hash32(hashB);
        hashC = hash32(hashC);
        hashD = hash32(hashD);

        float A = float(hashA) / 4294967295.0f * 8.0f;
        float B = float(hashB) / 4294967295.0f * 8.0f;
        float C = float(hashC) / 4294967295.0f * 8.0f;
        float D = float(hashD) / 4294967295.0f * 8.0f;

        float height;

        if (fracX + fracZ <= 1.0f) {
            float weightA = 1.0f - fracX - fracZ;
            float weightB = fracZ;
            float weightC = fracX;

            height = A * weightA + B * weightB + C * weightC;
        }

        else {
            float weightB = 1.0f - fracX;
            float weightD = fracX + fracZ - 1.0f;
            float weightC = 1.0f - fracZ;

            height = B * weightB + D * weightD + C * weightC;
                }
        return height;
    }

    void GenerateTerrainMesh() {
        std::vector<Vector3> vertex_array;
        std::vector<float> heights;
        heights.resize(resolution * resolution);
        std::vector<unsigned short> indices_array;
        vertex_array.reserve(resolution * resolution);
        Vector3 vertex_pos = {0, 0, 0};
        float spacing = float(width) / (resolution - 1);
        Mesh new_mesh = { 0 };

        for (uint16_t i = 0; i < resolution; i++) {
            for (uint16_t j = 0; j < resolution; j++) {
                vertex_pos = {(i * spacing), 0, (j * spacing)};

                float height = getHeight(vertex_pos.x, vertex_pos.z);

                heights[i * resolution + j] = height;

                vertex_pos.y = height;

                vertex_array.push_back(vertex_pos);
            }
        }

        for (uint16_t i = 0; i < resolution - 1; i++) {
            for (uint16_t j = 0; j < resolution - 1; j++) {
                uint32_t a = i * resolution + j;
                uint32_t b = a + 1;
                uint32_t c = (i + 1) * resolution + j;
                uint32_t d = c + 1;

                indices_array.push_back(a);
                indices_array.push_back(b);
                indices_array.push_back(c);

                indices_array.push_back(b);
                indices_array.push_back(d);
                indices_array.push_back(c);
            }
        }

        new_mesh.triangleCount = indices_array.size() / 3;
        new_mesh.vertexCount = vertex_array.size();

        std::vector<float> c_vertex_array;

        for (auto vec : vertex_array) {
            c_vertex_array.push_back(vec.x);
            c_vertex_array.push_back(vec.y);
            c_vertex_array.push_back(vec.z);
        }

        new_mesh.vertices = (float*)MemAlloc(c_vertex_array.size() * sizeof(float));
        memcpy(new_mesh.vertices, c_vertex_array.data(), c_vertex_array.size() * sizeof(float));

        new_mesh.indices = (unsigned short*)MemAlloc(indices_array.size() * sizeof(unsigned short));
        memcpy(new_mesh.indices, indices_array.data(), indices_array.size() * sizeof(unsigned short));

        UploadMesh(&new_mesh, false);

        floor = new_mesh;

        heightmap = rlLoadTexture(heights.data(), resolution, resolution, RL_PIXELFORMAT_UNCOMPRESSED_R32, 1);
    }

    void GenerateTerrain()
    {
        GenerateTerrainMesh();

        unsigned int terrainMinX = 0;

        unsigned int terrainMinZ = 0;

        for (int cz = 0; cz < chunkZ; cz++)
        {
            for (int cx = 0; cx < chunkX; cx++)
            {
                ChunkCPU new_chunk;

                new_chunk.box.min.x = int16_t(
                    terrainMinX + cx * chunkSize);

                new_chunk.box.min.z = int16_t(
                    terrainMinZ + cz * chunkSize);


                new_chunk.box.max.x = int16_t(
                    new_chunk.box.min.x + chunkSize);

                new_chunk.box.max.z = int16_t(
                    new_chunk.box.min.z + chunkSize);

                new_chunk.box.min.y = 0;

                new_chunk.box.max.y = 8;

                new_chunk.seed = hash32(50 + cx * 73856093u + cz * 19349663u);

                new_chunk.instances.push_back(GrassInstance(2154242)); // Test

                chunks.push_back(new_chunk);

                }

        }
         
    }
};

class Terrain
{
public:
    customMath::Vector3 position = {0, 0, 0};
    customMath::Vector3 sunDirection = {0.5, -1.0, 0.3};
    float sun_intensity = 10;

    Model grass_model_near;
    Model grass_model_far;
    Model grass_model_very_far;
    Model grass_model_farthest;

    Shader instancing_shader;
    Shader instancing_shader_far;

    Shader terrain_shader;

    Material terrain_material = LoadMaterialDefault();

    float gpu_frame_time = 0;

    int windTimeLoc = -1;

    int seed_loc = -1;

    int minx_loc = -1;
    int maxx_loc = -1;
    int minz_loc = -1;
    int maxz_loc = -1;
    int heights_loc = -1;

    int mvp_loc = -1;

    unsigned int chunks_visible = 0;

    unsigned int chunks_draw_LOD0 = 0;
    unsigned int chunks_draw_LOD1 = 0;
    unsigned int chunks_draw_LOD2 = 0;
    unsigned int chunks_draw_LOD3 = 0;

    TerrainGeneration terrain_gen = TerrainGeneration(5000, 55, 255);

    Texture2D h_map = Texture2D(terrain_gen.heightmap, terrain_gen.resolution, terrain_gen.resolution, 1, RL_PIXELFORMAT_UNCOMPRESSED_R32);

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
                "assets/shaders/instancing.vert",
                "assets/shaders/instancing.frag"
            );

        instancing_shader_far =
            LoadShader(
                "assets/shaders/instancing_far.vert",
                "assets/shaders/instancing.frag"
            );

        terrain_shader = LoadShader("assets/shaders/terrain.vert", "assets/shaders/terrain.frag");
        terrain_material.shader = terrain_shader;

        seed_loc = GetShaderLocation(instancing_shader, "chunkSeed");

        minx_loc = GetShaderLocation(instancing_shader, "minx");
        maxx_loc = GetShaderLocation(instancing_shader, "maxx");
        minz_loc = GetShaderLocation(instancing_shader, "minz");
        maxz_loc = GetShaderLocation(instancing_shader, "maxz");
        heights_loc = GetShaderLocation(instancing_shader, "terrainHeightmap");

        mvp_loc = GetShaderLocation(instancing_shader, "mvp");

        instancing_shader.locs[SHADER_LOC_MATRIX_MVP] = GetShaderLocation(instancing_shader, "mvp");
        terrain_shader.locs[SHADER_LOC_MATRIX_MVP] = GetShaderLocation(terrain_shader, "mvp");

        instancing_shader.locs[SHADER_LOC_VERTEX_POSITION] = GetShaderLocationAttrib(instancing_shader, "vertexPosition");

        windTimeLoc = GetShaderLocation(instancing_shader, "windTime");

        rlActiveTextureSlot(1);
        rlEnableTexture(terrain_gen.heightmap);
        SetShaderValueTexture(instancing_shader, heights_loc, h_map);
        SetShaderValueTexture(instancing_shader_far, heights_loc, h_map);
    }

    const float GetDistance(const ChunkCPU& chunk, const Camera3D& camera) const {
            float centreX = (float(chunk.box.min.x) + float(chunk.box.max.x)) / 2;

            float centreY = (float(chunk.box.min.y) + float(chunk.box.max.y)) / 2;

            float centreZ = (float(chunk.box.min.z) + float(chunk.box.max.z)) / 2;


            float distance =
                (camera.position.x - centreX) * (camera.position.x - centreX)
                +
                (camera.position.y - centreY) * (camera.position.y - centreY)
                +
                (camera.position.z - centreZ) * (camera.position.z - centreZ);

            return distance;
    }

    void SetShaders(const Shader& shader, const ChunkCPU &chunk, const Matrix &mvp, const int &step, const float &windTime, const bool &far) {
            if (!far) {
                SetShaderValue(shader, windTimeLoc, &windTime, SHADER_UNIFORM_FLOAT);
            }

            int tempMiX = int(chunk.box.min.x);
            int tempMaX = int(chunk.box.max.x);
            int tempMiZ = int(chunk.box.min.z);
            int tempMaZ = int(chunk.box.max.z);
            
            SetShaderValue(shader, seed_loc, &chunk.seed, SHADER_UNIFORM_UINT);
            SetShaderValue(shader, minx_loc, &tempMiX, SHADER_UNIFORM_INT);
            SetShaderValue(shader, maxx_loc, &tempMaX, SHADER_UNIFORM_INT);
            SetShaderValue(shader, minz_loc, &tempMiZ, SHADER_UNIFORM_INT);
            SetShaderValue(shader, maxz_loc, &tempMaZ, SHADER_UNIFORM_INT);
    }

    void Update(Camera3D& camera)
    {
        Frustum camera_frustum = GetFrustum(camera);

        for (auto& chunk : terrain_gen.chunks)
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
        unsigned int grass_vaoId = grass_model_near.meshes[0].vaoId;
        unsigned int grass_tri_count = grass_model_near.meshes[0].triangleCount;

        float windTime = static_cast<float>(GetTime());

        chunks_visible = 0;
        chunks_draw_LOD0 = 0;
        chunks_draw_LOD1 = 0;
        chunks_draw_LOD2 = 0;
        chunks_draw_LOD3 = 0;

        Matrix view_matrix = rlGetMatrixModelview();
        Matrix projection_matrix = rlGetMatrixProjection();

        Matrix mvp = MatrixMultiply(view_matrix, projection_matrix);

        Matrix floor_transform = MatrixCompose({0, 0, 0}, QuaternionIdentity(), {1, 1, 1});

        rlDisableBackfaceCulling();
        DrawMesh(terrain_gen.floor, terrain_material, floor_transform);
        rlEnableBackfaceCulling();

        float start_time = GetTime();

        BeginShaderMode(instancing_shader);

        SetShaderValueMatrix(instancing_shader, mvp_loc, mvp);
        SetShaderValueMatrix(instancing_shader_far, mvp_loc, mvp);

        rlEnableVertexArray(grass_vaoId);

        for (auto& chunk : terrain_gen.chunks)
        {
            if (!chunk.visible || chunk.current_LOD != 0)
                continue;

            chunks_visible++;
            chunks_draw_LOD0++;

            SetShaders(instancing_shader, chunk, mvp, 1, windTime, false);

            
            rlDrawVertexArrayElementsInstanced(0, grass_tri_count * 3, nullptr, 800 * 60);
            

        }

        rlDisableVertexArray();

        EndShaderMode();

        BeginShaderMode(instancing_shader_far);

        for (auto& chunk : terrain_gen.chunks)
        {
            if (!chunk.visible || chunk.current_LOD == 0 || chunk.current_LOD == 4)
                continue;

            chunks_visible++;

            Mesh* grass_mesh;

            u_int8_t step = 1;
            u_int16_t grass = 0;
            u_int8_t multiplicator = 0;

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

        gpu_frame_time = GetTime() - start_time;
    }
};

