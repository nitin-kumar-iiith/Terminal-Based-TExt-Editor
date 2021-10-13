//command mode dispaying whatever is typed on status bar
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <termios.h>
#include <sys/ioctl.h>//checked allowed or not
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <time.h>
#include <stdarg.h>
#include <string>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <cstring>
#include <dirent.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h> 
#include <pwd.h>
#include <grp.h>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h> 
#include <pwd.h>
#include <grp.h>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h> 
#include <pwd.h>
#include <grp.h>
#include <iostream>
#include <dirent.h>
#include <cstring>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h> 
#include <pwd.h>
#include <grp.h>
#include <iostream>
#include <bits/stdc++.h>
#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
/*** Data ***/
using namespace std;
#define ABUF_INIT {NULL, 0}
#define KILO_VERSION "0.0.1"
#define KILO_TAB_STOP 8
#define CTRL_KEY(k) ((k) & 0x1f)
#define T_B 1099511627776
#define G_B 1073741824
#define M_B 1048576
#define K_B 1024
string command;
int curr_pos=0;
string home_dir;
char cwd[PATH_MAX];
bool absolute(string);
bool last_move=false;//false no movve, non < and non > move previously
//true means previous move was < or >
using namespace std;
void enterDir(int);
void initEditor();
void backspaceDir(int);
void move_right(int);
void move_left(int);
void move_home(int);
void editorRefreshScreen();
void editorProcessKeypress();
void directoryOpen(string);
void editorSetStatusMessage(const char *, ...);
bool mode=false;
void editorAppendRow(char *, size_t);
void die(const char *);
int current_pos_in;
typedef struct erow {
    int size;
    int rsize;
    char *chars;
    char *render;
}erow;
void editorUpdateRow(erow *);
vector <string>history;
struct dirent *dir_entry;
struct curr_dir_info{
  string filename;
  string filePermission;
  string fileSize;
  string filesUserID;
  string filesGroupId;
  string filelastmodified;
};
vector <curr_dir_info*> fils_info;
struct editorConfig {///////////////////////////////////////////////////
    int cx;
    erow *row;
    int cy;
    char *filename;
    int coloff;
    char statusmsg[80];
    int rowoff;
    time_t statusmsg_time;
    int screenrows;
    struct termios orig_termios;
    int rx;
    int screencols;
    int numrows;
};
struct editorConfig E;
enum editorKey {///////////////////////////////////////////////
    ARROW_LEFT = 1000,
    ARROW_RIGHT,
    ARROW_UP,
    ARROW_DOWN,
    DEL_KEY,
    HOME_KEY,
    END_KEY,
    PAGE_UP,
    PAGE_DOWN
};
/*** append buffer ***/
struct abuf {//////////////////////////////////////////////////////
    char *b;
    int len;
};
/*** Copy Files ***/
int create_dir_copy_files(string input,struct stat sb)
{
	struct stat info;
	//string input="/home/nitink/Documents/AOS/Assignmnet1/delete/hello/1/2/3/";
    	int i=0;
    	input+='/';
    	string temp="";
    	while(i<input.length()){
    		for(;input[i]!='/'&&i<input.length();i++){
    			temp+=input[i];
    		}
    		if (mkdir(temp.c_str(),0777) != 0){;}
		temp+='/';i++;	
    	}
    	return 1;
}
int main_copy_files(string source_file, string dest_directory){
	char buf;
	struct stat sb;
	struct passwd *pwd;
        struct group *grp;
	//string first_name="/home/nitink/Documents/AOS/Assignmnet1/delete/sdfsdf/asdfsadg/aew";
	string first_name=source_file;
	//this works with the assumption that parent directory of destination file exists,,even if it doesnot code will work
    	int i = source_file.length() - 1; // last character
    	string lastword;//last word is the file name
    	while (i != 0 && source_file[i]!='/')
    	{
      		--i;
    	}
    	if(i==0)lastword=source_file;//entire name is file name only, i.e. the path does not has directory name
    	else{
    		lastword = first_name.substr(i+1); // +1 to skip leading space or '/' i guess
    	}
    	string dest_file;
    	//cout << lastword << endl;
    	if(dest_directory[dest_directory.length()-1]!='/')
    		dest_file=dest_directory+'/'+lastword;
    	else if(dest_directory[dest_directory.length()-1]=='/')
    		dest_file=dest_directory+lastword;
	if(stat(first_name.c_str(), &sb) == -1) {
                exit(EXIT_FAILURE);
        }
        if(dest_directory!=""){
        	create_dir_copy_files(dest_directory,sb);
        }
	int fd_one, fd_two;
	fd_one = open(first_name.c_str(), O_RDONLY);
	if (fd_one == -1){
		printf("Error opening first_file\n");
		close(fd_one);
		return 0;
	}      	
	fd_two = open(dest_file.c_str(), O_WRONLY | O_CREAT, sb.st_mode);
	while(read(fd_one, &buf, 1)){
		write(fd_two, &buf, 1);
	}
  	if (fchown(fd_two, sb.st_uid, sb.st_gid) == -1) {
      		cout<<"chown fail\n";
  	}
	close(fd_one);
	close(fd_two);
	return 0;
}



/*** Copy Directories ***/
int create_dir(string input,struct stat sb)
{
	struct stat info;
	//string input="/home/nitink/Documents/AOS/Assignmnet1/delete/hello/1/2/3/";
    	int i=0;
    	input+='/';
    	string temp="";
    	while(i<input.length()){
    		for(;input[i]!='/'&&i<input.length();i++){
    			temp+=input[i];
    		}
    		if (mkdir(temp.c_str(),0777) != 0){;}
		temp+='/';i++;	
    	}
    	return 1;
}
int copy_files(string source,string destination,string file_name){
char cwd[PATH_MAX];
	char buf;
	struct stat sb;
	struct passwd *pwd;
        struct group *grp;
        
    	string source_file=source+'/'+file_name;
	string destination_file=destination+'/'+file_name;
	if(stat(source_file.c_str(), &sb) == -1) {
                exit(EXIT_FAILURE);
        }
        if(destination!=""){
        	create_dir(destination,sb);
        }
	int fd_one, fd_two;
	fd_one = open(source_file.c_str(), O_RDONLY);
	if (fd_one == -1){
		//printf("Error opening first_file\n");
		close(fd_one);
		return 0;
	}
	fd_two = open(destination_file.c_str(), O_WRONLY | O_CREAT, sb.st_mode);
	while(read(fd_one, &buf, 1)){
		write(fd_two, &buf, 1);
	}
	//printf("Successful copy\n");
  	if (fchown(fd_two, sb.st_uid, sb.st_gid) == -1) {
      		//cout<<"chown fail\n";
      		;
  	}
	close(fd_one);
	close(fd_two);
	return 0;
}
void copy_helper(string first_name,string second_name){
	char cwd[PATH_MAX];
	char cwd2[PATH_MAX];
	struct stat sb;
	struct stat inner;
	struct dirent *de;
	DIR *dr;
	if(stat(first_name.c_str(), &sb) < 0) {//unable to get stats of source directory or file
                //die("stat");
                //exit(EXIT_FAILURE);
                return;
        }
	if((S_ISDIR(sb.st_mode))){
		if (getcwd(cwd, sizeof(cwd)) != NULL) {//not really needed?
			//cout<<cwd<<endl;
			;
          	}
		dr = opendir(first_name.c_str());//opened source directory
		if(dr==NULL){
			 return;
		}		
		if(chdir(first_name.c_str())<0){cout<<"chdir issue";return;}//moved to source directory 
		while ((de = readdir(dr)) != NULL){
			if( strcmp(".",(de->d_name))==0 || strcmp("..",(de->d_name))==0 ){
				create_dir(second_name,sb);
				continue;
			}
			if(stat(de->d_name, &inner) == 0){//reading details about one of file/folder in present sirectory successful
				if(S_ISDIR(inner.st_mode)){//so we have picked a folder from current directory
					string copy_this=de->d_name;
					string new_source=first_name+'/'+copy_this;
					string new_destination=second_name+'/'+copy_this;
					copy_helper(new_source,new_destination);////////////////
					//chdir("..");	
				}
				else{
					string file_name=de->d_name;
					copy_files(first_name,second_name,file_name);////////////////////////////
				}
			}
			else{	
				//cout<<"unable to get file/folder stats";
				;
			}
    		}
    		//remove(first_name.c_str());
    		chdir("..");
		closedir(dr);
		
	}
	else{
		//remove(first_name.c_str());
		copy_files(first_name,second_name,"noname");//we got a file as input this should not have happened
		return;
	}	
}

