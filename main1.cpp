#include <iostream>
#include <unistd.h>
#include <string>
#include <cstring>
#include <fcntl.h>
#include <sys/stat.h>
#include <fstream>
#include <cstdlib>
using namespace std;

double* inputData;
double* outl;
double*** hl;
double** inpLayer;
double*** hidden;

//
//
// This is the code for weights parsing
//
//
//

string intToString(int number) {
    string result;
    if (number == 0) {
        result = "0";
    }
    else {
        while (number > 0) {
            char digit = '0' + (number % 10);
            result = digit + result;
            number /= 10;
        }
    }
    return result;
}

int stringLen(string s)
{
    int t = 0;
    while (s[t] != 0)
    {
        t++;
    }
    return t;
}

int stringToInt(string s) {
    int result = 0;
    int t = 0;
    char c;
    while (s[t] != 0) {
        c = s[t];
        int digit = c - '0';
        result = result * 10 + digit;
        t++;
    }
    return result;
}
void extractor(int& input_layer, int& hiddenlayers, int& neurons, string s)
{
    string currentNumber = "";
    int extractedCount = 0;
    for (char ch : s) {
        if (isdigit(ch)) {
            currentNumber += ch;
        }
        else if (!currentNumber.empty()) {
            if (extractedCount == 0)
            {
                hiddenlayers= std::stoi(currentNumber);
            }
            else if (extractedCount == 1)
            {
                input_layer = std::stoi(currentNumber);
            }
            else
            {
                neurons = std::stoi(currentNumber);
            }
            extractedCount++;
            currentNumber = "";
            if (extractedCount == 3) {
                break;
            }
        }
    }

}
int weightfinder(string s)
{
    int comma = 0;
    int i = 0;
    while (s[i] != 0)
    {
        if (s[i] == ',')
        {
            comma++;
        }
        i++;
    }
    comma++;
    return comma;
}
double stringToDouble(const std::string& str) {
    double result = 0.0;
    double fraction = 1.0;
    int sign = 1;
    int i = 0;

    // Check if the number is negative
    if (str[i] == '-') {
        sign = -1;
        i++;
    }

    // Process digits before the decimal point
    while (str[i] != '.' && i < str.length()) {
        result = result * 10 + (str[i] - '0');
        i++;
    }

    // Process digits after the decimal point
    if (str[i] == '.') {
        i++;
        while (i < str.length()) {
            fraction /= 10.0;
            result += (str[i] - '0') * fraction;
            i++;
        }
    }

    return result * sign;
}
void extractNumbers(string input, int rowIndex, double**& array, int limit) {
    string number = "";
    int columnIndex = 0;

    for (int i = 0; i < input.length(); i++) {
        if (input[i] == ',' || input[i] == ' ') {
            if (number != "") {
                array[rowIndex][columnIndex] = stringToDouble(number);
                cout << stringToDouble(number) << endl;
                columnIndex++;
                number = "";
            }
        }
        else {
            number += input[i];
        }
        if (columnIndex == limit)
            break;
    }

    if (number != "") {
        array[rowIndex][columnIndex] = stringToDouble(number);
    }
}

void extractNumbers2(string input, int rowIndex, int c1, double***& array, int limit) {
    string number = "";
    int columnIndex = 0;

    for (int i = 0; i < input.length(); i++) {
        if (input[i] == ',' || input[i] == ' ') {
            if (number != "") {
                array[rowIndex][c1][columnIndex] = stringToDouble(number);
              //  cout << stringToDouble(number) << endl;
                columnIndex++;
                number = "";
            }
        }
        else {
            number += input[i];
        }
        if (columnIndex == limit)
            break;
    }

    if (number != "") {
        array[rowIndex][c1][columnIndex] = stringToDouble(number);
    }
}
void extractNumbers3(string input, double& a) {
    string number = "";
    int columnIndex = 0;

    for (int i = 0; i < input.length(); i++) {
        if (input[i] == ',' || input[i] == ' ') {
            if (number != "") {
                a = stringToDouble(number);
                //  cout << stringToDouble(number) << endl;
                columnIndex++;
                number = "";
            }
        }
        else {
            number += input[i];
        }
    }

    if (number != "") {
        a = stringToDouble(number);
    }
}

