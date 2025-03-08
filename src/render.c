#include "render.h"

SDL_GPUBuffer *vertex_buffer;
SDL_GPUGraphicsPipeline *graphics_pipeline;

SDL_GPUShader *load_shader(const char *name, SDL_GPUShaderStage stage, uint32_t num_samplers, uint32_t num_storage_textures, uint32_t num_storage_buffers, uint32_t num_uniform_buffers)
{
	char full_path[256];
	const char *base_path = SDL_GetBasePath();
	const char *entrypoint;
	SDL_GPUShaderFormat device_formats = SDL_GetGPUShaderFormats(device);
	SDL_GPUShaderFormat format;

	if (device_formats & SDL_GPU_SHADERFORMAT_SPIRV)
	{
		SDL_snprintf(full_path, sizeof(full_path), "%s/../shaders/spirv/%s.spv", base_path, name);
		entrypoint = "main";
		format = SDL_GPU_SHADERFORMAT_SPIRV;
	} else if (device_formats & SDL_GPU_SHADERFORMAT_DXIL)
	{
		SDL_snprintf(full_path, sizeof(full_path), "%s/../shaders/dxil/%s.dxil", base_path, name);
		entrypoint = "main";
		format = SDL_GPU_SHADERFORMAT_DXIL;
	} else if (device_formats & SDL_GPU_SHADERFORMAT_MSL)
	{
		SDL_snprintf(full_path, sizeof(full_path), "%s/../shaders/msl/%s.msl", base_path, name);
		entrypoint = "main0";
		format = SDL_GPU_SHADERFORMAT_MSL;
	} else 
	{
		SDL_Log("failed to find compatible gpu shader format");
		return NULL;
	}

	size_t code_size;
	void *code = SDL_LoadFile(full_path, &code_size);

	if (code == NULL)
	{
		SDL_Log("LoadFile failed: %s", SDL_GetError());
	}
	
	SDL_GPUShaderCreateInfo shader_create_info = 
	{
		.code_size = code_size,
		.code = code,
		.entrypoint = entrypoint,
		.format = format,
		.stage = stage,
		.num_samplers = num_samplers,
		.num_storage_textures = num_storage_textures,
		.num_storage_buffers = num_storage_buffers,
		.num_uniform_buffers = num_uniform_buffers
	};

	SDL_GPUShader *shader = SDL_CreateGPUShader(device, &shader_create_info);

	if (shader == NULL)
	{
		SDL_Log("CreateGPUShader failed: %s", SDL_GetError());
		SDL_free(code);
		return NULL;
	}
	
	SDL_free(code);

	return shader;
}

/*
init:
	- create gpu shaders
	- create graphics pipeline
*/

bool render_init()
{
	// create gpu shaders

	SDL_GPUShader *vertex_shader = load_shader("vert", SDL_GPU_SHADERSTAGE_VERTEX, 0, 0, 0, 0);
	SDL_GPUShader *fragment_shader = load_shader("frag", SDL_GPU_SHADERSTAGE_FRAGMENT, 0, 0, 0, 0);
	
	if(vertex_shader == NULL || fragment_shader == NULL)
	{
		return false;
	}

	// create graphics pipeline

	SDL_GPUVertexBufferDescription vertex_buffer_descriptions[1] = {{ .slot = 0, .input_rate = SDL_GPU_VERTEXINPUTRATE_VERTEX, .instance_step_rate = 0, .pitch = sizeof(vertex) }};

	uint32_t num_vertex_buffers = sizeof(vertex_buffer_descriptions) / sizeof(SDL_GPUVertexBufferDescription);

	SDL_GPUVertexAttribute vertex_attributes[2] = {{ .buffer_slot = 0, .format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3, .location = 0, .offset = 0 }, { .buffer_slot = 0, .format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT4, .location = 1, .offset = sizeof(vec3) }};

	uint32_t num_vertex_attributes = sizeof(vertex_attributes) / sizeof(SDL_GPUVertexAttribute);
	
	SDL_GPUVertexInputState vertex_input_state = { .vertex_buffer_descriptions = vertex_buffer_descriptions, .num_vertex_buffers = num_vertex_buffers, .vertex_attributes = vertex_attributes, .num_vertex_attributes = num_vertex_attributes };

	SDL_GPUColorTargetDescription color_target_descriptions[1] = {{ .format = SDL_GetGPUSwapchainTextureFormat(device, window) }};
	
	uint32_t num_color_targets = sizeof(color_target_descriptions) / sizeof(SDL_GPUColorTargetDescription);

	SDL_GPUGraphicsPipelineTargetInfo target_info = { .color_target_descriptions = color_target_descriptions, .num_color_targets = num_color_targets };

	SDL_GPUGraphicsPipelineCreateInfo graphics_pipeline_createinfo = { .vertex_shader = vertex_shader, .fragment_shader = fragment_shader, .vertex_input_state = vertex_input_state, .primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST, .target_info = target_info };

	graphics_pipeline = SDL_CreateGPUGraphicsPipeline(device, &graphics_pipeline_createinfo);

	if(graphics_pipeline == NULL)
	{
		SDL_Log("CreateGPUGraphicsPipeline failed: %s", SDL_GetError());
		return false;
	}

	return true;
}