int main_copy_directory(string source,string destination){
	char cwd[PATH_MAX];// give path starting with '/' but does not ends with '/'
	if (getcwd(cwd, sizeof(cwd)) != NULL) {
		//cout<<cwd<<endl;
        }
        string CWD=cwd;
        string real_source;//=CWD;
        string real_destination;//=CWD;
        if(absolute(source)==false&&source[0]!='/'){
        	real_source=CWD+'/'+source;        	
        }
        else if(absolute(source)==false&&source[0]=='/'){
        	real_source=CWD+source;
        }
        else if(absolute(source)==true){
        	real_source=source;
        }
        if(absolute(destination)==false&&destination[0]!='/'){
        	real_destination=CWD+'/'+destination;
        }
        else if(absolute(destination)==false&&destination[0]=='/'){
        	real_destination=CWD+destination;
        }
        else if(absolute(destination)==true){
        	real_destination=destination;
        }
	copy_helper(real_source,real_destination);
	return 0;
}



/*** Create Directory ***/
int main_create_dir(string dir_name, string dir_loc)
{
	struct stat info;
    	string create;
    	if(dir_loc[dir_loc.length()-1]!='/'){
    		create=dir_loc+'/'+dir_name+'/';
    	}
    	else if(dir_loc[dir_loc.length()-1]=='/'){
    		create=dir_loc+dir_name+'/';
    	}
    	int i=0;
    	string temp;
    	//cout<<create<<endl;
    	while(i<create.length()){
    		for(;create[i]!='/'&&i<create.length();i++){
    			temp+=create[i];
    		}
    		if (temp!="" &&mkdir(temp.c_str(),0777) != 0)
    		{
        		//cout << "Not created as it already exists or some error" << endl;
    		}
		temp+='/';
		i++;	
    	}
    	return 0;
}


/*** Create File ***/
int create_dir_create_file(string name){
	struct stat info;
	string input=name;
	//string input="/home/nitink/Documents/AOS/Assignmnet1/delete/hello/1/2/3/";
	///home/nitink/Documents/AOS/Assignmnet1/delete/hello/1/2/3/
    	int i=0;
    	string temp;
    	while(i<input.length()){
    		for(;input[i]!='/'&&i<input.length();i++){
    			temp+=input[i];
    		}
    		//cout<<temp<<":";
    		//if( S_ISDIR(info.st_mode) ){
    		//	printf( "%s is a directory\n", temp );
    		//}
		//else{
			//printf( "%s is no directory\n", temp );
    			if (temp!="" && mkdir(temp.c_str(),0777) != 0){
        			//cout << "Not created as it already exists or some error" << endl;
    			}
		//}
		temp+='/';i++;	
    	}
    	return 0;
}
int main_create_file(string file_name, string dir_name)
{
	
	//create_file <file_name> <destination_path>’
	//Create Directory - ‘create_dir <dir_name> <destination_path>’
	struct stat info;
	///home/nitink/Documents/AOS/Assignmnet1/delete/hello/1saedf/2sredf/3hello/
	//string input="/home/nitink/Documents/AOS/Assignmnet1/delete/hello/1awsersaedf/2aesrsredf/3asehello/txt.txt";
	//main_create_file
	///home/nitink/Documents/AOS/Assignmnet1/delete/hello/1/2/3/
    	//int i=input.length()-1;
    	//while(i){
    		//if(input[i]=='/')
    			//break;
    		//i--;
    	//}
    	//string dir=input.substr(0,i);
    	//cout<<dir;
    	string to_create;
    	string create;
    	if(dir_name[dir_name.length()-1]!='/'){
    		to_create=dir_name+'/';
    		create=dir_name+'/'+file_name;
    	}
    	else if(dir_name[dir_name.length()-1]=='/'){
    		to_create=dir_name;
    		create=dir_name+file_name;
    	}
    	create_dir_create_file(to_create);
    	fstream file;
   	file.open(create.c_str(),ios::out);
   	if(!file){
       		cout<<"Error in creating file!!!";	
   	}
  	else
   		cout<<"File created successfully.";
   	file.close();
    	return 0;
}


/*** Delete Files ***/
int main_delelte_file(string first_name){
	//string first_name="/home/nitink/Documents/AOS/Assignmnet1/delete/sdfsdf/asdfsadg/sdfsdf/aew";
	int del = remove(first_name.c_str());
   	if (!del){
      		//printf("The file is Deleted successfully:");cout<<first_name<<endl;
   	}
   	else{
   		printf("\nthe file is not Deleted:");
      		return 1;
   	}
   	return 0;
}

/*** Delete Directories ***/
void delete_file_dir(string first_name){
char cwd[PATH_MAX];
	struct stat sb;
	struct stat inner;
	struct dirent *de;
	DIR *dr;
	if(stat(first_name.c_str(), &sb) < 0) {//unable to fetch fstat
                //die("stat");
                //exit(EXIT_FAILURE);
                return;
        }
	if((S_ISDIR(sb.st_mode))){
		if (getcwd(cwd, sizeof(cwd)) != NULL) {
			
          	}
		dr = opendir(first_name.c_str());
		if(dr==NULL){
			 //cout<<"unable to open";
			 return;
		}
		
		if(chdir(first_name.c_str())<0)return;
		while ((de = readdir(dr)) != NULL){
			if( strcmp(".",(de->d_name))==0 || strcmp("..",(de->d_name))==0 ){
				continue;
			}
			if(stat(de->d_name, &inner) == 0){
				if(S_ISDIR(inner.st_mode)){
				
						//chdir(de->d_name);
						string delete_this=de->d_name;
						delete_file_dir(delete_this);
						//chdir("..");
						remove(delete_this.c_str());
						
					}
					else{
						int del = remove(de->d_name);
   						if (!del){//The file is Deleted successfully
      							//return ;
   						}
   						else{//the file is not Deleted
      							return ;
   						}
      							
					}
			}
			else{	
				//chdir("..");
				return; 
			}
    		}
    		remove(first_name.c_str());
    		chdir("..");
		closedir(dr);
		
	}
	else{
		remove(first_name.c_str());
		return;
	}	
}
int main_delete_files_folder(string first_name){
	char cwd[PATH_MAX];
///home/nitink/Documents/AOS/Assignmnet1/delete/New Folder
	//string first_name="sdfsdf/asdfsadg";
	if (getcwd(cwd, sizeof(cwd)) != NULL) {//starting point
         
        }
	delete_file_dir(first_name);
	if (getcwd(cwd, sizeof(cwd)) != NULL) {//after deleting
        }
	string temp;
	for(int i=first_name.length()-1;i>=0;i--){
		if(first_name[i]=='/'){
			temp = first_name.substr(i + 1);
		}
	}
	//cout<<temp<<endl;
	if (getcwd(cwd, sizeof(cwd)) != NULL) {
		//cout<<cwd<<endl;
        }
	//if(chdir("..")<0)return 1;
	if(temp!="")
		remove(temp.c_str());
	//goto back and delete destination folder also
	return 0;
}

