#version 430
#extension GL_ARB_shading_language_420pack : require

#define HLSLCC_ENABLE_UNIFORM_BUFFERS 1
#if HLSLCC_ENABLE_UNIFORM_BUFFERS
#define UNITY_UNIFORM
#else
#define UNITY_UNIFORM uniform
#endif
#define UNITY_SUPPORTS_UNIFORM_LOCATION 1
#if UNITY_SUPPORTS_UNIFORM_LOCATION
#define UNITY_LOCATION(x) layout(location = x)
#define UNITY_BINDING(x) layout(binding = x, std140)
#else
#define UNITY_LOCATION(x)
#define UNITY_BINDING(x) layout(std140)
#endif
precise vec4 u_xlat_precise_vec4;
precise ivec4 u_xlat_precise_ivec4;
precise bvec4 u_xlat_precise_bvec4;
precise uvec4 u_xlat_precise_uvec4;
UNITY_LOCATION(0) uniform  sampler2D FrameTex;
 struct rgbBuffer_type {
	uint[1] value;
};

layout(std430, binding = 0) buffer rgbBuffer {
	rgbBuffer_type rgbBuffer_buf[];
};

layout(rgba32f, binding = 1) uniform image2D imgOutput;

int u_xlati0;
uint u_xlatu0;
float u_xlat1;
uvec4 u_xlatu1;
bvec3 u_xlatb1;
vec3 u_xlat2;
ivec3 u_xlati2;
uvec3 u_xlatu2;
float u_xlat3;
bool u_xlatb3;
float u_xlat4;
float u_xlat6;
float u_xlat7;
bool u_xlatb7;
layout(local_size_x = 8, local_size_y = 8, local_size_z = 1) in;
void main()
{
    u_xlatu0 = (uvec2(textureSize(FrameTex, 0)).x);

    // rgbBuffer_buf[gl_GlobalInvocationID.x].value[0] = u_xlatu0;
    // return;
    //uvec3	gl_GlobalInvocationID	global index of the current work item gl_GlobalInvocationID gives us the absolute coordinate of the current pixel.Remember that we only have one single invocation per work group as we set all local dimensions to 1.
    u_xlatu1.xy = gl_GlobalInvocationID.xy;
    u_xlatu1.z = uint(0u);
    u_xlatu1.w = uint(0u);
    u_xlat2.xyz = texelFetch(FrameTex, ivec2(u_xlatu1.xy), int(u_xlatu1.w)).xyz;
    u_xlati0 = int(gl_GlobalInvocationID.y) * int(u_xlatu0) + int(gl_GlobalInvocationID.x);
    u_xlatb1.xyz = greaterThanEqual(vec4(0.0, 0.0, 0.0, 0.0), u_xlat2.xyzx).xyz;
    if(u_xlatb1.x){
        u_xlat1 = 0.0;
    } else {
        u_xlatb7 = 0.00313080009>=u_xlat2.x;
        if(u_xlatb7){
            u_xlat1 = u_xlat2.x * 12.9200001;
        } else {
            u_xlatb7 = u_xlat2.x<1.0;
            if(u_xlatb7){
                u_xlat7 = log2(u_xlat2.x);
                u_xlat7 = u_xlat7 * 0.416666687;
                u_xlat7 = exp2(u_xlat7);
                u_xlat1 = u_xlat7 * 1.05499995 + -0.0549999997;
            } else {
                u_xlat2.x = log2(u_xlat2.x);
                u_xlat2.x = u_xlat2.x * 0.454545438;
                u_xlat1 = exp2(u_xlat2.x);
            }
        }
    }
    if(u_xlatb1.y){
        u_xlat2.x = 0.0;
    } else {
        u_xlatb3 = 0.00313080009>=u_xlat2.y;
        if(u_xlatb3){
            u_xlat2.x = u_xlat2.y * 12.9200001;
        } else {
            u_xlatb3 = u_xlat2.y<1.0;
            if(u_xlatb3){
                u_xlat3 = log2(u_xlat2.y);
                u_xlat3 = u_xlat3 * 0.416666687;
                u_xlat3 = exp2(u_xlat3);
                u_xlat2.x = u_xlat3 * 1.05499995 + -0.0549999997;
            } else {
                u_xlat4 = log2(u_xlat2.y);
                u_xlat4 = u_xlat4 * 0.454545438;
                u_xlat2.x = exp2(u_xlat4);
            }
        }
    }
    if(u_xlatb1.z){
        u_xlat2.y = 0.0;
    } else {
        u_xlatb3 = 0.00313080009>=u_xlat2.z;
        if(u_xlatb3){
            u_xlat2.y = u_xlat2.z * 12.9200001;
        } else {
            u_xlatb3 = u_xlat2.z<1.0;
            if(u_xlatb3){
                u_xlat3 = log2(u_xlat2.z);
                u_xlat3 = u_xlat3 * 0.416666687;
                u_xlat3 = exp2(u_xlat3);
                u_xlat2.y = u_xlat3 * 1.05499995 + -0.0549999997;
            } else {
                u_xlat6 = log2(u_xlat2.z);
                u_xlat6 = u_xlat6 * 0.454545438;
                u_xlat2.y = exp2(u_xlat6);
            }
        }
    }
    u_xlat2.xy = u_xlat2.xy * vec2(255.0, 255.0);
    u_xlat2.xy = roundEven(u_xlat2.xy);
    u_xlat2.x = u_xlat2.x * 256.0;
    u_xlatu2.xy = uvec2(u_xlat2.xy);
    u_xlat6 = u_xlat1 * 255.0;
    u_xlat6 = roundEven(u_xlat6);
    u_xlat6 = u_xlat6 * 65536.0;
    u_xlatu2.z = uint(u_xlat6);
    u_xlati2.xz = ivec2(u_xlatu2.xz & uvec2(65280u, 16711680u));
    u_xlati2.x = u_xlati2.z + u_xlati2.x;
    u_xlati2.x = int(bitfieldInsert(u_xlati2.x, int(u_xlatu2.y), 0 & 0x1F, 8));
    rgbBuffer_buf[u_xlati0].value[(0 >> 2)] = uint(u_xlati2.x);

vec4 value = vec4(0.0, 0.0, 0.0, 1.0);
    ivec2 texelCoord = ivec2(gl_GlobalInvocationID.xy);
    
    value.x = u_xlat2.x;
    value.y = u_xlat2.y;
    imageStore(imgOutput, texelCoord, value);

    return;
}