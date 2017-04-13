#include "dumper.hpp"
#include <iostream>

using namespace std;

Dumper::Dumper(const char *folder_path, const char *file_name, const size_t buffer_size){
	/* Opening File */
	sprintf(this->file_path, "%s/%s", folder_path, file_name);
	if(this->file_buffer.open(this->file_path, ios::out) == NULL){
		fprintf(stderr, "[Dumper] Error opening file '%s' !", this->file_path);
		delete this;
	}
	else{
		this->buffer_size = buffer_size;
		this->buffer_count = 0;
		this->output_stream = new ostream(&(this->file_buffer));
	}	
}

Dumper::~Dumper(){
	delete this->output_stream;
}

void Dumper::write(const char *data){
	string data_str(data);
	
	/* Write on file when buffer is full */
	if(this->buffer_count + data_str.size() > this->buffer_size){
		this->output_stream->flush();
		cout << "\nDump uma vez";
	}

	(*this->output_stream) << data_str;
	this->buffer_count += data_str.size();
}

void Dumper::close(){
	fprintf(stderr, "[Dumper] Closing Dumper of file '%s' !", this->file_path);

	/* Dumping remaining data */ 
	if(this->buffer_count >0)
		this->output_stream->flush();

	/* Closing file */
	if(this->file_buffer.is_open())
		this->file_buffer.close();
}