/*** Move Dir ***/
int create_dir_move(string input,struct stat sb)
{
	struct stat info;
	//string input="/home/nitink/Documents/AOS/Assignmnet1/delete/hello/1/2/3/";
    	int i=0;
    	input+='/';
    	string temp="";
    	while(i<input.length()){
    		for(;input[i]!='/'&&i<input.length();i++){
    			temp+=input[i];
    		}
    		if (mkdir(temp.c_str(),0777) != 0){;}
		temp+='/';i++;	
    	}
    	return 1;
}
int copy_files_move(string source,string destination,string file_name){
	char cwd[PATH_MAX];
	char buf;
	struct stat sb;
	struct passwd *pwd;
        struct group *grp;
    	string source_file=source+'/'+file_name;
	string destination_file=destination+'/'+file_name;
	if(stat(source_file.c_str(), &sb) == -1) {
                exit(EXIT_FAILURE);
        }
        if(destination!=""){
        	create_dir_move(destination,sb);
        }
	int fd_one, fd_two;
	fd_one = open(source_file.c_str(), O_RDONLY);
	if (fd_one == -1){
		//printf("Error opening first_file\n");
		close(fd_one);
		return 0;
	}
	fd_two = open(destination_file.c_str(), O_WRONLY | O_CREAT, sb.st_mode);
	while(read(fd_one, &buf, 1)){
		write(fd_two, &buf, 1);
	}
  	if (fchown(fd_two, sb.st_uid, sb.st_gid) == -1) {
      		//cout<<"chown fail\n";
      		;
  	}
	close(fd_one);
	close(fd_two);
	return 0;
}
void copy_helper_move(string first_name,string second_name){
	char cwd[PATH_MAX];
	char cwd2[PATH_MAX];
	struct stat sb;
	struct stat inner;
	struct dirent *de;
	DIR *dr;
	if(stat(first_name.c_str(), &sb) < 0) {return;}
	if((S_ISDIR(sb.st_mode))){
		if (getcwd(cwd, sizeof(cwd)) != NULL) {//not really needed?
			//cout<<cwd<<endl;
			;
          	}
		
		dr = opendir(first_name.c_str());//opened source directory
		if(dr==NULL){
			 //cout<<"unable to open"<<first_name;
			 return;
		}
		if(chdir(first_name.c_str())<0){cout<<"chdir issue";return;}//moved to source directory 
		while ((de = readdir(dr)) != NULL){
			if( strcmp(".",(de->d_name))==0 || strcmp("..",(de->d_name))==0 ){
				create_dir_move(second_name,sb);
				continue;
			}
			if(stat(de->d_name, &inner) == 0){//reading details about one of file/folder in present sirectory successful
			//cout<<de->d_name<<endl;;
				if(S_ISDIR(inner.st_mode)){//so we have picked a folder from current directory
					string copy_this=de->d_name;
					string new_source=first_name+'/'+copy_this;
					string new_destination=second_name+'/'+copy_this;
					copy_helper_move(new_source,new_destination);////////////////
				}
				else{
					string file_name=de->d_name;
					copy_files_move(first_name,second_name,file_name);////////////////////////////
				}
			}
			else{// unable to get file/folder stats
				;
			}
    		}
    		chdir("..");
		closedir(dr);
	}
	else{
		copy_files_move(first_name,second_name,"noname");//we got a file as input this should not have happened
		return;
	}	
}
void delete_file_dir_move(string first_name){
	char cwd[PATH_MAX];
	struct stat sb;
	struct stat inner;
	struct dirent *de;
	DIR *dr;
	if(stat(first_name.c_str(), &sb) < 0) {
                //die("stat");
                //exit(EXIT_FAILURE);
                return;
        }
	if((S_ISDIR(sb.st_mode))){
		if (getcwd(cwd, sizeof(cwd)) != NULL) {
			//cout<<cwd<<endl;
          	}
		dr = opendir(first_name.c_str());
		if(dr==NULL){
			 //cout<<"unable to open";
			 return;
		}
		if(chdir(first_name.c_str())<0)return;
		while ((de = readdir(dr)) != NULL){
			if( strcmp(".",(de->d_name))==0 || strcmp("..",(de->d_name))==0 ){
				continue;
			}
			if(stat(de->d_name, &inner) == 0){
				if(S_ISDIR(inner.st_mode)){
				//cout<<(de->d_name)<<"\n";
						//chdir(de->d_name);
						string delete_this=de->d_name;
						delete_file_dir_move(delete_this);
						//chdir("..");
						remove(delete_this.c_str());
					}
					else{
						int del = remove(de->d_name);
   						if (!del){
      							//printf("The file is Deleted successfully:");cout<<de->d_name<<endl;
      							//return ;
   						}
   						else{
   							//printf("\nthe file is not Deleted:");
      							return ;
   						}	
					}
			}
			else{	
				//chdir("..");
				return; 
			}
    		}
    		remove(first_name.c_str());
    		chdir("..");
		closedir(dr);
		
	}
	else{
		remove(first_name.c_str());
		return;
	}	
}
int move_dir(string source,string destination){
	char cwd[PATH_MAX];
	if (getcwd(cwd, sizeof(cwd)) != NULL) {
		//cout<<"before copying files cwd:"<<cwd<<endl;
        }
        string CWD=cwd;
	copy_helper_move(source,destination);
	if(chdir(CWD.c_str())<0)return 0;
	if (getcwd(cwd, sizeof(cwd)) != NULL) {
		//cout<<"after copying:"<<cwd<<endl;
        }
	delete_file_dir_move(source);
	if (getcwd(cwd, sizeof(cwd)) != NULL) {
		//cout<<"after deleting CWD:"<<cwd<<endl;
        }
	string temp;
	for(int i=source.length()-1;i>=0;i--){
		if(source[i]=='/'){
			temp = source.substr(i + 1);
		}
	}
	//cout<<"is it deleted?"<<temp<<endl;//
	if (getcwd(cwd, sizeof(cwd)) != NULL) {
		//cout<<cwd<<endl;
        }
	if(temp!="")
		remove(temp.c_str());
	return 0;

}

int main_move_dir(string source, string destination)//asumption this is called to copy directory only
{
	char cwd[PATH_MAX];// give path starting with '/' but does not ends with '/'	
	if (getcwd(cwd, sizeof(cwd)) != NULL) {
		//cout<<cwd<<endl;
        }
        string CWD=cwd;
        string real_source;//=CWD;
        string real_destination;//=CWD;
        if(absolute(source)==false&&source[0]!='/'){
        	real_source=CWD+'/'+source;        	
        }
        else if(absolute(source)==false&&source[0]=='/'){
        	real_source=CWD+source;
        }
        else if(absolute(source)==true){
        	real_source=source;
        }
        if(absolute(destination)==false&&destination[0]!='/'){
        	real_destination=CWD+'/'+destination;
        }
        else if(absolute(destination)==false&&destination[0]=='/'){
        	real_destination=CWD+destination;
        }
        else if(absolute(destination)==true){
        	real_destination=destination;
        }
	move_dir(real_source,real_destination);
  if(chdir(CWD.c_str())<0){die(CWD.c_str());}
	return 0;
}


/*** Move Files ***/
void move_file(string source,string destination){//works even if destination exists or not
	char cwd[PATH_MAX];
	char buf;
        struct stat sb;
       	int i = source.length() - 1; // last character
    	if(stat(source.c_str(), &sb) == -1) {
               // exit(EXIT_FAILURE);
               ;
        }
    	string file_name;
    	while (i != 0 && source[i]!='/')
    	{
      		--i;
    	}
    	file_name = source.substr(i+1);
    	//cout<<"File Name"<<file_name<<endl;
    	//cout<<"source Name"<<source<<endl;
    	//cout<<"dest Name"<<destination<<endl;
        if(destination!=""){
        	if(destination[destination.length()-1]!='/'){
        		destination+='/';
        	}
        	create_dir_move(destination,sb);
        }
	int fd_one, fd_two;
	fd_one = open(source.c_str(), O_RDONLY);
	if (fd_one == -1){
		printf("Error opening first_file\n");
		close(fd_one);
		return ;
	}
	string dest_file;
	dest_file=destination+file_name;
	fd_two = open(dest_file.c_str(), O_WRONLY | O_CREAT, sb.st_mode);
	while(read(fd_one, &buf, 1)){
		write(fd_two, &buf, 1);
	}
  	if (fchown(fd_two, sb.st_uid, sb.st_gid) == -1) {
      		cout<<"chown fail\n";
  	}
	close(fd_one);
	close(fd_two);
	remove(source.c_str());
}
int main_move_file(string source, string destination)//asumption this is called to copy directory only
{
	char cwd[PATH_MAX];// give path starting with '/' but does not ends with '/'
	/////////////////////////////////////////
	if (getcwd(cwd, sizeof(cwd)) != NULL) {
		//cout<<cwd<<endl;
        }
        string CWD=cwd;
        string real_source;//=CWD;
        string real_destination;//=CWD;
        if(absolute(source)==false&&source[0]!='/'){
        	real_source=CWD+'/'+source;        	
        }
        else if(absolute(source)==false&&source[0]=='/'){
        	real_source=CWD+source;
        }
        else if(absolute(source)==true){
        	real_source=source;
        }
        if(absolute(destination)==false&&destination[0]!='/'){
        	real_destination=CWD+'/'+destination;
        }
        else if(absolute(destination)==false&&destination[0]=='/'){
        	real_destination=CWD+destination;
        }
        else if(absolute(destination)==true){
        	real_destination=destination;
        }
	///////////////////////////////////////
	move_file(real_source,real_destination);
	return 0;
}


