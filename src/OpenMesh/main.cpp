#include <iostream>
#include <vector>
// -------------------- OpenMesh
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

typedef OpenMesh::TriMesh_ArrayKernelT<>  MyMesh;

#pragma comment(lib, "OpenMeshCored")

int main(int argc, char **argv)
{
	MyMesh  mesh;


	// check command line options
	if (argc != 8){
		std::cerr << "Usage:  " << argv[0] << " #infile outfile (t/r/s) x y z w\n";
		return 1;
	}


	// read mesh from stdin
	if (!OpenMesh::IO::read_mesh(mesh, argv[1])) {
		std::cerr << "Error: Cannot read mesh from " << argv[1] << std::endl;
		return 1;
	}


	// this vector stores the computed centers of gravity
	std::vector<MyMesh::Point>  cogs;
	std::vector<MyMesh::Point>::iterator cog_it;
	cogs.reserve(mesh.n_vertices());
	std::string ct = argv[3];
	glm::vec4 val = glm::vec4(atof(argv[4]), atof(argv[5]),atof(argv[6]), atof(argv[7]) );

	// smoothing mesh argv[1] times
	MyMesh::VertexIter          v_it, v_end(mesh.vertices_end());
	MyMesh::VertexVertexIter    vv_it;
	MyMesh::Point               cog;
	cogs.clear();
	glm::mat4 trans = glm::mat4(1.0f);

	if (ct == "t") {
		trans = glm::translate(glm::mat4(1.0f), glm::vec3(val));
	}
	else if (ct == "s") {
		trans= glm::scale(glm::mat4(1.0f), glm::vec3(val)) ;
	}
	else if (ct == "r") {
		trans= glm::rotate(glm::mat4(1.0f), val.w, glm::vec3(val));
	}

	for (v_it=mesh.vertices_begin(); v_it != v_end; ++v_it) {
		cog = mesh.point(*v_it);
		glm::vec4 vex = glm::vec4(cog[0], cog[1], cog[2], 1.0f);
		vex = trans * vex;
		cog[0] = vex.x; cog[1] = vex.y; cog[2] = vex.z;
		cogs.push_back(cog);
	}

	for (v_it=mesh.vertices_begin(), cog_it=cogs.begin(); v_it != v_end; ++v_it, ++cog_it)
	    mesh.set_point(*v_it, *cog_it);


	// write mesh to stdout
	if (!OpenMesh::IO::write_mesh(mesh, argv[2]))
	{
		std::cerr << "Error: cannot write mesh to " << argv[2] << std::endl;
		return 1;
	}
	std::cin.get();
	return 0;
}
