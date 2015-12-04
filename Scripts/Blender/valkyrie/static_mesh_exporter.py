
import mathutils
import valkyrie.mesh
import bpy



def test():
	
	mesh_data_for_export = { }
	
	mesh_data_for_export["MyObject"] = 8
	
	
	for obj in bpy.context.scene.objects:
		print ("Object: ", obj)
		
		data = obj.data
		if isinstance(data, bpy.types.Mesh):
			mesh = valkyrie.Mesh()
			mesh.setup_from_blender_mesh(data)
			
			matrix = obj.matrix_world
			
			another_mesh0 = valkyrie.MeshData(False, 0)
			another_mesh0.merge_mesh (mesh.mesh_datas[0], matrix)
			another_mesh1 = valkyrie.MeshData(False, 1)
			another_mesh1.merge_mesh (mesh.mesh_datas[1], matrix)
			
			another_mesh = valkyrie.Mesh()
			another_mesh.mesh_datas.append(another_mesh0)
			another_mesh.mesh_datas.append(another_mesh1)
			
			another_mesh.debug()
		
	
