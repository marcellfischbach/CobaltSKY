
import valkyrie.mesh
import bpy

def test():
	
	for obj in bpy.context.scene.objects:
		print ("Object: ", obj)
		
		data = obj.data
		if isinstance(data, bpy.types.Mesh):
			mesh = valkyrie.Mesh()
			mesh.setup_from_blender_mesh(data)
			mesh.debug()
		
	