/*** Rename File ***/
int main_rename(string old_name, string new_name)
{
    int value= rename(old_name.c_str(), new_name.c_str()); 
    // Print the result
    if(!value){
        printf("%s", "File name changed successfully");
    }
    else{
        perror("Error");
    }
    return 0;
}

/*** Search File/Folder ***/
bool search(string first_name,string to_search){// searches directory_name or file_name not directory_location or file_location
	bool flag=false;
	if(strcmp(to_search.c_str(),(first_name.c_str()))==0){
   		flag=true;
		return flag;
   	}
	struct stat sb;
	struct stat inner;
	struct dirent *de;
	DIR *dr;
	if(stat(first_name.c_str(), &sb) < 0) {
                //cout<<first_name<<":";
                //cout<<"unable to fetch fstat\n"; 
                return flag;
        }
	if((S_ISDIR(sb.st_mode))){
		if (getcwd(cwd, sizeof(cwd)) != NULL) {
			//cout<<cwd<<endl;
          	}
		dr = opendir(first_name.c_str());
		if(dr==NULL){
			 //cout<<"unable to open";
			 return flag;
		}
		if(chdir(first_name.c_str())<0)return flag;
		while ((de = readdir(dr)) != NULL){
			//cout<<de->d_name<<endl;
			if( strcmp(".",(de->d_name))==0 || strcmp("..",(de->d_name))==0 ){
				//cout<<de->d_name;
				continue;
			}
			if(stat(de->d_name, &inner) == 0){
			//cout<<de->d_name<<endl;;
				if(S_ISDIR(inner.st_mode)){
					//cout<<"mujhe folder ke andar folder mila:";
					//cout<<(de->d_name)<<"\n";
					if(strcmp(to_search.c_str(),(de->d_name))==0){
						flag=true;
						return flag;
					}
					else{
						string find_this=de->d_name;
						flag=search(find_this,to_search);
					}
					//chdir("..");
					//remove(delete_this.c_str());	
					//return flag;
				}
				else{
					//int del = remove(de->d_name);
					//cout<<"mai idhar aaya hu";
					//cout<<"mujhe folder ke andar file mila:";
					//cout<<(de->d_name)<<"\n";
   					if(strcmp(to_search.c_str(),(de->d_name))==0){
   						flag=true;
						return flag;
   					}	
   					//return flag;	
				}
			}
			else{	
				//chdir("..");
				if(strcmp(to_search.c_str(),first_name.c_str())==0){
   					flag=true;
					return flag;
   				}
				//return flag;
			}
    		}
    		//remove(first_name.c_str());
    		chdir("..");
		closedir(dr);
	}
	else{
		if(strcmp(to_search.c_str(),(first_name.c_str()))==0){
			flag=true;
			return flag;
		}
		//return flag;
	}	
	return flag;
}
int main_search(string first_name){
	char cwd[PATH_MAX];
	//string first_name="3hellosdfsdfsf";
	//string first_name="geeksforgeeks.txt";
	bool found=search(".",first_name);	
  string result;
	if(found)
    result="True";
	else result="False";
  editorSetStatusMessage(result.c_str());
  editorRefreshScreen();
  unsigned int microseconds=3000000;
  usleep(microseconds);
	return 0;
}

/*** Check Absolute path or not ***/
bool absolute(string str){
	int j;
	if(str[0]=='~'&&str[1]=='/')
		j=2;
	else if(str[0]=='~'&&str[1]!='/')
		j=1;
	else if(str[0]=='/')
		j=1;
	else 
		j=0;
	int i;
	for(i=j;i<str.length();i++){
		if(str[i]=='/'){
			i--;
			break; 
		}
	}
	string keyword=str.substr(j, i);
	//cout<<keyword<<":";
	if( strcmp("bin",(keyword.c_str()))==0 || strcmp("libx32",(keyword.c_str()))==0 || strcmp("snap",(keyword.c_str()))==0 || strcmp("boot",(keyword.c_str()))==0 || strcmp("media",(keyword.c_str()))==0 || strcmp("srv",(keyword.c_str()))==0 || strcmp("dev",(keyword.c_str()))==0 || strcmp("mnt",(keyword.c_str()))==0 || strcmp("sys",(keyword.c_str()))==0 || strcmp("etc",(keyword.c_str()))==0 || strcmp("opt",(keyword.c_str()))==0 || strcmp("tmp",(keyword.c_str()))==0 || strcmp("home",(keyword.c_str()))==0 || strcmp("proc",(keyword.c_str()))==0 || strcmp("usr",(keyword.c_str()))==0 || strcmp("lib",(keyword.c_str()))==0 || strcmp("root",(keyword.c_str()))==0 || strcmp("var",(keyword.c_str()))==0 || strcmp("lib32",(keyword.c_str()))==0 || strcmp("run",(keyword.c_str()))==0 || strcmp("lib64",(keyword.c_str()))==0 || strcmp("sbin",(keyword.c_str()))==0  )
		return true;
	return false;
}

/*** Preprocess Command Input ***/
string sanitise_input(string input){// removes '.' from input '..' from input
	string loc="";
	if(input[0]=='~'){
		loc=home_dir+input.substr(1);
		return loc;
	}
	else if(absolute(input))
		return input;
	else if(absolute(input)==false&& input[0]=='/'){
		loc=input.substr(1);
		return loc;
	}
	return input;
}
/*** Is Dir ***/
int is_dir(string address){
  struct stat sb;
	if(stat(address.c_str(), &sb) < 0) {
    return -1;//error occured
  }
  else{
    if((S_ISDIR(sb.st_mode))){return 1;}
    else return 0;
  }
  return 0;
}//1: Directory, 0:file, -1:error

























void abAppend(struct abuf *ab, const char *s, int len) {////////////////////////////
    char *new_ = (char *)realloc(ab->b, ab->len + len);
    if (new_ == NULL) return;// unable to allocate memory
    memcpy(&new_[ab->len], s, len);
    ab->b = new_;
    ab->len += len;
}
void abFree(struct abuf *ab) {/////////////////////////////////////////
    free(ab->b);
}
/*** Error handling ***/
void die(const char *s) {/////////////////////////////////////////////
    write(STDOUT_FILENO, "\x1b[2J", 4);//clear screen
    write(STDOUT_FILENO, "\x1b[H", 3);//reposition cursor
    perror(s);
    exit(1);
}
/*** canonical mode ***/
void disableRawMode() {//////////////////////////////////////////////////////
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &E.orig_termios) == -1)die("tcsetattr");
}
void enableRawMode() {///////////////////////////////////////////////////////
    if (tcgetattr(STDIN_FILENO, &E.orig_termios) == -1) die("tcgetattr");
    atexit(disableRawMode);
    struct termios raw = E.orig_termios;
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    raw.c_oflag &= ~(OPOST);
    raw.c_cflag |= (CS8);
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    //raw.c_cc[VMIN] = 0;
    //raw.c_cc[VTIME] = 1;
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("tcsetattr");
}
/*** input handle ***/
int editorReadKey() {///////////////
  int nread;
  char c;
  while ((nread = read(STDIN_FILENO, &c, 1)) != 1) {
    if (nread == -1 && errno != EAGAIN) die("read");
  }
  if (c == '\x1b') {
    char seq[3];
    if (read(STDIN_FILENO, &seq[0], 1) != 1) return '\x1b';
    if (read(STDIN_FILENO, &seq[1], 1) != 1) return '\x1b';
    if (seq[0] == '[') {
      if (seq[1] >= '0' && seq[1] <= '9') {
        if (read(STDIN_FILENO, &seq[2], 1) != 1) return '\x1b';
        if (seq[2] == '~') {
          switch (seq[1]) {
            case '1': return HOME_KEY;
            case '3': return DEL_KEY;
            case '4': return END_KEY;
            case '5': return PAGE_UP;
            case '6': return PAGE_DOWN;
            case '7': return HOME_KEY;
            case '8': return END_KEY;
          }
        }
      } else {
        switch (seq[1]) {
          case 'A': return ARROW_UP;
          case 'B': return ARROW_DOWN;
          case 'C': return ARROW_RIGHT;
          case 'D': return ARROW_LEFT;
          case 'H': return HOME_KEY;
          case 'F': return END_KEY;
        }
      }
    } else if (seq[0] == 'O') {
      switch (seq[1]) {
        case 'H': return HOME_KEY;
        case 'F': return END_KEY;
      }
    }
    return '\x1b';
  } else {
      if(c==13){
          return 13;//enter
      }
      if(c==8){//should be 127 for backspace
          return 8;
      }
      if(c==58){
          return 58;
      }
      
    return c;
  }
}
/*** cursor ***/
int getCursorPosition(int *rows, int *cols) {////////////////
  char buf[32];
  unsigned int i = 0;
  if (write(STDOUT_FILENO, "\x1b[6n", 4) != 4) return -1;
  while (i < sizeof(buf) - 1) {
    if (read(STDIN_FILENO, &buf[i], 1) != 1) break;
    if (buf[i] == 'R') break;
    i++;
  }
  buf[i] = '\0';
  if (buf[0] != '\x1b' || buf[1] != '[') return -1;
  if (sscanf(&buf[2], "%d;%d", rows, cols) != 2) return -1;
  return 0;
}
int getWindowSize(int *rows, int *cols) {///////////////////////
  struct winsize ws;
  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
    if (write(STDOUT_FILENO, "\x1b[999C\x1b[999B", 12) != 12) return -1;
    return getCursorPosition(rows, cols);
  } else {
    *cols = ws.ws_col;
    *rows = ws.ws_row;
    return 0;
  }
}


