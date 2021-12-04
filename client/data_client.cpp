#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <thread>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <iostream>
#include <sstream>

#define ADDRESS_IN "10.0.0.10"
#define ADDRESS_OUT "10.0.0.2"
#define PORT 34455
#define UDP_BUFF_SIZE 64

int32_t readLightSensor()
{
   const char* filePath    = "/sys/bus/iio/devices/iio:device0/in_illuminance_raw";
   const int   BUFFER_SIZE = 10;
   int32_t     result      = -1;
   char        buffer[BUFFER_SIZE];

   memset(buffer, 0, BUFFER_SIZE);

   fd_set      fileDescriptorSet;
   timeval     timeout { .tv_sec  = 3, .tv_usec = 0 };

   int fileDescriptor = open(filePath, O_RDONLY);

   FD_ZERO (&fileDescriptorSet);
   FD_SET (fileDescriptor, &fileDescriptorSet);

   if (fileDescriptor == -1)
   {
     std::cout << "Unable to open " << filePath << std::endl;
   }
   else
   {
      // if ready is < 0 --> file open problem, --> if is 0 => timeout
      int ready = select(fileDescriptor+1, &fileDescriptorSet, NULL, NULL, &timeout);

      if (ready < 0)
      {
         std::cout << "File not ready " << filePath << std::endl;
      }
      else if (ready == 0)
      {
         std::cout << "Timeout reading from file " << filePath << std::endl;
      }
      else
      {
         for (uint8_t i = 0; i < 3; i++)
         {
            result = read(fileDescriptor, buffer, BUFFER_SIZE);

            if (result == -1)
            {
                std::cout << "Error reading from light sensor.Retry " << i << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(200));
            }
            else
            {
                std::stringstream param;
                param << "%";
                param << BUFFER_SIZE;
                param << "d";
                sscanf(buffer, param.str().c_str(), &result);
                break;
            }
         }
      }

   close(fileDescriptor);
   }

   return result;
}


int main()
{
   int sockFD;
   struct sockaddr_in addr;
   struct sockaddr_in servaddr;

   std::string dataStr = "Test";

   char buffer[UDP_BUFF_SIZE];
   int n;
   socklen_t len;

   // Creating socket
   if ( (sockFD = socket(AF_INET, SOCK_DGRAM, 0)) < 0 )
   {
     perror("Socket creation failed");
     exit(EXIT_FAILURE);
   }


   memset(&servaddr, 0, sizeof(addr));

   addr.sin_family = AF_INET;
   addr.sin_port = htons(PORT);

   if(inet_pton(AF_INET, ADDRESS_IN, &addr.sin_addr)<=0)
   {
     std::cout << "\nInvalid address/ Address not supported \n";
     return -1;
   }

   memset(&servaddr, 0, sizeof(servaddr));
   servaddr.sin_family = AF_INET;
   servaddr.sin_port = htons(PORT);
   servaddr.sin_addr.s_addr = INADDR_ANY;

//   if(inet_pton(AF_INET, ADDRESS_OUT, &servaddr.sin_addr)<=0)
//    {
//        std::cout << "\nInvalid address/ Address not supported \n";
//        return -1;
//    }

   while (1)
   {
      dataStr = std::to_string( readLightSensor() );
      char msg_array[dataStr.length() + 1];
      strcpy(msg_array, dataStr.c_str());
      sendto(sockFD, msg_array, sizeof(msg_array), MSG_CONFIRM,
             (const struct sockaddr *) &addr, sizeof(addr));
      printf("Sending : %s\n", msg_array);

//      std::string s(msg_array);
//      std::cout << "Sending " << s;

//      n = recvfrom( sockFD, (char *)buffer, UDP_BUFF_SIZE, MSG_WAITALL,
//                    (struct sockaddr *)&servaddr, &len );
//      printf("Received : %s\n", buffer);

//      buffer[n] = '\0';
//      std::cout << "Received " << buffer;

      std::this_thread::sleep_for(std::chrono::milliseconds(2000));
   }

   close(sockFD);
   return 0;
}
