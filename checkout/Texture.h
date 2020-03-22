#pragma once
#include"libs.h"

class Texture {
private:
	GLuint texture_id;
	int width;
	int height;
	GLint textureUnit;
	GLenum textureType;
	unsigned char* loadImage(const char* filename) {
		unsigned char*ans_img=SOIL_load_image(filename, &this->width, &this->height, NULL, SOIL_LOAD_RGBA);
		return ans_img;
	}
public:
	Texture(const char* filename,GLenum type,GLint texture_unit) {
		this->textureUnit = texture_unit;
		this->textureType = type;
		unsigned char* image = loadImage(filename);

		glGenTextures(1, &texture_id);
		glBindTexture(type, texture_id);

		glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		if (image) {
			glTexImage2D(type, 0, GL_RGBA, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
			glGenerateMipmap(type);
		}
		else {
			std::cout << "ERROR::TEXTURE LOADING FAILED" << filename<< "\n";
		}
		unbind();
		SOIL_free_image_data(image);

	}
	~Texture() {
		glDeleteTextures(1, &this->texture_id);
	}
	void unbind() {
		glActiveTexture(0);
		glBindTexture(this->textureType, 0);
	}
	GLuint getID() const {
		return this->texture_id;
	}
	void bind() {
		glActiveTexture(GL_TEXTURE0 + this->textureUnit);
		glBindTexture(this->textureType, this->texture_id);
	}
	GLint getTextureUnit() { return this->textureUnit; }
};	