void extractNumbers4(string input, double*& a) {
    string number = "";
    int columnIndex = 0;

    for (int i = 0; i < input.length(); i++) {
        if (input[i] == ',' || input[i] == ' ') {
            if (number != "") {
                a[columnIndex] = stringToDouble(number);
                //  cout << stringToDouble(number) << endl;
                columnIndex++;
                number = "";
            }
        }
        else {
            number += input[i];
        }
    }

    if (number != "") {
        a[columnIndex] = stringToDouble(number);
    }
}
void read_func()
{
	fstream read("Configuration.txt");
	string s;
    int t = 0;
    string n = "";
    int inputlayer = 0;
    int layers = 0;
    int neurons = 0;
	while (getline(read, s)) {
		// process each line
       // cout << s << endl;
        if (t == 1)
        {
           // cout << s << endl;
            extractor(inputlayer, layers, neurons, s);
            cout << "Extracted" << inputlayer << layers << neurons << endl;
            break;
        }

        t++;
	}
    getline(read, s);
    getline(read, s);
    int w = weightfinder(s);
    cout << "Number of Input Layers: " << inputlayer << endl;
    cout << "Number of Input Layers weights: " << w<< endl;
    double** inpLayer = new double* [inputlayer];
    for (int i = 0; i < inputlayer; i++)
    {
        inpLayer[i] = new double[w];
    }

   // extractNumbers(s, 0, inpLayer);
   // cout << s << endl;
    //extractNumbers(s, 0, inpLayer);
    for (int i = 0; i < inputlayer; i++)
    {
        cout << s << endl;
        extractNumbers(s, i, inpLayer,w);
        getline(read, s);
        
    }
  //  system("cls");
    for (int i = 0; i < inputlayer; i++)
    {
        for (int j = 0; j < w; j++)
        {
            cout << inpLayer[i][j] << " ";

        }
        cout << endl;
    }
    getline(read, s);
    cout << s << endl;
    int hiddenlayers = layers - 2;
    double*** hl = new double** [hiddenlayers];
    for (int i = 0; i < hiddenlayers; i++)
    {
             
            getline(read, s);
        hl[i] = new double*[neurons];
        for (int j = 0; j < neurons; j++)
        {
            
            hl[i][j] = new double[w];
            extractNumbers2(s, i, j, hl, w);
            for (int k = 0; k < w; k++)
            {
                cout << hl[i][j][k] << " ";
            }
            cout << endl;
                getline(read, s);
        }
        cout << endl << endl << endl;
        getline(read, s);
    }

    getline(read, s);
    cout << s << endl;
    double* outl = new double[w];
    for (int i = 0; i < w; i++)
    {
        getline(read, s);
        extractNumbers3(s,outl[i]);
        cout << outl[i] << endl;
        
    }
    getline(read, s);
    cout << s << endl;
    getline(read, s);
    cout << s << endl;
    double* inputData = new double[inputlayer];
    
        getline(read, s);
        extractNumbers4(s, inputData);
        for (int i = 0; i < inputlayer; i++)
        {
            cout << inputData[i] << endl;
        }
        system("cls");
         hidden = new double** [hiddenlayers+2];
        hidden[0] = inpLayer;
            for (int i = 1; i < hiddenlayers + 1; i++)
            {
                hidden[i] = hl[i - 1];               
            }
            hidden[hiddenlayers + 1] = new double* [1];
            hidden[hiddenlayers + 1][0] = outl;
        
            for (int i = 0; i < inputlayer; i++)
            {
                for (int j = 0; j < w; j++)
                {
                    cout << hidden[0][i][j] << " ";

                }
                cout << endl;
            }
            cout << endl << endl;

            for (int i = 1; i < hiddenlayers+1; i++)
            {
                for (int j = 0; j < neurons; j++)
                {
                    for (int k = 0; k < w; k++)
                    {
                        cout << hidden[i][j][k] << " ";
                    }
                    cout << endl;
                }
                cout << endl;
                cout << endl;
            }
            for (int i = 0; i < w; i++)
            {
                cout << hidden[hiddenlayers + 1][0][i] << " ";
            }
}




//
//
//
//
//
//
//
//
//
//
//
//  This is the code for main
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//


