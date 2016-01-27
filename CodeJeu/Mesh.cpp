// --------------------------------------------------------------------------
// Copyright(C) 2009-2015
// Tamy Boubekeur
//
// All rights reserved.
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License (http://www.gnu.org/licenses/gpl.txt)
// for more details.
// --------------------------------------------------------------------------

#include "Mesh.h"
#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std;

void Mesh::loadOBJ{
	FILE* fp = fopen(sFileName.c_str(), "rt");

	if(fp == NULL)return false;

	char line[255];

	vector<glm::vec3> vVertices;
	vector<glm::vec2> vTexCoords;
	vector<glm::vec3> vNormals;

	iNumFaces = 0;

	while(fgets(line, 255, fp))
	{
		// Error flag, that can be set when something is inconsistent throughout
		// data parsing
		bool bError = false;

		// If it's an empty line, then skip
		if(strlen(line) <= 1)
			continue;

		// Now we're going to process line
		stringstream ss(line);
		string sType;
		ss >> sType;
		// If it's a comment, skip it
		if(sType == "#")
			continue;
		// Vertex
		else if(sType == "v")
		{
			glm::vec3 vNewVertex;
			int dim = 0;
			while(dim < 3 && ss >> vNewVertex[dim])dim++;
			vVertices.push_back(vNewVertex);
			iAttrBitField |= 1;
		}
		// Texture coordinate
		else if(sType == "vt")
		{
			glm::vec2 vNewCoord;
			int dim = 0;
			while(dim < 2 && ss >> vNewCoord[dim])dim++;
			vTexCoords.push_back(vNewCoord);
			iAttrBitField |= 2;
		}
		// Normal
		else if(sType == "vn")
		{
			glm::vec3 vNewNormal;
			int dim = 0;
			while(dim < 3 && ss >> vNewNormal[dim])dim++;
			vNewNormal = glm::normalize(vNewNormal);
			vNormals.push_back(vNewNormal);
			iAttrBitField |= 4;
		}
		// Face definition
		else if(sType == "f")
		{
			string sFaceData;
			// This will run for as many vertex definitions as the face has
			// (for triangle, it's 3)
			while(ss >> sFaceData)
			{
				vector<string> data = split(sFaceData, "/");
				int iVertIndex = -1, iTexCoordIndex = -1, iNormalIndex = -1;

				// If there were some vertices defined earlier
				if(iAttrBitField&1)
				{
					if(ESZ(data[0]) > 0)sscanf(data[0].c_str(), "%d", &iVertIndex);
					else bError = true;
				}
				// If there were some texture coordinates defined earlier
				if(iAttrBitField&2 && !bError)
				{
					if(ESZ(data) >= 1)
					{
						// Just a check whether face format isn't f v//vn
						// In that case, data[1] is empty string
						if(ESZ(data[1]) > 0)sscanf(data[1].c_str(), "%d", &iTexCoordIndex);
						else bError = true;
					}
					else bError = true;
				}
				// If there were some normals defined defined earlier
				if(iAttrBitField&4 && !bError)
				{
					if(ESZ(data) >= 2)
					{
						if(ESZ(data[2]) > 0)sscanf(data[2].c_str(), "%d", &iNormalIndex);
						else bError = true;
					}
					else bError = true;
				}
				if(bError)
				{
					fclose(fp);
					return false;
				}

				// Check whether vertex index is within boundaries (indexed from 1)
				if(iVertIndex > 0 && iVertIndex <= ESZ(vVertices))
					vboModelData.addData(&vVertices[iVertIndex-1], sizeof(glm::vec3));
				if(iTexCoordIndex > 0 && iTexCoordIndex <= ESZ(vTexCoords))
					vboModelData.addData(&vTexCoords[iTexCoordIndex-1], sizeof(glm::vec2));
				if(iNormalIndex > 0 && iNormalIndex <= ESZ(vNormals))
					vboModelData.addData(&vNormals[iNormalIndex-1], sizeof(glm::vec3));
			}
			iNumFaces++;
		}
		// Shading model, for now just skip it
		else if(sType == "s")
		{
			// Do nothing for now
		}
		// Material specified, skip it again
		else if(sType == "usemtl")
		{
			// Do nothing for now
		}
	}

	fclose(fp);

	if(iAttrBitField == 0)
		return false;

	// Create VBO

	vboModelData.createVBO();
	vboModelData.bindVBO();

	vboModelData.uploadDataToGPU(GL_STATIC_DRAW);

	// Create one VAO

	glGenVertexArrays(1, &uiVAO);
	glBindVertexArray(uiVAO);
	int iDataStride = 0;
	if(iAttrBitField&1)iDataStride += sizeof(glm::vec3);
	if(iAttrBitField&2)iDataStride += sizeof(glm::vec2);
	if(iAttrBitField&4)iDataStride += sizeof(glm::vec3);

	if(iAttrBitField&1)
	{
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, iDataStride, 0);
	}
	// Texture coordinates
	if(iAttrBitField&2)
	{
		glEnableVertexAttribArray(1);
		int iDataOffset = 0;
		if(iAttrBitField&1)iDataOffset += sizeof(glm::vec3);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, iDataStride, (void*)iDataOffset);
	}
	// Normal vectors
	if(iAttrBitField&4)
	{
		glEnableVertexAttribArray(2);
		int iDataOffset = 0;
		if(iAttrBitField&1)iDataOffset += sizeof(glm::vec3);
		if(iAttrBitField&2)iDataOffset += sizeof(glm::vec2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, iDataStride, (void*)iDataOffset);
	}
	bLoaded = true;

	// Material should be in the same directory as model
	loadMaterial(getDirectoryPath(sFileName)+sMtlFileName);

	return true;
}
}

void Mesh::loadOFF (const std::string & filename) {
	ifstream in (filename.c_str ());
    if (!in)
        exit (1);
	string offString;
    unsigned int sizeV, sizeT, tmp;
    in >> offString >> sizeV >> sizeT >> tmp;
    V.resize (sizeV);
    T.resize (sizeT);
    for (unsigned int i = 0; i < sizeV; i++)
        in >> V[i].p;
    int s;
    for (unsigned int i = 0; i < sizeT; i++) {
        in >> s;
        for (unsigned int j = 0; j < 3; j++)
            in >> T[i].v[j];
    }
    in.close ();
    centerAndScaleToUnit ();
    recomputeNormals ();
}

void Mesh::recomputeNormals () {
    for (unsigned int i = 0; i < V.size (); i++)
        V[i].n = Vec3f (0.0, 0.0, 0.0);
    for (unsigned int i = 0; i < T.size (); i++) {
        Vec3f e01 = V[T[i].v[1]].p -  V[T[i].v[0]].p;
        Vec3f e02 = V[T[i].v[2]].p -  V[T[i].v[0]].p;
        Vec3f n = cross (e01, e02);
        n.normalize ();
        for (unsigned int j = 0; j < 3; j++)
            V[T[i].v[j]].n += n;
    }
    for (unsigned int i = 0; i < V.size (); i++)
        V[i].n.normalize ();
}

void Mesh::centerAndScaleToUnit () {
    Vec3f c;
    for  (unsigned int i = 0; i < V.size (); i++)
        c += V[i].p;
    c /= V.size ();
    float maxD = dist (V[0].p, c);
    for (unsigned int i = 0; i < V.size (); i++){
        float m = dist (V[i].p, c);
        if (m > maxD)
            maxD = m;
    }
    for  (unsigned int i = 0; i < V.size (); i++)
        V[i].p = (V[i].p - c) / maxD;
}
