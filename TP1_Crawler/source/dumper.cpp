#include "dumper.hpp"

using namespace std;

Dumper::Dumper(const char *folder_path, const char *file_name, const size_t &buffer_size){
	/* Opening File */
	sprintf(this->file_path, "%s/%s", folder_path, file_name);
	file = fopen(file_path, "w");
	if(file == NULL){
		fprintf(stderr, "[Dumper] Error opening file '%s' !", this->file_path);
		delete this;
	}

	/*this->folder_path = folder_path;
	this->file_name = file_name;*/
	this->buffer_size = buffer_size;

	//this->logger = logger;
}

void Dumper::write(const char *data){
	this->buffer.push(string(data));

	/* Write on file when buffer is full */
	if(this->buffer.size() >= this->buffer_size){
		while(!this->buffer.empty()){
			fprintf(this->file, "%s", this->buffer.front().c_str());
			this->buffer.pop();
		}
	}
}

void Dumper::close(){
	fprintf(stderr, "[Dumper] Closing Dumper of file '%s' !", this->file_path);

	/* Dumping remaining lines */ 
	while(!this->buffer.empty()){
		fprintf(this->file, "%s", this->buffer.front().c_str());
		this->buffer.pop();
	}

	/* Closing file */
	if(this->file != NULL)
		fclose(this->file);
}