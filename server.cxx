/* The server process */
#define _GNU_SOURCE

#include "local.h"
#include <map>

int main() {
    int n, done, dummyfifo, publicfifo, privatefifo;
    struct message msg;
    FILE *fin;
    static char buffer[PIPE_BUF];

    // Track clients by PID
    std::map<int,bool> clients;
    int active_clients = 0;

    mknod(PUBLIC, S_IFIFO | 0666, 0);

    if ((publicfifo = open(PUBLIC, O_RDONLY)) == -1 ||
        (dummyfifo = open(PUBLIC, O_WRONLY | O_NDELAY)) == -1) {
        perror(PUBLIC);
        return 1;
    }

    while (read(publicfifo, (char*)&msg, sizeof(msg)) > 0) {

        int pid = atoi(msg.fifo_name + strlen("/tmp/fifo"));

        // NEW: Register this client FIFO if new
        if (clients.find(pid) == clients.end()) {
            clients[pid] = true;
            active_clients++;
        }

        /* Check if client is exiting */
        if (!strcmp(msg.cmd_line, "__CLIENT_EXIT__")) {
            fprintf(stderr, "Client %d exited.\n", pid);
            clients.erase(pid);
            active_clients--;

            unlink(msg.fifo_name);

            if (active_clients == 0) {
                fprintf(stderr, "No more clients. Server shutting down.\n");
                break;
            }

            continue;
        }

        // ----- Normal command handling -----
        n = done = 0;
        do {
            if ((privatefifo = open(msg.fifo_name, O_WRONLY | O_NDELAY)) == -1)
                sleep(3);
            else {
                fin = popen(msg.cmd_line, "r");
                write(privatefifo, "\n", 1);
                while ((n = read(fileno(fin), buffer, PIPE_BUF)) > 0) {
                    write(privatefifo, buffer, n);
                    memset(buffer, 0x0, PIPE_BUF);
                }
                pclose(fin);
                close(privatefifo);
                done = 1;
            }
        } while (++n < 5 && !done);

        if (!done) {
            write(fileno(stderr),
                "\nNOTE: SERVER ** NEVER ** accessed private FIFO\n", 48);
        }
    }

    unlink(PUBLIC);
    return 0;
}