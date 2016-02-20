#version 330

in vec4 vk_BoneIndex;
in vec4 vk_BoneWeight;

uniform mat4 vk_MatsSkeleton[64];


vec4 vk_bone_to_model4f(vec4 position)
{
	vec4 v = vk_MatsSkeleton[int(vk_BoneIndex.x)] * position * vk_BoneWeight.x
			 	 + vk_MatsSkeleton[int(vk_BoneIndex.y)] * position * vk_BoneWeight.y
	       + vk_MatsSkeleton[int(vk_BoneIndex.z)] * position * vk_BoneWeight.z
	       + vk_MatsSkeleton[int(vk_BoneIndex.w)] * position * vk_BoneWeight.w;
	//v = vk_MatsSkeleton[vk_BoneIndex.x] * position;
	return v;
}


vec3 vk_bone_to_model3f(vec3 position)
{
	return vk_bone_to_model4f(vec4(position, 0.0)).xyz;
}

