shadercross -t vertex vert.hlsl -o ../spirv/vert.spv 
shadercross -t vertex vert.hlsl -o ../msl/vert.msl
shadercross -t vertex vert.hlsl -o ../dxil/vert.dxil

shadercross -t fragment frag.hlsl -o ../spirv/frag.spv 
shadercross -t fragment frag.hlsl -o ../msl/frag.msl
shadercross -t fragment frag.hlsl -o ../dxil/frag.dxil
