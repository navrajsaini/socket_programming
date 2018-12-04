#include "ClientSocket.h"
#include "SocketException.h"
#include <iostream>
#include <string>

//conver the string to it's binary int rep
void str_to_int (std::string data, int bin_len[])
{
   int dec;
   for (int i = 0; i < data.length(); i++)
   {
      dec = data[i];
      for (int j = 7; j > 0; j--)
      {
	 bin_len[(i*8)+j] = dec % 2;
	 dec = dec / 2;
      }
      //place the 0 in the parity bit
      bin_len[i*8] = 0;
   }
   
}
//check the parity for the binary rep
void parit (int bin_len[], int size)
{
   int parity = 0;
   for (int i = 0; i < size; i++)
   {
      //increment the parity for every 1 there is in the array
      if (bin_len[i] == 1)
	 parity++;
   }
//if the parity mod 2 is 1, so odd,
//place a 1 in the parity spot of the first
//bit
   if (parity % 2 == 1)
      bin_len[0] = 1;
   else if (parity % 2 == 0)
      bin_len[0] = 0;
}

//function to convert the int array to string
std::string int_to_str (int bin_len[], int size)
{
   
   std::string data_c = "";
   for (int i = 0; i < size; i++)
   {
      switch (bin_len[i])
      {
	 case 0:
	    data_c += '0';
	    break;
	 case 1:
	    data_c += '1';
	    break;
      }
   }
   return data_c;
}

int main(int argc, int argv[])
{
   int counter = 0;
   try{
//variables
      std::string serv_name;
      int port_name;
      std::string reply,//the frame from the server
	 data,//the frame is stored locally here
	 data_c,
	 data_rep;
      
//get the IP address for the server and the port number.     
      std::cout << "Please enter the IP address for the server (localhost for default): ";
      std::cin >> serv_name;
      std::cout << "Please enter the port the server is using (30000 for default): ";
      std::cin >> port_name;
      
      ClientSocket client_socket(serv_name, port_name);
      // Usually in real applications, the following
      // will be put into a loop. 
      try {
	 for (int i = 0; i < 1000; i++)//get 1000 frames from the user
	 {
	    counter++;
//get the frame from the server
	    client_socket >> reply;
	    data = reply;
//bin_len: the binary data for the data is stored here
	    int bin_len[data.length()*8];
//set size to the size of bin_len
	    int size = (sizeof(bin_len)/sizeof(*bin_len));

//create the parity for the frame received

//function call to the string to binary function
	    str_to_int (data, bin_len);
//function call to int array to parity
	    parit (bin_len, size);
//function call to int array to string
	    data_c = int_to_str (bin_len, size);

//let the server know we got the frame
	    client_socket << "recieved the frame";
//get the parity check for the frame
	    client_socket >> reply;
//store the parity check locally
	    data_rep = reply;
//check the parity frame created with the one recieved
	    if (data_c[0] == data_rep[0])
	       client_socket << "ACK";
	    else
	    {
//*************************IF THERE's A NAK******************************
	       client_socket << "NAK";
//get the retransmission of the recieved
	       client_socket >> reply;
	       data = reply;
//create the parity for the frame recieved
	       int bin_len[data.length()*8];
	       int size = (sizeof(bin_len)/sizeof(*bin_len));


	       //function call to the string to binary function
	       str_to_int (data, bin_len);
//function call to int array to parity
	       parit (bin_len, size);
//function call to int array to string
	       data_c = int_to_str (bin_len, size);

//get the parity check for the frame
	       client_socket >> reply;
//store the parity check locally
	       data_rep = reply;
//check the parity frame created with the one recieved
	       if (data_c[0] == data_rep[0])
		  client_socket << "ACK";
	       else
		  client_socket << "NAK";
	       
//clear out the variables for the next transmission
	       data_c = data_rep = data = "";
	    }//end of else
	    
//clear out the variables for the next transmission
	    data_c = data_rep = data = "";
	    std::cout << "\n" << counter << "\n";
	 }//end of for loop
      }
      catch(SocketException&){
      }
      std::cout << "We received this response from the server:\n\"" << reply << "\"\n";;
   }
   catch(SocketException& e){
      std::cout << "Exception was caught:" << e.description() << "\n";
   }

   return 0;
}
