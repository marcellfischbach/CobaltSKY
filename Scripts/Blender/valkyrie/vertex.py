
import valkyrie

class Vertex:

		def __init___(self):
				self.x = 0
				self.y = 0
				self.z = 0
				
				self.nx = 0
				self.ny = 0
				self.nz = 0
				
				
				self.tu = 0
				self.tv = 0
				
		def equals(self, other):
			return self.x == other.x and self.y == other.y and self.z == other.z and self.nx == other.nx and self.ny == other.ny and self.nz == other.nz and self.tu == other.tu and self.tv == other.tv
		
	
from valkyrie.vertex import Vertex as vkVertex
