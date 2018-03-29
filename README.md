# BarberProblem

Please save c++ file "main.cpp" to linux system, then compile it using the following commands on linux:

  g++ -std=c++11 main.cpp -o main -pthread

Then run it by:

  ./main


Remarks:

1. I changed the generating rate of customers from every 3 seconds to every 2 seconds, for better observation of barber sleeeping, barber waking up and working, customer waiting in the waiting room, and customer leaving.

2. pthread is only used for compiling, no pthread methods were used in c++ program.
