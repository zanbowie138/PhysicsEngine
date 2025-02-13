#pragma once
#include <filesystem>
#include <fstream>
#include <iostream>
#include <queue>


#include <glm/vec3.hpp>
#include <glm/geometric.hpp>
#include <glm/gtx/hash.hpp>
#include "core/GlobalTypes.h"
#include "MeshProcessing.h"

// https://users.csc.calpoly.edu/~zwood/teaching/csc570/final06/jseeba/
namespace Utils
{
// 	inline MeshData DecimateMesh(const MeshData& inputData, size_t targetVertexCount)
// 	{
// 		const float threshold = 0;
//
// 		// Quadric for each triangle
// 		// TODO: Make more storage efficient (quadric matrix is symmetrical along diagonal)
// 		std::vector<glm::mat4> quadrics(inputData.indices.size() / 3);
//
// 		// Every two indices is a pair
// 		std::vector<size_t> pairs(inputData.indices.size() * 2);
//
// 		// Calculate quadric for each triangle
// 		// https://math.stackexchange.com/questions/2686606/equation-of-a-plane-passing-through-3-points
// 		for (size_t i = 0; i < inputData.indices.size(); i += 3)
// 		{
// 			// Add all edges of the triangle to the pairs list (triIdxOffset is defined in MeshProcessing.h)
// 			// TODO: Organize files better
// 			for (size_t j = 0; j < 6; j++)
// 				pairs[i * 2 + j] = inputData.indices[i + triIdxOffset[j]];
//
// 			// Every 3 indices is a triangle
// 			glm::vec3 v1 = inputData.vertices[inputData.indices[i + 0]].position;
// 			glm::vec3 v2 = inputData.vertices[inputData.indices[i + 1]].position;
// 			glm::vec3 v3 = inputData.vertices[inputData.indices[i + 2]].position;
//
// 			// Get two edges of the triangle
// 			glm::vec3 edge1 = v2 - v1;
// 			glm::vec3 edge2 = v3 - v1;
//
// 			// Calculate triangle normal
// 			glm::vec3 normal = glm::normalize(glm::cross(edge1, edge2));
//
// 			// Calculate offset or "d" in plane equation using one of the vertices
// 			float offset = -glm::dot(v1, normal);
//
// 			// Plane equation format is ax + by + cz + d where a^2 + b^2 + c^2 = 1
// 			auto a = normal.x;
// 			auto b = normal.y;
// 			auto c = normal.z;
// 			auto d = offset;
//
// 			// Based on plane equation, calculate quadric matrix for triangle
// 			quadrics[i / 3] = glm::mat4
// 			{
// 				a * a, a * b, a * c, a * d,
// 				a * b, b * b, b * c, b * d,
// 				a * c, b * c, c * c, c * d,
// 				a * d, b * d, c * d, d * d
// 			};
// 		}
//
// 		// Accumulate quadric for every vertice of all the quadrics of the triangles it is a part of
// 		std::vector<glm::mat4> verticeQuadrics(inputData.vertices.size(), glm::mat4(0));
// 		for (size_t i = 0; i < inputData.indices.size(); ++i)
// 		{
// 			// Add current triangle's quadric to each vertex's quadric accumulation
// 			verticeQuadrics[inputData.indices[i]] += quadrics[i / 3];
// 		}
//
// 		// Calculate error for each pair of vertices
// 		typedef std::pair<float, size_t> errorPair;
// 		std::priority_queue <errorPair, std::vector<errorPair>, std::greater<>> errors;
// 		std::vector<glm::vec3> contractionPoints;
// 		for (size_t i = 0; i < pairs.size(); i += 2)
// 		{
// 			glm::mat4 q = verticeQuadrics[pairs[i]];
//
// 			glm::mat4 temp =
// 			{
// 				q[0][0], q[0][1], q[0][2], q[0][3],
// 				q[1][0], q[1][1], q[1][2], q[1][3],
// 				q[2][0], q[2][1], q[2][2], q[2][3],
// 				0, 0, 0, 1
// 			};
//
// 			//assert(glm::determinant(temp) != 0);
//
// 			glm::vec3 contractionPoint = glm::vec4(0, 0, 0, 1) * glm::inverse(temp);
// 			contractionPoints.push_back(contractionPoint);
//
// 			float error = glm::dot(glm::vec4(contractionPoint, 1.0f) * q, glm::vec4(contractionPoint, 1.0f)); // TODO: check algorithm
// 			errors.push(std::make_pair(error, i));
// 		}
//
// 		std::cout << errors.top().first << std::endl;
//
// 		auto outputData = inputData;
//
// 		// Remove vertices until target vertex count is reached
// 		for (size_t i = 0; i < inputData.vertices.size() - targetVertexCount; ++i)
// 		{
// 			// Get the pair with the lowest error
// 			auto pair = errors.top();
// 			errors.pop();
//
// 			// Get the two vertices of the pair
// 			auto v1 = inputData.vertices[pairs[pair.second]];
// 			auto v2 = inputData.vertices[pairs[pair.second + 1]];
//
// 			// Calculate the contraction point
// 			glm::vec3 contractionPoint = contractionPoints[pair.second / 2];
//
// 			// Calculate the new vertex
// 			MeshPt newVertex;
// 			newVertex.position = contractionPoint;
// 			newVertex.normal = glm::normalize(v1.normal + v2.normal);
//
// 			// Add the new vertex to the mesh
// 			outputData.vertices.push_back(newVertex);
//
// 			// Update the indices
// 			for (size_t j = 0; j < outputData.indices.size(); ++j)
// 			{
// 				if (outputData.indices[j] == pairs[pair.second + 1])
// 					outputData.indices[j] = outputData.vertices.size() - 1;
// 			}
//
// 			// Update the quadrics
// 			verticeQuadrics.push_back(verticeQuadrics[pairs[pair.second]] + verticeQuadrics[pairs[pair.second + 1]]);
//
// 			// Update the pairs
// 			for (size_t j = 0; j < pairs.size(); j += 2)
// 			{
// 				if (pairs[j] == pairs[pair.second + 1])
// 					pairs[j] = inputData.vertices.size() - 1;
// 			}
//
// 			// Update the errors
// 			for (size_t j = 0; j < pairs.size(); j += 2)
// 			{
// 				glm::mat4 q = verticeQuadrics[pairs[j]];
//
// 				glm::mat4 temp =
// 				{
// 					q[0][0], q[0][1], q[0][2], q[0][3],
// 					q[1][0], q[1][1], q[1][2], q[1][3],
// 					q[2][0], q[2][1], q[2][2], q[2][3],
// 					0, 0, 0, 1
// 				};
//
// 				glm::vec3 contractionPoint = glm::vec4(0, 0, 0, 1) * glm::inverse(temp);
// 			}
//
//
//
// 			/*glm::mat4 q = verticeQuadrics[pairs[errors.top().second]] + verticeQuadrics[pairs[errors.top().second + 1]];
// 			glm::mat4 temp =
// 			{
// 				q[0][0], q[0][1], q[0][2], q[0][3],
// 				q[1][0], q[1][1], q[1][2], q[1][3],
// 				q[2][0], q[2][1], q[2][2], q[2][3],
// 				0, 0, 0, 1
// 			};
//
// 			glm::vec3 v = glm::vec4(0, 0, 0, 1) * glm::inverse(temp);
//
// 			std::cout << glm::to_string(v) << std::endl;*/
// 		}
// 	}
}