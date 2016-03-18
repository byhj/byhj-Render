#pragma once
////// VERTEX SHADER
struct VS_INPUT
{
	float2 pos : POSITION0;
	float2 size : POSITION1;
	float2 uvStart : TEXCOORD0;
	float2 uvEnd : TEXCOORD1;
};

VS_INPUT main(VS_INPUT input)
{
	VS_INPUT output;
	output.pos = mul(float4(input.pos, 0.f, 1.f), World).xy * InvResolution * 2.f - float2(1.f, 1.f);
	output.pos.y = -output.pos.y;
	output.size = input.size * InvResolution * 2.f;
	output.size.y = -output.size.y;
	output.uvStart = input.uvStart;
	output.uvEnd = input.uvEnd;

	return output;
}
