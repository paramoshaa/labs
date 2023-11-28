//Вариант 10. О мудрецах.Два мудреца рассуждают, говоря строго по очереди(MPI).Однако
//из - за увлеченности, они слышат и повторяют вслух(на экран) лишь каждое второе
//сообщение, сказанное другим.


#include <string>
#include <iostream>
#include <mpi.h>
#include <cstdlib>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        // 1-ый мудрец рассуждает
        for (int i = 0; i < 5; ++i) {
            std::string message = "message " + std::to_string(i) + "!";

            std::cout << "I am the first sage and my message is " << message << std::endl;

            MPI_Send(message.c_str(), message.size() + 1, MPI_CHAR, 1, 0, MPI_COMM_WORLD);

            char messages[11];
            MPI_Recv(messages, sizeof(messages), MPI_CHAR, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            int j = atoi(&messages[8]);

            if (j % 2 == 0) {
                std::cout << "a first sage said: " << messages << std::endl;
            }
        }
    }
    else if (rank == 1) {
        // 2-й мудрец рассуждает
        for (int i = 0; i < 5; ++i) {

            char message[11];
            MPI_Recv(message, sizeof(message), MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            int j = atoi(&message[8]);

            if (j % 2 == 0) {
                std::cout << "a second sage said: " << message << std::endl;
            }

            std::string messages = "message " + std::to_string(i) + "!";

            std::cout << "I am the second sage and my message is " << messages << std::endl;

            MPI_Send(messages.c_str(), messages.size() + 1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
        }
    }

    MPI_Finalize();

    return 0;
}
