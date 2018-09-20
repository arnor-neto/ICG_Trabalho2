#include <vector>
#include<iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <bits/stdc++.h>

std::vector<glm::vec4> loadModel(const std::string &file_name ) {
			std::ifstream fin(file_name.c_str() );
			std::vector<glm::vec4> vertices(0);

			if( !fin.fail() )
				fin.close();
			else {
				std::cerr << "Couldn't open file: " << file_name << std::endl;
				return vertices;
			}

			Assimp::Importer assimp_importer;
			const aiScene* assimp_scene_ = assimp_importer.ReadFile( file_name, aiProcess_Triangulate);
			
			if( !assimp_scene_ ) {
				std::cerr << assimp_importer.GetErrorString() << std::endl;
				return vertices;
			}

			if( assimp_scene_->HasMeshes() ) {
				
				for( unsigned int mesh_id = 0; mesh_id < assimp_scene_->mNumMeshes; mesh_id++ ) {
					const aiMesh *mesh_ptr = assimp_scene_->mMeshes[mesh_id];

					for( unsigned int vertex_id = 0; vertex_id < mesh_ptr->mNumVertices; vertex_id += 3 ) {
						const aiVector3D *vertex_ptr = &mesh_ptr->mVertices[vertex_id];
					
						glm::vec4 v0( vertex_ptr[0].x, vertex_ptr[0].y, vertex_ptr[0].z,1 );
						glm::vec4 v1( vertex_ptr[1].x, vertex_ptr[1].y, vertex_ptr[1].z,1 );
						glm::vec4 v2( vertex_ptr[2].x, vertex_ptr[2].y, vertex_ptr[2].z,1 );
						
						vertices.push_back(v0);
						vertices.push_back(v1);
						vertices.push_back(v2);
					}
				
				}

			}

			return vertices;
}