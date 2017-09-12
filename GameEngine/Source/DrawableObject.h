#pragma once

#include "../JuceLibraryCode/JuceHeader.h";

class DrawableObject {
public:

	DrawableObject() {
		vertices.add(Vector3D<GLfloat>(1.0f, 1.0f, 0.0f));
		vertices.add(Vector3D<GLfloat>(1.0f, -1.0f, 0.0f));
		vertices.add(Vector3D<GLfloat>(-1.0f, -1.0f, 0.0f));
		vertices.add(Vector3D<GLfloat>(-1.0f, 1.0f, 0.0f));

	}

	~DrawableObject() {

	}
	Array<GLfloat> getVertices() {
		Array<GLfloat> glVertices;
		for (auto &vertex : vertices) {
			glVertices.add(vertex.x);

			glVertices.add(vertex.y);
			glVertices.add(vertex.z);
		}

		return glVertices;

	}

	void translate(Vector3D<GLfloat> translate) {
		for (auto &vertex : vertices) {
			vertex += translate;
			
		}
	}

private:
	Vector3D<GLfloat> position;
	int size;
	Array<Vector3D<GLfloat> > vertices;

};