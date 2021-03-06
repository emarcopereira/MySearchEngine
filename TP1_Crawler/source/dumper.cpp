#include "dumper.hpp"
#include <iostream>

using namespace std;

Dumper::Dumper(const char *folder_path, const char *file_name, const size_t buffer_size){
	/* Opening File */
	sprintf(this->file_path, "%s/%s", folder_path, file_name);
	/*if(this->file_buffer.open(this->file_path, ios::out) == NULL){
		fprintf(stderr, "\n[Dumper] Error opening file '%s' !", this->file_path);
		delete this;
	}*/
	if((this->dumper_file = fopen(this->file_path, "w")) == NULL){
		fprintf(stderr, "\n[Dumper] Error opening file '%s' !", this->file_path);
		delete this;
	}
	else{
		this->buffer_size = buffer_size;
		this->buffer_count = 0;
		//this->output_stream = new ostream(&(this->file_buffer));
	}	
}

Dumper::~Dumper(){
	this->close();
	
	/*if(this->output_stream != NULL)
		delete this->output_stream;*/
}

// void Dumper::write(CkString &data){
// 	(*this->output_stream) << data.getString();
// 	this->buffer_count += data.getNumChars();

// 	/* Write on file when buffer is full */
// 	if(this->buffer_count + data.getNumChars() > this->buffer_size){
// 		this->output_stream->flush();
// 		//cout << "\nDump uma vez";
// 		this->buffer_count = 0;
// 	}
// }

// void Dumper::write(string &data){
// 	(*this->output_stream) << data.c_str();
// 	this->buffer_count += data.size();

// 	/* Write on file when buffer is full */
// 	if(this->buffer_count + data.size() > this->buffer_size){
// 		this->output_stream->flush();
// 		//cout << "\nDump uma vez";
// 		this->buffer_count = 0;
// 	}
// }

void Dumper::close(){
	/* Dumping remaining data */ 
	/*if(this->buffer_count > 0)
		this->output_stream->flush();*/

	/* Closing file */
	/*if(this->file_buffer.is_open())
		this->file_buffer.close();*/
	if(this->dumper_file != NULL){
		fprintf(stderr, "\n[Dumper] Closing Dumper of file '%s' !", this->file_path);
		fclose(this->dumper_file);
	}
}