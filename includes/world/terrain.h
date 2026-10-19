#pragma once

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

#ifdef DEBUG_BUILD
    #include "../timer.h"
#endif

constexpr uint8_t VISIBLE = 1 << 0;
constexpr uint8_t LOD_MASK = 0b00000110;

struct Chunk
{
    EcoBoundingBox box = {};
    uint8_t flags = 0;
};

static_assert(sizeof(Chunk) <= 14);
static_assert(sizeof(MicroVector3) == 4);

typedef struct TerrainData {
    unsigned int resolution = 255;
    unsigned int width = 5000;
    unsigned int heightmap = 0;
    unsigned int chunkSize = 55;
    unsigned int chunkX = ceil(width / chunkSize);
    unsigned int chunkZ = ceil(float(width) / float(chunkSize));
    Mesh ground;

    std::vector<Chunk> chunks;
    uint32_t seeds_buffer[8281];

} TerrainData;

constexpr float getHeight(float x, float z) {
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

inline void GenerateTerrainMesh(TerrainData &data) {
    std::vector<Vector3> vertex_array;
    vertex_array.reserve(data.resolution * data.resolution);

    std::vector<float> heights;
    heights.resize(data.resolution * data.resolution);

    std::vector<unsigned short> indices_array;
        
    Vector3 vertex_pos = {0, 0, 0};
    float spacing = float(data.width) / (data.resolution - 1);
    Mesh new_mesh = { 0 };

    for (uint16_t i = 0; i < data.resolution; i++) {
        for (uint16_t j = 0; j < data.resolution; j++) {
            vertex_pos = {(i * spacing), 0, (j * spacing)};

            float height = getHeight(vertex_pos.x, vertex_pos.z);

            heights[i * data.resolution + j] = height;

            vertex_pos.y = height;

            vertex_array.push_back(vertex_pos);
        }
    }

    for (uint16_t i = 0; i < data.resolution - 1; i++) {
        for (uint16_t j = 0; j < data.resolution - 1; j++) {
            uint32_t a = i * data.resolution + j;
            uint32_t b = a + 1;
            uint32_t c = (i + 1) * data.resolution + j;
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

    data.ground = new_mesh;

    data.heightmap = rlLoadTexture(heights.data(), data.resolution, data.resolution, RL_PIXELFORMAT_UNCOMPRESSED_R32, 1);
}

inline void GenerateTerrain(TerrainData &data) {
    #ifdef DEBUG_BUILD
        measure(GenerateTerrainMesh, data, "GenerateTerrainMesh");
    #else
        GenerateTerrainMesh(data);
    #endif

    unsigned int terrainMinX = 0;
    unsigned int terrainMinZ = 0;

    for (int cz = 0; cz < data.chunkZ; cz++)
    {
        for (int cx = 0; cx < data.chunkX; cx++)
        {
            Chunk new_chunk;

            new_chunk.box.min.x = int16_t(
                terrainMinX + cx * data.chunkSize);

            new_chunk.box.min.z = int16_t(
                terrainMinZ + cz * data.chunkSize);


            new_chunk.box.max.x = int16_t(
                new_chunk.box.min.x + data.chunkSize);

            new_chunk.box.max.z = int16_t(
                new_chunk.box.min.z + data.chunkSize);

            new_chunk.box.min.y = 0;

            new_chunk.box.max.y = 8;

            data.chunks.push_back(new_chunk);

            data.seeds_buffer[cx * data.chunkX + cz] = hash32(50 + cx * 73856093u + cz * 19349663u);;

            }
        }
}

typedef struct TerrainRenderer
{
    TerrainData *data = nullptr;

     Matrix floor_transform = MatrixCompose({0, 0, 0}, QuaternionIdentity(), {1, 1, 1});

    Model grass_model_near = LoadModel("assets/grass/grass_LOD0.glb");
    Model grass_model_far = LoadModel("assets/grass/grass_LOD1.glb");
    Model grass_model_very_far = LoadModel("assets/grass/grass_LOD2.glb");;
    Model grass_model_farthest = LoadModel("assets/grass/grass_LOD3.glb");;

    Shader instancing_shader = LoadShader("assets/shaders/instancing.vert", "assets/shaders/instancing.frag");

    Shader terrain_shader = LoadShader("assets/shaders/terrain.vert", "assets/shaders/terrain.frag");

    Material terrain_material = LoadMaterialDefault();

    unsigned int seeds_ssbo = 0;

    int seed_loc = GetShaderLocation(instancing_shader, "chunkSeed");
    int minx_loc = GetShaderLocation(instancing_shader, "minx");
    int maxx_loc = GetShaderLocation(instancing_shader, "maxx");
    int minz_loc = GetShaderLocation(instancing_shader, "minz");
    int maxz_loc = GetShaderLocation(instancing_shader, "maxz");
    int stride_loc = GetShaderLocation(instancing_shader, "instanceStride");
    int heights_loc = GetShaderLocation(instancing_shader, "terrainHeightmap");
    int mvp_loc = GetShaderLocation(instancing_shader, "mvp");
    int windTimeLoc = GetShaderLocation(instancing_shader, "windTime");

    Texture2D h_map{};

    void Initialize(TerrainData &data) {
        this->data = &data;

        seeds_ssbo = rlLoadShaderBuffer(sizeof(data.seeds_buffer), data.seeds_buffer, RL_STATIC_COPY);
        setShaders();
        h_map = Texture2D(this->data->heightmap, this->data->resolution, this->data->resolution, 1, RL_PIXELFORMAT_UNCOMPRESSED_R32);
    }

    void setShaders() {
        instancing_shader.locs[SHADER_LOC_MATRIX_MVP] = GetShaderLocation(instancing_shader, "mvp");
        terrain_shader.locs[SHADER_LOC_MATRIX_MVP] = GetShaderLocation(terrain_shader, "mvp");
        instancing_shader.locs[SHADER_LOC_VERTEX_POSITION] = GetShaderLocationAttrib(instancing_shader, "vertexPosition");
        rlActiveTextureSlot(1);
        rlEnableTexture(data->heightmap);
        SetShaderValueTexture(instancing_shader, heights_loc, h_map);
        terrain_material.shader = terrain_shader;
    }

} TerrainRenderer;

constexpr float GetDistance(const Chunk& chunk, const Camera3D& camera) {
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

typedef struct Terrain {
    TerrainData data;

    TerrainRenderer renderer;

    Terrain() {
        GenerateTerrain(data);
        renderer.Initialize(data);
    }

    void Update(Camera3D& camera)
    {
        Frustum camera_frustum = GetFrustum(camera);

        for (auto& chunk : data.chunks)
        {
            if (!IsBoxOutsideFrustum(chunk.box, camera_frustum))
                chunk.flags |= VISIBLE;
            else chunk.flags &= ~VISIBLE;

            bool visible = chunk.flags & VISIBLE;
            if (visible) {
                
                float distance = GetDistance(chunk, camera);
                if (distance < 250 * 250) {chunk.flags = (chunk.flags & ~LOD_MASK) | (0 << 1);}
                else if (distance < 600 * 600) {chunk.flags = (chunk.flags & ~LOD_MASK) | (1 << 1);}
                else if (distance < 1000 * 1000) {chunk.flags = (chunk.flags & ~LOD_MASK) | (2 << 1);}
                else if (distance < 2000 * 2000) {chunk.flags = (chunk.flags & ~LOD_MASK) | (3 << 1);}
                else [[likely]] {chunk.flags &= ~VISIBLE;}
            }
        }
    }

    void chunkSetShaders(const Shader& shader, const Chunk &chunk, const Matrix &mvp, const int &step, const float &windTime) {

            int tempMiX = int(chunk.box.min.x);
            int tempMaX = int(chunk.box.max.x);
            int tempMiZ = int(chunk.box.min.z);
            int tempMaZ = int(chunk.box.max.z);
            
            SetShaderValue(shader, renderer.minx_loc, &tempMiX, SHADER_UNIFORM_INT);
            SetShaderValue(shader, renderer.maxx_loc, &tempMaX, SHADER_UNIFORM_INT);
            SetShaderValue(shader, renderer.minz_loc, &tempMiZ, SHADER_UNIFORM_INT);
            SetShaderValue(shader, renderer.maxz_loc, &tempMaZ, SHADER_UNIFORM_INT);

            SetShaderValue(shader, renderer.stride_loc, &step, SHADER_UNIFORM_UINT);
    }

    void Draw(const Camera3D& camera)
    {

        float windTime = static_cast<float>(GetTime());

        Matrix view_matrix = rlGetMatrixModelview();
        Matrix projection_matrix = rlGetMatrixProjection();

        Matrix mvp = MatrixMultiply(view_matrix, projection_matrix);
        
        DrawMesh(data.ground, renderer.terrain_material, renderer.floor_transform);

        SetShaderValueMatrix(renderer.instancing_shader, renderer.mvp_loc, mvp);
        SetShaderValue(renderer.instancing_shader, renderer.windTimeLoc, &windTime, SHADER_UNIFORM_FLOAT);
        rlBindShaderBuffer(renderer.seeds_ssbo, 0);

        BeginShaderMode(renderer.instancing_shader);

        for (auto& chunk : data.chunks)
        {
            
            if ((chunk.flags & VISIBLE) == 0)
                continue;

            Mesh* grass_mesh;

            u_int8_t step = 1;
            u_int16_t grass = 0;
            u_int8_t multiplicator = 0;

            switch ((chunk.flags & LOD_MASK) >> 1) {
                case 0:
                    step = 1;
                    grass = 800;
                    multiplicator = 20;
                    grass_mesh = &renderer.grass_model_near.meshes[0];
                    break;
                case 1: 
                    step = 2;
                    grass = 600;
                    multiplicator = 13;
                    grass_mesh = &renderer.grass_model_far.meshes[0];
                    break;
                case 2:
                    step = 4;
                    grass = 400;
                    multiplicator = 10;
                    grass_mesh = &renderer.grass_model_very_far.meshes[0];
                    break;
                case 3:
                    step = 8;
                    grass = 50;
                    multiplicator = 5;
                    grass_mesh = &renderer.grass_model_farthest.meshes[0];
                    break;
            }

            [[assume(grass_mesh != nullptr)]];

            unsigned int grass_vaoId = grass_mesh->vaoId;
            unsigned int grass_tri_count = grass_mesh->triangleCount;

            chunkSetShaders(renderer.instancing_shader, chunk, mvp, 1, windTime);

            rlEnableVertexArray(grass_vaoId);
            rlDrawVertexArrayElementsInstanced(0, grass_tri_count * 3, nullptr, grass * multiplicator);
            rlDisableVertexArray();

        }

        EndShaderMode();

    }
    
} Terrain;