/*** status bar ***/
void editorDrawStatusBar(struct abuf *ab) {///////////////////////////
    abAppend(ab, "\x1b[7m", 4);
    char status[80];
    int len = snprintf(status, sizeof(status), "%.20s - %d Files or Folders", E.filename ? E.filename : "[No Name]", E.numrows);
    //the above can be used to display normal/command mode
    if (len > E.screencols) len = E.screencols;
    abAppend(ab, status, len);//append the message on buffer
    while (len < E.screencols) {
        abAppend(ab, " ", 1);//append blank space on buffer
        len++;
    }
    abAppend(ab, "\x1b[m", 3);
    abAppend(ab, "\r\n", 2);
}

/*** message bar***/
void editorDrawMessageBar(struct abuf *ab) {/////////////////////////////
  abAppend(ab, "\x1b[K", 3);
  int msglen = strlen(E.statusmsg);
  if (msglen > E.screencols) msglen = E.screencols;
  if (msglen && time(NULL) - E.statusmsg_time < 5)
    abAppend(ab, E.statusmsg, msglen);
}

void editorSetStatusMessage(const char *fmt, ...) {////////////////////////
  va_list ap;//to handle all the arguments seent to this function & to print them all instead of few
  va_start(ap, fmt);
  vsnprintf(E.statusmsg, sizeof(E.statusmsg), fmt, ap);
  va_end(ap);
  E.statusmsg_time = time(NULL);
}
/*** output ***/
int editorRowCxToRx(erow *row, int cx) {/////////////////////////////
  int rx = 0;
  int j;
  for (j = 0; j < cx; j++) {
    if (row->chars[j] == '\t')
      rx += (KILO_TAB_STOP - 1) - (rx % KILO_TAB_STOP);
    rx++;
  }
  return rx;
}
void editorDrawRows(struct abuf *ab) {//////////////////////
  int y;
  for (y = 0; y < E.screenrows; y++) {//print till dcreen size only,itterate over all row of screen
    //if (y >= E.numrows) {
    int filerow = y + E.rowoff;//for scrolling part, filerow tell wich row of file to print
    if (filerow >= E.numrows) {//number of rows to be is more than screen row size
      if (E.numrows == 0 && y == E.screenrows / 3) {// if number of rows to be printed is zero
        char welcome[80];
        int welcomelen = snprintf(welcome, sizeof(welcome), "Kilo editor -- version %s", KILO_VERSION);
        //above line copy whatever to be outputed in buffer names as "welcome" , if size of what is to be printer
        //is more than sizeof(welcome) then discared following characters but count them and return that numeric count.
        //into welcomelen
        if (welcomelen > E.screencols) welcomelen = E.screencols;
        int padding = (E.screencols - welcomelen) / 2;//find how much padding to give
        //actually it was  padding = E.screencols/2 - welcomelen / 2;
        if (padding) {// print first ~ on our message line 
          abAppend(ab, "~", 1);
          padding--;
        }
        while (padding--) abAppend(ab, " ", 1);//print left padding for our message
        abAppend(ab, welcome, welcomelen);//print message after left padding
      }
      else { //for those line where welcome length or file content is not to be displayed just print ~
        abAppend(ab, "~", 1);
      }
    } 
    else {//print buffer content
      int len = E.row[filerow].rsize - E.coloff;
      if (len < 0) len = 0;
      if (len > E.screencols) len = E.screencols;
      abAppend(ab, &E.row[filerow].render[E.coloff], len);
    }
    abAppend(ab, "\x1b[K", 3);
    abAppend(ab, "\r\n", 2);
  }
}
/*** input ***/
void editorMoveCursor(int key) {////////////////////////////////////////////
     erow *row = (E.cy >= E.numrows) ? NULL : &E.row[E.cy];
  switch (key) {
    case ARROW_LEFT:
      if (E.cx != 0) {
        E.cx--;
      } else if (E.cy > 0) {//if x coordinate was 0, and we pressed left arrow key,, then adjust y cooredinat ealso
        E.cy--;
        E.cx = E.row[E.cy].size;
      }
      break;
    case ARROW_RIGHT:
      //if (E.cx != E.screencols - 1) { //commented to allow horizontal scrolling
       // E.cx++;
      //}
            if (row && E.cx < row->size) {
        E.cx++;
              } else if (row && E.cx == row->size) {
        E.cy++;
        E.cx = 0;
      }
      break;
    case ARROW_UP:
      if (E.cy != 0) {
        E.cy--;
      }
      break;
    case ARROW_DOWN:
      /*if (E.cy != E.screenrows - 1) { //original no scroll
        E.cy++;
      }*/
            if (E.cy < E.numrows) {//adds scroll feature on press down key
        E.cy++;
      }
      break;
  }
    row = (E.cy >= E.numrows) ? NULL : &E.row[E.cy];// do adjust ment to not go beyond the line
  int rowlen = row ? row->size : 0;
  if (E.cx > rowlen) {
    E.cx = rowlen;
  }

}
/*** Scroll ***///////////////////////////////////////////////
void editorScroll() {//for finding which line of buffer tha to be printed
    E.rx = 0;
  if (E.cy < E.numrows) {
    E.rx = editorRowCxToRx(&E.row[E.cy], E.cx);
  }

  if (E.cy < E.rowoff) {
    E.rowoff = E.cy;
  }
  if (E.cy >= E.rowoff + E.screenrows) {
    E.rowoff = E.cy - E.screenrows + 1;
  }
  if (E.rx < E.coloff) {
    E.coloff = E.rx;
  }
  if (E.rx >= E.coloff + E.screencols) {
    E.coloff = E.rx - E.screencols + 1;
  }
}
void getCommand(int c){

}
void editorRowInsertChar(erow *row, int at, int c) {//////////////////////////////////
  if (at < 0 || at > row->size) at = row->size;
  row->chars = (char*)realloc(row->chars, row->size + 2);
  memmove(&row->chars[at + 1], &row->chars[at], row->size - at + 1);
  row->size++;
  row->chars[at] = c;
  editorUpdateRow(row);
}
void goto_dir(string dest){
    const char * dests=dest.c_str();
    int ch=chdir(dests);
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        //printf("Current working dir: %s\n", cwd);
        if((!last_move)){//this move is enter hence it is not < or >
            history.push_back(cwd);
            last_move=false;
            curr_pos=history.size();
        }
        else if((last_move)){//this move is enter hence it is not < or >
            history.erase(history.begin()+curr_pos,history.end());//check +1 or not
            history.push_back(cwd);
            last_move=false;
            curr_pos=history.size()-1;
        }
    } 
    else {
        die("getcwd() error");
        //return 1;
    }
    /*if the change of directory was successful it will print successful otherwise it will print not successful*/
    if(ch<0)
        die("chdir change of directory unsuccessful");
    else{
        directoryOpen(".");
    }
}
void process_string(string query){

    string command;
    vector <string> source;
    string destination;
    string temp;
    bool command_fetched=false;
    for(int i=13;i<query.length();i++){
        if(( query[i] != ' ' )&& command_fetched==false){
            command+=query[i];
        }
        else if(query[i] == ' ' && command_fetched==false){
            command_fetched=true;
            continue;
        }
        else if(( query[i] != ' ' ) && command_fetched==true){
            temp+=query[i];
        }
        else if(( query[i] == ' ' ) && command_fetched==true&& (temp!="")){
            source.push_back(temp);
            temp="";
        }
        
        
    }source.push_back(temp);
    if(command=="search"&&source.size()==1){//working
      main_search(source[0]);
      return;
    }
    else if(command=="search"&&source.size()!=1){
      die("mismatched Number of parameter(s)");
    }//will work for . & ./ also

    if(command=="rename"&&source.size()==2){//working
      //command:rename foo.txt bar.txt
      destination=source[source.size()-1];
      source.pop_back();
      string src1=sanitise_input(source[0]);
      string src2=sanitise_input(destination);
      main_rename(src1,src2);      
      return;
    }
    else if(command=="rename"&&source.size()!=2){
      die("mismatched Number of parameter(s)");
    }//will work for . & ./ also

    if(command=="create_file"&&source.size()==2){//working
      //command: create_file <file_name> <destination_path>
      destination=source[source.size()-1];
      source.pop_back();
	    string dir_loc=sanitise_input(destination);
	    main_create_file(source[0],dir_loc);
      return;
    }
    else if(command=="create_file"&&source.size()!=2){
      die("mismatched Number of parameter(s)");
    }//will work for . & ./ also
    
    if(command=="create_dir"&&source.size()==2){//working
      //command: create_dir <dir_name> <destination_path>
      destination=source[source.size()-1];
      source.pop_back();
	    string dir_loc=sanitise_input(destination);
	    main_create_dir(source[0],dir_loc);//all working
      return;
    }
    else if(command=="create_dir"&&source.size()!=2){
      die("mismatched Number of parameter(s)");
    }//will work for . & ./ also

    if(command=="copy"&&source.size()>=2){//1: Directory, 0:file, -1:error
        destination=source[source.size()-1];
        source.pop_back();
        for(int i=0;i<source.size();i++){
          int temp=is_dir(source[i]);
          if(temp==0){//working for files
            //copy file command: copy foo.txt bar.txt baz.mp4 ~/foobar 
	          string src_loc=sanitise_input(source[i]);
	          string dest_loc=sanitise_input(destination);//dest may not exist
	          main_copy_files(src_loc,dest_loc);
          }// works for . also
          else if(temp==1){
            //copy dir command: copy footxt bartxt bazmp4 ~/foobar 
            //copies the content of a folder inside a destination folder
	          //does not copies the folder inside destintaion folder
	          string src_loc=sanitise_input(source[i]);
	          string dest_loc=sanitise_input(destination);//dest may not exist
	          main_copy_directory(src_loc,dest_loc);
          }
          else if(temp==-1){
            die("copy command is_dir check failed");
          }
        }
        return ;
    }
    else if(command=="copy"&&source.size()<2){
        die("number of parameter is too less");
    }
    if(command=="move"&&source.size()>=2){
        destination=source[source.size()-1];
        source.pop_back();
        string locs;
        for(int i=0;i<source.size();i++){

          int temp=is_dir(source[i]);
          if(temp==0){//working for files
            //move file command: copy foo.txt bar.txt baz.mp4 ~/foobar 
	          string src_loc=sanitise_input(source[i]);
	          string dest_loc=sanitise_input(destination);//dest may not exist
	          main_move_file(src_loc,dest_loc);
          }// works for . also
          else if(temp==1){
            //moves the content of a folder inside a destination folder
	          //does not moves the folder inside destintaion folder
	          //command: move footxt bartxt bazmp4 ~/foobar 
	          string src_loc=sanitise_input(source[i]);
	          string dest_loc=sanitise_input(destination);//dest may not exist
	          main_move_dir(src_loc,dest_loc);
          }
          else if(temp==-1){
            die("move command is_dir check failed");
          }
        }
        return ;
    }
    else if(command=="move"&&source.size()<2){
        die("number of parameter is too less");
    }
    if(command=="delete_file"&&source.size()==1){
      //command: delete_file <file_path>’
	    string src_loc=sanitise_input(source[0]);
	    main_delelte_file(src_loc);
      return ;
    }
    else if(command=="delete_file"&&source.size()!=1){
      die("number of parameter is incorrect for the command");
    }

    if(command=="delete_dir"&&source.size()==1){
      //Delete Directory command: ‘delete_dir <dir_path>’
	    string src_loc=sanitise_input(source[0]);
	    main_delete_files_folder(src_loc);
      return ;
    }
    else if(command=="delete_dir"&&source.size()!=1){
      die("number of parameter is incorrect for the command");
    }
    if(command=="goto"&&source.size()==1){
        goto_dir(source[0].c_str());
        return ;
    }
    else{
        die("Invalid Command");
    }
}


