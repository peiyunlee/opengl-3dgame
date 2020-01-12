#include "ObjLoader.h"

bool LoadObj(const char*  path, std::vector<float> *vertices, std::vector<float> *normals, std::vector<float> *uvs, char type) {
	char lineHeader[1000];
	std::vector<float> temp_vertices;
	std::vector<float> temp_normals;
	std::vector<float> temp_uvs;
	std::vector<unsigned int> indices;
	float vertex[3], normal[3],uv[3];
	unsigned int index[3][3], f = 0;
	FILE* objfile = fopen(path, "r");
	
	if (objfile == NULL) {
		printf("Impossible to open the obj file !\n");
		return false;
	}

	while (fscanf(objfile, "%s", lineHeader) != EOF) {
		if (strcmp(lineHeader, "v") == 0) {
			fscanf(objfile, "%f %f %f\n", &vertex[0], &vertex[1], &vertex[2]);
			temp_vertices.push_back(vertex[0]);
			temp_vertices.push_back(vertex[1]);
			temp_vertices.push_back(vertex[2]);
		}
		else if (strcmp(lineHeader, "vt") == 0) {
			fscanf(objfile, "%f %f %f\n", &uv[0], &uv[1], &uv[2]);
			temp_uvs.push_back(uv[0]);
			temp_uvs.push_back(uv[1]);
			temp_uvs.push_back(uv[2]);
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			fscanf(objfile, "%f %f %f\n", &normal[0], &normal[1], &normal[2]);
			temp_normals.push_back(normal[0]);
			temp_normals.push_back(normal[1]);
			temp_normals.push_back(normal[2]);
		}
		else if (strcmp(lineHeader, "f") == 0) {
			if (type == 'M') {
				int matches = fscanf(objfile, "%u/%u/%u %u/%u/%u %u/%u/%u", &index[0][0], &index[1][0], &index[2][0], &index[0][1], &index[1][1], &index[2][1], &index[0][2], &index[1][2], &index[2][2]);
				//printf("matches=%d,f=%u\n", matches, f++);
				if (matches != 9) {
					printf("Type of file is different !\n");
					return false;
				}
			}
			else if(type == 'B') {
				int matches = fscanf(objfile, "%u//%u %u//%u %u//%u", &index[0][0], &index[2][0], &index[0][1], &index[2][1], &index[0][2], &index[2][2]);
				//printf("matches=%d,f=%u\n", matches, f++);
				if (matches != 6) {
					printf("Type of file is different !\n");
					return false;
				}
			}
			else if (type == 'C') {
				int matches = fscanf(objfile, "%u/%u %u/%u %u/%u", &index[0][0], &index[2][0], &index[0][1], &index[2][1], &index[0][2], &index[2][2]);
				//printf("matches=%d,f=%u\n", matches, f++);
				if (matches != 6) {
					printf("Type of file is different !\n");
					return false;
				}
			}
			
			indices.push_back(index[0][0]);
			indices.push_back(index[1][0]);
			indices.push_back(index[2][0]);
			indices.push_back(index[0][1]);
			indices.push_back(index[1][1]);
			indices.push_back(index[2][1]);
			indices.push_back(index[0][2]);
			indices.push_back(index[1][2]);
			indices.push_back(index[2][2]);
		}
	}
	for (unsigned int i = 0; i < indices.size(); i += 3) {
		vertices->push_back(temp_vertices[(indices[i] - 1) * 3]);
		vertices->push_back(temp_vertices[(indices[i] - 1) * 3 + 1]);
		vertices->push_back(temp_vertices[(indices[i] - 1) * 3 + 2]);
		uvs->push_back(temp_uvs[(indices[i + 1] - 1) * 3]);
		uvs->push_back(temp_uvs[(indices[i + 1] - 1) * 3 + 1]);
		uvs->push_back(temp_uvs[(indices[i + 1] - 1) * 3 + 2]);
		normals->push_back(temp_normals[(indices[i + 2] - 1) * 3]);
		normals->push_back(temp_normals[(indices[i + 2] - 1) * 3 + 1]);
		normals->push_back(temp_normals[(indices[i + 2] - 1) * 3 + 2]);
	}

	fclose(objfile);
}