#version 330

in vec4 cs_BoneIndex;
in vec4 cs_BoneWeight;

uniform mat4 cs_MatsSkeleton[64];


vec4 cs_bone_to_model4f(vec4 position)
{
	vec4 v = cs_MatsSkeleton[int(cs_BoneIndex.x)] * position * cs_BoneWeight.x
			 	 + cs_MatsSkeleton[int(cs_BoneIndex.y)] * position * cs_BoneWeight.y
	       + cs_MatsSkeleton[int(cs_BoneIndex.z)] * position * cs_BoneWeight.z
	       + cs_MatsSkeleton[int(cs_BoneIndex.w)] * position * cs_BoneWeight.w;
	//v = cs_MatsSkeleton[cs_BoneIndex.x] * position;
	return v;
}


vec3 cs_bone_to_model3f(vec3 position)
{
	return cs_bone_to_model4f(vec4(position, 0.0)).xyz;
}

