#include <iostream>
#include <unistd.h>
#include <string>
#include <pthread.h>
#include <cstring>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
using namespace std;

sem_t sem ;
pthread_mutex_t mymutex;

float** weights;
int* values;
string forwardval="";
int n=0;

struct mydata{
       int num_weight;
       int num;
       float *weight;
       float* value;
};


//
//
//  This is the thread function working as a neuron
//
//
void* neuron(void*p){

      mydata *m = (mydata*)p;
    //  cout<<"This is neuron number: "<<m->num<<endl;
      float res=0;
      for (int i=0;i<m->num_weight;i++){
        //  cout<<"Weight "<<i<<" : "<<m->weight[i]<<endl;
       //   cout<<"value "<<i<<" : "<<m->value[i]<<endl;
          res = res+(m->weight[i]*m->value[i]);
      }
      
   //   cout<<"New Answer "<<m->num<<" : "<<res<<endl;
      
      while (m->num!=n){
      
      }
      sem_wait(&sem);
      //semaphore
      pthread_mutex_lock ( &mymutex ) ;
      forwardval=forwardval+"_"+to_string(res);
      n++;
      pthread_mutex_unlock ( &mymutex) ;
      sem_post(&sem);

}



//
//
//  This is main function
//
//

int main(int argc, char* argv[]){
sem_init(&sem,0,1) ;
sem_t* sem1 ;
sem1 = sem_open("/semaphore", O_CREAT, 0644, 1);
    cout<<"This is layer number: "<<argv[1]<<endl;
    cout<<"Number of nuerons: "<<argv[2]<<endl;
    string f = argv[1];
    int layernum=stoi(argv[1]);
    int tcount = stoi(argv[2]);
    int num_weights = stoi(argv[3]);
    char buf[1024];
    int last = stoi(argv[4]);
    
    while(true){
    
    //
    //
    // Getting values from pipe
    //
    //
    
    string pipe_name22 = "v"+f ; // Pipe name
    char* pipe_name2 = (char*)pipe_name22.c_str();
    int pipe_fd2;  
    int bytes_read=0;

    mkfifo(pipe_name2, 0666);
    string msg = "";
    
    
    pipe_fd2 = open(pipe_name2, O_RDONLY);
   cout<<"*****************my pipe is open**************"<<endl;
    if (pipe_fd2 == -1) {
        cerr << "Error: Failed to open named pipe for reading" << endl;
        return 1;
    }

    // Read data from pipe
    while ((bytes_read = read(pipe_fd2, buf, sizeof(buf))) > 0) {
        buf[bytes_read] = '\0';  // Null-terminate buffer
        //cout << "Received data from pipe: " << buf << endl;
    }
    close(pipe_fd2);
    
    
    
    
    
    
    float *values = new float[num_weights];
    
    
int i=0;
int j=0;
int x=1;
string w="";
    while(buf[x]!='\0'){
         if (buf[x]=='_'){
            cout<<"value: "<<w<<endl;
            values[i]=stof(w);
            
            i++;
            
           
            w="";
            
         }
         else{
            w=w+buf[x];
         }
         x++;
    }
    
    values[i]=stof(w);
    cout<<"Values1 :"<<values[i]<<endl;    
    
    
    //
    //
    // getting weights from pipe
    //
    //
    
    string pipe_name1 = "w"+f;  // Pipe name
    char* pipe_name = (char*)pipe_name1.c_str();
    int pipe_fd;  // Pipe file descriptor
   /* char buf[1024];  // Buffer for reading and writing data
    int bytes_read;  // Number of bytes read from pipe*/

    // Create named pipe
    mkfifo(pipe_name, 0666);

    pipe_fd = open(pipe_name, O_RDONLY);
    if (pipe_fd == -1) {
        cerr << "Error: Failed to open named pipe for reading" << endl;
        return 1;
    }

    // Read data from pipe
    while ((bytes_read = read(pipe_fd, buf, sizeof(buf))) > 0) {
        buf[bytes_read] = '\0';  // Null-terminate buffer
        cout << "Received data from pipe: " << buf << endl;
    }
    
    x=1;
     w="";
 
    weights = new float*[tcount];
    for (int i=0;i<tcount;i++){
        weights[i]=new float[num_weights];    
    }
    
     i=0;
     j=0;
    while(buf[x]!='\0'){
         if (buf[x]=='_'){
            cout<<"weight: "<<w<<endl;
            weights[i][j]=stof(w);
            
            if (j==num_weights-1){
                j=0;
                i++;
            }
            else{
            j++;
            }
            
           
            w="";
            
         }
         else{
            w=w+buf[x];
         }
         x++;
    }
    
    cout<<"weight1: "<<w<<endl;
    weights[i][j]=stof(w);
    cout<<"Real weights"<<endl;
    for (int i=0;i<tcount;i++){
        for (int j=0;j<num_weights;j++){
            cout<<weights[i][j]<<" - ";
        }
        cout<<endl;
    }
    
    cout<<"end exit"<<endl;
    // Close pipe file descriptor
    close(pipe_fd);
    
    
    
    /*int a= fork();
    
    if (a==0){
       
       
       
    }
    */
    
    //
    //
    //  Threads and neuron
    //
    //
    
     sem_wait(sem1);
    int *count = new int[tcount];
    pthread_t *thread = new pthread_t[tcount];
    mydata *m = new mydata[tcount];
     for (int i=0;i<tcount;i++){
     
          count[i]=i;
          
          m[i].num=i;
          m[i].num_weight=num_weights;
          m[i].weight = weights[i];
          m[i].value = values;
          pthread_create(&thread[i],NULL,neuron,(void*)&m[i]);
     }
    
    
    
    sem_post(sem1);
   
   
    while(n!=tcount){
    
    }
    
    if (layernum!=last){
    
    string next_pipe = "v"+to_string(layernum+1);  // Pipe name
    char* next_pipe_c = (char*)next_pipe.c_str();
    int next_fd;  
   
  
    mkfifo(next_pipe_c, 0666);
    
    next_fd = open(next_pipe_c, O_WRONLY);
    
    
    write(next_fd, (char*)forwardval.c_str(), forwardval.length());
    
    close(next_fd);
    
    
    string bpipe_name1 = "b"+f;  // Pipe name
   
    char* bpipe_name = (char*)bpipe_name1.c_str();
    int bpipe_fd;  // Pipe file descriptor
    bytes_read=0;
   /* char buf[1024];  // Buffer for reading and writing data
    int bytes_read;  // Number of bytes read from pipe*/

    // Create named pipe
    mkfifo(bpipe_name, 0666);

    bpipe_fd = open(bpipe_name, O_RDONLY);
    if (bpipe_fd == -1) {
        cerr << "Error: Failed to open named pipe for reading" << endl;
        return 1;
    }

    // Read data from pipe
    while ((bytes_read = read(bpipe_fd, buf, sizeof(buf))) > 0) {
        buf[bytes_read] = '\0';  // Null-terminate buffer
        cout << "Received data from pipe: " << buf << endl;
    }
    
   string bbnext_pipe;
    if (layernum!=1){
     bbnext_pipe = "b"+to_string(layernum-1);  // Pipe name
    }
    else{
    bbnext_pipe = "m";
    }
    char* bbnext_pipe_c = (char*)bbnext_pipe.c_str();
    int bbnext_fd;  
   
  
    mkfifo(bbnext_pipe_c, 0666);
    
    bbnext_fd = open(bbnext_pipe_c, O_WRONLY);
    
    
    write(bbnext_fd, buf, 1024);
    
    close(bbnext_fd);
    
    
    
    
    
    
    
    
    }
    else{
    
    //
    //
    // Last layer
    //
    //
       
        cout<<tcount<<" The output of last layer: "<<forwardval<<endl;
        
        int u=1;
        string t="";
        /*while(forwardval[u]!='_'){
              t=t+forwardval[u];
              u++;
              cout<<"-";
        }
        float res1 = stof(t);
        u++;*/
        while(forwardval[u]!='\0'){
              t=t+forwardval[u];
              u++;
              cout<<"*";
        }
        double ss = stod(t);
        
        float res1 = ((ss*ss)+ss+1)/2.0;
        float res2 = ((ss*ss)-ss)/2.0;
        
        forwardval = "_"+to_string(res1)+"_"+to_string(res2);
        
    string next_pipe = "b"+to_string(layernum-1);  // Pipe name
    char* next_pipe_c = (char*)next_pipe.c_str();
    int next_fd;  
   
    
    
    mkfifo(next_pipe_c, 0666);
     cout<<"********before Backpropagation*******"<<endl;
    next_fd = open(next_pipe_c, O_WRONLY);
    
    
    write(next_fd, (char*)forwardval.c_str(), forwardval.length());
    
    close(next_fd);
    
    cout<<"********Starting Backpropagation*******"<<endl;
    
        
    }
    
    
    
    
    }
    
    
    
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
    while(true){};
    



}
