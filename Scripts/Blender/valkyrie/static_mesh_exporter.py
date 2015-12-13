
import mathutils
import valkyrie.mesh
import bpy
import struct
import collections


		
class Entry:
	
	ET_Mesh = 0
	ET_CollisionShape = 1
	ET_Skeleton = 2
	ET_Geometry = 3
	
	def __init__(self):
		self.name = ""
		self.type = 0
		self.offset = 0
		self.size = 0
		self.data = []
		
	def get_size(self):
		return len(self.name) + 4 + 1 + 3 * 4
	

		
class StaticMeshExporter:
	
	def __init__(self):
		self.entries = []
	
	def export (self, filename):
		self.mesh_stream = ""
		self.file = open(filename, 'wb')
		self.file.write(struct.pack('<I', StaticMeshExporterBin.VERSION))
		
		self._prepare_headers ()
		self._export_headers ()
		self._export_entries ()
		
		self.file.close()
		
	def _prepare_headers(self):
		# header offset
		offset = 4 + 4
		for entry in self.entries:
			offset += entry.get_size()
			
		for entry in self.entries:
			entry.offset = offset
			offset += entry.size
			
	def _export_headers(self):
		self.file.write (struct.pack('<I', len(self.entries)))
		for entry in self.entries:
			self._export_string (entry.name)
			self.file.write (struct.pack('<III', entry.type, entry.offset, entry.size))

	def _export_entries (self):
		for entry in self.entries:
			self.file.write (bytes(entry.data))
		
	def prepare_meshes(self, num_lods):
		mm = valkyrie.MultiMesh()
		mm.prepare_meshes(num_lods)
		
		mmw = valkyrie.mesh.MultiMeshWriter()
		mmw.write(mm)
		
		e = Entry()
		e.name = "Mesh"
		e.type = Entry.ET_Mesh
		e.size = len(mmw.stream)
		e.data = mmw.stream

		self.entries.append(e)
		
		
	def _export_string(self, string):
		_string = bytes(string, 'latin1')
		self.file.write(struct.pack("<I%dsb" % (len(_string)), len(_string), _string, 0))
		
	