void editorInsertChar(int c) {
    //if(mode){
        if (E.cy == E.numrows) {
            //string abc="";
            //char *a=const_cast<char*> ( abc.c_str() );
            char *a=(char*)malloc(2);//this can fail anytime
            editorAppendRow(a, 0);
        }
        //editorRowInsertChar(&E.row[E.cy], E.cx, c);
        //editorRowInsertChar(&E.row[0], E.cx, c);//this can fail
        //typed="sadf";
        //while()
        string typed="Command Mode:";
        editorSetStatusMessage(typed.c_str());
        editorRefreshScreen();
        //editorProcessKeypress();
        int nread;
        char cb;
        while (true) {
            int status=read(STDIN_FILENO, &cb, 1);
            if (status == -1 && errno != EAGAIN) die("read");
            //typed+=cb;
            if(cb == 27){
                //die("Exited");
                typed="Normal Mode";
                editorSetStatusMessage(typed.c_str());
                editorRefreshScreen();
                return ;
            }
            else if(cb == 13){
                //die("Exited");
                process_string(typed);
                typed="Command Mode:";
                editorSetStatusMessage(typed.c_str());
                editorRefreshScreen();
                //return ;
                //break;
            }
            else{ 
                if(cb=='q'&& typed.length()==13){die("Program Exited");}
                typed+=cb;
                editorSetStatusMessage(typed.c_str());
                editorRefreshScreen();
            }
            //editorProcessKeypress();
        }
        // while ((nread = read(STDIN_FILENO, &cb, 1)) != 1) {
        //     if (nread == -1 && errno != EAGAIN) die("read");
        //     typed+=cb;
        //     editorSetStatusMessage(typed.c_str());
        //     editorRefreshScreen();
        //     editorProcessKeypress();
        //     die("failed");
        // }
        
        //editorSetStatusMessage(c);
        E.cx++;
    //}
    //else{
    //    mode=true;
    //}
}
void editorProcessKeypress() {////////////////////////////////////////////
  int c = editorReadKey();
  switch (c) {
    case 113 ://input is 'q' to exit
      write(STDOUT_FILENO, "\x1b[2J", 4);//clear scrren before exit
      write(STDOUT_FILENO, "\x1b[H", 3);
      exit(0);
      break;
    case HOME_KEY:
      E.cx = 0;
      break;
    case END_KEY:
        //E.cx = E.screencols - 1;//go till last row of screen
         if (E.cy < E.numrows)//go till last row of  that line
        E.cx = E.row[E.cy].size;
      break;
    case PAGE_UP:
    case PAGE_DOWN:
      {
          if (c == PAGE_UP) {
          E.cy = E.rowoff;
        } else if (c == PAGE_DOWN) {
          E.cy = E.rowoff + E.screenrows - 1;
          if (E.cy > E.numrows) E.cy = E.numrows;
        }
        int times = E.screenrows;
        while (times--)
          editorMoveCursor(c == PAGE_UP ? ARROW_UP : ARROW_DOWN);
      }
      break;
    case ARROW_UP:
    case ARROW_DOWN:
    case ARROW_LEFT:
    case ARROW_RIGHT:
      editorMoveCursor(c);
      break;
    //case 1999:
    case 13:
      enterDir(c);
      break;
    case 127:
      backspaceDir(c);
      break;
    case 111:
      move_left(c);//o
      break;
    case 112:
      move_right(c);//p
      break;
      
    case 104:
      move_home(c);//p
      break;
    case 58:
    {
        editorInsertChar(c);
        break;

    }
        //getCommand(c);
        
    //default:
      //break;
  }
}
void editorUpdateRow(erow *row) {///////////////////////////////////////////////
  int tabs = 0;
  int j;
  for (j = 0; j < row->size; j++)
    if (row->chars[j] == '\t') tabs++;
  free(row->render);
  row->render = (char*)malloc(row->size + tabs*(KILO_TAB_STOP - 1) + 1);
  int idx = 0;
  for (j = 0; j < row->size; j++) {
    if (row->chars[j] == '\t') {
      row->render[idx++] = ' ';
      while (idx % KILO_TAB_STOP != 0) row->render[idx++] = ' ';
    } else {
      row->render[idx++] = row->chars[j];
    }
  }
  row->render[idx] = '\0';
  row->rsize = idx;
}
void editorAppendRow(char *s, size_t len) {/////////////////////////////
  E.row = (erow*)realloc(E.row, sizeof(erow) * (E.numrows + 1));//check it can be (char*) also
  //allocate memory for row
  int at = E.numrows;
  E.row[at].size = len;
  E.row[at].chars = (char*)malloc(len + 1);//check it can be (erow*) also
  memcpy(E.row[at].chars, s, len);
  E.row[at].chars[len] = '\0';
  
  E.row[at].rsize = 0;
  E.row[at].render = NULL;
    editorUpdateRow(&E.row[at]);
  E.numrows++;
}


