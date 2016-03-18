struct GS_OUTPUT
{
	float2 uv : TEXCOORD0;
	float4 pos : SV_Position;
};

[maxvertexcount(4)]
void main(point GS_INPUT input[1], inout TriangleStream stream)
{
	float2 pos = input[0].pos;
	float2 size = input[0].size;
	float2 uvBegin = input[0].uvStart;
	float2 uvEnd = input[0].uvEnd;

	GS_OUTPUT output0, output1, output2, output3;
	output0.pos = float4(pos, 1.0f, 1.0f);
	output0.uv = uvBegin;

	output1.pos = float4(pos.x + size.x, pos.y, 1.0f, 1.0f);
	output1.uv = float2(uvEnd.x, uvBegin.y);

	output2.pos = float4(pos.x, pos.y + size.y, 1.0f, 1.0f);
	output2.uv = float2(uvBegin.x, uvEnd.y);

	output3.pos = float4(pos + size, 1.0f, 1.0f);
	output3.uv = uvEnd;

	stream.Append(output0);
	stream.Append(output1);
	stream.Append(output2);
	stream.Append(output3);
	stream.RestartStrip();
}