int main(){
read_func();
    cout<<"Size of input layer: ";
    int input_size=0;
    cin>>input_size;
    
    cout<<"Number of hidden layer: ";
    int num_hidden=0;
    cin>>num_hidden;
    
    cout<<"Size of output layer: ";
    int out_size=0;
    cin>>out_size;
    
    int s=0;
    int val1=1;
    int val2=2;
    
   
    float* values = new float[input_size];
    for (int i=0;i<input_size;i++){
        values[i] = i;
    }
    int *neuron_count = new int[num_hidden+2];
    neuron_count[0]=input_size;
    for (int i=1;i<num_hidden+1;i++){
    
        cout<<"Size of hidden layer "<<i<<" : ";
        cin>>s;
        neuron_count[i]=s;
        
    }    
    neuron_count[num_hidden+1]=out_size;
    
    
    string a4 = to_string(num_hidden+1);
     
    for (int h=0;h<2;h++){
    for (int i=1;i<num_hidden+2;i++){
    string a1 = to_string(i);
    string a2 = to_string(neuron_count[i]);
    string a3;
    if (i==1){
    a3 = to_string(2);
    }
    else{
    a3 = to_string(neuron_count[i-1]);
    }
    
    if (h==0){
    char* args[] = { "./layer1", (char*)a1.c_str(),(char*)a2.c_str(),(char*)a3.c_str(),(char*)a4.c_str(),NULL};
    int a = fork();
    if (a==0){
     execv(args[0], args);
    
    }
       
    }
    }
    
    
    //
    //
    //   putting data into value pipe
    //
    //
    string pipe_name22 = "v"+to_string(1);  // Pipe name
    char* pipe_name2 = (char*)pipe_name22.c_str();
    int pipe_fd2;  
   
  
    mkfifo(pipe_name2, 0666);
    string msg = "";
    
    for (int i=0;i<input_size;i++){
        msg=msg+"_"+to_string(values[i]);
    }
    
    cout<<"start pepsi"<<endl;
    pipe_fd2 = open(pipe_name2, O_WRONLY);
    
    
    write(pipe_fd2, (char*)msg.c_str(), msg.length());
    cout<<"End pepsi"<<endl;
    close(pipe_fd2);
    
    
    
    
    
      int x=0;
    for (int i=1;i<num_hidden+2;i++){
    
    
   // cout<<"4"<<endl;
   // int a =0;
    string w="";
 
   /* for (int j=0;j<neuron_count[i-1]*neuron_count[i];j++){
    
        w=w+"_"+to_string(x);
        cout<<"Generated weight: "<<w<<endl;
        x++;
    }*/
    
    for (int j=0;j<neuron_count[i];j++){
        for (int k=0;k<neuron_count[i-1];k++){
            w = w +"_"+to_string(hidden[i-1][k][j]);
             cout<<"Generated weight: "<<w<<endl;
        }
    }
   
    //
    //
    // Putting weights into pipe
    //
    //
    string pipe_name1 = "w"+to_string(i);  // Pipe name
    char* pipe_name = (char*)pipe_name1.c_str();
    int pipe_fd;  
    char buf[1024];  
    int bytes_read;  

    mkfifo(pipe_name, 0666);
    cout<<"start cola"<<endl;
    pipe_fd = open(pipe_name, O_WRONLY);

    if (pipe_fd == -1) {
        cerr << "Error: Failed to open named pipe for writing" << endl;
        return 1;
    }
    
    string msg = to_string(val1)+"_"+to_string(val2);
    write(pipe_fd, (char*)w.c_str(), w.length());
    close(pipe_fd);
    }
    ///////
    string bpipe_name1 = "m";  // Pipe name
   
    char* bpipe_name = (char*)bpipe_name1.c_str();
    int bpipe_fd;  // Pipe file descriptor
   
    char buf[1024];  // Buffer for reading and writing data
    int bytes_read=0;  // Number of bytes read from pipe*/

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
        cout << "Received data from main pipe: " << buf << endl;
    }
    
    
    
    //
    //   Values recieved after backpropagation
    //
    
 
        
        int u=1;
        string t="";
        while(buf[u]!='_'){
              t=t+buf[u];
              u++;
             
        }
        values[0] = stof(t);
        u++;
        t="";
        while(buf[u]!='\0'){
              t=t+buf[u];
              u++;
              
        }
        values[1] = stod(t);
        
    
    
    
    
    
    //
    //
    //
    }
    while (true){};

    


}
