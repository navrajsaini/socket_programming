/*
  By: Navraj Saini, Bhupinder Gill, Tamanna
  Final class project for Networking

*/
#include "ServerSocket.h"
#include "SocketException.h"
#include <string>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdarg.h>
#include <stdlib.h>

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
//parity function
void parit (int bin_len[], int counter, int rand, int size)
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
   if (counter % 5 == 1)
   {
      if (bin_len[0] == 1)
	 bin_len[0] = 0;
      else
	 bin_len[0] = 1;
    
   }
}

std::string int_to_arr (int bin_len[], int size)
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
   srand (time(NULL));
   int counter = 0;
//open the file with the data being sent
   std::ifstream ifs;
   ifs.open("input", std::ifstream::in);
   
   std::cout << "running....\n";
   try{
      // Create the socket
      ServerSocket server(30000);
      
      while (true){
	 counter++;
//random num gen
	 int random = rand() % 5;
	 
	 ServerSocket new_sock;
	 server.accept(new_sock);
	 
	 // For multiple threading, you need to create
	 // a new thread here and pass new_sock to it.
	 // The thread will use new_sock to communicate
	 // with the client.
	 try{
	    while (true){

//the variables being used
	       std::string data;
	       std::string data_c;
	       std::string repl;
	       std::string ack;
	       
//keep sending out frames till the file is empty
	       while (!ifs.eof())
	       {
		  ifs >> data;
//variable to convert the string to binary
		  int bin_len[data.length()*8];
		  int size = (sizeof(bin_len)/sizeof(*bin_len));

//function to convert the data to int
		  str_to_int(data, bin_len);
//function to create the parity
		  parit (bin_len, counter, random, size);
		  
//function to turn the int array into a string
		  data_c = int_to_arr (bin_len, size);


		  new_sock << data;//send the data
		  new_sock >> repl;//wait for a responce

		  new_sock << data_c;//send the parity bits
		  new_sock >> ack;//get the ack 

		  if (ack == "ACK")
//got a ack
		     std::cout << "got the " << ack << "\n";
//got a NAK
		  else
		  {
		     std::cout << "got the " << ack << " retransmitting. \n";
		     
		     //resend the data frame
		     new_sock << data;
		     //create a new parity bit

		     int parity = 0;
		     for (int i = 0; i < size; i++)
		     {
//increment the parity for every 1 there is in the array
			if (bin_len[i] == 1)
			   parity++;
      
//if the parity mod 2 is 1, so odd,
//place a 1 in the parity spot of the first
//bit
			if (parity % 2 == 1)
			   bin_len[0] = 1;
			else if (parity % 2 == 0)
			   bin_len[0] = 0;
		     }
		     //array of ints to string
		     data_c = "";
		     data_c = int_to_arr (bin_len, size);
		     //resend the parity frame
		     
		     new_sock << data_c;
		     new_sock >> repl;//get a reply back
		     //std::cout << "The ACK for the retransmission is: " << repl << "\n";
		  }
		  data_c = "";
	       }
	    }
	 }
	 catch(SocketException&){
	 }
      }
   }
   catch (SocketException& e){
      std::cout << "Exception was caught:" << e.description() << "\nExiting.\n";
   }
   return 0;
}
