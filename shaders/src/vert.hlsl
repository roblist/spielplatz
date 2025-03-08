struct input 
{
    float3 position : TEXCOORD0;
	float4 color : TEXCOORD1;
};

struct output
{
    float4 position : SV_Position;
	float4 color : TEXCOORD0; 
};

output main(input input)
{
    output output;
	output.position = float4(input.position, 1.0f);
	output.color = input.color;

    return output;
}