/*
upload:
	- create gpu buffer
	- create gpu transfer buffer
	- copy vertices into tranfer buffer
	- acquire command buffer
	- begin copy pass

	- upload to gpu buffer

	- end copy pass
	- submit command buffer
	- release transfer buffer
*/

bool render_upload(vertex vertices[], size_t num_vertices)
{
	vertex_buffer = SDL_CreateGPUBuffer(device, &(SDL_GPUBufferCreateInfo){ .usage = SDL_GPU_BUFFERUSAGE_VERTEX, .size = num_vertices * sizeof(vertex) });

	if (vertex_buffer == NULL)
	{
		SDL_Log("CreateGPUBuffer failed: %s", SDL_GetError());
		return false;
	}

	SDL_GPUTransferBufferCreateInfo transfer_buffer_createinfo = { .usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD, .size = num_vertices * sizeof(vertex) };

	SDL_GPUTransferBuffer *transfer_buffer = SDL_CreateGPUTransferBuffer(device, &transfer_buffer_createinfo);

	if(transfer_buffer == NULL)
	{
		SDL_Log("CreateGPUTransferBuffer failed: %s", SDL_GetError());
		return false;
	}

	vertex *transfer_vertices = SDL_MapGPUTransferBuffer(device, transfer_buffer, false);

	if(transfer_vertices == NULL)
	{
		SDL_Log("MapGPUTransferBuffer failed: %s", SDL_GetError());
		return false;
	}

	memcpy(transfer_vertices, vertices, num_vertices * sizeof(vertex));

	SDL_UnmapGPUTransferBuffer(device, transfer_buffer);

	SDL_GPUCommandBuffer *upload_command_buffer = SDL_AcquireGPUCommandBuffer(device);

	if (upload_command_buffer == NULL)
	{
		SDL_Log("AcquiteGPUCommandBuffer failed: %s", SDL_GetError());
		return false;
	}

	SDL_GPUCopyPass *copy_pass = SDL_BeginGPUCopyPass(upload_command_buffer);
	
	SDL_UploadToGPUBuffer(copy_pass, &(SDL_GPUTransferBufferLocation){ .transfer_buffer = transfer_buffer, .offset = 0 }, &(SDL_GPUBufferRegion){ .buffer = vertex_buffer, .offset = 0, .size = num_vertices * sizeof(vertex) }, false);
	
	SDL_EndGPUCopyPass(copy_pass);

	if(!SDL_SubmitGPUCommandBuffer(upload_command_buffer))
	{
		SDL_Log("SubmitGPUCommandBuffer failed: %s", SDL_GetError());
		return false;
	}

	SDL_ReleaseGPUTransferBuffer(device, transfer_buffer);

	return true;
}

/*
draw:
	- aquire gpu command buffer
	- wait and acquire gpu swapchain texture
	- begin gpu render pass

	- bind gpu graphics pipeline
	- bind gpu vertex buffer
	
	- draw gpu primitives

	- end gpu render pass
	- submit gpu command buffer
*/

bool render_draw(size_t num_vertices)
{
	SDL_GPUCommandBuffer *command_buffer = SDL_AcquireGPUCommandBuffer(device);

	if (command_buffer == NULL)
	{
		SDL_Log("AcquireGPUCommandBuffer failed: %s", SDL_GetError());
		return false;
	}

	SDL_GPUTexture *swapchain_texture;

	if (!SDL_WaitAndAcquireGPUSwapchainTexture(command_buffer, window, &swapchain_texture, NULL, NULL))
	{
		SDL_Log("WaitAndAcquireGPUSwapchainTexture failed: %s", SDL_GetError());
		return false;
	}
	
	SDL_GPUColorTargetInfo color_target_infos[1] = {{ .texture = swapchain_texture, .clear_color = (SDL_FColor){ 0.0f, 0.0f, 0.0f, 1.0f }, .load_op = SDL_GPU_LOADOP_CLEAR, .store_op = SDL_GPU_STOREOP_STORE }};

	uint32_t num_color_targets = sizeof(color_target_infos) / sizeof(SDL_GPUColorTargetInfo);

	SDL_GPURenderPass *render_pass = SDL_BeginGPURenderPass(command_buffer, color_target_infos, num_color_targets, NULL);

	SDL_GPUBufferBinding bindings[1] = {{ .buffer = vertex_buffer, .offset = 0 }};

	uint32_t num_bindings = sizeof(bindings) / sizeof(SDL_GPUBufferBinding);

	SDL_BindGPUGraphicsPipeline(render_pass, graphics_pipeline);
	SDL_BindGPUVertexBuffers(render_pass, 0, bindings, num_bindings);

	SDL_DrawGPUPrimitives(render_pass, num_vertices, 1, 0, 0); 

	SDL_EndGPURenderPass(render_pass);

	if(!SDL_SubmitGPUCommandBuffer(command_buffer))
	{
		SDL_Log("SubmitGPUCommandBuffer failed: %s", SDL_GetError());
		return false;
	}

	return true;
}

void render_quit()
{
	SDL_ReleaseGPUGraphicsPipeline(device, graphics_pipeline);
	SDL_ReleaseGPUBuffer(device, vertex_buffer);
}