/*** file i/o ***/
void editorOpen(char *filename) {/////////////////////////////////
  free(E.filename);
  
  E.filename = strdup(filename);//creates new strings from given aregument and returns pointer to new one
  FILE *fp = fopen(filename, "r");
  if (!fp) die("fopen");
  char *line = NULL;
  size_t linecap = 0;
  ssize_t linelen;
  while ((linelen = getline(&line, &linecap, fp)) != -1) {//while you are getting lines , keep adding them to buffer structure
    while (linelen > 0 && (line[linelen - 1] == '\n' ||   line[linelen - 1] == '\r'))
      linelen--;
    editorAppendRow(line, linelen);
  }
  free(line);
  fclose(fp);
}
/*** directory ***/

string get_details(char * file_name)
{
    //string str = charPtr;
        struct stat sb;
        struct passwd *pwd;
        struct group *grp;
        //string str="stat.cpp";
        string ans=file_name;
        string file_Name=file_name;
        
        curr_dir_info *new_file=new curr_dir_info();
        new_file->filename=file_Name;
        //new_file.filename=file_Name;
        //ans+=str;
        //const char *file_name = str.c_str();
        /*if (argc != 2) {
                fprintf(stderr, "Usage: %s <pathname>\n", argv[0]);
                exit(EXIT_FAILURE);
        }*/
        //if (stat(argv[1], &sb) == -1) {
        if(stat(file_name, &sb) == -1) {
                die("stat");
                //exit(EXIT_FAILURE);
        }
        ans+="\t";
        string file_size;
        if( T_B <((long long) sb.st_size)){
                ans+=to_string((((long long) sb.st_size)/ T_B ));
                ans+="TB";
                file_size+=to_string((((long long) sb.st_size)/ T_B ));
                file_size+="TB";
        }
        else if( G_B <((long long) sb.st_size)){
                ans+=to_string((((long long) sb.st_size)/ G_B ));
                ans+="GB";
                file_size+=to_string((((long long) sb.st_size)/ G_B ));
                file_size+="GB";
        }
        else if( M_B <((long long) sb.st_size)){
                ans+=to_string((((long long) sb.st_size)/ M_B ));
                ans+="MB";

                file_size+=to_string((((long long) sb.st_size)/ M_B ));
                file_size+="MB";
        }
        else if( K_B <((long long) sb.st_size)){
                ans+=to_string((((long long) sb.st_size)/ K_B ));
                ans+="KB";

                file_size+=to_string((((long long) sb.st_size)/ K_B ));
                file_size+="KB";
        }
        else if(K_B>((long long) sb.st_size))
        {
                ans+=to_string((((long long) sb.st_size)));
                ans+="B";
                file_size+=to_string((((long long) sb.st_size)));
                file_size+="B";
        }
        new_file->fileSize=file_size;
   
        ans+="\t";
        string user_id;
        if ((pwd = getpwuid(sb.st_uid)) != NULL){
          ans+=(pwd->pw_name);
          user_id+=(pwd->pw_name);
        }
        else{
          ans+=(sb.st_uid);
          user_id+=(sb.st_uid);
        }
                
   new_file->filesUserID=user_id;
   
        ans+="\t";
        string group_id;
        if ((grp = getgrgid(sb.st_gid)) != NULL){
          ans+=(grp->gr_name);
          group_id+=(grp->gr_name);
        }    
        else{
          ans+=(sb.st_gid);
          group_id+=(sb.st_gid);
        } 
        new_file->filesGroupId=group_id;
        
        ans+="\t";
        string permission;
        if((S_ISDIR(sb.st_mode))){ans+="d";permission+="d";}else{ans+="-";permission+="-";}
        if((sb.st_mode & S_IRUSR)){ans+="r";permission+="r";}else{ans+="-";permission+="-";}
        if((sb.st_mode & S_IWUSR)){ans+="w";permission+="w";}else{ans+="-";permission+="-";}
        if((sb.st_mode & S_IXUSR)){ans+="x";permission+="x";}else{ans+="-";permission+="-";}
        if((sb.st_mode & S_IRGRP)){ans+="r";permission+="r";}else{ans+="-";permission+="-";}
        if((sb.st_mode & S_IWGRP)){ans+="w";permission+="w";}else{ans+="-";permission+="-";}
        if((sb.st_mode & S_IXGRP)){ans+="x";permission+="x";}else{ans+="-";permission+="-";}
        if((sb.st_mode & S_IROTH)){ans+="r";permission+="r";}else{ans+="-";permission+="-";}
        if((sb.st_mode & S_IWOTH)){ans+="w";permission+="w";}else{ans+="-";permission+="-";}
        if((sb.st_mode & S_IXOTH)){ans+="x";permission+="x";}else{ans+="-";permission+="-";}
        ans+="\t";
        new_file->filePermission=permission;
        string modified_time;
        modified_time+=ctime(&sb.st_mtime);
        new_file->filelastmodified=modified_time;
        ans+=ctime(&sb.st_mtime);
        //cout<<ans;
        fils_info.push_back(new_file);
        return ans;
}
void directoryOpen(string str){/////////////////////
    //free(E.filename);
  //E.filename = strdup(filename);//creates new strings from given aregument and returns pointer to new one
  //FILE *fp = fopen(filename, "r");
  //if (!fp) die("fopen");
  
  initEditor();
    char *line = NULL;
    size_t linecap = 0;
    ssize_t linelen;
    struct dirent *de;
    E.filename =strdup(str.c_str());
    //char * holder=E.filename;
    vector <string> strs;
    DIR *dr = opendir(str.c_str());
    if (!dr){
        string temp="opendir (unable to open):"+str;
        die(temp.c_str());}
    
    
    while ((de = readdir(dr)) != NULL){
            strs.push_back(get_details(de->d_name));
    }
    for(int i=0;i<strs.size();i++){
        char * holder=strdup(strs[i].c_str());
        linelen=strs[i].length();
        //const char * temp=strs[i].c_str();
        //while (linelen > 0 && (holder[linelen - 1] == '\n' ||   holder[linelen - 1] == '\r'))
        //    linelen--;
        editorAppendRow(holder, linelen-1);
    }  
    //str.c_str();
  /*while ((linelen = getline(&line, &linecap, fp)) != -1) {//while you are getting lines , keep adding them to buffer structure
    while (linelen > 0 && (line[linelen - 1] == '\n' ||   line[linelen - 1] == '\r'))
      linelen--;
    editorAppendRow(line, linelen);
  }*/
  //free(line);
  //fclose(fp);
  closedir(dr);
}
/*** referesh screen ***/
void editorRefreshScreen() {/////////////////////////////////////
  editorScroll();
  struct abuf ab = ABUF_INIT;//initializing structure
  abAppend(&ab, "\x1b[?25l", 6);//6 is length of character
  abAppend(&ab, "\x1b[H", 3);//3 is length of chararcter
  editorDrawRows(&ab);//draw the buffer contents
  editorDrawStatusBar(&ab);// fro normal /command mode
  editorDrawMessageBar(&ab);//for message bar,where we can write the command
  char buf[32];
  //snprintf(buf, sizeof(buf), "\x1b[%d;%dH", E.cy + 1, E.cx + 1);
  //snprintf(buf, sizeof(buf), "\x1b[%d;%dH", (E.cy - E.rowoff) + 1, E.cx + 1);
  snprintf(buf, sizeof(buf), "\x1b[%d;%dH", (E.cy - E.rowoff) + 1,(E.rx - E.coloff) + 1);
  //E.cy,E.cx will tell the row number of column number you want
  //above is a fix for cursor position during horizontal and vertical scrolling
  abAppend(&ab, buf, strlen(buf));
  abAppend(&ab, "\x1b[?25h", 6);
  write(STDOUT_FILENO, ab.b, ab.len);
  abFree(&ab);
}
/*** init ***/
void initEditor() {/////////////////////////////////////////////
  E.cx = 0;
  E.cy = 0;
  E.numrows = 0;
  E.row = NULL;
  E.rowoff = 0;
  E.coloff = 0;
  E.rx = 0;
  fils_info.clear();
  E.filename = NULL;
  E.statusmsg[0] = '\0';
  E.statusmsg_time = 0;
  if (getWindowSize(&E.screenrows, &E.screencols) == -1) die("getWindowSize");
  //E.screenrows -= 1;
  E.screenrows -= 2;//one for status bar and one for message bar
}

