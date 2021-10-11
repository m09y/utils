#include <iostream> 
#include "SyncEventBuffer.h" 
#include <thread>
using namespace std; 

int main()
{
   SyncEventBuffer mybuffer;

   std::thread mythread([&mybuffer]()
   {
      cout << "Thread 1 started ..."<<endl;
      
      mybuffer.synced_push(9);
   } );

   std::this_thread::sleep_for(std::chrono::seconds(1));
   
   std::thread mythread2([&mybuffer]()
   {
      cout << "Thread 2 started ..."<<endl;
      
      mybuffer.synced_push(10);

   } );   

   mybuffer.continuous_pop();

   //mybuffer.stop();
   mythread.join();
   mythread2.join();

}