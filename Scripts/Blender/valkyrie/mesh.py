
import valkyrie


class MeshData:

	def __init__(self, material_index):
		self.material_index = material_index
		self.vertices = valkyrie.UniqueList(True)
		self.trigons = valkyrie.UniqueList(True)
	
	

class Mesh:
	
	def __init__(self):
		self.mesh_datas = []
		
	
	def debug(self):
		print("Mesh:", len(self.mesh_datas))
		for md in self.mesh_datas:
			print ("  Data: ", md.material_index)
			
			print ("    Vertices: ", len(md.vertices.list))
			print ("    Trigon: ", len(md.trigons.list))
			
	
	
	def setup_from_blender_mesh(self, mesh):
		current_md = None
		
		uv_layer = Mesh._find_active_uv_layer (mesh)
		
		for poly in mesh.polygons:
			if current_md == None or current_md.material_index != poly.material_index:
				current_md = self._get_mesh_data(poly.material_index)
			map = [[0, 1, 2], [0, 2, 3]]
			num_trigs = poly.loop_total - 2
			for i in range(num_trigs):
				i0 = self._insert_vertex_at_loop(mesh,poly.loop_start + map[i][0], poly, uv_layer, current_md) 
				i1 = self._insert_vertex_at_loop(mesh,poly.loop_start + map[i][1], poly, uv_layer, current_md) 
				i2 = self._insert_vertex_at_loop(mesh,poly.loop_start + map[i][2], poly, uv_layer, current_md) 
				
				trig = valkyrie.Trigon()
				trig.v0 = i0
				trig.v1 = i1
				trig.v2 = i2
				
				current_md.trigons.add(trig)
				

	def _get_mesh_data(self, material_index):
		for md in self.mesh_datas:
			if md.material_index == material_index:
				return md
		
		md = MeshData(material_index)
		self.mesh_datas.append(md)
		return md

			
	def _insert_vertex_at_loop(self, mesh, loop_idx, poly, uv_layer, mesh_data):
		loop = mesh.loops[loop_idx]
		vertex = mesh.vertices[loop.vertex_index]
					
		v = valkyrie.Vertex ()
		v.x = vertex.co.x
		v.y = vertex.co.y
		v.z = vertex.co.z
		
		if poly.use_smooth:
			v.nx = vertex.normal.x
			v.ny = vertex.normal.y
			v.nz = vertex.normal.z
		else:
			v.nx = poly.normal.x
			v.ny = poly.normal.y
			v.nz = poly.normal.z
		
		if uv_layer != None:
			uv_loop = uv_layer.data[loop_idx]
			v.tu = uv_loop.uv[0]
			v.tv = uv_loop.uv[1]
		else:
			v.tu = 0.0
			v.tv = 0.0
		
		idx = mesh_data.vertices.add(v)
		return idx
		

			
	def _find_active_uv_layer(mesh):
		# first find the uv_texture that's active for rendering
		for uv_texture in mesh.uv_textures:
			if uv_texture.active_render == True:
				return Mesh._find_active_uv_layer_by_name(mesh, uv_texture.name)
				
		return None

	def _find_active_uv_layer_by_name(mesh, name):
		for uv_layer in mesh.uv_layers:
			if uv_layer.name == name:
				return uv_layer
		return  None