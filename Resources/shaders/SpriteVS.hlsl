#include"Sprite.hlsli"
VSOutput main(float4 pos:POSITION,float2 uv : TEXCOORD)
{
     VSOutput output; // ピクセルシェーダーに渡す値
    //output.svpos = pos;
     output.svpos = mul(mat,pos);
    output.uv = uv;
    return output;
}




//#include "Basic.hlsli"
//
//VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
//{
//    VSOutput output;
//    output.svpos = mul(mat, pos); // 座標に行列を乗算
//    output.normal = normal;
//    output.uv = uv;
//    return output;
//}