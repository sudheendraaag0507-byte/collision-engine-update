#define _CRT_SECURE_NO_WARNINGS
#include "../include/Recorder.h"
#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdio.h>
#include <string.h>

using namespace std;

int color = 4;
char* spec = nullptr;
int Width;
int Height;
int previous = -1;
unsigned int current = 0;
FILE* ffmpeg = nullptr;

void isRecord(int choice) {
	if (choice == 1) {
		ffmpeg = _popen(spec,"wb");
		if (!ffmpeg) {
			cout << "Could not open FFmpeg pipe!" << endl;
			return ;
		}
	}
	else if(choice == 0 && spec != nullptr ) {
		_pclose(ffmpeg);
		delete[] spec;
		spec = nullptr;
	}
	else cout << "invalid argument"<<endl;
}
void screenRec(int width, int height, int fps, const char* name,unsigned int*PBO) {
	
	// creating a buffer for storing pixels data in it 
	glGenBuffers(2, PBO);
	// Allocate memory for both PBOs
	for (int i = 0; i < 2; i++) {
		glBindBuffer(GL_PIXEL_PACK_BUFFER, PBO[i]);
		// Width * Height * 4 (RGBA bytes)
		glBufferData(GL_PIXEL_PACK_BUFFER, width * height * 4, NULL, GL_STREAM_READ);
	}
	glBindBuffer(GL_PIXEL_PACK_BUFFER, 0); // Unbind for safety
	Width = width;
	Height = height;
	spec = new char [512];
	sprintf(spec,"ffmpeg -y -f rawvideo -s %dx%d -pix_fmt rgba -r %d -i - -vf vflip -c:v libx264 -preset fast -pix_fmt yuv420p %s.mp4",width,height,fps,name);
	
}
void ifRecord(int option,unsigned int*PBO) {
	if(!ffmpeg) return;
	if (option == 1) {
		if(previous++ >= 0){
			previous = (previous++)%2;
			glBindBuffer(GL_PIXEL_PACK_BUFFER, PBO[previous]);

			void*key = glMapBuffer(GL_PIXEL_PACK_BUFFER,GL_READ_ONLY);
			// see always glmap will return void pointer so catching it with the void type pointer is wise
			fwrite(key, 4, Width * Height, ffmpeg);
			glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
		}
	}
}
void bufferReader(unsigned int*PBO) {
	glBindBuffer(GL_PIXEL_PACK_BUFFER, PBO[current]);
	// here i provide the id of the buffer 
	glReadPixels(0, 0, Width, Height, GL_RGBA, GL_UNSIGNED_BYTE, (void*)0);
	// here am writting (void*) coz i am not trying to update any buffer
	current = (++current) % 2;
}