void move_left(int c){
  if(curr_pos<=0){
     curr_pos=0;
  }
  else{
    if(mode==false){
        // string loc;
        // loc=E.row[E.cy].chars;
        // string dest;
        // int i=0;
        // while((loc[i]!=' ')&&(loc[i]!='\t')&&(loc[i]!='\n')&&(loc[i]!='\0')){
        //     //later change it to (loc[i]!='\t')
        //     dest+=loc[i];i++;
        // }
        // dest+="\0";
        // const char * dests=dest.c_str();
        string dest=history[--curr_pos];
        dest+="\0";
        const char * dests=dest.c_str();
        int ch=chdir(dests);
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
          //printf("Current working dir: %s\n", cwd);
          //this move is enter hence it is not < or >
          last_move=true;
        } 
        else {
          die("getcwd() error");
          //return 1;
        }
        /*if the change of directory was successful it will print successful otherwise it will print not successful*/
        if(ch<0)
            die("chdir change of directory unsuccessful");
        else{
            directoryOpen(".");
        }
    }
  }
  directoryOpen(".");
}
void move_right(int c){
  if(curr_pos>=history.size()-1){
     curr_pos=history.size()-1;
  }
  else{
    if(mode==false){
        // string loc;
        // loc=E.row[E.cy].chars;
        // string dest;
        // int i=0;
        // while((loc[i]!=' ')&&(loc[i]!='\t')&&(loc[i]!='\n')&&(loc[i]!='\0')){
        //     //later change it to (loc[i]!='\t')
        //     dest+=loc[i];i++;
        // }
        // dest+="\0";
        // const char * dests=dest.c_str();
        string dest=history[++curr_pos];
        dest+="\0";
        const char * dests=dest.c_str();
        int ch=chdir(dests);
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
          //printf("Current working dir: %s\n", cwd);
          //this move is enter hence it is not < or >
          last_move=true;
        } 
        else {
          die("getcwd() error");
          //return 1;
        }
        /*if the change of directory was successful it will print successful otherwise it will print not successful*/
        if(ch<0)
            die("chdir change of directory unsuccessful");
        else{
            directoryOpen(".");
        }
    }
  }
  directoryOpen(".");
}
void move_home(int c){//last change made here
  if(mode==false){
        string dest=history[0];
        dest+="\0";
        const char * dests=dest.c_str();
        int ch=chdir(dests);
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
          //printf("Current working dir: %s\n", cwd);
          if((!last_move)){//this move is home hence it is not < or >
            history.push_back(cwd);
            last_move=false;
            curr_pos=history.size();
          }
          else if((last_move)){//this move is home hence it is not < or >
            history.erase(history.begin()+curr_pos,history.end());//check +1 or not
            history.push_back(cwd);
            last_move=false;
            curr_pos=history.size()-1;
          }

        } 
        else {
          die("getcwd() error");
          //return 1;
        }
        /*if the change of directory was successful it will print successful otherwise it will print not successful*/
        if(ch<0)
            die("chdir change of directory unsuccessful");
        else{
            directoryOpen(".");
        }
    }
}
void enterDir(int c){//enter key pressed
    if(mode==false){
        // string loc;
        // loc=E.row[E.cy].chars;
        string dest;
        dest=fils_info[E.cy]->filename;// now space seperated folder are also opening
        //int i=0;
        // while((loc[i]!=' ')&&(loc[i]!='\t')&&(loc[i]!='\n')&&(loc[i]!='\0')){
        //     //later change it to (loc[i]!='\t')
        //     dest+=loc[i];i++;
        // }
        char file=(fils_info[E.cy]->filePermission)[0];
        if(file=='d'){
            dest+="\0";
            const char * dests=dest.c_str();
            int ch=chdir(dests);
            if (getcwd(cwd, sizeof(cwd)) != NULL) {
                //printf("Current working dir: %s\n", cwd);
                if((!last_move)){//this move is enter hence it is not < or >
                    history.push_back(cwd);
                    last_move=false;
                    curr_pos=history.size();
                }
                else if((last_move)){//this move is enter hence it is not < or >
                    history.erase(history.begin()+curr_pos,history.end());//check +1 or not
                    history.push_back(cwd);
                    last_move=false;
                    curr_pos=history.size()-1;
                }
            } 
            else {
                die("getcwd() error");
            //return 1;
            }
            /*if the change of directory was successful it will print successful otherwise it will print not successful*/
            if(ch<0)
                die("chdir change of directory unsuccessful");
            else{
                directoryOpen(".");
            }
        }
        if(file=='-'){
            int pid = fork();
            if (pid == 0) // if child process
            {
                execl("/usr/bin/xdg-open","xdg-open", dest.c_str(), (char *)0);
                exit(0);
            }
        } 
    }
}
void backspaceDir(int c){
    if(mode==false){
        //string loc;
        //loc=E.row[1].chars;
        string dest="..";
        //int i=0;
        //while((loc[i]!=' ')&&(loc[i]!='\t')&&(loc[i]!='\n')&&(loc[i]!='\0')){
        //    //later change it to (loc[i]!='\t')
        //    dest+=loc[i];i++;
        //}
        dest+="\0";
        const char * dests=dest.c_str();
        int ch=chdir(dests);
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
        if((!last_move)){//this move is backspace hence it is not < or >
            history.push_back(cwd);
            last_move=false;
            curr_pos=history.size();
          }
          else if((last_move)){//this move is backspace hence it is not < or >
            history.erase(history.begin()+curr_pos,history.end());//check +1 or not
            history.push_back(cwd);
            last_move=false;
            curr_pos=history.size()-1;
          }

        } 
        else {
          die("getcwd() error");
          //return 1;
        }
        /*if the change of directory was successful it will print successful otherwise it will print not successful*/
        if(ch<0)
            die("chdir change of directory unsuccessful");
        else{
            directoryOpen(".");
        }
    }
}
int main(int argc, char *argv[]) {
  //if (getcwd(cwd, sizeof(cwd)) != NULL) {
	  
  //}
  enableRawMode();
  initEditor();
    const char * ab=".";
    directoryOpen(ab);
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
      history.push_back(cwd);
      home_dir=cwd;
    }
    else{
      die("unable to get cwd");
    }
  //}
  editorSetStatusMessage("Normal Mode");//can be used to work in command mode
  //whatever you print in status msg will be printed there
  while (1) {
    editorRefreshScreen();
    editorProcessKeypress();
  }
  return 0;
}
